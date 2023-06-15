#include "Canvas.hpp"
#include "Space.hpp"
#include "utilities.hpp"
#include <stdint.h>
const int H = 500;
const int W = 500;
Canvas<H, W> cv;
const int cz = 800;
const int POV = 60;
const float speed = 0.01;

extern "C" {
void render(int dt) {
  Vec3D<float> a{-200, 0, -100};
  Vec3D<float> b{0, -100, 200};
  Vec3D<float> c{300, 200, 0};

  Camera camera{{0, 0, cz}, POV, {0, 0, 0}};

  cv.fill(0xff000000);
  float angle = dt * speed;
  Vec3D<float> rotations = {0, angle};
  a = Vec3D<float>::rotateAll(a, rotations);
  b = Vec3D<float>::rotateAll(b, rotations);
  c = Vec3D<float>::rotateAll(c, rotations);
  Vec3D<float> v1 = a - b;
  Vec3D<float> v2 = a - c;
  uint32_t color;
  Vec3D<float> normal = Vec3D<float>::crossProduct(v1, v2);
  int h = 0, s = 100, v = 100;
  if (normal.z > 0)
    h = 90;
  Vec3D<float> aCam = camera.projectOnCamera(a);
  Vec3D<float> bCam = camera.projectOnCamera(b);
  Vec3D<float> cCam = camera.projectOnCamera(c);

  Vec3D<float> aCv = camera.projectOnCanvas(a, cv);
  Vec3D<float> bCv = camera.projectOnCanvas(b, cv);
  Vec3D<float> cCv = camera.projectOnCanvas(c, cv);

  auto plane = planeFromNormal(normal, a);
  cv.drawTriangle(
      aCv.x, aCv.y, bCv.x, bCv.y, cCv.x, cCv.y, [&](int x, int y) -> uint32_t {
        Vec3D<float> cameraViewCoord = camera.canvas2CameraView(x, y, cv);
        float d = (aCam * normal) / (cameraViewCoord * normal);
        Vec3D<float> p = (d * cameraViewCoord) + camera.pos;
        int vz = p.z / 6 + 50;
        LOG(vz);
        if (vz > 100)
          vz = 100;
        if (vz < 0)
          vz = 0;
        vz = 100 - vz;
        // LOG(cameraViewCoord)
        return HSV2RGB(h, s, vz) | 0xff000000;

        // if (z)
      });
}

int main() { render(4000); }
}

COMMON_EXPORTS(W, H)