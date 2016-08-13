# include <stdio.h>
# include <string.h>
#include <sys/types.h>
#include "DlfInterfaceApi.h"
#include "diag-init.h"




#define MAX_LTE_CMD_INDEX 256
#define MAX_LTE_EVENT_INDEX 256

static u_int16_t Lte_Log_Code[MAX_LTE_CMD_INDEX] = {0};
static u_int16_t Lte_Event[MAX_LTE_EVENT_INDEX] = {0};


void insertLteCmdLimit(int index)
{
	Lte_Log_Code[index++]= 0xB902;
	Lte_Log_Code[index]= 0x0;
}

int32_t setLTELogMask(int layerType)
{
	int32_t returnValue = -1;
#ifdef LTE_ENABLE
	u_int32_t Index = 0;
	u_int32_t Event_Index = 0;

	if (layerType & LTE_LOWER_LAYER_1)//5.2 - 5.26
	{
			//Lte_Log_Code[Index++]= 0xB111;
			//Lte_Log_Code[Index++]= 0xB113;
			//Lte_Log_Code[Index++]= 0xB114;
			//Lte_Log_Code[Index++]= 0xB115;
			//Lte_Log_Code[Index++]= 0xB116;
			//Lte_Log_Code[Index++]= 0xB119;
			//Lte_Log_Code[Index++]= 0xB11B;
			//Lte_Log_Code[Index++]= 0xB11C;
			//Lte_Log_Code[Index++]= 0xB11D;
			//Lte_Log_Code[Index++]= 0xB120;
			//Lte_Log_Code[Index++]= 0xB122;
			//Lte_Log_Code[Index++]= 0xB123;
			//Lte_Log_Code[Index++]= 0xB125;
			Lte_Log_Code[Index++]= 0xB12A;
			Lte_Log_Code[Index++]= 0xB12C;
			Lte_Log_Code[Index++]= 0xB12E;
			Lte_Log_Code[Index++]= 0xB130;
			Lte_Log_Code[Index++]= 0xB132;
			Lte_Log_Code[Index++]= 0xB139;
			Lte_Log_Code[Index++]= 0xB13C;
			Lte_Log_Code[Index++]= 0xB140;
			Lte_Log_Code[Index++]= 0xB144;
			Lte_Log_Code[Index++]= 0xB146;
			Lte_Log_Code[Index++]= 0xB14D;
			Lte_Log_Code[Index++]= 0xB14E;
	}
	if (layerType & LTE_MANAGEMENT_LAYER_LOG)
	{

			Lte_Log_Code[Index++] = 0xB160;
			Lte_Log_Code[Index++] = 0xB161;
			Lte_Log_Code[Index++] = 0xB162;
			Lte_Log_Code[Index++] = 0xB163;
			Lte_Log_Code[Index++] = 0xB164;
			Lte_Log_Code[Index++] = 0xB165;
			Lte_Log_Code[Index++] = 0xB166;
			Lte_Log_Code[Index++] = 0xB1B0;
			Lte_Log_Code[Index++] = 0xB167;
			Lte_Log_Code[Index++] = 0xB168;
			Lte_Log_Code[Index++] = 0xB169;
			Lte_Log_Code[Index++] = 0xB16A;
			Lte_Log_Code[Index++] = 0xB16B;
			Lte_Log_Code[Index++] = 0xB16C;
			Lte_Log_Code[Index++] = 0xB16D;
			Lte_Log_Code[Index++] = 0xB16E;
			Lte_Log_Code[Index++] = 0xB16F;
			Lte_Log_Code[Index++] = 0xB170;
			Lte_Log_Code[Index++] = 0xB171;
			Lte_Log_Code[Index++] = 0xB172;
			Lte_Log_Code[Index++] = 0xB173;
			Lte_Log_Code[Index++] = 0xB18A;
			Lte_Log_Code[Index++] = 0xB198;
			Lte_Log_Code[Index++] = 0xB1B1;
			Lte_Log_Code[Index++] = 0xB176;
			Lte_Log_Code[Index++] = 0xB177;
			Lte_Log_Code[Index++] = 0xB179;
			Lte_Log_Code[Index++] = 0xB19B;
			Lte_Log_Code[Index++] = 0xB19C;
			Lte_Log_Code[Index++] = 0xB19D;
			Lte_Log_Code[Index++] = 0xB17A;
			Lte_Log_Code[Index++] = 0xB17C;
			Lte_Log_Code[Index++] = 0xB17D;
			Lte_Log_Code[Index++] = 0xB17E;
			Lte_Log_Code[Index++] = 0xB17F;
			Lte_Log_Code[Index++] = 0xB180;
			Lte_Log_Code[Index++] = 0xB181;
			Lte_Log_Code[Index++] = 0xB183;
			Lte_Log_Code[Index++] = 0xB185;
			Lte_Log_Code[Index++] = 0xB186;
			Lte_Log_Code[Index++] = 0xB187;
			Lte_Log_Code[Index++] = 0xB189;
			Lte_Log_Code[Index++] = 0xB192;
			Lte_Log_Code[Index++] = 0xB193;
			Lte_Log_Code[Index++] = 0xB194;
			Lte_Log_Code[Index++] = 0xB195;
			Lte_Log_Code[Index++] = 0xB197;
			Lte_Log_Code[Index++] = 0xB18B;
			Lte_Log_Code[Index++] = 0xB1A0;
			Lte_Log_Code[Index++] = 0xB1A1;
			Lte_Log_Code[Index++] = 0xB1A2;
			Lte_Log_Code[Index++] = 0xB1A3;
			Lte_Log_Code[Index++] = 0xB1A4;
			Lte_Log_Code[Index++] = 0xB1A5;
			Lte_Log_Code[Index++] = 0xB1A6;
			Lte_Log_Code[Index++] = 0xB1B5;
			Lte_Log_Code[Index++] = 0xB1BA;
			Lte_Log_Code[Index++] = 0xB1BB;
			Lte_Log_Code[Index++] = 0xB1BC;
			Lte_Log_Code[Index++] = 0xB1BD;
			Lte_Log_Code[Index++] = 0xB1BE;
			Lte_Log_Code[Index++] = 0xB1B7;
			Lte_Log_Code[Index++] = 0xB1B8;
			Lte_Log_Code[Index++] = 0xB1B9;
	}
	if(layerType & LTE_RRC_LAYER_LOG)
	{
			Lte_Log_Code[Index++] = 0xB0C0;
			Lte_Log_Code[Index++] = 0xB0C1;
			Lte_Log_Code[Index++] = 0xB0C2;
			Lte_Log_Code[Index++] = 0xB0C3;
			Lte_Log_Code[Index++] = 0xB0C4;
			Lte_Log_Code[Index++] = 0xB0C5;
			Lte_Log_Code[Index++] = 0xB0C6;
	 }
     if (layerType &  LTE_MAC_LAYER_LOG)
     {
			Lte_Log_Code[Index++] = 0xB060;
			Lte_Log_Code[Index++] = 0xB061;
			Lte_Log_Code[Index++] = 0xB062;
			Lte_Log_Code[Index++] = 0xB063;
			Lte_Log_Code[Index++] = 0xB064;
			Lte_Log_Code[Index++] = 0xB067;
     }
	 if (layerType & LTE_NAS_LAYER_LOG)
	 {
			Lte_Log_Code[Index++] = 0xB0E0;
			Lte_Log_Code[Index++] = 0xB0E1;
			Lte_Log_Code[Index++] = 0xB0E2;
			Lte_Log_Code[Index++] = 0xB0E3;
			Lte_Log_Code[Index++] = 0xB0E4;
			Lte_Log_Code[Index++] = 0xB0E5;
			Lte_Log_Code[Index++] = 0xB0E6;
			Lte_Log_Code[Index++] = 0xB0EA;
			Lte_Log_Code[Index++] = 0xB0EB;
			Lte_Log_Code[Index++] = 0xB0EC;
			Lte_Log_Code[Index++] = 0xB0ED;
			//Lte_Log_Code[Index++] = 0xB0EE;
			//Lte_Log_Code[Index++] = 0xB0EF;
			Lte_Log_Code[Index++] = 0xB0F5;
			Lte_Log_Code[Index++] = 0xB0F6;
	 }
	 if (layerType & LTE_RLC_LAYER_LOG)
	 {
			Lte_Log_Code[Index++] = 0xB081;
			Lte_Log_Code[Index++] = 0xB082;
			Lte_Log_Code[Index++] = 0xB083;
			Lte_Log_Code[Index++] = 0xB084;
			Lte_Log_Code[Index++] = 0xB085;
			Lte_Log_Code[Index++] = 0xB086;
			Lte_Log_Code[Index++] = 0xB087;
			Lte_Log_Code[Index++] = 0xB091;
			Lte_Log_Code[Index++] = 0xB092;
			Lte_Log_Code[Index++] = 0xB093;
			Lte_Log_Code[Index++] = 0xB094;
			Lte_Log_Code[Index++] = 0xB095;
			Lte_Log_Code[Index++] = 0xB096;
			Lte_Log_Code[Index++] = 0xB097;
    }
	if (layerType & LTE_PDCP_LAYER_LOG)
	{
			Lte_Log_Code[Index++] = 0xB0A0;
			Lte_Log_Code[Index++] = 0xB0A1;
			Lte_Log_Code[Index++] = 0xB0A2;
			Lte_Log_Code[Index++] = 0xB0A3;
			Lte_Log_Code[Index++] = 0xB0A4;
			Lte_Log_Code[Index++] = 0xB0A5;
			Lte_Log_Code[Index++] = 0xB0B0;
			Lte_Log_Code[Index++] = 0xB0B2;
			Lte_Log_Code[Index++] = 0xB0B3;
			Lte_Log_Code[Index++] = 0xB0B4;
			Lte_Log_Code[Index++] = 0xB0B5;
			Lte_Log_Code[Index++] = 0xB0CF;
	}
	if (layerType & LTE_EVENT)
	{
			Lte_Event[Event_Index++] = 1938;
			Lte_Event[Event_Index++] = 1963;
			Lte_Event[Event_Index++] = 1498;
			Lte_Event[Event_Index++] = 1499;
			Lte_Event[Event_Index++] = 1500;
			Lte_Event[Event_Index++] = 1501;
			Lte_Event[Event_Index++] = 1503;
			Lte_Event[Event_Index++] = 1502;
			Lte_Event[Event_Index++] = 1718;
			Lte_Event[Event_Index++] = 1719;
			Lte_Event[Event_Index++] = 1720;
			Lte_Event[Event_Index++] = 1605;
			Lte_Event[Event_Index++] = 1606;
			Lte_Event[Event_Index++] = 1994;
			Lte_Event[Event_Index++] = 1607;
			Lte_Event[Event_Index++] = 1608;
			Lte_Event[Event_Index++] = 1995;
			Lte_Event[Event_Index++] = 1609;
			Lte_Event[Event_Index++] = 1610;
			Lte_Event[Event_Index++] = 1611;
		    Lte_Event[Event_Index++] = 1612;
			Lte_Event[Event_Index++] = 1613;
			Lte_Event[Event_Index++] = 1614;
			Lte_Event[Event_Index++] = 1615;
			Lte_Event[Event_Index++] = 1616;
			Lte_Event[Event_Index++] = 1807;
			Lte_Event[Event_Index++] = 1808;
			Lte_Event[Event_Index++] = 1617;
			Lte_Event[Event_Index++] = 1618;
			Lte_Event[Event_Index++] = 1957;
			Lte_Event[Event_Index++] = 1958;
			Lte_Event[Event_Index++] = 1619;
			Lte_Event[Event_Index++] = 1888;
			Lte_Event[Event_Index++] = 1970;
			Lte_Event[Event_Index++] = 1971;
			Lte_Event[Event_Index++] = 1972;
			Lte_Event[Event_Index++] = 1973;
			Lte_Event[Event_Index++] = 1966;
			Lte_Event[Event_Index++] = 1967;
			Lte_Event[Event_Index++] = 1633;
			Lte_Event[Event_Index++] = 1634;
			Lte_Event[Event_Index++] = 1635;
			Lte_Event[Event_Index++] = 1636;
			Lte_Event[Event_Index++] = 1631;
			Lte_Event[Event_Index++] = 1632;
			Lte_Event[Event_Index++] = 1968;
			Lte_Event[Event_Index++] = 1969;
			Lte_Event[Event_Index++] = 1627;
			Lte_Event[Event_Index++] = 1628;
			Lte_Event[Event_Index++] = 1629;
			Lte_Event[Event_Index++] = 1630;
			Lte_Event[Event_Index++] = 1637;
			Lte_Event[Event_Index++] = 1638;
			Lte_Event[Event_Index++] = 2112;
			Lte_Event[Event_Index++] = 0;
	}
	insertLteCmdLimit(Index);
	returnValue = diaginit_diag_com_write_command(0x0B,Lte_Log_Code);
	returnValue = diaginit_diag_com_write_event(Lte_Event);
	memset(Lte_Log_Code,'\0',sizeof(Lte_Log_Code));
	memset(Lte_Event,'\0',sizeof(Lte_Event));
#endif
	return returnValue;
}



