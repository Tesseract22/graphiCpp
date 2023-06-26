

#include "Canvas.hpp"
#include "Math.hpp"
#include "Space.hpp"
#include "Tea.hpp"
#include "Vec3D.h"
#include <stdint.h>

int test[1] = {1000000000};
const int H = 1000;
const int W = 1000;
Canvas<H, W> cv;
int buf[W * H];
const int cz = -5;
const float POV = 60;
const float speed = 0.01;

extern "C" void putInt(int x);
extern "C" void putFloat(int x);
#ifdef DEBUG
void putInt(int x) { printf("%i\n", x); }
void putFloat(float x) { printf("%f\n", x); }
#endif

using namespace gcmath;

extern "C" {
void render(int dt) {
  Vec3D rotations = {0, dt * speed, 0};
  static Camera camera{{0, -5, cz}, POV, {30, 0, 0}};
  cv.fill(0xff000000);
  for (int i = 0; i < vlen; ++i) {
    Vec3D p = Vec3D::rotateAll({vs[i][0], vs[i][1], vs[i][2]}, rotations);
    Vec3D cm_p = camera.projectOnCanvas(p, cv);
    // putInt(1111111111);
    // cv.drawCircle(cm_p.x, cm_p.y, 5, CONST_PICKER(0xffff007f));
    cv.drawPixel(cm_p.x, cm_p.y, 0xffff7f7f);
  }
  for (int i = 85; i < 100; ++i) {
    Vec3D cm_1 = camera.projectOnCanvas(
        Vec3D::rotateAll({vs[fs[i][0]][0], vs[fs[i][0]][1], vs[fs[i][0]][2]},
                         rotations),
        cv);
    Vec3D cm_2 = camera.projectOnCanvas(
        Vec3D::rotateAll({vs[fs[i][1]][0], vs[fs[i][1]][1], vs[fs[i][1]][2]},
                         rotations),
        cv);
    Vec3D cm_3 = camera.projectOnCanvas(
        Vec3D::rotateAll({vs[fs[i][2]][0], vs[fs[i][2]][1], vs[fs[i][2]][2]},
                         rotations),
        cv);

    Vec3D cv_1 = camera.projectOnCamera(Vec3D::rotateAll(
        {vs[fs[i][0]][0], vs[fs[i][0]][1], vs[fs[i][0]][2]}, rotations));
    Vec3D cv_2 = camera.projectOnCamera(Vec3D::rotateAll(
        {vs[fs[i][1]][0], vs[fs[i][1]][1], vs[fs[i][1]][2]}, rotations));
    Vec3D cv_3 = camera.projectOnCamera(Vec3D::rotateAll(
        {vs[fs[i][2]][0], vs[fs[i][2]][1], vs[fs[i][2]][2]}, rotations));

    auto make_picker = [=](Vec3D a, Vec3D b, Vec3D c, int hue) {
      return [=](int x, int y) -> uint32_t {
        Vec3D cameraViewCoord = camera.canvas2CameraView(x, y, cv);
        Vec3D normal = Vec3D::crossProduct(a - b, a - c);
        float d = (a * normal) / (cameraViewCoord * normal);
        Vec3D p = (d * cameraViewCoord) + camera.pos;

        if (buf[y * W + x] > p.z) {
          return 0xffff7f00;
        }
        return 0;
      };
    };

    cv.drawTriangle(cm_1.x, cm_1.y, cm_2.x, cm_2.y, cm_3.x, cm_3.y,
                    CONST_PICKER(0xff003f7f));
    // cv.drawLine(cm_1.x, cm_1.y, cm_2.x, cm_2.y, CONST_PICKER(0X3f0000ff));
    // cv.drawLine(cm_3.x, cm_3.y, cm_2.x, cm_2.y, CONST_PICKER(0X3f00ff00));
    // cv.drawLine(cm_3.x, cm_3.y, cm_1.x, cm_1.y, CONST_PICKER(0X3fff0000));
  }
}
int main() { render(0); }
COMMON_EXPORTS(W, H)
}
