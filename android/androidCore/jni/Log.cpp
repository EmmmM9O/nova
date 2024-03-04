#include "core/Log.hpp"
#include "android/log.h"
#include <string>
#define LOG_TAG "novaJNI"
namespace nova {
void logger::printCosnole(std::string str, LogLevel level) {
  switch (level) {
  case LogLevel::Info:
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG,"%s",str.c_str());
    break;
  case LogLevel::Error:
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,"%s",str.c_str());
    break;
  case LogLevel::Warn:
    __android_log_print(ANDROID_LOG_WARN, LOG_TAG,"%s",str.c_str());
    break;
  case LogLevel::Debug:
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,"%s",str.c_str());
    break;
  case LogLevel::None:
    __android_log_print(ANDROID_LOG_UNKNOWN, LOG_TAG,"%s",str.c_str());
    break;
  }
}
} // namespace nova
