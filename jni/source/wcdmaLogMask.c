# include <stdio.h>
# include <string.h>
#include <sys/types.h>
#include "DlfInterfaceApi.h"
#include "diag-init.h"
#include "errors.h"


#define MAX_WCDMA_CMD_INDEX 256
#define MAX_WCDMA_EVENT_INDEX 256

static u_int16_t WCDMA_Log_Code[MAX_WCDMA_CMD_INDEX] = {0};
static u_int16_t NAS_Log_Code[MAX_WCDMA_CMD_INDEX] = {0};

void insertWCDMACmdLimit(int index)
{
	WCDMA_Log_Code[index++]= 0x4910;
	WCDMA_Log_Code[index]= 0x0;
}
void insertNASCmdLimit(int index)
{
		NAS_Log_Code[index++] = 0x7150;
		NAS_Log_Code[index] = 0;
		Log_Trace (__FILE__,__LINE__,__func__,LOGL_SUCCESS,"NSA Command Limit Inserted ");
}

int setWCDMALogMask(int layerType)
{
	int returnValue = -1;
#ifdef WCDMA_ENABLE
	u_int32_t Index = 0, Index1 = 0;
	if (layerType & WCDMA_LAYER_1_LOG)
	{
			WCDMA_Log_Code[Index++] = 0x4003;
			WCDMA_Log_Code[Index++] = 0x4004;
			WCDMA_Log_Code[Index++] = 0x4005;
			WCDMA_Log_Code[Index++] = 0x4006;
			WCDMA_Log_Code[Index++] = 0x4010;
			WCDMA_Log_Code[Index++] = 0x4011;
			WCDMA_Log_Code[Index++] = 0x4015;
			WCDMA_Log_Code[Index++] = 0x4017;
			WCDMA_Log_Code[Index++] = 0x4018;
			WCDMA_Log_Code[Index++] = 0x4019;
			WCDMA_Log_Code[Index++] = 0x401A;
			WCDMA_Log_Code[Index++] = 0x401C;
			WCDMA_Log_Code[Index++] = 0x4108;
			WCDMA_Log_Code[Index++] = 0x4109;
			WCDMA_Log_Code[Index++] = 0x410A;
			WCDMA_Log_Code[Index++] = 0x410B;
			WCDMA_Log_Code[Index++] = 0x410C;
			WCDMA_Log_Code[Index++] = 0x410D;
			WCDMA_Log_Code[Index++] = 0x410E;
			WCDMA_Log_Code[Index++] = 0x4110;
			WCDMA_Log_Code[Index++] = 0x4111;
			WCDMA_Log_Code[Index++] = 0x4112;
			WCDMA_Log_Code[Index++] = 0x4113;
			WCDMA_Log_Code[Index++] = 0x4115;
			WCDMA_Log_Code[Index++] = 0x4116;
			WCDMA_Log_Code[Index++] = 0x4138;
			WCDMA_Log_Code[Index++] = 0x413F;
			WCDMA_Log_Code[Index++] = 0x414C;
			WCDMA_Log_Code[Index++] = 0x414D;
			WCDMA_Log_Code[Index++] = 0x414E;
			WCDMA_Log_Code[Index++] = 0x4155;
			WCDMA_Log_Code[Index++] = 0x4156;
			WCDMA_Log_Code[Index++] = 0x4160;
			WCDMA_Log_Code[Index++] = 0x4165;
			WCDMA_Log_Code[Index++] = 0x4166;
			WCDMA_Log_Code[Index++] = 0x4174;
			WCDMA_Log_Code[Index++] = 0x4175;
			WCDMA_Log_Code[Index++] = 0x4176;
			WCDMA_Log_Code[Index++] = 0x4177;
			WCDMA_Log_Code[Index++] = 0x4178;
			WCDMA_Log_Code[Index++] = 0x4179;
			WCDMA_Log_Code[Index++] = 0x4180;
			WCDMA_Log_Code[Index++] = 0x4181;
			WCDMA_Log_Code[Index++] = 0x4182;
			WCDMA_Log_Code[Index++] = 0x4183;
			WCDMA_Log_Code[Index++] = 0x4184;
			WCDMA_Log_Code[Index++] = 0x4185;
			WCDMA_Log_Code[Index++] = 0x4721;
			WCDMA_Log_Code[Index++] = 0x4722;
			WCDMA_Log_Code[Index++] = 0x4733;
			WCDMA_Log_Code[Index++] = 0x4186;
          }
	if(layerType & WCDMA_LAYER_2_LOG)
	{
			WCDMA_Log_Code[Index++] = 0x4002;
			WCDMA_Log_Code[Index++] = 0x4117;
			WCDMA_Log_Code[Index++] = 0x4119;
			WCDMA_Log_Code[Index++] = 0x411A;
			WCDMA_Log_Code[Index++] = 0x411B;
			WCDMA_Log_Code[Index++] = 0x411D;
            WCDMA_Log_Code[Index++] = 0x411F;
			WCDMA_Log_Code[Index++] = 0x4120;
			WCDMA_Log_Code[Index++] = 0x4121;
			WCDMA_Log_Code[Index++] = 0x4123;
			WCDMA_Log_Code[Index++] = 0x4124;
			WCDMA_Log_Code[Index++] = 0x4132;
			WCDMA_Log_Code[Index++] = 0x4133;
            WCDMA_Log_Code[Index++] = 0x4134;
			WCDMA_Log_Code[Index++] = 0x4135;
			WCDMA_Log_Code[Index++] = 0x413A;
			WCDMA_Log_Code[Index++] = 0x413B;
			WCDMA_Log_Code[Index++] = 0x413C;
			WCDMA_Log_Code[Index++] = 0x4145;
			WCDMA_Log_Code[Index++] = 0x4146;
			WCDMA_Log_Code[Index++] = 0x414A;
			WCDMA_Log_Code[Index++] = 0x4157;
			WCDMA_Log_Code[Index++] = 0x4161;
			WCDMA_Log_Code[Index++] = 0x4162;
			WCDMA_Log_Code[Index++] = 0x4168;
			WCDMA_Log_Code[Index++] = 0x4169;
			WCDMA_Log_Code[Index++] = 0x418B;
	 }
     if (layerType &  WCDMA_LAYER_3_LOG)
     {
			WCDMA_Log_Code[Index++] = 0x4125;
			WCDMA_Log_Code[Index++] = 0x4126;
			WCDMA_Log_Code[Index++] = 0x4127;
			WCDMA_Log_Code[Index++] = 0x412F;
     }
     if  (layerType & HSPDA_LAYER_1_LOG)
     {
			WCDMA_Log_Code[Index++] = 0x4200;
			WCDMA_Log_Code[Index++] = 0x4203;
			WCDMA_Log_Code[Index++] = 0x4204;
			WCDMA_Log_Code[Index++] = 0x4205;
			WCDMA_Log_Code[Index++] = 0x4206;
			WCDMA_Log_Code[Index++] = 0x4207;
			WCDMA_Log_Code[Index++] = 0x4208;
			WCDMA_Log_Code[Index++] = 0x420A;
			WCDMA_Log_Code[Index++] = 0x420B;
			WCDMA_Log_Code[Index++] = 0x421C;
			WCDMA_Log_Code[Index++] = 0x421F;
			WCDMA_Log_Code[Index++] = 0x422E;
			WCDMA_Log_Code[Index++] = 0x4221;
			WCDMA_Log_Code[Index++] = 0x4222;
			WCDMA_Log_Code[Index++] = 0x4225;
			WCDMA_Log_Code[Index++] = 0x422B;
			WCDMA_Log_Code[Index++] = 0x4214;
			
      }

      if  (layerType & HSPDA_MAC_LAYER_LOG)
      {
			WCDMA_Log_Code[Index++] = 0x4210;
			WCDMA_Log_Code[Index++] = 0x4211;
			WCDMA_Log_Code[Index++] = 0x4212;
			WCDMA_Log_Code[Index++] = 0x4213;
			WCDMA_Log_Code[Index++] = 0x421E;
			WCDMA_Log_Code[Index++] = 0x4220;
      }

       if  (layerType & HSUPA_LAYER_1_LOG)
       {
			WCDMA_Log_Code[Index++] = 0x4301;
			WCDMA_Log_Code[Index++] = 0x4303;
			WCDMA_Log_Code[Index++] = 0x4305;
			WCDMA_Log_Code[Index++] = 0x4307;
       }

       if   (layerType & HSUPA_MAC_LAYER_LOG)
       {
			WCDMA_Log_Code[Index++] = 0x4309;
			WCDMA_Log_Code[Index++] = 0x4311;
			WCDMA_Log_Code[Index++] = 0x4321;
			WCDMA_Log_Code[Index++] = 0x4322;
        }
        if  (layerType  &  UTMS_NAS_LAYER_LOG)
        {
        	NAS_Log_Code[Index1++] = 0x7130;
			NAS_Log_Code[Index1++] = 0x7131;
			NAS_Log_Code[Index1++] = 0x7132;
			NAS_Log_Code[Index1++] = 0x7133;
			NAS_Log_Code[Index1++] = 0x7134;
			NAS_Log_Code[Index1++] = 0x7135;
			NAS_Log_Code[Index1++] = 0x7136;
			NAS_Log_Code[Index1++] = 0x7137;
			NAS_Log_Code[Index1++] = 0x713D;
			NAS_Log_Code[Index1++] = 0x713E;
			NAS_Log_Code[Index1++] = 0x7138;
			NAS_Log_Code[Index1++] = 0x7139;
			NAS_Log_Code[Index1++] = 0x713A;
		}
        if (layerType & WCDMA_GPS_LOG)
        {
        	NAS_Log_Code[Index1++] = 0x7002;
        	NAS_Log_Code[Index1++] = 0x7003;
        	NAS_Log_Code[Index1++] = 0x7005;
        	NAS_Log_Code[Index1++] = 0x7008;
        	NAS_Log_Code[Index1++] = 0x7009;
        	NAS_Log_Code[Index1++] = 0x700D;
        	NAS_Log_Code[Index1++] = 0x701E;
        	NAS_Log_Code[Index1++] = 0x701F;
        }
        
		insertWCDMACmdLimit(Index);
		insertNASCmdLimit(Index1);
		returnValue = diaginit_diag_com_write_command(0x04,WCDMA_Log_Code);
		returnValue = diaginit_diag_com_write_command(0x07,NAS_Log_Code);
		memset(WCDMA_Log_Code,'\0',sizeof(WCDMA_Log_Code));
		memset(NAS_Log_Code,'\0',sizeof(NAS_Log_Code));
#endif
		Log_Trace (__FILE__,__LINE__,__func__,LOGL_SUCCESS,"NSA Command Write Status = %d ",returnValue);
		return returnValue;
}





