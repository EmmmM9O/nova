#include "Graphics.hpp"

#include <fmt/core.h>
#include <fmt/format.h>
#include <stdexcept>
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
    throw std::runtime_error("Shader contains GL_ES specific code; this should "
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
    res = "#ifdef GL_ES\n"
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
    res = "#ifndef GL_ES\n"
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
  _isCompiled = true;
}
Shader::ShaderKey Shader::loadShader(Shader::ShaderType type,
                                     const std::string &source) {
  ShaderKey shader = Gl::createShader(type);
  if (shader == 0)
    return -1;
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
Shader SpriteBatch::createShader() {}
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

} // namespace nova
auto fmt::formatter<nova::GlType>::format(nova::GlType obj,
                                          format_context &ctx) const {
  return fmt::format_to(ctx.out(), "{}", nova::to_string(obj));
}
auto fmt::formatter<nova::GLVersion>::format(const nova::GLVersion &obj,
                                             format_context &ctx) const {
  return fmt::format_to(ctx.out(), "{}", nova::to_string(obj));
}
