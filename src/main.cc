
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
const int H = 500;
const int W = 500;

int dist(int x0, int y0, int x1, int y1) {
  return (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1);
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

template <int H, int W> struct Canvas {
  void fill(uint32_t color) {
    for (int i = 0; i < H * W; ++i) {
      cv[i] = color;
    }
  }
  void drawRect(int x0, int y0, int w, int h, uint32_t color) {
    for (int y = clampY(y0); y < clampY(h + y0); ++y) {
      for (int x = clampX(x0); x < clampX(w + x0); ++x) {
        cv[y * W + x] = blend(color, cv[y * W + x]);
      }
    }
  }

  void drawCircle(int x0, int y0, int r, uint32_t color, int antiAlias = 1) {
    int rr = r * r;
    int rr2 = (r + 1) * (r + 1);
    int inner = rr / 2;
    int ra = rr * antiAlias * antiAlias;
    int a = ALPHA(color);
    uint32_t truncColor = color & 0x00ffffff;
    for (int y = clampY(y0 - r); y < clampY(y0 + r + 1); ++y) {
      int yd = (y - y0) * (y - y0);
      for (int x = clampX(x0 - r); x < clampX(x0 + r + 1); ++x) {
        int xd = (x - x0) * (x - x0);
        int d = xd + yd;
        if (d <= rr2) {
          int aCum = 0;
          uint32_t colora = 0;
          if (xd > inner || yd > inner) {
            for (int ya = 0; ya < antiAlias; ya++) {
              for (int xa = 0; xa < antiAlias; xa++) {
                if (dist(x0 * antiAlias, y0 * antiAlias, x * antiAlias + xa,
                         y * antiAlias + ya) <= ra) {
                  LOG(ra)
                  LOG(dist(x0 * antiAlias, y0 * antiAlias, x * antiAlias + xa,
                           y * antiAlias + ya))
                  aCum += a;
                }
              }
            }
            aCum /= (antiAlias * antiAlias);
            // LOG(aCum)
            colora = truncColor | (aCum << 24);
          } else {
            colora = color;
          }
          cv[y * W + x] = blend(colora, cv[y * W + x]);
        }
      }
    }
  }

  void drawLine(int x0, int y0, int x1, int y1, uint32_t color) {
    if (x0 > x1)
      return drawLine(x1, y1, x0, y0, color);
    float grad = (float)(y0 - y1) / (float)(x0 - x1);
    float intery = x0 + grad;
    uint32_t truncColor = color & 0x00ffffff;
    for (int x = clampX(x0); x < clampX(x1); ++x) {

      int y = intery;
      if (clampY(y) != y)
        continue;
      int a = LFPART(intery) * ALPHA(color);
      cv[y * W + x] = blend(truncColor | (a << 24), cv[y * W + x]);

      y++;
      if (clampY(y) != y)
        continue;
      a = FPART(intery) * ALPHA(color);
      cv[y * W + x] = blend(truncColor | (a << 24), cv[y * W + x]);
      intery += grad;
    }
  }

  void drawTriangleFlat(int x0, int y0, int w, int xt, int yt);
  void drawTriangle(int x0, int y0, int x1, int y1, int x2, int x3);
  int clampX(int x) { return x < W ? (x > 0 ? x : 0) : W; }
  int clampY(int y) { return y < H ? (y > 0 ? y : 0) : H; }
  uint32_t cv[H * W];
};

Canvas<H, W> cv;

extern "C" {
void test() {
  cv.fill(0xff00ff00);
  cv.drawRect(50, 50, 100, 100, 0xa0ff0000);
  cv.drawCircle(W / 2, H / 2, H / 3, 0xe00000ff, 3);
  cv.drawLine(0, 0, 200, 100, 0xff00ffff);
}
int WIDTH() { return W; }
int HEIGHT() { return H; }
}

int main() { test(); }
