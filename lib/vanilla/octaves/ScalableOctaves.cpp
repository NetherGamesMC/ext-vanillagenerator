#include "ScalableOctaves.h"

void ScalableOctaves::setScale(double scale) {
  setXScale(scale);
  setYScale(scale);
  setZScale(scale);
}

double ScalableOctaves::getXScale() const {
  return xScale;
}

double ScalableOctaves::getYScale() const {
  return yScale;
}

double ScalableOctaves::getZScale() const {
  return zScale;
}

void ScalableOctaves::setXScale(double scaleX) {
  this->xScale = scaleX;
}

void ScalableOctaves::setYScale(double scaleY) {
  this->yScale = scaleY;
}

void ScalableOctaves::setZScale(double scaleZ) {
  this->zScale = scaleZ;
}