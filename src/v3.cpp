#include <cmath>
#include "v3.h"

V3 V3::operator+=(const V3& other) {
  x_ += other.x_;
  y_ += other.y_;
  z_ += other.z_;
  return *this;
}

V3 V3::operator-=(const V3& other) {
  x_ -= other.x_;
  y_ -= other.y_;
  z_ -= other.z_;
  return *this;
}
  
V3 V3::operator+(const V3& other) const {
  return V3(*this) += other;
}

V3 V3::operator-(const V3& other) const {
  return V3(*this) -= other;
}

V3 V3::operator-() const {
  return V3(-x_, -y_, -z_);
}

V3 V3::operator*(float k) const {
  return V3(x_ * k, y_ * k, z_ * k);
}

V3 V3::operator/(float k) const {
  return V3(x_ / k, y_ / k, z_ / k);
}

float V3::length() const {
  return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

V3 V3::unit() const {
  float len = length();
  return V3(x_ / len, y_ / len, z_ / len);
}

float V3::dot(const V3& other) const {
  return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}

V3 V3::cross(const V3& other) const {
  return V3(y_ * other.z_ - z_ * other.y_,
            z_ * other.x_ - x_ * other.z_,
            x_ * other.y_ - y_ * other.x_);
}