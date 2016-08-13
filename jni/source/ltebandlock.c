#include <stdio.h>
#include <endian.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#include "interfaceapi.h"
#include "errors.h"
#include "decode.h"
#include "diag-cmd.h"
#include "ltebandlock.h"

extern int gManufactureType;
extern pthread_mutex_t sychMutex;

u_int64_t FreqBandSet;
u_int16_t FreqBandReadStatus;
int  FreqBandWriteStatus;
u_int64_t gbandlockFreq;
unsigned short globalUrfcnFreq;



/* function to call Nv item read to get LTE band locked on device  */
void  ltebandlock_nv_read_response(unsigned char *buf,int length)
{
	FreqBandReadStatus = 0xff;/* FF is not possible value */
	FreqBandSet = 0;
	nv_read_response   *nvr = (nv_read_response *)&buf[0];
	memcpy((char*)&FreqBandSet,nvr->buf,8);
	FreqBandReadStatus = nvr->status;
}

u_int64_t ltebandlock_get_lte_freq_band(int device9660)
{
	if(!device9660){
		d_warning("LTEBandLock : get Band ,FreqBandReadStatus = %d ",FreqBandReadStatus);
		if(!FreqBandReadStatus){
			d_warning("LTEBandLock : get Band ,FreqBandSet  = %llx ",FreqBandSet);
			return FreqBandSet;
		}
		else
			return 0;
	}else
		return gbandlockFreq;

}

void ltebandlock_nv_write_response(unsigned char *buf,int length)
{
	FreqBandWriteStatus =-1;
	nv_write_response   *nvr = (nv_write_response *)&buf[0];
	memcpy((char*)&FreqBandSet,nvr->buf,8);
	FreqBandWriteStatus = nvr->status;
}

void ltebandlock_nv_read_cmd(int fd,u_int16_t NvItem){
	int writing_status;
	char *write_buf;
	int len;
	write_buf = (char *)MALLOC(1024);
	if(gManufactureType == SAMSUNG)
	{
		write_buf[0]= 0x20;
		write_buf[1]= 0;
		write_buf[2] =0;
		write_buf[3] =0;
		len= cmd_nv_read(&write_buf[4],1020,NvItem);
		writing_status = write(fd, write_buf, len+4);
		d_log("Read NV into the file \n");
		d_arraylog(write_buf , len);
		if (writing_status == -1)
		{
			printf("Writing error to Diag Device %d \n",errno);
		}
		FREE(write_buf);
	}else printf("This feature is only for Samsung Devices");
}

/* function to call Nv item write for LTE bandlocking */
void ltebandlock_nv_write_cmd(int fd,u_int16_t NvItem,u_int64_t band){

	int writing_status;
	char *write_buf;
	int len;
	write_buf = (char *)MALLOC(1024);
	if(gManufactureType == SAMSUNG)
	{
		write_buf[0]= 0x20;
		write_buf[1]= 0;
		write_buf[2] =0;
		write_buf[3] =0;
		len= cmd_nv_write(&write_buf[4],1020,NvItem,band);
		writing_status = write(fd, write_buf, len+4);
		d_log("Write NV into the file \n");
		d_arraylog(write_buf , len);
		if (writing_status == -1)
		{
			printf("Writing error to Diag Device %d \n",errno);
		}
		FREE(write_buf);
	}else printf("This feature is only for Samsung Devices");
}

/*Function to get LTE band set on device */
u_int64_t ltebandlock_get_uarfn_value(int fd)
{
   u_int64_t freq;
   pthread_mutex_lock(&sychMutex);
   freq = 0;
   if (gManufactureType==SAMSUNG)
   {
	  char path[] = "/nv/item_files/modem/mmode/lte_bandprefa";
	  path[39]= '\0';
	  diag_get_efs_request(fd , 8, path ,LTE_BAND_SEQ_NUM);
	  sleep(1);
	  freq = gbandlockFreq;
   }
   pthread_mutex_unlock(&sychMutex);
   return freq;

}

/* function to set LTE band */
int lte_bandlock(int fd,u_int64_t band )
{
   int returntype ;
   pthread_mutex_lock(&sychMutex);

   if (gManufactureType==SAMSUNG)
   {
	  char path[] = "/nv/item_files/modem/mmode/lte_bandprefa" ;
	  path[39]= '\0';
	  /*printf("strlength of path %d \n", strlen(path));*/
	  diag_put_efs_request(fd , 8, (char *)&band, path);
	  returntype = 0 ;
   }else returntype = -1;
   pthread_mutex_unlock(&sychMutex);
   return returntype;
}


/*  function to select corresponding  value */
int ltebandlock_set_lte_freqband(int fd,u_int64_t band , int device9660)
{
	//u_int64_t LTE_Band=0;
	FreqBandWriteStatus = -1;
       /*
	if (band &  E_UTRA_OPERATING_BAND_1){
	      LTE_Band = LTE_Band | (u_int64_t)0x000000000000001;
	  }
	if (band &  E_UTRA_OPERATING_BAND_2){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000000002;
	  }
	if (band &  E_UTRA_OPERATING_BAND_3){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000000004;
	  }
	if (band &  E_UTRA_OPERATING_BAND_4){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000000008;
	  }
	if (band &  E_UTRA_OPERATING_BAND_5){
	         LTE_Band = LTE_Band | (u_int64_t)0x000000000000010;
	  }
	if (band &  E_UTRA_OPERATING_BAND_6){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000000020;
	  }
	if (band &  E_UTRA_OPERATING_BAND_7){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000000040;
	  }
	if (band &  E_UTRA_OPERATING_BAND_8){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000000080;
	  }
	if (band &  E_UTRA_OPERATING_BAND_9){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000000100;
	  }
	if (band &  E_UTRA_OPERATING_BAND_10){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000000200;
	  }
	if (band &  E_UTRA_OPERATING_BAND_11){
	     LTE_Band = LTE_Band | (u_int64_t)0x000000000000400;
	  }
	if (band &  E_UTRA_OPERATING_BAND_12){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000000800;
	  }
	if (band &  E_UTRA_OPERATING_BAND_13){
	     LTE_Band = LTE_Band | (u_int64_t)0x000000000001000;
	  }
	if (band &  E_UTRA_OPERATING_BAND_14){
	     LTE_Band = LTE_Band | (u_int64_t)0x000000000002000;
	  }
	if (band &  E_UTRA_OPERATING_BAND_17){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000010000;
	  }
	if (band &  E_UTRA_OPERATING_BAND_18){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000020000;
	  }
	if (band &  E_UTRA_OPERATING_BAND_19){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000040000;
	  }
	if (band &  E_UTRA_OPERATING_BAND_20){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000080000;
	  }
	if (band &  E_UTRA_OPERATING_BAND_21){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000000100000;
	  }
	if (band &  E_UTRA_OPERATING_BAND_24){
	     LTE_Band = LTE_Band | (u_int64_t)0x000000000800000;
	  }
	if (band &  E_UTRA_OPERATING_BAND_25){
	       LTE_Band = LTE_Band | (u_int64_t)0x000000001000000;
	  }
	if (band &  E_UTRA_OPERATING_BAND_33){
	     LTE_Band = LTE_Band | (u_int64_t)0x000000100000000;
	  }
	if (band &  E_UTRA_OPERATING_BAND_34){
	 LTE_Band = LTE_Band | (u_int64_t)0x000000200000000;

	  }
	if (band &  E_UTRA_OPERATING_BAND_35){
	 LTE_Band = LTE_Band | (u_int64_t)0x000000400000000;

	  }
	if (band &  E_UTRA_OPERATING_BAND_36){
	 LTE_Band = LTE_Band | (u_int64_t)0x000000800000000;

	  }
	if (band &  E_UTRA_OPERATING_BAND_37){
	 LTE_Band = LTE_Band | (u_int64_t)0x000001000000000;

	  }
	if (band &  E_UTRA_OPERATING_BAND_38){
	 LTE_Band = LTE_Band | 0x000002000000000;

	  }
	if (band &  E_UTRA_OPERATING_BAND_39){
	 LTE_Band = LTE_Band | (u_int64_t)0x000004000000000;

	  }
	if (band &  E_UTRA_OPERATING_BAND_40){
	 LTE_Band = LTE_Band | (u_int64_t)0x000008000000000;

	  }
	if (band &  E_UTRA_OPERATING_BAND_41){
	 LTE_Band = LTE_Band | (u_int64_t)0x000010000000000;

	  }
	if (band &  E_UTRA_OPERATING_BAND_42){
	 LTE_Band = LTE_Band | (u_int64_t)0x000020000000000;

	  }
	if (band &  E_UTRA_OPERATING_BAND_43)
	{
	 LTE_Band = LTE_Band | (u_int64_t)0x000040000000000;
	}
*/
	if(band)
	{
		if(device9660)
			lte_bandlock(fd,band);
		else
		 ltebandlock_nv_write_cmd(fd,0x1AAC,band);
		sleep(1);

	}
	return FreqBandWriteStatus; /* 0 is for success */
}

/* function to set Uarfcn value from device */
int Wcdma_set_uarfn_value(unsigned short frequency,int fd)
{
   int returntype ;
   pthread_mutex_lock(&sychMutex);

   if (gManufactureType==SAMSUNG)
   {
	  char path[] = "/nv/item_files/wcdma/rrc/wcdma_rrc_freq_lock_item" ;
	  path[50]= '\0';
	  diag_put_efs_request(fd , 2, (char *)&frequency, path);
	  returntype = 0 ;
   }else returntype = -1;
   pthread_mutex_unlock(&sychMutex);
   return returntype;

}
/* function to get Uarfcn value from device */
unsigned short Wcdma_get_uarfn_value(int fd)
{
   unsigned short freq;
   pthread_mutex_lock(&sychMutex);
   freq = 0;
   if (gManufactureType==SAMSUNG)
   {
	  char path[] = "/nv/item_files/wcdma/rrc/wcdma_rrc_freq_lock_item";
	  path[50]= '\0';
	  /*printf("strlength of path %d \n", strlen(path));*/
	  diag_get_efs_request(fd , 2, path ,WCDMA_FREQ_SEQ_NUM);
	  sleep(1);
	  freq = globalUrfcnFreq;
   }
   pthread_mutex_unlock(&sychMutex);
  return freq;
}






