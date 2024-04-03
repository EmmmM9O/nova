#include "Graphics.hpp"

#include <fmt/core.h>
#include <fmt/format.h>
#include <unistd.h>

#include <chrono>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

#include "core/ASync.hpp"
#include "core/Core.hpp"
#include "core/Log.hpp"
#include "core/Util.hpp"
#include "core/application.hpp"
#include "core/function.hpp"
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
GLVersion::GLVersion(systemType appType, const std::string &vendorString,
                     const std::string &rendererString,
                     const std::string &versionString)
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
    this->vendorString = "";
    this->rendererString = "";
  }
}

void GLVersion::extractVersion(const std::string &patternString,
                               const std::string &versionString) {
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
std::string GLVersion::toString() const {
  return to_string(type) + " " + std::to_string(majorVersion) + "." +
         std::to_string(minorVersion) + "." + std::to_string(releaseVersion) +
         " / " + vendorString + " / " + rendererString;
}

Shader::Shader(const char *vertexShader, const char *fragmentShader)
    : Shader(std::string(vertexShader), std::string(fragmentShader)) {}
Shader::Shader(const std::string &vertexShader_,
               const std::string &fragmentShader_) {
  if (vertexShader_.empty())
    throw std::invalid_argument("vertex shader must not be null");
  if (fragmentShader_.empty())
    throw std::invalid_argument("fragment shader must not be null");
  auto vertexShader = preprocess(vertexShader_, false);
  auto fragmentShader = preprocess(fragmentShader_, true);
  if (!prependVertexCode.empty())
    vertexShader = prependVertexCode + vertexShader;
  if (!prependFragmentCode.empty())
    fragmentShader = prependFragmentCode + fragmentShader;
  this->vertexShaderSource = vertexShader;
  this->fragmentShaderSource = fragmentShader;
  compileShaders(vertexShader, fragmentShader);
  if (isCompiled()) {
    fetchAttributes();
    fetchUniforms();
  } else {
    throw std::invalid_argument("Failed to compile shader: " + log);
  }
}
Shader::Shader(const std::filesystem::path &vertexShader,
               const std::filesystem::path &fragmentShader)
    : Shader(readFile(vertexShader), readFile(fragmentShader)) {
  if (!log.empty()) {
    Log_warn("Shader {} | {}:\n{}", vertexShader.string(),
             fragmentShader.string(), log);
  }
}
void Shader::apply() {}
std::string preprocess(const std::string &source, bool fragment) {
  if (source.find("#ifdef GL_ES") != source.npos) {
    throw std::runtime_error(
        "Shader contains GL_ES specific code; this should "
        "be handled by the preprocessor. Code: \n```\n" +
        source + "\n```");
  }
  if (source.find("#version") != source.npos) {
    throw std::runtime_error(
        "Shader contains explicit version requirement; this should be handled "
        "by the preprocessor. Code: \n```\n" +
        source + "\n```");
  }
  std::string res;
  if (fragment) {
    res =
        "#ifdef GL_ES\n"
        "precision " +
        std::string(source.find("#define HIGHP") != source.npos &&
                            source.find("//#define HIGHP") == source.npos
                        ? "highp"
                        : "mediump") +
        " float;\n"
        "precision mediump int;\n"
        "#else\n"
        "#define lowp  \n"
        "#define mediump \n"
        "#define highp \n"
        "#endif\n" +
        source;
  } else {
    res =
        "#ifndef GL_ES\n"
        "#define lowp  \n"
        "#define mediump \n"
        "#define highp \n"
        "#endif\n" +
        source;
  }
  return res;
}
void Shader::compileShaders(const std::string &vertexShader,
                            const std::string &fragmentShader) {
  vertexShaderHandle = loadShader(Gl::VERTEX_SHADER, vertexShader);
  fragmentShaderHandle = loadShader(Gl::FRAGMENT_SHADER, fragmentShader);
  if (vertexShaderHandle == -1 || fragmentShaderHandle == -1) {
    _isCompiled = false;
    return;
  }
  program = linkProgram(createProgram());
  if (program == -1) {
    _isCompiled = false;
    return;
  }

  _isCompiled = true;
}
std::string Shader::getLog() const { return log; }
bool Shader::isCompiled() const { return _isCompiled; }
bool Shader::isDisposed() const { return disposed; }
void Shader::bind() { Gl::useProgram(program); }
void Shader::dispose() {
  if (disposed) return;
  Gl::useProgram(0);
  Gl::deleteShader(vertexShaderHandle);
  Gl::deleteShader(fragmentShaderHandle);
  Gl::deleteProgram(program);
  disposed = true;
}
Shader::ShaderKey Shader::loadShader(Shader::ShaderType type,
                                     const std::string &source) {
  ShaderKey shader = Gl::createShader(type);
  if (shader == 0) return -1;
  Gl::shaderSource(shader, source.c_str());
  Gl::compileShader(shader);
  GInt isCompiled = 0;
  Gl::getShaderiv(shader, Gl::COMPILE_STATUS, &isCompiled);
  std::string infoLog = Gl::getShaderInfoLog(shader);
  if (!infoLog.empty()) {
    log += type == Gl::VERTEX_SHADER ? "Vertex shader\n" : "Fragment shader:\n";
    log += infoLog;
  }
  if (isCompiled == Gl::FALSE) {
    return -1;
  }
  return shader;
}
Shader::ProgramKey Shader::createProgram() {
  Shader::ProgramKey program = Gl::createProgram();
  return program != 0 ? program : -1;
}
Shader::ProgramKey Shader::linkProgram(Shader::ProgramKey program) {
  if (program == -1) return -1;
  Gl::attachShader(program, vertexShaderHandle);
  Gl::attachShader(program, fragmentShaderHandle);
  Gl::linkProgram(program);
  GInt linked;
  Gl::getProgramiv(program, Gl::LINK_STATUS, &linked);
  if (linked == Gl::FALSE) {
    log = Gl::getProgramInfoLog(program);
    return -1;
  }
  return program;
}

void Blending::apply() {
  Gl::enable(Gl::BLEND);
  Gl::blendFuncSeparate(src, dst, srcAlpha, dstAlpha);
}

Blending::Blending(GEnum src, GEnum dst, GEnum srcAlpha, GEnum dstAlpha)
    : src(src), dst(dst), srcAlpha(srcAlpha), dstAlpha(dstAlpha) {}

Blending::Blending(GEnum src, GEnum dst)
    : src(src), dst(dst), srcAlpha(src), dstAlpha(dst) {}
DisableBlending::DisableBlending(GEnum src, GEnum dst, GEnum srcAlpha,
                                 GEnum dstAlpha)
    : Blending(src, dst, srcAlpha, dstAlpha) {}
DisableBlending::DisableBlending(GEnum src, GEnum dst) : Blending(src, dst) {}
Blending Blending::normal = Blending(Gl::SRC_ALPHA, Gl::ONE_MINUS_SRC_ALPHA);
Blending Blending::additive = Blending(Gl::SRC_ALPHA, Gl::ONE);
void DisableBlending::apply() { Gl::disable(Gl::BLEND); }
DisableBlending DisableBlending::disable =
    DisableBlending(Gl::SRC_ALPHA, Gl::ONE_MINUS_SRC_ALPHA);
bool Blending::equal(Blending *blending) {
  return src == blending->src && dst == blending->dst &&
         srcAlpha == blending->srcAlpha && dstAlpha == blending->dstAlpha;
}

void Batch::z(float z) { m_z = sortAscending ? z : -z; }
void Batch::setSort(bool sort) {}
void Batch::setSortAscending(bool ascend) { sortAscending = ascend; }
void Batch::setColor(const Color &tint) {
  color.set(tint);
  colorPacked = tint.toFloatBits();
}
void Batch::setColor(float r, float g, float b, float a) {
  color.set(r, g, b, a);
  colorPacked = color.toFloatBits();
}
Color Batch::getColor() const { return color; }
Color Batch::getMixColor() const { return mixColor; }
void Batch::setPackedColor(float packedColor) {
  this->color.abgr8888(packedColor);
  this->colorPacked = packedColor;
}
void Batch::setPackedMixColor(float packedColor) {
  this->mixColor.abgr8888(packedColor);
  this->mixColorPacked = packedColor;
}
float Batch::getPackedColor() const { return colorPacked; }
float Batch::getPackedMixColor() const { return mixColorPacked; }
void Batch::setMixColor(const Color &tint) {
  mixColor.set(tint);
  mixColorPacked = tint.toFloatBits();
}
void Batch::setMixColor(float r, float g, float b, float a) {
  mixColor.set(r, g, b, a);
  mixColorPacked = mixColor.toFloatBits();
}
void Batch::draw(Runnable request) { request(); }
void Batch::setBlending(Blending *blending) {
  if (!this->blending->equal(blending)) {
    flush();
  }
  this->blending = blending;
}
Blending *Batch::getBlending() { return blending; }
void Batch::dispose() {
  mesh.dispose();
  if (ownsShader) shader.dispose();
}
Shader SpriteBatch::createShader() {
  return Shader(
      "attribute vec4 a_position;"
      "attribute vec4 a_color;"
      "attribute vec2 a_texCoord0;"
      "attribute vec4 a_mix_color;"
      "uniform mat4 u_projTrans;"
      "varying vec4 v_color;"
      "varying vec4 v_mix_color;"
      "varying vec2 v_texCoords;"
      ""
      "void main(){"
      "   v_color = a_color;"
      "   v_color.a = v_color.a * (255.0/254.0);"
      "   v_mix_color = a_mix_color;"
      "   v_mix_color.a *= (255.0/254.0);"
      "   v_texCoords = a_texCoord0;"
      "   gl_Position = u_projTrans * a_position;"
      "}",

      ""
      "varying lowp vec4 v_color;"
      "varying lowp vec4 v_mix_color;"
      "varying highp vec2 v_texCoords;"
      "uniform highp sampler2D u_texture;"
      ""
      "void main(){"
      "  vec4 c = texture2D(u_texture, v_texCoords);"
      "  gl_FragColor = v_color * mix(c, vec4(v_mix_color.rgb, c.a), "
      "v_mix_color.a);"
      "}");
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
                                             format_context &ctx) const {
  return fmt::format_to(ctx.out(), "{}", nova::to_string(obj));
}
