#ifndef CSNRRDECODER_H_
#define CSNRRDECODER_H_

#ifdef __cplusplus
extern "C" {
#endif

int decodegsmRRBuffer(void *p_encoded_data,
		u_int32_t    encoded_buflen,
		u_int32_t    type,/*Not used*/
		u_int32_t Id);

void CSNDecoderCCevents(void *encoded_data, int length, u_int64_t cmdmillSeconds);

void setTMSI(u_int32_t network_tmsi);

#ifdef __cplusplus
}
#endif
#endif
