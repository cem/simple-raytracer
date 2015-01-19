#include "sphere.h"

Sphere::Sphere(V3 origin_coord, float radius, Material* material)
    : origin_coord_(origin_coord), radius_(radius) {
  material_ = material;
}

Intersection Sphere::intersect(Ray ray) {
  // Solve a quadratic equation
  float a, b, c, D;
  a = ray.direction_unit_vec_.dot(ray.direction_unit_vec_);
  b = ray.direction_unit_vec_.dot((ray.origin_coord_ - origin_coord_) * 2);
  c = origin_coord_.dot(origin_coord_) + ray.origin_coord_.dot(ray.origin_coord_) - ray.origin_coord_.dot(origin_coord_) * 2 - radius_ * radius_;
  D = b * b - a * c * 4;

  Intersection intersection;
  intersection.happened_ = false;
  if (D >= 0) {
    float sqrt_D = sqrt(D);
    float t = (-0.5) * (b + sqrt_D) / a;
    if (t > 0) {
      float distance = sqrt(a) * t;
      V3 hitpoint = ray.origin_coord_ + ray.direction_unit_vec_ * t;
      V3 normal = (hitpoint - origin_coord_) / radius_;

      intersection.happened_ = true;
      intersection.contact_coord_ = hitpoint;
      intersection.normal_unit_vec_ = normal;
    }
  }

  return intersection;
}