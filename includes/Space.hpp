#pragma once
#include "Canvas.hpp"
#include "utilities.hpp"
template <typename T> struct Vec3D {
  T x;
  T y;
  T z;
  friend Vec3D operator+(const Vec3D &v1, const Vec3D &v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
  }
  friend Vec3D operator-(const Vec3D &v1) { return {-v1.x, -v1.y, -v1.z}; }
  friend Vec3D operator-(const Vec3D &v1, const Vec3D &v2) {
    return v1 + (-v2);
  }
  static Vec3D crossProduct(const Vec3D &v1, const Vec3D &v2) {
    return {v1.y * v2.z - v1.z * v2.y, -(v1.x * v2.z - v1.z * v2.x),
            v1.x * v2.y - v1.y * v2.x};
  }
  static float innerProduct(const Vec3D &v1, const Vec3D &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
  }
  static Vec3D rotateAll(const Vec3D &v, const Vec3D<float> &rotations) {
    auto cp = v;
    rotate(cp.y, cp.z, rotations.x);
    rotate(cp.x, cp.z, rotations.y);
    rotate(cp.x, cp.y, rotations.z);
    return cp;
  }
};

auto planeFromNormal(const Vec3D<float> &n, const Vec3D<float> &p) {
  return [n, p](float x, float y) -> float {
    return (Vec3D<float>::innerProduct(n, p) - n.x * x - n.y * y) / n.z;
  };
}

struct Camera {
  Camera(const Vec3D<float> &pos, float pov, Vec3D<float> rotations)
      : pos(pos), pov(pov), rotations(rotations) {}
  Vec3D<float> pos;
  float pov;
  Vec3D<float> rotations;

  Vec3D<float> projectOnCameraCoord(const Vec3D<float> &v) {
    Vec3D<float> rel = v - pos;
    rel = Vec3D<float>::rotateAll(rel, rotations);
    return {rel.x / rel.z, rel.y / rel.z, 1};
  }
  template <int H, int W>
  Vec3D<float> projectOnCanvas(const Vec3D<float> &v, const Canvas<H, W> &cv) {
    Vec3D<float> cameraCoord = projectOnCameraCoord(v);
    LOG(tanPov)
    return {cameraCoord.x * (W / (tanPov * 2)) + W / 2,
            cameraCoord.y * (H / (tanPov * 2)) + H / 2, 1};
  }

private:
  float tanPov = tanp(DEG2RAD(pov) / 2);
};