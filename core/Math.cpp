#include "Math.hpp"

#include "math.h"
namespace nova {
	float PI=M_PI;
	float radiansToDegrees=(180.0f) / PI;
	float radDeg=radiansToDegrees;
float Angles::angle(float x, float y, float x2, float y2) {
  float ang = atan2(x2 - x, y2 - y) * radDeg;
  if (ang < 0) ang += 360.0f;
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

}  // namespace nova
