

#include "Canvas.hpp"
#include "Demo.h"
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

using namespace gcmath;

extern "C" {
void render(int dt) {
  cv.blending = false;
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
  Vec3D x1{-l / 2, -l / 2, -l / 2};
  Vec3D x2{l / 2, -l / 2, -l / 2};
  Vec3D x3{-l / 2, -l / 2, l / 2};
  Vec3D x4{l / 2, -l / 2, l / 2};

  Vec3D x11{-l / 2, l / 2, -l / 2};
  Vec3D x22{l / 2, l / 2, -l / 2};
  Vec3D x33{-l / 2, l / 2, l / 2};
  Vec3D x44{l / 2, l / 2, l / 2};
  Vec3D rotations = {angle * 0.5f, 0, -angle};
  x1 = Vec3D::rotateAll(x1, rotations);
  x2 = Vec3D::rotateAll(x2, rotations);
  x3 = Vec3D::rotateAll(x3, rotations);
  x4 = Vec3D::rotateAll(x4, rotations);
  x11 = Vec3D::rotateAll(x11, rotations);
  x22 = Vec3D::rotateAll(x22, rotations);
  x33 = Vec3D::rotateAll(x33, rotations);
  x44 = Vec3D::rotateAll(x44, rotations);

  //   LOG(camera.projectOnCameraView(x1))
  //   LOG(camera.projectOnCameraView(x2))
  //   LOG(camera.projectOnCameraView(x3))
  //   LOG(camera.projectOnCameraView(x4))

  test(x1, 0x7fff0000) test(x2, 0x7f00ff00) test(x3, 0x7f0000ff)
      test(x4, 0x7f007f7f) test(x11, 0x7fff0000) test(x22, 0x7f00ff00)
          test(x33, 0x7f0000ff) test(x44, 0x7f007f7f);
  const uint32_t alpha = 0xff000000;
  auto make_picker = [&camera](Vec3D a, Vec3D b, Vec3D c, int hue,
                               float zshift = 1) {
    return [&camera, a, b, c, hue, zshift](int x, int y) -> uint32_t {
      Vec3D cameraViewCoord = camera.canvas2CameraView(x, y, cv);
      Vec3D normal = Vec3D::crossProduct(a - b, a - c);
      float d = (a * normal) / (cameraViewCoord * normal);
      Vec3D p = (d * cameraViewCoord) + camera.pos;
      p.z += zshift;
      if (buf[y * W + x] > p.z) {
        int vz = p.z / 6 + 50;
        vz = gcmath::min(gcmath::max(vz, 100), 0);
        vz = 100 - vz;
        buf[y * W + x] = p.z;
        return alpha | HSV2RGB(hue, 100, vz);
      }
      return 0;
    };
  };

  const uint32_t c1 = (alpha) | HSV2RGB(0, 100, 100);
  const uint32_t c2 = (alpha) | HSV2RGB(60, 100, 100);
  const uint32_t c3 = (alpha) | HSV2RGB(120, 100, 100);
  const uint32_t c4 = (alpha) | HSV2RGB(180, 100, 100);
  const uint32_t c5 = (alpha) | HSV2RGB(240, 100, 100);
  const uint32_t c6 = (alpha) | HSV2RGB(300, 100, 100);
  //   auto isPointOnLine = [](float x1, float y1, float x2, float y2, float x,
  //                           float y) {
  //     if (x1 == x2 || x == x1)
  //       return x == x2;
  //     if (y1 == y2 || y == y1)
  //       return y == y2;
  //     return ((y2 - y1) / (x2 - x1)) == ((y - y1) / (x - x1));
  //   };
  cv.drawTriangle(cv_x1.x, cv_x1.y, cv_x11.x, cv_x11.y, cv_x2.x, cv_x2.y,
                  make_picker(cm_x1, cm_x11, cm_x2, 0));

  cv.drawTriangle(cv_x22.x, cv_x22.y, cv_x11.x, cv_x11.y, cv_x2.x, cv_x2.y,
                  make_picker(cm_x1, cm_x11, cm_x2, 0));

  cv.drawTriangle(cv_x1.x, cv_x1.y, cv_x11.x, cv_x11.y, cv_x3.x, cv_x3.y,
                  make_picker(cm_x1, cm_x11, cm_x3, 60));
  cv.drawTriangle(cv_x33.x, cv_x33.y, cv_x11.x, cv_x11.y, cv_x3.x, cv_x3.y,
                  make_picker(cm_x33, cm_x11, cm_x3, 60));

  cv.drawTriangle(cv_x2.x, cv_x2.y, cv_x22.x, cv_x22.y, cv_x4.x, cv_x4.y,
                  make_picker(cm_x2, cm_x22, cm_x4, 120));
  cv.drawTriangle(cv_x44.x, cv_x44.y, cv_x22.x, cv_x22.y, cv_x4.x, cv_x4.y,
                  make_picker(cm_x44, cm_x22, cm_x4, 120));

  cv.drawTriangle(cv_x3.x, cv_x3.y, cv_x33.x, cv_x33.y, cv_x4.x, cv_x4.y,
                  make_picker(cm_x3, cm_x33, cm_x4, 180));
  cv.drawTriangle(cv_x44.x, cv_x44.y, cv_x33.x, cv_x33.y, cv_x4.x, cv_x4.y,
                  make_picker(cm_x44, cm_x33, cm_x4, 180));

  cv.drawTriangle(cv_x1.x, cv_x1.y, cv_x2.x, cv_x2.y, cv_x3.x, cv_x3.y,
                  make_picker(cm_x1, cm_x2, cm_x3, 240));
  cv.drawTriangle(cv_x4.x, cv_x4.y, cv_x2.x, cv_x2.y, cv_x3.x, cv_x3.y,
                  make_picker(cm_x1, cm_x2, cm_x3, 240));

  cv.drawTriangle(cv_x11.x, cv_x11.y, cv_x22.x, cv_x22.y, cv_x33.x, cv_x33.y,
                  make_picker(cm_x11, cm_x22, cm_x33, 300));
  cv.drawTriangle(cv_x44.x, cv_x44.y, cv_x22.x, cv_x22.y, cv_x33.x, cv_x33.y,
                  make_picker(cm_x44, cm_x22, cm_x33, 300));

#undef test
}
COMMON_EXPORTS(W, H)
}
