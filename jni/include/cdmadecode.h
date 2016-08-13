/*
 * cdmadecode.h
 * Created on: 23-Aug-2013
 *  File description : This file contains all the structure of cdma
 */

#ifndef CDMADECODE_H_
#define CDMADECODE_H_


 enum
{
	SEARCHER_STATUS_SUBPACKET = 0x01,
	RF_SUBPACKET = 0x02,
	ACTIVE_INFO_SUBPACKET = 0x03,
	FINGER_INFO_SUBPACKET = 0x04,
	FILTERED_INFO_SUBPACKET = 0x05,
	SEARCHER1_SUBPACKET = 0x06,
	SEARCHER4_SUBPACKET = 0x07,
	SLEEP_STATS_SUBPACKET = 0x08,
	QPCH_SUBPACKET = 0x09,
	AFC_SUBPACKET = 0x0A,
	TRAFFIC_DEMOD_STATUS_SUBPACKET = 0x0B,
	CHANNEL_ESTIMATOR_SUBPACKET = 0x0C,
	ACQUISITION_INFO_SUBPACKET = 0x0D,
	TC_CALL_STATISTICS_SUBPACKET = 0x0E,
	RESERVED_1 = 0x0F,
	RESERVED_2 = 0x10,
	RESERVED_3 = 0x11,
	IDLE_DMOD_STATS_SUBPACKET = 0x12
};


/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *::::::::::::::::::::Common For EVDO and CDMA::::::::::::::::::::::::::::::::::::
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

/* Version structure */
struct CdmapktVesion{
u_int8_t version;
u_int8_t numofsubPkt;
u_int16_t reserved;
}__attribute__ ((packed));
typedef struct  CdmapktVesion  CdmapktVesion;

struct cdma_generalized_searcher_log
{
	u_int8_t	version;
	u_int8_t	number_subpackets;
	u_int16_t	reserved;
}__attribute__ ((packed ));
typedef struct cdma_generalized_searcher_log cdma_generalized_searcher_log;


struct subpacket_header
{
	u_int8_t	subpacket_id;
	u_int8_t	subpacket_version;
	u_int16_t	subpacket_size;
}__attribute__ ((packed ));
typedef struct subpacket_header subpacket_header;


/**********rf subpacket***********/
struct rf_subpacket
{
	//subpacket_header	header;
	u_int8_t			rx0_band_class;
	u_int8_t			rx1_band_class;
	u_int16_t			rx0_cdma_channel;
	u_int16_t			rx1_cdma_channel;
	int8_t				rx0_agc;
	int8_t				rx1_agc;
	int16_t				tx_power;
	u_int8_t			tx_gain_adj;
	u_int8_t			tx_power_limit;
}__attribute__ ((packed ));
typedef struct rf_subpacket rf_subpacket;


/**********active info subpacket***********/
struct active_information_odd
{
	u_int16_t			pilot_pn;
	u_int8_t			cell;
	u_int8_t			num_valid_peaks;
	u_int32_t			window_center;
	u_int32_t			support_channel_mask;
	//u_int32_t			peak_pos[max_num_peaks];
	//u_int16_t			peak_energy[max_num_peaks];
	u_int16_t			reserved;
}__attribute__ ((packed ));
typedef struct active_information_odd active_information_odd;


struct active_information_even
{
	u_int16_t			pilot_pn;
	u_int8_t			cell;
	u_int8_t			num_valid_peaks;
	u_int32_t			window_center;
	u_int32_t			support_channel_mask;
	//u_int32_t			peak_pos[max_num_peaks];
	//u_int16_t			peak_energy[max_num_peaks];
}__attribute__ ((packed ));
typedef struct active_information_even active_information_even;


struct active_info_subpacket
{
	//subpacket_header	header;
	u_int16_t			coherent_integration_len;
	u_int8_t			non_coherent_integration_len;
	u_int8_t			truncation_factor;
	u_int8_t			num_actives;
	u_int8_t			max_num_peaks;
	u_int16_t			referance_pn;
	//active_information	active_information[];
}__attribute__ ((packed ));
typedef struct active_info_subpacket active_info_subpacket;


/**********finger info subpacket***********/
struct finger_info
{
	u_int16_t			pilot_pn;
	u_int16_t			rssi;
	u_int16_t			rx0_rssi;
	u_int16_t			rx1_rssi;
	u_int32_t			position;
	u_int32_t			enabled_channel_mask;
	u_int16_t			status;
	u_int16_t			reserved;
}__attribute__ ((packed ));
typedef struct finger_info finger_info;

struct finger_info_subpacket
{
	//subpacket_header	header;
	u_int8_t			pilot_filter_gain_factor;
	u_int8_t			reserved;
	u_int8_t			antenna_config;
	u_int8_t			num_fingers;
	finger_info			finger_info[0];
}__attribute__ ((packed ));
typedef struct finger_info_subpacket finger_info_subpacket;


/**********sleep stats subpacket***********/
struct sleep_stats_subpacket
{
	//subpacket_header	header;
	u_int8_t			slot_type;
	u_int8_t			sci;
	u_int16_t			slot_number;
	u_int8_t			reacq_data_packet_id;
	u_int8_t			reacq_data_packet_size;
	u_int8_t			technology_data_packet_id;
	u_int8_t			technology_data_packet_size;
}__attribute__ ((packed ));
typedef struct sleep_stats_subpacket sleep_stats_subpacket;


//to be added if (reacq_data_packet_id==1)
struct reacq_data_packet
{
	u_int16_t			rf_band_class_and_channel;
	u_int16_t			pilot_pn;
	int16_t				slew;
	u_int16_t			time;
	u_int8_t			type;
	u_int8_t			neighbors_searched;
	u_int16_t			old_active_pilot;
	int32_t				reacq_linear_ecio;
	u_int32_t			old_active_linear_ecio;
}__attribute__ ((packed ));
typedef struct reacq_data_packet reacq_data_packet;


//to be added if ((reacq_data_packet_id==2)||(reacq_data_packet_id==3))
struct prio_data_packet
{
	u_int8_t			reserved[3];
	u_int8_t			neighbors_searched;
}__attribute__ ((packed ));
typedef struct prio_data_packet prio_data_packet;


//to be added if ((technology_data_packet_id==1)||(technology_data_packet_id==2)||(technology_data_packet_id==3))
struct technology_data_packet_online
{
	u_int32_t			sleep_time;
	u_int16_t			rf_agc_decode_boundary;
	u_int16_t			setup_time;
}__attribute__ ((packed ));
typedef struct technology_data_packet_online technology_data_packet_online;


//to be added if (technology_data_packet_id==4)
struct technology_data_packet_offline
{
	u_int32_t			sleep_time;
	u_int16_t			rf_agc_wakeup;
	u_int16_t			rf_agc_recording_end;
	u_int16_t			timeline_active_time;
	u_int16_t			record_time;
	u_int8_t			antenna;
	u_int8_t			queue;
	u_int8_t			num_paths;
	u_int8_t			reserved;
}__attribute__ ((packed ));
typedef struct technology_data_packet_offline technology_data_packet_offline;




/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *::::::::::::::::::Command for EVDO only:::::::::::::::::::::::::::::
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

/****************************************************************/

struct carrier_info
{
		u_int8_t			carrier_id[3];
		u_int8_t			power_info;
		int16_t				tx_open_loop_adj;
		int16_t				tx_closed_loop_adj;
		int16_t				tx_pilot_power;
		int16_t				total_tx_power_carrier;
		int16_t				rx_agc0;
		int16_t				rx_agc1;
}__attribute__ ((packed ));
typedef struct carrier_info carrier_info;

struct evdo_multicarrier_power
{
	u_int8_t			version;
	u_int8_t			lna_state;
	u_int8_t			tx_enabled;
	u_int16_t			hstr;
	int16_t				total_tx_power;
	u_int8_t			num_carrier;
	carrier_info		carrier_data[0];
}__attribute__ ((packed ));
typedef struct evdo_multicarrier_power evdo_multicarrier_power;



/****************************************************************/

struct finger_struct
{
	u_int16_t			pilot_pn;
	u_int32_t			rtc_offset;
	u_int16_t			rssi;//as per qxdm //fix
	u_int8_t			multiple_data; // lockflag[0], antenna[1], diversity[2], finger index [6 to 3]
	u_int8_t			idx; //rpc[2:0] , aspidx[5:3]
	u_int16_t			antenna0_c_i;
	u_int16_t			antenna1_c_i;
}__attribute__ ((packed ));
typedef struct finger_struct finger_struct;


struct evdo_finger
{
	u_int8_t			srch_state;
	u_int32_t			mstr;
	u_int16_t			mstr_error;
	u_int16_t			mstr_pilot_pn;
	u_int8_t			num_fingers;
	finger_struct		finger[0];
}__attribute__ ((packed ));
typedef struct evdo_finger evdo_finger;


/*******************************************************************/

struct evdo_power
{
	u_int8_t			state;
	int16_t				tx_open_loop_adj;
	int16_t				tx_closed_loop_adj;
	int16_t				tx_pilot_power;
	int16_t				tx_total_power;
	int16_t				rx_agc0;
	int16_t				rx_agc1;
}__attribute__ ((packed ));
typedef struct evdo_power evdo_power;

/*******************************************************************/

struct evdo_air_link_summary
{
	u_int8_t			DRCBuffer[8];
	u_int16_t			Best_ASP_SINR_Buffer[16];
	u_int8_t			flags;
	u_int16_t			ASP_Filtered_SINR[6];
	u_int16_t			Pilot_PN_ASP[6];
	u_int16_t			RPC[6];
	u_int16_t			PER_Inst;
	u_int16_t			PER_Sequence;
	u_int16_t			HSTR;
	u_int8_t			RPC_Cell_Index[6];
}__attribute__ ((packed ));
typedef struct evdo_air_link_summary evdo_air_link_summary;

/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *:::::::::::::::::::::::Response struct::::::::::::::::::::::::::::::
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

/*****************************FOR FER**********************************/

struct temporal_analyser_graph_response
{
	u_int8_t			cmd_code;
	u_int8_t			rx_agc;
	u_int8_t			tx_adj;
	u_int8_t			tx_voc_rate;
	u_int8_t			rx_voc_rate;
	u_int32_t			bad_frames;
	u_int32_t			total_frames;
}__attribute__ ((packed ));
typedef struct temporal_analyser_graph_response temporal_analyser_graph_response;


/*****************************FOR AMSS**********************************/
/*
struct extended_build_id_response
{
	u_int8_t			cmd_code;
	u_int8_t			version;
	u_int16_t			reserved[2];
	//u_int16_t			msm_revision; if version==0
	//u_int20_t			msm_revision; if version==1
	//u_int32_t			msm_revision; if version==2 //amss
	//u_int16_t			reserved; if version==0
	//u_int12_t			reserved; if version==1
	//u_int0_t			reserved; if version==2
	u_int32_t			msm_revision; //msm_revision+reserved = 4 bytes
	u_int32_t			mobile_model_number;
	//variable mobile_software_revision;
	//varialble mobile_model_string;
}__attribute__ ((packed ));
typedef struct extended_build_id_response extended_build_id_response;



/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *:::::::::::::::::::1xRTT EVENT struct:::::::::::::::::::::::::::::::
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

/****************************************************************/
/*
struct cm_serving_system_event_1
{
	u_int8_t			version;
	u_int32_t			ss_event;
}__attribute__ ((packed ));
typedef struct cm_serving_system_event_1 cm_serving_system_event_1;

//changed_field 4 or 8 bytes depending on version

struct cm_serving_system_event_2
{
	u_int32_t			srv_status;
	u_int32_t			srv_domain;
	u_int32_t			srv_capability;
	u_int32_t			sys_mode;
	u_int32_t			roam_status;
}__attribute__ ((packed ));
typedef struct cm_serving_system_event_2 cm_serving_system_event_2;

//sys_id 20 or 37 bytes depending on AMSS target release

struct cm_serving_system_event_3
{
	u_int8_t			is_sys_forbidden;
	u_int8_t			is_colloc;
	u_int16_t			rssi;
	u_int16_t			ecio;
	u_int32_t			io;
	u_int8_t			sinr;
}__attribute__ ((packed ));
typedef struct cm_serving_system_event_3 cm_serving_system_event_3;
//rest packet not needed

*/

#endif

