
#pragma once
#ifdef __linux__
#include "desktop/include/glad.h"
#endif
namespace nova {
class Gl {
public:
#ifdef __linux__
  using GInt = GLuint;
  using GEnum = GLenum;
#endif
  GInt createShader(GEnum type,const char* data);

#ifdef __window__

#endif
};
} // namespace nova
