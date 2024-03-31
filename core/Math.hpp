#pragma once
#include <array>
#include <cmath>
#include <string>
namespace nova {
extern float PI;
extern float radiansToDegrees;
extern float radDeg;
extern float degreesToRadians;
class Angles {
public:
  static float angle(float x, float y, float x2, float y2);
};
class Position {
public:
  virtual float getX() = 0;
  virtual float getY() = 0;
  virtual bool within(float x, float y, float dst);
  virtual bool within(Position *other, float dst);
  virtual float dst2(float x, float y);
  virtual float dst(float x, float y);
  virtual float dst(Position *other);
  virtual float dst2(Position *other);
  virtual float angleTo(float x, float y);
  virtual float angleTo(Position *other);
};
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
  using data = std::array<float, 9>;
  data val;
  Mat();
  Mat(data values);
  Mat *setOrtho(float x, float y, float width, float height);
  Mat *idt();
  Mat *mul(Mat m);
  Mat *mulLeft(Mat m);
  Mat *setToRotation(float degrees);
  Mat *setToRotationRad(float radians);
  Mat *setToTranslation(float x, float y);
  Mat *setToScaling(float scaleX, float scaleY);
  std::string toString();
  float det();
  Mat *inv();
  Mat *set(Mat mat);
  Mat *set(data values);
  Mat *trn(float x, float y);
  Mat *translate(float x, float y);
  Mat *rotate(float degrees);
  Mat *rotateRad(float radians);
  Mat *scale(float scaleX, float scaleY);
  data getValues();
  float getRotationRad();
  float getRotation();
  Mat *scl(float scale);
  Mat *transpose();

private:
  static void mul(data mata, data matb);
  std::array<float, 9> tmp;
};
std::string to_string(Mat);
} // namespace nova
