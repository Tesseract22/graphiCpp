#pragma once
#include "Math.hpp"
#ifdef DEBUG
#include <iostream>
#endif

template <typename T> struct Vec3 {
  T x;
  T y;
  T z;
#ifndef WASM_DEMO
  friend std::ostream &operator<<(std::ostream &stream, const Vec3 &v) {
    stream << v.x << ' ' << v.y << ' ' << v.z;
    return stream;
  }
#endif
  friend Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
  }
  friend Vec3 operator-(const Vec3 &v1) { return {-v1.x, -v1.y, -v1.z}; }
  friend Vec3 operator-(const Vec3 &v1, const Vec3 &v2) { return v1 + (-v2); }
  friend float operator*(const Vec3 &v1, const Vec3 &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
  }
  friend Vec3 operator*(const Vec3 &v, float scaler) {
    return {v.x * scaler, v.y * scaler, v.z * scaler};
  }
  friend Vec3 operator*(float scaler, const Vec3 &v) {
    return {v.x * scaler, v.y * scaler, v.z * scaler};
  }
  friend Vec3 operator/(const Vec3 &v, float scaler) {
    return {v.x / scaler, v.y / scaler, v.z / scaler};
  }
  static Vec3 crossProduct(const Vec3 &v1, const Vec3 &v2) {
    return {v1.y * v2.z - v1.z * v2.y, -(v1.x * v2.z - v1.z * v2.x),
            v1.x * v2.y - v1.y * v2.x};
  }
  static float innerProduct(const Vec3 &v1, const Vec3 &v2) { return v1 * v2; }
  static Vec3 scalerProduct(const Vec3<float> &v, float scaler) {
    return v * scaler;
  }
  static Vec3 rotateAll(const Vec3 &v, const Vec3<float> &rotations) {
    auto cp = v;
    gcmath::rotateFloat(cp.y, cp.z, rotations.x);
    gcmath::rotateFloat(cp.x, cp.z, rotations.y);
    gcmath::rotateFloat(cp.x, cp.y, rotations.z);
    return cp;
  }
  static auto planeFromNormal(const Vec3<float> &n, const Vec3<float> &p) {
    return [n, p](float x, float y) -> float {
      return (Vec3<float>::innerProduct(n, p) - n.x * x - n.y * y) / n.z;
    };
  }

  float length() { return sqrtp(x * x + y * y + z * z); }
};

using Vec3D = Vec3<float>;