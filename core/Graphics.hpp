#pragma once
#include <any>

#include "application.hpp"
namespace nova {
class Color {
 public:
  float r, g, b, a;
  Color();
  Color(int rgba8888);
  Color(float r, float g, float b, float a);
};
class Graphics : public Disposable {
 private:
  std::any lastCursor;

 public:
};
}  // namespace nova
