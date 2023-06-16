#include "Canvas.hpp"
#include "Math.hpp"
#include "Space.hpp"
#include <stdint.h>
const int H = 500;
const int W = 500;
Canvas<H, W> cv;
const int cz = -400;
const int POV = 60;
const float speed = 0.01;
using gcmath::dist;
using gcmath::pow2;
extern "C" {
void render(int dt) {
  int r = 100;
  cv.fill(0xff000000);
  Vec3D pos{50, 50, 100 + dt * speed};
  Camera camera{{0, 0, cz}, POV, {0, 0, 0}};
  auto e = camera.projectSphereOnCanvas(pos, r, cv);
  cv.drawEllipse(
      e.x, e.y, e.major, e.minor,
      [e](int x, int y) {
        int d = pow2(e.major) - dist(x, y, (int)e.x, (int)e.y);
        int a = d * 200 / pow2(e.major) + 55;
        return (a << 24) | 0x00ff0000;
      },
      e.rot);
}

int main() { render(4000); }
}

COMMON_EXPORTS(W, H)