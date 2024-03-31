#include "Math.hpp"

#include "fmt/format.h"
#include "math.h"
namespace nova {
float PI = M_PI;
float radiansToDegrees = (180.0f) / PI;
float radDeg = radiansToDegrees;
float degreesToRadians = (PI / 180.0f);
float Angles::angle(float x, float y, float x2, float y2) {
  float ang = atan2(x2 - x, y2 - y) * radDeg;
  if (ang < 0)
    ang += 360.0f;
  return ang;
}
bool Position::within(float x, float y, float dst) {
  return dst2(x, y) <= dst * dst;
}
bool Position::within(Position *other, float dst) {
  return within(other->getX(), other->getY(), dst);
}
float Position::dst2(float x, float y) {
  const float dx = getX() - x;
  const float dy = getY() - y;
  return dx * dx + dy * dy;
}
float Position::dst(float x, float y) {
  const float dx = getX() - x;
  const float dy = getY() - y;
  return sqrt(dx * dx + dy * dy);
}
float Position::dst(Position *other) {
  return dst(other->getX(), other->getY());
}
float Position::dst2(Position *other) {
  return dst2(other->getX(), other->getY());
}
float Position::angleTo(float x, float y) {
  return Angles::angle(getX(), getY(), x, y);
}
float Position::angleTo(Position *other) {
  return angleTo(other->getX(), other->getY());
}
Mat::Mat() { idt(); }
Mat::Mat(data values) { set(values); }
void Mat::mul(data mata, data matb) {
  float v00 =
      mata[M00] * matb[M00] + mata[M01] * matb[M10] + mata[M02] * matb[M20];
  float v01 =
      mata[M00] * matb[M01] + mata[M01] * matb[M11] + mata[M02] * matb[M21];
  float v02 =
      mata[M00] * matb[M02] + mata[M01] * matb[M12] + mata[M02] * matb[M22];

  float v10 =
      mata[M10] * matb[M00] + mata[M11] * matb[M10] + mata[M12] * matb[M20];
  float v11 =
      mata[M10] * matb[M01] + mata[M11] * matb[M11] + mata[M12] * matb[M21];
  float v12 =
      mata[M10] * matb[M02] + mata[M11] * matb[M12] + mata[M12] * matb[M22];

  float v20 =
      mata[M20] * matb[M00] + mata[M21] * matb[M10] + mata[M22] * matb[M20];
  float v21 =
      mata[M20] * matb[M01] + mata[M21] * matb[M11] + mata[M22] * matb[M21];
  float v22 =
      mata[M20] * matb[M02] + mata[M21] * matb[M12] + mata[M22] * matb[M22];

  mata[M00] = v00;
  mata[M10] = v10;
  mata[M20] = v20;
  mata[M01] = v01;
  mata[M11] = v11;
  mata[M21] = v21;
  mata[M02] = v02;
  mata[M12] = v12;
  mata[M22] = v22;
}
Mat *Mat::setOrtho(float x, float y, float width, float height) {
  const float right = x + width, top = y + height;

  float x_orth = 2 / (right - x);
  float y_orth = 2 / (top - y);

  float tx = -(right + x) / (right - x);
  float ty = -(top + y) / (top - y);

  val[M00] = x_orth;
  val[M11] = y_orth;

  val[M02] = tx;
  val[M12] = ty;
  val[M22] = 1.0f;
  return this;
}
Mat *Mat::idt() {
  auto val = this->val;
  val[M00] = 1;
  val[M10] = 0;
  val[M20] = 0;
  val[M01] = 0;
  val[M11] = 1;
  val[M21] = 0;
  val[M02] = 0;
  val[M12] = 0;
  val[M22] = 1;
  return this;
}
Mat *Mat::mul(Mat m) {
  auto val = this->val;

  float v00 =
      val[M00] * m.val[M00] + val[M01] * m.val[M10] + val[M02] * m.val[M20];
  float v01 =
      val[M00] * m.val[M01] + val[M01] * m.val[M11] + val[M02] * m.val[M21];
  float v02 =
      val[M00] * m.val[M02] + val[M01] * m.val[M12] + val[M02] * m.val[M22];

  float v10 =
      val[M10] * m.val[M00] + val[M11] * m.val[M10] + val[M12] * m.val[M20];
  float v11 =
      val[M10] * m.val[M01] + val[M11] * m.val[M11] + val[M12] * m.val[M21];
  float v12 =
      val[M10] * m.val[M02] + val[M11] * m.val[M12] + val[M12] * m.val[M22];

  float v20 =
      val[M20] * m.val[M00] + val[M21] * m.val[M10] + val[M22] * m.val[M20];
  float v21 =
      val[M20] * m.val[M01] + val[M21] * m.val[M11] + val[M22] * m.val[M21];
  float v22 =
      val[M20] * m.val[M02] + val[M21] * m.val[M12] + val[M22] * m.val[M22];

  val[M00] = v00;
  val[M10] = v10;
  val[M20] = v20;
  val[M01] = v01;
  val[M11] = v11;
  val[M21] = v21;
  val[M02] = v02;
  val[M12] = v12;
  val[M22] = v22;

  return this;
}
Mat *Mat::mulLeft(Mat m) {

  auto val = this->val;
  float v00 =
      m.val[M00] * val[M00] + m.val[M01] * val[M10] + m.val[M02] * val[M20];
  float v01 =
      m.val[M00] * val[M01] + m.val[M01] * val[M11] + m.val[M02] * val[M21];
  float v02 =
      m.val[M00] * val[M02] + m.val[M01] * val[M12] + m.val[M02] * val[M22];

  float v10 =
      m.val[M10] * val[M00] + m.val[M11] * val[M10] + m.val[M12] * val[M20];
  float v11 =
      m.val[M10] * val[M01] + m.val[M11] * val[M11] + m.val[M12] * val[M21];
  float v12 =
      m.val[M10] * val[M02] + m.val[M11] * val[M12] + m.val[M12] * val[M22];

  float v20 =
      m.val[M20] * val[M00] + m.val[M21] * val[M10] + m.val[M22] * val[M20];
  float v21 =
      m.val[M20] * val[M01] + m.val[M21] * val[M11] + m.val[M22] * val[M21];
  float v22 =
      m.val[M20] * val[M02] + m.val[M21] * val[M12] + m.val[M22] * val[M22];

  val[M00] = v00;
  val[M10] = v10;
  val[M20] = v20;
  val[M01] = v01;
  val[M11] = v11;
  val[M21] = v21;
  val[M02] = v02;
  val[M12] = v12;
  val[M22] = v22;

  return this;
}
Mat *Mat::setToRotation(float degrees) {
  return setToRotationRad(degreesToRadians * degrees);
}
Mat *Mat::setToRotationRad(float radians) {
  float cos = std::cos(radians);
  float sin = std::sin(radians);

  auto val = this->val;
  val[M00] = cos;
  val[M10] = sin;
  val[M20] = 0;

  val[M01] = -sin;
  val[M11] = cos;
  val[M21] = 0;

  val[M02] = 0;
  val[M12] = 0;
  val[M22] = 1;

  return this;
}
Mat *Mat::setToTranslation(float x, float y) {
  auto val = this->val;

  val[M00] = 1;
  val[M10] = 0;
  val[M20] = 0;

  val[M01] = 0;
  val[M11] = 1;
  val[M21] = 0;

  val[M02] = x;
  val[M12] = y;
  val[M22] = 1;

  return this;
}
Mat *Mat::setToScaling(float scaleX, float scaleY) {
  auto val = this->val;
  val[M00] = scaleX;
  val[M10] = 0;
  val[M20] = 0;
  val[M01] = 0;
  val[M11] = scaleY;
  val[M21] = 0;
  val[M02] = 0;
  val[M12] = 0;
  val[M22] = 1;
  return this;
}
std::string Mat::toString() {
  auto val = this->val;
  return fmt::format(
      "[{:.2f}|{:.2f}|{:.2f}]\n[{:.2f}|{:.2f}|{:.2f}]\n[{:.2f}|{:.2f}|{:.2f}]",
      val[M00], val[M01], val[M02], val[M10], val[M11], val[M12], val[M20],
      val[M21], val[M22]);
}
float Mat::det() {
  auto val = this->val;
  return val[M00] * val[M11] * val[M22] + val[M01] * val[M12] * val[M20] +
         val[M02] * val[M10] * val[M21] - val[M00] * val[M12] * val[M21] -
         val[M01] * val[M10] * val[M22] - val[M02] * val[M11] * val[M20];
}

} // namespace nova
