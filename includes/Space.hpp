#pragma once

struct Pos3D {
  int x;
  int y;
  int z;
};

struct Camera {
  Pos3D pos;
  float pov;
  // angle?
};