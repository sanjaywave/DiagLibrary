#ifndef WCDMADECODE_H_
#define WCDMADECODE_H_


//(ID: 4110)/*WCDMA_ACTIVE_SET*/
struct wcdma_active_set {
	u_int8_t num_cells;
 	u_int16_t freq_dl;
}__attribute__ ((packed));
typedef struct wcdma_active_set  wcdma_active_set;

struct wcdma_active_set_info {
	u_int8_t ref_cell;
 	u_int16_t prim_sc_code;
 	u_int8_t cell_tpc;
 	u_int8_t cpi_div_cell;
 	u_int8_t sc_pich;
 	u_int8_t sec_sc_code;
	u_int8_t chan_code_idx;
 	u_int32_t acell_pos;
}__attribute__ ((packed));
typedef struct wcdma_active_set_info wcdma_active_set_info;
 /*********************************************************************/

//(ID : 4111) /*WCDMA_NEIGHBOR_SET*/
struct wcdma_neighbor_set{
 	u_int8_t num_freq;
}__attribute__ ((packed));
typedef struct wcdma_neighbor_set wcdma_neighbor_set;

struct wcdma_neighbor_set_freq {
	u_int16_t freq_dl;
 	u_int8_t num_neigh_cell;
}__attribute__ ((packed));
typedef struct wcdma_neighbor_set_freq  wcdma_neighbor_set_freq ;

struct wcdma_neighbor_set_cell {
	u_int16_t prim_scr_code;
 	u_int8_t cpl_div_cell;
 	u_int32_t ncell_pos;
}__attribute__ ((packed));
typedef struct wcdma_neighbor_set_cell wcdma_neighbor_set_cell;
/************************************************************************/

//0x410c /*WCDMA_PHYSICAL_CHANNELS_UPLINKS*/
struct wcdma_physical_channels_uplinks {
	u_int8_t max_tx_pwr;
	u_int16_t  dpcch_pwr_offset;
 	u_int8_t   pcp_len;
 	u_int8_t   pca;
 	u_int8_t  tpc_step_size;
 	u_int8_t  tfci_present;
 	u_int8_t  fbi_len;
 	u_int8_t  punct_lim_val;
 	u_int8_t  init_tx_pwr;
 	u_int8_t  dpcch_slot_fmt;
 	u_int8_t  scrambling_code_type;
 	u_int32_t scrambling_code;
 	u_int8_t sf_min;
}__attribute__ ((packed));
typedef struct wcdma_physical_channels_uplinks wcdma_physical_channels_uplinks;
/************************************************************************/

//0x401c /*WCDMA_LIST_SEARCH_VER_5*/
struct per_specified_input_task {
	u_int16_t nc;//3
	u_int8_t nn; //4
	u_int32_t pn_pos;
	u_int16_t scr_code :13; //5
	u_int16_t network_type :3;//6
	u_int8_t ovsf;
	u_int32_t dummy;
}__attribute__ ((packed));
typedef struct per_specified_input_task per_specified_input_task;

struct per_finished_input_task {
	u_int32_t pos[4];
	u_int32_t eng[4];//7 ,8 , 9, 10
	u_int32_t filt_eng[3];//11, 12, 13
}__attribute__ ((packed));
typedef struct per_finished_input_task per_finished_input_task;

struct wcdma_list_serarch_ver_5 {
	int16_t rx_agc;//1
	u_int8_t num_task_spec;//2
	per_specified_input_task p_per_specified_input_task[0];
	per_finished_input_task p_per_finished_input_task[0];
	//u_int8_t ant_id;
	/*ant_id is handeled at decoder level*/
}__attribute__ ((packed));
typedef struct wcdma_list_serarch_ver_5 wcdma_list_serarch_ver_5;
/************************************************************************/

//0x4186 /*wcdma_temporal_analysis_ed2*/

struct per_cell_structure {
	u_int8_t cell_index;
	u_int16_t scr_code_index;
	u_int8_t service_mask;
	u_int16_t index_ref;
	u_int8_t internalfield2;
	u_int16_t internalfield3;
	u_int8_t primary_ref_finger;
	u_int8_t secondary_ref_finger;
	u_int32_t reference_position;
	u_int8_t secondary_cpich_ovsf;
	u_int16_t active_physical_channels;
	u_int32_t internalfield4;
}__attribute__ ((packed ));
typedef struct per_cell_structure per_cell_structure;

struct per_path_structure {
	u_int32_t energy;
	u_int32_t position;
	u_int8_t cell_index;
	u_int8_t finger_index;
	u_int16_t nc;
	u_int8_t nn;
	u_int8_t ant_and_virtual_path_info;
}__attribute__ ((packed ));
typedef struct per_path_structure per_path_structure;

struct per_finger_structure {
	u_int8_t cell_index;
	u_int8_t finger_id;
	u_int8_t finger_lock_status;
	u_int16_t txdiv_cp_eng;
	u_int16_t total_cp_eng;
	int16_t internalfield5;
	u_int8_t internalfield :2;
	u_int8_t paired_finger_index :4;
	u_int8_t finger_antenna :1;
	u_int8_t reserved :1;
	u_int8_t sixTapEn;
}__attribute__ ((packed ));
typedef struct per_finger_structure per_finger_structure;

struct wcdma_temporal_analysis_ed2 {
	u_int8_t version_num;
	u_int8_t num_cell;
	//cell_log
	per_cell_structure per_cell_struct[0];
	//u_int8_t num_paths; //to be handeled at index level
	//paths
	per_path_structure per_path_struct[0];
	//u_int8_t num_fingers; //to be handeled at index level
	//fingure_log
	per_finger_structure per_finger_struct[0];
	//u_int8_t internal_field[33]; //to be handeled at index level
}__attribute__ ((packed ));
typedef struct wcdma_temporal_analysis_ed2 wcdma_temporal_analysis_ed2;

/***********************************************************************************************/

//0x4179 /*wcdma_pn_search_log_packet_ed2*/
struct per_specified_ip_task {
	u_int16_t nc;
	u_int8_t nn;
	u_int8_t antenna_rxd_info : 2;
	u_int8_t reserved : 3;
	u_int8_t carrier_info : 1;
	//u_int8_t reserved1 : 2;
	u_int8_t ovsf : 2;
	u_int16_t pn_pos;
	u_int16_t scr_code;
	u_int16_t reserved5;
	//u_int8_t set
	u_int8_t set_type : 3;
	u_int8_t mbms_set : 2;
	u_int8_t antanna_select : 1;
	u_int8_t reserved2 : 2;
}__attribute__ ((packed ));
typedef struct per_specified_ip_task per_specified_ip_task;

struct per_specified_result_task {
	u_int32_t filter_eng[3];
	u_int32_t position[6];
	u_int32_t energy[6];

}__attribute__ ((packed ));
typedef struct per_specified_result_task per_specified_result_task;

struct wcdma_pn_search_log_packet_ed2 {
	u_int8_t version_num;
	int16_t rx_carr0_agc_0;
	int16_t rx_carr0_agc_1;
	int16_t rx_carr1_agc_0;
	int16_t rx_carr1_agc_1;
	int8_t reserved4;
	int16_t freq_hyp;
	u_int8_t num_task_spec;
	//u_int8_t num_results_per_task;
	u_int8_t pn_pos_lsb;
	per_specified_ip_task perSpecifiedIpTask[0];
	per_specified_result_task perSpecifiedResultTask[0];
}__attribute__ ((packed ));
typedef struct wcdma_pn_search_log_packet_ed2 wcdma_pn_search_log_packet_ed2;

/***********************************************************************************************/

//0x4125 /*WCDMA_RRC_STATES*/
struct wcdma_rrc_states{
   u_int8_t rrc_state;
};
typedef struct wcdma_rrc_states  wcdma_rrc_states;
/************************************************************************/

//0x4127 /*WCDMA_CELL_ID_NEW*/
struct wcdma_cell_id_new {
        u_int32_t ul_uarfcn;
        u_int32_t dl_uarfcn;
        u_int32_t cell_id;
        u_int32_t ura_id;
        u_int16_t psc;
        u_int8_t mcc[3];
        u_int8_t mnc[3];
        u_int32_t lac_id;
        u_int32_t rac_id;
        }__attribute__ ((packed));
typedef struct wcdma_cell_id_new wcdma_cell_id_new;

/***************************************************************************/

//0x4127 /*WCDMA_CELL_ID*/
struct wcdma_cell_id {
   u_int32_t ul_uarfcn;
   u_int32_t dl_uarfcn;
   u_int32_t cell_id;
   u_int16_t ura_id;
   u_int8_t cell_acess_reset;
   u_int8_t cell_accs;
}__attribute__ ((packed));
typedef struct wcdma_cell_id  wcdma_cell_id;

/*************************************************************/

//0x4005  /*WCDMA_CELL_RESELECTION_PACKET*/
struct wcdma_cell_reselection_packet{
  u_int8_t num_wcdma_cells;
  u_int8_t num_gsm_cells;
}__attribute__ ((packed));
typedef struct wcdma_cell_reselection_packet  wcdma_cell_reselection_packet;

struct wcdma_cell_reselection_packet_info{
	u_int16_t rf_channel;
 	u_int16_t  psc;
 	int8_t rscp;
 	int16_t rank_cell_rscp;
 	int8_t ecio;
 	int16_t rank_ecio;
}__attribute__ ((packed));
typedef struct wcdma_cell_reselection_packet_info  wcdma_cell_reselection_packet_info;

struct gsm_cell_reselection_packet_info{
	u_int16_t arfcn;
	u_int16_t bsic;
	int8_t rssi;
	int16_t rank;
}__attribute__ ((packed));
typedef struct gsm_cell_reselection_packet_info  gsm_cell_reselection_packet_info;
/*************************************************************/

struct wcdma_rrc_signaling_messages{
	u_int8_t logical_channel_type;
	u_int8_t rb_identifier;
	u_int16_t length;
	u_int8_t wcdma_rrc_data[0];
}__attribute__ ((packed ));
typedef struct wcdma_rrc_signaling_messages  wcdma_rrc_signaling_messages;

/********************************************************************************************/

void  wcdma_decode_log_msg(int code, unsigned char *buf ,int length, u_int8_t timestamp[8]);

#if 0
/*
 }__attribute__ ((packed ));
typedef struct wcdma_finger_for_each_finger  wcdma_finger_for_each_finger;



struct wcdma_finger_info_num_frig{
u_int8_t num_fing;
wcdma_finger_for_each_finger  wcdma_finger_for_each_finger[0];

}__attribute__ ((packed ));
typedef struct wcdma_finger_info_num_frig  wcdma_finger_info_num_frig;

struct wcdma_finger_info_each_path{
 u_int32_t path_enr;
 u_int32_t pn_pos_path;
 u_int16_t p_cpich_psc;
 u_int8_t unused;
 u_int8_t s_cpich_ssc;
 u_int8_t fing_chan_code_idx;
 u_int8_t fing_idx;
}__attribute__ ((packed ));
typedef struct wcdma_finger_info_each_path   wcdma_finger_info_each_path;

struct wcdma_finger_info{
	u_int32_t tx_pos;
	u_int16_t coh_int_length;
	u_int8_t non_coh_int_length;
	u_int8_t  num_path;
	wcdma_finger_info_each_path  wcdma_finger_info_each_path[0];
}__attribute__ ((packed ));
typedef struct wcdma_finger_info  wcdma_finger_info;


/******************************************************************/
struct  wcdma_temp_analysis_Edition_2_fing_struct{
	u_int8_t 	cell_idx;
	u_int8_t	fing;
	u_int8_t	lock;
	u_int32_t	fing_pos;
	u_int8_t	fing_state;
	u_int16_t	cp_eng;
	u_int16_t	txdiv_cp_eng;
	u_int16_t	tot_cp_eng;
	u_int8_t	reserved;
}__attribute__ ((packed ));
typedef struct wcdma_temp_analysis_Edition_2_fing_struct  wcdma_temp_analysis_Edition_2_fing_struct;

struct wcdma_temp_analysis_Edition_2_fing {
	u_int8_t num_fing;
	wcdma_temp_analysis_Edition_2_fing_struct wcdma_temp_analysis_Edition_2_fing_struct[0];
}__attribute__ ((packed ));
typedef struct  wcdma_temp_analysis_Edition_2_fing wcdma_temp_analysis_Edition_2_fing;


struct wcdma_temp_analysis_Edition_2_path_struct{
	 u_int32_t eng;
	 u_int32_t pos;
	 u_int8_t  cell_idx;
	 u_int8_t 	fing;
	 u_int16_t 	nc;
	 u_int8_t	nn;
	 u_int8_t 	ant_and_vir_path_info;
	 wcdma_temp_analysis_Edition_2_fing  wcdma_temp_analysis_Edition_2_fing[0];
}__attribute__ ((packed ));
typedef struct wcdma_temp_analysis_Edition_2_path_struct wcdma_temp_analysis_Edition_2_path_struct;

struct wcdma_temp_analysis_Edition_2_path{
 u_int8_t num_path;
 wcdma_temp_analysis_Edition_2_path_struct  wcdma_temp_analysis_Edition_2_path_struct[0];
}__attribute__ ((packed ));
typedef struct wcdma_temp_analysis_Edition_2_path wcdma_temp_analysis_Edition_2_path;



struct wcdma_temp_analysis_Edition_2_cell_struct{
	u_int8_t cell_idx;
	u_int16_t  scr_code_idx;
	u_int8_t   svc_mask;
	u_int16_t   index_ref;
	u_int8_t 	pri_ref_fing;
	u_int8_t	sec_ref_fing;
	u_int32_t	ref_pos;
	u_int8_t	sec_cpich_ovsf;
	u_int16_t	active_phy_channels;
wcdma_temp_analysis_Edition_2_path wcdma_temp_analysis_Edition_2_path[0];
}__attribute__ ((packed ));
typedef struct wcdma_temp_analysis_Edition_2_cell_struct wcdma_temp_analysis_Edition_2_cell_struct;
struct wcdma_temp_analysis_Edition_2{
  u_int8_t version_num;
  u_int8_t num_cell;
  wcdma_temp_analysis_Edition_2_cell_struct wcdma_temp_analysis_Edition_2_cell_struct[0];
}__attribute__ ((packed ));
typedef struct wcdma_temp_analysis_Edition_2  wcdma_temp_analysis_Edition_2;
#endif


#endif /* WCDMADECODE_H_ */
