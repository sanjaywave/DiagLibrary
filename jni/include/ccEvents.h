#ifndef CCEVENTS_H_
#define CCEVENTS_H_


/*
 * CC Event Enum
 */
enum
{
	CALL_START_INDICATION = 1, /* DataField : - MO/MT */
	SETUP,  /* Data Field :- Dialed Number */
	CALL_PROCEEDING ,
	PROGRESS ,
	ALERTING ,
	CONNECT ,
	CONNECT_ACK ,
	DISCONNECT  , /* Data field :- Release Cause */
	RELEASE ,
	RELEASE_COMPLETE
};
#endif /* CCEVENTS_H_ */


  
/*  The below is call back functions...
 *  int sendInformation ( int systype, int notifier ,int identifier , char *buf , int length)
 *  All CC event will be sent as a event.
 *  systype : -  WCDMA/GSM
 *  notifier : - EVENT_CC
 *  identifier : - above EVENT Identifier ( CALL_START_INDICATOR/SETUP)
 *  buf[0-7] : -  8 bytes time in millseconds ;
 *  buf[8-X] :- and after that data fields if any.
 *  length :- length of buf ( 
 *
 */

 /*
  * CALL_START_INDICATION ( total bytes will be 12)
  * MO = 0 (int) buf[8 -11]
  * MT = 1 (int) buf[8-11]
  */

 /*
  * SETUP ( total bytes will be 18 bytes)
  * Data field :- Dialed Number Digit [11]
  * Dialed Number = buf[8-18] ( 10 digit Number)
  */

 /*
  * DISCONNECT ,
  * Data field :- AS per Table 10.5.123/3GPP TS 24.008: Cause information element values (int)
  * DISCONNECT CAUSE = buf[8-11]
. */
