LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH))

LOCAL_MODULE    := nova_static
LOCAL_MODULE_FILE := novalib
LOCAL_C_INCLUDES:=$(LOCAL_PATH)\
		  $(LOCAL_PATH)/..\
		  $(LOCAL_PATH)/include\
		  $(LOCAL_PATH)/../third_party/fmt/include
FILE_LIST:=$(wildcard $(LOCAL_PATH)/*.cpp)\
$(wildcard $(LOCAL_PATH)/egl/*.cpp)\
$(wildcard $(LOCAL_PATH)/../core/*.cpp)
LOCAL_CPPFLAGS += -std=c++20\
		  -frtti
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_WHOLE_STATIC_LIBRARIES := fmt_static
LOCAL_LDLIBS:=-landroid\
	      -lGLESv3\
	      -lEGL\
	      -llog

include $(BUILD_SHARED_LIBRARY)

$(call import-module,../third_party/fmt/support/)
