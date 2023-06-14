

#include "Canvas.hpp"
#include "utilities.hpp"
#include <stdint.h>
const int H = 500;
const int W = 500;
Canvas<H, W> cv;
const int cz = 500;
const float POV = 60;
const int cw = 300;
const float speed = 0.01;
extern "C" {
void render(int dt) {

  cv.fill(0xff000000);
  int x1, x2, x3, x4;
  int z1, z2, z3, z4;

  int yt = cw / 2;
  int yd = -cw / 2;

  x1 = -cw / 2;
  x3 = x1;
  x2 = cw / 2;
  x4 = x2;

  z1 = cw / 2;
  z2 = z1;
  z3 = -cw / 2;
  z4 = z3;
  float angle = dt * speed;
  //   angle = 50;
  /*
   *
   * x3--x4
   * |  . |--x
   * x1--x2
   *    |
   *    z
   * camera
   */

  auto rotate = [](int &x, int &y, float angle) -> void {
    int tmpX = cosp(DEG2RAD(angle)) * x - sinp(DEG2RAD(angle)) * y;
    y = sinp(DEG2RAD(angle)) * x + cosp(DEG2RAD(angle)) * y;
    x = tmpX;
  };
  rotate(x1, z1, angle);
  rotate(x2, z2, angle);
  rotate(x3, z3, angle);
  rotate(x4, z4, angle);

  int y0 = (H - cw) / 2;
  // back
  cv.drawLine(projectScreen(project(x1, z1 + cz, 0), W, POV),
              projectScreen(project(yt, z1 + cz, 0), H, POV),
              projectScreen(project(x2, z2 + cz, 0), W, POV),
              projectScreen(project(yt, z2 + cz, 0), H, POV),
              CONST_PICKER(0xff0000ff));

  cv.drawLine(projectScreen(project(x1, z1 + cz, 0), W, POV),
              projectScreen(project(yd, z1 + cz, 0), H, POV),
              projectScreen(project(x2, z2 + cz, 0), W, POV),
              projectScreen(project(yd, z2 + cz, 0), H, POV),
              CONST_PICKER(0xff0000ff));

  cv.drawLine(projectScreen(project(x1, z1 + cz, 0), W, POV),
              projectScreen(project(yt, z1 + cz, 0), H, POV),
              projectScreen(project(x1, z1 + cz, 0), W, POV),
              projectScreen(project(yd, z1 + cz, 0), H, POV),
              CONST_PICKER(0xff0000ff));

  cv.drawLine(projectScreen(project(x2, z2 + cz, 0), W, POV),
              projectScreen(project(yt, z2 + cz, 0), H, POV),
              projectScreen(project(x2, z2 + cz, 0), W, POV),
              projectScreen(project(yd, z2 + cz, 0), H, POV),
              CONST_PICKER(0xff0000ff));
  // front
  cv.drawLine(projectScreen(project(x3, z3 + cz, 0), W, POV),
              projectScreen(project(yt, z3 + cz, 0), H, POV),
              projectScreen(project(x4, z4 + cz, 0), W, POV),
              projectScreen(project(yt, z4 + cz, 0), H, POV),
              CONST_PICKER(0xff00ff00));

  cv.drawLine(projectScreen(project(x3, z3 + cz, 0), W, POV),
              projectScreen(project(yd, z3 + cz, 0), H, POV),
              projectScreen(project(x4, z4 + cz, 0), W, POV),
              projectScreen(project(yd, z4 + cz, 0), H, POV),
              CONST_PICKER(0xff00ff00));

  cv.drawLine(projectScreen(project(x3, z3 + cz, 0), W, POV),
              projectScreen(project(yt, z3 + cz, 0), H, POV),
              projectScreen(project(x3, z3 + cz, 0), W, POV),
              projectScreen(project(yd, z3 + cz, 0), H, POV),
              CONST_PICKER(0xff00ff00));

  cv.drawLine(projectScreen(project(x4, z4 + cz, 0), W, POV),
              projectScreen(project(yt, z4 + cz, 0), H, POV),
              projectScreen(project(x4, z4 + cz, 0), W, POV),
              projectScreen(project(yd, z4 + cz, 0), H, POV),
              CONST_PICKER(0xff00ff00));
  // left

  cv.drawLine(projectScreen(project(x1, z1 + cz, 0), W, POV),
              projectScreen(project(yt, z1 + cz, 0), H, POV),
              projectScreen(project(x3, z3 + cz, 0), W, POV),
              projectScreen(project(yt, z3 + cz, 0), H, POV),
              CONST_PICKER(0xffff0000));

  cv.drawLine(projectScreen(project(x1, z1 + cz, 0), W, POV),
              projectScreen(project(yd, z1 + cz, 0), H, POV),
              projectScreen(project(x3, z3 + cz, 0), W, POV),
              projectScreen(project(yd, z3 + cz, 0), H, POV),
              CONST_PICKER(0xffff0000));
  // right
  cv.drawLine(projectScreen(project(x2, z2 + cz, 0), W, POV),
              projectScreen(project(yt, z2 + cz, 0), H, POV),
              projectScreen(project(x4, z4 + cz, 0), W, POV),
              projectScreen(project(yt, z4 + cz, 0), H, POV),
              CONST_PICKER(0xffff0000));

  cv.drawLine(projectScreen(project(x2, z2 + cz, 0), W, POV),
              projectScreen(project(yd, z2 + cz, 0), H, POV),
              projectScreen(project(x4, z4 + cz, 0), W, POV),
              projectScreen(project(yd, z4 + cz, 0), H, POV),
              CONST_PICKER(0xffff0000));
}
int main() { render(9000); }
COMMON_EXPORTS(W, H)
}
