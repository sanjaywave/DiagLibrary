#ifndef LTENASDECODER_H_
#define LTENASDECODER_H_

#ifdef __cplusplus
extern "C" {
#endif
void decodeLTENASbuffer(void *p_encoded_data,
			u_int32_t    encoded_buflen,
			u_int32_t    type,/*Not used*/
			u_int32_t Id);

/*LTE_NAS_ESM_PLAIN_OTA_INCOMING_MSG*/
struct lte_nas_incoming_msg{
u_int8_t log_version;
u_int8_t std_version;
u_int8_t std_version_major;
u_int8_t std_version_minor;
u_int8_t data[0];
}__attribute__ ((packed));
typedef struct lte_nas_incoming_msg lte_nas_incoming_msg;

/*LTE_NAS_ESM_PLAIN_OTA_OUTGOING_MSG*/
struct lte_nas_outgoing_msg{
u_int8_t log_version;
u_int8_t std_version;
u_int8_t std_version_major;
u_int8_t std_version_minor;
u_int8_t data[0];
}__attribute__ ((packed));
typedef struct lte_nas_outgoing_msg lte_nas_outgoing_msg;

void LTENAShashTableEntry(unsigned char *buf, int length, int direction, u_int64_t cmdmillSeconds);
void NAShashTableEntry(unsigned char *buf, int length, u_int64_t cmdmillSeconds);
void nasDecoder(int key);

#ifdef __cplusplus
}
#endif

#endif /* LTENASDECODER_H_ */
