#pragma once
#include "Canvas.hpp"
#include "Math.hpp"
#include <stdint.h>
#define CONST_PICKER(color) [](int x, int y) -> uint32_t { return color; }

#ifdef DEBUG
#include <stdlib.h>
extern "C" unsigned long long alloc(unsigned long long size) {
  return (unsigned long long)malloc(size);
}
#else
extern "C" unsigned long long alloc(unsigned long long size);
#endif

template <int H, int W> struct Canvas {
  struct Ret {
    float x;
    float y;
  };
  const int h = H;
  const int w = W;
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
    int rr_out = (r + 1) * (r + 1);
    int rr_in = (r - 1) * (r - 1);
    int ra = rr * antiAlias * antiAlias;

    for (int y = clampY(y0 - r); y < clampY(y0 + r + 1); ++y) {
      int yd = (y - y0) * (y - y0);
      for (int x = clampX(x0 - r); x < clampX(x0 + r + 1); ++x) {
        int xd = (x - x0) * (x - x0);
        int d = xd + yd;
        int a = gcmath::ALPHA(color(x, y));
        uint32_t truncColor = color(x, y) & 0x00ffffff;
        if (d <= rr_out) {
          int aCum = 0;
          uint32_t colora = 0;
          // this control where to enable anti-alias
          if (d >= rr_in) {
            for (int ya = 0; ya < antiAlias; ya++) {
              for (int xa = 0; xa < antiAlias; xa++) {
                if (gcmath::dist(x0 * antiAlias, y0 * antiAlias,
                                 x * antiAlias + xa,
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
  void drawEllipse(int x0, int y0, int a, int b, Functor color, float rot,
                   int aa = 1) {
    float cosR = cosp(gcmath::DEG2RAD(rot));
    float sinR = sinp(gcmath::DEG2RAD(rot));
    auto rotate = [sinR, cosR, x0, y0](int x, int y) -> Ret {
      return {cosR * (x - x0) - sinR * (y - y0) + x0,
              sinR * (x - x0) + cosR * (y - y0) + y0};
    };
    auto [xtl, ytl] = rotate(x0 - a, y0 - b);
    auto [xtr, ytr] = rotate(x0 + a, y0 - b);
    auto [xbl, ybl] = rotate(x0 - a, y0 + b);
    auto [xbr, ybr] = rotate(x0 + a, y0 + b);
    float xs[4] = {xtl, xtr, xbl, xbr};
    float ys[4] = {ytl, ytr, ybl, ybr};
    float xstart = W, ystart = H, xend = 0, yend = 0;
    for (int i = 0; i < 4; ++i) {
      if (xs[i] < xstart)
        xstart = xs[i];
      if (xs[i] > xend)
        xend = xs[i];
      if (ys[i] < ystart)
        ystart = ys[i];
      if (ys[i] > yend)
        yend = ys[i];
    }

    unsigned rr_out = (a + 1) * (a + 1) * (b + 1) * (b + 1);
    unsigned rr_in = (a - 2) * (a - 2) * (b - 2) * (b - 2);
    unsigned ra = a * a * b * b * aa * aa;

    for (int y = clampY(ystart); y < clampY(yend + 1); ++y) {

      for (int x = clampX(xstart); x < clampX(xend + 1); ++x) {
        unsigned d = gcmath::pow2(((x - x0) * cosR - (y - y0) * sinR) * b) +
                     gcmath::pow2(((x - x0) * sinR + (y - y0) * cosR) * a);
        uint32_t colorRevised = color(x, y);
        uint8_t alpha = gcmath::ALPHA(colorRevised);
        uint32_t truncColor = color(x, y) & 0x00ffffff;
        if (d <= rr_out) {
          unsigned aCum = 0;
          // this control where to enable anti-alias
          if (d >= rr_in) {
            for (int ya = 0; ya < aa; ya++) {
              for (int xa = 0; xa < aa; xa++) {
                unsigned da = gcmath::pow2(((x * aa + xa - x0 * aa) * cosR -
                                            (y * aa + ya - y0 * aa) * sinR) *
                                           b) +
                              gcmath::pow2(((x * aa + xa - x0 * aa) * sinR +
                                            (y * aa + ya - y0 * aa) * cosR) *
                                           a);
                if (da <= ra) {
                  aCum += alpha;
                }
              }
            }
            aCum /= (aa * aa);
            // LOG(aCum)
            colorRevised = truncColor | (aCum << 24);
          }
          cv[y * W + x] = blend(colorRevised, cv[y * W + x]);
        }
      }
    }
  }
  template <typename Functor>
  void drawEllipse(int x0, int y0, int a, int b, Functor color,
                   int antiAlias = 1) {
    // https://math.stackexchange.com/questions/426150/what-is-the-general-equation-of-the-ellipse-that-is-not-in-the-origin-and-rotate
    unsigned rr_out = (a + 1) * (a + 1) * (b + 1) * (b + 1);
    unsigned rr_in = (a - 2) * (a - 2) * (b - 2) * (b - 2);
    unsigned ra = a * a * b * b * antiAlias * antiAlias;

    for (int y = clampY(y0 - b); y < clampY(y0 + b + 1); ++y) {
      unsigned yd = (y - y0) * (y - y0) * a * a;
      for (int x = clampX(x0 - a); x < clampX(x0 + a + 1); ++x) {
        unsigned xd = (x - x0) * (x - x0) * b * b;
        unsigned d = xd + yd;
        uint8_t alpha = gcmath::ALPHA(color(x, y));
        uint32_t truncColor = color(x, y) & 0x00ffffff;
        if (d <= rr_out) {
          unsigned aCum = 0;
          uint32_t colora = 0;
          // this control where to enable anti-alias
          if (d >= rr_in) {
            for (int ya = 0; ya < antiAlias; ya++) {
              for (int xa = 0; xa < antiAlias; xa++) {
                if (gcmath::pow2((x * antiAlias + xa - x0 * antiAlias) * b) +
                        gcmath::pow2((y * antiAlias + ya - y0 * antiAlias) *
                                     a) <=
                    ra) {
                  aCum += alpha;
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
        int a = gcmath::LFPART(intery) * gcmath::ALPHA(color(x, y));
        cv[y * W + x] = blend(truncColor | (a << 24), cv[y * W + x]);

        y++;
        if (y >= H)
          continue;

        a = gcmath::FPART(intery) * gcmath::ALPHA(color(x, y));

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
        int a = gcmath::LFPART(intery) * gcmath::ALPHA(color(x, y));
        cv[y * W + x] = blend(truncColor | (a << 24), cv[y * W + x]);

        x++;
        if (clampX(x) != x)
          continue;
        a = gcmath::FPART(intery) * gcmath::ALPHA(color(x, y));
        cv[y * W + x] = blend(truncColor | (a << 24), cv[y * W + x]);
        intery += grad;
      }
    }
  }
  template <typename Functor>
  void drawTriangleFlat(int x0, int y0, int w, int xt, int yt, Functor color,
                        bool antiAlias = false) {
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
          gcmath::swap(x1, x2);
        if (antiAlias) {
          x1++;
        }
        x2++;

        for (int x = clampX(x1); x < clampX(x2); ++x) {
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
          gcmath::swap(x1, x2);
        if (antiAlias) {
          x1++;
        }
        x2++;
        for (int x = clampX(x1); x < clampX(x2); ++x) {
          cv[y * W + x] = blend(color(x, y), cv[y * W + x]);
        }
        //   cv[y * W + clampX(x1)] = blend(color, cv[y * W + clampX(x1)]);
        //   cv[y * W + clampX(x2)] = blend(color, cv[y * W + clampX(x2)]);
        intery1 -= grad1;
        intery2 -= grad2;
      }
    }
    if (antiAlias) {
      drawLine(x0, y0, xt, yt, color);
      drawLine(x0 + w, y0, xt, yt, color);
    }
  }
  template <typename Functor>
  void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2,
                    Functor color) {
    if (y0 >= y1 && y1 >= y2) {
      if (y0 == y2)
        return drawLine(x0, y0, x2, y2, color);
      float grad = (float)(x0 - x2) / (float)(y0 - y2);
      int xm = grad * (y1 - y2) + x2;
      int w = xm - x1;

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
  void inline drawPixel(int x, int y, uint32_t color) {
    cv[clampY(y) * W + clampX(x)] = color;
  }
  int clampX(int x) { return x < W ? (x > 0 ? x : 0) : W; }
  int clampY(int y) { return y < H ? (y > 0 ? y : 0) : H; }
  uint32_t cv[H * W];
  struct Ellipse {
    float x, y;
    float major, minor;
    float rot;
  };
  bool blending = true;

private:
  //   static int blend(uint32_t over, uint32_t back) {
  //     int a255 = 255 * gcmath::ALPHA(over) +
  //                (255 - gcmath::ALPHA(over)) * (gcmath::ALPHA(back));
  //     int r = (255 * gcmath::ALPHA(over) * gcmath::RED(over) +
  //              (255 - gcmath::ALPHA(over)) * (gcmath::ALPHA(back)) *
  //                  gcmath::RED(back)) /
  //             a255;
  //     int g = (255 * gcmath::ALPHA(over) * gcmath::GREEN(over) +
  //              (255 - gcmath::ALPHA(over)) * (gcmath::ALPHA(back)) *
  //                  gcmath::GREEN(back)) /
  //             a255;
  //     int b = (255 * gcmath::ALPHA(over) * gcmath::BLUE(over) +
  //              (255 - gcmath::ALPHA(over)) * (gcmath::ALPHA(back)) *
  //                  gcmath::BLUE(back)) /
  //             a255;
  //     a255 /= 255;
  //     return (a255 << 24) | (b << 16) | (g << 8) | (r << 0);
  //   }

  uint32_t blend(uint32_t over, uint32_t back) {
    if (!blending) {
      if (gcmath::ALPHA(over) == 0)
        return back;
      else
        return over;
    }
    uint8_t a =
        gcmath::min(gcmath::ALPHA(over) +
                        gcmath::ALPHA(back) * (255 - gcmath::ALPHA(over)) / 255,
                    255);
    uint8_t r =
        gcmath::min(gcmath::RED(over) +
                        gcmath::RED(back) * (255 - gcmath::ALPHA(over)) / 255,
                    255);
    uint8_t g =
        gcmath::min(gcmath::GREEN(over) +
                        gcmath::GREEN(back) * (255 - gcmath::ALPHA(over)) / 255,
                    255);
    uint8_t b =
        gcmath::min(gcmath::BLUE(over) +
                        gcmath::BLUE(back) * (255 - gcmath::ALPHA(over)) / 255,
                    255);
    return (a << 24) | (b << 16) | (g << 8) | (r << 0);
  }
};