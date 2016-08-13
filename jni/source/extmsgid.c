#include <sys/types.h>
#include <stdio.h>
#include "extmsgid.h"

u_int16_t convert_into_external_code (u_int16_t command_code){
	u_int16_t external_commamd_code=0;
	switch(command_code)
	{
	   /*************System command code ********************************/
		case 0x65 : external_commamd_code = SYSTEM_TIME_CMD;
			     break;
		/******************************Gsm code conversion*******************/
		case 0x51FA: external_commamd_code = GRR_STATE;
			     break;
		case 0x51FC: external_commamd_code = GRR_CELL_RESELECTION_MEASURMENT;
			     break;
		case 0x512C: external_commamd_code = GSM_RR_STATE_MESSAGE;
			     break;
		case 0x5134:  external_commamd_code = GSM_RR_CELLINFORM;
			      break;
		case 0x5135:  external_commamd_code = GSM_RR_CHANNEL_CONFIGURE;
			      break;
		case 0x513A:  external_commamd_code = GSM_RR_SACCH_REPORT;
			      break;
		case 0x5133:  external_commamd_code = GSM_RR_CELL_OPTIONS;
			      break;
		case 0x51FE:  external_commamd_code = GRR_PACKET_SYSTEM_INFORMATION;
			      break;
		case 0x5233:  external_commamd_code = GSM_TIMING_ADVANCE;
			      break;
		case 0x5139:  external_commamd_code = GSM_RR_RADIO_LINK_TIMEOUT_COUNTER;
			      break;
		case 0x506C:  external_commamd_code = GSM_L1_BRUST_METRICS;
			      break;
		case 0x5228:  external_commamd_code = GSM_MAC_UL_TBF_ESTABLISH;
			      break;
		case 0x5205:  external_commamd_code = GPRS_OR_EGPRS_RLC_UPLINK_HEADER;
			      break;
		case 0x5245:  external_commamd_code = GSM_L1_BRUST_METRICS_A;
			      break;
		case 0x5249:  external_commamd_code = GSM_RR_SACCH_REPORT;
			      break;
		case 0x524D:  external_commamd_code = GSM_L1_MESSAGE_METRICS_A;
			      break;
		case 0x520F:  external_commamd_code = RLC_UL_ACK_OR_NACK_PARAMETERS;
			      break;
		case 0x5206:  external_commamd_code = GPRS_OR_EGPRS_RLC_DOWNLINK_HEADER;
			      break;
		case 0x522A:  external_commamd_code = MAC_DL_TBF_ESTABLISH;
			      break;
		case 0x5212:  external_commamd_code = GSM_LLC_ME_INFORMATION;
			      break;
		case 0x5202:  external_commamd_code = GSM_RLC_UL_STATISTICS;
			      break;
		case 0x5207:  external_commamd_code = GPRS_EGPRS_RLC_UL_STATISTICS;
			      break;
		case 0x5211:  external_commamd_code = EGPRS_RLC_DOWNLINK_ACK_OR_NACK;
			      break;
		case 0x5210:  external_commamd_code = RLC_DL_ACK_OR_NACK_PARAMETERS;
			      break;
		case 0x522C:  external_commamd_code =GPRS_OR_EGPRS_MAC_UPLINK_ACK_OR_NACK; 
			      break;
		case 0x520A:  external_commamd_code = GSM_RLC_DL_STATICS;
			      break;
		case 0x5131:  external_commamd_code = GSM_RR_RACH_CONTROL_PARAMETRES;
			      break;
		case 0x506F:  external_commamd_code = GSM_PAGING_DATA;
			      break;
		case 0x5132:  external_commamd_code = GSM_RR_CONTROL_CHANNEL_DESC_PARAMS;
			      break;
		case 0x512F:  external_commamd_code = GSM_RR_SIGNALING_MESSAGE;
			      break;
		case 0x5209:  external_commamd_code = EGPRS_RLC_UPLINK_ACK_NACK;
			      break;
		case 0x5079:  external_commamd_code = GSM_L1_ENHANCED_MESSAGE_METRICS;
			      break;
		case 0x5215:  external_commamd_code = LLC_PDU_STATISTICS;
				break;

	 /***************************wcdma code conversion ********************/

		case 0x4005:  external_commamd_code = WCDMA_CELL_RESELECTION_PACKET;
			      break;
		case 0x410C:  external_commamd_code = WCDMA_PHYSICAL_CHANNELS_UPLINKS;
			      break;
		case 0x4110:  external_commamd_code = WCDMA_ACTIVE_SET;
			      break;
		case 0x4111:  external_commamd_code = WCDMA_NEIGHBOR_SET;
			      break;
		case 0x4127:  external_commamd_code = WCDMA_CELL_ID;
			      break;
		case 0x412F:  external_commamd_code = WCDMA_RRC_SIGNALING_MESSAGE;
			      break;
		case 0x4125:  external_commamd_code = WCDMA_RRC_STATES;
			      break;
		case 0x4208:  external_commamd_code = HS_DSCH_HARQ_STA_LOG_PKT;
			      break;
		case 0x4210:  external_commamd_code = MAC_HS_CONFIG_LOG_PKT;
			      break;
		case 0x4211:  external_commamd_code = MAC_HS_HEADER_LOG_PKT;
			      break;
		case 0x4212:  external_commamd_code = MAC_HS_STATUS_LOG_PKT;
			      break;
		case 0x4213:  external_commamd_code = MAC_HS_RESET_LOG_PKT;
			      break;
		case 0x4214:  external_commamd_code = HS_DECODE_STATUS_LOG_PKT_VER_2;
			      break;
		case 0x4010:  external_commamd_code = WCDMA_L1_POWER_CONTROLL_WITH_COMPRESSED_MODE_PKT;
			      break;
		case 0x410D:  external_commamd_code = WCDMA_PRACH;
			      break;
		case 0x4116:  external_commamd_code = WCDMA_BLER;
			      break;
		case 0x4132:  external_commamd_code = WCDMA_RLC_DL_AM_CONFIGURATION_V1;
			      break;
		case 0x4160:  external_commamd_code = WCDMA_RACH_PARAMETERS;
			      break;
		case 0x4138:  external_commamd_code = WCDMA_RLC_UL_AM_STATISTICS_VER_2;
			      break;
		case 0x414A:  external_commamd_code = WCDMA_RLC_DL_AM_STATISTICS;
			      break;
		case 0x4209:  external_commamd_code = HS_CQI_STATISTICS;
			      break;
		case 0x420B : external_commamd_code = UL_HS_DPCCH_DEBUG_LOG_PkT;
			      break;
		case 0x4222 : external_commamd_code = HS_DECODE_STATUS_LOG_PKT_EDITION_3;
					break;
		case 0x4311 : external_commamd_code = EUL_L1_MAC_STATISTICS_LOG_PKT;
			      break;
		case 0x401C : external_commamd_code = WCDMA_LIST_SEARCH_VER_5;
				  break;
		case 0x4186 : external_commamd_code = WCDMA_TEMPORAL_ANALYSIS_ED2;
				  break;
		case 0x4179 : external_commamd_code = WCDMA_PN_SEARCH_LOG_PACKET_ED2;
				  break;

	/*********************NAS ITEM*******************************/

		case 0x7130:  external_commamd_code = UMTS_NAS_GMM_STATE;
			      break;
		case 0x7131:  external_commamd_code =UMTS_NAS_MM_STATE;
			      break;
		case 0x713A:  external_commamd_code = UMTS_NAS_SIGNALING;
			      break;
		case 0x7133:  external_commamd_code =UMTS_NAS_CS_CONNECTION_MANAGEMENT_CALL_SETUP;
			      break;
		case 0x7138:  external_commamd_code =UMTS_UE_DYNAMIC_ID;
				  break;

	/*****************************LTE***************************/
		case 0xB0C0:  external_commamd_code = LTE_OTA_MESSAGE;
			      break;
		case 0xB0C1:  external_commamd_code = LTE_MIB_MESSAGE;
			      break;
		case 0xB0C2:  external_commamd_code = LTE_SERVING_CELL_INFO;
			      break;
		case 0xB193:  external_commamd_code = LTE_ML1_SERVING_CELL_MEASUREMENT;
			      break;
		case 0xB17F:  external_commamd_code = LTE_ML1_SERVING_CELL_MEASUREMENT_AND_EVALUTION;
			      break;
		case 0xB166:  external_commamd_code = LTE_ML1_PRACH_CONFIGURATION;
			      break;
		case 0xB060:  external_commamd_code = MAC_RACH_CFG_SUBPACKET;
			      break;
		case 0xB062:  external_commamd_code = MAC_RACH_ATTEMPT_SUBPACKET;
			      break;
		case 0xB063:  external_commamd_code = LTE_MAC_DL_TRANSPORT_BLOCK;
				  break;
		case 0xB067:  external_commamd_code = MAC_UL_TX_STATS_SUBPACKET;
			      break;
		case 0xB064:  external_commamd_code = MAC_UL_TB_SUBPACKET;
			      break;
		case 0xB16C:  external_commamd_code = LTE_ML1_DCI_INFORMATION_REPORT;
			      break;
		case 0xB0E5:  external_commamd_code = LTE_NAS_ESM_BEARER_CONTEXT_INFO;
				  break;
		case 0xB0E2:  external_commamd_code = LTE_NAS_ESM_PLAIN_OTA_INCOMING_MSG;
			      break;
		case 0xB0E3:  external_commamd_code = LTE_NAS_ESM_PLAIN_OTA_OUTGOING_MSG;
			      break;
		case 0xB0EE:  external_commamd_code = LTE_NAS_EMM_STATE;
				  break;
		case 0xB087:  external_commamd_code = LTE_RLC_DL_STATISTICS;
			      break;
		case 0xB097:  external_commamd_code = LTE_RLC_UL_STATISTICS;
			      break;
		case 0xB132:  external_commamd_code = LTE_LL1_PDSCH_DECORDINR_RESULTS;
				  break;
		case 0xB139:  external_commamd_code = LTE_LL1_PUSCH_TX_REPORT ;
				  break;
		case 0xB14D:  external_commamd_code = LTE_LL1_PUCCH_CSF ;
				  break;
		case 0xB167:  external_commamd_code = LTE_ML1_RANDOM_ACCESS_REQUEST_REPORT ;
				  break;
		case 0xB171:  external_commamd_code = LTE_ML1_SRS_POWER_CONTROLL_REPORT ;
				  break;
		case 0xB173:  external_commamd_code = LTE_ML1_PDSCH_STAT_INDICATON ;
				  break;
		case 0xB16F:  external_commamd_code = LTE_ML1_PUCCH_POWER_CONTROLL ;
				  break;
		case 0xB168:  external_commamd_code = LTE_ML1_RANDOM_ACCESS_RESPONSE_REPORT ;
				  break;
		case 0xB0A4:  external_commamd_code = PDCP_DL_STATISTICS ;
				  break;
		case 0xB0B4:  external_commamd_code = PDCP_UL_STATISTICS ;
				  break;
		case 0xB16B:  external_commamd_code = LTE_ML1_PDCCH_PHICH_INDICATION_REPORT;
				  break;
		case 0xB16D:  external_commamd_code = LTE_ML1_GM_TX_REPORT;
				  break;
		case 0xB0EC:  external_commamd_code = LTE_NAS_EMM_PLAIN_OTA_INCOMING_MSG;
				  break;
		case 0xB0ED:  external_commamd_code = LTE_NAS_EMM_PLAIN_OTA_OUTGOING_MSG;
				  break;
	/****************************EVENTs*****************************/

		case 1606:  external_commamd_code = LTE_EVENT_RRC_STATE_CHANGE ;
			 	 break;
		case 1966:  external_commamd_code = LTE_EVENT_NAS_EMM_OTA_INCOMING_MESSAGE ;
			 	 break;
		case 1967:  external_commamd_code = LTE_EVENT_NAS_EMM_OTA_OUTGOING_MESSAGE ;
			  	  break;
		case 1968:  external_commamd_code = LTE_EVENT_NAS_ESM_OTA_INCOMING_MESSAGE ;
			  	  break;
		case 1969:  external_commamd_code = LTE_EVENT_NAS_ESM_OTA_OUTGOING_MESSAGE ;
			  	  break;
		case 1615:  external_commamd_code = LTE_EVENT_MOBILITY_FROM_E_UTRA_STARTED ;
				  break;
		case 1616:  external_commamd_code = LTE_EVENT_MOBILITY_FROM_E_UTRA_FAILED ;
				  break;
		case 1611:  external_commamd_code = LTE_EVENT_NEW_CELL_INDICATION ;
				  break;
		case 1609:  external_commamd_code = LTE_EVENT_RRC_DOWNLINK_MESSAGE ;
				  break;
		case 1610:  external_commamd_code = LTE_EVENT_RRC_UPLINK_MESSAGE ;
				  break;
		case 1613 :  external_commamd_code = LTE_HANDOVER_FAILURE;
				  break;
		case 1617 :  external_commamd_code = LTE_EVENT_INTER_RAT_RESELECTION_FROM_E_UTRAN_START;
				  break;
		case 1618 :  external_commamd_code = LTE_EVENT_INTER_RAT_RESELECTION_FROM_E_UTRAN_END;
				  break;
		case 1807 :  external_commamd_code = LTE_EVENT_INTER_RAT_REDIRECTION_FROM_E_UTRAN_START;
				  break;
		case 1808 :  external_commamd_code = LTE_EVENT_INTER_RAT_REDIRECTION_FROM_E_UTRAN_END;
				  break;
		case 1608 :  external_commamd_code = LTE_EVENT_RADIO_LINK_FAILURE;
				  break;
		case 1995 :  external_commamd_code = LTE_EVENT_RADIO_LINK_FAILURE_STAT;
				  break;
	    case 1635 :  external_commamd_code = LTE_EVENT_NAS_ESM_INCOMING_MESSAGE;
				  break;
		case 1636 :  external_commamd_code = LTE_EVENT_NAS_ESM_OUTGOING_MESSAGE;
		          break;
	    case 1501 :  external_commamd_code = LTE_EVENT_RACH_ACCESS_START;
		   		  break;
		case 1503 :  external_commamd_code = LTE_EVENT_RACH_ACCESS_RESULT;
		   		break;

/***************************************CDMA*************************************/
		case 0x1069 :  external_commamd_code = EVDO_POWER_LOG ;
				  break;
		case 0x119A :  external_commamd_code = SRCH_TNG_FINGER_STATUS;
				  break;
		case 0x119B :  external_commamd_code = SRCH_TNG_1X_SEARCHER_DUMP;
				  break;
		case 0x119C :  external_commamd_code = SRACH_TNG_GENERAL_STATUS;
				  break;
		case 0x119D :  external_commamd_code = SRCH_TNG_DEMOD_INFO;
				  break;
		case 0x119E :  external_commamd_code = SRCH_TNG_SLEEP_STATS;
				  break;
		case 0x11A2 :  external_commamd_code = SRCH_TNG_FINGER_CHANNEL_ESTIMATE;
				  break;
		case 0x129C :  external_commamd_code = EVDO_MULTICARRIER_POWER;
				  break;
		//case 0x134F :  external_commamd_code = CM_SERVING_SYSTEM_EVENT;
				//  break;
		case 0x108A :  external_commamd_code = EVDO_FINGER_LOG;
				  break;
		case 0x1068 :  external_commamd_code = EVDO_AIR_LINK_SUMMARY;
				  break;
		case 0x19   :  external_commamd_code = TEMPORAL_ANALYZER_GRAPH;
				  break;
		default: external_commamd_code = 0;
					 break;

	}
	return external_commamd_code;
}







