#ifndef NASDECODE_H_
#define NASDECODE_H_



//0x7130  /*UMTS_NAS_GMM_STATE*/
struct umts_nas_gmm_state{
	u_int8_t gmm_state;
	u_int8_t gmm_substate;
	u_int8_t gmm_updata;

}__attribute__ ((packed));
typedef struct umts_nas_gmm_state umts_nas_gmm_state;

/********************************************************************************/
// 0x7131 /*UMTS_NAS_MM_STATE*/
struct umts_nas_mm_state{
	u_int8_t mm_state;
	u_int8_t mm_substate;
	u_int8_t mm_updata;
}__attribute__ ((packed));
typedef struct umts_nas_mm_state umts_nas_mm_state;

/*********************************************************************************/
/*713A*/ /*UMTS_NAS_SIGNALING*/
struct umts_nas_signaling{
 u_int8_t msg_dir;
 u_int32_t msg_lenth;
 u_int8_t data[0];
}__attribute__ ((packed));
typedef struct umts_nas_signaling umts_nas_signaling;

/*****************************************************************************************/
/*7138*/ /*UMTS_UE_DYNAMIC_ID*/
struct umts_ue_dynamic_id{
	u_int8_t tmsi_ptmsi;  // 0-tmsi 1-ptmsi, 1 byte
	u_int32_t p_tmsi;     //tmsi or ptmsi value
}__attribute__ ((packed));
typedef struct umts_ue_dynamic_id umts_ue_dynamic_id;

/*****************************************************************************************/




void  nas_decode_log_msg(int code, unsigned char *buf ,int length, u_int8_t timestamp[8]);
#endif /* NASDECODE_H_ */
