#include "Gl.hpp"
#include <string>
#include <vector>
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
  return std::string(buf.data());
}
void Gl::getShaderInfoLog(GUInt shader, GSize maxLength, GSize *length,
                          GChar *infoLog) {
  glGetShaderInfoLog(shader, maxLength, length, infoLog);
}
} // namespace nova
