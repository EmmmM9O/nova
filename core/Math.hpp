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
  virtual float getX() const = 0;
  virtual float getY() const = 0;
  virtual bool within(float x, float y, float dst) const;
  virtual bool within(Position *other, float dst) const;
  virtual float dst2(float x, float y) const;
  virtual float dst(float x, float y) const;
  virtual float dst(Position *other) const;
  virtual float dst2(Position *other) const;
  virtual float angleTo(float x, float y) const;
  virtual float angleTo(Position *other) const;
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
  Mat *mul(const Mat &m);
  Mat *mulLeft(const Mat &m);
  Mat *setToRotation(float degrees);
  Mat *setToRotationRad(float radians);
  Mat *setToTranslation(float x, float y);
  Mat *setToScaling(float scaleX, float scaleY);
  std::string toString() const;
  float det() const;
  Mat *inv();
  Mat *set(const Mat &mat);
  Mat *set(const data &values);
  Mat *trn(float x, float y);
  Mat *translate(float x, float y);
  Mat *rotate(float degrees);
  Mat *rotateRad(float radians);
  Mat *scale(float scaleX, float scaleY);
  data &getValues();
  float getRotationRad() const;
  float getRotation() const;
  Mat *scl(float scale);
  Mat *transpose();

private:
  static void mul(data mata, data matb);
  data tmp;
};
std::string to_string(Mat);
} // namespace nova
