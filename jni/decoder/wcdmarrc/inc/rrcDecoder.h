#ifndef RRCDECODER_H_
#define RRCDECODER_H_
typedef unsigned int U32_t;
typedef unsigned char U8_t;

typedef enum{
	RRC_UL_CCCH = 0x00,
	RRC_UL_DCCH = 0x01,
	RRC_DL_CCCH = 0x02,
	RRC_DL_DCCH = 0x03,
	RRC_BCCH_BCH = 0x04,
	RRC_PCCH = 0x06,
	RRC_MCCH = 0x50,
	RRC_MSCH = 0x5F
}rrc_logical_channel_type_et;/*SM: To add remaining*/

int decodeRRC(void *p_encoded_data,
			U32_t                  encoded_buflen,
			rrc_logical_channel_type_et logical_channel,
			U32_t Id);/*Id = 0 will be recieved here always*/
void WCDMAmessageName(unsigned char *buf, int length);
void WCDMADecoderCCevents(char *p_encoded_data,
					 int  encoded_buflen,
					 rrc_logical_channel_type_et logical_channel,
					 u_int64_t cmdmillSeconds);

#endif /* FILERETERIVE_H_ */
