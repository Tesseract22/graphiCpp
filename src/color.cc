

#include "Canvas.hpp"
#include "utilities.hpp"
#include <stdint.h>
const int H = 500;
const int W = 500;
Canvas<H, W> cv;

extern "C" {
int main() {
  cv.fill(0xff000000);
  cv.drawRect(W / 4, H / 6, W / 2, H * 2 / 3, [](int x, int y) {
    int h = (y - H / 6) * 360 / (H * 2 / 3);
    return HSV2RGB(h, 75, 75);
  });
  //   cv.drawCircle(
  //       W / 2, H / 2, H / 3,
  //       [](int x, int y) {
  //         int r = (x - W / 2) * (x - W / 2) + (y - H / 2) * (y - H / 2);
  //         uint8_t g = r * 0xff / (H * H / 9);
  //         return 0xa0f00000 | (g << 8);
  //       },
  //       3);
  cv.drawLine(0, 0, H, W, [](int x, int y) {
    uint8_t r = x * 255 / W;
    uint8_t g = 255 - y * 255 / H;
    return 0xff000000 | (r) | (g << 8);
  });
  // 0x000ff 0x00ffff 0x00ff00 - 0xff0000
  //   cv.drawTriangleFlat(W / 4, H * 2 / 3, W / 2, W / 2, H / 4, 0xe000ffff);
  //   cv.drawTriangle(0, 0, 100, 50, 244, 153, 0xf0ff00ff);
  //   cv.drawLine(0, H * 2 / 3 + 1, W, H * 2 / 3 + 1, 0xff000000);

  //   cv.drawLine(100, 100, 200, 30, 0xff000000);

  //   cv.drawLine(210, 30, 110, 100, 0xff000000);

  //   cv.drawLine(0, 100 + 2, W, 100 + 2, 0xff000000);
  //   cv.drawLine(0, 30 - 2, W, 30 - 2, 0xff000000);
}
int WIDTH() { return W; }
int HEIGHT() { return H; }
}
