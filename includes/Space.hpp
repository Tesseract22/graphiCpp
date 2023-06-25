#pragma once
#include "Canvas.hpp"
#include "Math.hpp"
#include "Vec3D.h"

#ifdef DEBUG
#include <iostream>
#endif
using gcmath::DEG2RAD;
using gcmath::pi;
struct Camera {
  Camera(const Vec3D &pos, float pov, Vec3D rotations)
      : pos(pos), pov(pov), rotations(rotations) {}
  Vec3D pos;
  float pov;
  Vec3D rotations;
  Vec3D projectOnCamera(const Vec3D &v) {
    return Vec3D::rotateAll(v - pos, rotations);
  }
  Vec3D projectOnCameraView(const Vec3D &v) {
    Vec3D rel = projectOnCamera(v);
    return {rel.x / rel.z, rel.y / rel.z, 1};
  }
  template <int H, int W>
  Vec3D projectOnCanvas(const Vec3D &v, const Canvas<H, W> &cv) {
    Vec3D cameraView = projectOnCameraView(v);

    static float wTanPov = W / (tanPov * 2);
    static float hTanPov = H / (tanPov * 2);
    return {cameraView.x * wTanPov + W / 2.f, cameraView.y * hTanPov + H / 2.f,
            1};
  }
  template <int H, int W>
  inline Vec3D canvas2CameraView(int x, int y, const Canvas<H, W> &cv) const {
    static float wTanPov = W / (tanPov * 2);
    static float hTanPov = H / (tanPov * 2);
    return {(x - W / 2.f) / wTanPov, (y - H / 2.f) / hTanPov, 1};
  }
  template <int H, int W>
  inline Vec3D cameraView2Canvas(Vec3D v, const Canvas<H, W> &cv) const {
    static float wTanPov = W / (tanPov * 2);
    static float hTanPov = H / (tanPov * 2);
    return {v.x * wTanPov + W / 2.f, v.y * hTanPov + H / 2.f, 1};
  }
  inline Vec3D camera2Absolute(const Vec3D &v) const {
    return Vec3D::rotateAll(v, -rotations) + pos;
  }

  template <int H, int W>
  typename Canvas<H, W>::Ellipse projectSphereOnCanvas(const Vec3D absPos,
                                                       float radius,
                                                       const Canvas<H, W> &cv) {
    Vec3D posCamView = projectOnCameraView(absPos);
    Vec3D posCam = projectOnCamera(absPos);
    Vec3D viewNorm = {0, 0, 1};
    Vec3D l = {posCamView.x, posCamView.y, 0};
    float posCamViewLen = posCamView.length();
    float sinAlpha =
        Vec3D::innerProduct(posCamView, viewNorm) / (posCamViewLen * 1);
    float alpha = asinp(sinAlpha);
    float sinTheta =
        sqrtp(gcmath::pow2(radius) / (posCam * posCam + gcmath::pow2(radius)));
    float theta = asinp(sinTheta);
    float beta1 = pi - theta - alpha;
    float sinBeta1 = sinp(beta1);

    float d1 = (posCamView.length() * sinTheta / sinBeta1) / l.length();

    float beta2 = pi - theta - (pi - alpha);
    float sinBeta2 = sinp(beta2);
    float d2 = (posCamView.length() * sinTheta / sinBeta2) / l.length();
    Vec3D ca1 = cameraView2Canvas(posCamView + d1 * l, cv);
    Vec3D ca2 = cameraView2Canvas(posCamView - d2 * l, cv);
    Vec3D test = cameraView2Canvas(posCamView, cv);
    Vec3D center = (ca1 + ca2) / 2;
    float major = (ca1 - ca2).length() / 2;
    float minor = sqrtp(gcmath::pow2(major) -
                        gcmath::pow2(cosp(beta2) / cosp(theta) * major));
    float tmp = l * Vec3D{1, 0, 0} / l.length();
    float rot = gcmath::RAD2DEG(acosp(tmp));

    return {center.x, center.y, major, minor, rot};
  }

private:
  float tanPov = tanp(DEG2RAD(pov) / 2);
};
