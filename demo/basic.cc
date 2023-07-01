#include "Canvas.hpp"
#include "Demo.h"
#include <stdint.h>

const int H = 500;
const int W = 500;
Canvas<H, W> cv;

extern "C" {

void render(int dt) {
  cv.fill(0xff00ff00);
  cv.drawRect(W / 4, H / 6, W / 2, H * 2 / 3,
              [](int x, int y) { return 0xffff0000; });
  cv.drawCircle(W / 2, H / 2, H / 3, CONST_PICKER(0xff0000ff), 3);
  cv.drawLine(0, 0, H, W, CONST_PICKER(0xff000000));
  cv.drawTriangleFlat(W / 4, H * 2 / 3, W / 2, W / 2, H / 4,
                      CONST_PICKER(0xe000ffff));
  cv.drawTriangle(W / 2, H / 5, W / 4, H * 3 / 4, W / 2, H / 2,
                  CONST_PICKER(0x7fff00ff));
  cv.drawTriangle(W / 2, H / 5, W * 3 / 4, H * 3 / 4, W / 2, H / 2,
                  CONST_PICKER(0x7fff00ff));
  cv.drawEllipse(W / 2, H / 2, H / 4, H / 3, CONST_PICKER(0x7fffff00), 3);

  cv.drawEllipse(W / 4, H / 4, H / 6, H / 9, CONST_PICKER(0x40ff0000), 30, 3);

  cv.drawTriangle(100, 20, 300, 20, 200, 20, CONST_PICKER(0x7fff7f00));

  cv.drawTriangleFlat(50, 200, 100, 20, 200, CONST_PICKER(0xff000000));
}
}

COMMON_EXPORTS(W, H)