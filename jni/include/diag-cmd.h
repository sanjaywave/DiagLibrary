#ifndef LIBQCDM_COMMANDS_H
#define LIBQCDM_COMMANDS_H


typedef u_int8_t qcdmbool;
#ifndef TRUE
#define TRUE ((u_int8_t) 1)
#endif
#ifndef FALSE
#define FALSE ((u_int8_t) 0)
#endif

#define DIAG_CONTROL_CHAR 0x7E
#define DIAG_TRAILER_LEN  3


u_int16_t dm_crc16 (const char *buffer, size_t len);

size_t dm_escape (const char *inbuf,
                  size_t inbuf_len,
                  char *outbuf,
                  size_t outbuf_len);


size_t dm_encapsulate_buffer (char *inbuf,
                              size_t cmd_len,
                              size_t inbuf_len,
                              char *outbuf,
                              size_t outbuf_len);

#define qcdm_return_val_if_fail(e, v) \
{ \
    if (!(e)) { \
        qcdm_warn (0, "failed: " #e "\n"); \
        return v; \
    } \
}
/*********************************************************************/

enum {
    DIAG_CMD_VERSION_INFO = 0,  /* Version info */
    DIAG_CMD_SUBSYS            = 75,  /* Subsystem commands */
    DIAG_CMD_EVENT_REPORT      = 96,  /* Static Event reporting */
    DIAG_CMD_LOG_CONFIG        = 115, /* New logging config command */
    DIAG_CMD_DYNAMIC_LOG_CONFIG = 120,
    DIAG_CMD_EVENT_SET_MASK    = 130,
    DIAG_SUBSYS_PUT_CMD_CODE = 38,
    DIAG_SUBSYS_GET_CMD_CODE = 39,
    DIAG_SUBSYS_GSM_ID  = 8,
    DIAG_SUBSYS_CMD_CODE  = 5,
    DIAG_CMD_LOG_ON_DEMAND_REQUEST= 120, /*LOG_ON_DEMAND_REQUEST command*/
    DIAG_GPS_SUBSYS_ID = 13,
    DIAG_GPS_SUBSYS_CMD_CODE = 4,
    DIAG_TIME_REQUEST_CMD_CODE = 29

};

enum {
    DIAG_CMD_LOG_CONFIG_OP_GET_RANGE = 0x01,
    DIAG_CMD_LOG_CONFIG_OP_SET_MASK = 0x03,
    DIAG_CMD_LOG_CONFIG_OP_GET_MASK = 0x04,
    DIAG_CMD_LOG_CONFIG_OP_GET_LTE_MASK=0X82,
    DIAG_CMD_SUBSYS_ID = 0x13  /* subsystem identifier set to 19 for file */
};

/**********************************************************************/
/* Generic DM command header */
struct DMCmdHeader {
    u_int8_t code;
} __attribute__ ((packed));
typedef struct DMCmdHeader DMCmdHeader;

/* DIAG_CMD_SUBSYS */
struct DMCmdSubsysHeader {
    u_int8_t code;
    u_int8_t subsys_id;
    u_int16_t subsys_cmd;
} __attribute__ ((packed));
typedef struct DMCmdSubsysHeader DMCmdSubsysHeader;

/* logging mask request */
struct logging_mask_request {
    u_int8_t cmd_code;  
    u_int32_t mask;
}__attribute__ ((packed));
typedef struct logging_mask_request logging_mask_request;

struct log_packet_details {
    u_int16_t length;
    u_int16_t command_code;
    u_int8_t timestamp[8];
    u_int8_t data[0];
} __attribute__ ((packed));
typedef struct log_packet_details  log_packet_details ;


struct DMCmdDynamicLog {
    u_int8_t code;
    u_int16_t logcmd_id;   
} __attribute__ ((packed));
typedef struct DMCmdDynamicLog DMCmdDynamicLog;

struct DMCmdLogConfig {
    u_int8_t code;
    u_int8_t pad[3];
    u_int32_t op;
    u_int32_t equipid;
    u_int32_t num_items;
    u_int8_t mask[0];
} __attribute__ ((packed));
typedef struct DMCmdLogConfig DMCmdLogConfig;

struct DMCmdLogConfigRsp {
    u_int8_t code;
    u_int8_t pad[3];
    u_int32_t op;
    u_int32_t result;  /* 0 = success */
    u_int32_t equipid;
    union {
        u_int32_t get_range_items[16];
        struct {
                 u_int32_t num_items;
                 u_int8_t mask[0];
        } get_set_items;
    } u;
} __attribute__ ((packed));
typedef struct DMCmdLogConfigRsp DMCmdLogConfigRsp;

struct DMCmdEventMask {
    u_int8_t code;
    u_int8_t pad[3];
    u_int16_t len;
    u_int8_t mask[0];
} __attribute__ ((packed));
typedef struct DMCmdEventMask DMCmdEventMask;

struct DMCmdEventReport {
    u_int8_t code;
    u_int8_t on;
} __attribute__ ((packed));
typedef struct DMCmdEventReport DMCmdEventReport;

struct DMCmdGetNVItem{
	u_int8_t cmd_code;
	u_int16_t nvItemID;
	u_int8_t buf[128];
	u_int16_t status;
	}__attribute__ ((packed));
typedef struct DMCmdGetNVItem DMCmdGetNVItem;


/* EFS2 Diag get request*/
struct DMCmdGetEfs{
	u_int8_t code;
	u_int8_t subsys_id;
	u_int16_t subsys_cmd_code;
	u_int32_t data_length;
	u_int32_t path_length;
	u_int16_t sequence_num;
	u_int8_t path[0];                     /* variable */
	}__attribute__ ((packed));
typedef struct DMCmdGetEfs DMCmdGetEfs;

/* EFS2 Diag get response */
struct DMCmdGetResponseEfs{
	u_int8_t code;
	u_int8_t subsys_id;
	u_int16_t subsys_cmd_code;
	u_int32_t num_bytes;
	u_int32_t err_num;
	u_int16_t sequence_num;
	u_int8_t data[0];                       /* variable */
}__attribute__((packed));
typedef struct DMCmdGetResponseEfs DMCmdGetResponseEfs;

/* EFS2 DIag put request */
struct DMCmdPutEfs{
	u_int8_t code;
	u_int8_t subsys_id;
	u_int16_t subsys_cmd_code;
	u_int32_t data_length;
	u_int32_t flags;
	u_int16_t sequence_num;
	u_int8_t data[0];
	}__attribute__ ((packed));
typedef struct DMCmdPutEfs DMCmdPutEfs;

/* EFS2 Diag put response */
struct DMCmdPutResponseEfs{
	u_int8_t code;
	u_int8_t subsys_id;
	u_int16_t subsys_cmd_code;
	u_int16_t sequence_num;
	u_int32_t err_num;
}__attribute__ ((packed));
typedef struct DMCmdPutResponseEfs DMCmdPutResponseEfs;

/* Gsm log pkt request */
struct DMCGsmlogPktRequest{
	u_int8_t code;
	u_int8_t subsys_id;
	u_int16_t subsys_cmd_code;
	u_int16_t log_pkt_type;
	u_int8_t  zero_stats;
	u_int8_t additional_info;
}__attribute__ ((packed));
typedef struct DMCGsmlogPktRequest DMCGsmlogPktRequest;

/* Log On Demand Request */
struct DMClogOnDemandRequest
{
	u_int8_t cmdcode;
	u_int16_t subsys_cmd_code;
}__attribute__ ((packed));
typedef struct DMClogOnDemandRequest DMClogOnDemandRequest;

/* GPS Last Location request*/
struct DMCmdGpsLastLocation
{
	u_int8_t  cmdcode;
	u_int8_t  subsys_id;
	u_int16_t subsys_cmd_code;
	u_int32_t reserved;
}__attribute__ ((packed));
typedef struct  DMCmdGpsLastLocation DMCmdGpsLastLocation;

/* TIME request*/
struct DMTimeRequest{
	u_int8_t cmd_code;
}__attribute__ ((packed));
typedef struct  DMTimeRequest DMTimeRequest;




/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *:::::::::::::::::::CDMA Request struct::::::::::::::::::::::::::::::
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/


struct cdma_request
{
	u_int8_t			cmd_code;
}__attribute__ ((packed ));
typedef struct cdma_request cdma_request;





/*********************************************************************************/





size_t cmd_log_config_set_mask_new (char *buf,size_t len,u_int32_t equip_id,u_int16_t items[]);
size_t cmd_hdr_dynamic_Logging_message(char *buf, size_t len ,u_int16_t logcode );
size_t cdm_cmd_event_report_new (char *buf, size_t len, qcdmbool start);
size_t cdm_cmd_event_mask (char *buf,size_t len,u_int16_t items[]);
void myarrayprint(char *buf , int size);
size_t cmd_efs_diag_get (char *buf, size_t len, u_int32_t data_len, u_int32_t path_len, char * path ,u_int16_t seqNumber);
size_t cmd_efs_diag_put(char *buf, size_t len,   u_int32_t data_len,   u_int32_t path_len,  char * data,  char * path);
void *packetProcessing(void * arg);
size_t cmd_gps_last_location(char *buf, size_t len);

/************************************************************************************/

#endif  /* LIBQCDM_COMMANDS_H */



