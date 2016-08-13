#include <stdio.h>
#include <stdlib.h>
#include "TS24301Msgs.h"
#include "rtxsrc/rtxDiag.h"
#include "rtxsrc/rtxDiagBitTrace.h"
#include "rtxsrc/rtxError.h"
#include "rtxsrc/rtxFile.h"
#include "rtxsrc/rtxMemLeakCheck.h"
#include "rtxsrc/rtxPrintStream.h"
#include "nasDecoderWrapper.h"
//#include "hash_table.h"
//#include "bulkBuffer.h"
//#include "errors.h"
#include "lteNasDecoder.h"

static char decodeMsgBuffer[60000];

const char decodefails[]="Decoding Failed";

static char msgName[][64][100] = {
		{{"0"}},{{"0"}},
		{
			"Dummy",
			"Attach request",
			"Attach accept",
			"Attach complete",
			"Attach reject",
			"Detach request",
			"Detach accept",
			"Dummy",
			"Tracking area update request",
			"Tracking area update accept",
			"Tracking area update complete",
			"Tracking area update reject",
			"Extended service request",
			"Dummy",
			"Service reject",
			"Dummy",
			"GUTI reallocation command",
			"GUTI reallocation complete",
			"Authentication request",
			"Authentication response",
			"Authentication reject",
			"Identity request",
			"Identity response",
			"Dummy",
			"Dummy",
			"Dummy",
			"Dummy",
			"Dummy",
			"Authentication failure",
			"Security mode command",
			"Security mode complete",
			"Security mode reject",
			"EMM status",
			"EMM information",
			"Downlink NAS transport",
			"Uplink NAS transport",
			"CS Service notification",
			"Dummy",
			"Dummy",
			"Dummy",
			"Downlink generic NAS transport",
			"Uplink generic NAS transport"
		},
		{{"0"}},{{"0"}},{{"0"}},{{"0"}},{{"0"}},{{"0"}},{{"0"}},
		{
			"Dummy",
			"Activate default EPS bearer context request",
			"Activate default EPS bearer context accept",
			"Activate default EPS bearer context reject",
			"Dummy",
			"Activate dedicated EPS bearer context request",
			"Activate dedicated EPS bearer context accept",
			"Activate dedicated EPS bearer context reject",
			"Dummy",
			"Modify EPS bearer context request",
			"Modify EPS bearer context accept",
			"Modify EPS bearer context reject",
			"Dummy",
			"Deactivate EPS bearer context request",
			"Deactivate EPS bearer context accept",
			"Dummy",
			"PDN connectivity request",
			"PDN connectivity reject",
			"PDN disconnect request",
			"PDN disconnect reject",
			"Bearer resource allocation request",
			"Bearer resource allocation reject",
			"Bearer resource modification request",
			"Bearer resource modification reject",
			"Dummy",
			"ESM information request",
			"ESM information response",
			"Notification",
			"Dummy",
			"Dummy",
			"Dummy",
			"Dummy",
			"Dummy",
			"Dummy",
			"Dummy",
			"Dummy",
			"Dummy",
			"Dummy",
			"Dummy",
			"Dummy",
			"ESM status"
		}
};

void decodeLTENASbuffer(void *p_encoded_data,
			u_int32_t    encoded_buflen,
			u_int32_t    type,/*Not used*/
			u_int32_t Id)/*Id = 0 will be recieved here always*/
{
	TS24301Msg_PDU data;
	OSCTXT	*p_lctxt = (OSCTXT *)MALLOC(sizeof(OSCTXT));

	OSOCTET*     msgbuf = (OSOCTET *)p_encoded_data;

	int          ret;
	OSSIZE       len = encoded_buflen;
	OSBOOL       trace = TRUE, verbose = FALSE;

	rtInitContext (p_lctxt);

	rtxSetDiag (p_lctxt, verbose);

	/* Set message buffer pointer */

	ret = rtxCtxtSetBufPtr (p_lctxt, msgbuf, len);
	if (0 != ret) {
		printf ("\nrtxCtxtSetBufPtr failed; status = %d\n", ret);
		rtxErrPrint (p_lctxt);
		rtxFreeContext (p_lctxt);
		if (NULL != p_lctxt)
		{
			FREE(p_lctxt);
		}
		return ;
	}

	/* Enable bit tracing */

	if (trace) {
		rtxDiagCtxtBitFieldListInit (p_lctxt);
	}

	/* Set protocol version number */

	rtxCtxtSetProtocolVersion (p_lctxt, 8);

	/* Call decode function */

	ret = NASDec_TS24301Msg_PDU (p_lctxt, &data);

	if (verbose) {
		printf ("\nBit fields: 2222 \n");
		rtxDiagBitTracePrint (p_lctxt->pBitFldList, "");
		printf ("\n");

	}

	if (trace) {
		/* Register print-to-stream callback function */
		//rtxSetPrintStream(p_lctxt,writeToBuffer, decodeMsgBuffer);
		/* Invoke generated print-to-stream function */
		asn1PrtToStrm_TS24301Msg_PDU (p_lctxt, "PDU", &data);
	}
	rtxFreeContext (p_lctxt);
	if (NULL != p_lctxt)
	{
		FREE(p_lctxt);
	}
}

#if 0
void LTENAShashTableEntry(unsigned char *buf, int length, int direction, u_int64_t cmdmillSeconds)
{
	int msgid = 0, protocol = 0, i;
	char *ptr;
	unsigned int key = 0;
	char *message;
	//char *msgName = NULL;
	if(direction == 1){
		lte_nas_incoming_msg *lnespoim=(lte_nas_incoming_msg *)&buf[0];
		message = (char *)lnespoim->data;
		ptr = malloc(sizeof(char)*length+sizeof(int)+sizeof(char));
		if(ptr != NULL) {
			*((int *)ptr) = length;
			*((char *)(ptr+sizeof(int))) = 0x01;
			TRACE_LOG("\nLTENAS data size = %d\n\n", length);
			memcpy(ptr+sizeof(int)+sizeof(char), buf, length);
			key = hash_table_add(CATEGORY_NAS, ptr);
		}
	}else {
		lte_nas_outgoing_msg *lnespoim=(lte_nas_outgoing_msg *)&buf[0];
		message = (char *)lnespoim->data;
		ptr = malloc(sizeof(char)*length+sizeof(int)+sizeof(char));
		if(ptr != NULL) {
			*((int *)ptr) = length;
			*((char *)(ptr+sizeof(int))) = 0x02;
			TRACE_LOG("\nLTENAS data size = %d\n\n", length);
			memcpy(ptr+sizeof(int)+sizeof(char), buf, length);
			key = hash_table_add(CATEGORY_NAS, ptr);
		}
	}


	if(((message[0]) & 0x0f) == 2 || ((message[0]) & 0x0f) == 10) {
	    msgid = (message[1] & 0x3f);
	}
	protocol = (message[0] & 0x0f);

	NASsetnetworkEventsNameProto(msgName[protocol][msgid], key, cmdmillSeconds);
	//nasDecoder(key);
}
#endif
#if 0
void nasDecoder(int key)
{
	int msgid = 0;
	char *buff = '\0', *buf = '\0';
	buff = (char *)hash_table_get(key);
	buf = buff + sizeof(int);
	nas_signaling *uns = (nas_signaling*)&buf[0];
	decodeNASbuffer(uns->data, uns->msg_lenth, 0, 0);
	printf("\n\nbuffer : %s\n\n", decodeMsgBuffer);
}
#endif
