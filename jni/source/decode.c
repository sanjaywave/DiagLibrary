#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <endian.h>
#include <sys/types.h>
#include <pthread.h>
#include "decode.h"
#include "interfaceapi.h"
#include "extmsgid.h"
#include "errors.h"
#include "CdmaContext.h"

extern int globalUrfcnFreq;
extern u_int64_t gbandlockFreq;
extern LTECellInfo mLTE_CellInfo;
extern WCDMACellInfo myCellInfo;
extern GSMCellInfo mGSMCellInfo;
int systemTypeDetected = NOT_INIT;
int lastSysTypeDetected = NOT_INIT;
extern pthread_mutex_t cellInfoMutex;
#define MAX_CONTINUE_PACKET  10
static unsigned int maxCountPacketWCDMA = 0;
static unsigned int maxCountPacketGSM = 0;
static unsigned int maxCountPacketLTE = 0;
static unsigned int maxCountPacketCDMA = 0;


u_int64_t timeDiff = 0;
#ifdef CDMA_ENABLE
	CdmaKpi  mCdmaKpi;
	Kpi_CDMA mkpi_cdma;
#endif


int decode_get_state_info()
 {
    return systemTypeDetected;
 }

void setSystemtype(u_int16_t cmd)
 {
 int series = (cmd >> 12);
 if (4==series)
 {
      maxCountPacketWCDMA++;
      maxCountPacketGSM =0;
      maxCountPacketLTE= 0;
      maxCountPacketCDMA = 0;		
      if(systemTypeDetected == NOT_INIT)
      {
      	systemTypeDetected = DETECT_WCDMA;
      	lastSysTypeDetected = DETECT_WCDMA;
      }
      else if ((systemTypeDetected != DETECT_WCDMA) && (maxCountPacketWCDMA > MAX_CONTINUE_PACKET))
      { 	/*set the memset for LTE and GSM*/
         memset(&mLTE_CellInfo,0,sizeof(mLTE_CellInfo));
         memset(&mGSMCellInfo,0,sizeof(mGSMCellInfo));
         systemTypeDetected = DETECT_WCDMA;
      }
  }
  else if (5==series)
  {
	  //d_warning("GSM Command Code received %2x\n",cmd);
      maxCountPacketWCDMA =0;
      maxCountPacketGSM++;
      maxCountPacketLTE= 0;
      maxCountPacketCDMA = 0;		
      if(systemTypeDetected == NOT_INIT)
      {
      	systemTypeDetected = DETECT_GSM;
      	lastSysTypeDetected = DETECT_GSM;
      }
      else if ((systemTypeDetected != DETECT_GSM) && (maxCountPacketGSM > MAX_CONTINUE_PACKET))
      { 	/*set the memset for LTE and WCDMA*/
      	memset(&myCellInfo,0,sizeof(myCellInfo));
      	memset(&mLTE_CellInfo,0,sizeof(mLTE_CellInfo));
      	systemTypeDetected = DETECT_GSM;

     }
  }
  else if (11==series)
  {
	  //d_warning("LTE Command Code received %2x\n",cmd);
      maxCountPacketWCDMA =0;
      maxCountPacketGSM = 0;
      maxCountPacketLTE++;
      maxCountPacketCDMA = 0;		
      if(systemTypeDetected == NOT_INIT)
      {
      	systemTypeDetected = DETECT_LTE;
      	lastSysTypeDetected = DETECT_LTE;
      }
      else if((systemTypeDetected !=DETECT_LTE )&& (maxCountPacketLTE > MAX_CONTINUE_PACKET))
      { /*set the memset for GSM and WCDMA*/
      	memset(&myCellInfo,0,sizeof(myCellInfo));
      	memset(&mGSMCellInfo,0,sizeof(mGSMCellInfo));
      	systemTypeDetected = DETECT_LTE;

      }
  }
  else if (1==series )
  {
	  //d_warning("CDMA Command Code received %2x\n",cmd);
      maxCountPacketWCDMA =0;
      maxCountPacketGSM = 0;
      maxCountPacketLTE = 0;
      maxCountPacketCDMA++;		
      if(systemTypeDetected == NOT_INIT)
      {
      	systemTypeDetected = DETECT_CDMA;
      	lastSysTypeDetected = DETECT_CDMA;
      }
      else if ((systemTypeDetected != DETECT_CDMA) && (maxCountPacketCDMA > MAX_CONTINUE_PACKET))
      { 	/*set the memset for LTE, GSM and WCDMA*/
     	memset(&myCellInfo,0,sizeof(myCellInfo));
      	memset(&mLTE_CellInfo,0,sizeof(mLTE_CellInfo));
      	memset(&mGSMCellInfo,0,sizeof(mGSMCellInfo));
      	systemTypeDetected = DETECT_CDMA;

     }
  }
 	/*EVENT_IRAT_TECHNOLOGY_CHANGE */
 	if((lastSysTypeDetected != systemTypeDetected ) && (lastSysTypeDetected != NOT_INIT))
 	{
 		event_irat_technology_change RadioTechnologyChange;
 		RadioTechnologyChange.Current_technology = systemTypeDetected;
 		RadioTechnologyChange.Last_technology = lastSysTypeDetected;
 		lastSysTypeDetected = systemTypeDetected;
 		sendInformation(systemTypeDetected, EVENT_DATA , EVENT_IRAT_TECHNOLOGY_CHANGE ,(char *)&RadioTechnologyChange,sizeof(RadioTechnologyChange));
 	}
}

int  decode_log_msg(u_int16_t command_code, unsigned char *buf ,int length, u_int8_t timestamp[8])
{
   setSystemtype(command_code);
	int code =convert_into_external_code(command_code);
	pthread_mutex_lock(&cellInfoMutex);
	if ((code >= 1000)&&(code<2000)){
#ifdef GSM_ENABLE
		gms_decode_log_msg(code,buf ,length, timestamp);
#endif
	}else if((code>=2000)&&(code<3000)){
#ifdef WCDMA_ENABLE
		wcdma_decode_log_msg(code,buf ,length, timestamp);
#endif
	}else if((code>=3000)&&(code<4000)){
#ifdef LTE_ENABLE
		lte_decode_log_msg(code,buf ,length, timestamp);
#endif
	}else if((code>=4000)&&(code<5000)){
		nas_decode_log_msg(code,buf ,length, timestamp);
	}else if((code>=6000)&&(code<7000)){
#ifdef CDMA_ENABLE
	cdma_decode_log_msg(&mCdmaKpi ,code,buf ,length); //timestamp);
#endif
	}
	pthread_mutex_unlock(&cellInfoMutex);
	return 0;
}


int DmTimeResponse(unsigned char *buff, unsigned int packetlength)
{
	u_int64_t DMmilisecond = 0 ,sysmilisecond = 0 ;
	struct timeval enter;
	//int i = 0;
	//timeDiff = 0 ; /* Clear the last Value */
	int retval = -1;
	/* Check for incoming buffer length */
	//VERIFY_LENGTH(dm_time_response ,packetlength,0);
	//printf("\nDM Time Responce Command");
	//for(i = 0 ;i < packetlength ;i++ )printf(" %2x",buff[i]);
	dm_time_response *dtr = (dm_time_response *)&buff[0];
	//printf("\ncounter = %llu ",dtr->counter );
	DMmilisecond = ((unsigned long long)dtr->counter*125)/100; /* This time is in milisecond */
	/* Get the system time */
	retval = gettimeofday(&enter, NULL);
	if(!retval){
		sysmilisecond = ((unsigned long long)enter.tv_sec*1000) + (enter.tv_usec/1000);
		timeDiff = sysmilisecond - DMmilisecond;
		printf("TIME DIFF = %llu",timeDiff);
	}else{
		d_warning ("\nDmTimeResponse : GetSystem time failed");
		return  retval;
	}
}

/*
 * Function to get the time difference
 */
unsigned long long getTimeDiff(){
	return timeDiff;
}

/*
int get_CDMACellInfo(CdmaKpi *cdmaKpi ){
   int retval = -1;
#ifdef CDMA_ENABLE
	 *cdmaKpi = mCdmaKpi;
	 retval = 0;
#endif
	 return retval;
}
*/

int get_CDMA_Info(Kpi_CDMA *kpi_cdma){
	pthread_mutex_lock(&cellInfoMutex);
	 int retval = -1;
	 int i;
#ifdef CDMA_ENABLE
//	get_CDMACellInfo(&mCdmaKpi);
	 mkpi_cdma.ActivePilotPN = mCdmaKpi.ActivePilotPN;
	 mkpi_cdma.Frame_err_rate = mCdmaKpi.Frame_err_rate;
	 mkpi_cdma.RxPwr_0 = mCdmaKpi.RxPwr_0;
	 mkpi_cdma.RxPwr_1 = mCdmaKpi.RxPwr_1;
	 mkpi_cdma.SNR = mCdmaKpi.SNR;
	 mkpi_cdma.TxADJ = mCdmaKpi.TxADJ;
	 mkpi_cdma.TxTotalPwr = mCdmaKpi.TxTotalPwr;
	 mkpi_cdma.maxPilot = mCdmaKpi.maxPilot;
	 for (i=0 ; i<mCdmaKpi.maxPilot ;i++ )
		 {
		 	 mkpi_cdma.pilotInfo[i].C_I = mCdmaKpi.pilotInfo[i].C_I;
		 	 mkpi_cdma.pilotInfo[i].EcIO = mCdmaKpi.pilotInfo[i].EcIO;
		 	 mkpi_cdma.pilotInfo[i].pilotPN = mCdmaKpi.pilotInfo[i].pilotPN;
		 }
	 *kpi_cdma = mkpi_cdma;
	retval = 0;
#endif
	pthread_mutex_unlock(&cellInfoMutex);
		 return retval;
}


void decode_msg_response (unsigned char *buf ,int length)
{
  DMCmd_sub_sys_header *cmd = (DMCmd_sub_sys_header *) &buf[0];
  switch(cmd->subsys_cmd)
  {
  	  case 0x4002:
  	  {
  		  d_log("\nRSSI_Message Response :- %x",cmd->subsys_cmd);
  		  rssi_req_message * rssicmd = (rssi_req_message *)&buf[0];
  		  d_log("\nDIAG_SUBSYS_CMD_F : %d",rssicmd->cmd_code);
  		  d_log("\nDIAG_SUBSYS_LTE: %d",rssicmd->subsys_id);
  		  d_log("\nSUBSYS_CMD_CODE : %d",rssicmd->subsys_cmd_code);
  		  d_log("\nValidation: %d",rssicmd->valid);
  		  d_log("\nE-UTRA Operating Band Value: %d",rssicmd->band);
  		  d_log("\nE-UTRA EARFCN Value : %d",rssicmd->earfcn);
  		  d_log("\nPhysical cell ID: %d",rssicmd->physical_cell_id);
  		  d_log("\nReceived Signal Strength indication: %d",rssicmd->rssi);
  		  d_log("\nReference Signal Receive Power: %d",rssicmd->rsrp);
  		  d_log("\nReference signal receive power: %d",rssicmd->rsrq);
  		  d_log("\nHighest Tx Power for all channels: %d",rssicmd->tx_power);
  	  }
	  break;
  	  default:
  		  d_log("\nUnknow packet");
        break;
  }
}
void decode_event(unsigned int event ,char *eventData , u_int8_t lengthData ,char *time , u_int8_t lengthTime)
{    
   switch (event)
   {
		case 447 :
			d_log ("\n----WCDMA InterRat Handover Procedure Started-----");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			sendInformation (WCDMA,EVENT_DATA,EVENT_InterRat_Handover_Start,eventData,lengthData);
			break;
		case 448 :
			d_log ("\n-----WCDMA InterRat Handover Procedure End-----------");
			d_log("\nEventId = %d",event);
			d_log("\nData:-");
			d_arraylog(eventData , lengthData);
			sendInformation (WCDMA,EVENT_DATA,EVENT_InterRat_Handover_End,eventData,lengthData);
			break;
		case 570 :
			d_log ("\n------WCDMA Call Dropped Event Receive---------");
			d_log("\nEventId = %d",event);
	  	  	d_log("\nData:- ");
	  	 	d_arraylog(eventData , lengthData);
	  	 	sendInformation (WCDMA,EVENT_DATA,EVENT_CallDrop,eventData,lengthData);
	  	 	break;
		case 571 :
			d_log ("\n--------WCDMA RRC State Change Event---------");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			sendInformation (WCDMA,EVENT_DATA,EVENT_RRC_STATE,eventData,lengthData);
			break;
		case 619 :
			d_log ("\n------WCDMA Out of Service------------");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			//sendInformation (WCDMA,EVENT_DATA,EVENT_InterRat_Handover_Start,buf,length);
			break;
		case 627 :
			d_log ("\n--------WCDMA RRC RB Setup Failure-------");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			sendInformation (WCDMA,EVENT_DATA,EVENT_RB_Setup_Failure,eventData,lengthData);
			break;
		case 628 :
			d_log ("\n-----WCDMA RRC Physical Channel Establishment failure----");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			sendInformation (WCDMA,EVENT_DATA,EVENT_RRC_Phy_Channel_Failure,eventData,lengthData);
			break;
		case 1022 :
			d_log ("\n----MM State Change-----" );
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			sendInformation (WCDMA,EVENT_DATA,EVENT_MM_STATE,eventData,lengthData);
			break;
		case 1023 :
			d_log ("\n-----GMM State Change------");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			sendInformation (WCDMA,EVENT_DATA,EVENT_GMM_STATE,eventData,lengthData);
			break;
		case 1105 :
	 	 	d_log ("\n------WCDMA Rat Change-----");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			sendInformation (WCDMA,EVENT_DATA,EVENT_RRC_WCDMA_Rat_Change,eventData,lengthData);
			break;
		case 1541 :
			d_log ( "\n------WCDMA RRC TIMER Expiry-----");
			d_log("\nEventId = %d",event);
	 		d_log("\nData:- ");
	 		d_arraylog(eventData , lengthData);
	 		sendInformation (WCDMA,EVENT_DATA,EVENT_RRC_TIMER_EXPIRY,eventData,lengthData);
	 		break;
		case 1548 :
			d_log ("\n----WCDMA CELL SELECTION FAILURE--------");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
		  	sendInformation (WCDMA,EVENT_DATA,EVENT_CELL_SELECTION_FAILURE,eventData,lengthData);
		 	 break;
		case 1606:
			d_log ("\n---LTE RRC State Change Event Receive---");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			sendInformation (LTE,EVENT_DATA,EVENT_LTE_RRC_STATE_CHANGE,eventData,lengthData);
			break;
		case 1609 :
			d_log ("\n-----LTE RRC Downlink Message-----");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			break;
		case 1610 :
			d_log ("\n------LTE RRC Uplink Message----");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			break;
		case 1613 :
			d_log ("\n------LTE Handover failure ----");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			sendInformation (LTE,EVENT_DATA,EVENT_LTE_HANDOVER_FAILURE,eventData,lengthData);
			break;
		case 1615 :
			d_log ("\n------LTE Mobility from E-UTRA started----");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			sendInformation (LTE,EVENT_DATA,EVENT_LTE_MOBILITY_E_UTRA_STARTED,eventData,lengthData);
			break;
		case 1616 :
			d_log ("\n----- LTE Mobility from E-UTRA failed---------");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			sendInformation (LTE,EVENT_DATA,EVENT_LTE_MOBILITY_E_UTRA_FAILED,eventData,lengthData);
			break;
		case 1617 :
			d_log ("\n------Inter-RAT reselection from E-UTRAN start-------");
			d_log("\nEventId = %d",event);
			inter_rat_reselection_from_e_utran_start *irrfeus =(inter_rat_reselection_from_e_utran_start *) &eventData[0];
			if(irrfeus->rat == 4)
			  d_log("\nLTE Inter-RAT reselection from E-UTRAN ");
			d_log("\nFreq-Band indicator  = %d",irrfeus->freq_band_indicator);
			d_log("\nFrequency  = %d",irrfeus->frequency );
			d_log("\nCell ID   = %d",irrfeus->cell_id);
			sendInformation (LTE,EVENT_DATA,EVENT_LTE_INTERAT_RESEL_START,eventData,lengthData);
			break;
		case 1618 :
			d_log ("\n------Inter-RAT reselection from E-UTRAN end------------");
			d_log("\nEventId = %d",event);
			inter_rat_reselection_from_e_utran_end *irrfeue =(inter_rat_reselection_from_e_utran_end *) &eventData[0];
			if(irrfeue->rat == 4)
				d_log("\nLTE Inter-RAT reselection from E-UTRAN ");
			sendInformation (LTE,EVENT_DATA,EVENT_LTE_INTERAT_RESEL_END,eventData,lengthData);
			break;
	  case 1807 :
		   d_log ("\n------Inter-RAT redirection from E-UTRAN start------------");
		   d_log("\nEventId = %d",event);
		   inter_rat_redirection_from_e_utran_start *irredfeus =(inter_rat_redirection_from_e_utran_start*)  &eventData[0];
		   if(irredfeus->rat == 4)
			  d_log("\nLTE Inter-RAT redirection from E-UTRAN start");
		   sendInformation (LTE,EVENT_DATA,EVENT_LTE_INTERAT_REDIR_START,eventData,lengthData);
		   break;
	  case 1808 :
	  {
		  int status;
		  d_log ("\n------Inter-RAT redirection from E-UTRAN end ------------");
		  d_log("\nEventId = %d",event);
		  inter_rat_redirection_from_e_utran_end *irredfeue =(inter_rat_redirection_from_e_utran_end *) &eventData[0];
		  status = irredfeue->status_casue;
		  switch(status)
		  {
		  	  case  0x0 :
		  		  d_log("Status-cause :- Success");
		  		  break;
		  	  case  0x01 :
		  		  d_log("Status-cause :- Failed");
		  		  break;
		  	  case  0x02 :
		  		  d_log("Status-cause :-Aborted-Service Request ");
		  		  break;
		  	  case  0x03 :
		  		  d_log("Status-cause :-Aborted-Manual PLMN Search Request  ");
		  		  break;
		  	  case  0x04 :
		  		  d_log("Status-cause :-Aborted-Deactivate  ");
		  		  break;
		  	  default:
		  		  d_log(" Unknown Status cause");
		  }
		  sendInformation (LTE,EVENT_DATA,EVENT_LTE_INTERAT_REDIR_END,eventData,lengthData);
		}
		break;
	  case 1611 :
	  {
		  d_log ("\n-----EVENT_LTE_NEW_CELL_SELECTION-----------");
		  d_log("\nEventId = %d",event);
		  event_lte_new_cell_selection *elncs = (event_lte_new_cell_selection *)&eventData[0];
		  d_log("\nCause = %d",elncs->cause);
		  d_log("\nFreq = %d",elncs->freq);
		  d_log("\nCell Id = %d",elncs->cell_id);
		  sendInformation (LTE,EVENT_DATA,EVENT_LTE_NEW_CELL_SELECTION,eventData,lengthData);
	  }
	  break;
	  case 1607:
	  {
		  d_log ("\n-----EVENT_LTE_OUT_OF_SERVICE-----------");
		  event_lte_out_of_service *eloos = (event_lte_out_of_service*)&eventData[0];
		  d_log("\nCounter = %d",eloos->counter);
		  sendInformation (LTE,EVENT_DATA,EVENT_LTE_OUT_OF_SERVICE,eventData,lengthData);

	  }
	  break;
	  case 1608:
	  {
		  d_log ("\n-----EVENT_LTE_RADIO_LINK_FAILURE---------");
		  event_lte_radio_link_failure *elrlf = (event_lte_radio_link_failure *)&eventData[0];
		  d_log("\nCounter = %d",elrlf->counter);
		  sendInformation (LTE,EVENT_DATA,EVENT_LTE_RADIO_LINK_FAILURE,eventData,lengthData);

	  }
	  break;
	  case 1995:
	  {
		  d_log ("\n-----EVENT_LTE_RADIO_LINK_FAILURE_STAT---------");
		  event_lte_radio_link_failure_stat *elrlrs = (event_lte_radio_link_failure_stat *)&eventData[0];
		  d_log("\nRLF Count since RRC connected = %d",elrlrs->rlf_count_since_rrc_connected);
		  d_log("\nRLF Count since LTE active = %d",elrlrs->rlf_count_since_lte_active);
		  d_log("\nRFL cause = %d",elrlrs->rlf_cause);
		  sendInformation (LTE,EVENT_DATA,EVENT_LTE_RADIO_LINK_FAILURE_STAT,eventData,lengthData);

	  }
	  break;
		default :
			d_log ("\nUnknown Event rReceived");
			d_log("\nEventId = %d",event);
			d_log("\nData:- ");
			d_arraylog(eventData , lengthData);
			break;
   }
}

void decode_fs_information (char *buf ,unsigned int length)
{
	DMCmd_sub_sys_header *cmd = (DMCmd_sub_sys_header *) &buf[0];
	switch(cmd->subsys_cmd)
	{
		case 38:
		{
			d_log("\n Put Response Received :- %x",cmd->subsys_cmd);
			DMCmd_put_responseEfs * putResp = (DMCmd_put_responseEfs *)&buf[0];
			d_log("\n CMD_CODE: %d",putResp->code);
			/*d_log("\n CMD_CODE: %d",putResp->code);*/
			d_log("\n SUBSYS_ID : %d",putResp->subsys_id);
			/*d_log("\n SUBSYS_ID : %d",putResp->subsys_id);*/
			d_log("\n SUBSYS_CMD_CODE : %d",putResp->subsys_cmd_code);
			/*d_log("\n SUBSYS_CMD_CODE : %d",putResp->subsys_cmd_code);*/
			d_log("\n  SEQUENCE_NO : %d",putResp->sequence_num);
			d_log("\n ERROR_NO : %d",putResp->err_num);
			/*d_log("\n ERROR_NO : %d",putResp->err_num);*/
			d_log("\n---------------------------------------------------------------\n");
		}
	   break;
		case 39 :
		{

			d_log("\n Get Response Received :- %x",cmd->subsys_cmd);
			DMCmd_get_responseEfs * getResp = (DMCmd_get_responseEfs*)&buf[0];
			d_log("\n CMD_CODE: %d",getResp->code);
			d_log("\n SUBSYS_ID : %d",getResp->subsys_id);
			d_log("\n SUBSYS_CMD_CODE : %d",getResp->subsys_cmd_code);
			d_log("\n NUMBER_BYTES : %d",getResp->num_bytes);
			d_log("\n ERROR_NO : %d",getResp->err_num);
			d_log("\n ERROR_NO : %d",getResp->err_num);
			d_log("\n SEQUENCE_NO: %d",getResp->sequence_num);
			d_log("\n DATA: %s ", getResp->data);
			if (!getResp->err_num ){
			 if(getResp->sequence_num == WCDMA_FREQ_SEQ_NUM)
				 globalUrfcnFreq = *(unsigned short*) &(getResp->data[0]);
			 if(getResp->sequence_num == LTE_BAND_SEQ_NUM)
				 gbandlockFreq = *(u_int64_t*)&(getResp->data[0]);
			 }
			d_log("\n---------------------------------------------------------------\n");
		}
		break;
	   case  18:
		{
			d_log("\nChange Mode Command Response Received :- %x",cmd->subsys_cmd);
			// DMCmdChngModeResp* getResp = (DMCmdChngModeResp*)&buf[0];
			// d_log("\nERROR_NO : %d",getResp->err_num);
		}
		break;
		default:
			d_log("\nUnknow packet");
			break;
   }
}
