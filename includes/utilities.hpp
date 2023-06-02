#pragma once
#include <stdint.h>

#ifdef DEBUG
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