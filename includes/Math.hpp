#pragma once
#include <stdint.h>

#ifdef DEBUG
#include <cmath>
#include <iomanip>
#include <iostream>

#define LOG(arg) std::cout << arg << '\n';
#define IMPORTED(action)

extern "C" {
float sinp(float p) { return std::sin(p); }
float cosp(float p) { return std::cos(p); }
float tanp(float p) { return std::tan(p); }
float sqrtp(float p) { return std::sqrt(p); }
float asinp(float p) { return std::asin(p); }
float acosp(float p) { return std::acos(p); }
}

#else

#define LOG(arg)
#define IMPORTED(action) action
extern "C" {
float sinp(float p);
float cosp(float p);
float tanp(float p);
float sqrtp(float p);
float asinp(float p);
float acosp(float p);
}
#endif
#define ABS(x) (x > 0 ? x : (-x))
#define COMMON_EXPORTS(w, h)                                                   \
  extern "C" {                                                                 \
  int WIDTH() { return w; }                                                    \
  int HEIGHT() { return h; }                                                   \
  }

namespace gcmath {

const extern float pi = 3.141592653589793;
float pow(float x, int i) {
  float res = 1;
  for (int j = 0; j < i; ++j)
    res *= x;
  return res;
}
unsigned pow(int x, int i) {
  int res = 1;
  for (int j = 0; j < i; ++j)
    res *= x;
  return res;
}

uint8_t RED(uint32_t color) { return color & 0x000000ff; }
uint8_t GREEN(uint32_t color) { return (color >> 8) & 0x000000ff; }
uint8_t BLUE(uint32_t color) { return (color >> 16) & 0x000000ff; }
uint8_t ALPHA(uint32_t color) { return (color >> 24) & 0x000000ff; }
float FPART(float x) { return x - (int)x; }
float LFPART(float x) { return 1 - FPART(x); }

int dist(int x0, int y0, int x1, int y1) {
  return (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1);
}

template <typename T> void swap(T &a, T &b) {
  T tmp = a;
  a = b;
  b = tmp;
}

/**
 * @brief
 *
 * @param H 0-360
 * @param S 0-100
 * @param V 0-100
 * @return uint32_t
 */

float inline DEG2RAD(float deg) { return deg / 180 * pi; }
float inline RAD2DEG(float rad) { return rad * 180 / pi; }

uint32_t HSV2RGB(int H, int S, int V) {
  int C = V * S;
  float md = ((H / 60) % 2 + (H % 60) / 60.) - 1;
  if (md < 0)
    md = -md;
  int X = C * (1 - md);
  int m = V * 100 - C;
  int r_, g_, b_;
  if (H >= 0 && H < 60) {
    r_ = C;
    g_ = X;
    b_ = 0;
  } else if (H < 120) {
    r_ = X;
    g_ = C;
    b_ = 0;
  } else if (H < 180) {
    r_ = 0;
    g_ = C;
    b_ = X;
  } else if (H < 240) {
    r_ = 0;
    g_ = X;
    b_ = C;
  } else if (H < 300) {
    r_ = X;
    g_ = 0;
    b_ = C;
  } else if (H < 360) {
    r_ = C;
    g_ = 0;
    b_ = X;
  }
  auto cal = [m](int c) -> int { return (int)((c + m) * 255 / 10000); };
  return (cal(r_) << 0) | ((cal(g_)) << 8) | ((cal(b_)) << 16);
}

inline unsigned long pow2(int x) { return x * x; }

void rotate(int &x, int &y, float angle) {
  int tmpX = cosp(DEG2RAD(angle)) * x - sinp(DEG2RAD(angle)) * y;
  y = sinp(DEG2RAD(angle)) * x + cosp(DEG2RAD(angle)) * y;
  x = tmpX;
}
void rotate(float &x, float &y, float angle) {
  int tmpX = cosp(DEG2RAD(angle)) * x - sinp(DEG2RAD(angle)) * y;
  y = sinp(DEG2RAD(angle)) * x + cosp(DEG2RAD(angle)) * y;
  x = tmpX;
}

float pointLineDist(int xp, int yp, int x1, int y1, int x2, int y2) {
  // (x - x1) / (x2 - x1) = (y - y1) / (y2 - y1)
  // k = (y2 - y1) / (x2 - x1)
  // k(x - x1) = y - y1
  // k*x - y - k*x1 + y1 = 0
  // A = k
  // B = -1
  // C = -k*x1 + y1
  //   LOG(xp << ' ' << yp)
  //   LOG(yp - y2)
  if (x2 == x1)
    return ABS((xp - x2));
  if (y2 == y1)
    return ABS((yp - y2));
  float grad = (float)(y2 - y1) / (x2 - x1);
  float a = (grad * xp - yp + (-grad * x1 + y1));
  if (a < 0)
    a = -a;
  return a / sqrtp(grad * grad + 1);
}

float project(int x, int z, int cx) { return (float)(x - cx) / z; }
int projectScreen(float x, int w, float angle) {
  float ratio = w / (tanp(DEG2RAD(angle) / 2.) * 2);
  // 120
  // 2 sqrt(3)
  // 250 / sqrt(3)
  return x * ratio + w / 2;
}

template <typename T> T inline max(T a, T b) { return a > b ? a : b; }
template <typename T> T inline min(T a, T b) { return a < b ? a : b; }
} // namespace gcmath