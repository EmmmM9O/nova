#include "Graphics.hpp"

#include <fmt/core.h>
#include <fmt/format.h>

#include <string>

#include "core/application.hpp"
namespace nova {
std::string to_string(GlType gl) {
  switch (gl) {
    case GlType::GLES:
      return "GLES";
    case GlType::OpenGL:
      return "OpenGL";
    case GlType::WebGL:
      return "WebGL";
    case GlType::NONE:
      return "NONE";
    default:
      return "NoGLType";
  }
}
GLVersion::GLVersion() {}
GLVersion::GLVersion(systemType appType, std::string vendorString,
                     std::string rendererString, std::string versionString)
    : vendorString(vendorString), rendererString(rendererString) {
  if (appType == systemType::Android)
    type = GlType::GLES;
  else if (appType == systemType::Linux || appType == systemType::Window)
    type = GlType::OpenGL;
  else
    type = GlType::NONE;
}
std::string GLVersion::toString() {
  return to_string(type) + " " + std::to_string(majorVersion) + "." +
         std::to_string(minorVersion) + "." + std::to_string(releaseVersion) +
         " / " + vendorString + " / " + rendererString;
}
std::string to_string(GLVersion version) { return version.toString(); }
}  // namespace nova
auto fmt::formatter<nova::GlType>::format(nova::GlType obj,
                                          format_context &ctx) const {
  return formatter<string_view>::format(to_string(obj), ctx);
}
auto fmt::formatter<nova::GLVersion>::format(nova::GLVersion obj,
                                             format_context &ctx) const {
  return formatter<string_view>::format(to_string(obj), ctx);
}
