LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
$(call import-add-path,$(LOCAL_PATH))
LOCAL_MODULE    := native-lib
LOCAL_MODULE_FILENAME := native-lib
LOCAL_SRC_FILES := native-lib.cpp
LOCAL_WHOLE_STATIC_LIBRARIES := cppformat_static
include $(BUILD_SHARED_LIBRARY)

$(call import-module,../../cppformat)
