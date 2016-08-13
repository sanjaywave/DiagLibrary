#ifndef INTERFACEAPI_H
#define	INTERFACEAPI_H

#include <stdbool.h>
#include <sys/types.h>

#define MAX_GSM_NEIGH_CELL 6
#define MAX_GSM_NEIG_SET 32

#define MAX_WCDMA_NEIGH_CELL 32
#define MAX_WCDMA_ACTIVE_SET 32
#define MAX_WCDMA_NEIG_SET 32


// Enum for manufacture
enum
{
  HTC,
  SAMSUNG
};
// Enum for systype
enum
{
  GSM=1,
  WCDMA,
  LTE,
  CDMA

};

// Enum for Lte Info
enum{
		DISABLE = 0,
		ENABLE = 1
};
// Enum for Notifier Type
enum
{
  LOGGING_DATA = 1,
  EVENT_DATA
};


enum
{
  LOG_GSM_DISABLE = 0x00000000,// Disable all logging

  LOG_GSM_ENABLE_ALL = 0xFFFFFFFF,

  LOG_GSM_RR_STATE_MESSAGES =0x00000001,

  LOG_GSM_CELL_INFORMATIOM = 0x00000002,

  LOG_GSM_CALL_INFORMATIOM = 0x00000004,

  LOG_GRR_PACKET_SYSTEM_INFORMATION = 0x00000008,

  LOG_GPRS_L2_INFORMATION = 0x00000010,

  LOG_GSM_RACH_INFORMATION = 0x00000020,

  LOG_GSM_PAGING_INFORMATION = 0x00000040,

  LOG_GSM_RR_Message = 0x00000080,

  LOG_GSM_UMTS_INFORMATION = 0x00000100

};

//Identifier for Logging WCDMA
enum
{
  LOG_WCDMA_DISABLE = 0x00000000,// Disable all logging

  LOG_WCDMA_ENABLE_ALL=0xFFFFFFFF,
        
  LOG_WCDMA_RRC_MESSAGES = 0x00000001,

  LOG_WCDMA_RRC_STATE_MESSAGES = 0x00000002,
   
  LOG_WCDMA_CELL_INFORMATION = 0x00000004,

  LOG_WCDMA_ACTIVE_AND_NEIGBOURING_INFORMATION = 0x00000008,

  LOG_WCDMA_L2_INFORMATION = 0x000000010,

  LOG_WCDMA_BLER_REPORT = 0x000000020,

  LOG_WCDMA_RACH_INFORMATION = 0x000000040,

  LOG_WCDMA_UMTS_INFORMATION = 0x00000080,

  LOG_WCDMA_SIR_LOGING = 0x00000100

};
/* The logging of messages are in different categories */


//Identifier for Logging LTE
enum
{
  LOG_LTE_DISABLE = 0x00000000,// Disable all logging

  LOG_LTE_ENABLE_ALL=0xFFFFFFFF,

  LOG_LTE_RRC_MESSAGES = 0x00000001,

  LOG_LTE_CELL_INFORMATION = 0x00000002,

  LOG_LTE_EMM_INFORMATION = 0x00000004,

  LOG_LTE_PHYSICAL_LAYER = 0x00000008,

  LOG_LTE_SESSION_INFORMATION = 0x00000010,

  LOG_LTE_RACH_INFORMATION = 0x00000020,

  LOG_LTE_MOBILITY_INFORMATION = 0x00000040,

  LOG_LTE_RLC_INFORMATION = 0x00000080,

  LOG_LTE_MAC_INFORMATION = 0x00000100,

  LOG_LTE_PDCP_INFORMATION = 0x00000200,

};
//Identifier for Events
enum
{
  EVENT_DISABLE = 0x00000000, // Disable all events

  EVENT_RRC_STATE = 0x00000001,/*571 */

  EVENT_MM_STATE  = 0x00000002,/*1022 */

  EVENT_GMM_STATE = 0x00000004, /*1023 */

  EVENT_InterRat_Handover_Start = 0x00000008,/*447 */

  EVENT_InterRat_Handover_End = 0x00000010,/*448 */

  EVENT_CallDrop = 0x00000020,/*570 */
      
  EVENT_RB_Setup_Failure = 0x00000040,/*627 */
  
  EVENT_RRC_Phy_Channel_Failure = 0x00000080,/*628 */

  EVENT_RRC_WCDMA_Rat_Change = 0x00000100,/*1105*/

  EVENT_RRC_TIMER_EXPIRY = 0x00000200,/*1541*/
  
  EVENT_CELL_SELECTION_FAILURE = 0x00000400,/*1548*/
        
  EVENT_LTE_RRC_STATE_CHANGE = 0x00000800,

  EVENT_LTE_INTERAT_RESEL_START = 0x00001000,
        
  EVENT_LTE_INTERAT_RESEL_END = 0x00002000,
        
  EVENT_LTE_INTERAT_REDIR_START = 0x00004000,
        
  EVENT_LTE_INTERAT_REDIR_END = 0x00008000,

  EVENT_SYSTEM_BLOCK = 0x00010000,  /*System Blocking event*/

  EVENT_LTE_HANDOVER_FAILURE = 0x00020000,

  EVENT_LTE_MOBILITY_E_UTRA_STARTED = 0x00040000,

  EVENT_LTE_MOBILITY_E_UTRA_FAILED = 0x00080000,

  EVENT_IRAT_TECHNOLOGY_CHANGE = 0x00100000,

  EVENT_LTE_NEW_CELL_SELECTION = 0x00200000,

  EVENT_LTE_OUT_OF_SERVICE = 0x00400000,

  EVENT_LTE_RADIO_LINK_FAILURE = 0x00800000,

  EVENT_LTE_RADIO_LINK_FAILURE_STAT = 0x01000000,

  EVENT_CC = 0x02000000, /* for CC */

};

enum {

    E_UTRA_OPERATING_BAND_1 = 0x00000001,

    E_UTRA_OPERATING_BAND_2 = 0x00000002,

    E_UTRA_OPERATING_BAND_3 = 0x00000004,

    E_UTRA_OPERATING_BAND_4 = 0x00000008,

    E_UTRA_OPERATING_BAND_5 = 0x00000010,

    E_UTRA_OPERATING_BAND_6 = 0x00000020,

    E_UTRA_OPERATING_BAND_7 = 0x00000040,

    E_UTRA_OPERATING_BAND_8 = 0x00000080,

    E_UTRA_OPERATING_BAND_9 = 0x00000100,

    E_UTRA_OPERATING_BAND_10 = 0x00000200,

    E_UTRA_OPERATING_BAND_11 = 0x00000400,

    E_UTRA_OPERATING_BAND_12 = 0x00000800,

    E_UTRA_OPERATING_BAND_13 = 0x00001000,

    E_UTRA_OPERATING_BAND_14 = 0x00002000,

    E_UTRA_OPERATING_BAND_17 = 0x00010000,

    E_UTRA_OPERATING_BAND_18 = 0x00020000,

    E_UTRA_OPERATING_BAND_19 = 0x00040000,

    E_UTRA_OPERATING_BAND_20 = 0x00080000,

    E_UTRA_OPERATING_BAND_21 = 0x000100000,

    E_UTRA_OPERATING_BAND_24 = 0x000800000,

    E_UTRA_OPERATING_BAND_25 = 0x01000000,

    /*E_UTRA_OPERATING_BAND_33 = 0x00100000000,

    E_UTRA_OPERATING_BAND_34 = 0x00200000000,

    E_UTRA_OPERATING_BAND_35 = 0x00400000000,

    E_UTRA_OPERATING_BAND_36 = 0x00800000000,

    E_UTRA_OPERATING_BAND_37 = 0x000001000000000,

    E_UTRA_OPERATING_BAND_38 = 0x000002000000000,

    E_UTRA_OPERATING_BAND_39 = 0x000004000000000,

    E_UTRA_OPERATING_BAND_40 = 0x000008000000000,

    E_UTRA_OPERATING_BAND_41 = 0x000010000000000,

    E_UTRA_OPERATING_BAND_42 = 0x000020000000000,

    E_UTRA_OPERATING_BAND_43 = 0x000040000000000*/
};


/*************This structure below is for GSM RRC Information***********/
struct GSMNeigbCellInfo
{
  u_int16_t bccnNeigARFCN;//0X5137
  int8_t  rxlev_arg;
  u_int32_t c1;
  u_int32_t c2;
  u_int8_t  serving_LAI; //0 means same LAI as serving LAI, 1- Different LAI as serving cell
};
typedef struct GSMNeigbCellInfo GSMNeigbCellInfo;


struct GSMCellInfo
{
  u_int8_t   timestamp[8];
  char   	 MCC[4];
  char   	 MNC[4];
  u_int16_t  bcchARFCN;
  u_int8_t   BSIC[2];
  u_int16_t  cell_id;
  u_int16_t  LAC;
  u_int8_t   LAI[5];
  char 		 CGI[30];
  u_int16_t  Cell_Selection_Priority;
  u_int8_t   NCCPermit;
  int8_t   rxlev_arg;
  u_int32_t  c1;
  u_int32_t  c2;
  int8_t   Rxlev_Full_Avg;
  int8_t   Rxlev_Sub_Avg;
  u_int8_t   Rxqual_Full_Avg;
  u_int8_t   Rxqual_Sub_Avg;
  u_int8_t   no_of_neigbourcell;
  GSMNeigbCellInfo neigCellInfo[MAX_GSM_NEIGH_CELL];
};
typedef struct GSMCellInfo GSMCellInfo;

/************* This structure below is for WCDMA RRC Information*********/
struct wcdma_neigbourcellInfo
{
  u_int16_t URAFCN_Downlink;
  u_int16_t psc;
  int8_t rscp;
  int16_t rank_rscp;
  float ecio;
  int16_t rank_ecio;
};
typedef struct wcdma_neigbourcellInfo wcdma_neigbourcellInfo;

struct gsm_neigbourcellInfo
{
  u_int16_t arfcn;
  u_int16_t bsic;
  int8_t rssi;
  int16_t rank;
};
typedef struct gsm_neigbourcellInfo gsm_neigbourcellInfo;

struct NeigbourcellInfo
{
  u_int8_t num_of_wcdma_cell;
  u_int8_t num_of_gsm_cell;
  wcdma_neigbourcellInfo wcdmaNeigbourData[MAX_WCDMA_NEIGH_CELL]; //This includes the array wcdma neigbourcell
  gsm_neigbourcellInfo gsmNeigbourData[MAX_WCDMA_NEIGH_CELL]; //This includes the array gsm neigbourcell
};
typedef struct NeigbourcellInfo NeigbourcellInfo;
 
struct ActiveSetInfo
{
  u_int8_t  is_reference_cell;
  u_int16_t PRIM_SC_CODE;
  u_int32_t ACELL_POS;
  u_int8_t  CHAN_CODE_IDX;
  u_int8_t  CELL_TPC;
  u_int8_t  Diversity;
  float 	ECIO;
  int8_t 	RSCP;
};
typedef struct ActiveSetInfo ActiveSetInfo;


struct NeigbouringSetInfo
{
  u_int16_t URAFCN_Downlink;
  u_int16_t PRIM_SC_CODE;
  u_int8_t  Diversity;
  u_int32_t NCELL_POS;
};
typedef struct  NeigbouringSetInfo  NeigbouringSetInfo;

struct WCDMACellInfo
{
  u_int8_t timestamp[8];
  u_int32_t  URAFCN_Downlink;
  u_int32_t  URAFCN_Uplink;
  u_int32_t  cell_id;
  u_int16_t  Max_Tx_Power;
  u_int16_t  PRIM_SC_CODE;
  u_int8_t   Num_of_activeSet;
  u_int8_t   Num_of_neigbourSet;
  ActiveSetInfo activeSetInfo[MAX_WCDMA_ACTIVE_SET];
  NeigbouringSetInfo neigbourSetInfo[MAX_WCDMA_NEIG_SET];
  NeigbourcellInfo neigCellInfo;
};
typedef struct WCDMACellInfo  WCDMACellInfo ;

/************LTE Cell Information**********************/
struct LTECellInfo
{
  u_int8_t timestamp[8];
  u_int16_t physical_cell_id;
  u_int16_t dl_freq;
  u_int16_t ul_freq;
  float  dl_bandwidth;
  float  ul_bandwidth;
  u_int32_t cell_identity;
  u_int16_t tracking_area_code;
  u_int32_t frequency_band_indicator;
  u_int16_t mcc;
  u_int8_t num_of_mnc_digits;
  u_int16_t mnc;
  u_int8_t num_tx_antennas;
  u_int32_t cell_timingRx0;
  u_int32_t cell_timingRx1;
  float inst_rsrp_rx0 ;
  float inst_rsrp_rx1 ;
  float inst_rsrp ;
  float inst_rsrq_rx0 ;
  float inst_rsrq_rx1 ;
  float inst_rsrq ;
  float inst_rssi_rx0 ;
  float inst_rssi_rx1;
  float inst_rssi ;
  float sinr_rx0 ;
  float sinr_rx1 ;
};
typedef struct LTECellInfo  LTECellInfo  ;


struct LTEThroughput
{
	float upLinkRLCThroughput;
	float downlinkRLCThroughput;
};
typedef struct LTEThroughput LTEThroughput;

// added fpr lte
struct LTEUplinkPhyStatus
{
	unsigned int	Uplink_WideBand_CQI_CW_0;
	unsigned int	Uplink_Wideband_CQI_CW_1;
	unsigned int	Uplink_Wideband_PMI;
	unsigned int	Uplink_Rank_Index;
	unsigned int	UL_SubBand_CQI_CW_0;
	unsigned int	UL_SubBand_CQI_CW_1;
	unsigned int	UL_NUM_SubBand;
	unsigned int	UL_MCS_Index_Average;
};
typedef struct LTEUplinkPhyStatus LTEUplinkPhyStatus;



struct LTEDownLinkPhyStatus
{
	unsigned int		DL_MCS_Index_Average;
};
typedef struct LTEDownLinkPhyStatus LTEDownLinkPhyStatus;

/* flag =  DISABLE = 0 ENABLE = 1
 * interval = 1 means 500msec , For at the end of test case , means END_OF_TEST_CASE ( interval = 0xFFFF)
 * */
void SetLtePhysicalLayerReporting ( unsigned int flag ,unsigned int interval);
/* This API provide the function to reterive the Downlink Phy Status parameter */

void get_lte_downlink_phy_status(LTEDownLinkPhyStatus* downlinkstatus );

/* This API provide the function to reterive the Uplink Phy Status parameter */

void get_lte_uplink_phy_status(LTEUplinkPhyStatus* uplinkstatus);


void (*callback)(int notifierType , int Identifier , char * buf , unsigned int length);

int init_diag_port( unsigned int manufacture ,unsigned int systype, void (*callback)(int, int , char * , unsigned int ),char*fileName);

//Api for opening the device Diagport (0 -Successful and -1 Failure)

int set_logging_mask( int systype, int mask);//Api for setting logging mask to device.

int get_logging_mask (int  systype);//Api for getting the responce for logging mask.

int set_event_mask( int mask);//Api for setting event mask to device.

int get_event_mask ( );//Api for getting the responce for event mask

int set_uarfn_value(unsigned short frequency);//Api for setting Uarfn frequency to the device

int close_diag_port();//Api for closing the device Diagport.

bool is_freq_set_success();//Api to check the is device is latched to desire Uarfn  frequency.(return True and False( only for wcdma)
 
unsigned short get_uarfn_value();//Api for getting the set Urafcn frequency to the device(return frequency set by "setUrafnValue" method)

int get_GSMCellInfo(GSMCellInfo *gsmCellInfo );//Api for GSM RRC Information.

int get_WCDMACellInfo (WCDMACellInfo *wcdmacellInfo);//Api for WCDMA RRC Information

int get_LTECellInfo (LTECellInfo *lteCellInfo);//Api for LTE RRC Information

int  start_file_write(char* imei,char* testid,int imeilen,int testidlen);// Api for starting the capture process

int  stop_file_write();//Api for stop the capture process

int  set_GUIDInformation (char *guidInfo,int guidInfolen); /* Guide info length should not be greater than 50  */

int  set_GeoLocationInfomation (double Lat,double longitude,double altitude ,int speed,int direction,int locationAccurancy);

int  get_state_info();

int get_DM_version(char *version);//Api to get the version

int get_LTEThroughput(LTEThroughput *ltethroughput);// Api to get the lte throughput

/* device9660 to 1 for S3 otherwise 0 */
int set_lte_freqband(u_int64_t  band , int device9660);/*Api to set the Lte band, return 0 on success,Non zero value on failure */
/* device9660 to 1 for S3 otherwise 0 */
u_int64_t get_lte_freqband(int device9660);/* Api to get the Lte band, return band on success,Zero on failure */

u_int32_t set_gps_last_position();

void diag_com_cdma_request(int fd,u_int16_t code);

/*
 *  Api to set  DLFfile path, return 0 on success else -1
 *  Arg :- pointer to DlfFilePath ,length of path( must be less than 350 bytes)
 */	
 int setDLFFilePath(char *dlfFilePath , unsigned int length); 

/* 
 * Api to set TXTfile path, return 0 on success else -1 
 *  Arg :- pointer to txtFilePath,length of path(must be less than 350 bytes)
 */	
 int setTXTFilePath(char *txtFilePath ,unsigned int length);

 /*
  * Api to set Debug file path, return 0 on success else -1
  *  Arg :- pointer to debugFilePath,length of path(must be less than 350 bytes)
  */
 int setDebugFilePath(char *debugFilePath ,unsigned int length);

//All Data Buffer Formats are in LittleEnd.
// NotificationType - Event , LoggingData
//void * callback(NotifierType type, int identifier ,char *data , unsigned int length );
// BELOW Define the format for each Logging Message
/* RRC OVER-THE_AIR Message LOGGING FORMAT 
struct wcdma_rrc_signaling_messages{
 u_int8_t timestamp[8];
 u_int8_t logical_channel_type;
  0x0 - CCCH on RACH-Uplink, 0x1 - DCCH Uplink, 0x02 - CCCH Downlink, 0x03 - DCCH Downlink, 0x04 - BCCH-BCH Downlink, 0x06 - BCCH-PCCH Downlink, 0x50 -MCCH over FACH, 0x5F - MSCH over FACH, 0x80 -CTCH over FACH,0x0FE - MASTER COMPLITE LIST/SYSTEM INFO  /
                         
 u_int8_t rb_identifier;
 u_int16_t length;/ length of nassendInformation (WCDMA,EVENT_DATA,EVENT_InterRat_Handover_End,eventData,lengthData); */

/* RRC STATE LOGGING FORMAT 
u_int8_t timestamp[8];
u_int8_t state;  / 0 - DISCONNECTED , 1 - CONNECTING , 2- CELL FACH , 3- CELL-DCH , 4 - CELL-PCH , 5 - URA-PCH /
*/          

/* NAS LOGGING FORMAT 
struct umts_nas_signaling{ 
 u_int8_t timestamp[8];
 u_int8_t msg_dir;  / 0 -To UE, 1 - From UE /
 u_int32_t msg_lenth; / length of Nas OTA message / 
 int8_t data[0];  / Variable Nas OTA message /  
}

struct LOG_NAS_MM_State{
 u_int8_t timestamp[8];
 u_int8_t gmm_state; / 0 - GMM_NULL, 1 - GMM_DEREGISTERED, 2 -GMM_REGISTERED_INITIATED, 3 - GMM_REGISTERED, 4 -GMM_DEREGISTERED_INITIATED, 5-GMM_ROUTING_AREA_UPDATE_INITIATED, 6 – GMM_SERVICE_REQUEST_INITIATED /

 u_int8_t gmm_substate; / 0 – GMM_NORMAL_SERVICE, 1 – GMM_LIMITED_SERVICE, 2 – GMM_ATTACH_NEEDED, 3 – GMM_ATTEMPTING_TO_ATTACH, 4 –  GMM_NO_IMSI, 5 – GMM_NO_CELL_AVAILABLE, 6 – GMM_PLMN_SEARCH, 7 – GMM_SUSPENDED, 8 – GMM_UPDATE_NEEDED, 9 – GMM_ATTEMPING_TO_UPDATE, 10 – GMM_ATTEMPTING_TO_UPDATE_MM, 11 – GMM_IMSI_DETACH_INITIATED, 12 – GMM_NULL_SUBSTATE / 

u_int8_t gmm_update; / 0 – GMM_GU1_UPDATED, 1 – GMM_GU2_NOT_UPDATED, 2 –GMM_GU3_PLMN_NOT_ALLOWED, 3 – GMM_GU3_ROUTING_AREA_NOT_ALLOWED /
}

struct LOG_NAS_MM state
{
u_int8_t timestamp[8];
u_int8_t MM_STATE; / 0 – MM_NULL, 3 – MM_LOCATION_UPDATE_INITIATED, 5 – MM_WAIT_FOR_OUTGOING_MM_CONNECTION, 6 – MM_CONNECTION_ACTIVE, 7 – MM_IMSI_DETACH_INITIATED, 8 – MM_PROCESS_CM_SERVICE_PROMPT, 9 – MM_WAIT_FOR_NETWORK_COMMAND, 10 – MM_LOCATION_UPDATE_REJECTED, 13 – MM_WAIT_FOR_RR_CONNECTION_LU, 14 – MM_WAIT_FOR_RR_CONNECTION_MM, 15 – MM_WAIT_FOR_RR_CONNECTION_IMSI_DETACH, 17 – MM_REESTABLISHMENT_INITIATED, 18 – MM_WAIT_FOR_RR_ACTIVE, 19 – MM_IDLE, 20 –MM_WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION, 21 – MM_WAIT_FOR_RR_CONNECTION_REESTABLISHMENT, 22 - MM_WAIT_FOR_REESTABLISH_DECISION, 23 – MM_LOCATION_UPDATING_PENDING, 25 – MM_RR_CONNECTION_RELEASE_NOT_ALLOWED /

u_int8_t MM_SUB_STATE; / 0- MM_NULL_SUBSTATE, 1 - MM_NO_IMSI, 2– MM_PLMN_SEARCH, 3 – MM_LIMITED_SERVICE, 4 – MM_ATTEMPTING_TO_UPDATE, 5 – MM_LOCATION_UPDATE_NEEDED, 6 – MM_NO_CELL_AVAILABLE, 7 – MM_PLMN_SEARCH_NORMAL_SERVICE, 8 – MM_NORMAL_SERVICE /

 u_int8_t MM_UPDATE; / 0 – UPDATED, 1 – NOT_UPDATED, 2 – ROAMING_NOT_ALLOWED, 3 – LOCATION_AREA_NOT_ALLOWED /
}*/

/* EVENT LOGGING FORMAT
struct eventdata {
u_int16_t length;
int8_t event_data[0];
}*/

/**********************************************************************************************************************************/
 /****LTE OTA Message*/ 
/*struct lte_ota_messages{
  u_int8_t timestamp[8];
	u_int8_t version;
	u_int8_t rrc_rel;
	u_int8_t rrc_ver;
	u_int8_t rb_id;
	u_int16_t physical_cell_id;
	u_int16_t freq;
	u_int16_t sfn;
	u_int8_t pdu_num;
*2 – BCCH_DL_SCH Message, 3 – MCCH Message, 4 – PCCH Message, 5 – DL_CCCH Message, 6 – DL_DCCH Message, 7 – UL_CCCH Message, 8 – UL_DCCH   Message,9 – UE EUTRA Capability Message, 10 – VarShortMAC_Input, 11 – UE EUTRA Capability v9a0_IEs,12 – System Information Block Type1 Message /
	u_int16_t encoded_msg_len;
	u_int8_t encoded_data[0]; */


/*Added On 22 April  2013 */

/*EVENT_LTE_NEW_CELL_SELECTION  */
/*struct  event_lte_new_cell_selection{
u_int8_t cause ;  0x0 – Cell selection, 0x1 – Cell reselection, 0x2 – Handover, 0x3 – Redirection
u_int16_t  freq; Frequency – DL-ARFCN
u_int16_t  cell_id;  Physical cell identity */

/*EVENT_IRAT_TECHNOLOGY_CHANGE*/

/*Updated  On 2 May  2013 */
/*struct event_irat_technology_change
{
	u_int32_t Last_technology; 0-NOT INIT, 1 - GSM,2- WCDMA,3 - LTE
	u_int32_t Current_technology; 0-NOT INIT, 1 - GSM,2- WCDMA,3 - LTE
} */
/*Updated  On 23 May  2013 */

/* EVENT_LTE_OUT_OF_SERVICE */
/*struct event_lte_out_of_service{
 u_int16_t  counter;  (counter to indicate how many times UE has gone OOS)
}*/
/*EVENT_LTE_RADIO_LINK_FAILURE */
/*struct event_lte_radio_link_failure{
 u_int16_t  counter;(counter to indicate how many times UE has detected RL failure (Since power up))
 }*/
/* EVENT_LTE_RADIO_LINK_FAILURE_STAT */
/*struct event_lte_radio_link_failure_stat{
u_int16_t rlf_count_since_rrc_connected;(Indicate how many times UE has detected RL failure (Since UE went into RRC CONNECTED state))
u_int16_t rlf_count_since_lte_active;(Indicate how many times UE has detected RL failure (Since LTE stack got activated))
u_int16_t rlf_cause;(0x00 - CFG_FAILURE, 0x01 – HO_FAILURE,0x02 – RLF,0x03 – RACH_PROBLEM,0x04 – MAX_RETRX, 0x05 – IP_CHECK_FAILURE
,0x06 – SIB_READ_FAILURE, 0x07 – OTHER_FAILURE,0x08 –MAX)
}*/

#endif /* INTERFACEAPI_H */

