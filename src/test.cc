
#include "utilities.hpp"

int main() {
  int h;
  float s, v;
  h = 30;
  s = 100;
  v = 100;
  LOG(std::hex << HSV2RGB(h, s, v));
}