# include <stdio.h>
# include <string.h>
#include <sys/types.h>
#include "DlfInterfaceApi.h"
#include "diag-init.h"




#define MAX_GPS_CMD_INDEX 32
#define MAX_GPS_EVENT_INDEX 32

static u_int32_t Index = 0;
static u_int16_t GPS_Log_Code[MAX_GPS_CMD_INDEX] = {0};


void insertGPSCmdLimit(int index)
{
	GPS_Log_Code[index]= 0x0;
}

int setGPSLogMask(int layerType)
{
	int returnValue = -1; 
        u_int32_t Index = 0;

	if (layerType & GPS_LAYER_1_LOG)
	{
			GPS_Log_Code[Index++] = 0x1014;
			GPS_Log_Code[Index++] = 0x1462;
			GPS_Log_Code[Index++] = 0x1FEE;
	}


	insertGPSCmdLimit(Index);
	returnValue = diaginit_diag_com_write_command(0x01,GPS_Log_Code);
	memset(GPS_Log_Code,'\0',sizeof(GPS_Log_Code));
	return returnValue;
}

