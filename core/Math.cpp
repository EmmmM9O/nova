#include "Math.hpp"

#include <stdexcept>

#include "fmt/format.h"
#include "math.h"
namespace nova {
float PI = M_PI;
float radiansToDegrees = (180.0f) / PI;
float radDeg = radiansToDegrees;
float degreesToRadians = (PI / 180.0f);
float Angles::angle(float x, float y, float x2, float y2) {
  float ang = atan2(x2 - x, y2 - y) * radDeg;
  if (ang < 0) ang += 360.0f;
  return ang;
}
bool Position::within(float x, float y, float dst) const {
  return dst2(x, y) <= dst * dst;
}
bool Position::within(Position *other, float dst) const {
  return within(other->getX(), other->getY(), dst);
}
float Position::dst2(float x, float y) const {
  const float dx = getX() - x;
  const float dy = getY() - y;
  return dx * dx + dy * dy;
}
float Position::dst(float x, float y) const {
  const float dx = getX() - x;
  const float dy = getY() - y;
  return sqrt(dx * dx + dy * dy);
}
float Position::dst(Position *other) const {
  return dst(other->getX(), other->getY());
}
float Position::dst2(Position *other) const {
  return dst2(other->getX(), other->getY());
}
float Position::angleTo(float x, float y) const {
  return Angles::angle(getX(), getY(), x, y);
}
float Position::angleTo(Position *other) const {
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
  auto &val = this->val;
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
  auto &val = this->val;
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
Mat *Mat::mul(const Mat &m) {
  auto &val = this->val;

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
Mat *Mat::mulLeft(const Mat &m) {
  auto &val = this->val;
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

  auto &val = this->val;
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
  auto &val = this->val;

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
  auto &val = this->val;
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
std::string Mat::toString() const {
  auto &val = this->val;
  return fmt::format(
      "[{:.2f}|{:.2f}|{:.2f}]\n[{:.2f}|{:.2f}|{:.2f}]\n[{:.2f}|{:.2f}|{:.2f}]",
      val[M00], val[M01], val[M02], val[M10], val[M11], val[M12], val[M20],
      val[M21], val[M22]);
}
float Mat::det() const {
  auto &val = this->val;
  return val[M00] * val[M11] * val[M22] + val[M01] * val[M12] * val[M20] +
         val[M02] * val[M10] * val[M21] - val[M00] * val[M12] * val[M21] -
         val[M01] * val[M10] * val[M22] - val[M02] * val[M11] * val[M20];
}
Mat *Mat::inv() {
  float det = this->det();
  if (det == 0) throw new std::runtime_error("Can't invert a singular matrix");

  float inv_det = 1.0f / det;
  auto &tmp = this->tmp, &val = this->val;

  tmp[M00] = val[M11] * val[M22] - val[M21] * val[M12];
  tmp[M10] = val[M20] * val[M12] - val[M10] * val[M22];
  tmp[M20] = val[M10] * val[M21] - val[M20] * val[M11];
  tmp[M01] = val[M21] * val[M02] - val[M01] * val[M22];
  tmp[M11] = val[M00] * val[M22] - val[M20] * val[M02];
  tmp[M21] = val[M20] * val[M01] - val[M00] * val[M21];
  tmp[M02] = val[M01] * val[M12] - val[M11] * val[M02];
  tmp[M12] = val[M10] * val[M02] - val[M00] * val[M12];
  tmp[M22] = val[M00] * val[M11] - val[M10] * val[M01];

  val[M00] = inv_det * tmp[M00];
  val[M10] = inv_det * tmp[M10];
  val[M20] = inv_det * tmp[M20];
  val[M01] = inv_det * tmp[M01];
  val[M11] = inv_det * tmp[M11];
  val[M21] = inv_det * tmp[M21];
  val[M02] = inv_det * tmp[M02];
  val[M12] = inv_det * tmp[M12];
  val[M22] = inv_det * tmp[M22];

  return this;
}
Mat *Mat::set(const data &values) {
  this->val = values;
  return this;
}
Mat *Mat::trn(float x, float y) {
  val[M02] += x;
  val[M12] += y;
  return this;
}
Mat *Mat::translate(float x, float y) {
  auto &val = this->val;
  tmp[M00] = 1;
  tmp[M10] = 0;
  tmp[M20] = 0;

  tmp[M01] = 0;
  tmp[M11] = 1;
  tmp[M21] = 0;

  tmp[M02] = x;
  tmp[M12] = y;
  tmp[M22] = 1;
  mul(val, tmp);
  return this;
}
Mat *Mat::rotate(float degrees) {
  return rotateRad(degreesToRadians * degrees);
}
Mat *Mat::rotateRad(float radians) {
  if (radians == 0) return this;
  float cos = (float)std::cos(radians);
  float sin = (float)std::sin(radians);
  auto &tmp = this->tmp;

  tmp[M00] = cos;
  tmp[M10] = sin;
  tmp[M20] = 0;

  tmp[M01] = -sin;
  tmp[M11] = cos;
  tmp[M21] = 0;

  tmp[M02] = 0;
  tmp[M12] = 0;
  tmp[M22] = 1;
  mul(val, tmp);
  return this;
}
Mat *Mat::scale(float scaleX, float scaleY) {
  auto &tmp = this->tmp;
  tmp[M00] = scaleX;
  tmp[M10] = 0;
  tmp[M20] = 0;
  tmp[M01] = 0;
  tmp[M11] = scaleY;
  tmp[M21] = 0;
  tmp[M02] = 0;
  tmp[M12] = 0;
  tmp[M22] = 1;
  mul(val, tmp);
  return this;
}
Mat::data &Mat::getValues() { return val; }

float Mat::getRotation() const {
  return radiansToDegrees * std::atan2(val[M10], val[M00]);
}
float Mat::getRotationRad() const { return std::atan2(val[M10], val[M00]); }
Mat *Mat::scl(float scale) {
  val[M00] *= scale;
  val[M11] *= scale;
  return this;
}
Mat *Mat::transpose() {
  auto &val = this->val;
  float v01 = val[M10];
  float v02 = val[M20];
  float v10 = val[M01];
  float v12 = val[M21];
  float v20 = val[M02];
  float v21 = val[M12];
  val[M01] = v01;
  val[M02] = v02;
  val[M10] = v10;
  val[M12] = v12;
  val[M20] = v20;
  val[M21] = v21;
  return this;
}
}  // namespace nova
