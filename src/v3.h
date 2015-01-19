#pragma once

class V3 {
 public:  
  V3(float x, float y, float z) : x_(x), y_(y), z_(z) {}
  V3() : x_(0), y_(0), z_(0) {}

  float x() const { return x_; }
  float y() const { return y_; }
  float z() const { return z_; }

  V3 operator+=(const V3& other);
  V3 operator-=(const V3& other);
  V3 operator+(const V3& other) const;
  V3 operator-(const V3& other) const;
  V3 operator-() const;
  V3 operator*(float k) const;
  V3 operator/(float k) const;

  float length() const;
  V3 unit() const;
  float dot(const V3& other) const;
  V3 cross(const V3& other) const;

 private:
  float x_, y_, z_;
};