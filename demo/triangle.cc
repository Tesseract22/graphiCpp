#include "Canvas.hpp"
#include "Demo.h"
#include "Math.hpp"
#include "Space.hpp"
#include "Vec3D.h"
#include <stdint.h>

const int H = 500;
const int W = 500;
Canvas<H, W> cv;
const int cz = -600;
const int POV = 60;
const float speed = 0.01;
using gcmath::HSV2RGB;
extern "C" {
void render(int dt) {
  Vec3D a{-200, 0, -100};
  Vec3D b{0, -100, 200};
  Vec3D c{300, 200, 0};

  Camera camera{{0, 0, cz}, POV, {0, 0, 0}};

  cv.fill(0xff000000);
  float angle = dt * speed;
  Vec3D rotations = {0, angle};
  a = Vec3D::rotateAll(a, rotations);
  b = Vec3D::rotateAll(b, rotations);
  c = Vec3D::rotateAll(c, rotations);
  Vec3D v1 = a - b;
  Vec3D v2 = a - c;
  uint32_t color;
  Vec3D normal = Vec3D::crossProduct(v1, v2);
  int h = 0, s = 100, v = 100;
  if (normal.z > 0)
    h = 90;
  Vec3D aCam = camera.projectOnCamera(a);
  Vec3D bCam = camera.projectOnCamera(b);
  Vec3D cCam = camera.projectOnCamera(c);

  Vec3D aCv = camera.projectOnCanvas(a, cv);
  Vec3D bCv = camera.projectOnCanvas(b, cv);
  Vec3D cCv = camera.projectOnCanvas(c, cv);

  auto plane = Vec3D::planeFromNormal(normal, a);
  cv.drawTriangle(
      aCv.x, aCv.y, bCv.x, bCv.y, cCv.x, cCv.y,
      [&](int x, int y) -> uint32_t {
        Vec3D cameraViewCoord = camera.canvas2CameraView(x, y, cv);
        float d = (aCam * normal) / (cameraViewCoord * normal);
        Vec3D p = (d * cameraViewCoord) + camera.pos;
        int vz = p.z / 6 + 50;
        if (vz > 100)
          vz = 100;
        if (vz < 0)
          vz = 0;
        vz = 100 - vz;
        return HSV2RGB(h, s, vz) | 0xff000000;
      },
      true);
}
}

COMMON_EXPORTS(W, H)