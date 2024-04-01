#pragma once
#include <fmt/core.h>

#include "application.hpp"
#include "core/Color.hpp"
#include "core/Math.hpp"
#include "core/function.hpp"
#include <filesystem>
#include <string>
#include <vector>
namespace nova {

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
  std::string toString() const;
  GLVersion(systemType appType, const std::string &vendorString,
            const std::string &rendererString,
            const std::string &versionString);

private:
  void extractVersion(const std::string &patternString,
                      const std::string &versionString);
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
public:
  static std::string positionAttribute, texcoordAttribute, mixColorAttribute,
      colorAttribute, normalAttribute;
  static bool pedantic;
  static std::string prependVertexCode, prependFragmentCode;
  void dispose() override;
  Shader(std::string vertexShader, std::string fragmentShader);
  Shader(std::filesystem::path vertexShader,
         std::filesystem::path fragmentShader);
  std::string getLog();
  bool isCompiled();
  int fetchUniformLocation(std::string name, bool pedantic);
  void setUniformi(std::string name, int value);
  void setUniformi(int location, int value);
  void setUniformf(int location, float value);
  void setUniformf(std::string name, float value);
  void setUniformi(int location, int value1, int value2, int value3,
                   int value4);
  void setUniformi(std::string name, int value1, int value2, int value3,
                   int value4);
  void setUniformi(int location, int value1, int value2, int value3);
  void setUniformi(std::string name, int value1, int value2, int value3);
  void setUniformi(int location, int value1, int value2);
  void setUniformi(std::string name, int value1, int value2);
  void bind();
  bool isDisposed();
  void disableVertexAttribute(std::string name);

protected:
  std::string preprocess(std::string source, bool fragment);
  int createProgram();

private:
  void fetchUniforms();
  void fetchAttributes();
  int fetchAttributeLocation(std::string name);
  int fetchUniformLocation(std::string name);

  int linkProgram(int program);
  void compileShaders(std::string vertexShader, std::string fragmentShader);
  int loadShader(int type, std::string source);
  std::string fragmentShaderSource, vertexShaderSource;
  bool _isCompiled, disposed;
  std::string log = "";
  int program, vertexShaderHandle, fragmentShaderHandle;
  std::vector<std::string> uniformNames, attributeNames;
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
  float m_z;
  Texture *lastTexture = nullptr;
  Shader shader, customShader;
  bool ownsShader;
  float mixColorPacked;
  float colorPacked;
  Color mixColor;

  bool apply;
  bool sortAscending = true;
  int idx = 0;
  Mat transformMatrix, projectionMatrix, combinedMatrix;

public:
  void z(float z);
  void setSort(bool sort);
  void setSortAscending(bool ascend);
  Color getMixColor();
  void setMixColor(float r, float g, float b, float a);
  void setMixColor(Color tint);
  float getPackedColor();
  void setPackedColor(float packedColor);
  float getPackedMixColor();
  void setPackedMixColor(float packedColor);

  void dispose() override;
  void setColor(Color color);
  void setColor(float r, float g, float b, float a);
  Color getColor();
  virtual void draw(TextureRegion region, float x, float y, float originX,
                    float originY, float width, float height,
                    float rotation) = 0;
  virtual void draw(Texture texture, float *spriteVertices, int offset,
                    int count);
  void draw(Runnable request);
  virtual void flush();
  void setBlending(Blending blending);
  Blending getBlending();
  void discard();
  Mat getProjection();
  Mat getTransform();
  void setProjection(Mat projection);
  void setTransform(Mat transform);
  void setupMatrices();
  Shader getShader();
  void setShader(Shader shader, bool apply);
  void setShader(Shader shader);
  void switchTexture(Texture texture);
};
class SpriteBatch : public Batch {
public:
  static const int VERTEX_SIZE = 2 + 1 + 2 + 1;
  static const int SPRITE_SIZE = 4 * VERTEX_SIZE;

protected:
  float *vertices;

private:
  int totalRenderCalls = 0;
  int maxSpritesInBatch = 0;

public:
  SpriteBatch();
  SpriteBatch(size_t size);
  SpriteBatch(int size, Shader defaultShader);
  void flush() override;
  void draw(TextureRegion region, float x, float y, float originX,
            float originY, float width, float height, float rotation) override;
  void draw(Texture texture, float *spriteVertices, int offset,
            int count) override;
  static Shader createShader();
};
class SortedSpriteBatch : public SpriteBatch {};
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
  auto format(const nova::GLVersion &version, format_context &ctx) const;
};
