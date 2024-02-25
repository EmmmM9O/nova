LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := nova_core
LOCAL_MODULE_FILENAME := libnova_core

LOCAL_ALL_C_FILES:=$(wildcard core/**.cpp)
LOCAL_SRC_FILES := $(LOCAL_ALL_C_FILES)

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_CFLAGS += -std=c++2a -fexceptions

include $(BUILD_STATIC_LIBRARY)

