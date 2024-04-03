
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
  static const GEnum LINK_STATUS = GL_LINK_STATUS;
  static const GEnum FALSE = GL_FALSE;
  static const GEnum TRUE = GL_TRUE;
  static const GEnum ONE = GL_ONE;
  static const GEnum BLEND = GL_BLEND;
  static const GEnum SRC_COLOR = GL_SRC_COLOR;
  static const GEnum SRC_ALPHA = GL_ALPHA;
  static const GEnum ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA;
  static GUInt createShader(GEnum type);
  static void shaderSource(GUInt shader, GSize count,
                           const GChar *const *string, const GInt *length);
  static void compileShader(GUInt shader);
  static void getShaderInfoLog(GUInt shader, GSize maxLength, GSize *length,
                               GChar *infoLog);
  static void getShaderiv(GUInt shader, GEnum pname, GInt *params);
  static GUInt createProgram();
  static void attachShader(GUInt program, GUInt shader);
  static void linkProgram(GUInt program);
  static void getProgramiv(GUInt program, GEnum pname, GInt *params);
  static void getProgramInfoLog(GUInt program, GSize maxLength, GSize *length,
                                GChar *infoLog);
  static void useProgram(GUInt program);
  static void deleteShader(GUInt shader);
  static void deleteProgram(GUInt program);
  static void enable(GEnum cap);
  static void disable(GEnum cap);
  static void blendFuncSeparate(GEnum sfactorRGB, GEnum dfactorRGB,
                                GEnum sfactorAlpha, GEnum dfactorAlpha);
  // extra
  static void shaderSource(GUInt shader, const char *source);
  static std::string getShaderInfoLog(GUInt shader);
  static std::string getProgramInfoLog(GUInt program);
};
}  // namespace nova
