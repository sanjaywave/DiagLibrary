#ifndef LIBQCDM_ERRORS_H
#define LIBQCDM_ERRORS_H

#include <sys/types.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

enum {  LOGL_ERR = 1, LOGL_WARN = 2, LOGL_DEBUG = 3 ,LOGL_SUCCESS = 4 };

enum {
	DM_LOGL_ERR   = 0x00000001,
	DM_LOGL_WARN  = 0x00000002,
	DM_LOGL_INFO  = 0x00000004,
	DM_LOGL_DEBUG = 0x00000008
};

#define MY_ASSERT(_expression)\
	if(_expression) {\
		d_warning("buffer out of bound\n");\
		return;\
	}\



#define VERIFY_LENGTH(_msg,_buflen,_index)\
({\
		if((sizeof(_msg)+_index) > _buflen)\
		{\
			d_warning("Incoming buffer length is smaller than required ");\
			return;\
		}\
})


/*  
 *  _cmdName is name of command for which version check  to be done
 *   _NilVersion is version of command which have being decoded
 *   _comingVersion is version comming from logs
 */

#define VALIDATE_VERSION(_cmdName,_NilVersion,_comingVersion)\
{\
	if(_NilVersion != _comingVersion)\
	{\
		d_warning("Version error: %s,NiLServerVersion: %d,ComingVersion: %d ",_cmdName,_NilVersion,_comingVersion);\
		break;\
	}\
}

#define VALIDATE_CONFIG_PARAMETER(_path,_length,_maxlength,_msg)\
{\
	if( _length >(_maxlength- 1))\
	{\
		d_warning("%s path is longer than as expected ",_msg);\
		return -1;\
	}\
}

#define dm_return_if_fail(e) \
{ \
    if (!(e)) { \
        dm_warn (0, "failed: " #e "\n"); \
        return; \
    } \
}

#define dm_return_val_if_fail(e, v) \
{ \
    if (!(e)) { \
        dm_warn (0, "failed: " #e "\n"); \
        return v; \
    } \
}

#define d_warning(fmt, args...) d_warning_func("\n%s : %s : %d : WARNING:- ",  __FILE__, __func__, __LINE__);\
d_warning_func(fmt, ##args)

#ifdef DEBUG

	#define  d_log(fmt, args...)    d_log_func("\nFILE: %s, FUNC: %s, LINE: %d\t DM LOG:- ",  __FILE__, __func__, __LINE__);\
						d_log_func(fmt, ##args)
	#define d_arraylog	d_arraylog_func

#else

	#define d_log(fmt, args...)
	#define d_arraylog

#endif

#define dm_dbg(domain, ...) \
	dm_log (__FILE__, __LINE__, __func__, domain, DM_LOGL_DEBUG, ## __VA_ARGS__ )

#define dm_warn(domain, ...) \
	dm_log (__FILE__, __LINE__, __func__, domain, DM_LOGL_WARN, ## __VA_ARGS__ )

#define qcdm_err(domain, ...) \
	dm_log (__FILE__, __LINE__, __func__, domain, DM_LOGL_ERR, ## __VA_ARGS__ )

//extern FILE *malloc_filep;

#define MALLOC(_size) ({\
	char *_ptr = malloc(_size);\
	/*fprintf(malloc_filep, "malloc: %d, %s, 0x%p\n", __LINE__, __func__, _ptr);*/\
	_ptr;\
	})
#define FREE(_ptr) ({\
	/*fprintf(malloc_filep, "free: %d, %s, 0x%p\n", __LINE__, __func__, _ptr);*/\
	free(_ptr);\
})

void dm_log (const char *file,int line, const char *func, int domain, int level,
                const char *format, ...);

void Log_Trace(const char *file,int line,const char *func, int level,
				const char *format, ...);
void d_flush();

int closeDMLogFile();
void d_arraylog_func(char *buf , int size);
void d_log_func(const char *format, ...);
int dm_initialise_logfile_mutex();
void dm_destroy_logfile_mutex();
int enableLog();
int isDirPresent(char *path);

#endif  /* LIBQCDM_ERRORS_H */

