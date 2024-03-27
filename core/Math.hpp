#pragma once
#include <array>
namespace nova {
class Mat {
public:
  static const int M00 = 0;
  static const int M01 = 3;
  static const int M02 = 6;
  static const int M10 = 1;
  static const int M11 = 4;
  static const int M12 = 7;
  static const int M20 = 2;
  static const int M21 = 5;
  static const int M22 = 8;
  std::array<float, 9> val;
  Mat();


private:
  std::array<float, 9> tmp;
};
} // namespace nova
