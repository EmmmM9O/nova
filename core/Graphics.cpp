#include "Graphics.hpp"

#include <fmt/core.h>
#include <fmt/format.h>
#include <unistd.h>

#include <chrono>
#include <regex>
#include <sstream>
#include <string>

#include "core/ASync.hpp"
#include "core/Core.hpp"
#include "core/Util.hpp"
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
    extractVersion("OpenGL ES (\\d(\\.\\d){0,2})", versionString);
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
  std::regex re(patternString);
  std::smatch m;
  if (std::regex_search(versionString, m, re)) {
    auto resultSplit = split(m[1].str(), '.');
    majorVersion = parseInt(resultSplit[0], 2);
    minorVersion = resultSplit.size() < 2 ? 0 : parseInt(resultSplit[1], 0);
    releaseVersion = resultSplit.size() < 3 ? 0 : parseInt(resultSplit[2], 0);
  } else {
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
void Graphics::setupTask() {
  nova::Core::context->post(async::Timer(
      [this](auto, async::Timer *timer, auto) -> void {
        update();
        if (!running()) {
          timer->cancel();
        }
      },
      0.0_asecond, std::chrono::milliseconds(16), -1));
}

}  // namespace nova
auto fmt::formatter<nova::GlType>::format(nova::GlType obj,
                                          format_context &ctx) const {
  return fmt::format_to(ctx.out(), "{}", nova::to_string(obj));
}
auto fmt::formatter<nova::GLVersion>::format(const nova::GLVersion &obj,
                                             format_context &ctx) const
     {
  return fmt::format_to(ctx.out(), "{}", nova::to_string(obj));
}
