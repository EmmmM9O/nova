#pragma once
#include <fmt/core.h>

#include <string>

#include "application.hpp"
#include "core/Math.hpp"
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
namespace Colors {
extern Color black, darkGray, gray, lightGray, white, clear;
extern float whiteRgba,clearRgba,blackRgba;
extern float blackFloatBits,clearFloatBits,whiteFloatBits;
}
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
  void dispose() override;
};
class Shader : public Disposable {
  void dispose() override;
};
class Blending {
public:
  static Blending *disabled, additive, normal;
  int src, dst, srcAlpha, dstAlpha;
  virtual void apply();
  Blending(int src, int dst, int srcAlpha, int dstAlpha);
  Blending(int src, int dst);
};
class Batch : public Disposable {
protected:
  Mesh mesh;
  Color color = Color(1, 1, 1, 1);
  float z;
  Texture *lastTexture = nullptr;
  Shader *shader, *customShader = nullptr;
  bool ownsShader;
  float mixColorPacked;
  float colorPacked;
  Color mixColor;

  bool apply;
  bool sortAscending = true;
  int idx = 0;
  Mat transformMatrix, projectionMatrix, combinedMatrix;

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
