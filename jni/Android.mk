LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CFLAGS :=
 ifeq ($(NDK_DEBUG),1)
 	LOCAL_CFLAGS += -g -DDEBUG 
 endif
LOCAL_CFLAGS += -DGSM_ENABLE -DWCDMA_ENABLE -DLTE_ENABLE -DCDMA_ENABLE -DMEMORY_CORRUPTION_REPORT -DMINIMUM_DM
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/decoder/nasDecoder/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/decoder/wcdmarrc/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/decoder/csnRRDecoder/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/decoder/thirdparty/objsys/linux
LOCAL_MODULE := libDM
LOCAL_SRC_FILES := $(wildcard source/*.c)
#LOCAL_SRC_FILES += $(wildcard decoder/nasDecoder/src/*.c)
#LOCAL_SRC_FILES += $(wildcard decoder/wcdmarrc/src/*.c)
#LOCAL_SRC_FILES += $(wildcard decoder/csnRRDecoder/src/*.c)
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
 ifeq ($(NDK_DEBUG),1)
	 LOCAL_CFLAGS := -g -DDEBUG 
 endif 
LOCAL_CFLAGS += -DGSM_ENABLE -DWCDMA_ENABLE -DLTE_ENABLE -DCDMA_ENABLE
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_MODULE := DMExe
LOCAL_SRC_FILES := test_stub/test.c
#LOCAL_SHARED_LIBRARIES := libDM
#LIB_PATH := $(LOCAL_PATH)/decoder/thirdparty/objsys/linux/lib/libasn1rt.a
#LIB_PATH += $(LOCAL_PATH)/decoder/thirdparty/objsys/linux/lib/libasn1per.a
LOCAL_STATIC_LIBRARIES := libDM
LOCAL_LDLIBS = $(LIB_PATH)
include $(BUILD_EXECUTABLE)
