
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

    float grad = (float)(y0 - y1) / (float)(x0 - x1);
    uint32_t truncColor = color & 0x00ffffff;
    float intery;
    if (grad <= 1 && grad >= -1) {
      if (x0 > x1)
        return drawLine(x1, y1, x0, y0, color);
      intery = y0;

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
    } else {
      if (y0 > y1)
        return drawLine(x1, y1, x0, y0, color);
      grad = (float)(x0 - x1) / (float)(y0 - y1);
      intery = x0;
      for (int y = clampY(y0); y < clampY(y1); ++y) {
        int x = intery;
        if (clampX(x) != x)
          continue;
        int a = LFPART(intery) * ALPHA(color);
        cv[y * W + x] = blend(truncColor | (a << 24), cv[y * W + x]);

        x++;
        if (clampX(x) != x)
          continue;
        a = FPART(intery) * ALPHA(color);
        cv[y * W + x] = blend(truncColor | (a << 24), cv[y * W + x]);
        intery += grad;
      }
    }
  }

  void drawTriangleFlat(int x0, int y0, int w, int xt, int yt, uint32_t color) {
    float grad1 = (float)(x0 - xt) / (float)(y0 - yt);
    float grad2 = (float)(x0 + w - xt) / (float)(y0 - yt);

    float intery1 = x0;
    float intery2 = x0 + w;
    for (int y = y0; y != yt; y = yt > y0 ? y + 1 : y - 1) {

      int x1 = intery1;
      int x2 = intery2;
      if (x2 < x1)
        swap(x1, x2);
      for (int x = clampX(x1 + 1); x < clampX(x2 + 1); ++x) {
        cv[y * W + x] = blend(color, cv[y * W + x]);
      }
      //   cv[y * W + clampX(x1)] = blend(color, cv[y * W + clampX(x1)]);
      //   cv[y * W + clampX(x2)] = blend(color, cv[y * W + clampX(x2)]);
      intery1 = yt > y0 ? intery1 + grad1 : intery1 - grad1;
      intery2 = yt > y0 ? intery2 + grad2 : intery2 - grad2;
    }
    LOG(x0 << ' ' << y0)
    LOG(xt << ' ' << yt)
    drawLine(x0, y0, xt, yt, color);
    drawLine(x0 + w, y0, xt, yt, color);
  }
  void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2,
                    uint32_t color) {
    if (y0 >= y1 && y1 >= y2) {
      float grad = (float)(x0 - x2) / (float)(y0 - y2);
      int xm = grad * (y1 - y2) + x2;
      int w = xm - x1;
      drawTriangleFlat(x1, y1, w, x0, y0, color);
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

Canvas<H, W> cv;

extern "C" {
void test() {
  cv.fill(0xff00ff00);
  cv.drawRect(W / 4, H / 6, W / 2, H * 2 / 3, 0xa0ff0000);
  cv.drawCircle(W / 2, H / 2, H / 3, 0xe00000ff, 3);
  cv.drawLine(400, 0, H, W, 0xffffffff);
  cv.drawTriangleFlat(W / 4, H * 2 / 3, W / 2, W / 2, H / 4, 0xe000ffff);
  cv.drawTriangle(0, 0, 100, 50, 244, 153, 0xf0ff00ff);
  cv.drawLine(0, H * 2 / 3 + 1, W, H * 2 / 3 + 1, 0xff000000);

  cv.drawLine(100, 100, 200, 30, 0xff000000);

  cv.drawLine(210, 30, 110, 100, 0xff000000);

  cv.drawLine(0, 100 + 2, W, 100 + 2, 0xff000000);
  cv.drawLine(0, 30 - 2, W, 30 - 2, 0xff000000);
}
int WIDTH() { return W; }
int HEIGHT() { return H; }
}

int main() { test(); }
