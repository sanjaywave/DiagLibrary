#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdlib.h>
#include"interfaceapi.h"
typedef signed char	      int8_t;
typedef unsigned char	    u_int8_t;
typedef short		     int16_t;
typedef unsigned short	   u_int16_t;
typedef int		     int32_t;
typedef unsigned int	   u_int32_t;

typedef struct log_packet_details {
    u_int16_t length;
    u_int16_t command_code;
    u_int8_t timestamp[8];
    u_int8_t data[0];
} log_pack;
//typedef struct log_packet_details  log_packet_details;
// include header file for test_packet

//extern GSMCellInfo  mGSMCellInfo;

extern WCDMACellInfo myCellInfo ;
int main()
{
	//file open in binary mode.
	//while (EOF)
	// read first two bytes for length
	// read again for complete log_packet structure of legth read above
	//print the buffer

	FILE *fp;
	size_t count;
	u_int8_t * buff= NULL;//[BUFSIZ];
	log_pack * msg =NULL;
	int index =0;
	u_int16_t length = 0;
	u_int16_t command_code = 0;
	buff = malloc(sizeof(u_int8_t)*BUFSIZ);
	int i =0;
	unsigned int packetCount =0 ;
	for( i=0;i<2;i++)
	{
		
		char strfilename[20]={'\0'};
		strcpy(strfilename,"./test");
		sprintf(strfilename+strlen(strfilename),"%d",i);
		printf("The file name is %s", strfilename);	
		strcat(strfilename,".dlf");
		fp = fopen(strfilename , "rb");
		
		while(1)
		{
			count = fread(&length , 2 , 1 , fp);
			if(count == 0)
				break;
			

		printf("Read length : 	%d\n" , length);
		memcpy(buff, &length, sizeof(u_int16_t));
		count = fread((buff +sizeof(u_int16_t)), length-sizeof(u_int16_t) , 1 , fp);
		//decode_log_msg(msg->command_code,msg->data,msg->length,msg->timestamp);
		/*for (index =0;index <length;index++)
		{
			printf("buffer at index %d is %x\n", index, buff[index]);
		}*/
		msg = buff;
		packetCount++;
		printf("\ncommand code in buffer %x Packet Count = %d\n",msg->command_code , packetCount);
		decode_log_msg(msg->command_code,msg->data,msg->length,msg->timestamp);

		long long timestamp = *( long long*)(&myCellInfo.timestamp[0]);
		
		printf("CELL_ID = %x	PRIM_SC_CODE = %d	UPLINK = %d	DOWNLINK = %d	Timestamp = %lld\n" , myCellInfo.cell_id , myCellInfo.PRIM_SC_CODE , myCellInfo.URAFCN_Downlink , myCellInfo.URAFCN_Uplink , timestamp );	
		
		printf("Active_Set_Info PRIM_SC_CODE = %d	ECIO = %f	RSCP = %d\n " , myCellInfo.activeSetInfo[0].PRIM_SC_CODE , myCellInfo.activeSetInfo[0].ECIO , myCellInfo.activeSetInfo[0].RSCP);

//		printf("Timestamp in GSM = %d	BSIC[0] = %x	BSIC[1] = %x\n" , (int)mGSMCellInfo.timestamp , mGSMCellInfo.BSIC[0] , mGSMCellInfo.BSIC[1]);
		memset(buff, 0, sizeof(sizeof(u_int8_t)*BUFSIZ));
                
		msg = NULL;
	
		}
		fclose(fp);

	}
	return 0;
}
