#ifndef GSMDECODE_H_
#define GSMDECODE_H_

struct gsm_rr_signaling
{
    int8_t subId;
    int8_t channelType;
    int8_t msgType;
    int8_t length;
    int8_t l3Message[0];
}__attribute__ ((packed));
typedef struct gsm_rr_signaling gsm_rr_signaling;

/*******************************************************************/

/*GSM_RR_STATE_MESSAGE*/
struct gsm_rr_state_message
{
    int8_t rr_state;
    int8_t rr_substate;
    int8_t rr_status;
    int8_t mode;
}__attribute__ ((packed));
typedef struct gsm_rr_state_message gsm_rr_state_message;
/***************************************************************************/

 /*GSM_RR_CELLINFORM*/
struct gsm_rr_cellInform
{
    int16_t bcchArfcn;
    int8_t bsic_plmn_color ;
    int8_t bsic_bs_color ;
    int16_t cellIdentity;
    int8_t  lai[5];
    int8_t  cellSelectionPriority;
    int8_t  nccPermit;
}__attribute__ ((packed));
typedef struct gsm_rr_cellInform gsm_rr_cellInform;
/****************************************************************************/
struct gsm_rr_cipher
{
    int8_t cipherState;
    int8_t cipherAlgo;
}__attribute__ ((packed));
typedef struct gsm_rr_cipher gsm_rr_cipher;
/***********************************************************************************/
  /*GRR_CELL_RESELECTION_MEASURMENT*/
struct grr_cell_reselection_measurment
{
    int16_t servingBCCHArfcn;
    int16_t servingPBCCHArfcn;
    int8_t  servingPriortyClass;
    int8_t  servingRxlev;
    int32_t servingC1;
    int32_t servingC2;
    int32_t servingC31;
    int32_t servingC32;
    int8_t  servingFive_second_timer_state;
    int8_t  cellReSelectStatus;
    int8_t  recent_cell_selection;
    int8_t  neigbCellCount;
}__attribute__ ((packed));
typedef struct grr_cell_reselection_measurment grr_cell_reselection_measurment;

struct gsm_neigb_info
{
    int16_t BcchArfcn;
    int16_t PBcchArfcn;
    int8_t  priorityClass;
    int8_t  servingRxlev;
    int32_t C1;
    int32_t C2;
    int32_t C31;
    int32_t C32;
    int8_t  five_secong_timer_state;
    int8_t cell_reselection_sataus;
    int8_t serving_lai;
}__attribute__ ((packed));
typedef struct gsm_neigb_info gsm_neigb_info;
/***********************************************************/
/*GSM_RR_SACCH_REPORT*/
struct gsm_rr_sacch_report_ncell_measurements_measurement{
u_int16_t bcch_arfcn;
u_int8_t rxlev_avg;
u_int8_t bsic;

}__attribute__ ((packed));
typedef struct gsm_rr_sacch_report_ncell_measurements_measurement  gsm_rr_sacch_report_ncell_measurements_measurement;

struct gsm_rr_sacch_report_ncell_measurements{
u_int16_t ncell_count;
gsm_rr_sacch_report_ncell_measurements_measurement gsm_rr_sacch_report_ncell_measurements_measurement[0];

}__attribute__ ((packed));

typedef struct gsm_rr_sacch_report_ncell_measurements   gsm_rr_sacch_report_ncell_measurements;

struct gsm_rr_sacch_report{
u_int16_t serving_bcch_arfcn;
u_int8_t rxlev_full_avg;
u_int8_t rxlev_sub_avg;
u_int8_t rxqual_full_avg;
u_int8_t rxqual_sub_avg;
gsm_rr_sacch_report_ncell_measurements gsm_rr_sacch_report_ncell_measurements[0];
}__attribute__ ((packed));
typedef struct gsm_rr_sacch_report gsm_rr_sacch_report;

/***************************************************************************/

 /*GSM_RR_CHANNEL_CONFIGURATION */
struct gsm_rr_channel_configuration {
	u_int8_t num_ded_chans;
	u_int8_t dtx_indicator;
	u_int8_t power_level;
	u_int8_t starting_time_valid;
	u_int16_t starting_time;
	u_int8_t cipher_flag;
	u_int8_t cipher_algorithm;
	u_int8_t channel_mode_1;
	u_int8_t channel_mode_2;
	}__attribute__ ((packed));
typedef struct gsm_rr_channel_configuration gsm_rr_channel_configuration;

struct channel_config{
	u_int8_t channel_type;
	u_int8_t  subchannel;
	u_int8_t tn;
	u_int8_t tsc;
	u_int8_t hopping_flag;
	u_int8_t maio;
	u_int8_t hsn;
	u_int8_t freq_count;
	}__attribute__ ((packed));
typedef struct channel_config channel_config;
struct freq_list{
	u_int16_t freq;
}__attribute__ ((packed));
typedef struct freq_list freq_list;

/**************************************************************************/

 /*GSM_RR_CELL_OPTIONS*/
struct gsm_rr_cell_options{
	u_int8_t pwrc;
	u_int8_t dtx;
	u_int8_t radio_link_timeout;

}__attribute__((packed));
typedef struct gsm_rr_cell_options gsm_rr_cell_options;
/*****************************************************************************/

/*GRR_PACKET_SYSTEM_INFORMATION*/
struct grr_packet_system_information{
	u_int8_t plmn_id[3];
	u_int16_t lac;
	u_int8_t rac;
	u_int8_t cell_id;
	u_int8_t non_gprs_cell_present;
	u_int8_t att;
	u_int8_t t3212;
	u_int8_t neci;
	u_int8_t pwrc;
	u_int8_t dtx;
	u_int8_t radio_link_timeout;
	u_int8_t b_ag_blks_res;
	u_int8_t bs_cc_chans;
	u_int8_t bs_ccch_sdcch_comb;
	u_int8_t bs_pa_mfrms;
	u_int8_t max_retrans;
	u_int8_t tx_interger;
	u_int8_t access_controll_rettry;
	u_int8_t ms_txpwr_max_ccch;
}__attribute__((packed));
typedef struct grr_packet_system_information grr_packet_system_information;
/***************************************************************/
//GSM_TIMIMG_ADVANCE
struct gsm_timing_advance{
	u_int8_t ta_index;
	u_int8_t ta_timeslot;
	u_int8_t ta_assigned_value;
	u_int8_t ta_continous_value;
}__attribute__((packed));
typedef struct gsm_timing_advance gsm_timing_advance;

/*******************************************************************/
//GSM_L1_BURST_METRICS
struct gsm_l1_brust_metrics{
u_int8_t  channel;
  // four burst metric
 //gsm_l1_brust_metrics_structure gsml1bms[4];

}__attribute__((packed));
typedef struct gsm_l1_brust_metrics gsm_l1_brust_metrics;

struct gsm_l1_brust_metrics_structure{
	u_int32_t  fn;
	u_int16_t arfcn;
	u_int32_t rssi	;
	u_int16_t power;
	u_int16_t dc_offset_i;
	u_int16_t dc_offset_q;
	u_int16_t freq_offset;
	u_int16_t timing_offset;
	u_int16_t snr;
	u_int8_t gain_state;

}__attribute__((packed));
typedef struct gsm_l1_brust_metrics_structure gsm_l1_brust_metrics_structure;
/*****************************************************************/
// GSM_RR_RADIO_LINK_TIMEOUT_COUNTER
struct gsm_rr_radio_link_timeout_counter{
	u_int8_t  s_counter;

}__attribute__((packed));
typedef struct gsm_rr_radio_link_timeout_counter gsm_rr_radio_link_timeout_counter;
/****************************************************************************/

// GSM_L1_BRUST_METRICS_A
struct gsm_l1_brust_metrics_a {
	u_int8_t channel;
	u_int32_t fn;
	u_int8_t tn;
	u_int16_t arfcn;
	u_int32_t rssi;
	u_int16_t power;
	u_int16_t dc_offset_i;
	u_int16_t dc_offset_q;
	u_int16_t freq_offset;
	u_int16_t timing_offset;
	u_int8_t usf;
	u_int16_t snr;
	u_int8_t gain_state;
	u_int8_t modulation_detected;
	u_int8_t filter_used;
	u_int16_t aci_pwr_n200khz;
	u_int16_t aci_pwr_0hz;
	u_int16_t aci_pwr_p200khz;
}__attribute__((packed));
typedef struct gsm_l1_brust_metrics_a  gsm_l1_brust_metrics_a ;

/***************************************************************/

//  GSM_RLC-UL_ACK/NACK_PARAMETERS
struct gsm_rl_cul_ack_or_nack_parameter{
	u_int8_t ul_tfi;
	u_int8_t fai;
	u_int8_t ssn;
	u_int8_t cs;
	u_int8_t cv;
	u_int8_t va;
	u_int8_t vs;
	u_int8_t si;
	u_int32_t  hu32;
	u_int32_t  lu32;
}__attribute__((packed));
typedef struct gsm_rl_cul_ack_or_nack_parameter gsm_rl_cul_ack_or_nack_parameter;

/*********************************************************************/
 //GSM_L1_MESSAGE_METRICS_A all parametes are in 5245

/*********************************************************************/
//GPRS_OR_EGPRS_RLC_DOWMLINK_HEADER
struct gprs_or_egprs_rlc_downlink_header{
	// need help


}__attribute__((packed));
typedef struct gprs_or_egprs_rlc_downlink_header gprs_or_egprs_rlc_downlink_header;
/********************************************************************/

//MAC_DL_TBF_ESTABLISHS
struct mac_dl_tbf_establishs{
	u_int8_t dl_tfi;
	u_int8_t rlc_mode;
	u_int8_t dl_ts_bmap;
	u_int8_t egprs_tbf;
	u_int16_t egprs_window_size;
	u_int8_t egprs_link_qual_mode;
	u_int8_t bep_period2;

}__attribute__((packed));
typedef struct mac_dl_tbf_establishs mac_dl_tbf_establishs;

/********************************************************************/

/*GPRS_OR_EGPRS_RLC_UPLINK_HEADER*/
struct gprs_or_egprs_rlc_uplink_header{
	// need help
}__attribute__((packed));
typedef struct gprs_or_egprs_rlc_uplink_header gprs_or_egprs_rlc_uplink_header;
/**************************************************************/

//****0x5228  //MAC_UL_TBF_ESTABLISH
struct mac_ul_tbf_establish{
	u_int8_t tbf_req_cause;
	u_int8_t access_granted;
	u_int8_t radio_priority;
	u_int8_t peack_throughput;
	u_int8_t ul_tfi;
	u_int8_t rlc_mode;
	u_int8_t ul_ts_bmap;
	u_int8_t egprs_tbf;
	u_int16_t egprs_window_size;
	u_int8_t resegment;
	u_int8_t bep_period2;
}__attribute__((packed));
typedef struct  mac_ul_tbf_establish  mac_ul_tbf_establish;

/***********************************************************************/

// **0x51FA /* GSM_GRR_STATE
struct gsm_grr_state{
	u_int8_t active_state;
	u_int8_t  grr_state;
}__attribute__((packed));
typedef struct gsm_grr_state gsm_grr_state;

/**********************************************************************/

/* GSM_LLC_ME_INFORMATION*/
struct gsm_llc_me_information{
	u_int8_t llme_state;
	u_int8_t llme_xid_version;
	u_int32_t llme_new_tlli;
	u_int32_t llme_old_tlli;
	u_int8_t llme_encrypt_alg;
	u_int64_t llme_encrypt_key;
	u_int32_t llme_input_offset_iov_iu;

}__attribute__((packed));
typedef struct gsm_llc_me_information gsm_llc_me_information;

/*****************************************************************************/

/*GSM_RLC_UL_STATISTICS*/
struct gsm_rlc_ul_statistics{
	u_int8_t llme_state;
	u_int8_t llme_xid_version;
	u_int32_t llme_new_tlli;
}__attribute__((packed));
typedef struct gsm_rlc_ul_statistics gsm_rlc_ul_statistics;

/*5210 - RLC_DL_ACK_OR_NACK_PARAMETERS*/
struct grr_dl_rlc_ack_nack{
u_int8_t dl_tfi;
u_int8_t fai;
u_int8_t ssn;
u_int8_t vq;
u_int32_t hu32;
u_int32_t lu32;
u_int8_t cs;
}__attribute__ ((packed));
typedef struct grr_rlc_ack_nack   grr_dl_rlc_ack_nack;


/*RLC_UL_ACK_OR_NACK_PARAMETERS*/
struct grr_ul_rlc_ack_nack{
u_int8_t ul_tfi;
u_int8_t fai;
u_int8_t ssn;
u_int8_t cs;
u_int8_t cv;
u_int8_t va;
u_int8_t vs;
u_int8_t si;
u_int32_t hu32;
u_int32_t lu32;
}__attribute__ ((packed));
typedef struct grr_rlc_ack_nack   grr_ul_rlc_ack_nack;

/*EGPRS_RLC_UPLINK_ACK_NACK*/
struct egprs_rlc_uplink_ack_nack{
u_int8_t tfi;
u_int8_t mask;
u_int16_t ssn;
u_int8_t crbb_length;
u_int8_t crbb[16];
u_int8_t urbb_length;
u_int8_t urbb[20];
u_int8_t gmsk_bep;
u_int8_t psk_bep;
u_int8_t c_value;
}__attribute__ ((packed));
/*************************************************************/
/*RLC_UL_STATISTICS*/
struct rlc_ul_statistics{
u_int8_t rlc_ul_state;
u_int8_t ul_xfer_substate;
u_int32_t ack_tx_pdu_cnt;
u_int32_t ack_tx_oct_cnt;
u_int32_t unack_tx_pdu_cnt;
u_int32_t unack_tx_oct_cnt;
u_int16_t coding_sch_changes;
u_int8_t ul_seq_va;
u_int8_t ul_seq_vs;
u_int8_t ul_seq_nseg;
u_int8_t ul_seq_vsf;
u_int8_t ul_seq_pack_vsf;
u_int8_t ul_seq_nack_vsf;
u_int8_t n3102;
u_int8_t n3104;
u_int32_t ul_gen_purpose_count_a;
u_int32_t ul_gen_purpose_count_b;
u_int32_t ul_gen_purpose_count_c;
}__attribute__ ((packed));
typedef struct rlc_ul_statistics  rlc_ul_statistics;


/*RLC_DL_STATISTICS*/
struct rlc_dl_statistics{
u_int8_t rlc_dl_state;
u_int32_t ack_rx_pdu_cnt;
u_int32_t ack_rx_oct_cnt;
u_int32_t unack_rx_pdu_cnt;
u_int32_t unack_pdu_filled_cnt;
u_int32_t unack_rx_oct_cnt;
u_int32_t unack_oct_filled_cnt;
u_int32_t ack_nack_req_cnt;
u_int8_t dl_seq_vq;
u_int8_t dl_seq_vr;
u_int8_t dl_seq_ssn;
u_int32_t dl_gen_purpose_conut_a;
u_int32_t dl_gen_purpose_conut_b;
u_int32_t dl_gen_purpose_conut_c;
}__attribute__ ((packed));
typedef struct rlc_dl_statistics  rlc_dl_statistics;

/* LLC_PDU_STATISTICS */
struct llc_pdu_statistics {
	u_int8_t gllc_sapi;
	u_int32_t  ack_l3pdu_tx_cnt;
	u_int32_t  ack_oct_tx_cnt;
	u_int32_t  ack_l3pdu_retx_cnt;
	u_int32_t	ack_oct_retx_cnt;
	u_int32_t	ack_l3pdu_rx_cnt;
	u_int32_t   ack_oct_rx_cnt;
	u_int32_t	unack_l3pdu_tx_cnt;
	u_int32_t	unack_oct_tx_cnt;
	u_int32_t   unack_l3pdu_rx_cnt;
	u_int32_t   unack_oct_rx_cnt;
	u_int32_t 	llpdu_tx_cnt;
	u_int32_t 	llpdu_rx_cnt;
	u_int32_t  	llpdu_fcs_err_cnt;
	u_int32_t  	llpdu_frm_rej_cnt;
	u_int32_t  	llpdu_tlli_err_cnt;
	u_int32_t  	llpdu_addr_err_cnt;
	u_int32_t  	llpdu_short_err_cnt;
	}__attribute__ ((packed));
typedef struct llc_pdu_statistics  llc_pdu_statistics;



void gms_decode_log_msg(int code, unsigned char *buf ,int length, u_int8_t timestamp[8]);
#endif /* GSMDECODE_H_ */
