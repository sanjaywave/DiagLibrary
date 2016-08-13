#ifndef _NAS_DECODERWRAPPER_H
#define _NAS_DECODERWRAPPER_H
void decodeNASbuffer(	void         *p_encoded_data,
			u_int32_t    encoded_buflen,
			u_int32_t    type,/*Not used*/
			u_int32_t Id);/*Id = 0 will be recieved here always*/

void NASDecoderCCevents(unsigned char *buf, int length, u_int64_t cmdmillSeconds);


#endif
