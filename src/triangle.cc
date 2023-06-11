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
void render(int dt) {
  int x1 = -200;
  int y1 = 0;
  int z1 = -100;

  int x2 = 0;
  int y2 = -100;
  int z2 = 200;

  int x3 = 300;
  int y3 = 200;
  int z3 = 0;

  int r = 5000;
  cv.fill(0xff000000);
  float angle = dt * speed;
  rotate(x1, z1, angle);
  rotate(x2, z2, angle);
  rotate(x3, z3, angle);
  cv.drawTriangle(projectScreen(project(x1, cz + z1, 0), W, POV),
                  projectScreen(project(y1, cz + z1, 0), H, POV),
                  projectScreen(project(x2, cz + z2, 0), W, POV),
                  projectScreen(project(y2, cz + z2, 0), H, POV),
                  projectScreen(project(x3, cz + z3, 0), W, POV),
                  projectScreen(project(y3, cz + z3, 0), H, POV),
                  [](int x, int y) -> uint32_t { return 0xffff0000; });
  //   cv.drawCircle(projectScreen(project(x1, cz + z1, 0), W, POV),
  //                 projectScreen(project(y1, cz + z1, 0), H, POV),
  //                 r / sqrtp((cz + z1) * (cz + z1) + x1 * x1 + y1 * y1),
  //                 CONST_PICKER(0xffff0000));
  //   cv.drawCircle(projectScreen(project(x2, cz + z2, 0), W, POV),
  //                 projectScreen(project(y2, cz + z2, 0), H, POV),
  //                 r / sqrtp((cz + z2) * (cz + z2) + x2 * x2 + y2 * y2),
  //                 CONST_PICKER(0xffff0000));
  //   cv.drawCircle(projectScreen(project(x3, cz + z3, 0), W, POV),
  //                 projectScreen(project(y3, cz + z3, 0), H, POV),
  //                 r / sqrtp((cz + z3) * (cz + z3) + x3 * x3 + y3 * y3),
  //                 CONST_PICKER(0xffff0000));
}

int main() { render(0); }
}

COMMON_EXPORTS(W, H)