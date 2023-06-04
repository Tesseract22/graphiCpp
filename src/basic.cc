#include "Canvas.hpp"
#include <stdint.h>

const int H = 500;
const int W = 500;
Canvas<H, W> cv;

extern "C" {
int main() {
  cv.fill(0xff00ff00);
  cv.drawRect(W / 4, H / 6, W / 2, H * 2 / 3, CONST_PICKER(0xffff0000));
  cv.drawCircle(W / 2, H / 2, H / 3, CONST_PICKER(0xff0000ff), 3);
  cv.drawLine(400, 0, H, W, CONST_PICKER(0xff000000));
  // cv.drawTriangleFlat(W / 4, H * 2 / 3, W / 2, W / 2, H / 4, 0xe000ffff);
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