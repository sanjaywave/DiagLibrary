#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <endian.h>
#include <sys/types.h>
#include "gsmdecode.h"
#include "interfaceapi.h"
#include "extmsgid.h"
#include "errors.h"
#include "filewriter.h"
#ifndef HOST
#include "csnRRDecoder.h"
#endif

extern bool getCCEventEnableStatus();
extern GSMCellInfo mGSMCellInfo;
static time_t Gsmtime = 0;

/*MMC digit are always between 0-9 */
void calMcc(u_int8_t *ptr){
  char tempMCC[4]={'\0'};
  sprintf(tempMCC,"%d",(ptr[0] & 0x0f));
  sprintf(tempMCC+1,"%d",((ptr[0] & 0xf0) >> 4));
  sprintf(tempMCC+2,"%d",(ptr[1] & 0x0f));
  strncpy(mGSMCellInfo.MCC,tempMCC,4);
}

/*MNC digit are always between 0-9 */
void calMnc(u_int8_t *ptr){
	 char tempMNC[4]={'\0'};
	 if((ptr[1] & 0xf0) == 0xf0)
	 {
		 sprintf(tempMNC,"0");
	 }else
	  sprintf(tempMNC,"%d",((ptr[1] & 0xf0) >> 4));
	  sprintf(tempMNC+1,"%d",(ptr[2] & 0x0f));
	  sprintf(tempMNC+2,"%d",((ptr[2] & 0xf0) >> 4));

	 strncpy(mGSMCellInfo.MNC,tempMNC,4);
}

/*LAI 16 bit HEX format */

void calLac(u_int8_t *ptr)
{
	u_int16_t tempbuff=0x0;

	tempbuff=ptr[5];

	tempbuff=tempbuff << 8 | ptr[4];

	mGSMCellInfo.LAC=tempbuff;

}
  void  calCgi()
  {
  	char cellGlobalID[30]={'\0'};
  	char tempString[10]= {'\0'};

  	strncat(cellGlobalID,mGSMCellInfo.MCC,3);
  	strcat(cellGlobalID,"-");
  	strncat(cellGlobalID,mGSMCellInfo.MNC,3);
  	strcat(cellGlobalID,"-");
  	sprintf(tempString,"%d",mGSMCellInfo.LAC);
  	strcat(cellGlobalID,tempString);
  	strcat(cellGlobalID,"-");

  	sprintf(tempString,"%d",mGSMCellInfo.cell_id);
  	strcat(cellGlobalID,tempString);
  	strcpy(mGSMCellInfo.CGI,cellGlobalID );

}

void  gms_decode_log_msg(int code, unsigned char *buf ,int length, u_int8_t timestamp[8]){
#ifdef DEBUG
	time (&Gsmtime);/* This will get system time */
#endif
  switch(code){
	  case GSM_RR_SIGNALING_MESSAGE:
	  {
		  if(getCCEventEnableStatus()) { 
			/* send cc events only if they are enabled */
			  u_int64_t cmdmillSeconds = 0;
			  cmdTimestamp *cts = (cmdTimestamp *)&timestamp[0];
			  cmdmillSeconds = ((unsigned long long)cts->counter*125)/100;

		#ifndef HOST
				//added by me to test compilation
		#ifndef MINIMUM_DM
			  CSNDecoderCCevents((void*)buf, length, cmdmillSeconds);
		#endif
		#endif	
		  }
	  }
	  break;
  	  case GSM_RR_CELLINFORM :
	  {
		  d_log("Command code :-  %d",code);
		  d_log("\nTime  :- %s", ctime(&Gsmtime));
		  /* Check for incoming buffer length */
		  VERIFY_LENGTH(gsm_rr_cellInform ,length,0);
		  /*Update Time Stamp*/
		  memcpy(mGSMCellInfo.timestamp,buf-8 ,8);
		  gsm_rr_cellInform *grci=(gsm_rr_cellInform *)&buf[0];
		  mGSMCellInfo.bcchARFCN =((grci->bcchArfcn & 0xf000) >> 12);
		  mGSMCellInfo.BSIC[1]=grci->bsic_bs_color;
		  mGSMCellInfo.BSIC[0]=grci->bsic_plmn_color;
		  mGSMCellInfo.cell_id = grci->cellIdentity;
		  mGSMCellInfo.Cell_Selection_Priority=grci->cellSelectionPriority;
		  mGSMCellInfo.NCCPermit=grci->nccPermit;
		  memcpy(mGSMCellInfo.LAI,grci->lai,5);
		  /*Calculation of MCC*/
		  calMcc(grci->lai);
		  /*Calculation of MNC*/
		  calMnc(grci->lai);
		  /*Calculation of LAI*/
		  calLac(grci->lai);
		  /*Calculation of CGI*/
		  calCgi();
		  break;
	  }
	  case GRR_CELL_RESELECTION_MEASURMENT :
	  {
		  u_int32_t count ,index = 0;
		  d_log("\nCommand code :-  %d",code);
		  d_log("\nTime  :- %s", ctime(&Gsmtime));
		  d_arraylog(buf,length);
		  /* Check for incoming buffer length */
		  VERIFY_LENGTH(grr_cell_reselection_measurment ,length,index);
		  /*Update Time Stamp*/
		  memcpy(mGSMCellInfo.timestamp,buf-8 ,8);
		  grr_cell_reselection_measurment *gcrm = (grr_cell_reselection_measurment *)&buf[index];
		  mGSMCellInfo.rxlev_arg=( -110 + gcrm->servingRxlev);
		  d_log("servingC1 : %d",gcrm->servingC1);
		  mGSMCellInfo.c1=gcrm->servingC1;
		  d_log("servingC2 : %d",gcrm->servingC2);
		  mGSMCellInfo.c2=gcrm->servingC2;
		  d_log("servingTimer : %d",gcrm->servingFive_second_timer_state);
		  d_log("cellReSelectStatus : %d",gcrm->cellReSelectStatus);
		  d_log("neigbCellCount : %d",gcrm->neigbCellCount);
		  mGSMCellInfo.no_of_neigbourcell=gcrm->neigbCellCount;
		  index += sizeof(*gcrm);
		  for(count = 0; count< gcrm->neigbCellCount ;count++){
			  /* Check for incoming buffer length */
			  VERIFY_LENGTH(gsm_neigb_info ,length,index);
			  gsm_neigb_info *gni = (gsm_neigb_info*)&buf[index];
			  mGSMCellInfo.neigCellInfo[count].bccnNeigARFCN = gni->BcchArfcn;
			  mGSMCellInfo.neigCellInfo[count].rxlev_arg = (-110 + gni->servingRxlev);
			  mGSMCellInfo.neigCellInfo[count].c1 = gni->C1;
			  mGSMCellInfo.neigCellInfo[count].c2 = gni->C2;
			  mGSMCellInfo.neigCellInfo[count].serving_LAI = gni->serving_lai;
			  index += sizeof(*gni);
		  }
		  break;
	  }
  	 case GSM_RR_SACCH_REPORT:
		{
			u_int32_t index = 0,loopCount,count = 0;
			d_log("Command msgId :-  %d",code);
			/* Check for incoming buffer length */
			VERIFY_LENGTH(gsm_rr_sacch_report ,length,index);
			/*Update Time Stamp*/
			memcpy(mGSMCellInfo.timestamp,buf-8 ,8);
			gsm_rr_sacch_report *grsr = (gsm_rr_sacch_report *)&buf[index];
			/*RxLev_Full*/
			mGSMCellInfo.Rxlev_Full_Avg= (-110 + grsr->rxlev_full_avg);
			/* RxLev_Sub*/
			mGSMCellInfo.Rxlev_Sub_Avg = (-110 + grsr->rxlev_sub_avg);
			/*RxQual_Full*/
			mGSMCellInfo.Rxqual_Full_Avg = (grsr->rxqual_full_avg);
			/*RxQual_Sub*/
			mGSMCellInfo.Rxqual_Sub_Avg =(grsr->rxqual_sub_avg);
			index +=sizeof( *grsr);
			/* Check for incoming buffer length */
			VERIFY_LENGTH(gsm_rr_sacch_report_ncell_measurements,length,index);
			gsm_rr_sacch_report_ncell_measurements *grsrnm = (gsm_rr_sacch_report_ncell_measurements*)&buf[index];
			d_log("NCELL_COUNT:-%x",grsrnm->ncell_count);
			count = grsrnm->ncell_count;
			index += sizeof(*grsrnm);
			if(count > 6 && length <= 32 ){
				d_warning(" Neighbour Cell information is incorrect received \n ");
				return ;
			}
			if (count > 6) count = 6;
			for(loopCount = 0 ; loopCount < count ; loopCount++){
				/* Check for incoming buffer length */
				VERIFY_LENGTH(gsm_rr_sacch_report_ncell_measurements_measurement,length,index);
				gsm_rr_sacch_report_ncell_measurements_measurement *grsrnmm = (gsm_rr_sacch_report_ncell_measurements_measurement *)&buf[index];
				index +=sizeof(*grsrnmm);
			}
		}
		break;
  	 default:
  		  d_log("Unknown packet %x \n",code);
  		  break;
  	  }
}
