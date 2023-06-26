

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
const int cz = -600;
const int cy = -200;
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
  cv.blending = false;
  Vec3D rotations = {0, dt * speed, 0};
  static Camera camera{{0, cy, cz}, POV, {0, 0, 0}};
  static auto make_picker = [=](Vec3D a, Vec3D b, Vec3D c, int hue) {
    return [=](int x, int y) -> uint32_t {
      Vec3D cameraViewCoord = camera.canvas2CameraView(x, y, cv);
      Vec3D normal = Vec3D::crossProduct(a - b, a - c);
      float d = (a * normal) / (cameraViewCoord * normal);
      Vec3D p = (d * cameraViewCoord);
      if (buf[y * W + x] > p.z) {
        buf[y * W + x] = p.z;
        p = p + camera.pos;
        int vz = (p.z) / 6 + 50;
        if (vz > 100)
          vz = 100;
        if (vz < 0)
          vz = 0;
        vz = 100 - vz;

        return 0xff000000 | HSV2RGB(hue, vz, vz);
      }
      return 0;
    };
  };

  cv.fill(0xff000000);
  for (int i = 0; i < W * H; ++i)
    buf[i] = 999999999;

  for (int i = 0; i < flen; ++i) {
    // for fucking god sake
    // faces are 1-index, not 0-index
    Vec3D cm_1 =
        camera.projectOnCamera(Vec3D::rotateAll(vs[fs[i][0] - 1], rotations));
    Vec3D cm_2 =
        camera.projectOnCamera(Vec3D::rotateAll(vs[fs[i][1] - 1], rotations));
    Vec3D cm_3 =
        camera.projectOnCamera(Vec3D::rotateAll(vs[fs[i][2] - 1], rotations));
    Vec3D cv_1 = camera.projectOnCanvas(
        Vec3D::rotateAll(vs[fs[i][0] - 1], rotations), cv);
    Vec3D cv_2 = camera.projectOnCanvas(
        Vec3D::rotateAll(vs[fs[i][1] - 1], rotations), cv);
    Vec3D cv_3 = camera.projectOnCanvas(
        Vec3D::rotateAll(vs[fs[i][2] - 1], rotations), cv);

    // putInt(fs[i][0]);
    // putInt(fs[i][1]);
    // putInt(fs[i][2]);
    cv.drawTriangle(cv_1.x, cv_1.y, cv_2.x, cv_2.y, cv_3.x, cv_3.y,
                    make_picker(cm_1, cm_2, cm_3, 120));
    // cv.drawLine(cv_1.x, cv_1.y, cv_2.x, cv_2.y, CONST_PICKER(0X3f0000ff));
    // cv.drawLine(cv_3.x, cv_3.y, cv_2.x, cv_2.y, CONST_PICKER(0X3f00ff00));
    // cv.drawLine(cv_3.x, cv_3.y, cv_1.x, cv_1.y, CONST_PICKER(0X3fff0000));
  }
  //   for (int i = 0; i < vlen; ++i) {
  //     Vec3D p = Vec3D::rotateAll(vs[i], rotations);
  //     Vec3D cm_p = camera.projectOnCanvas(p, cv);
  //     cv.drawPixel(cm_p.x, cm_p.y, 0xffff7f7f);
  //   }
}
int main() { render(0); }
COMMON_EXPORTS(W, H)
}
