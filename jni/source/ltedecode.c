#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <endian.h>
#include <sys/types.h>
#include <pthread.h>
#include "ltedecode.h"
#include "interfaceapi.h"
#include "extmsgid.h"
#include "errors.h"
#include "lte_parameters.h"



#define THROUGHPUT_CALCULATION_TIME 2000
extern LTECellInfo mLTE_CellInfo;
LTEThroughput mLTEThroughput;
//static localParameter mlocalParameter;
static PDUInfo  RlcDLThroughtput;
static PDUInfo RlcULThroughtput;
extern pthread_mutex_t cellInfoMutex;

static time_t ltetime;


enum
{
	MAC_CFG_TYPE_SUB_PACKET = 0x00,
	MAC_DL_CFG_SUB_PACKET = 0x01,
	MAC_UL_CFG_SUB_PACKET = 0x02,
	MAC_RACH_CFG_SUB_PACKET = 0x03,
	MAC_LC_INFO_SUB_PACKET = 0x04,
	MAC_RACH_REASON_SUB_PACKET = 0x05,
	MAC_RACH_ATTEMPT_SUB_PACKET = 0x06,
	MAC_DL_TB_SUB_PACKET = 0x07,
	MAC_UL_TB_SUB_PACKET = 0x08,
	MAC_QOS_INTERNAL_SUB_PACKET = 0x09,
	MAC_BUF_STATUS_INTERNAL_SUB_PACKET = 0x0A,
	MAC_UL_Tx_STATS_SUB_PACKET = 0x0B,
	MAC_EMBMS_STATS_sub_packet = 0x0C,
	MAC_EMBMS_CFG_SUB_PACKET=0x0D,
	RLC_DL_CONFIGURATION_SUB_PACKET = 0x40,
	RLC_DL_PDU_SUB_PACKET = 0x41,
	RLC_DL_STATISTICS_SUB_PACKET = 0x42,
	RLC_UL_CONFIGURATION_SUB_PACKET = 0x45,
	RLC_UL_PDU_SUB_PACKET = 0x46,
	RLC_UL_STATISTICS_SUB_PACKET = 0x47,
	UNIFIED_RLC_DL_PDU_SUB_PACKET = 0x50,
	RLC_EMBMS_STATS_SUB_PACKET = 0x51,
	RLC_EMBMS_CONFIG_SUB_PACKET = 0x52,
	PDCP_DL_STATISTICS_SUBPKT  = 0xC4,
	PDCP_UL_STATISTICS_SUBPKT = 0xC5,
};


struct timestamp {
	u_int64_t prec:6;
	u_int64_t unused:2;
	u_int64_t CFM:8;
	u_int64_t time:48;
}__attribute__ ((packed));


/*
 * Function to calculate timediff in miliseconds
 * Return time in miliseconds
 */

u_int64_t timediff( void *startTime, void *endTime)
{
	u_int64_t timeDuration;
	struct timestamp *st, *et, start, end;
	st = (struct timestamp *)startTime;

	start = *st;

	et = (struct timestamp *) endTime;
	end = *et;

	timeDuration = (unsigned long long)end.time - (unsigned long long)start.time;
	timeDuration = timeDuration*125;

	timeDuration += ((end.prec - start.prec)*125)/40;
	timeDuration = timeDuration/100;

	return timeDuration;
}

/* Function to calculate throughput (PDCP UL/DL, RLC UL/DL )
* arguments lastrecode structure, current time stamp,total current PDU sum
* return calculated throughput
*/

void  calculateThroughput(PDUInfo *mpudinfo,char *currentTimeStamp, u_int64_t curPDUtotals , float *Kpi)
{
	float calculatedthroughput = 0.0;
	u_int64_t difftimeBetweenLogs = 0;
	if(mpudinfo->totalpdu <= curPDUtotals){
		if(!mpudinfo->isFirstCount){
			/* This is first count, only insert data to structure */
			mpudinfo->isFirstCount = 1;
		}else {
			/* Get the time difference */
			difftimeBetweenLogs  = timediff(mpudinfo->timestamp , currentTimeStamp);
			if(difftimeBetweenLogs >= THROUGHPUT_CALCULATION_TIME){
				/* if time diff is equal or greater than equal 2 sec  then calculate throughput */
				calculatedthroughput = (((curPDUtotals - mpudinfo->totalpdu)*8.00))/(difftimeBetweenLogs);
				*Kpi = calculatedthroughput;
			}else return; /* No need to do any calculation */
		}
	}
	mpudinfo->totalpdu = curPDUtotals;
	memcpy(mpudinfo->timestamp,currentTimeStamp,sizeof(mpudinfo->timestamp));
}




void  lte_decode_log_msg(int code, unsigned char *buf ,int length, u_int8_t timestamp[8]){
#ifdef DEBUG
	time (&ltetime);/* This will get system time */
#endif

switch(code){
  	  	  case LTE_OTA_MESSAGE:
		  {
			  d_log("\nLTE OTA Message  :- %d",code);
			  d_log("\nTime  :- %s", ctime(&ltetime));
			  /* Check for incoming buffer length */
			  VERIFY_LENGTH(lte_ota_messages ,length,0);
			  lte_ota_messages * otacmd =(lte_ota_messages *)&buf[0];
			  VALIDATE_VERSION("LTE_OTA_MESSAGE",2,otacmd->version);
			  d_log("Version number: %d",otacmd->version);
			  d_log("\nRRC release: %d",otacmd->rrc_rel);
			  d_log("\nRRC version: %d",otacmd->rrc_ver);
			  d_log("\nRadio bearer ID: %d",otacmd->rb_id);
			  d_log("\nPhysical cell ID: %d",otacmd->physical_cell_id);
			  d_log("\nE-ARFCN frequency: %d",otacmd->freq);
			  d_log("\nSFN-SubFrameNumber: %d",otacmd->sfn);
			  d_log("\nPDU-ProtocolDataUnit :-");
			  switch(otacmd->pdu_num){
			  	  case 0x2: d_log("BCCH_DL_SCH Message");break;
			  	  case 0x3: d_log("MCCH Message "); break;
			  	  case 0x4: d_log("PCCH Message "); break;
			  	  case 0x5: d_log("DL_CCCH Message "); break;
			  	  case 0x6: d_log("DL_DCCH Message "); break;
			  	  case 0x7: d_log("UL_CCCH Message "); break;
			  	  case 0x8: d_log("UL_DCCH Message"); break;
			  	  case 0x9: d_log("UE EUTRA Capability Message"); break;
			  	  case 0xA: d_log("VarShortMAC_Input "); break;
			  	  case 0xB: d_log("UE EUTRA Capability v9a0_IEs  "); break;
			  	  case 0xC: d_log("System Information Block Type1 Message"); break;
			  	  default : d_log("Unknown message type");
			  }
			  d_log("\nEncoded message length: %d",otacmd->encoded_msg_len);
			  d_log("\nASN Encoded Message Start ");
			  d_log("\n");
			  if(otacmd->encoded_msg_len < 6156){
			  d_arraylog(otacmd->encoded_data,otacmd->encoded_msg_len);
		  }
		  else {
			  d_log("Length is greater, LENGTH = %d",otacmd->encoded_msg_len);
		  }
	  }
	  break;
	  case LTE_MIB_MESSAGE:
		  {
			  d_log("\n LTE MIB message :- %d",code);
			  d_log("\nTime  :- %s", ctime(&ltetime));
			  /* Check for incoming buffer length */
			  VERIFY_LENGTH(lte_mib_messages ,length,0);
			  lte_mib_messages * mibcmd =(lte_mib_messages *)&buf[0];
			  VALIDATE_VERSION("LTE_MIB_MESSAGE",1,mibcmd->version);
			  /*Update Time Stamp*/
			  memcpy(mLTE_CellInfo.timestamp,buf-8 ,8);
			  mLTE_CellInfo.num_tx_antennas= mibcmd->num_tx_antennas;
			  u_int8_t bandwith=mibcmd->dl_bandwidth;
			  d_log("\nBandWidth : %d",bandwith);
			  d_log("\n");
			  switch(bandwith){
			  	  case   0x6 : d_log("Downlink bandwidth:-1.4 MHz ");
			  		  break;
			  	  case   0xf : d_log("Downlink bandwidth:-3 MHz ");
			  		  break;
			  	  case   0x19: d_log("Downlink bandwidth:-5 MHz ");
			  		  break;
			  	  case  0x32:  d_log("Downlink bandwidth:-10 MHz ");
			  		  break;
			  	  case  0x4b : d_log("Downlink bandwidth:-15 MHz ");
			  		  break;
			  	  case  0x64 : d_log("Downlink bandwidth:-20 MHz ");
			  		  break;
			  	  default: d_log(" Unknown bandwidth found");
			  }
	  }
	  break;

	  case LTE_SERVING_CELL_INFO :
	  {
		  d_log("\n LTE serving cell info  :- %d ",code);
		  d_log("\nTime  :- %s", ctime(&ltetime));
		  d_arraylog(buf , length);
		  d_log("\nTime  :- %s", ctime(&time));
		  /* Check for incoming buffer length */
		  VERIFY_LENGTH(lte_serving_cell_info ,length,0);
		  lte_serving_cell_info * cellinfocmd =(lte_serving_cell_info *)&buf[0];
		  VALIDATE_VERSION("LTE_SERVING_CELL_INFO",2,cellinfocmd->version);
		  /*Update Time Stamp*/
		  memcpy(mLTE_CellInfo.timestamp,buf-8 ,8);
		  d_log("Log Packet version number: %d",cellinfocmd->version);
		  d_log("\nphy_cell_id  : %d",cellinfocmd->physical_cell_id);
		  if (mLTE_CellInfo.physical_cell_id != cellinfocmd->physical_cell_id)
		  memset(&mLTE_CellInfo,0,sizeof(mLTE_CellInfo));
		  mLTE_CellInfo.physical_cell_id=cellinfocmd->physical_cell_id;
		  d_log("\nDownlink frequency as E-ARFCN: %d",cellinfocmd->dl_freq);
		  mLTE_CellInfo.dl_freq =cellinfocmd->dl_freq;
		  d_log("\nUplink frequency as E-ARFCN: %d",cellinfocmd->ul_freq);
		  mLTE_CellInfo.ul_freq =cellinfocmd->ul_freq;
		  d_log("\nDownlink bandwidth: %d",cellinfocmd->dl_bandwidth);
		  if(cellinfocmd->dl_bandwidth ==6)
			  mLTE_CellInfo.dl_bandwidth=1.4;
		  else
			  mLTE_CellInfo.dl_bandwidth=cellinfocmd->dl_bandwidth/5;
		  d_log("\nUplink bandwidth: %d",cellinfocmd->ul_bandwidth);

		  if(cellinfocmd->ul_bandwidth ==6)
				mLTE_CellInfo.ul_bandwidth=1.4;
		  else
				mLTE_CellInfo.ul_bandwidth = cellinfocmd->ul_bandwidth/5;
		  d_log("\nCell Identity : %d",cellinfocmd->cell_identity);
		  mLTE_CellInfo.cell_identity=cellinfocmd->cell_identity;
		  d_log("\nTracking area code: %d",cellinfocmd->tracking_area_code);
		  mLTE_CellInfo.tracking_area_code=cellinfocmd->tracking_area_code;
		  d_log("\nFrequency band indicator: %d",cellinfocmd->frequency_band_indicator);
		  mLTE_CellInfo.frequency_band_indicator=cellinfocmd->frequency_band_indicator;
		  d_log("\nMCC(PLMN): %d",cellinfocmd->mcc);
		  mLTE_CellInfo.mcc=cellinfocmd->mcc;
		  d_log("\nNumber of digits in the MNC: %d",cellinfocmd->num_of_mnc_digits);
		  mLTE_CellInfo.num_of_mnc_digits=cellinfocmd->num_of_mnc_digits;
		  d_log("\nMNC(PLMN): %d",cellinfocmd->mnc);
		  mLTE_CellInfo.mnc=cellinfocmd->mnc;
		  d_log("\nAllowed access on the cell : %d",cellinfocmd->allowed_access);
	  }
	  break;

	  case LTE_ML1_SERVING_CELL_MEASUREMENT :
	 	  {
	 		  u_int32_t index = 4,subpacket_count = 0;
	 		  d_log("\nLTE ML1 serving cell measurement Command :- %d",code);
	 		  d_log("\nTime  :- %s", ctime(&ltetime));
	 		  lte_ml1_serving_cell_measurement *lmscm = (lte_ml1_serving_cell_measurement*)&buf[0];
	 		  VALIDATE_VERSION("LTE_ML1_SERVING_CELL_MEASUREMENT",1,lmscm->version);
	 		  /*Update Time Stamp*/
	 		  memcpy(mLTE_CellInfo.timestamp,buf-8 ,8);
	 		  d_log("Version  :- %d", lmscm->version);
	 		  subpacket_count = lmscm->no_sub_packets;
	 		  while(subpacket_count != 0){
	 			  lte_ml1_serving_cell_measurement_log_subpacket *lmscmlspkt = (lte_ml1_serving_cell_measurement_log_subpacket*)&buf[index];
	 			  index = index +4;
	 			  // check the condition for  a subpacket
	 			  if(lmscmlspkt->sub_packet_version != 0){
	 				  if((lmscmlspkt->sub_packet_id == 0x8) &&( lmscmlspkt->sub_packet_version == 2)){
	 					  lte_ml1_serving_cell_measurement_log_1st_subpacket_data  *lmscml1sd =  (lte_ml1_serving_cell_measurement_log_1st_subpacket_data *)&buf[index];
	 					  //d_log("\nSrxlev :- %d",lmscml1sd->srxlev);
	 					  //mLTE_CellInfo.srxlev = -30+ lmscml1sd->srxlev;
	 					  d_log("\nInst Measured RSRP  :- %d",lmscml1sd->inst_measured_rsrp);
	 					  d_log("\nInst Measured RSSI :- %d",lmscml1sd->inst_measured_rssi);
	 					  d_log("\nDisplay_avg  :- %d",lmscml1sd->display_avg);
	 					  /*check for Standards version= REL 9*/
	 					  if (lmscml1sd->Standards_version==0x01){
	 						  lte_ml1_serving_cell_measurement_log_subpacket_REL9_extra *lmscmlsRel9=(lte_ml1_serving_cell_measurement_log_subpacket_REL9_extra*)&buf[index+16];
	 						  d_log("\nSqual  :- %d",lmscmlsRel9->squal);
	 					  }
	 				  }else if(lmscmlspkt->sub_packet_id == 0x19 && lmscmlspkt->sub_packet_version == 3){
	 					  lte_ml1_serving_cell_measurement_log_second_subpacket_data *lmscmlssd =(lte_ml1_serving_cell_measurement_log_second_subpacket_data *)&buf[index];
	 					  d_log("\nearfcn  :- %d",lmscmlssd->earfcn);
	 					  d_log("\nphy_cell_id  :- %d",lmscmlssd->phy_cell_id);
	 					  if (mLTE_CellInfo.physical_cell_id != lmscmlssd->phy_cell_id)
	 					 		 memset(&mLTE_CellInfo,0,sizeof(mLTE_CellInfo));
	 					  mLTE_CellInfo.physical_cell_id  =  lmscmlssd->phy_cell_id;
	 					  d_log("\ncurrent_sfn  :- %d",lmscmlssd->current_sfn);
	 					  d_log("\ncell_timing[0] :- %d",lmscmlssd->cell_timingRx0);
	 					  mLTE_CellInfo.cell_timingRx0=lmscmlssd->cell_timingRx0;
	 					  d_log("\ncell_timing[1] :- %d",lmscmlssd->cell_timingRx1);
	 					  mLTE_CellInfo.cell_timingRx1=lmscmlssd->cell_timingRx1;
	 					  d_log("\ncell_timing_sfn[0] :- %d",lmscmlssd->cell_timing_sfn0);
	 					  d_log("\ncell_timing_sfn[1] :- %d",lmscmlssd->cell_timing_sfn1);
	 					  d_log("\ninst_rsrp_rx[0]  :- %f",-180+(lmscmlssd->inst_rsrp_rx0*0.0625));
	 					  mLTE_CellInfo.inst_rsrp_rx0=(-180 + (lmscmlssd->inst_rsrp_rx0*0.0625));
	 					  d_log("\ninst_rsrp_rx[1]  :- %f",-180+(lmscmlssd->inst_rsrp_rx1*0.0625 ));
	 					  mLTE_CellInfo.inst_rsrp_rx1=(-180 +(lmscmlssd->inst_rsrp_rx1*0.0625));
	 					  d_log("\ninst_measured_rsrp :- %f",-180+(lmscmlssd->inst_measured_rsrp*0.0625));
	 					  mLTE_CellInfo.inst_rsrp=-180+(lmscmlssd->inst_measured_rsrp*0.0625);
	 					  d_log("\ninst_rsrq   :- %f",-30+lmscmlssd->inst_rsrq*0.0625);
	 					  mLTE_CellInfo.inst_rsrq=-30 +lmscmlssd->inst_rsrq*0.0625;
	 					  d_log("\ninst_rsrq_rx[0]  :- %f",-30+lmscmlssd->inst_rsrq_rx0*0.0625);
	 					  mLTE_CellInfo.inst_rsrq_rx0=(-30 + lmscmlssd->inst_rsrq_rx0*0.0625);
	 					  d_log("\ninst_rsrq_rx[1]  :- %f",(-30+lmscmlssd->inst_rsrq_rx1*0.0625) );
	 					  mLTE_CellInfo.inst_rsrq_rx1=(-30 + lmscmlssd->inst_rsrq_rx1*0.0625);
	 					  d_log("\ninst_rssi_rx[0]  :- %f",-110 + (lmscmlssd->inst_rssi_rx0*0.0625));
	 					  mLTE_CellInfo.inst_rssi_rx0=-110 +(lmscmlssd->inst_rssi_rx0*0.0625);
	 					  d_log("\ninst_rssi_rx[1]    :- %f",-110 + (lmscmlssd->inst_rssi_rx1*0.0625));
	 					  mLTE_CellInfo.inst_rssi_rx1=(-110 + (lmscmlssd->inst_rssi_rx1*0.0625));
	 					  d_log("\ninst_rssi  :- %f",-110+(lmscmlssd->inst_rssi*0.0625));
	 					  mLTE_CellInfo.inst_rssi= (-110+(lmscmlssd->inst_rssi*0.0625));
	 					  d_log("\nres_freq_error    :- %d",lmscmlssd->res_freq_error);
	 					  d_log("\nsinr_rx[0]   :- %f",-20 +(lmscmlssd->sinr_rx0*0.1));
	 					  mLTE_CellInfo.sinr_rx0= -20+(lmscmlssd->sinr_rx0*0.1);
	 					  d_log("\nsinr_rx[1]  :- %f",-20+(lmscmlssd->sinr_rx1*0.1));
	 					  mLTE_CellInfo.sinr_rx1= -20+(lmscmlssd->sinr_rx1*0.1);
	 					  }else{
	 						  	  d_log("\nUnknown Subpacket Type");
	 					  };
	 			  }
	 			  index = index+lmscmlspkt->sub_packet_size-4;
	 			  d_log("\nIndex = %d", index);
	 			  subpacket_count--;
	 			  d_log("\nNo of SubPkt remaining= %d",subpacket_count);
	 		  }
	 	  }
	 	  break;

	case  LTE_RLC_DL_STATISTICS :/*B087*/
	case  LTE_RLC_UL_STATISTICS :/*B097*/
	{
		u_int32_t Num_of_subpkts = 0,SubIndex = 0,Index = 0;
		u_int64_t DL_NUM_RST=0, DL_NUM_PDU_BYTES = 0,DL_NUM_STATES_BYTES = 0,DL_NUM_NVALID_PDU_BYTES=0;
		u_int64_t UL_NUM_RST = 0, UL_NUM_SDU_BYTES = 0,UL_NUM_RETX_PDU_BYTES = 0,UL_NUM_POLL_BYTES = 0  \
					   ,NUM_NEW_DATA_PDU_BYTES = 0,NUM_CTRL_PDU_BYTES = 0  ;
		/* Check for incoming buffer */
		VERIFY_LENGTH( rlc_log_packet_structure ,length , Index);
		rlc_log_packet_structure *rlps =(rlc_log_packet_structure*)&buf[Index];
		VALIDATE_VERSION("LTE_RLC_DL_STATISTICS & LTE_RLC_UL_STATISTICS",1,rlps->version);
		d_log("\nNumber_of_subpkts= %d",rlps->number_subpackets);
		Num_of_subpkts=rlps->number_subpackets;
		Index  += sizeof(*rlps);
		while(Num_of_subpkts){
			SubIndex = Index;
			/* Check for incoming buffer */
			VERIFY_LENGTH(rlc_log_sub_packet_structure ,length , SubIndex);
			rlc_log_sub_packet_structure *rlsps = (rlc_log_sub_packet_structure *)&buf[SubIndex];
			d_log("\nSub_packet_id= %d",rlsps->sub_packet_id);
			d_log("\nSub_packet_version= %d",rlsps->sub_packet_version);
			d_log("\nSub_packet_size= %d",rlsps->sub_packet_size);
			SubIndex += sizeof(*rlsps);
			switch(rlsps->sub_packet_id){
				case RLC_DL_CONFIGURATION_SUB_PACKET :
					break;
				case RLC_DL_PDU_SUB_PACKET :
					break;
				case RLC_DL_STATISTICS_SUB_PACKET :/*B087*/
				{
					u_int32_t num_of_channel= 0;
					VALIDATE_VERSION("RLC_DL_STATISTICS_SUB_PACKET",2,rlsps->sub_packet_version);
					/* Check for incoming buffer */
					VERIFY_LENGTH(rlc_dl_statistics_subpacket_dl ,length , SubIndex);
					rlc_dl_statistics_subpacket_dl *rdssdl =(rlc_dl_statistics_subpacket_dl*)&buf[SubIndex];
					d_log("\nNUM_RB=%d",rdssdl->num_rb);
					SubIndex += sizeof(*rdssdl);
					for(num_of_channel=0;num_of_channel<rdssdl->num_rb;num_of_channel++){
						/* Check for incoming buffer */
						VERIFY_LENGTH(rlc_dl_statistics_subpacket_num_rb ,length , SubIndex);
						rlc_dl_statistics_subpacket_num_rb  *rdssnr=(rlc_dl_statistics_subpacket_num_rb*)&buf[SubIndex];
						DL_NUM_RST += rdssnr->num_rst;
						DL_NUM_PDU_BYTES += rdssnr->data_pdu_bytes;
						DL_NUM_STATES_BYTES += rdssnr->status_rxed_bytes;
						DL_NUM_NVALID_PDU_BYTES += rdssnr->invalid_pdu_bytes;
						SubIndex += sizeof(*rdssnr);
					}
					/* RLC_DL_Througput */
					calculateThroughput(&(RlcDLThroughtput),(char *)timestamp ,DL_NUM_PDU_BYTES+DL_NUM_STATES_BYTES+DL_NUM_NVALID_PDU_BYTES ,\
							&(mLTEThroughput.downlinkRLCThroughput));
				}
				break;

				case RLC_UL_CONFIGURATION_SUB_PACKET :
					break;

				case RLC_UL_PDU_SUB_PACKET  :
					break;

				case  RLC_UL_STATISTICS_SUB_PACKET :/*B097*/
				{
					u_int32_t num_of_channel= 0;
					VALIDATE_VERSION("RLC_UL_STATISTICS_SUB_PACKET",1,rlsps->sub_packet_version);
					/* Check for incoming buffer */
					VERIFY_LENGTH(rlc_log_packet_structure_subpackets_ul ,length , SubIndex);
					rlc_log_packet_structure_subpackets_ul *rlpss=(rlc_log_packet_structure_subpackets_ul*)&buf[SubIndex];
					d_log("\nNUM_RB=%d",rlpss->num_rb);
					SubIndex += sizeof(*rlpss);
					for(num_of_channel=0;num_of_channel< rlpss->num_rb;num_of_channel++){
						/* Check for incoming buffer */
						VERIFY_LENGTH(rlc_log_packet_structure_subpackets_num_rb ,length , SubIndex);
						rlc_log_packet_structure_subpackets_num_rb  *rlpssnr=(rlc_log_packet_structure_subpackets_num_rb*)&buf[SubIndex];
						UL_NUM_RST += rlpssnr->num_rst;
						NUM_NEW_DATA_PDU_BYTES += rlpssnr->num_new_data_pdu_bytes;
						NUM_CTRL_PDU_BYTES += rlpssnr->num_ctrl_pdu_tx;
						UL_NUM_SDU_BYTES +=rlpssnr->num_sdu_bytes;
						UL_NUM_RETX_PDU_BYTES +=rlpssnr->num_retx_pdu_bytes;
						UL_NUM_POLL_BYTES  +=rlpssnr->num_poll;
						SubIndex += sizeof(*rlpssnr);
					}
					/*RLC_UL_Througput*/
					calculateThroughput(&(RlcULThroughtput),(char *)timestamp,NUM_NEW_DATA_PDU_BYTES + NUM_CTRL_PDU_BYTES + UL_NUM_RETX_PDU_BYTES , \
							&(mLTEThroughput.upLinkRLCThroughput));
				}
				break;

				case UNIFIED_RLC_DL_PDU_SUB_PACKET :
					break;

				case RLC_EMBMS_STATS_SUB_PACKET :
					break;

				case RLC_EMBMS_CONFIG_SUB_PACKET :
					break;

				default :
					break;
			}
			Index += rlsps->sub_packet_size;/* Increase index to next subpacket*/
			Num_of_subpkts--; /* Decrease number of subpacket */
		}

	}
		break;

	  /**********************************************************/
	  	  case LTE_ML1_PDSCH_STAT_INDICATON :
	  	  {
	  		VALIDATE_VERSION("LTE_ML1_PDSCH_STAT_INDICATON",4, buf[0]);
	  		  updatedownlinkMSCIndex(buf ,length);
	  	  }
	  	  break;

	  	  case 	LTE_LL1_PUCCH_CSF:
	  	  {
	  		VALIDATE_VERSION("LTE_LL1_PUCCH_CSF",2,buf[0]);
	  		  updateuplinkChannelInfo( buf , length);
	  	  }
	  	  break;

	  	  case 	LTE_ML1_DCI_INFORMATION_REPORT:
	  	  {
	  		VALIDATE_VERSION("LTE_ML1_DCI_INFORMATION_REPORT",2,buf[0]);
	  		  call_for_uplink_mcs_index( buf , length);
	  	  }
	  	  break;

	  	  default:
	  	  {
	  		  d_log("\nLTE Unknown packet = %d",code);
	  	  }
	  	 break;
	    }

}

/* Function to calculate LTE Throughput */
int get_LTEThroughput (LTEThroughput *ltethroughput){
	pthread_mutex_lock(&cellInfoMutex);
	*ltethroughput= mLTEThroughput;
	pthread_mutex_unlock(&cellInfoMutex);
  return 0;
}




