#include "triangle.h"

#include "scene_base.h"

namespace {
bool onSameSide(V3 t0, V3 t1, V3 p0, V3 p1) {
  return ((p1-p0).cross(t0 - p0)).dot((p1-p0).cross(t1 - p0)) >= 0;
}
}

Triangle::Triangle(V3 p0, V3 p1, V3 p2, Material* material)
    : p0_(p0), p1_(p1), p2_(p2) {
  material_ = material;
}

Intersection Triangle::intersect(Ray ray) {
  Intersection result;
  result.happened_ = false;

  V3 u, v, normal;
  
  u = p1_ - p0_;
  v = p2_ - p0_;
  normal = u.cross(v);

  V3 p0_to_ray = ray.origin_coord_ - p0_;
  float r, a, b;
  a = - normal.dot(p0_to_ray);
  b = normal.dot(ray.direction_unit_vec_);
  if (abs(b) < 0.0001)
    return result;

  r = a / b;
  if (r < 0)
    return result;

  result.contact_coord_ = ray.origin_coord_ + ray.direction_unit_vec_ * r;

  V3 w = result.contact_coord_ - p0_;
  
  if (onSameSide(w, u, v, V3(0,0,0)) &&
      onSameSide(w, V3(0,0,0), u, v) &&
      onSameSide(w, v, V3(0,0,0), u)) {
    result.happened_ = true;
    result.normal_unit_vec_ = normal.unit();
  }

  return result;
}