#include "Canvas.hpp"
#include "Space.hpp"
#include "utilities.hpp"
#include <stdint.h>
const int H = 500;
const int W = 500;
Canvas<H, W> cv;
const int cz = 800;
const int POV = 70;
const float speed = 0.01;

extern "C" {
void render(int dt) {
  Vec3D<float> a{-200, 0, -100};
  Vec3D<float> b{0, -100, 200};
  Vec3D<float> c{300, 200, 0};

  Camera camera{{0, 0, cz}, POV, {0, 0, 0}};

  cv.fill(0xff000000);
  float angle = dt * speed;
  Vec3D<float> rotations = {0, angle, 0};
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
  Vec3D<float> aCam = camera.projectOnCameraCoord(a);
  Vec3D<float> bCam = camera.projectOnCameraCoord(b);
  Vec3D<float> cCam = camera.projectOnCameraCoord(c);

  Vec3D<float> aCv = camera.projectOnCanvas(a, cv);
  Vec3D<float> bCv = camera.projectOnCanvas(b, cv);
  Vec3D<float> cCv = camera.projectOnCanvas(c, cv);
  //   int x1_ = projectScreen(project(x1, cz + z1, 0), W, POV),
  //       y1_ = projectScreen(project(y1, cz + z1, 0), H, POV),
  //       x2_ = projectScreen(project(x2, cz + z2, 0), W, POV),
  //       y2_ = projectScreen(project(y2, cz + z2, 0), H, POV),
  //       x3_ = projectScreen(project(x3, cz + z3, 0), W, POV),
  //       y3_ = projectScreen(project(y3, cz + z3, 0), H, POV);
  auto plane = planeFromNormal(normal, a);
  LOG(a.x << ' ' << aCam.x << ' ' << aCv.x)
  LOG(b.x << ' ' << bCam.x << ' ' << bCv.x)
  cv.drawTriangle(aCv.x, aCv.y, bCv.x, bCv.y, cCv.x, cCv.y,
                  [h, s, v, plane](int x, int y) -> uint32_t {
                    float z = plane(x, y);
                    int vz = v;
                    vz = (z / 3) + 50;
                    LOG(z << ' ' << vz)
                    if (vz > 100)
                      vz = 100;
                    if (vz < 10)
                      vz = 10;
                    return HSV2RGB(h, s, vz) | 0xff000000;

                    // if (z)
                  });
}

int main() { render(4000); }
}

COMMON_EXPORTS(W, H)