//
//  tuple.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 21.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/math_helper.h>

#include <array>
#include <cstring>
#include <ostream>


namespace sunray
{
  class Tuple
  {
  public:
    static constexpr uint8_t elements = 4;
    using Vec = std::array<double, elements>;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    Tuple()
    : x_{0.0}
    , y_{0.0}
    , z_{0.0}
    , w_{0.0}
    {
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    Tuple(double x, double y, double z, double w)
    : x_{x}
    , y_{y}
    , z_{z}
    , w_{w}
    {
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    Tuple(const Tuple& t)
    {
      memcpy(v_.data(), t.v_.data(), sizeof(Vec));
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    Tuple(Tuple&& t) noexcept
    : v_{t.v_}
    {
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    explicit Tuple(const Vec& v)
    {
      memcpy(v_.data(), v.data(), sizeof(Vec));
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    explicit Tuple(Vec&& v) noexcept
    : v_{v}
    {
    }

    ~Tuple() = default;

    double x() const
    {
      return x_;
    }

    double y() const
    {
      return y_;
    }

    double z() const
    {
      return z_;
    }

    double w() const
    {
      return w_;
    }

    Tuple& operator=(Tuple&& t) noexcept
    {
      v_ = t.v_;
      return *this;
    }

    Tuple& operator=(const Tuple& t)
    {
      v_ = t.v_;
      return *this;
    }

    bool operator==(const Tuple& rhs) const
    {
      return Approx(x_) == rhs.x_ && Approx(y_) == rhs.y_ && Approx(z_) == rhs.z_ && Approx(w_) == rhs.w_;
    }

    bool operator!=(const Tuple& rhs) const
    {
      return Approx(x_) != rhs.x_ || Approx(y_) != rhs.y_ || Approx(z_) != rhs.z_ || Approx(w_) != rhs.w_;
    }

    operator const double*() const
    {
      return v_.data();
    }

    friend Tuple operator+(const Tuple& lhs, const Tuple& rhs)
    {
      Tuple t;
      t.x_ = lhs.x_ + rhs.x_;
      t.y_ = lhs.y_ + rhs.y_;
      t.z_ = lhs.z_ + rhs.z_;
      t.w_ = lhs.w_ + rhs.w_;

      return t;
    }

    friend Tuple operator-(const Tuple& lhs, const Tuple& rhs)
    {
      Tuple t;
      t.x_ = lhs.x_ - rhs.x_;
      t.y_ = lhs.y_ - rhs.y_;
      t.z_ = lhs.z_ - rhs.z_;
      t.w_ = lhs.w_ - rhs.w_;

      return t;
    }

    friend Tuple operator*(const Tuple& lhs, double scale)
    {
      Tuple t;
      t.x_ = lhs.x_ * scale;
      t.y_ = lhs.y_ * scale;
      t.z_ = lhs.z_ * scale;
      t.w_ = lhs.w_ * scale;

      return t;
    }

    friend Tuple operator/(const Tuple& lhs, double scale)
    {
      if (Approx(scale) == 0.0) {
        throw std::invalid_argument{"devide by zero"};
      }
      Tuple t;
      t.x_ = lhs.x_ / scale;
      t.y_ = lhs.y_ / scale;
      t.z_ = lhs.z_ / scale;
      t.w_ = lhs.w_ / scale;

      return t;
    }

    double magnitude() const
    {
      return sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
    }

    Tuple normalize() const
    {
      Tuple t{*this};
      const auto mag = magnitude();
      if (mag != Approx(0.0)) {
        const auto m = (double{1.0} / mag);
        t.x_ = t.x_ * m;
        t.y_ = t.y_ * m;
        t.z_ = t.z_ * m;
        t.w_ = t.w_ * m;
      }
      return t;
    }

    double scalarProduct(const Tuple& rhs) const
    {
      return x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_ + w_ * rhs.w_;
    }

    Tuple crossProduct(const Tuple& rhs) const
    {
      Tuple t;
      t.x_ = (y_ * rhs.z_) - (z_ * rhs.y_);
      t.y_ = (z_ * rhs.x_) - (x_ * rhs.z_);
      t.z_ = (x_ * rhs.y_) - (y_ * rhs.x_);

      return t;
    }

    Tuple operator-() const
    {
      return Tuple(-x_, -y_, -z_, -w_);
    }

    Tuple negate() const
    {
      return Tuple(-x_, -y_, -z_, -w_);
    }

    Tuple reflect(const Tuple& normal) const
    {
      return *this - normal * 2 * scalarProduct(normal);
    }

    friend std::ostream& operator<<(std::ostream& stream, const Tuple& tuple)
    {
      stream << "x: " << tuple[0] << " y: " << tuple[1] << " z: " << tuple[2] << " w: " << tuple[3];
      return stream;
    }

  private:
    union {
      struct {
        double x_;
        double y_;
        double z_;
        double w_;
      };
      Vec v_;
    };
  };

  inline bool is_point(const Tuple& t)
  {
    return t[3] == Approx(1.0);
  }

  inline bool is_vector(const Tuple& t)
  {
    return t[3] == Approx(0.0);
  }


  using Vector = Tuple;
  using Point = Tuple;

  inline Vector create_vector(double x, double y, double z)
  {
    return Vector{x, y, z, 0.0};
  }

  inline Point create_point(double x, double y, double z)
  {
    return Point{x, y, z, 1.0};
  }

}
