#include "Canvas.hpp"
#include "utilities.hpp"
#include <stdint.h>
const int H = 500;
const int W = 500;
Canvas<H, W> cv;
const int cz = 500;
const int POV = 90;
const float speed = 0.01;
extern "C" {
void render(int x, int y) {
  cv.fill(0xff000000);
  cv.drawCircle(x, y, 50, CONST_PICKER(0xffffffff));
}
int main() { render(0, 0); }
}

COMMON_EXPORTS(W, H)
