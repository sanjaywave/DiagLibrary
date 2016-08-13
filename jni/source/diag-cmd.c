#include <string.h>
#include <stdlib.h>
#include <endian.h>
#include "diag-cmd.h"
#include "errors.h"

#define le32toh   

static const u_int16_t crc_table[256] = {
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
    0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
    0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
    0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
    0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
    0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
    0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
    0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
    0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
    0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
    0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
    0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
    0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
    0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
    0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
    0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
    0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
    0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
    0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
    0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
    0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
    0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
    0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
    0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
    0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
    0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
    0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
    0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
    0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

/* Calculate the CRC for a buffer using a seed of 0xffff */
u_int16_t
dm_crc16 (const char *buffer, size_t len)
{
    u_int16_t crc = 0xffff;

    while (len--)
            crc = crc_table[(crc ^ *buffer++) & 0xff] ^ (crc >> 8);
    return ~crc;
}

#define DIAG_ESC_CHAR     0x7D  /* Escape sequence 1st character value */
#define DIAG_ESC_MASK     0x20  /* Escape sequence complement value */


size_t
dm_escape (const char *inbuf,
           size_t inbuf_len,
           char *outbuf,
           size_t outbuf_len)
{
    const char *src = inbuf;
    char *dst = outbuf;
    size_t i = inbuf_len;

    dm_return_val_if_fail (inbuf != NULL, 0);
    dm_return_val_if_fail (inbuf_len > 0, 0);
    dm_return_val_if_fail (outbuf != NULL, 0);
    dm_return_val_if_fail (outbuf_len > inbuf_len, 0);

 if (outbuf_len <= inbuf_len << 1) {
        size_t outbuf_required = inbuf_len + 1; /* +1 for the trailing control char */

        /* Each escaped character takes up two bytes in the output buffer */
        while (i--) {
            if (*src == DIAG_CONTROL_CHAR || *src == DIAG_ESC_CHAR)
                outbuf_required++;
            src++;
        }

        if (outbuf_len < outbuf_required)
            return 0;
    }

 src = inbuf;


    i = inbuf_len;
    while (i--) {
        if (*src == DIAG_CONTROL_CHAR || *src == DIAG_ESC_CHAR) {
            *dst++ = DIAG_ESC_CHAR;
            *dst++ = *src ^ DIAG_ESC_MASK;
        } else
            *dst++ = *src;
        src++;
    }

    return (dst - outbuf);
}


size_t
dm_encapsulate_buffer (char *inbuf,   
                       size_t cmd_len,
                       size_t inbuf_len,
                       char *outbuf,
                       size_t outbuf_len)
{
    u_int16_t crc;
    size_t escaped_len;

    dm_return_val_if_fail (inbuf != NULL, 0);
    dm_return_val_if_fail (cmd_len >= 1, 0);
    dm_return_val_if_fail (inbuf_len >= cmd_len + 2, 0); /* space for CRC */
    dm_return_val_if_fail (outbuf != NULL, 0);

    /* Add the CRC */
    crc = dm_crc16 (inbuf, cmd_len);
    //printf("Encapsulate buffer cmd_len %d ", cmd_len);
    inbuf[cmd_len++] = crc & 0xFF;
    inbuf[cmd_len++] = (crc >> 8) & 0xFF;

    escaped_len = dm_escape (inbuf, cmd_len, outbuf, outbuf_len);
    dm_return_val_if_fail (outbuf_len > escaped_len, 0);
    outbuf[escaped_len++] = DIAG_CONTROL_CHAR;

    return escaped_len;
}
size_t
dm_unescape (const char *inbuf,
             size_t inbuf_len,
             char *outbuf,
             size_t outbuf_len,
             qcdmbool *escaping)
{
    size_t i, outsize;

    dm_return_val_if_fail (inbuf_len > 0, 0);
    dm_return_val_if_fail (outbuf_len >= inbuf_len, 0);
    dm_return_val_if_fail (escaping != NULL, 0);

    for (i = 0, outsize = 0; i < inbuf_len; i++) {
        if (*escaping) {
            outbuf[outsize++] = inbuf[i] ^ DIAG_ESC_MASK;
            *escaping = FALSE;
        } else if (inbuf[i] == DIAG_ESC_CHAR)
            *escaping = TRUE;
        else
            outbuf[outsize++] = inbuf[i];

        /* About to overrun output buffer size */
        if (outsize >= outbuf_len)
            return 0;
    }

    return outsize;
}
/**
 * dm_decapsulate_buffer:
 * @inbuf: buffer in which to look for a QCDM packet
 * @inbuf_len: length of valid data in @inbuf
 * @outbuf: buffer in which to put decapsulated QCDM packet
 * @outbuf_len: max size of @outbuf
 * @out_decap_len: on success, size of the decapsulated QCDM packet
 * @out_used: on either success or failure, amount of data used; caller should
 *  discard this much data from @inbuf before the next call to this function
 * @out_need_more: when TRUE, indicates that more data is required before
 *  and determination about a valid QCDM packet can be made; caller should add
 *  more data to @inbuf before calling this function again.
 *
 * Attempts to retrieve, unescape, and CRC-check a QCDM packet from the given
 * buffer.
 *
 * Returns: FALSE on error (packet was invalid or malformed, or the CRC check
 *  failed, etc) and places number of bytes to discard from @inbuf in @out_used.
 *  When TRUE, either more data is required (in which case @out_need_more will
 *  be TRUE), or a QCDM packet was successfully retrieved from @inbuf and the
 *  decapsulated packet of length @out_decap_len was placed into @outbuf.  In
 *  all cases the caller should advance the buffer by the number of bytes
 *  returned in @out_used before calling this function again.
 **/

qcdmbool
dm_decapsulate_buffer (const char *inbuf,
                       size_t inbuf_len,
                       char *outbuf,
                       size_t outbuf_len,
                       size_t *out_decap_len,
                       size_t *out_used,
                       qcdmbool *out_need_more)
{
    qcdmbool escaping = FALSE;
    size_t i, pkt_len = 0, unesc_len;
    u_int16_t crc, pkt_crc;

    dm_return_val_if_fail (inbuf != NULL, FALSE);
    dm_return_val_if_fail (outbuf != NULL, FALSE);
    dm_return_val_if_fail (outbuf_len > 0, FALSE);
    dm_return_val_if_fail (out_decap_len != NULL, FALSE);
    dm_return_val_if_fail (out_used != NULL, FALSE);
    dm_return_val_if_fail (out_need_more != NULL, FALSE);

    *out_decap_len = 0;
    *out_used = 0;
    *out_need_more = FALSE;

    if (inbuf_len < 4) {
        *out_need_more = TRUE;
        return TRUE;
    }

    /* Find the async control character */
    for (i = 0; i < inbuf_len; i++) {
        if (inbuf[i] == DIAG_CONTROL_CHAR) {
            /* If the control character shows up in a position before a valid
             * DM packet length (4), the packet is malformed.
             */
            if (i < 3) {
                /* Tell the caller to advance the buffer past the control char */
                *out_used = i + 1;
                return FALSE;
            }

            pkt_len = i;
            break;
        }
    }

    /* No control char yet, need more data */
    if (!pkt_len) {
        *out_need_more = TRUE;
        return TRUE;
    }

    /* Unescape first; note that pkt_len */
    unesc_len = dm_unescape (inbuf, pkt_len, outbuf, outbuf_len, &escaping);
    if (!unesc_len) {
        /* Tell the caller to advance the buffer past the control char */
        *out_used = pkt_len + 1;
        return FALSE;
    }

    if (escaping) {
        *out_need_more = TRUE;
        return TRUE;
    }

    /* Check the CRC of the packet's data */
    crc = dm_crc16 (outbuf, unesc_len - 2);
    pkt_crc = outbuf[unesc_len - 2] & 0xFF;
    pkt_crc |= (outbuf[unesc_len - 1] & 0xFF) << 8;
    if (crc != pkt_crc) {
        *out_used = pkt_len + 1; /* packet + CRC + 0x7E */
        return FALSE;
    }

    *out_used = pkt_len + 1; /* packet + CRC + 0x7E */
    *out_decap_len = unesc_len - 2; /* decap_len should not include the CRC */
    return TRUE;
}



/************************************************************
      Following are the command that are generated 
*************************************************************/

size_t
cmd_hdr_rssi_message (char *buf, size_t len)
{
    char cmdbuf[sizeof (DMCmdSubsysHeader) + 2];
    DMCmdSubsysHeader *cmd = (DMCmdSubsysHeader *) &cmdbuf[0];
    dm_return_val_if_fail (buf != NULL, 0);
    dm_return_val_if_fail (len >= sizeof (*cmd) + DIAG_TRAILER_LEN, 0);
    memset (cmd, 0, sizeof (*cmd));
    cmd->code = DIAG_CMD_SUBSYS;
    cmd->subsys_id = 68;
    cmd->subsys_cmd = 0x4002;
    return dm_encapsulate_buffer (cmdbuf, sizeof (*cmd), sizeof (cmdbuf), buf, len);
}

/******************************************************************/

size_t
cmd_hdr_gprs_packet_Logging_message (char *buf, size_t len ,u_int16_t logcode)
{
    char cmdbuf[sizeof (DMCGsmlogPktRequest) + 2];
    DMCGsmlogPktRequest *cmd = (DMCGsmlogPktRequest*) &cmdbuf[0];
    dm_return_val_if_fail (buf != NULL, 0);
    dm_return_val_if_fail (len >= sizeof (*cmd) + DIAG_TRAILER_LEN, 0);
    memset (cmd, 0, sizeof (*cmd));
    cmd->code = DIAG_CMD_SUBSYS;
    cmd->subsys_id = DIAG_SUBSYS_GSM_ID;
    cmd->subsys_cmd_code=DIAG_SUBSYS_CMD_CODE;
    cmd->log_pkt_type = logcode;
    cmd->zero_stats = 1;// Zero all counts and return values
    cmd->additional_info=0;
    return dm_encapsulate_buffer (cmdbuf, sizeof (*cmd), sizeof (cmdbuf), buf, len);
}
size_t
cmd_hdr_Log_On_Demand_Request_Logging_message(char *buf, size_t len ,u_int16_t logcode)
{
    char cmdbuf[sizeof (DMCmdDynamicLog) + 2];
    DMClogOnDemandRequest *cmd = (DMClogOnDemandRequest *) &cmdbuf[0];
    dm_return_val_if_fail (buf != NULL, 0);
    dm_return_val_if_fail (len >= sizeof (*cmd) + DIAG_TRAILER_LEN, 0);
    memset (cmd, 0, sizeof (*cmd));
    cmd->cmdcode = DIAG_CMD_LOG_ON_DEMAND_REQUEST;
    cmd->subsys_cmd_code = logcode;
    return dm_encapsulate_buffer (cmdbuf, sizeof (*cmd), sizeof (cmdbuf), buf, len);
}


size_t
cmd_hdr_dynamic_Logging_message(char *buf, size_t len ,u_int16_t logcode)
{
    char cmdbuf[sizeof (DMCmdDynamicLog) + 2];
    DMCmdDynamicLog *cmd = (DMCmdDynamicLog *) &cmdbuf[0];
    dm_return_val_if_fail (buf != NULL, 0);
    dm_return_val_if_fail (len >= sizeof (*cmd) + DIAG_TRAILER_LEN, 0);
    memset (cmd, 0, sizeof (*cmd));
    cmd->code = DIAG_CMD_DYNAMIC_LOG_CONFIG;
    cmd->logcmd_id = logcode;
    return dm_encapsulate_buffer (cmdbuf, sizeof (*cmd), sizeof (cmdbuf), buf, len);
}

/*********************************************************************/

static size_t
cmd_log_config_new (char *buf,    
                         size_t len,
                         u_int32_t op,
                         u_int32_t equip_id,
                         u_int16_t items[])
{
    DMCmdLogConfig *cmd;
    u_int16_t highest = 0;
    u_int32_t items_len = 0;
    size_t cmdsize = 0, cmdbufsize;
    u_int32_t i;
    u_int16_t log_code;

    dm_return_val_if_fail (buf != NULL, 0);
    dm_return_val_if_fail ((equip_id & 0xFFF0) == 0, 0);

    /* Find number of log items */
    if (items) {
        while (items_len < 4095 && items[items_len]) {
            /* Find highest log item so we can size the items mask */
            log_code = items[items_len] & 0x0FFF;
            if (log_code > highest)
                highest = log_code;
            items_len++;
        }
    }
    cmdsize = sizeof (DMCmdLogConfig) + ((highest + 7) / 8);
    cmdbufsize = cmdsize + DIAG_TRAILER_LEN;

    dm_return_val_if_fail (len >= cmdsize, 0);

    cmd = calloc (1, cmdbufsize);
    cmd->code = DIAG_CMD_LOG_CONFIG;
    cmd->op = htole32 (op);
    d_log("\ncmd->op = %u ",cmd->op);
    cmd->equipid = htole32 (equip_id);
    d_log("\n cmd->equipid = %u ",cmd->equipid);

    if (items) {
        /* Set up the bitmask of log items */
        for (i = 0; i < items_len; i++) {
            log_code = items[i] & 0x0FFF;  /* Strip off equip ID */
            cmd->mask[log_code / 8] |= 1 << log_code % 8;
        }
        cmd->num_items = htole32 (highest);
    }

    return dm_encapsulate_buffer ((char *) cmd, cmdsize, cmdbufsize, buf, len);
}

/******************************************************************************/

size_t
cmd_log_config_set_mask_new (char *buf,               
                                  size_t len,
                                  u_int32_t equip_id,
                                  u_int16_t items[])
{
    return cmd_log_config_new (buf,
                                    len,
                                    DIAG_CMD_LOG_CONFIG_OP_SET_MASK,
                                    equip_id,
                                    items);
}

/******************************************************************************/
size_t
cdm_cmd_event_report_new (char *buf, size_t len, qcdmbool start)
{
    char cmdbuf[4];
    DMCmdEventReport *cmd = (DMCmdEventReport *) &cmdbuf[0];

    dm_return_val_if_fail (buf != NULL, 0);
    dm_return_val_if_fail (len >= sizeof (*cmd) + DIAG_TRAILER_LEN, 0);

    memset (cmd, 0, sizeof (*cmd));
    cmd->code = DIAG_CMD_EVENT_REPORT;
    cmd->on = start ? 1 : 0;

    return dm_encapsulate_buffer (cmdbuf, sizeof (*cmd), sizeof (cmdbuf), buf, len);
}
/*************************************************************************************/
size_t
cdm_cmd_event_mask (char *buf,
                         size_t len,
                         u_int16_t items[])
{
    DMCmdEventMask *cmd;
    u_int16_t highest = 0;
    u_int32_t items_len = 0;
    size_t cmdsize = 0, cmdbufsize;
    u_int32_t i;
    u_int16_t event_code;

    dm_return_val_if_fail (buf != NULL, 0);
    

    /* Find number of log items */
    if (items) {
        while (items_len < 4095 && items[items_len]) {
            /* Find highest log item so we can size the items mask */
            event_code = items[items_len] ;
            if (event_code > highest){
                highest = event_code;
                  
            }    
            items_len++;
        }
    }
    cmdsize = sizeof (DMCmdEventMask) + ((highest + 7) / 8);
    cmdbufsize = cmdsize + DIAG_TRAILER_LEN;

    dm_return_val_if_fail (len >= cmdsize, 0);

    cmd = calloc (1, cmdbufsize);
    cmd->code = DIAG_CMD_EVENT_SET_MASK;
    cmd->len = htole16 (highest);
    
    if (items) {
        /* Set up the bitmask of log items */
        for (i = 0; i < items_len; i++) {
            event_code = items[i] ;  /* Strip off equip ID */
            cmd->mask[event_code / 8] |= 1 << event_code % 8;
        }
    }
   return dm_encapsulate_buffer ((char *) cmd, cmdsize, cmdbufsize, buf, len);
} 
/********************************************************************************/


/*function - EFS2_DIAG_GET Request*/

size_t
cmd_efs_diag_get (char *buf, size_t len,u_int32_t data_len,
                                     u_int32_t path_len, char * path ,u_int16_t seqNumber )
{
    static int i=100;
    int buf_len =0;	
    char cmdbuf[sizeof (DMCmdGetEfs) + path_len+ 2];
    DMCmdGetEfs *cmd = (DMCmdGetEfs *) &cmdbuf[0];
    dm_return_val_if_fail (buf != NULL, 0);
    dm_return_val_if_fail (len >= sizeof (*cmd) + DIAG_TRAILER_LEN, 0);
    memset (cmd, 0, sizeof (*cmd));
    
    cmd->code=DIAG_CMD_SUBSYS;
    cmd->subsys_id = DIAG_CMD_SUBSYS_ID;
    cmd->subsys_cmd_code = DIAG_SUBSYS_GET_CMD_CODE;
    cmd->data_length = data_len;
    cmd->path_length = path_len;
    cmd->sequence_num = seqNumber;  //constant value
    memcpy(cmd->path, path, path_len);
    buf_len = sizeof (DMCmdGetEfs) + path_len ;
    memcpy(buf,cmdbuf,buf_len);
   /*memcpy(&cmd->path[path_len],'\0',1); // Adding the NULL at the end*/
   /* printf(" cmdbuf sizz%d , cmd size%d Path %d,Datalen %d ",sizeof (*cmd),sizeof (cmdbuf),path_len,data_len);*/
    return dm_encapsulate_buffer (cmdbuf, sizeof (*cmd)+path_len, sizeof (cmdbuf), buf, len); /*1 is added for null*/
}
/***********************************************************************************/
/*function - EFS2_DIAG_PUT Request*/

size_t
cmd_efs_diag_put(char *buf, size_t len, u_int32_t data_len, u_int32_t path_len,	
				      char * data, char * path )
{ 
   static int i=200;
   int buf_len =0;	 
   char cmdbuf[sizeof(DMCmdPutEfs)+path_len+data_len+2];
   DMCmdPutEfs *cmd = (DMCmdPutEfs *) &cmdbuf[0];
    dm_return_val_if_fail (buf != NULL, 0);
    dm_return_val_if_fail (len >= sizeof (*cmd) + DIAG_TRAILER_LEN, 0);
  
    memset (cmd, 0, sizeof (*cmd));
    
    cmd->code=DIAG_CMD_SUBSYS;
    cmd->subsys_id = DIAG_CMD_SUBSYS_ID;
    cmd->subsys_cmd_code = DIAG_SUBSYS_PUT_CMD_CODE;
    cmd->data_length = data_len;
    cmd->flags = 0x0040;                            /*0x0040(O_CREATE) is recommended*/
    cmd->sequence_num = i++;                        /*constant value*/
    memcpy(cmd->data, data, data_len);
    memcpy(&cmd->data[data_len] ,path, path_len);
    /* memcpy(&cmd->data[data_len+path_len],'\0',1); // Adding the NULL at the end*/
   buf_len = sizeof (DMCmdGetEfs) + path_len +data_len;
   memcpy(buf,cmdbuf,buf_len);

   return dm_encapsulate_buffer (cmdbuf, sizeof (*cmd)+path_len+data_len, sizeof (cmdbuf), buf, len);
	
}


/* Nv read  */
size_t
cmd_nv_read(char *buf, size_t len ,u_int16_t logcode)
{
   char cmdbuf[sizeof(DMCmdGetNVItem)+2];
   DMCmdGetNVItem *cmd = (DMCmdGetNVItem *) &cmdbuf[0];
   dm_return_val_if_fail (buf != NULL, 0);
   dm_return_val_if_fail (len >= sizeof (*cmd) + DIAG_TRAILER_LEN, 0);
   memset (cmd,'\0', sizeof (*cmd));
   cmd->cmd_code=DIAG_SUBSYS_PUT_CMD_CODE;  /* Nv read cmd */
   cmd->nvItemID= logcode;
   return dm_encapsulate_buffer (cmdbuf, sizeof (*cmd),sizeof (cmdbuf),buf,len);
}

/* Nv write */
size_t
cmd_nv_write(char *buf, size_t len ,u_int16_t logcode,u_int64_t Band)
{
	int i = 0;
   char cmdbuf[sizeof(DMCmdGetNVItem)+2];
   DMCmdGetNVItem *cmd = (DMCmdGetNVItem *) &cmdbuf[0];
   dm_return_val_if_fail (buf != NULL, 0);
   dm_return_val_if_fail (len >= sizeof (*cmd) + DIAG_TRAILER_LEN, 0);
   memset (cmd,'\0', sizeof (*cmd));
   cmd->cmd_code=DIAG_SUBSYS_GET_CMD_CODE; /* Nv write cmd */
   cmd->nvItemID= logcode;
   memcpy(cmd->buf,(char*)&Band,8);
   printf("\nBuffer Data ");
   for(i=0;i < 8;i++){
	 printf("\nBuffer[%d] = %u",i,cmd->buf[i]);
   }
 return dm_encapsulate_buffer (cmdbuf, sizeof (*cmd),sizeof (cmdbuf),buf,len);
}

/*GPS Last Position */
size_t
cmd_gps_last_location(char *buf, size_t len)
{
	Log_Trace (__FILE__,__LINE__,__func__,LOGL_SUCCESS,"Function Called");
	char cmdbuf[sizeof (DMCmdGpsLastLocation) + 2];
	DMCmdGpsLastLocation *cmd = (DMCmdGpsLastLocation *)&cmdbuf[0];
	dm_return_val_if_fail (buf != NULL, 0);
	dm_return_val_if_fail (len >= sizeof (*cmd) + DIAG_TRAILER_LEN, 0);
	memset (cmd,'\0', sizeof (*cmd));
	cmd->cmdcode= DIAG_CMD_SUBSYS;
	cmd->subsys_id = DIAG_GPS_SUBSYS_ID;
	cmd->subsys_cmd_code = DIAG_GPS_SUBSYS_CMD_CODE;
	return dm_encapsulate_buffer (cmdbuf, sizeof (*cmd),sizeof (cmdbuf),buf,len);
}


/* Timestamp request */
size_t
cmd_dm_time_request(char *buf, size_t len )
{
	char cmdbuf[sizeof(DMTimeRequest)+2];
	DMTimeRequest *cmd = (DMTimeRequest *) &cmdbuf[0];
	dm_return_val_if_fail (buf != NULL, 0);
	dm_return_val_if_fail (len >= sizeof (*cmd) + DIAG_TRAILER_LEN, 0);
    memset (cmd,'\0', sizeof (*cmd));
    cmd->cmd_code = DIAG_TIME_REQUEST_CMD_CODE;
 return dm_encapsulate_buffer (cmdbuf, sizeof (*cmd),sizeof (cmdbuf),buf,len);
}
/************************cdma requsets********************/

size_t
cdma_cmd_write(char *buf, size_t len ,u_int16_t logcode)
{
	int i = 0;
	char cmdbuf[sizeof(cdma_request)+2];
    cdma_request *cmd = (cdma_request *) &cmdbuf[0];
    dm_return_val_if_fail (buf != NULL, 0);
    dm_return_val_if_fail (len >= sizeof (*cmd) + DIAG_TRAILER_LEN, 0);
    memset (cmd,'\0', sizeof (*cmd));
    cmd->cmd_code = logcode;
    return dm_encapsulate_buffer (cmdbuf, sizeof (*cmd),sizeof (cmdbuf),buf,len);
}
