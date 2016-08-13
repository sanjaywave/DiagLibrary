#ifndef FREQBAND_H_
#define FREQBAND_H_

/* NvRead */
struct  nv_read_response{
	u_int8_t cmd_code;
	u_int16_t nvItemID;
	u_int8_t buf[128];
	u_int16_t status;
}__attribute__ ((packed));
typedef struct nv_read_response   nv_read_response;

/* Nv Write */
struct  nv_write_response{
	u_int8_t cmd_code;
	u_int16_t nvItemID;
	u_int8_t buf[128];
	u_int16_t status;
}__attribute__ ((packed));
typedef struct nv_write_response  nv_write_response;

u_int64_t ltebandlock_get_lte_freq_band(int device9660);
void ltebandlock_nv_read_response(unsigned char *buf,int length);
void ltebandlock_nv_read_cmd(int fd,u_int16_t NvItem);
void ltebandlock_nv_write_cmd(int fd,u_int16_t NvItem,u_int64_t band);
int  ltebandlock_set_lte_freqband(int fd,u_int64_t band , int device9660);
void ltebandlock_nv_write_response(unsigned char *buf,int length);

unsigned short Wcdma_get_uarfn_value(int fd);
int Wcdma_set_uarfn_value(unsigned short frequency,int fd);


#endif /* FREQBAND_H_ */
