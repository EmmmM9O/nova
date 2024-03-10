#include "Graphics.hpp"

#include <fmt/core.h>
#include <fmt/format.h>

#include <regex>
#include <string>

#include "core/Util.hpp"
#include "core/Log.hpp"
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
  if (type == GlType::GLES) {
    extractVersion(R"(OpenGL ES (\d+(\.\d+){0,2}))", versionString);
  } else if (type == GlType::WebGL) {
    extractVersion("WebGL (\\d(\\.\\d){0,2})", versionString);
  } else if (type == GlType::OpenGL) {
    extractVersion("(\\d(\\.\\d){0,2})", versionString);
  } else {
    majorVersion = -1;
    minorVersion = -1;
    releaseVersion = -1;
    vendorString = "";
    rendererString = "";
  }
}

void GLVersion::extractVersion(std::string patternString,
                               std::string versionString) {
	Log_debug("extract {}",versionString);
  std::regex re(patternString);
  std::smatch m;
  if (std::regex_match(versionString, m, re)) {
	Log_debug("extract successful {}",m[1].str());
    auto resultSplit = split(m[1].str(), '.');
    majorVersion = parseInt(resultSplit[0], 2);
    minorVersion = resultSplit.size() < 2 ? 0 : parseInt(resultSplit[1], 0);
    releaseVersion = resultSplit.size() < 3 ? 0 : parseInt(resultSplit[2], 0);
  } else {
	Log_debug("extract failed {}","error");
    majorVersion = 2;
    minorVersion = 0;
    releaseVersion = 0;
  }
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
