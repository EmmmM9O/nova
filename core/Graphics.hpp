#pragma once
#include <fmt/core.h>

#include <string>

#include "application.hpp"
namespace nova {
class Color {
public:
  float r, g, b, a;
  Color();
  Color(unsigned int rgba8888);
  Color(std::string hex);
  Color(float r, float g, float b, float a);
  void clamp();
  int rgba8888();
  int rgba();
  static int rgba8888(float r, float g, float b, float a);
  std::string toString();
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

private:
  void extractVersion(std::string patternString, std::string versionString);
};
class GLTexture : public Disposable {
public:
  int glTarget;
  int width, height;
  void dispose() override;
};
class Texture : public GLTexture {};
class TextureRegion {
public:
  Texture texture;
  int width, height;
  float u, v, u2, v2;
  float scale = 1.0f;
};
class Mesh : public Disposable {
public:
  int vertexSize;
};
class Batch : public Disposable {
protected:
  Color color = Color(1, 1, 1, 1);
  float z;
  Texture *lastTexture = nullptr;
  bool apply;

public:
  void dispose() override;
  void setColor(Color color);
  void setColor(float r, float g, float b, float a);
  Color getColor();
  virtual void draw(TextureRegion region, float x, float y, float originX,
                    float originY, float width, float height,
                    float rotation) = 0;
};
std::string to_string(GLVersion gl);
class Graphics : public Disposable {
public:
  virtual GLVersion getGLVersion() = 0;
  virtual void update() = 0;
  virtual void destory() = 0;
  virtual void change_surface(int width, int height) = 0;
  virtual bool running() = 0;
  void setupTask();
};
class Draw {
  static Color getColor();
  static void color(Color color);
  static void alpha(float alpha);
};

} // namespace nova
template <> struct fmt::formatter<nova::GlType> : formatter<string_view> {
  auto format(nova::GlType type, format_context &ctx) const;
};
template <> struct fmt::formatter<nova::GLVersion> : formatter<string_view> {
  auto format(nova::GLVersion version, format_context &ctx) const;
};
