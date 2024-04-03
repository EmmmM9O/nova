#include "Gl.hpp"

#include <string>
#include <vector>

#include "GLES3/gl3.h"
namespace nova {
void Gl::shaderSource(GUInt shader, const char *source) {
  const GChar *shaderSources[1] = {source};
  shaderSource(shader, 1, shaderSources, NULL);
}
GUInt Gl::createShader(GEnum type) { return glCreateShader(type); }
void Gl::shaderSource(GUInt shader, GSize count, const GChar *const *string,
                      const GInt *length) {
  glShaderSource(shader, count, string, length);
}
void Gl::compileShader(GUInt shader) { glCompileShader(shader); }
void Gl::getShaderiv(GUInt shader, GEnum pname, GInt *params) {
  glGetShaderiv(shader, pname, params);
}
std::string Gl::getShaderInfoLog(GUInt shader) {
  GInt maxsize = 0;
  getShaderiv(shader, INFO_LOG_LENGTH, &maxsize);
  std::vector<GChar> buf(maxsize);
  GSize size;
  getShaderInfoLog(shader, maxsize, &size, &buf[0]);
  return std::string(buf.begin(), buf.begin() + size);
}
void Gl::getShaderInfoLog(GUInt shader, GSize maxLength, GSize *length,
                          GChar *infoLog) {
  glGetShaderInfoLog(shader, maxLength, length, infoLog);
}
GUInt Gl::createProgram() { return glCreateProgram(); }

void Gl::attachShader(GUInt program, GUInt shader) {
  glAttachShader(program, shader);
}
void Gl::linkProgram(GUInt program) { glLinkProgram(program); }
void Gl::getProgramiv(GUInt program, GEnum pname, GInt *params) {
  glGetProgramiv(program, pname, params);
}
void Gl::getProgramInfoLog(GUInt program, GSize maxLength, GSize *length,
                           GChar *infoLog) {
  glGetShaderInfoLog(program, maxLength, length, infoLog);
}
std::string Gl::getProgramInfoLog(GUInt program) {
  GInt maxsize = 0;
  getShaderiv(program, INFO_LOG_LENGTH, &maxsize);
  std::vector<GChar> buf(maxsize);
  GSize size;
  getProgramInfoLog(program, maxsize, &size, &buf[0]);
  return std::string(buf.begin(), buf.begin() + size);
}
void Gl::useProgram(GUInt program) { glUseProgram(program); }
void Gl::deleteShader(GUInt shader) { glDeleteShader(shader); }
void Gl::deleteProgram(GUInt program) { glDeleteProgram(program); }
void Gl::enable(GEnum cap) { glEnable(cap); }
void Gl::disable(GEnum cap) { glDisable(cap); }
void Gl::blendFuncSeparate(GEnum sfactorRGB, GEnum dfactorRGB,
                           GEnum sfactorAlpha, GEnum dfactorAlpha) {
  glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}
}  // namespace nova
