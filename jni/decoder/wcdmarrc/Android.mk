LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
ifeq ($(NDK_DEBUG),1)
 	LOCAL_CFLAGS := -g -DDEBUG 
endif
LOCAL_MODULE := WCDMADecoder
LOCAL_SRC_FILES :=  src/rrc.c src/rrcDecoder.c src/rrcPrint.c src/rrcPrintStream.c\
 src/rrcPrintString.c src/rrcSIBDecode.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../thirdparty/objsys/linux
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include
LOCAL_LDLIBS := -ldl -llog
include $(BUILD_STATIC_LIBRARY)
