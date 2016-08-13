#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "rrc.h"
#include "rtsrc/rtPrint.h"
#include "rtxsrc/rtxCommon.h"
#include "rtsrc/rtContext.h"
#include "errors.h"
#include "rrcDecoder.h"
#include "ccEvents.h"
#include "interfaceapi.h"


#define MAX_MSG_NAME 200
static unsigned char eventbuff[20];
static int ismt = 0;

extern int sendInformation ( int systype, int notifier ,int identifier , char *buf , int length);

/*To check for presence of optional-ie*/
#define WAVEMSGANALYSER_CHECK_OPT_IE(p_encoded_data)	((*(char *)(p_encoded_data + 0)) & 0x80)

/*To fetch message-id from encoded buffer when optional-ie is not present*/
#define WAVEMSGANALYSER_FETCH_MSG_ID(msg_table_input, p_encoded_data, maxbits)\
msg_table_input = *(p_encoded_data) << 1;\
msg_table_input = msg_table_input >> (8 - maxbits);\

/*To fetch message-id from encoded buffer when optional-ie is present*/
#define WAVEMSGANALYSER_FETCH_MSG_ID_OPTPRESENT(msg_table_input, p_encoded_data, maxbits)\
int num_spillover_bits = (maxbits + 36 + 1) % 8;\
int second_last_byte = (maxbits + 36 + 1) / 8;\
\
if (num_spillover_bits < maxbits)\
{\
	msg_table_input = *(p_encoded_data + second_last_byte - 1) << (8 - (maxbits - num_spillover_bits));\
	msg_table_input = msg_table_input >> (8 - maxbits);\
	msg_table_input |= (*(p_encoded_data + second_last_byte) >> (8 - num_spillover_bits));\
}\
else if (num_spillover_bits == maxbits)\
{\
	msg_table_input = *(p_encoded_data + second_last_byte) >> (8 - num_spillover_bits);\
}\
else \
{\
	msg_table_input = *(p_encoded_data + second_last_byte) << (num_spillover_bits - maxbits);\
	msg_table_input = msg_table_input >> (8 - maxbits);\
}


char EventIDInter[][50]=
{
   "2a",
   "2b",
   "2c",
   "2d",
   "2e",
   "2f",
   "spare2_32",
   "spare1_45"
};

char EventIDIntra[][50]=
{
   "1a" ,
   "1b" ,
   "1c" ,
   "1d" ,
   "1e" ,
   "1f" ,
   "1g" ,
   "1h" ,
   "1i" ,
   "1j" ,
   "spare6_11" ,
   "spare5_15" ,
   "spare4_21" ,
   "spare3_27" ,
   "spare2_33" ,
   "spare1_46"
} ;

char dlTransportChannelType[][25]=
{
	"NULL",
	"dch",
	"fach",
	"dsch",
	"dch_and_dsch",
	"hsdsch",
	"dch_and_hsdsch"
};

char ValueToSring[][MAX_MSG_NAME]=
{
	"NULL",
	"ACTIVE_SET_UPDATE",
	"ASSISTANCE_DATA_DELIVERY",
	"CELL_CHANGE_ORDER_FROM_UTRAN",
	"DL_DCCH_MESSAGE_TYPE_CELL_UPDATE_CONFIRM",
	"COUNTER_CHECK",
	"DOWNLINK_DIRECT_TRANSFER",
	"HANDOVER_FROM_UTRAN_COMMAND_GSM",
	"HANDOVER_FROM_UTRAN_COMMAND_CDMA2000",
	"MEASUREMENT_CONTROL",
	"PAGING_TYPE2",
	"RRC_DL_DCCH_MESSAGE_TYPE_PHYSICAL_CHANNEL_RECONFIGURATION",
	"DL_DCCH_MESSAGE_TYPE_PHYSICAL_SHARED_CHANNEL_ALLOCATION",
	"DL_DCCH_MESSAGE_TYPE_RADIO_BEARER_RECONFIGURATION",
	"DL_DCCH_MESSAGE_TYPE_RADIO_BEARER_RELEASE",
	"DL_DCCH_MESSAGE_TYPE_RADIO_BEARER_SETUP",
	"DL_DCCH_MESSAGE_TYPE_RRC_CONNECTION_RELEASE",
	"RRCURITY_MODE_COMMAND",
	"SIGNALLING_CONNECTION_RELEASE",
	"DL_DCCH_MESSAGE_TYPE_TRANSPORT_CHANNEL_RECONFIGURATION",
	"TRANSPORT_FORMAT_COMBINATION_CONTROL",
	"UE_CAPABILITY_ENQUIRY",
	"UE_CAPABILITY_INFORMATION_CONFIRM",
	"UPLINK_PHYSICAL_CHANNEL_CONTROL",
	"DL_DCCH_MESSAGE_TYPE_URA_UPDATE_CONFIRM",
	"UTRAN_MOBILITY_IN_FORMATION",
	"HANDOVER_FROM_UTRAN_COMMAND_GERANIU",
	"DL_DCCH_MESSAGE_TYPE_MBMS_MODIFIED_SERVICES_INFORMATION",
	"ETWSPRIMARY_NOTIFICATION_WITH_SECURITY",
	"HANDOVER_FROM_UTRANCMD_EUTRA",
	"ACTIVE_SET_UPDATE_COMPLETE",
	"ACTIVE_SET_UPDATE_FAILURE",
	"CELL_CHANGE_ORDER_FROM_UTRAN_FAILURE",
	"COUNTER_CHECK_RESPONSE",
	"HANDOVER_TO_UTRAN_COMPLETE",
	"INITIAL_DIRECT_TRANSFER",
	"HANDOVER_FROM_UTRAN_FAILURE",
	"MEASUREMENT_CONTROL_FAILURE",
	"MEASUREMENT_REPORT",
	"PHYSICAL_CHANNEL_RECONFIGURATION_COMPLETE",
	"PHYSICAL_CHANNEL_RECONFIGURATION_FAILURE",
	"RADIO_BEARER_RECONFIGURATION_COMPLETE",
	"RADIO_BEARER_RECONFIGURATION_FAILURE",
	"RADIO_BEARER_RELEASE_COMPLETE",
	"RADIO_BEARER_RELEASE_FAILURE",
	"RADIO_BEARER_SETUP_COMPLETE",
	"RADIO_BEARER_SETUP_FAILURE",
	"RRC_CONNECTION_RELEASE_COMPLETE",
	"RRC_CONNECTION_SETUP_COMPLETE",
	"RRC_STATUS",
	"SECURITY_MODE_COMPLETE",
	"SECURITY_MODE_FAILURE",
	"SIGNALLING_CONNECTION_RELEASE_INDICATION",
	"TRANSPORT_CHANNEL_RECONFIGURATION_COMPLETE",
	"TRANSPORT_CHANNEL_RECONFIGURATION_FAILURE",
	"TRANSPORT_FORMAT_COMBINATION_CONTROL_FAILURE",
	"UE_CAPABILITY_INFORMATION",
	"UPLINK_DIRECT_TRANSFER",
	"UTRAN_MOBILITY_INFORMATION_CONFIRM",
	"UTRAN_MOBILITY_INFORMATION_FAILURE",
	"MBMS_MODIFICATION_REQUEST",
	"DL_CCCH_MESSAGE_TYPE_CELL_UPDATE_CONFIRM",
	"RRC_CONNECTION_REJECT ",
	"DL_CCCH_MESSAGE_TYPE_RRC_CONNECTION_RELEASE",
	"RRC_CONNECTION_SETUP",
   "DL_CCCH_MESSAGE_TYPE_URA_UPDATE_CONFIRM",
	"ETWS_PRIMARY_NOTIFICATIONWITH_SECURITY",
	"CELL_UPDATE",
	"RRC_CONNECTION_REQUEST",
	"URA_UPDATE",
	"PAGING_TYPE_1",
	"DL_SHCCH_MESSAGE_TYPE_PHYSICAL_SHARED_CHANNEL_ALLOCATION",
	"PUSCH_CAPACITY_REQUEST",
	"SYSTEM_INFORMATION ",
	"SYSTEM_INFORMATION_CHANGE_INDICATION",
	"SYS_INFMTION_BCH",
	"MBMS_ACCESS_INFORMATION",
	"MBMS_COMMON_PTMRB_INFORMATION",
	"MBMS_CURRENT_CELL_PTMRB_INFORMATION",
	"MBMS_GENERAL_INFORMATION",
	"MCCH_MESSAGE_TYPE_MBMS_MODIFIED_SERVICES_INFORMATION",
	"MBMS_NEIGHBOURING_CELL_PTMRB_INFORMATION",
	"MBMS_UNMODIFIED_SERVICES_INFORMATION",
	"MBMS_SCHEDULING_INFORMATION",
	"SYS_INFO_TYPE_1",
	"SYS_INFO_TYPE_2",
	"SYS_INFO_TYPE_3",
	"SYS_INFO_TYPE_4",
	"SYS_INFO_TYPE_5",
	"SYS_INFO_TYPE_6",
	"SYS_INFO_TYPE_7",
	"SYS_INFO_TYPE_8",
	"SYS_INFO_TYPE_9",
	"SYS_INFO_TYPE_10",
	"SYS_INFO_TYPE_11",
	"SYS_INFO_TYPE_11BIS",
	"SYS_INFO_TYPE_12",
	"SYS_INFO_TYPE_13",
	"SYS_INFO_TYPE_13_1",
	"SYS_INFO_TYPE_13_2",
	"SYS_INFO_TYPE_13_3",
	"SYS_INFO_TYPE_13_4",
	"SYS_INFO_TYPE_14",
	"SYS_INFO_TYPE_15",
	"SYS_INFO_TYPE_15BIS",
	"SYS_INFO_TYPE_15_1",
	"SYS_INFO_TYPE_15_1BIS",
	"SYS_INFO_TYPE_15_2",
	"SYS_INFO_TYPE_15_2BIS",
	"SYS_INFO_TYPE_15_2TER",
	"SYS_INFO_TYPE_15_3",
	"SYS_INFO_TYPE_15_3BIS",
	"SYS_INFO_TYPE_15_4",
	"SYS_INFO_TYPE_15_5",
	"SYS_INFO_TYPE_15_6",
	"SYS_INFO_TYPE_15_7",
	"SYS_INFO_TYPE_16"
};

typedef enum{
	RRC_ACTIVE_SET_UPDATE_MSG=1,
	RRC_ASSISTANCE_DATA_DELIVERY_MSG,
	RRC_CELL_CHANGE_ORDER_FROM_UTRAN_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_CELL_UPDATE_CONFIRM_MSG,
	RRC_COUNTER_CHECK_MSG,
	RRC_DOWNLINK_DIRECT_TRANSFER_MSG,
	RRC_HANDOVER_FROM_UTRAN_COMMAND_GSM_MSG,
	RRC_HANDOVER_FROM_UTRAN_COMMAND_CDMA2000_MSG,
	RRC_MEASUREMENT_CONTROL_MSG,
	RRC_PAGING_TYPE2_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_PHYSICAL_CHANNEL_RECONFIGURATION_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_PHYSICAL_SHARED_CHANNEL_ALLOCATION_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_RADIO_BEARER_RECONFIGURATION_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_RADIO_BEARER_RELEASE_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_RADIO_BEARER_SETUP_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_RRC_CONNECTION_RELEASE_MSG,
	RRC_SECURITY_MODE_COMMAND_MSG,
	RRC_SIGNALLING_CONNECTION_RELEASE_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_TRANSPORT_CHANNEL_RECONFIGURATION_MSG,
	RRC_TRANSPORT_FORMAT_COMBINATION_CONTROL_MSG,
	RRC_UE_CAPABILITY_ENQUIRY_MSG,
	RRC_UE_CAPABILITY_INFORMATION_CONFIRM_MSG,
	RRC_UPLINK_PHYSICAL_CHANNEL_CONTROL_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_URA_UPDATE_CONFIRM_MSG,	
	RRC_UTRAN_MOBILITY_IN_FORMATION_MSG,
	RRC_HANDOVER_FROM_UTRAN_COMMAND_GERANIU_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_MBMS_MODIFIED_SERVICES_INFORMATION_MSG,
	RRC_ETWSPRIMARY_NOTIFICATION_WITH_SECURITY,	
	RRC_HANDOVER_FROM_UTRANCMD_EUTRA,		
	RRC_ACTIVE_SET_UPDATE_COMPLETE_MSG,
	RRC_ACTIVE_SET_UPDATE_FAILURE_MSG,
	RRC_CELL_CHANGE_ORDER_FROM_UTRAN_FAILURE_MSG,
	RRC_COUNTER_CHECK_RESPONSE_MSG,
	RRC_HANDOVER_TO_UTRAN_COMPLETE_MSG, 
	RRC_INITIAL_DIRECT_TRANSFER_MSG,
	RRC_HANDOVER_FROM_UTRAN_FAILURE_MSG,
	RRC_MEASUREMENT_CONTROL_FAILURE_MSG,
	RRC_MEASUREMENT_REPORT_MSG,
	RRC_PHYSICAL_CHANNEL_RECONFIGURATION_COMPLETE_MSG,
	RRC_PHYSICAL_CHANNEL_RECONFIGURATION_FAILURE_MSG,
	RRC_RADIO_BEARER_RECONFIGURATION_COMPLETE_MSG,
	RRC_RADIO_BEARER_RECONFIGURATION_FAILURE_MSG,
	RRC_RADIO_BEARER_RELEASE_COMPLETE_MSG,
	RRC_RADIO_BEARER_RELEASE_FAILURE_MSG,
	RRC_RADIO_BEARER_SETUP_COMPLETE_MSG,
	RRC_RADIO_BEARER_SETUP_FAILURE_MSG,
	RRC_RRC_CONNECTION_RELEASE_COMPLETE_MSG, 
	RRC_RRC_CONNECTION_SETUP_COMPLETE_MSG, 
	RRC_RRC_STATUS_MSG, 
	RRC_SECURITY_MODE_COMPLETE_MSG,
	RRC_SECURITY_MODE_FAILURE_MSG,
	RRC_SIGNALLING_CONNECTION_RELEASE_INDICATION_MSG,
	RRC_TRANSPORT_CHANNEL_RECONFIGURATION_COMPLETE_MSG, 
	RRC_TRANSPORT_CHANNEL_RECONFIGURATION_FAILURE_MSG, 
	RRC_TRANSPORT_FORMAT_COMBINATION_CONTROL_FAILURE_MSG, 
	RRC_UE_CAPABILITY_INFORMATION_MSG, 
	RRC_UPLINK_DIRECT_TRANSFER_MSG, 
	RRC_UTRAN_MOBILITY_INFORMATION_CONFIRM_MSG,
	RRC_UTRAN_MOBILITY_INFORMATION_FAILURE_MSG, 
	RRC_MBMS_MODIFICATION_REQUEST_MSG,
	RRC_DL_CCCH_MESSAGE_TYPE_CELL_UPDATE_CONFIRM_MSG, 
	RRC_RRC_CONNECTION_REJECT_MSG ,
	RRC_DL_CCCH_MESSAGE_TYPE_RRC_CONNECTION_RELEASE_MSG, 
	RRC_RRC_CONNECTION_SETUP_MSG, 
	RRC_DL_CCCH_MESSAGE_TYPE_URA_UPDATE_CONFIRM_MSG,
	RRC_ETWS_PRIMARY_NOTIFICATIONWITH_SECURITY,
	RRC_CELL_UPDATE_MSG, 
	RRC_RRC_CONNECTION_REQUEST_MSG, 
	RRC_URA_UPDATE_MSG,
	RRC_PAGING_TYPE_1_MSG,
	RRC_DL_SHCCH_MESSAGE_TYPE_PHYSICAL_SHARED_CHANNEL_ALLOCATION_MSG,
	RRC_PUSCH_CAPACITY_REQUEST_MSG, 
	RRC_SYSTEM_INFORMATION_MSG ,
	RRC_SYSTEM_INFORMATION_CHANGE_INDICATION_MSG,
	RRC_SYS_INFMTION_BCH_MSG,
	RRC_MBMS_ACCESS_INFORMATION_MSG, 
	RRC_MBMS_COMMON_PTMRB_INFORMATION_MSG,
	RRC_MBMS_CURRENT_CELL_PTMRB_INFORMATION_MSG, 
	RRC_MBMS_GENERAL_INFORMATION_MSG,
	RRC_MCCH_MESSAGE_TYPE_MBMS_MODIFIED_SERVICES_INFORMATION_MSG, 
	RRC_MBMS_NEIGHBOURING_CELL_PTMRB_INFORMATION_MSG, 
	RRC_MBMS_UNMODIFIED_SERVICES_INFORMATION_MSG,
	RRC_MBMS_SCHEDULING_INFORMATION_MSG,
	RRC_SYS_INFO_TYPE_1_MSG,
	RRC_SYS_INFO_TYPE_2_MSG,
	RRC_SYS_INFO_TYPE_3_MSG,
	RRC_SYS_INFO_TYPE_4_MSG,
	RRC_SYS_INFO_TYPE_5_MSG,
	RRC_SYS_INFO_TYPE_6_MSG,
	RRC_SYS_INFO_TYPE_7_MSG,
	RRC_SYS_INFO_TYPE_8_MSG,
	RRC_SYS_INFO_TYPE_9_MSG,
	RRC_SYS_INFO_TYPE_10_MSG,
	RRC_SYS_INFO_TYPE_11_MSG,
	RRC_SYS_INFO_TYPE_11BIS_MSG,
	RRC_SYS_INFO_TYPE_12_MSG,
	RRC_SYS_INFO_TYPE_13_MSG,
	RRC_SYS_INFO_TYPE_13_1_MSG,
	RRC_SYS_INFO_TYPE_13_2_MSG,
	RRC_SYS_INFO_TYPE_13_3_MSG, 
	RRC_SYS_INFO_TYPE_13_4_MSG,
	RRC_SYS_INFO_TYPE_14_MSG,
	RRC_SYS_INFO_TYPE_15_MSG,
	RRC_SYS_INFO_TYPE_15BIS_MSG,
	RRC_SYS_INFO_TYPE_15_1_MSG,
	RRC_SYS_INFO_TYPE_15_1BIS_MSG,
	RRC_SYS_INFO_TYPE_15_2_MSG,
	RRC_SYS_INFO_TYPE_15_2BIS_MSG,
	RRC_SYS_INFO_TYPE_15_2TER_MSG,
	RRC_SYS_INFO_TYPE_15_3_MSG,
	RRC_SYS_INFO_TYPE_15_3BIS_MSG,
	RRC_SYS_INFO_TYPE_15_4_MSG,
	RRC_SYS_INFO_TYPE_15_5_MSG,
	RRC_SYS_INFO_TYPE_15_6_MSG,
	RRC_SYS_INFO_TYPE_15_7_MSG,
	RRC_SYS_INFO_TYPE_16_MSG    
}rrc_asn_msg_type_et;


static U32_t dl_dcch_msg_type_table[] =
{
	0,	/*	never come at this entry*/ 
	RRC_ACTIVE_SET_UPDATE_MSG,
	RRC_ASSISTANCE_DATA_DELIVERY_MSG,
	RRC_CELL_CHANGE_ORDER_FROM_UTRAN_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_CELL_UPDATE_CONFIRM_MSG,
	RRC_COUNTER_CHECK_MSG,
	RRC_DOWNLINK_DIRECT_TRANSFER_MSG,
	RRC_HANDOVER_FROM_UTRAN_COMMAND_GSM_MSG,
	RRC_HANDOVER_FROM_UTRAN_COMMAND_CDMA2000_MSG,
	RRC_MEASUREMENT_CONTROL_MSG,
	RRC_PAGING_TYPE2_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_PHYSICAL_CHANNEL_RECONFIGURATION_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_PHYSICAL_SHARED_CHANNEL_ALLOCATION_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_RADIO_BEARER_RECONFIGURATION_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_RADIO_BEARER_RELEASE_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_RADIO_BEARER_SETUP_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_RRC_CONNECTION_RELEASE_MSG,
	RRC_SECURITY_MODE_COMMAND_MSG,
	RRC_SIGNALLING_CONNECTION_RELEASE_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_TRANSPORT_CHANNEL_RECONFIGURATION_MSG,
	RRC_TRANSPORT_FORMAT_COMBINATION_CONTROL_MSG,
	RRC_UE_CAPABILITY_ENQUIRY_MSG,
	RRC_UE_CAPABILITY_INFORMATION_CONFIRM_MSG,
	RRC_UPLINK_PHYSICAL_CHANNEL_CONTROL_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_URA_UPDATE_CONFIRM_MSG,	
	RRC_UTRAN_MOBILITY_IN_FORMATION_MSG,
	RRC_HANDOVER_FROM_UTRAN_COMMAND_GERANIU_MSG,
	RRC_DL_DCCH_MESSAGE_TYPE_MBMS_MODIFIED_SERVICES_INFORMATION_MSG,
	RRC_ETWSPRIMARY_NOTIFICATION_WITH_SECURITY,
	RRC_HANDOVER_FROM_UTRANCMD_EUTRA
};


static U32_t ul_dcch_msg_type_table[] = 
{
	0,	/*	never come at this entry*/
	RRC_ACTIVE_SET_UPDATE_COMPLETE_MSG,
	RRC_ACTIVE_SET_UPDATE_FAILURE_MSG,
	RRC_CELL_CHANGE_ORDER_FROM_UTRAN_FAILURE_MSG,
	RRC_COUNTER_CHECK_RESPONSE_MSG,
	RRC_HANDOVER_TO_UTRAN_COMPLETE_MSG, 
	RRC_INITIAL_DIRECT_TRANSFER_MSG,
	RRC_HANDOVER_FROM_UTRAN_FAILURE_MSG,
	RRC_MEASUREMENT_CONTROL_FAILURE_MSG,
	RRC_MEASUREMENT_REPORT_MSG,
	RRC_PHYSICAL_CHANNEL_RECONFIGURATION_COMPLETE_MSG,
	RRC_PHYSICAL_CHANNEL_RECONFIGURATION_FAILURE_MSG,
	RRC_RADIO_BEARER_RECONFIGURATION_COMPLETE_MSG,
	RRC_RADIO_BEARER_RECONFIGURATION_FAILURE_MSG,
	RRC_RADIO_BEARER_RELEASE_COMPLETE_MSG,
	RRC_RADIO_BEARER_RELEASE_FAILURE_MSG,
	RRC_RADIO_BEARER_SETUP_COMPLETE_MSG,
	RRC_RADIO_BEARER_SETUP_FAILURE_MSG,
	RRC_RRC_CONNECTION_RELEASE_COMPLETE_MSG, 
	RRC_RRC_CONNECTION_SETUP_COMPLETE_MSG, 
	RRC_RRC_STATUS_MSG, 
	RRC_SECURITY_MODE_COMPLETE_MSG,
	RRC_SECURITY_MODE_FAILURE_MSG,
	RRC_SIGNALLING_CONNECTION_RELEASE_INDICATION_MSG,
	RRC_TRANSPORT_CHANNEL_RECONFIGURATION_COMPLETE_MSG, 
	RRC_TRANSPORT_CHANNEL_RECONFIGURATION_FAILURE_MSG, 
	RRC_TRANSPORT_FORMAT_COMBINATION_CONTROL_FAILURE_MSG, 
	RRC_UE_CAPABILITY_INFORMATION_MSG, 
	RRC_UPLINK_DIRECT_TRANSFER_MSG, 
	RRC_UTRAN_MOBILITY_INFORMATION_CONFIRM_MSG,
	RRC_UTRAN_MOBILITY_INFORMATION_FAILURE_MSG, 
	RRC_MBMS_MODIFICATION_REQUEST_MSG
};

static U32_t dl_ccch_msg_type_table[] = 
{
	0,	/*	never come at this entry*/
	RRC_DL_CCCH_MESSAGE_TYPE_CELL_UPDATE_CONFIRM_MSG, 
	RRC_RRC_CONNECTION_REJECT_MSG ,
	RRC_DL_CCCH_MESSAGE_TYPE_RRC_CONNECTION_RELEASE_MSG, 
	RRC_RRC_CONNECTION_SETUP_MSG, 
	RRC_DL_CCCH_MESSAGE_TYPE_URA_UPDATE_CONFIRM_MSG,
	RRC_ETWS_PRIMARY_NOTIFICATIONWITH_SECURITY
};


static U32_t ul_ccch_msg_type_table[] = 
{
	0,	/*	never come at this entry*/
	RRC_CELL_UPDATE_MSG, 
	RRC_RRC_CONNECTION_REQUEST_MSG, 
	RRC_URA_UPDATE_MSG
};



static U32_t pcch_msg_type_table[] = 
{
	0,	/*	never come at this entry*/
	RRC_PAGING_TYPE_1_MSG
}; 	

static U32_t mcch_msg_type_table[] =
{
	0,	/*	never come at this entry*/
	RRC_MBMS_ACCESS_INFORMATION_MSG, 
	RRC_MBMS_COMMON_PTMRB_INFORMATION_MSG,
	RRC_MBMS_CURRENT_CELL_PTMRB_INFORMATION_MSG, 
	RRC_MBMS_GENERAL_INFORMATION_MSG,
	RRC_MCCH_MESSAGE_TYPE_MBMS_MODIFIED_SERVICES_INFORMATION_MSG, 
	RRC_MBMS_NEIGHBOURING_CELL_PTMRB_INFORMATION_MSG, 
	RRC_MBMS_UNMODIFIED_SERVICES_INFORMATION_MSG
};


static U32_t msch_msg_type_table[] = 
{
	0,	/*	never come at this entry*/
	RRC_MBMS_SCHEDULING_INFORMATION_MSG
};	

typedef enum{
	RRC_FAILURE = 0,
	RRC_SUCCESS
}rrc_result_et;

typedef struct{
	void *p_mem_array[20];
	U32_t mem_count;
}rrc_self_info_t;




void* rrc_mem_get ( U32_t num_byte, rrc_self_info_t *p_self_info )
{
	void *p_mem_temp = NULL;

	if(NULL != (p_mem_temp = (void *)MALLOC(num_byte)))
	{
		p_self_info->p_mem_array[p_self_info->mem_count++] = p_mem_temp;
	}

	return p_mem_temp;
}

void rrc_mem_free ( rrc_self_info_t *p_self_info )
{
	U32_t i = 0;

	for (i = 0; i < p_self_info->mem_count; i++)
	{
		if(NULL != p_self_info->p_mem_array[i])
		{
			FREE(p_self_info->p_mem_array[i]);
		}
	}

	if(NULL != p_self_info)
	{
		FREE(p_self_info);
	}
}

int decodeRRC(void *p_encoded_data,
			U32_t                  encoded_buflen,
			rrc_logical_channel_type_et logical_channel,
			U32_t Id)/*Id = 0 will be recieved here always*/
{
	UL_CCCH_Message rrc_ul_ccch_pdu;
	UL_CCCH_Message *p_rrc_ul_ccch_pdu;

	UL_DCCH_Message rrc_ul_dcch_pdu;
	UL_DCCH_Message *p_rrc_ul_dcch_pdu;

	DL_CCCH_Message rrc_dl_ccch_pdu;
	DL_CCCH_Message *p_rrc_dl_ccch_pdu;

	DL_DCCH_Message rrc_dl_dcch_pdu;
	DL_DCCH_Message *p_rrc_dl_dcch_pdu;

	BCCH_BCH_Message rrc_bcch_bch_pdu;
	BCCH_BCH_Message *p_rrc_bcch_bch_pdu;

	PCCH_Message rrc_pcch_pdu;
	PCCH_Message *p_rrc_pcch_pdu;

	MCCH_Message rrc_mcch_pdu;
	MCCH_Message *p_rrc_mcch_pdu;

	MSCH_Message rrc_msch_pdu;
	MSCH_Message *p_rrc_msch_pdu;

	DL_SHCCH_Message rrc_dl_shcch_pdu;
	DL_SHCCH_Message *p_rrc_dl_shcch_pdu;

	UL_SHCCH_Message rrc_ul_shcch_pdu;
	UL_SHCCH_Message *p_rrc_ul_shcch_pdu;

	BCCH_FACH_Message rrc_bcch_fach_pdu ;
	BCCH_FACH_Message *p_rrc_bcch_fach_pdu;

	ismt = 0;

	OSCTXT *p_lctxt = NULL;
	U8_t ecode = 0, temp_char = 0;
	U32_t msg_id = 0, *p_msg_id = NULL, i = 0, num_bytes = 0, bits_pending = 0;

	OSRTDListNode *head = NULL;		
	
	RAB_InformationSetup_r5 *p_temp_rab_informationsetup_r5 = NULL;
	RB_InformationSetup_r5 *p_temp_rb_informationsetup_r5 = NULL;
	RB_MappingOption_r5 *p_temp_rb_mappingoption_r5 = NULL;
	DL_LogicalChannelMapping_r5 *p_temp_dl_logicalchannlmapping_r5 = NULL;

	BLER_MeasurementResults *p_temp_msmtresults = NULL;

	p_msg_id = &msg_id;

	rrc_self_info_t *p_self_info = (rrc_self_info_t *)MALLOC(sizeof(rrc_self_info_t));
	p_self_info->mem_count = 0;

	p_rrc_ul_ccch_pdu = &rrc_ul_ccch_pdu;
	p_rrc_ul_dcch_pdu = &rrc_ul_dcch_pdu;
	p_rrc_dl_ccch_pdu = &rrc_dl_ccch_pdu;
	p_rrc_dl_dcch_pdu = &rrc_dl_dcch_pdu;
	p_rrc_bcch_bch_pdu = &rrc_bcch_bch_pdu;
	p_rrc_pcch_pdu = &rrc_pcch_pdu;
	p_rrc_mcch_pdu = &rrc_mcch_pdu;
	p_rrc_msch_pdu = &rrc_msch_pdu;
	p_rrc_dl_shcch_pdu = &rrc_dl_shcch_pdu;
	p_rrc_ul_shcch_pdu = &rrc_ul_shcch_pdu;
	p_rrc_bcch_fach_pdu = &rrc_bcch_fach_pdu;

	if(NULL == p_msg_id)
	{
		//WARNING_LOG("Unable to Allocate memory for MSG ID \n");
		return 0;
	}

	p_lctxt = (OSCTXT *)rrc_mem_get(sizeof(OSCTXT), p_self_info);

	rtInitContext (p_lctxt);  
	pu_setTrace(p_lctxt, 0);
	pu_setBuffer(p_lctxt,p_encoded_data,encoded_buflen,0);/*To set buffer pointer for decoding*/

	switch (logical_channel)/*Based on the logical channel on which pdu is recieved, call the respective objsys function for decoding*/
	{	
		case RRC_UL_CCCH:
			{
				ecode = asn1PD_UL_CCCH_Message(p_lctxt, &rrc_ul_ccch_pdu);
				break;
			}
		case RRC_UL_DCCH:
			{
				ecode = asn1PD_UL_DCCH_Message(p_lctxt, &rrc_ul_dcch_pdu);
				break;
			}
		case RRC_DL_CCCH:
			{
				ecode = asn1PD_DL_CCCH_Message(p_lctxt, &rrc_dl_ccch_pdu);
				break;
			}
		case RRC_DL_DCCH:
			{
				ecode = asn1PD_DL_DCCH_Message(p_lctxt, &rrc_dl_dcch_pdu);
				break;
			}
		case RRC_BCCH_BCH:
			{
				ecode = asn1PD_BCCH_BCH_Message(p_lctxt, &rrc_bcch_bch_pdu);

				break;
			}
		case RRC_PCCH:
			{
				ecode = asn1PD_PCCH_Message(p_lctxt, &rrc_pcch_pdu);
				break;
			}
		case RRC_MCCH:
			{
				ecode = asn1PD_MCCH_Message(p_lctxt, &rrc_mcch_pdu);
				break;
			}
		case RRC_MSCH:
			{
				ecode = asn1PD_MSCH_Message(p_lctxt, &rrc_msch_pdu);
				break;
			}
		default:
			{
				//d_log("\nInvalid Logical Channel type = %d recieved, Unable to decode\n", logical_channel);
				return 0;
			}
	}

	if(0 != ecode)
	{
		/*rrc_mem_free( p_self_info );*/
		printf("\nObjsys decoding failed");
		/*return ;*/
	}

	//char *decodeMsgBuffer = malloc(sizeof(char)*60000);
	//memset(decodeMsgBuffer, 0, sizeof(char)*60000);
	//memset(decodeMsgBuffer, 0, 60000);
	//rtxSetPrintStream(p_lctxt,writeToBuffer, decodeMsgBuffer);


	switch (logical_channel)/*Extract the message-id from the pdu decoded*/
	{
		case RRC_UL_CCCH:
			{
				*p_msg_id = ul_ccch_msg_type_table[p_rrc_ul_ccch_pdu->message.t];
				if(0 == ecode)
				{
					if(rrc_ul_ccch_pdu.message.t == 2) {
						if(rrc_ul_ccch_pdu.message.u.rrcConnectionRequest->establishmentCause == terminatingConversationalCall) {
							ismt = 1;
						}
					}
					//asn1PrtToStrm_UL_CCCH_Message(p_lctxt,"UL_CCCH_Message", p_rrc_ul_ccch_pdu);
				}else
				{
					//WARNING_LOG("\nWCDMA_UL_CCCH");
					//WARNING_LOG("\n WCDMA RRC Decoding failed , Channel =  \n", logical_channel);
					//d_arraylog(p_encoded_data,encoded_buflen);
				}
				break;
			}	
		case RRC_UL_DCCH:
			{
				*p_msg_id = ul_dcch_msg_type_table[p_rrc_ul_dcch_pdu->message.t];
				if(0 == ecode)
				{
					//asn1PrtToStrm_UL_DCCH_Message(p_lctxt,"UL_DCCH_Message", p_rrc_ul_dcch_pdu);
				}else{
						//WARNING_LOG("\nWCDMA_UL_DCCH");
						//WARNING_LOG("\n WCDMA RRC Decoding failed , Channel =  \n", logical_channel);
					    //d_arraylog(p_encoded_data,encoded_buflen);
				}
				break;
			}
		case RRC_DL_CCCH:
			{
				*p_msg_id = dl_ccch_msg_type_table[p_rrc_dl_ccch_pdu->message.t];
				if(0 == ecode)
				{
					//asn1PrtToStrm_DL_CCCH_Message(p_lctxt,"DL_CCCH_Message", p_rrc_dl_ccch_pdu);
				}else{
					//WARNING_LOG("\nWCDMA_UL_CCCH");
					//WARNING_LOG("\n WCDMA RRC Decoding failed , Channel =  \n", logical_channel);
					//d_arraylog(p_encoded_data,encoded_buflen);
				}
				break;
			}
		case RRC_DL_DCCH:
			{
				*p_msg_id = dl_dcch_msg_type_table[p_rrc_dl_dcch_pdu->message.t];
				if(0 == ecode )
				{
					if(p_rrc_dl_dcch_pdu->message.t == 10) {
						if(p_rrc_dl_dcch_pdu->message.u.pagingType2->pagingCause == terminatingConversationalCall_1) {
							ismt = 1;
						}
					}
					//asn1PrtToStrm_DL_DCCH_Message(p_lctxt,"DL_DCCH_Message", p_rrc_dl_dcch_pdu);
				}else{
					 //WARNING_LOG("\nWCDMA_DL_DCCH");
					 //WARNING_LOG("\n WCDMA RRC Decoding failed , Channel =  \n", logical_channel);
					 //d_arraylog(p_encoded_data,encoded_buflen);
				}
				break;
			}
		case RRC_BCCH_BCH:
			{
				*p_msg_id = RRC_SYS_INFMTION_BCH_MSG;
				if(0 == ecode)
				{
					//asn1PrtToStrm_BCCH_BCH_Message(p_lctxt,"BCCH_BCH_Message", p_rrc_bcch_bch_pdu);
				}else{
					 //WARNING_LOG("\nWCDMA_BCCH_BCH");
					 //WARNING_LOG("\n WCDMA RRC Decoding failed , Channel =  \n", logical_channel);
					 //d_arraylog(p_encoded_data,encoded_buflen);
				}
				break;
			}
		case RRC_PCCH:
			{
				*p_msg_id = pcch_msg_type_table[p_rrc_pcch_pdu->message.t];
				if(0 == ecode)
				{
					//asn1PrtToStrm_PCCH_Message(p_lctxt,"PCCH_Message", p_rrc_pcch_pdu );
				}else{
					 //WARNING_LOG("\nWCDMA_RRC_PCCH");
					 //WARNING_LOG("\n WCDMA RRC Decoding failed , Channel =  \n", logical_channel);
					 //d_arraylog(p_encoded_data,encoded_buflen);
				}
				break;
			}
		case RRC_MCCH:
			{
				*p_msg_id = mcch_msg_type_table[p_rrc_mcch_pdu->message.t];
				if(0 == ecode)
				{
					//asn1PrtToStrm_MCCH_Message(p_lctxt,"MCCH_Message", p_rrc_mcch_pdu);
				}else{
					 //WARNING_LOG("\nWCDMA_RRC_MCCH");
					 //WARNING_LOG("\n WCDMA RRC Decoding failed , Channel =  \n", logical_channel);
					 //d_arraylog(p_encoded_data,encoded_buflen);
				}
				break;
			}
		case RRC_MSCH:
			{
				*p_msg_id = msch_msg_type_table[p_rrc_msch_pdu->message.t];
				if(0 == ecode)
				{
					//asn1PrtToStrm_MSCH_Message(p_lctxt,"MSCH_Message", p_rrc_msch_pdu);
				}else{
					//WARNING_LOG("\nWCDMA_RRC_MSCH");
					//WARNING_LOG("\n WCDMA RRC Decoding failed , Channel =  \n", logical_channel);
					//d_arraylog(p_encoded_data,encoded_buflen);
				}
				break;
			}

		default:
			break;
	}
	/* Decoding failed ,Free memory and return*/
	if(0 != ecode)
	{
		rtxFreeContext(p_lctxt);
		rrc_mem_free( p_self_info );
		return 0;
	}
	rtxFreeContext(p_lctxt);
	rrc_mem_free( p_self_info );
	return ismt;
}



/*
 * ===  FUNCTION  ======================================================================
 *         Name:  WCDMADecoderCCevents
 *
 *    Arguments:  1. p_encoded_data : Pointer to encoded buffer
 *	  	  2. encoded_buflen : Length of encoded buffer received in p_encoded_data.
 * 		  3. logical_channel: Logical channel over which the the encoded buffer was received
 * 		  4. cmdmillSeconds : time stamp from command
 *
 *  Description:  1. Checks for presence of optional ie & if NULL is received under p_msg_name, allocates memory to it
 * 		  2. Based on the logical channel over which message is received specific macros are called to fetch message-id from the encoded buffer.
 *
 * =====================================================================================
 */
void WCDMADecoderCCevents(char *p_encoded_data,
					 int  encoded_buflen,
					 rrc_logical_channel_type_et logical_channel,
					 u_int64_t cmdmillSeconds)
{
	/* 1-Start */
	int opt_ie_flag = WAVEMSGANALYSER_CHECK_OPT_IE(p_encoded_data);
	char msg_table_input = 0;
	int ismt = 1;
	memset(eventbuff, 0, sizeof(eventbuff));
	/* 1-End */
	/* 2-Start */
	switch (logical_channel)
	{
		case 	RRC_UL_CCCH :
			{
				if (!opt_ie_flag)
				{
					WAVEMSGANALYSER_FETCH_MSG_ID(msg_table_input, p_encoded_data, 2);/*2,3 bits of 1st byte*/
				}
				else/*Optional IE:- IntegrityCheckInfo present*/
				{
					WAVEMSGANALYSER_FETCH_MSG_ID_OPTPRESENT(msg_table_input, p_encoded_data, 2);/*5th byte: 6, 7 bit*/
				}
				//strncpy(*p_msg_name, ValueToSring[ul_ccch_msg_type_table[msg_table_input + 1]], strlen(ValueToSring[ul_ccch_msg_type_table[msg_table_input + 1]]));

				if(ul_ccch_msg_type_table[msg_table_input + 1] == RRC_RRC_CONNECTION_REQUEST_MSG) {
					if(decodeRRC(p_encoded_data, encoded_buflen, logical_channel, 0) == 1) {
						//mt call
						memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
						memcpy(eventbuff + sizeof(u_int64_t), (unsigned char *)& ismt,sizeof(int32_t));
						//printf("rrc connection request : %d\n\n", ismt);
						sendInformation(WCDMA, EVENT_CC, CALL_START_INDICATION, eventbuff, sizeof(u_int64_t) + sizeof(int32_t));
					}
				}

				break;
			}
		case 	RRC_UL_DCCH :
			{
				if (!opt_ie_flag)
				{
					WAVEMSGANALYSER_FETCH_MSG_ID(msg_table_input, p_encoded_data, 5);/*2,3,4,5,6 bits of 1st byte*/
				}
				else/*Optional IE:- IntegrityCheckInfo present*/
				{
					WAVEMSGANALYSER_FETCH_MSG_ID_OPTPRESENT(msg_table_input, p_encoded_data, 5); /*5th byte: 6, 7, 8 bit & 6th byte: 1, 2 bit*/
				}
				//strncpy(*p_msg_name, ValueToSring[ul_dcch_msg_type_table[msg_table_input + 1]], strlen(ValueToSring[ul_dcch_msg_type_table[msg_table_input + 1]]));
				break;
			}
		case 	RRC_DL_CCCH :
			{
				if (!opt_ie_flag)
				{
					WAVEMSGANALYSER_FETCH_MSG_ID(msg_table_input, p_encoded_data, 3); /*2,3,4 bits of 1st byte*/
				}
				else/*Optional IE:- IntegrityCheckInfo present*/
				{
					WAVEMSGANALYSER_FETCH_MSG_ID_OPTPRESENT(msg_table_input, p_encoded_data, 3); /*5th byte: 6, 7, 8 bit*/
				}
				//strncpy(*p_msg_name, ValueToSring[dl_ccch_msg_type_table[msg_table_input + 1]], strlen(ValueToSring[dl_ccch_msg_type_table[msg_table_input + 1]]));
				break;
			}
		case 	RRC_DL_DCCH :
			{
				if (!opt_ie_flag)
				{
					WAVEMSGANALYSER_FETCH_MSG_ID(msg_table_input, p_encoded_data, 5);/*2,3,4,5,6 bits of 1st byte*/
				}
				else/*Optional IE:- IntegrityCheckInfo present*/
				{
					WAVEMSGANALYSER_FETCH_MSG_ID_OPTPRESENT(msg_table_input, p_encoded_data, 5); /*5th byte: 6, 7, 8 bit & 6th byte: 1, 2 bit*/
				}
				//strncpy(*p_msg_name, ValueToSring[dl_dcch_msg_type_table[msg_table_input + 1]], strlen(ValueToSring[dl_dcch_msg_type_table[msg_table_input + 1]]));

				if(dl_dcch_msg_type_table[msg_table_input + 1] == RRC_PAGING_TYPE2_MSG) {
					if(decodeRRC(p_encoded_data, encoded_buflen, logical_channel, 0) == 1) {
						//mt call
						memcpy(eventbuff, (unsigned char *)&cmdmillSeconds, sizeof(u_int64_t));
						memcpy(eventbuff + sizeof(u_int64_t), (unsigned char *)& ismt,sizeof(int32_t));
						//printf("paging type 2 : %d\n\n", ismt);
						sendInformation(WCDMA, EVENT_CC, CALL_START_INDICATION, eventbuff, sizeof(u_int64_t) + sizeof(int32_t));
					}
				}
				break;
			}
		case 	RRC_PCCH :
			{
				//strncpy(*p_msg_name, "RRC_PAGING_TYPE_1_MSG", strlen("RRC_PAGING_TYPE_1_MSG"));
				break;
			}
	}
	/* 2-End */
}

