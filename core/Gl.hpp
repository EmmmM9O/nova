
#pragma once
#include <string>
#ifdef __ANDROID__
#include "GLES3/gl3.h"
#else
#ifdef __linux__
#include "desktop/include/glad.h"
#endif
#endif
namespace nova {
using GUInt = GLuint;
using GInt = GLint;
using GSize = GLsizei;
using GChar = GLchar;
using GEnum = GLenum;
class Gl {
public:
  static const GEnum COMPILE_STATUS = GL_COMPILE_STATUS;
  static const GEnum INFO_LOG_LENGTH = GL_INFO_LOG_LENGTH;
  static const GEnum VERTEX_SHADER = GL_VERTEX_SHADER;
  static const GEnum FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
  static const GEnum FALSE = GL_FALSE;
  static const GEnum TRUE = GL_TRUE;
  static GUInt createShader(GEnum type);
  static void shaderSource(GUInt shader, GSize count,
                           const GChar *const *string, const GInt *length);
  static void compileShader(GUInt shader);
  static void getShaderInfoLog(GUInt shader, GSize maxLength, GSize *length,
                               GChar *infoLog);
  static void getShaderiv(GUInt shader, GEnum pname, GInt *params);

  // extra
  static void shaderSource(GUInt shader, const char *source);
  static std::string getShaderInfoLog(GUInt shader);
};
} // namespace nova
