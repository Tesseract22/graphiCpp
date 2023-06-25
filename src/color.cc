

#include "Canvas.hpp"
#include "Math.hpp"
#include <stdint.h>
const int H = 500;
const int W = 500;
Canvas<H, W> cv;
using gcmath::HSV2RGB;
using gcmath::pointLineDist;
extern "C" {

void render(int dt) {
  cv.fill(0xff000000);
  cv.drawRect(W / 4, H / 6, W / 2, H * 2 / 3, [](int x, int y) {
    int h = (y - H / 6) * 360 / (H * 2 / 3);
    return HSV2RGB(h, 100, 100) | 0xff000000;
  });
  cv.drawCircle(
      W / 2, H / 2, H / 4,
      [](int x, int y) -> uint32_t {
        int r = (x - W / 2) * (x - W / 2) + (y - H / 2) * (y - H / 2);
        uint8_t g = r * 0xff / (H * H / 16);
        return 0x7ff00000 | (g << 8);
      },
      3);
  cv.drawLine(0, 0, H, W, [](int x, int y) {
    uint8_t r = x * 255 / W;
    uint8_t g = 255 - y * 255 / H;
    return 0xff000000 | (r) | (g << 8);
  });
  cv.drawTriangle(0, H / 2, W / 4, 0, W / 4, H, [](int x, int y) -> uint32_t {
    LOG(x << ' ' << y)
    static float ls1 = pointLineDist(0, H / 2, W / 4, 0, W / 4, H);
    float l1 = pointLineDist(x, y, W / 4, 0, W / 4, H);
    uint8_t r = l1 / ls1 * 255;

    static float ls2 = pointLineDist(W / 4, 0, W / 4, H, 0, H / 2);
    float l2 = pointLineDist(x, y, W / 4, H, 0, H / 2);
    uint8_t g = l2 / ls2 * 255;

    static float ls3 = pointLineDist(W / 4, H, 0, H / 2, W / 4, 0);
    float l3 = pointLineDist(x, y, 0, H / 2, W / 4, 0);
    uint8_t b = l3 / ls3 * 255;
    return 0xff000000 | r | (g << 8) | (b << 16);
  });
}
int main() { render(0); }
COMMON_EXPORTS(W, H)
}
