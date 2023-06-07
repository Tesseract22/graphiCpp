#include "Canvas.hpp"
#include "utilities.hpp"
#include <stdint.h>

const int H = 500;
const int W = 500;
Canvas<H, W> cv;

extern "C" {
int main() {
  cv.fill(0xff00ff00);
  cv.drawRect(W / 4, H / 6, W / 2, H * 2 / 3,
              [](int x, int y) { return 0xffff0000; });
  cv.drawCircle(W / 2, H / 2, H / 3, CONST_PICKER(0xff0000ff), 3);
  cv.drawLine(400, 0, H, W, CONST_PICKER(0xff000000));
  IMPORTED(sinp(5);)
  cv.drawTriangleFlat(W / 4, H * 2 / 3, W / 2, W / 2, H / 4,
                      CONST_PICKER(0xe000ffff));
  cv.drawTriangle(W / 2, H / 5, W / 4, H * 3 / 4, W / 2, H / 2,
                  CONST_PICKER(0x7fff00ff));
  cv.drawTriangle(W / 2, H / 5, W * 3 / 4, H * 3 / 4, W / 2, H / 2,
                  CONST_PICKER(0x7fff00ff));
  //   cv.drawLine(0, H * 2 / 3 + 1, W, H * 2 / 3 + 1, 0xff000000);

  //   cv.drawLine(100, 100, 200, 30, 0xff000000);

  //   cv.drawLine(210, 30, 110, 100, 0xff000000);

  //   cv.drawLine(0, 100 + 2, W, 100 + 2, 0xff000000);
  //   cv.drawLine(0, 30 - 2, W, 30 - 2, 0xff000000);
}
}

COMMON_EXPORTS(W, H)