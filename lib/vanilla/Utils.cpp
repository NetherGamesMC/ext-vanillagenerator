#include "Utils.h"

int Utils::floor(double x) {
  int floored = (int) x;
  return x < floored ? floored - 1 : floored;
}

double Utils::lerp(double delta, double start, double end) {
  return start + delta * (end - start);
}

double Utils::grad(int hash, double x, double y, double z) {
  hash &= 15;
  double u = hash < 8 ? x : y;
  double v = hash < 4 ? y : hash == 12 || hash == 14 ? x : z;
  return ((hash & 1) == 0 ? u : -u) + ((hash & 2) == 0 ? v : -v);
}

double Utils::fade(double x) {
  return x * x * x * (x * (x * 6 - 15) + 10);
}

int Utils::floorSimplex(double x) {
  return x > 0 ? (int) x : (int) x - 1;
}

double Utils::dot(const int8_t g[3], double x, double y) {
  return g[0] * x + g[1] * y;
}

double Utils::dot(const int8_t g[3], double x, double y, double z) {
  return g[0] * x + g[1] * y + g[2] * z;
}

long Utils::floorLong(double x) {
  return x >= 0 ? (long) x : (long) x - 1;
}