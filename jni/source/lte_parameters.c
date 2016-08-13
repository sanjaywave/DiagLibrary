#include <stdio.h>
#include <pthread.h>
#include "interfaceapi.h"
#include "decode.h"
#include "errors.h"
#include "lte_parameters.h"

#define  END_OF_TEST_CASE 0xFFFF

int phyLayerReportEnable = DISABLE;
unsigned int timeofprint;
static int firstPacketDLMCS = 0 ;
static int firstPacketULMCS = 0 ;
static LTEUplinkPhyStatus mLteUplinkPhyStatus;
static LTEDownLinkPhyStatus mlte_downlink_phy_status;
extern pthread_mutex_t cellInfoMutex;


void updatedownlinkMSCIndex( u_int8_t * buf , u_int32_t length)
{
	//printf(" updatedownlinkMSCIndex \n ");
	if (phyLayerReportEnable == ENABLE)
	{

		unsigned int countRecord=0;
		unsigned int countBlock;
		unsigned int total_mcs = 0, block_index = 0 , avg_dl_mcs = 0;
		int index = 0;
		/* Check for incoming buffer */
		VERIFY_LENGTH(lte_pdsch_stat_indication_header ,length ,index);
		lte_pdsch_stat_indication_header * pdsch_stats_header = (lte_pdsch_stat_indication_header *)&buf[index];
		index += sizeof(* pdsch_stats_header);
		if(pdsch_stats_header->version == 4) {
		/*	printf ("\nnum_of_records : %d ",pdsch_stats_header->num_of_records);
			printf ("\nreserved : %d ",pdsch_stats_header->reserved);
			printf ("\nversion : %d ",pdsch_stats_header->version);*/
			lte_pdsch_stat_indication_frame_info *pdsch_frame_info = NULL;
			for (countRecord=0;countRecord < pdsch_stats_header->num_of_records;countRecord++){
				/* Check for incoming buffer */
				VERIFY_LENGTH(lte_pdsch_stat_indication_frame_info ,length ,index);
				pdsch_frame_info = (lte_pdsch_stat_indication_frame_info *) &buf[index] ;
				index = index + sizeof(*pdsch_frame_info);
				/*printf ("\nframenumber : %d ",pdsch_frame_info->framenumber);
				printf ("\nnum_of_layers : %d ",pdsch_frame_info->num_of_layers);
				printf ("\nnum_of_rbs : %d ",pdsch_frame_info->num_of_rbs);
				printf ("\nnum_of_tb_blocks : %d ",pdsch_frame_info->num_of_tb_blocks);
				printf ("\nreserved : %d ",pdsch_frame_info->reserved);
				printf ("\nsubframe : %d ",pdsch_frame_info->subframe);*/
				lte_pdsch_stat_indication_tbInfo *pStatsIndication = NULL;
				for(countBlock=0; countBlock < pdsch_frame_info->num_of_tb_blocks; countBlock++){
					block_index++;
					/* Check for incoming buffer */
					VERIFY_LENGTH(lte_pdsch_stat_indication_tbInfo ,length ,index);
					pStatsIndication = (lte_pdsch_stat_indication_tbInfo *)&buf[index];
					total_mcs  += pStatsIndication->mcsIndex;
					index += sizeof(*pStatsIndication);

				/*	printf ("\nharqID : %d ",pStatsIndication->harqID);
					printf ("\nrv : %d ",pStatsIndication->rv);
					printf ("\nndi : %d ",pStatsIndication->ndi);
					printf ("\ncrc : %d ",pStatsIndication->crc);
					printf ("\nrnti : %d ",pStatsIndication->rnti);
					printf ("\ntbindex : %d ",pStatsIndication->tbindex);
					printf ("\ndiscardRe : %d ",pStatsIndication->discardRe);
					printf ("\ndidRecombine : %d ",pStatsIndication->didRecombine);
					printf ("\nreserved1 : %d ",pStatsIndication->reserved1);
					printf ("\ntbsize : %d ",pStatsIndication->tbsize);
					printf ("\nreserved2 : %d ",pStatsIndication->reserved2);
					printf ("\nmcsIndex : %d ",pStatsIndication->mcsIndex);
					printf ("\nnumberOfRbs : %d ",pStatsIndication->numberOfRbs);*/
				}

				if(countBlock < 3)
				{
					index = index + (3-countBlock) * sizeof(*pStatsIndication);
				}
				//lte_pdsch_stat_indication_frame_info *pdsch_frame_info = (lte_pdsch_stat_indication_frame_info*)&buf[index];
				//FIXED THIS BY SANJAY
				index = index + 2;//Increament for PMCH ID and AREA ID which is after TB information .


			}
			if(total_mcs){
				avg_dl_mcs = (total_mcs) / block_index;
				if ((timeofprint == END_OF_TEST_CASE) && (firstPacketDLMCS)){
					mlte_downlink_phy_status.DL_MCS_Index_Average = (mlte_downlink_phy_status.DL_MCS_Index_Average + avg_dl_mcs)/2;
				}else if ((timeofprint == END_OF_TEST_CASE) && (!firstPacketDLMCS)){
					mlte_downlink_phy_status.DL_MCS_Index_Average =  avg_dl_mcs;
					firstPacketDLMCS = 1 ;
				}else{
					mlte_downlink_phy_status.DL_MCS_Index_Average = avg_dl_mcs;
				}
			}
		}
	}
}

/*
 * Function  to   update Uplink channel infomation on LTEUplinkPhyStatus structure for LTE
 * Argument : - LTEUplinkPhyStatus struct  , Input buffer and buffer length
 */
void updateuplinkChannelInfo(u_int8_t *buf,unsigned int length)
{
	//printf(" updateuplinkChannelInfo \n ");
	if (phyLayerReportEnable == ENABLE){
		u_int32_t Index = 0;
		/* Check for incoming buffer */
		VERIFY_LENGTH(lte_ll1_pucch_csf ,length ,Index);
		lte_ll1_pucch_csf *llpc = (lte_ll1_pucch_csf *)&buf[Index];
		Index += sizeof(*llpc);
		if(llpc->version == 2){
			/* Check for incoming buffer */
			VERIFY_LENGTH(lte_ll1_pucch_csf_ver_v2 ,length ,Index);
			lte_ll1_pucch_csf_ver_v2 *llpcV2 =(lte_ll1_pucch_csf_ver_v2 *)&buf[Index];
			mLteUplinkPhyStatus.UL_NUM_SubBand = llpcV2->num_of_subbands;
			/*Common things are placed here*/
			if(!llpcV2 ->pucch_report_type ){
					mLteUplinkPhyStatus.UL_SubBand_CQI_CW_0= llpcV2 ->cqi_cw0;
					if (3 == llpcV2->pucch_reporting_mode){
						mLteUplinkPhyStatus.UL_SubBand_CQI_CW_1 = llpcV2 ->cqi_cw1;
					}
			}else if (llpcV2 ->pucch_report_type == 1){
				/* Wideband CQI, PMI Feedback Type 2*/
				/* Wideband CQI CW0 */
				/*Wideband CQI for CW1*/
				/*pucch_report_type = type 2 */
				mLteUplinkPhyStatus.Uplink_WideBand_CQI_CW_0 = llpcV2 ->cqi_cw0;
				if (1 == llpcV2->pucch_reporting_mode){
					mLteUplinkPhyStatus.Uplink_Wideband_CQI_CW_1 = llpcV2 ->cqi_cw1;
				}
				mLteUplinkPhyStatus.Uplink_Wideband_PMI = llpcV2 ->wideband_pmi;
			}
			else if (llpcV2 ->pucch_report_type == 2){
				/*RI Feedback Type 3*/
				/*Rank_Index*/
				mLteUplinkPhyStatus.Uplink_Rank_Index =llpcV2 ->rank_index;
			}else if (llpcV2 ->pucch_report_type == 3){
				/*Wideband CQI Feedback Type 4*/
				/* Wideband CQI  CW0 */
				mLteUplinkPhyStatus.Uplink_WideBand_CQI_CW_0 = llpcV2 ->cqi_cw0;
			}
		}
	}
}

/*
 * Function  to  uplink MCS Index for LTE
 * Argument : -  LTEUplinkPhyStatus struct , Input buffer and buffer length
 */
void call_for_uplink_mcs_index( u_int8_t *buf,unsigned int length)
{
	//printf(" call_for_uplink_mcs_index \n ");
	if(phyLayerReportEnable == ENABLE){
		u_int32_t index = 0, Counter;
		u_int32_t total_mcs = 0 ;
		u_int32_t avg_ul_mcs = 0;
		/* Check for incoming buffer */
		VERIFY_LENGTH(lte_ml1_dci_information_report ,length ,index);
		lte_ml1_dci_information_report *lmdir = (lte_ml1_dci_information_report *)&buf[index];
		index += sizeof(*lmdir);
		if(lmdir->version == 2){
			 for (Counter =0 ;Counter < lmdir->number_of_records ;Counter++){
				 /* Check for incoming buffer */
				 VERIFY_LENGTH(lte_ml1_dci_information_report_records ,length ,index);
				 lte_ml1_dci_information_report_records *lmdirr = (lte_ml1_dci_information_report_records *)&buf[index];
				 index += sizeof(*lmdirr);
				 total_mcs = total_mcs + lmdirr->mcs_index;
			 }

			 if(total_mcs){

				 avg_ul_mcs = (total_mcs) / lmdir->number_of_records;
				 if ((timeofprint == END_OF_TEST_CASE) && (firstPacketULMCS)){
					 mLteUplinkPhyStatus.UL_MCS_Index_Average =  (mLteUplinkPhyStatus.UL_MCS_Index_Average + avg_ul_mcs)/2;
				 }else if ((timeofprint == END_OF_TEST_CASE) && (!firstPacketULMCS)){
					 mLteUplinkPhyStatus.UL_MCS_Index_Average =  avg_ul_mcs;
					 firstPacketULMCS = 1;
				 }else{
					 mLteUplinkPhyStatus.UL_MCS_Index_Average =  avg_ul_mcs;
				 }
			 }
		 }
	}
}


void get_lte_uplink_phy_status(LTEUplinkPhyStatus* uplinkstatus)
{
	pthread_mutex_lock(&cellInfoMutex);
	// printf("  get_lte_uplink_phy_status \n ");
	//d_log_func("\n UL_MCS_Index_Average 	: %u ", mLteUplinkPhyStatus.UL_MCS_Index_Average);
	*uplinkstatus = mLteUplinkPhyStatus;
	pthread_mutex_unlock(&cellInfoMutex);
}

 void get_lte_downlink_phy_status(LTEDownLinkPhyStatus* downlinkstatus)
{
	 pthread_mutex_lock(&cellInfoMutex);
	// d_log_func("\n  DL_MCS_Index_Average 	: %u ", mlte_downlink_phy_status.DL_MCS_Index_Average);
	*downlinkstatus = mlte_downlink_phy_status;
	pthread_mutex_unlock(&cellInfoMutex);
}

void print_lte_phy_status()
{
	//printf(" print_lte_phy_status \n ");
	d_log_func("\n UL_MCS_Index_Average 	: %u ", mLteUplinkPhyStatus.UL_MCS_Index_Average);
	d_log_func("\n  DL_MCS_Index_Average 	: %u ", mlte_downlink_phy_status.DL_MCS_Index_Average);
	d_log_func("\n\n\n");

	d_log_func("\n ****************""Lte Uplink Phy Status""****************************");
	d_log_func("\n Lte Uplink Phy Status : UL_MCS_Index_Average 	: %d ", mLteUplinkPhyStatus.UL_MCS_Index_Average);
	d_log_func("\n Lte Uplink Phy Status : UL_NUM_SubBand 			: %d ", mLteUplinkPhyStatus.UL_NUM_SubBand);
	d_log_func("\n Lte Uplink Phy Status : UL_SubBand_CQI_CW_0 		: %d ", mLteUplinkPhyStatus.UL_SubBand_CQI_CW_0);
	d_log_func("\n Lte Uplink Phy Status : UL_SubBand_CQI_CW_1 		: %d ", mLteUplinkPhyStatus.UL_SubBand_CQI_CW_1);
	d_log_func("\n Lte Uplink Phy Status : Uplink_Rank_Index  		: %d ", mLteUplinkPhyStatus.Uplink_Rank_Index);
	d_log_func("\n Lte Uplink Phy Status : Uplink_WideBand_CQI_CW_0	: %d ", mLteUplinkPhyStatus.Uplink_WideBand_CQI_CW_0);
	d_log_func("\n Lte Uplink Phy Status : Uplink_Wideband_CQI_CW_1 : %d ", mLteUplinkPhyStatus.Uplink_Wideband_CQI_CW_1);
	d_log_func("\n Lte Uplink Phy Status : Uplink_Wideband_PMI		: %d ", mLteUplinkPhyStatus.Uplink_Wideband_PMI);

/*
	printf("\n UL_MCS_Index_Average 	: %u ", mLteUplinkPhyStatus.UL_MCS_Index_Average);
	printf("\n  DL_MCS_Index_Average 	: %u ", mlte_downlink_phy_status.DL_MCS_Index_Average);
	printf("\n\n\n");
	printf("\n ****************""Lte Uplink Phy Status""****************************");
	printf("\n Lte Uplink Phy Status : UL_MCS_Index_Average 	: %d ", mLteUplinkPhyStatus.UL_MCS_Index_Average);
	printf("\n Lte Uplink Phy Status : UL_NUM_SubBand 			: %d ", mLteUplinkPhyStatus.UL_NUM_SubBand);
	printf("\n Lte Uplink Phy Status : UL_SubBand_CQI_CW_0 		: %d ", mLteUplinkPhyStatus.UL_SubBand_CQI_CW_0);
	printf("\n Lte Uplink Phy Status : UL_SubBand_CQI_CW_1 		: %d ", mLteUplinkPhyStatus.UL_SubBand_CQI_CW_1);
	printf("\n Lte Uplink Phy Status : Uplink_Rank_Index  		: %d ", mLteUplinkPhyStatus.Uplink_Rank_Index);
	printf("\n Lte Uplink Phy Status : Uplink_WideBand_CQI_CW_0	: %d ", mLteUplinkPhyStatus.Uplink_WideBand_CQI_CW_0);
	printf("\n Lte Uplink Phy Status : Uplink_Wideband_CQI_CW_1 : %d ", mLteUplinkPhyStatus.Uplink_Wideband_CQI_CW_1);
	printf("\n Lte Uplink Phy Status : Uplink_Wideband_PMI		: %d ", mLteUplinkPhyStatus.Uplink_Wideband_PMI);
*/
}


void SetLtePhysicalLayerReporting (unsigned int flag ,unsigned int interval)
{
//	printf(" SetLtePhysicalLayerReporting \n ");
	phyLayerReportEnable = flag;
	timeofprint = interval;
	memset(&mLteUplinkPhyStatus,0,sizeof(mLteUplinkPhyStatus));
	memset(&mlte_downlink_phy_status,0,sizeof(mlte_downlink_phy_status));
}
