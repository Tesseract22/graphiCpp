#include "Canvas.hpp"
#include "utilities.hpp"
#include <stdint.h>

const int H = 500;
const int W = 500;
Canvas<H, W> cv;
const int radius = (H + W) * 10;
const int ct = 5;
const float L = (H + W) / 2;
const float POV = 60;
const int cz = (H + W) / 2;
const float speed = 0.01;
extern "C" {

void render(int dt) {
  cv.fill(0xff000000);

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
        int xr = x - W / 2, yr = y - H / 2, zr = z - L / 2;
        rotate(xr, zr, angle);
        rotate(xr, yr, angle / 2);
        rotate(yr, zr, angle / 4);
        float dd = (zr + cz) * (zr + cz);
        float h = sqrtp(dd - radius * radius) * radius / sqrtp(dd);
        float zplane = sqrtp(dd - radius * radius) * h / r;

        cv.drawCircle(
            projectScreen(project(xr, zr + cz, 0), W, POV),
            projectScreen(project(yr, zr + cz, 0), H, POV),
            radius / sqrtp((zr + cz) * (zr + cz) + (xr - 0) * (xr - W / 2)),
            [color](int x, int y) { return color; }, 3);
      }
    }
  }
}
int main() { render(10000); }
}

COMMON_EXPORTS(W, H)