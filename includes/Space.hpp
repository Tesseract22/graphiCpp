#pragma once
#include "Canvas.hpp"
#include "utilities.hpp"
#ifdef DEBUG
#include <iostream>
#endif
template <typename T> struct Vec3D {
  T x;
  T y;
  T z;
#ifdef DEBUG
  friend std::ostream &operator<<(std::ostream &stream, const Vec3D &v) {
    stream << v.x << ' ' << v.y << ' ' << v.z;
    return stream;
  }
#endif
  friend Vec3D operator+(const Vec3D &v1, const Vec3D &v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
  }
  friend Vec3D operator-(const Vec3D &v1) { return {-v1.x, -v1.y, -v1.z}; }
  friend Vec3D operator-(const Vec3D &v1, const Vec3D &v2) {
    return v1 + (-v2);
  }
  friend float operator*(const Vec3D &v1, const Vec3D &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
  }
  friend Vec3D operator*(const Vec3D &v, float scaler) {
    return {v.x * scaler, v.y * scaler, v.z * scaler};
  }
  friend Vec3D operator*(float scaler, const Vec3D &v) {
    return {v.x * scaler, v.y * scaler, v.z * scaler};
  }
  static Vec3D crossProduct(const Vec3D &v1, const Vec3D &v2) {
    return {v1.y * v2.z - v1.z * v2.y, -(v1.x * v2.z - v1.z * v2.x),
            v1.x * v2.y - v1.y * v2.x};
  }
  static float innerProduct(const Vec3D &v1, const Vec3D &v2) {
    return v1 * v2;
  }
  static Vec3D scalerProduct(const Vec3D<float> &v, float scaler) {
    return v * scaler;
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
  Vec3D<float> projectOnCamera(const Vec3D<float> &v) {
    return Vec3D<float>::rotateAll(v - pos, rotations);
  }
  Vec3D<float> projectOnCameraView(const Vec3D<float> &v) {
    Vec3D<float> rel = projectOnCamera(v);
    return {rel.x / rel.z, rel.y / rel.z, 1};
  }
  template <int H, int W>
  Vec3D<float> projectOnCanvas(const Vec3D<float> &v, const Canvas<H, W> &cv) {
    Vec3D<float> cameraCoord = projectOnCameraView(v);

    static float wTanPov = W / (tanPov * 2);
    static float hTanPov = H / (tanPov * 2);
    LOG(tanPov)
    return {cameraCoord.x * wTanPov + W / 2.f,
            cameraCoord.y * hTanPov + H / 2.f, 1};
  }
  template <int H, int W>
  inline Vec3D<float> canvas2CameraView(int x, int y, const Canvas<H, W> &cv) {
    static float wTanPov = W / (tanPov * 2);
    static float hTanPov = H / (tanPov * 2);
    return {(x - W / 2.f) / wTanPov, (y - H / 2.f) / hTanPov, 1};
  }
  inline Vec3D<float> Camera2Absolute(const Vec3D<float> &v) {
    Vec3D<float>::rotateAll(v, -rotations) + pos;
  }

private:
  float tanPov = tanp(DEG2RAD(pov) / 2);
};