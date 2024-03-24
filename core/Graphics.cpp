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
#include "core/Log.hpp"
#include "core/Util.hpp"
#include "core/application.hpp"
namespace nova {
int Color::rgba8888() {
  return ((int)(r * 255) << 24) | ((int)(g * 255) << 16) |
         ((int)(b * 255) << 8) | (int)(a * 255);
}
int Color::rgba8888(float r, float g, float b, float a) {
  return ((int)(r * 255) << 24) | ((int)(g * 255) << 16) |
         ((int)(b * 255) << 8) | (int)(a * 255);
}
Color::Color(unsigned int rgba8888) {
  r = ((rgba8888 & 0xff000000) >> 24) / 255.0f;
  g = ((rgba8888 & 0x00ff0000) >> 16) / 255.0f;
  b = ((rgba8888 & 0x0000ff00) >> 8) / 255.0f;
  a = ((rgba8888 & 0x000000ff)) / 255.0f;
}
int Color::rgba() { return rgba8888(); }
std::string Color::toString() {
  std::stringstream ss;
  ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(r)
     << std::setw(2) << static_cast<int>(g) << std::setw(2)
     << static_cast<int>(b) << std::setw(2) << static_cast<int>(a);
  return ss.str();
}
Color::Color(std::string hex) {
  std::istringstream iss(hex);
  iss >> std::hex;
  int tmp;
  iss >> tmp;
  r = tmp / 255.0f;
  iss >> tmp;
  g = tmp / 255.0f;
  iss >> tmp;
  b = tmp / 255.0f;
  iss >> tmp;
  a = tmp / 255.0f;
}
Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
  clamp();
}
void Color::clamp() {
  if (r < 0)
    r = 0;
  else if (r > 1)
    r = 1;

  if (g < 0)
    g = 0;
  else if (g > 1)
    g = 1;

  if (b < 0)
    b = 0;
  else if (b > 1)
    b = 1;

  if (a < 0)
    a = 0;
  else if (a > 1)
    a = 1;
}
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
      0.0_asecond, std::chrono::milliseconds(17), -1));
}

}  // namespace nova
auto fmt::formatter<nova::GlType>::format(nova::GlType obj,
                                          format_context &ctx) const {
  return formatter<string_view>::format(to_string(obj), ctx);
}
auto fmt::formatter<nova::GLVersion>::format(nova::GLVersion obj,
                                             format_context &ctx) const {
  return formatter<string_view>::format(to_string(obj), ctx);
}
