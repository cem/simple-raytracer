#pragma once

#include <cmath>
#include "v3.h"

struct Ray {
  Ray() {}
  Ray(V3 origin_coord, V3 direction_unit_vec)
      : origin_coord_(origin_coord), direction_unit_vec_(direction_unit_vec) {}
  
  Ray reflect(V3 axis_unit_vec);

  V3 origin_coord_;
  V3 direction_unit_vec_;
};

class Color {
 public:
  Color(float r, float g, float b)
      : r_(r), g_(g), b_(b) {}
  Color() : r_(0), g_(0), b_(0) {}

  float r() { return r_; }
  float g() { return g_; }
  float b() { return b_; }

  void clamp();
  void gammaCorrect();

  Color operator+=(const Color& other);
  Color operator+(const Color& other) const;
  Color operator*(float k) const;
  Color operator*(const Color& other) const;
  Color operator/(float k) const;

 private:
  float gammaEncode(float c);

  float r_, g_, b_;
};

struct Light {
  V3 origin_coord_;
  float intensity_;
  Color color_;
  float radius_;
};

struct Intersection {
  bool happened_;
  V3 contact_coord_;
  V3 normal_unit_vec_;
};

struct Material {
  float kAmbient, kDiffuse, kOcclusion, kReflection, kSpecular, shininess;
  Color cDiffuse, cReflection, cSpecular;
};

struct Camera {
  V3 origin_coord_;
  V3 plane_center_coord_;
  float plane_width_;
  float plane_height_;
};

class SceneObject {
 public:
  Material* material_;
  virtual Intersection intersect(Ray ray) = 0;
};