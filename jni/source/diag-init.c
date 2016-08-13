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
#include "diag-init.h"
#include "ipcQueue.h"

extern int gManufactureType;
extern int globalFD;
extern int init_done;

void *diag_com_listen(void *arg);
extern void exit();


/*function to open device*/
 int  diag_com_open ()
{
	int status;
	int fd;
	status = fd = open("/dev/diag_mdm", O_RDWR , O_NONBLOCK ,O_NDELAY);/* open Lte device*/
	if (status == -1) {
		status = fd = open("/dev/diag",O_RDWR);/*open Wcdma device*/
		if (status == -1) {
			d_warning("diag_com_ioctl : Not any device is open");
		} else {
			printf("\nWcdma/Gsm device is open");
		}
	} else {
		printf("\nLte Device is open");
	}
	return fd;
}

/*function to config ioctl on  device*/
int diag_com_ioctl(int fd)
{
	int ioctl_status;
	if(gManufactureType == SAMSUNG){
		printf("Ashish initdiag: diag init for 4.4.2");
		ioctl_status = ioctl(fd,7,0x20);
	} else  ioctl_status = ioctl(fd,7,2);
	if (ioctl_status == -1)
	{
		d_warning("diag_com_ioctl : Error in IOCTL logging , Errno = %d ",errno);
	}
	return ioctl_status;
}

/* function to write into device*/
int  diag_com_write_command(int fd,u_int8_t deviceType,u_int16_t items[])
{
	int writing_status;
	char *write_buf;
	int len;
	write_buf = (char *)MALLOC(1024);
	if(gManufactureType == SAMSUNG){
		write_buf[0]= 0x20;
	}else
		write_buf[0]= 0x40;
	write_buf[1]= 0;
	write_buf[2] =0;
	write_buf[3] =0;
	len= cmd_log_config_set_mask_new (&write_buf[4],1020,deviceType,items);
	d_log("Diag_com_write_command");
	//d_arraylog(write_buf , len+4);
	writing_status = write(fd, write_buf, len+4);
	if (writing_status == -1){
		d_warning("Diag_com_write_event : Writing error to Diag Device %d ",errno);
	}
	FREE(write_buf);
	return writing_status;
}

/*function to  write the event into device*/
int diag_com_write_event(int fd,u_int16_t wcdma_event_item[])
{
	int writing_status;
	char *write_buf;
	int len;
	write_buf = (char *)MALLOC(1024);
	if(gManufactureType == SAMSUNG){
		write_buf[0]= 0x20;
	}else
		write_buf[0]= 0x40;
	write_buf[1]= 0;
	write_buf[2] =0;
	write_buf[3] =0;
	len = cdm_cmd_event_report_new (&write_buf[4],1020,TRUE);/* command  to enable all events*/
	d_log("diag_com_write_event");
	d_arraylog(write_buf , len+4);
	writing_status = write(fd, write_buf, len+4);
	if (writing_status == -1){
		d_warning("Diag_com_write_event : Writing error,Disable event %d ",errno);
	}
	sleep(1);
	memset(&write_buf[4],0,1020);
	len = cdm_cmd_event_mask(&write_buf[4],1020,wcdma_event_item);/*method to configure the event*/
	d_arraylog(write_buf , len);
	writing_status = write(fd, write_buf, len+4);
	if (writing_status == -1){
		d_warning("Diag_com_write_event : Writing error,Enable event %d ",errno);
	}
	FREE(write_buf);
	return writing_status;
}

/*Function to write time request on the device */

 int Dm_cmd_time_request(int fd)
{
	int writing_status;
	char *write_buf;
	int len = 0;
	write_buf = (char *)MALLOC(1024);
	if(gManufactureType == SAMSUNG){
		write_buf[0]= 0x20;
	}else
		write_buf[0]= 0x40;
	write_buf[1]= 0;
	write_buf[2] =0;
	write_buf[3] =0;
	len= cmd_dm_time_request(&write_buf[4],1020);
	d_arraylog(write_buf , len+4);
	writing_status = write(fd, write_buf, len+4);
	if (writing_status == -1){
		d_warning("Diag_Dm_cmd_time_request ErrorNo =  %d ",errno);
	}
	FREE(write_buf);
	return writing_status;
}

/* function to read the diag device*/
static int  diag_com_read(int fd,char *read_buf)
{
	unsigned int dataLength;
	time_t currenttime;
	memset(read_buf ,0 ,50000);
	/*time(&currenttime);*/
	/*fprintf(filefdInternal, " Entry Read Buffer  %s \n " , ctime(&currenttime));*/
	dataLength =read(fd ,read_buf ,50000);
	/*time(&currenttime);*/
	/*fprintf(filefdInternal, " Exit Read Buffer  %s \n" , ctime(&currenttime));*/
	if (dataLength == -1){
		d_warning("Diag_com_read : Reading  error  in device  %d ",errno);
		exit(0);
	}
	return dataLength;
}

int dynamic_log_request(int fd)
{
	int writing_status;
	char *write_buf;
	int len;

	write_buf = (char *)MALLOC(512);
	if(gManufactureType == SAMSUNG){
		write_buf[0]= 0x20;
	}else
		write_buf[0]= 0x40;
	write_buf[1]= 0;
	write_buf[2] =0;
	write_buf[3] =0;
	d_log("Dynamic Logging for 0xB0C2\n ");
	len = cmd_hdr_dynamic_Logging_message (&write_buf[4],508, 0xB0C2);
	d_arraylog(write_buf , len);
	writing_status = write(fd, write_buf, len+4);
	if (writing_status == -1){
		d_warning("Dynamic_log_request : Writing error  while version %d ",errno);
	}
	FREE(write_buf);
	return  writing_status;
}

int Gsm_log_on_demand_request(int fd,u_int16_t cmdcode)
{
	int writing_status;
	char *write_buf;
	int len;
	write_buf = (char *)MALLOC(512);
	if(gManufactureType == SAMSUNG){
		write_buf[0]= 0x20;
	}else
		write_buf[0]= 0x40;
	write_buf[1]= 0;
	write_buf[2] =0;
	write_buf[3] =0;
	d_log("Gsm Log On Demand Request Cmd");
	len = cmd_hdr_Log_On_Demand_Request_Logging_message(&write_buf[4],508, cmdcode);
	d_arraylog(write_buf , len);
	writing_status = write(fd, write_buf, len+4);
	if (writing_status == -1){
		d_warning("Gsm_log_on_demand_request : Writing error = %d ",errno);
	}d_arraylog(write_buf , len+4);
	FREE(write_buf);
	return  writing_status;

}

void  diag_put_efs_request(int fd , u_int32_t data_len, char * data, char * path)
{
	int status;
	char *write_buf;
	int len;
	int path_len  ;
	write_buf = (char *)MALLOC(1024);
	if(gManufactureType == SAMSUNG){
		write_buf[0]= 0x20;
	}else
		write_buf[0]= 0x40;
	write_buf[1]= 0;
	write_buf[2] =0;
	write_buf[3] =0;
	/*prototype -> size_t cmd_efs_diag_put(char *buf, size_t len, u_int32_t data_len, char * data, char * path);*/
	path_len =  strlen(path)+1;
	len = cmd_efs_diag_put (&write_buf[4],1020, data_len,  path_len ,data, path);
	status = write(fd, write_buf, len+4);
	if (status ==  -1){
		d_warning("diag_put_efs_request : writing error =%d ",errno);
	}
	FREE(write_buf);
}

 /* void  diag_get_efs_request( globalFD)*/
 void  diag_get_efs_request(int fd , u_int32_t data_len,  char * path,u_int16_t seqNumber)
{
	int status;
	char *write_buf;
	int len;
	int path_len  ;
	write_buf = (char *)MALLOC(1024);
	if(gManufactureType == SAMSUNG){
		write_buf[0]= 0x20;
	}else
		write_buf[0]= 0x40;
	write_buf[1]= 0;
	write_buf[2] =0;
	write_buf[3] =0;
	path_len =  strlen(path)+1;
	len= cmd_efs_diag_get (&write_buf[4],1020,data_len,path_len, path,seqNumber);
	d_arraylog(write_buf,len+4);
	status = write(fd, write_buf, len+4);
	if (status == -1){
		d_warning("diag_get_efs_request : writing error = %d ",errno);
	}
	FREE(write_buf);
}

void
diag_process_data(queue_t q)
{
	PacketInfo pkt;
	char read_buf[50000];
	unsigned int dataLength;
        unsigned int *logtype = NULL;

	/* call the diag_com_read function to read the data*/
	dataLength = diag_com_read(globalFD, read_buf);
	d_log("diag_process_data");
	d_arraylog(read_buf , dataLength);
	logtype = (int*)&read_buf[0];
	if((*logtype == 32) || (*logtype == 64)) 
	{
		pkt.length = dataLength;
		pkt.msgPtr =  MALLOC(pkt.length);
		memcpy(pkt.msgPtr,read_buf,dataLength);
		if(sendQueue(q, pkt)!=0)
		{
			d_warning("Unable to Send Packet to Processing thread");
			Log_Trace (__FILE__, __LINE__, __func__, LOGL_WARN,
				"Unable to Send Packet to Processing thread");
			FREE(pkt.msgPtr);
		}	
	}		
}

/* function to listen the event on this fd*/
void *diag_com_listen(void * arg)
{
	queue_t q = (queue_t) arg; // We have the Queue now

	fd_set set;
	struct timeval enter , exit;
	struct timeval timeout;
	int retval = 0;
	/* select returns 0 if timeout, 
	                  1 if input atimeoutvailable, 
			 -1 if error. 
	*/
	while (1)
	{
		/* Initialize the file descriptor set. */
		/* Initialize the timeout data structure. */
		timeout.tv_sec = 0;
		timeout.tv_usec = 10*1000;
		FD_ZERO (&set);
		FD_SET (globalFD, &set);
		
		if(!init_done){
			d_log("Exiting Read thread ");
			return ;
		} /*This is existing the thread*/
		gettimeofday(&enter, NULL);
		retval = select(globalFD+1 ,&set,NULL ,NULL ,&timeout);
		if (!init_done) {
			d_log("Exiting Read thread ");
			return ;
		} /*This is existing the thread*/
		if (retval == -1){
			d_warning("error in select()");
		}else if (retval > 0) {
			if(FD_ISSET(globalFD, &set)) {
				diag_process_data(q);
			}
		}
		else if (retval == 0) {
			d_log("System come out from Select with timeout ");
		}
		gettimeofday(&exit, NULL);
		if (((exit.tv_sec-enter.tv_sec) >= 1)
			&& ((exit.tv_usec-enter.tv_usec) >200000)) {

			d_warning("Warning : DATA COLLECTION THREAD "
				"sec = %d usec %d \n", 
				(exit.tv_sec - enter.tv_sec),
				(exit.tv_usec - enter.tv_usec));
		}
	}
}


int  diag_com_close(int fd)
{
	init_done = 0;
	close(fd);
}

int diaginit_diag_com_write_command(u_int8_t deviceType,u_int16_t items[])
{
	return diag_com_write_command(globalFD,deviceType,items);
}
int diaginit_diag_com_write_event(u_int16_t event_item[]){
	return  diag_com_write_event(globalFD,event_item);
}

int  diag_com_gps_write_command(int fd)
{
	int writing_status;
	char *write_buf;
	int len;
	write_buf = (char *)MALLOC(1024);
	if(gManufactureType == SAMSUNG){
		write_buf[0]= 0x20;
	}else
		write_buf[0]= 0x40;
	write_buf[1]= 0;
	write_buf[2] =0;
	write_buf[3] =0;
	len= cmd_gps_last_location(&write_buf[4],1020);
	writing_status = write(fd, write_buf, len+4);
	d_log(" GPS Command write ");
	d_arraylog(write_buf,len+4);
	if (writing_status == -1){
		d_warning("diag_com_gps_write_command : Writing error = %d ",errno);
	}
	FREE(write_buf);
	Log_Trace (__FILE__,__LINE__,__func__,LOGL_SUCCESS,"writing_status = %d ",writing_status);
	return writing_status;
}



void diag_com_cdma_request(int fd,u_int16_t code){
	int writing_status;
	char *write_buf;
	int len;
	write_buf = (char *)MALLOC(1024);
	if(gManufactureType == SAMSUNG)
	{
		//printf("\n in diag_com_cdma_request \n");
		write_buf[0]= 0x20;
		write_buf[1]= 0;
		write_buf[2] =0;
		write_buf[3] =0;
		len= cdma_cmd_write(&write_buf[4],1020,code);
		writing_status = write(fd, write_buf, len+4);
		if (writing_status == -1)
		{
			printf("Writing error to Diag Device %d \n",errno);
		}
		FREE(write_buf);
	}
}
