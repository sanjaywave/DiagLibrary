LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := CSNDecoder
ifeq ($(NDK_DEBUG),1)
 		LOCAL_CFLAGS := -g -DDEBUG 
endif
LOCAL_SRC_FILES :=  src/CSNRR.c src/CSNRRDec.c src/csnRRDecoder.c src/CSNRRPrtToStrm.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../thirdparty/objsys/linux
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include
LOCAL_LDLIBS := -ldl -llog 
LOCAL_STATIC_LIBRARIES := DMLib util
include $(BUILD_STATIC_LIBRARY)
