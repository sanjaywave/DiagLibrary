#ifndef LTEDECODE_H_
#define LTEDECODE_H_


/*0xB0C0*/   /*LTE_OTA_MESSAGE*/
struct lte_ota_messages{
  u_int8_t version;
  u_int8_t rrc_rel;
  u_int8_t rrc_ver;
  u_int8_t rb_id;
  u_int16_t physical_cell_id;
  u_int16_t freq;
  u_int16_t sfn;
  u_int8_t pdu_num;
  u_int16_t encoded_msg_len;
  u_int8_t encoded_data[0];

}__attribute__ ((packed));
typedef struct lte_ota_messages lte_ota_messages;
/******************************************************/

/*0xB0C1*/  /*LTE_MIB_MESSAGE*/
struct lte_mib_messages{
  u_int8_t version;
  u_int16_t physical_cell_id;
  u_int16_t freq;
  u_int16_t sfn;
  u_int8_t num_tx_antennas;
  u_int8_t dl_bandwidth;
}__attribute__ ((packed ));
typedef struct lte_mib_messages lte_mib_messages;
/*********************************************************/

/*0xB0C2*/  /*LTE_SERVING_CELL_INFO*/
struct lte_serving_cell_info{
  u_int8_t version;
  u_int16_t physical_cell_id;
  u_int16_t dl_freq;
  u_int16_t ul_freq;
  u_int8_t dl_bandwidth;
  u_int8_t ul_bandwidth;
  u_int32_t cell_identity;
  u_int16_t tracking_area_code;
  u_int32_t frequency_band_indicator;
  u_int16_t mcc;
  u_int8_t num_of_mnc_digits;
  u_int16_t mnc;
  u_int8_t allowed_access;
}__attribute__ ((packed ));
typedef struct lte_serving_cell_info lte_serving_cell_info;

/**************************************************************/

/*0xB17F*/ /*LTE_ML1_SERVING_CELL_MEASUREMENT_AND _EVALUTION*/
struct lte_ml1_serving_cell_measurements_and_evaluation {
  u_int8_t version ;
  u_int8_t standards_version ;
  u_int16_t reserved1 ;
  u_int16_t e_arfcn;
  u_int16_t  physical_cell_id : 9;
  u_int16_t  serving_layer_priority : 4;
  u_int16_t  reserved2 : 3;
  u_int16_t measured_rsrp : 12;
  u_int16_t reserved3 : 12;
  u_int16_t average_rsrp : 12;
  u_int16_t reserved4 : 12;
  u_int16_t measured_rsrq : 10;
  u_int16_t reserved5 : 10;
  u_int16_t  average_rsrq : 10;
  u_int16_t reserved6 : 12;
  u_int16_t measured_rssi : 11;
  u_int16_t reserved7 : 11;
}__attribute__ ((packed ));
typedef struct lte_ml1_serving_cell_measurements_and_evaluation  lte_ml1_serving_cell_measurements_and_evaluation;

/*************************************************************/

/*0xB193*/ /* LTE_ML1_SERVING_CELL_MEASUREMENT*/
struct lte_ml1_serving_cell_measurement{
  u_int8_t version;
  u_int8_t no_sub_packets;
  u_int16_t reserved;
}__attribute__ ((packed));
typedef struct lte_ml1_serving_cell_measurement lte_ml1_serving_cell_measurement;

struct lte_ml1_serving_cell_measurement_log_subpacket{
  u_int8_t sub_packet_id;
  u_int8_t sub_packet_version;
  u_int16_t sub_packet_size;
  u_int8_t sub_packet_data[0];
}__attribute__ ((packed));
typedef struct lte_ml1_serving_cell_measurement_log_subpacket lte_ml1_serving_cell_measurement_log_subpacket;

struct lte_ml1_serving_cell_measurement_log_1st_subpacket_data{
  u_int8_t Standards_version ;
  u_int8_t reserve[3];
  u_int16_t inst_measured_rsrq : 9;
  u_int16_t srxlev : 7;
  u_int16_t inst_measured_rsrp : 11;
  u_int16_t reserved1 : 5;
  u_int16_t inst_measured_rssi :11;
  u_int16_t display_avg : 1;
  u_int16_t reserved2 : 4 ;
  u_int16_t avg_measured_rsrq : 9 ;
  u_int16_t reserved3 : 7;
  u_int16_t avg_measured_rsrp : 11;
  u_int16_t reserve4 : 5;
  u_int16_t avg_measured_rssi : 11;
  u_int16_t reserve5 : 5;
}__attribute__ ((packed));
typedef struct lte_ml1_serving_cell_measurement_log_1st_subpacket_data lte_ml1_serving_cell_measurement_log_1st_subpacket_data;


struct lte_ml1_serving_cell_measurement_log_subpacket_REL9_extra{
  u_int16_t squal : 7;
  u_int16_t reserved1 : 9;
}__attribute__ ((packed));
typedef struct lte_ml1_serving_cell_measurement_log_subpacket_REL9_extra
  lte_ml1_serving_cell_measurement_log_subpacket_REL9_extra;

struct lte_ml1_serving_cell_measurement_log_second_subpacket_data{
  u_int16_t earfcn;
  u_int16_t phy_cell_id : 9;
  u_int16_t reserved1 : 7 ;
  u_int32_t current_sfn : 10;
  u_int32_t cell_timingRx0 : 19;
  u_int32_t reserved2 : 3;
  u_int32_t cell_timingRx1 : 19;
  u_int32_t cell_timing_sfn0 : 10;
  u_int32_t reserved3 : 3;
  u_int32_t cell_timing_sfn1 :10;
  u_int32_t inst_rsrp_rx0 : 12;
  u_int32_t reserved4: 10;
  u_int32_t reserved5: 12;
  u_int32_t inst_rsrp_rx1 : 12;
  u_int32_t reserved6: 8;
  u_int32_t reserved7: 12;
  u_int32_t inst_measured_rsrp: 12;
  u_int32_t reserved8: 8;
  u_int32_t reserved9: 12;

  u_int32_t inst_rsrq_rx0: 10;
  u_int32_t reserved10: 10;
  u_int32_t inst_rsrq_rx1: 10;
  /*********************/
  u_int32_t reserved11: 10;
  u_int32_t inst_rsrq : 10; /*   32 bytes           */
  u_int32_t reserved12: 2;
/*********************/
  u_int32_t reserved13: 10;     /*   64 bytes           */
  u_int32_t inst_rssi_rx0: 11;
  u_int32_t inst_rssi_rx1: 11;
  u_int32_t inst_rssi: 11;
  u_int32_t reserved14: 21;
  /***************/
  u_int8_t reserved15[10];        /*   96 bytes */
  u_int16_t res_freq_error: 16;
/*****************/
  u_int8_t reserved16[8]; /*  96 bytes **/
  u_int32_t sinr_rx0: 9;
  u_int32_t sinr_rx1: 9;
  u_int32_t reserved17: 14;
	/*****************/
  u_int8_t reserved18[12]; /*  96 bytes **/
}__attribute__ ((packed));
typedef struct lte_ml1_serving_cell_measurement_log_second_subpacket_data
 lte_ml1_serving_cell_measurement_log_second_subpacket_data;

/***********************************************************************/
/* Struct to calculate PDU for PDCP and RLC */
typedef struct
{
  u_int64_t totalpdu;
  char timestamp[8];
  u_int32_t isFirstCount;

}PDUInfo;

/*************************************************************************************/



/* RLC LAYER */
/*LTE_RLC_DL_STATISTICS*/

struct rlc_dl_statistics_subpacket_num_rb{
u_int8_t rb_cfg_idx;
u_int8_t mode;
u_int32_t num_rst;
u_int32_t num_data_pdu;
u_int32_t data_pdu_bytes;
u_int32_t num_status_rxed;
u_int32_t status_rxed_bytes;
u_int32_t num_invalid_pdu;
u_int32_t invalid_pdu_bytes;
u_int32_t num_retx_pdu;
u_int32_t retx_pdu_bytes;
u_int32_t num_dup_pdu;
u_int32_t num_dup_bytes;
u_int32_t num_dropped_pdu;
u_int32_t dropped_pdu_bytes;
u_int32_t num_dropped_pdu_fc;
u_int32_t dropped_pdu_bytes_fc;
u_int32_t num_sdu;
u_int32_t num_sdu_bytes;
u_int32_t num_nonseq_sdu;
u_int32_t num_ctrl_pdu;
u_int32_t num_comp_nack;
u_int32_t num_segm_nack;
u_int32_t num_t_reord_exp;
u_int32_t reserved;
u_int32_t num_data_pdu_rst;
u_int32_t data_pdu_bytes_rst;
u_int32_t num_status_rxed_rst;
u_int32_t status_rxed_bytes_rst;
u_int32_t num_invalid_pdu_rst;
u_int32_t invalid_pdu_bytes_rst;
u_int32_t num_retx_pdu_rst;
u_int32_t retx_pdu_bytes_rst;
u_int32_t num_dup_pdu_rst;
u_int32_t num_dup_bytes_rst;
u_int32_t num_dropped_pdu_rst;
u_int32_t dropped_pdu_bytes_rst;
u_int32_t num_dropped_pdu_fc_rst;
u_int32_t dropped_pdu_bytes_fc_rst;
u_int32_t num_sdu_rst;
u_int32_t num_sdu_bytes_rst;
u_int32_t num_nonseq_sdu_rst;
u_int32_t num_ctrl_pdu_rst;
u_int32_t num_comp_nack_rst;
u_int32_t num_segm_nack_rst;
u_int32_t num_t_reord_exp_rst;
u_int32_t reserved1;
}__attribute__ ((packed));
typedef struct rlc_dl_statistics_subpacket_num_rb rlc_dl_statistics_subpacket_num_rb;

struct rlc_dl_statistics_subpacket_dl{
u_int8_t num_rb;
u_int32_t rlc_pdcp_q_full_cnt;
u_int32_t rlcdl_error_cnt;
rlc_dl_statistics_subpacket_num_rb rlc_dl_statistics_subpacket_num_rb[0];
}__attribute__ ((packed));
typedef struct rlc_dl_statistics_subpacket_dl rlc_dl_statistics_subpacket_dl;


/*LTE_RLC_UL_STATISTICS*/

struct rlc_log_packet_structure_subpackets_num_rb{
u_int8_t rb_cfg_idx;
u_int8_t mode;
u_int32_t num_rst;
u_int32_t num_new_data_pdu;
u_int32_t num_new_data_pdu_bytes;
u_int32_t num_sdu;
u_int32_t num_sdu_bytes;
u_int32_t num_ctrl_pdu_tx;
u_int32_t num_ctrl_pdu_bytes_tx;
u_int32_t num_retx_pdus;
u_int32_t num_retx_pdu_bytes;
u_int32_t num_ctrl_pdu_rx;
u_int32_t num_comp_nack;
u_int32_t num_segm_nack;
u_int32_t num_invalid_ctrl_pdu_rx;
u_int32_t num_poll;
u_int32_t num_t_poll_retx_expiry;
u_int32_t reserved;
u_int32_t num_new_data_pdu_rst;
u_int32_t num_new_data_pdu_bytes_rst;
u_int32_t num_sdu_rst;
u_int32_t num_sdu_bytes_rst;
u_int32_t num_ctrl_pdu_tx_rst;
u_int32_t num_ctrl_pdu_bytes_tx_rst;
u_int32_t num_retx_pdus_rst;
u_int32_t num_retx_pdu_bytes_rst;
u_int32_t num_ctrl_pdu_rx_rst;
u_int32_t num_comp_nack_rst;
u_int32_t num_segm_nack_rst;
u_int32_t num_invalid_ctrl_pdu_rx_rst;
u_int32_t num_poll_rst;
u_int32_t num_t_poll_retx_expiry_rst;
u_int32_t reserved1;
}__attribute__ ((packed));
typedef struct rlc_log_packet_structure_subpackets_num_rb rlc_log_packet_structure_subpackets_num_rb;

struct rlc_log_packet_structure_subpackets_ul{
u_int8_t num_rb;
u_int32_t rlcul_error_cnt;
rlc_log_packet_structure_subpackets_num_rb rlc_log_packet_structure_subpackets_num_rb[0];
}__attribute__ ((packed));
typedef struct rlc_log_packet_structure_subpackets_ul rlc_log_packet_structure_subpackets_ul;
/*********************************************************/
struct rlc_log_sub_packet_structure
{
 u_int8_t  sub_packet_id;
 u_int8_t  sub_packet_version;
 u_int16_t  sub_packet_size;
}__attribute__ ((packed));
typedef struct  rlc_log_sub_packet_structure
rlc_log_sub_packet_structure;
/*************************************************************/
struct rlc_log_packet_structure{
u_int8_t version;
u_int8_t number_subpackets;
u_int16_t reserved;
rlc_log_sub_packet_structure  rlc_log_sub_packet_structure[0];
}__attribute__ ((packed));
typedef struct rlc_log_packet_structure rlc_log_packet_structure;


void  lte_decode_log_msg(int code, unsigned char *buf ,int length, u_int8_t timestamp[8]);



#endif /* LTEDECODE_H_ */
