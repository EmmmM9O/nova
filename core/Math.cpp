#include "Math.hpp"
#include "math.h"
namespace nova {
bool Position::within(float x, float y, float dst) {}
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
float Position::angleTo(float x, float y) {}
float Position::angleTo(Position *other) {
  return angleTo(other->getX(), other->getY());
}

} // namespace nova
