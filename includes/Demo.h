#pragma once
#include "Canvas.hpp"
#define COMMON_EXPORTS(w, h)                                                   \
  extern "C" {                                                                 \
  int WIDTH() { return w; }                                                    \
  int HEIGHT() { return h; }                                                   \
  void *CANVAS() { return &cv; }                                               \
  }
extern "C" {
void render(int dt);
int WIDTH();
int HEIGHT();
}
