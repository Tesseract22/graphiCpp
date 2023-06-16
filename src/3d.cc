#include "Canvas.hpp"
#include "Math.hpp"
#include "Space.hpp"
#include <stdint.h>

const int H = 500;
const int W = 500;
Canvas<H, W> cv;
const int radius = 25;
const int ct = 5;
const float L = (H + W) / 2;
const float POV = 60;
const int cz = (H + W) / -2;
const float speed = 0.01;
using gcmath::dist;
using gcmath::pow2;
using gcmath::rotate;
extern "C" {

void render(int dt) {
  cv.fill(0xff000000);
  Camera camera{{0, 0, cz}, POV, {0, 0, 0}};
  for (int z = L / ct; z < L; z += L / ct) {
    for (int x = W / ct; x < W; x += W / ct) {
      //   LOG(projectX(x, z, W / 2))
      for (int y = H / ct; y < H; y += H / ct) {
        uint8_t r = x * 255 / W;
        uint8_t g = y * 255 / H;
        uint8_t b = z * 255 / L;
        uint32_t color = 0x7f000000 | (r) | (g << 8) | (b << 16);
        // LOG(x << ' ' << y << ' ' << z)
        // new_r / r =
        float angle = dt * speed;
        float xr = x - W / 2, yr = y - H / 2, zr = z - L / 2;
        rotate(xr, zr, angle);
        rotate(xr, yr, angle / 2);
        rotate(yr, zr, angle / 4);
        auto e = camera.projectSphereOnCanvas({xr, yr, zr}, radius, cv);
        cv.drawEllipse(
            e.x, e.y, e.major, e.minor,
            [color, e](int x, int y) {
              uint8_t total = 200;
              uint8_t base = 75;

              int d = pow2(e.major) - dist(x, y, (int)e.x, (int)e.y);
              int a = d * (total - base) / pow2(e.major) + base;
              return (a << 24) | (color & 0x00ffffff);
            },
            e.rot);
        // cv.drawCircle(
        //     projectScreen(project(xr, zr + cz, 0), W, POV),
        //     projectScreen(project(yr, zr + cz, 0), H, POV),
        //     radius / sqrtp((zr + cz) * (zr + cz) + (xr - 0) * (xr - W / 2)),
        //     [color](int x, int y) { return color; }, 3);
      }
    }
  }
}
int main() { render(10000); }
}

COMMON_EXPORTS(W, H)