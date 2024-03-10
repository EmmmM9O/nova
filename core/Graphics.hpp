#pragma once
#include <any>
#include <string>

#include "application.hpp"
#include "fmt/base.h"

namespace nova {
class Color {
 public:
  float r, g, b, a;
  Color();
  Color(int rgba8888);
  Color(float r, float g, float b, float a);
};

enum class GlType { OpenGL, GLES, WebGL, NONE };
std::string to_string(GlType gl);
class GLVersion {
 public:
  GLVersion();
  std::string vendorString;
  std::string rendererString;
  GlType type;
  int majorVersion;
  int minorVersion;
  int releaseVersion;
  std::string toString();
  GLVersion(systemType appType, std::string vendorString,
            std::string rendererString, std::string versionString);
};
std::string to_string(GLVersion gl);
class Graphics : public Disposable {
 public:
  virtual GLVersion getGLVersion() = 0;
};
}  // namespace nova
template <>
struct fmt::formatter<nova::GlType> : formatter<string_view> {
  auto format(nova::GlType type, format_context &ctx) const;
};
template <>
struct fmt::formatter<nova::GLVersion> : formatter<string_view> {
  auto format(nova::GLVersion version, format_context &ctx) const;
};
