LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
ifeq ($(NDK_DEBUG),1)
 	LOCAL_CFLAGS := -g -DDEBUG 
endif
LOCAL_MODULE := NASDecoder
LOCAL_SRC_FILES :=  src/lteNasDecoder.c src/nasDecode.c src/nasDecoderWrapper.c src/nasEncode.c src/nasUtil.c\
 src/TS24007L3.c src/TS24007L3Dec.c src/TS24007L3Enc.c src/TS24007L3PrtToStrm.c src/TS24008IES.c src/TS24008IESDec.c\
 src/TS24008IESEnc.c src/TS24008IESPrtToStrm.c src/TS24008Msgs.c src/TS24008MsgsDec.c src/TS24008MsgsEnc.c\
 src/TS24008MsgsPrtToStrm.c src/TS24011IES.c src/TS24011IESDec.c src/TS24011IESEnc.c src/TS24011IESPrtToStrm.c\
 src/TS24011Msgs.c src/TS24011MsgsDec.c src/TS24011MsgsEnc.c src/TS24011MsgsPrtToStrm.c src/TS24301IES.c\
 src/TS24301IESDec.c src/TS24301IESEnc.c src/TS24301IESPrtToStrm.c src/TS24301Msgs.c src/TS24301MsgsDec.c\
 src/TS24301MsgsEnc.c src/TS24301MsgsPrtToStrm.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../thirdparty/objsys/linux
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../include
LOCAL_LDLIBS := -ldl -llog
include $(BUILD_STATIC_LIBRARY)
