# include <stdio.h>
# include <string.h>
#include <sys/types.h>
#include "DlfInterfaceApi.h"
#include "diag-init.h"




#define MAX_GSM_CMD_INDEX 256
#define MAX_GSM_EVENT_INDEX 256

static u_int32_t Index = 0;
static u_int32_t Event_Index = 0;
static u_int16_t GSM_Log_Code[MAX_GSM_CMD_INDEX] = {0};
static u_int16_t NAS_Log_Code[MAX_GSM_CMD_INDEX] = {0};


void insertGSMCmdLimit(int index)
{
	GSM_Log_Code[index++]= 0x5C5C;
	GSM_Log_Code[index]= 0x0;
}

int setGSMLogMask(int layerType)
{
	int returnValue = -1; 
#ifdef GSM_ENABLE
        u_int32_t Index = 0, Index1 = 0;
        if (layerType & GSM_LAYER_1_LOG){
			GSM_Log_Code[Index++] = 0x5064;
//			GSM_Log_Code[Index++] = 0x5065;
			GSM_Log_Code[Index++] = 0x5066;
			GSM_Log_Code[Index++] = 0x506C;
			GSM_Log_Code[Index++] = 0x506F;
			GSM_Log_Code[Index++] = 0x5070;
			GSM_Log_Code[Index++] = 0x5071;
			GSM_Log_Code[Index++] = 0x5072;
			GSM_Log_Code[Index++] = 0x5073;
			GSM_Log_Code[Index++] = 0x5075;
			GSM_Log_Code[Index++] = 0x5076;
			GSM_Log_Code[Index++] = 0x5078;
			GSM_Log_Code[Index++] = 0x5079;
			GSM_Log_Code[Index++] = 0x507A;
			GSM_Log_Code[Index++] = 0x507B;
//			GSM_Log_Code[Index++] = 0x507C;
			GSM_Log_Code[Index++] = 0x5082;
			GSM_Log_Code[Index++] = 0x5083;
			GSM_Log_Code[Index++] = 0x5084;
			GSM_Log_Code[Index++] = 0x5085;
			GSM_Log_Code[Index++] = 0x5086;
			GSM_Log_Code[Index++] = 0x5087;
			GSM_Log_Code[Index++] = 0x5088;
			GSM_Log_Code[Index++] = 0x508C;
			//GSM_Log_Code[Index++] = 0x508D;
			GSM_Log_Code[Index++] = 0x508E;
//			GSM_Log_Code[Index++] = 0x507D;
			GSM_Log_Code[Index++] = 0x508F;
			GSM_Log_Code[Index++] = 0x5091;
			GSM_Log_Code[Index++] = 0x5092;
	}

	if(layerType & GSM_LAYER_2_LOG)
	{
			GSM_Log_Code[Index++] = 0x50C8;
			GSM_Log_Code[Index++] = 0x50C9;
			GSM_Log_Code[Index++] = 0x50CA;
	 }
     if (layerType &  GSM_LAYER_3_LOG)
     {
			GSM_Log_Code[Index++] = 0x512C;
			GSM_Log_Code[Index++] = 0x512E;
			GSM_Log_Code[Index++] = 0x512F;
			GSM_Log_Code[Index++] = 0x5130;
			GSM_Log_Code[Index++] = 0x5131;
			GSM_Log_Code[Index++] = 0x5132;
			GSM_Log_Code[Index++] = 0x5133;
			GSM_Log_Code[Index++] = 0x5134;
			GSM_Log_Code[Index++] = 0x5135;
			GSM_Log_Code[Index++] = 0x5136;
			GSM_Log_Code[Index++] = 0x5137;
			GSM_Log_Code[Index++] = 0x5138;
			GSM_Log_Code[Index++] = 0x5139;
			GSM_Log_Code[Index++] = 0x513A;
			GSM_Log_Code[Index++] = 0x513C;
			GSM_Log_Code[Index++] = 0x525C;
     }

        if (layerType & GSM_CS_data_LOG)
          {
			GSM_Log_Code[Index++] = 0x5190;
			GSM_Log_Code[Index++] = 0x5191;
           }

        if (layerType & GSM_inter_RAT_LOG)
        {       	GSM_Log_Code[Index++] = 0x5258;
			GSM_Log_Code[Index++] = 0x5259;
			GSM_Log_Code[Index++] = 0x525A;
			GSM_Log_Code[Index++] = 0x525B;
        }
       if (layerType & GSM_PHY_MDSP_IQ_LOG)
       {
			GSM_Log_Code[Index++] = 0x522F;
       }


	 if (layerType & GPRS_GRR_LAYER_LOG)
	 {
			GSM_Log_Code[Index++] = 0x51F4;
			GSM_Log_Code[Index++] = 0x51F5;
			GSM_Log_Code[Index++] = 0x51F6;
			GSM_Log_Code[Index++] = 0x51F7;
			GSM_Log_Code[Index++] = 0x51F8;
			GSM_Log_Code[Index++] = 0x51F9;
			GSM_Log_Code[Index++] = 0x51FA;
			GSM_Log_Code[Index++] = 0x51FB;
			GSM_Log_Code[Index++] = 0x51FC;
			GSM_Log_Code[Index++] = 0x51FD;
			GSM_Log_Code[Index++] = 0x51FE;
			GSM_Log_Code[Index++] = 0x51FF;
			GSM_Log_Code[Index++] = 0x5262;
			GSM_Log_Code[Index++] = 0x5263;
	 }
	 
	
	if (layerType & GPRS_RLC_UL_LAYER_LOG)
	{
			GSM_Log_Code[Index++] = 0x5200;
			GSM_Log_Code[Index++] = 0x5201;
			GSM_Log_Code[Index++] = 0x5202;
			GSM_Log_Code[Index++] = 0x5207;
			GSM_Log_Code[Index++] = 0x5209;
			GSM_Log_Code[Index++] = 0x520D;
			GSM_Log_Code[Index++] = 0x520F;
	 }
         if (layerType & GPRS_RLC_DL_LAYER_LOG)
        {
			GSM_Log_Code[Index++] = 0x520A;
			GSM_Log_Code[Index++] = 0x5208;
			GSM_Log_Code[Index++] = 0x520E;
			GSM_Log_Code[Index++] = 0x5210;
			GSM_Log_Code[Index++] = 0x5211;
	}

        if (layerType & GPRS_LLC_LAYER_LOG)
	{
			GSM_Log_Code[Index++] = 0x5212;
			GSM_Log_Code[Index++] = 0x5213;
			GSM_Log_Code[Index++] = 0x5214;
			GSM_Log_Code[Index++] = 0x5215;
			GSM_Log_Code[Index++] = 0x5216;
			GSM_Log_Code[Index++] = 0x5217;
			GSM_Log_Code[Index++] = 0x521A;
			GSM_Log_Code[Index++] = 0x5219;
			GSM_Log_Code[Index++] = 0x5218;

	}
        
        if (layerType & GPRS_SNDCP_LAYER_LOG)
	{
			GSM_Log_Code[Index++] = 0x521C;
			GSM_Log_Code[Index++] = 0x521D;
			GSM_Log_Code[Index++] = 0x521E;
			GSM_Log_Code[Index++] = 0x521F;
			GSM_Log_Code[Index++] = 0x5220;
			GSM_Log_Code[Index++] = 0x5221;
	}
        if (layerType & GPRS_MAC_LAYER_LOG)
	{
			GSM_Log_Code[Index++] = 0x5226;
			GSM_Log_Code[Index++] = 0x5227;
			GSM_Log_Code[Index++] = 0x5228;
			GSM_Log_Code[Index++] = 0x5229;
			GSM_Log_Code[Index++] = 0x522A;
			GSM_Log_Code[Index++] = 0x522B;
			GSM_Log_Code[Index++] = 0x5206;
			GSM_Log_Code[Index++] = 0x5205;
			GSM_Log_Code[Index++] = 0x522D;
			GSM_Log_Code[Index++] = 0x522C;
			GSM_Log_Code[Index++] = 0x5230;
			//GSM_Log_Code[Index++] = 0x5231;

	}
        if (layerType & GPRS_LAYER_1_LOG)
	{
			GSM_Log_Code[Index++] = 0x5232;
			GSM_Log_Code[Index++] = 0x5233;
			GSM_Log_Code[Index++] = 0x5234;
			GSM_Log_Code[Index++] = 0x5235;
			GSM_Log_Code[Index++] = 0x5236;
			GSM_Log_Code[Index++] = 0x5237;
			GSM_Log_Code[Index++] = 0x5245;
			GSM_Log_Code[Index++] = 0x5246;
			GSM_Log_Code[Index++] = 0x5247;
			GSM_Log_Code[Index++] = 0x5248;
			GSM_Log_Code[Index++] = 0x524D;
			GSM_Log_Code[Index++] = 0x524E;
			GSM_Log_Code[Index++] = 0x524F;
			GSM_Log_Code[Index++] = 0x5250;
			GSM_Log_Code[Index++] = 0x5251;
			GSM_Log_Code[Index++] = 0x5252;
			GSM_Log_Code[Index++] = 0x523F;
			GSM_Log_Code[Index++] = 0x522E;
			GSM_Log_Code[Index++] = 0x5284;

	}
        if  (layerType  &  GSM_NAS_LAYER_LOG)
        {   NAS_Log_Code[Index1++] = 0x7130;
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
			NAS_Log_Code[Index1++] = 0x7150;
			NAS_Log_Code[Index1] = 0;
        }
    insertGSMCmdLimit(Index);
	returnValue = diaginit_diag_com_write_command(0x05,GSM_Log_Code);
	returnValue = diaginit_diag_com_write_command(0x07,NAS_Log_Code);
	memset(GSM_Log_Code,'\0',sizeof(GSM_Log_Code));
	memset(NAS_Log_Code,'\0',sizeof(NAS_Log_Code));
#endif
	return returnValue;
}



