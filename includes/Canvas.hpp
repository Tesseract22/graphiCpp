#pragma once
#include "Canvas.hpp"
#include "utilities.hpp"
#include <stdint.h>

#define CONST_PICKER(color) [](int x, int y) -> uint32_t { return color; }

template <int H, int W> struct Canvas {
  void fill(uint32_t color) {
    for (int i = 0; i < H * W; ++i) {
      cv[i] = color;
    }
  }
  template <typename Functor>
  void drawRect(int x0, int y0, int w, int h, Functor color) {
    if (w < 0) {
      x0 += w;
      w = -w;
    }
    if (h < 0) {
      y0 += h;
      h = -h;
    }
    for (int y = clampY(y0); y < clampY(h + y0); ++y) {
      for (int x = clampX(x0); x < clampX(w + x0); ++x) {
        cv[y * W + x] = blend(color(x, y), cv[y * W + x]);
      }
    }
  }
  template <typename Functor>
  void drawCircle(int x0, int y0, int r, Functor color, int antiAlias = 1) {
    int rr = r * r;
    int rr2 = (r + 1) * (r + 1);
    int inner = rr / 2;
    int ra = rr * antiAlias * antiAlias;

    for (int y = clampY(y0 - r); y < clampY(y0 + r + 1); ++y) {
      int yd = (y - y0) * (y - y0);
      for (int x = clampX(x0 - r); x < clampX(x0 + r + 1); ++x) {
        int xd = (x - x0) * (x - x0);
        int d = xd + yd;
        int a = ALPHA(color(x, y));
        uint32_t truncColor = color(x, y) & 0x00ffffff;
        if (d <= rr2) {
          int aCum = 0;
          uint32_t colora = 0;
          if (xd > inner || yd > inner) {
            for (int ya = 0; ya < antiAlias; ya++) {
              for (int xa = 0; xa < antiAlias; xa++) {
                if (dist(x0 * antiAlias, y0 * antiAlias, x * antiAlias + xa,
                         y * antiAlias + ya) <= ra) {
                  aCum += a;
                }
              }
            }
            aCum /= (antiAlias * antiAlias);
            // LOG(aCum)
            colora = truncColor | (aCum << 24);
          } else {
            colora = color(x, y);
          }
          cv[y * W + x] = blend(colora, cv[y * W + x]);
        }
      }
    }
  }
  template <typename Functor>
  void drawLine(int x0, int y0, int x1, int y1, Functor color) {
    float grad = (float)(y0 - y1) / (float)(x0 - x1);

    float intery;
    if (grad <= 1 && grad >= -1) {

      if (x0 > x1)
        return drawLine(x1, y1, x0, y0, color);
      intery = y0;
      int x0c = clampX(x0);
      intery += grad * (x0c - x0);
      for (int x = x0c; x < clampX(x1); ++x) {
        int y = intery;
        uint32_t truncColor = color(x, y) & 0x00ffffff;
        if (y < 0 || y >= H)
          continue;
        int a = LFPART(intery) * ALPHA(color(x, y));
        cv[y * W + x] = blend(truncColor | (a << 24), cv[y * W + x]);

        y++;
        if (y >= H)
          continue;

        a = FPART(intery) * ALPHA(color(x, y));

        cv[y * W + x] = blend(truncColor | (a << 24), cv[y * W + x]);

        intery += grad;
      }
    } else {
      if (y0 > y1)
        return drawLine(x1, y1, x0, y0, color);
      grad = (float)(x0 - x1) / (float)(y0 - y1);
      intery = x0;
      int y0c = clampY(y0);
      intery += grad * (y0c - y0);
      for (int y = y0c; y < clampY(y1); ++y) {
        int x = intery;
        uint32_t truncColor = color(x, y) & 0x00ffffff;
        if (clampX(x) != x)
          continue;
        int a = LFPART(intery) * ALPHA(color(x, y));
        cv[y * W + x] = blend(truncColor | (a << 24), cv[y * W + x]);

        x++;
        if (clampX(x) != x)
          continue;
        a = FPART(intery) * ALPHA(color(x, y));
        cv[y * W + x] = blend(truncColor | (a << 24), cv[y * W + x]);
        intery += grad;
      }
    }
  }
  template <typename Functor>
  void drawTriangleFlat(int x0, int y0, int w, int xt, int yt, Functor color) {
    float grad1 = (float)(x0 - xt) / (float)(y0 - yt);
    float grad2 = (float)(x0 + w - xt) / (float)(y0 - yt);

    float intery1 = x0;
    float intery2 = x0 + w;
    if (yt > y0) {
      int y0c = clampY(y0);
      intery1 += (y0c - y0) * grad1;
      intery2 += (y0c - y0) * grad2;
      for (int y = y0c; y < clampY(yt); y++) {

        int x1 = intery1;
        int x2 = intery2;
        if (x2 < x1)
          swap(x1, x2);
        for (int x = clampX(x1 + 1); x < clampX(x2 + 1); ++x) {
          cv[y * W + x] = blend(color(x, y), cv[y * W + x]);
        }
        //   cv[y * W + clampX(x1)] = blend(color, cv[y * W + clampX(x1)]);
        //   cv[y * W + clampX(x2)] = blend(color, cv[y * W + clampX(x2)]);
        intery1 += grad1;
        intery2 += grad2;
      }
    } else {
      int y0c = clampY(y0);
      intery1 += (y0c - y0) * grad1;
      intery2 += (y0c - y0) * grad2;
      for (int y = y0c; y > clampY(yt); y--) {

        int x1 = intery1;
        int x2 = intery2;
        if (x2 < x1)
          swap(x1, x2);
        for (int x = clampX(x1 + 1); x < clampX(x2 + 1); ++x) {
          cv[y * W + x] = blend(color(x, y), cv[y * W + x]);
        }
        //   cv[y * W + clampX(x1)] = blend(color, cv[y * W + clampX(x1)]);
        //   cv[y * W + clampX(x2)] = blend(color, cv[y * W + clampX(x2)]);
        intery1 -= grad1;
        intery2 -= grad2;
      }
    }

    drawLine(x0, y0, xt, yt, color);
    drawLine(x0 + w, y0, xt, yt, color);
  }
  template <typename Functor>
  void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2,
                    Functor color) {
    if (y0 >= y1 && y1 >= y2) {
      float grad = (float)(x0 - x2) / (float)(y0 - y2);
      int xm = grad * (y1 - y2) + x2;
      int w = xm - x1;
      LOG(y0 << ' ' << y1 << ' ' << y2)
      drawTriangleFlat(x1, y1, w, x0, y0,
                       [y1, color](int x, int y) -> uint32_t {
                         if (y == y1)
                           return 0x00000000;
                         else
                           return color(x, y);
                       });
      //   LOG(x1 << ' ' << y1 << ' ' << x2 << ' ' << y2)
      drawTriangleFlat(x1, y1, w, x2, y2, color);

    } else {
      if (y0 <= y1 && y1 <= y2) {
        return drawTriangle(x2, y2, x1, y1, x0, y0, color);

      } else {
        return drawTriangle(x1, y1, x2, y2, x0, y0, color);
      }
    }
  }
  int clampX(int x) { return x < W ? (x > 0 ? x : 0) : W; }
  int clampY(int y) { return y < H ? (y > 0 ? y : 0) : H; }
  uint32_t cv[H * W];
};