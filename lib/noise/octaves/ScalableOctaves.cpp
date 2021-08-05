#include "ScalableOctaves.h"

void ScalableOctaves::SetScale(double scale) {
  SetXScale(scale);
  SetYScale(scale);
  SetZScale(scale);
}

double ScalableOctaves::GetXScale() const {
  return xScale;
}

double ScalableOctaves::GetYScale() const {
  return yScale;
}

double ScalableOctaves::GetZScale() const {
  return zScale;
}

void ScalableOctaves::SetXScale(double scaleX) {
  this->xScale = scaleX;
}

void ScalableOctaves::SetYScale(double scaleY) {
  this->yScale = scaleY;
}

void ScalableOctaves::SetZScale(double scaleZ) {
  this->zScale = scaleZ;
}