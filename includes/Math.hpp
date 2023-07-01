#pragma once
#include <stdint.h>

extern "C" {
float sinp(float p);
float cosp(float p);
float tanp(float p);
float sqrtp(float p);
float asinp(float p);
float acosp(float p);
}

#ifndef WASM_DEMO
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdlib.h>

#define LOG(arg) std::cout << arg << '\n';

#else

#define LOG(arg)
#define IMPORTED(action) action
#endif

#define ABS(x) (x > 0 ? x : (-x))

namespace gcmath {

const extern float pi;
float pow(float x, int i);
unsigned pow(int x, int i);

uint8_t RED(uint32_t color);
uint8_t GREEN(uint32_t color);
uint8_t BLUE(uint32_t color);
uint8_t ALPHA(uint32_t color);
float FPART(float x);
float LFPART(float x);

int dist(int x0, int y0, int x1, int y1);

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

uint32_t HSV2RGB(int H, int S, int V);

inline unsigned long pow2(int x) { return x * x; }

void rotateInt(int &x, int &y, float angle);
void rotateFloat(float &x, float &y, float angle);

float pointLineDist(int xp, int yp, int x1, int y1, int x2, int y2);

float project(int x, int z, int cx);
int projectScreen(float x, int w, float angle);
template <typename T> T inline max(T a, T b) { return a > b ? a : b; }
template <typename T> T inline min(T a, T b) { return a < b ? a : b; }
template <typename T> void swap(T &a, T &b) {
  T tmp = a;
  a = b;
  b = tmp;
}
} // namespace gcmath