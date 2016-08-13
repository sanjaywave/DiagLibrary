#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__


struct lte_pdsch_stat_indication_tb_Info{
u_int32_t harqID :4;
u_int32_t rv :2;
u_int32_t ndi :1;
u_int32_t crc :1;
u_int32_t rnti :4;
u_int32_t tbindex :1;
u_int32_t discardRe :1;
u_int32_t didRecombine :1;
u_int32_t reserved1 :1;
u_int32_t tbsize :14;
u_int32_t reserved2 :2;
u_int16_t mcsIndex :8;
u_int16_t numberOfRbs :8;
}__attribute__((packed));
typedef struct lte_pdsch_stat_indication_tb_Info lte_pdsch_stat_indication_tbInfo;

/*MCH Id and area Id is not used */

struct lte_pdsch_stat_indication_frame_info{
u_int32_t subframe :4;
u_int32_t framenumber :12;
u_int32_t num_of_rbs :8;
u_int32_t num_of_layers :8;
u_int8_t num_of_tb_blocks :8;
u_int8_t reserved;
lte_pdsch_stat_indication_tbInfo mlte_pdsch_stat_indication_tb_Info[0];
/*This also had
 * u_int32_t pmch id
 * u_int32_t area_d
 */
}__attribute__((packed));
typedef struct lte_pdsch_stat_indication_frame_info lte_pdsch_stat_indication_frame_info;

struct lte_pdsch_stat_indication_header{
u_int8_t version;
u_int8_t num_of_records;
u_int16_t reserved;
lte_pdsch_stat_indication_frame_info mlte_pdsch_stat_indication_frame_info[0];
}__attribute__((packed));

typedef struct lte_pdsch_stat_indication_header lte_pdsch_stat_indication_header;



/**********************************************************************************************************/



struct lte_ll1_pucch_csf_ver_v2
{
	u_int32_t start_system_sub_frame : 4 ;
	u_int32_t start_system_frame_num : 10 ;
	u_int32_t csf_tx_mode : 3;
	u_int32_t pucch_reporting_mode : 2;
	u_int32_t pucch_report_type : 2;
	u_int32_t size_bwp : 3;
	u_int32_t num_of_subbands : 4;
	u_int32_t bwindex: 3;
	u_int32_t rank_index : 1;
	u_int32_t sub_band_label :2 ;
	u_int32_t cqi_cw0 : 4;
	u_int32_t cqi_cw1 : 4;
	u_int32_t wideband_pmi : 4;
	u_int32_t reserved1 : 10;
}__attribute__ ((packed));
typedef struct lte_ll1_pucch_csf_ver_v2  lte_ll1_pucch_csf_ver_v2;

struct lte_ll1_pucch_csf
{
  u_int32_t version : 8;
  lte_ll1_pucch_csf_ver_v2  lte_ll1_pucch_csf_ver_v2[0];
}__attribute__ ((packed));
typedef struct lte_ll1_pucch_csf  lte_ll1_pucch_csf;


struct lte_ml1_dci_information_report_records
{
	u_int32_t pdcch_timing_sfn:10;
	u_int32_t  pdcch_timing_subframe_number:4;
	u_int32_t ul_grant_valid:1;
	u_int32_t riv_width:4;
	u_int32_t riv_value:13;

	u_int32_t hopping_flag:1;
	u_int32_t mcs_index:5;
	u_int32_t ndi:1;
	u_int32_t tpc:2;
	u_int32_t cyclic_shift_dmrs:3;
	u_int32_t duplex_mode:1;
	u_int32_t k_of_dci_0:3;
	u_int32_t ul_index_dai:2;
	u_int32_t reserved1:2;
	u_int32_t cqi_request:1;
	u_int32_t start_of_resource_block:7;
	u_int32_t reserved2:4;


	u_int32_t number_of_resource_blocks:7;
	u_int32_t tbs_index:5;
	u_int32_t modulation_type:2;
	u_int32_t redundancy_version_index:2;
	u_int32_t harq_id:3;
	u_int32_t pdcch_order_valid:1;
	u_int32_t dl_grant_valid:1;
	u_int32_t dl_grant_format_type:4;
	u_int32_t num_ack_nak_bits:2;
	u_int32_t dl_grant_tpc_command:3;
	u_int32_t tpc_dci_valid:1;
	u_int32_t tpc_dci_format_type:1;


	u_int32_t tpc_dci_rnti_type:4;
	u_int32_t tpc_dci_tpc_command:3;
	u_int32_t dai:2;
	u_int32_t reserved3:23;

}__attribute__ ((packed));
typedef struct lte_ml1_dci_information_report_records lte_ml1_dci_information_report_records;

struct lte_ml1_dci_information_report{
u_int8_t version;
u_int16_t reserved;
u_int8_t number_of_records;
lte_ml1_dci_information_report_records  lte_ml1_dci_information_report_records[0];
}__attribute__ ((packed));
typedef struct lte_ml1_dci_information_report  lte_ml1_dci_information_report;


void updatedownlinkMSCIndex( u_int8_t * buf , u_int32_t length);
void updateuplinkChannelInfo( u_int8_t *buf,unsigned int length);
void call_for_uplink_mcs_index( u_int8_t *buf,unsigned int length);

#endif /* PARAMETERS_H_ */
