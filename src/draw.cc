#include "Canvas.hpp"
#include "Space.hpp"
#include "utilities.hpp"
#include <stdint.h>

const int H = 2000;
const int W = 2000;
Canvas<H, W> cv;
const int cz = 500;
const int POV = 90;
const float speed = 0.01;
extern "C" {
void draw(int x, int y) { cv.drawCircle(x, y, 10, CONST_PICKER(0xffffffff)); }
void init() { cv.fill(0xff000000); }

int main() { init(); }
}

COMMON_EXPORTS(W, H)
