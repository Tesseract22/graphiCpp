#pragma once
#include <stdint.h>

#ifdef DEBUG
#include <iomanip>
#include <iostream>
#define LOG(arg) std::cout << arg << '\n';
#else
#define LOG(arg)
#endif

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

int blend(uint32_t over, uint32_t back) {
  int a255 = 255 * ALPHA(over) + (255 - ALPHA(over)) * (ALPHA(back));
  int r = (255 * ALPHA(over) * RED(over) +
           (255 - ALPHA(over)) * (ALPHA(back)) * RED(back)) /
          a255;
  int g = (255 * ALPHA(over) * GREEN(over) +
           (255 - ALPHA(over)) * (ALPHA(back)) * GREEN(back)) /
          a255;
  int b = (255 * ALPHA(over) * BLUE(over) +
           (255 - ALPHA(over)) * (ALPHA(back)) * BLUE(back)) /
          a255;
  a255 /= 255;
  return (a255 << 24) | (b << 16) | (g << 8) | (r << 0);
}
/**
 * @brief
 *
 * @param H 0-360
 * @param S 0-100
 * @param V 0-100
 * @return uint32_t
 */
uint32_t HSV2RGB(int H, int S, int V) {
  int C = V * S;
  float md = ((H / 60) % 2 + (H % 60) / 60.) - 1;
  if (md < 0)
    md = -md;
  int X = C * (1 - md);
  int m = V * 100 - C;
  int r_, g_, b_;
  LOG(C << ' ' << X)
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
  LOG(r_ << ' ' << g_ << ' ' << b_ << ' ' << m)
  auto cal = [m](int c) -> int { return (int)((c + m) * 255 / 10000); };
  return 0xff000000 | (cal(r_) << 0) | ((cal(g_)) << 8) | ((cal(b_)) << 16);
}