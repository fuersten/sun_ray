//
//  math_helper.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 21.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <cmath>
#include <cstdint>


namespace sunray
{
  static constexpr double epsilon = 0.0001f;
  static constexpr double PI = 3.14159265358979323846;
  static constexpr double PIdiv180 = 0.017453292519943296;
  static constexpr double PIdiv180inv = 57.2957795130823229;
  static constexpr double PI2 = PI / 2.0;

  inline double degToRad(double angle)
  {
    return angle * PIdiv180;
  }

  inline double radToDeg(double angle)
  {
    return angle * PIdiv180inv;
  }

  inline double abs(double arg)
  {
    return ::fabs(arg);
  }
  inline double sqrt(double arg)
  {
    return ::sqrt(arg);
  }
  inline double sin(double arg)
  {
    return ::sin(arg);
  }
  inline double cos(double arg)
  {
    return ::cos(arg);
  }
  inline uint16_t round(float val)
  {
    return static_cast<uint16_t>(lround(val));
  }
  inline uint16_t round(double val)
  {
    return static_cast<uint16_t>(lround(val));
  }
  inline double floor(double val)
  {
    return ::floor(val);
  }

  template<int n>
  double pow(double v)
  {
    static_assert(n > 0, "Power can’t be negative");
    double n2 = pow<n / 2>(v);
    return n2 * n2 * pow<n & 1>(v);
  }
  template<>
  inline double pow<1>(double v)
  {
    return v;
  }
  template<>
  inline double pow<0>(double)
  {
    return 1;
  }


  class Approx
  {
  public:
    explicit Approx(float f)
    : d_{f}
    {
    }

    explicit Approx(double d)
    : d_{d}
    {
    }

    friend bool operator==(const Approx& approx, double d)
    {
      auto ff = fabs(approx.d_ - d);
      return ff < epsilon;
    }

    friend bool operator!=(const Approx& approx, double d)
    {
      return !(approx == d);
    }

    friend bool operator==(double d, const Approx& approx)
    {
      auto ff = fabs(d - approx.d_);
      return ff < epsilon;
    }

    friend bool operator!=(double d, const Approx& approx)
    {
      return !(d == approx);
    }

  private:
    double d_;
  };
}
