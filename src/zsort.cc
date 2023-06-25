

#include "Canvas.hpp"
#include "Math.hpp"
#include "Space.hpp"
#include "Vec3D.h"
#include <stdint.h>

int test[1] = {1000000000};
const int H = 500;
const int W = 500;
Canvas<H, W> cv;
int buf[W * H];
const int cz = -300;
const float POV = 60;
const float speed = 0.01;
const int l = 150;

extern "C" void putInt(int x);
#ifdef DEBUG
void putInt(int x) { printf("%i\n", x); }
#endif

using namespace gcmath;

extern "C" {
void render(int dt) {

  cv.fill(0xff000000);
  for (int i = 0; i < W * H; ++i) {
    buf[i] = 9999999;
  }
  //   int x1, x2, x3, x4;
  //   int z1, z2, z3, z4;

  //   int yt = cw / 2;
  //   int yd = -cw / 2;

  //   x1 = -cw / 2;
  //   x3 = x1;
  //   x2 = cw / 2;
  //   x4 = x2;

  //   z1 = cw / 2;
  //   z2 = z1;
  //   z3 = -cw / 2;
  //   z4 = z3;
  float angle = dt * speed;
  //   angle = 50;
  /*    z
   *    |
   * x3--x4
   * |  . |--x
   * x1--x2
   *
   *
   * camera
   */
  Camera camera{{0, 0, cz}, POV, {0, 0, 0}};
#define test(v, color)                                                         \
  Vec3D cv_##v = camera.projectOnCanvas(v, cv);                                \
  Vec3D cm_##v = camera.projectOnCamera(v);
  static Vec3D points[] = {
      {-l / 2., -l / 2., -l / 2.}, {l / 2., -l / 2., -l / 2.},
      {-l / 2., -l / 2., l / 2.},  {l / 2., -l / 2., l / 2.},
      {-l / 2., l / 2., -l / 2.},  {l / 2., l / 2., -l / 2.},
      {-l / 2., l / 2., l / 2.},   {l / 2., l / 2., l / 2.},
  };
  int points_len = sizeof(points) / sizeof(Vec3D);

  Vec3D rotations = {angle * 0.5f, 45, -angle};
  for (int i = 0; i < points_len; ++i) {
    Vec3D cv_p = camera.projectOnCanvas(points[i], cv);
    cv.drawCircle(cv_p.x, cv_p.y, 10, CONST_PICKER(0xffff0000));
  }
  Vec3D cv_p = camera.projectOnCanvas(points[5], cv);
  for (auto &p : points) {
    Vec3D cm_p = camera.projectOnCanvas(p, cv);
    cv.drawCircle(cm_p.x, cm_p.y, 5, CONST_PICKER(0xffff0000));
  }
  cv.drawCircle(cv_p.x, cv_p.y, 10, CONST_PICKER(0xffff0000));

#undef test
}
int main() { render(0); }
COMMON_EXPORTS(W, H)
}
