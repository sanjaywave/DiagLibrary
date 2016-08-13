#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <errno.h>
#include <endian.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include "errors.h"
#include "interfaceapi.h"
#include "diag-cmd.h"
#include "decode.h"
#include "ipcQueue.h"
#include "DlfFileWriter.h"
#include "extmsgid.h"
#include "cdmadecode.h"


/* 10msec tick 4*100 = 4sec  in millsecond*/
#define RECEIVE_TICK_TIME 50 
#define WATCH_DOG_COUNT1  (60000/RECEIVE_TICK_TIME)

extern int gManufactureType;
extern int globalFD;
extern bool IsGprsL2InfoEnable;
extern int init_done;

int LogOnDemandRequestCounter=0;
u_int32_t isDMAllowed = 1;
u_int32_t isISFAllowed = 0;

static void diag_com_covert_to_packet(char *read_buf , unsigned int length);
u_int32_t extract_msg_response_from_buf(u_int8_t eventpacketType ,char *buf , unsigned int buf_len);
int  decode_log_msg(u_int16_t  command_code , u_int8_t *data,u_int16_t length, u_int8_t timestamp[8]);
static void  diag_com_decode_mask( char *buf , unsigned int buf_len);
u_int32_t  extract_log_packet_from_buf(char *buf,u_int32_t buf_len);
u_int32_t extract_event_packet_from_buf(char *buf , u_int32_t buf_len);
void decode_event(unsigned int event ,char *eventData , u_int8_t lengthData ,char *time , u_int8_t lengthTime);
void extract_msg_response(u_int8_t eventpacketType ,char *buf , unsigned int buf_len);
void less_received(char **buf , unsigned int *buf_len , int join);
/*
 * Function to listen the event on this fd
 * Return  0,if its has process the data ,otherwise return-1
 */

int processData(queue_t q)
{
	unsigned int returnType = -1; 
	PacketInfo pkt;
	//int *logtype;
	while(receiveQueue(q, &pkt) == 0)
	{
		if(gManufactureType == SAMSUNG) { // This should be tested by packetType???
			diag_com_covert_to_packet(pkt.msgPtr+4, pkt.length-4);
		} else if (gManufactureType == HTC) {
			diag_com_decode_mask(pkt.msgPtr+4, pkt.length-4);
		}
		FREE(pkt.msgPtr);
		returnType = 0;
	}
	return returnType;

}


void *packetProcessing(void * arg)
{
	queue_t q = (queue_t) arg; // We have the Queue now

	time_t currenttime;
	struct timeval timeout;
	int handle;
	unsigned int watchDogCount = 0;
	bool isEventSent = false;

	/* select returns 0 if timeout, 
	                  1 if input a timeoutvailable, 
			 -1 if error. */
	while (1)
	{
		/* Initialize the file descriptor set. */
		timeout.tv_sec = 0;
		//Wake up every 50 milliSecond and process the message queue...
		timeout.tv_usec = RECEIVE_TICK_TIME*1000; 
		handle = select(0 ,NULL,NULL ,NULL ,&timeout);
		if(!init_done)
		{
#if CHECK_STATS
			print_count_wcdma();
#endif
			printf("\nExiting Processing thread \n");
			return;
		}
		
		/* If enable GPRS than state is GSM than fetch stats from GSM */
		if (IsGprsL2InfoEnable 
			&& (decode_get_state_info() == DETECT_GSM))
		{
			/* System = GSM and IsGprsL2InfoEnable = true , 
			   every 3 sec  */
			if(LogOnDemandRequestCounter == 3 *100)
			{
				/* LogOnDemandRequestCounter == 3*/
				Gsm_log_on_demand_request(globalFD,0x5202);
				/* Gsm_log_on_demand_request(globalFD, 0x5215); */
				Gsm_log_on_demand_request(globalFD,0x520A);
				LogOnDemandRequestCounter=0;
			}
			LogOnDemandRequestCounter++;
		}

		/* processData return NON zero means queue 
		   does not have any data for processing */
		if (processData(q) == 0) {
			isEventSent = true;
			watchDogCount=0;
		} else watchDogCount++;

		if (watchDogCount == WATCH_DOG_COUNT1)
		{
			printf("Event generate on watchDogCount %d \n",
				watchDogCount);
			if(isEventSent)
			{
				d_warning("WatchDog Thread Blocked,"
					" Event need to generate \n");
				sendInformation (GSM, EVENT_DATA, 
						EVENT_SYSTEM_BLOCK, NULL, 0);
				sendInformation (WCDMA, EVENT_DATA, 
						EVENT_SYSTEM_BLOCK, NULL, 0);
				sendInformation (LTE, EVENT_DATA, 
						EVENT_SYSTEM_BLOCK, NULL, 0);
				isEventSent = false;
			}
			watchDogCount=0;
		}
	}
}

/*
 * function to convert readbuf to packet
 */
static void diag_com_covert_to_packet(char *read_buf , unsigned int length){
	if(length < 8) return;
	unsigned int len = 0;
	unsigned int numpkt = 0 , i = 0;
	memcpy(&numpkt, read_buf, 4);
	char *buf = &read_buf[4];
	length -= 4;
	for (i = 0 ; i < numpkt; i++) {
		memcpy(&len, buf, 4);
		if (length < len+4) return; // Buffer overrun
		buf += sizeof(unsigned int); //This is actual packet which need to be decode
		length -= 4;
		diag_com_decode_mask(buf, len);
		buf += len;
		length -= len;
	}
}
/*
 * function to decode read buffer
 */
static void  diag_com_decode_mask( char *buf , unsigned int buf_len)
{
	unsigned int buf_index = 0;
	unsigned int length, less_len;
	char *new_buf;
	char *less_buf = NULL;

	less_received(&less_buf, &less_len, 1);

	if (less_buf != NULL) {
		new_buf = (char *) MALLOC(buf_len + less_len);
		if (new_buf == NULL) {
			d_warning("Unable to allocate new_buf");
			printf("Unable to allocate new_buf12");
			return;
		}
		memcpy(new_buf, less_buf, less_len);
		memcpy(new_buf + less_len, buf, buf_len);
		length = less_len + buf_len;
	} else {
		new_buf = buf;
		length = buf_len;
	}

	while(length != 0)
	{
		u_int8_t eventpacketType = 0;
		u_int16_t packetType = 0;
		unsigned int packetlength = 0;
		char *temp_buf;

		eventpacketType = *(u_int8_t *)(new_buf + buf_index);
		memcpy(&packetType, new_buf + buf_index, sizeof(u_int16_t));
		temp_buf = new_buf + buf_index;

		if(packetType==0x0010)
		{
			/*Logging Response */
			packetlength = extract_log_packet_from_buf(temp_buf, length);/* RRC Message decode function called*/
		}else if (eventpacketType==0x60){
		   packetlength = extract_event_packet_from_buf(temp_buf, length);// event decoding function called
		}else {
			/* Dynamic Command Resposne */
			packetlength = extract_msg_response_from_buf(eventpacketType , temp_buf, length);
			//d_log("Message:: Index Buf:- %d , Packetlength:- %d ,temp_packetlength %d \n",buf_index, packetlength,temp_packetlength);
		}
		length -= packetlength;
		buf_index += packetlength;
	}

	if(less_buf != NULL) FREE(new_buf);
}

u_int32_t extract_msg_response_from_buf(u_int8_t eventpacketType ,char *buf , unsigned int buf_len)
{
	u_int32_t packet_length=0, error_len=0;
	bool success;
	char outbuf[2048];
	unsigned int decap_len = 0;
	unsigned int used = 0;
	bool more = FALSE;
	success = dm_decapsulate_buffer (buf,buf_len,outbuf,sizeof (outbuf),&decap_len, &used, &more);
	if(success){
		if(!more){
					extract_msg_response(eventpacketType, outbuf,decap_len);
					packet_length = used;
					return packet_length;
				}else{
					//d_warning("\n warning : Incomplete Log packet received \n");
					packet_length = buf_len;
					less_received(&buf, &buf_len, 0);

					d_arraylog(buf,packet_length);
					return packet_length;
		}
	}
	else
	{
		d_warning("\nwarning : CRC failure for MSG data\n");
		packet_length = used;
		return  packet_length;
	}
}
void extract_msg_response(u_int8_t eventpacketType ,char *buf , unsigned int buf_len)
{
	int i;
	if (eventpacketType ==0x73){
				/*Logging Message Response*/
	}else if (eventpacketType == 0x78){
				d_warning("warning : Log on Demand response received \n");
				d_arraylog(buf,buf_len);
	}else if(eventpacketType == 0x26){
				d_arraylog(buf,buf_len);
				ltebandlock_nv_read_response(buf,buf_len);
	}else if(eventpacketType == 0x27){
				d_arraylog(buf,buf_len);
				ltebandlock_nv_write_response(buf,buf_len);
	}else if(eventpacketType == 0x4B){
		/*Packet is receive in response File System*/
		if(buf[1]  == 0x13){
					decode_fs_information (buf,buf_len);
				}
	}else if(eventpacketType == DIAG_TIME_REQUEST_CMD_CODE){
			/* Time response received */
		DmTimeResponse(buf,buf_len);
	}
	else if(eventpacketType == 0x19){
    	decode_log_msg( 0x19, buf ,buf_len, NULL);
    }else{
				d_log("\nUnknown Packet Received");
				d_arraylog(buf, buf_len);
		}

}


u_int32_t  extract_log_packet_from_buf(char *buf , unsigned int buf_len)
{
	u_int32_t packet_length=0,error_len=0;
	bool success;
	char *outbuf;
	unsigned int decap_len = 0;
	unsigned int used = 0;
	bool more = FALSE;
	u_int8_t Timestamp[8]={0};
	/*Constant for Log pkt*/
	int timelength = 8;
	log_packet_details *rsp;
	outbuf = (char*)MALLOC(buf_len+3);
	if (outbuf == NULL) {
		d_warning("Unable to malloc outbuf\n");
		printf("Unable to malloc outbuf\n");
		exit (-1);
	}
	success = dm_decapsulate_buffer (buf,buf_len,outbuf,(buf_len+3),&decap_len, &used, &more);
	if(success){
		if(!more){

			char *p_timestamp=NULL;
			if(isISFAllowed)
			{
				/* Outbuf+4 is to escape the first 4 byte (10 0 and 2 bytes length)*/
				 InputDlfDataBuff(outbuf+4,decap_len-4);
			}
			rsp = (log_packet_details *) (outbuf+4);
			if(isDMAllowed)
			{
				/* Data length should not be greater than 0 */
				if(rsp->length > 12)/* Command + TimeStamp + Length */
				{
					buffer_to_file_write(rsp->timestamp,timelength,rsp->command_code,rsp->data,rsp->length-12);
					error_len = decode_log_msg(rsp->command_code,rsp->data,rsp->length-12, rsp->timestamp); /* decoding function*/
				}
			}
			packet_length = used;
			FREE(outbuf);
			outbuf = NULL;
			return packet_length;
		}else{
			//d_warning("\nwarning : Incomplete Log packet received \n");
			packet_length = buf_len;
			less_received(&buf, &buf_len, 0);
			FREE(outbuf);
			outbuf = NULL;
			return packet_length;
		}
	}else {
		d_warning("Log data CRC failed ,Code = %2x%2x",buf[7],buf[6]);
		packet_length = used;
		FREE(outbuf);
		outbuf = NULL;
		return  packet_length;
	}
}



/*function of find the event command code from event bytes stream.*/
void  processEvents(char *buf , unsigned int buf_len)
{
	u_int16_t event_length;
	u_int16_t event_id;
	unsigned int  eventID = 0;/*variable  to get the 12 bits event id*/
	u_int8_t  time_stamp_len = 0;
	u_int8_t  actual_timestamp_len = 0;
	u_int8_t  payload_data_len = 0;
	unsigned int  payload_data_flag = 0;
	unsigned int  time_flag = 0;
	char *timeptr = NULL ;/* pointer to the event time stamp.*/
	char* dataptr = NULL ; /* pointer to event data.*/
	unsigned int event_index = 1;
	memcpy(&event_length, (buf + event_index), sizeof(u_int16_t));

	//printf("event_length : %u\n",event_length);

	if(event_length >buf_len) {
		d_warning("something is wrong event length is greater than buffer length");
		return;
	}

	event_index += 2 ;/*LENGTH INCREAMENT*/
	d_log("\nEnter ProcessEvents  %d \n", event_length);
	while (event_length !=0)
	{
		memcpy(&event_id, buf + event_index, sizeof(u_int16_t)); /* increase the pointer to index len*/
		//printf("event_id : %u\n",event_id);
		eventID = event_id & 0x0fff;
		/*Reduce event length*/
		event_length -= 2;
		time_flag = (event_id & 0x8000) >>15;
		d_log("Time_flag = %d \n",time_flag);
		event_index = event_index + 2;
		if(time_flag){
			/*
			 * Event timestamp size 2
			 */
			time_stamp_len = 2;
			if(timeptr != NULL){
			/*
			 * Make the time stamp of 8 bytes.
			 * memcpy 2 byes at 3 and 4 bytes of the event with 8 bytes time event.
			 */
				memcpy(timeptr+1,&buf[event_index],2);
				actual_timestamp_len = 8 ;
			}else{
			/*
			 * This case may hit with have not capture the full
			 *  time stamp event.
			 */
				d_warning("Incorrect Timing received on events \n");
				timeptr=&buf[event_index];
				actual_timestamp_len= 2;
			}
		}else{
			time_stamp_len = actual_timestamp_len = 8;
			/*pointer to time data*/
			timeptr=&buf[event_index];
		}
		event_length -= time_stamp_len;
		event_index += time_stamp_len;
		payload_data_flag =(event_id >>13) & 0x3;
		d_log("payload_data_flag =%d \n", payload_data_flag);
		if(payload_data_flag==0){
			payload_data_len =0;
			/* data pointer is not defined*/
		}else if (payload_data_flag==1){
			payload_data_len =1;
			dataptr=&buf[event_index];
		}else if(payload_data_flag==2){
			payload_data_len =2;
			dataptr=&buf[event_index];
		}else if(payload_data_flag==3){
			payload_data_len =(u_int8_t )(buf[event_index]);
			if(payload_data_len==0) {
				d_warning("Payload Data Length 0 : Incorrect Information received \n");
				break;
			}
			event_index++;
			dataptr=&buf[event_index];
			event_length-- ;/*This is payload length field*/
		}
		d_log("payloadDataLength %d , event_index %d \n",payload_data_len ,event_index);
		if (event_length >= payload_data_len)
		{
			event_length -= payload_data_len;
		}else{

			break;
		}
		event_index = event_index + payload_data_len;
		d_log("Write Event into Text File,Event Id  = %d",eventID);
		if(payload_data_len){
			buffer_to_file_write(timeptr,actual_timestamp_len,eventID,dataptr,payload_data_len);
			decode_event(eventID ,dataptr,payload_data_len,timeptr,actual_timestamp_len);
		}
		d_log("\nEnd of Event Loop Length == %d", event_length);
	}
}


u_int32_t extract_event_packet_from_buf(char *buf , u_int32_t buf_len)
{
	bool success;
	char outbuf[10000];
	unsigned int decap_len = 0;
	unsigned int used = 0;
	bool more = FALSE;
	u_int32_t packet_length;
	success = dm_decapsulate_buffer (buf,buf_len,outbuf,sizeof (outbuf),&decap_len, &used, &more);
	if(success){
		if(!more){
						
			//d_log("\nProcess Events more %d Incoming buffer len %d  decap_len %d used %d  \n",more,buf_len,decap_len,used);
			processEvents(outbuf,decap_len);
			packet_length = used;
			return packet_length;
		}else{
			//d_warning("\nwarning : Incomplete Log packet received \n");
			packet_length = buf_len;
			less_received(&buf, &buf_len, 0);
			d_arraylog(buf,packet_length);
			return packet_length;
		}
	}else {
		d_warning("\nwarning : Event CRC failure   \n");
		packet_length = used;
		return packet_length;
	}
}


void is_DLF_allowed(u_int32_t DLFState){
	isISFAllowed = DLFState;
};
void is_DM_allowed(u_int32_t DMstate){
	 isDMAllowed = DMstate;
}

// join =1 for joining , 0 for new data
void
less_received(char **buf , unsigned int *buf_len , int join)
{
#define MAX_DRIVER_PACKET_SIZE 8192
	static char *pkt = NULL;
	static int len = 0;
	static int max_packet_len = MAX_DRIVER_PACKET_SIZE;
	if (pkt == NULL) {
		pkt = (char *) MALLOC(MAX_DRIVER_PACKET_SIZE);
		if (pkt == NULL) {
			d_warning("Unable to allocate pkt\n");
			d_flush();
			printf("Unable to allocate pkt");
			exit(-1);
		}
	}

	if (!join)
	{
		if (*buf_len >= max_packet_len) {
			d_warning("Too Big buffer %d: Reallocating\n", max_packet_len);
			max_packet_len = *buf_len;
			free(pkt);
			pkt = (char *) MALLOC(max_packet_len);
			if (pkt == NULL) {
				d_warning("Unable to allocate pkt\n");
				d_flush();
				printf("Unable to allocate pkt");
				exit(-1);
			}
		}
		len = *buf_len;
		memcpy (pkt, *buf, *buf_len);
	}
	else
	{
		if (len == 0) {
			*buf = NULL;
			*buf_len = 0;
		} else {
			*buf = pkt;
			*buf_len = len;
			len = 0;
		}
	}
}
