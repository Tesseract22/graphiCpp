#include "Canvas.hpp"
#include "utilities.hpp"
#include <stdint.h>

const int H = 500;
const int W = 500;
Canvas<H, W> cv;

extern "C" {
int main() {
  cv.fill(0xff000000);
  const int radius = W * 10;
  const int ct = 5;
  const float L = 500;
  const float POV = 60;
  const int cz = 250;
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
        LOG(project(x, z, W / 2))
        cv.drawCircle(
            projectScreen(project(x, z + cz, W / 2), W, DEG2RAD(POV)),
            projectScreen(project(y, z + cz, H / 2), H, DEG2RAD(POV)),
            radius / (z + cz),
            {[](int x, int y, uint32_t color) { return color; }, color}, 3);
      }
    }
  }
}
}

COMMON_EXPORTS(W, H)