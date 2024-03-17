LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH))

LOCAL_MODULE    := native-lib
LOCAL_MODULE_FILE := native-lib
LOCAL_C_INCLUDES:=$(LOCAL_PATH)\
		  $(LOCAL_PATH)/../../../\
		  $(LOCAL_PATH)/../../../android/include\
		  $(LOCAL_PATH)/../../../android\
		  $(LOCAL_PATH)/../include\
		  $(LOCAL_PATH)/../../../third_party/fmt/include
FILE_LIST:=$(wildcard $(LOCAL_PATH)/*.cpp)
LOCAL_CPPFLAGS += -std=c++20\
		  -frtti
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_WHOLE_STATIC_LIBRARIES := nova_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,../../../android)
