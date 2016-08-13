#ifndef DECODE_H_
#define DECODE_H_

#define WCDMA_FREQ_SEQ_NUM 10
#define LTE_BAND_SEQ_NUM 11



enum
{
	NOT_INIT = 0,
	DETECT_GSM,
	DETECT_WCDMA,
	DETECT_LTE,
	DETECT_CDMA
};

struct DMCmd_sub_sys_header {
  u_int8_t code;
  u_int8_t subsys_id;
  u_int16_t subsys_cmd;
} __attribute__ ((packed));
typedef struct DMCmd_sub_sys_header DMCmd_sub_sys_header;


struct DMCmd_put_responseEfs{
   u_int8_t code;
   u_int8_t subsys_id;
  	u_int16_t subsys_cmd_code;
  	u_int16_t sequence_num;
   u_int32_t err_num;
}__attribute__((packed));
typedef struct DMCmd_put_responseEfs DMCmd_put_responseEfs;

//EFS2 Diag get response
struct DMCmd_get_responseEfs{
	u_int8_t code;
	u_int8_t subsys_id;
	u_int16_t subsys_cmd_code;
	u_int32_t num_bytes;
	u_int32_t err_num;
	u_int16_t sequence_num;
	u_int8_t data[0];
}__attribute__((packed));
typedef struct DMCmd_get_responseEfs DMCmd_get_responseEfs;

/**************************************************/

struct rssi_req_message{
   u_int8_t cmd_code;
   u_int8_t subsys_id;
   u_int16_t subsys_cmd_code;
   u_int8_t valid;
   u_int8_t band;
   u_int16_t earfcn;
   u_int16_t physical_cell_id;
   u_int16_t rssi;
   u_int16_t rsrp;
   u_int16_t rsrq;
   u_int16_t tx_power;
}__attribute__ ((packed ));
typedef struct rssi_req_message rssi_req_message;
/***************************************************/

struct event_decode{
   u_int8_t cmd_code;
   u_int16_t data_length;
   u_int16_t event_id;
}__attribute__ ((packed ));
typedef struct event_decode event_decode;
/************************************************************/


//(ID: 1618)
struct inter_rat_reselection_from_e_utran_end{
	u_int8_t rat ;
}__attribute__ ((packed));
typedef struct inter_rat_reselection_from_e_utran_end inter_rat_reselection_from_e_utran_end;

/*****************************************************************************/
//(ID: 1617)
struct inter_rat_reselection_from_e_utran_start {
   u_int8_t rat ;
   u_int32_t freq_band_indicator ;
   u_int16_t frequency ;
   u_int16_t cell_id ;
}__attribute__ ((packed));
typedef struct inter_rat_reselection_from_e_utran_start inter_rat_reselection_from_e_utran_start;

/**************************************************************************/
//(ID: 1808)
struct inter_rat_redirection_from_e_utran_end  {
  u_int8_t rat ;
  u_int8_t status_casue ;
}__attribute__ ((packed));
typedef struct inter_rat_redirection_from_e_utran_end inter_rat_redirection_from_e_utran_end;

/**************************************************************************/
//(ID: 1807)
struct inter_rat_redirection_from_e_utran_start  {
  u_int8_t rat ;
}__attribute__ ((packed));
typedef struct inter_rat_redirection_from_e_utran_start inter_rat_redirection_from_e_utran_start;
/**************************************************************************/

//(ID: 1613)/*HANDOVER_FAILURE*/
struct handover_failure{
u_int16_t frequency;
u_int16_t cell_id ;
u_int8_t cause;
}__attribute__ ((packed));
typedef struct handover_failure handover_failure;

/*****************************************************************************************/

//(ID:1615)/*MOBILITY_FROM_E_ULTRA_STARTED*/
struct mobility_from_e_ultra_started{
	u_int8_t rat_procedure;
}__attribute__ ((packed));
typedef struct mobility_from_e_ultra_started mobility_from_e_ultra_started;
/******************************************************************************************/

//(ID:1616)/*MOBILITY_FROM_E_ULTRA_FAILED*/
struct mobility_from_e_ultra_failed{
	u_int8_t status_cause;
}__attribute__ ((packed));
typedef struct mobility_from_e_ultra_failed mobility_from_e_ultra_failed;

/**********************************************************************************************/


/*EVENT_IRAT_TECHNOLOGY_CHANGE*/

struct event_irat_technology_change
{
	u_int32_t Last_technology;
	u_int32_t Current_technology;
}__attribute__ ((packed));
typedef struct event_irat_technology_change event_irat_technology_change;

/*EVENT_LTE_NEW_CELL_SELECTION  */
struct  event_lte_new_cell_selection{
u_int8_t cause;
u_int16_t  freq;
u_int16_t  cell_id;
}__attribute__ ((packed));
typedef struct event_lte_new_cell_selection  event_lte_new_cell_selection;

/* EVENT_LTE_OUT_OF_SERVICE */
struct event_lte_out_of_service{
 u_int16_t  counter;
}__attribute__ ((packed));
typedef struct event_lte_out_of_service  event_lte_out_of_service;

/*EVENT_LTE_RADIO_LINK_FAILURE */
struct event_lte_radio_link_failure{
 u_int16_t  counter;
}__attribute__ ((packed));
typedef struct  event_lte_radio_link_failure  event_lte_radio_link_failure;

/* EVENT_LTE_RADIO_LINK_FAILURE_STAT */
struct event_lte_radio_link_failure_stat{
u_int16_t rlf_count_since_rrc_connected;
u_int16_t rlf_count_since_lte_active;
u_int16_t rlf_cause;
}__attribute__ ((packed));
typedef struct  event_lte_radio_link_failure_stat  event_lte_radio_link_failure_stat;


/* Time responce decoder structure */
struct dm_time_response{
u_int8_t   cmd_code ;
u_int64_t  unused : 16;
u_int64_t  counter : 48;
}__attribute__ ((packed));
typedef struct  dm_time_response  dm_time_response;

/* Time response function */
int DmTimeResponse(unsigned char *buff, unsigned int packetlength);
unsigned long long getTimeDiff();

int decode_get_state_info();

#endif /* DECODE_H_ */
