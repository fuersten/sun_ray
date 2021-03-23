//
//  color.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 23.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/math_helper.h>

#include <array>
#include <cstring>
#include <ostream>


namespace sunray
{
  class Color
  {
  public:
    static constexpr uint8_t color_depth = 3;
    using Vec = std::array<float, color_depth>;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    Color(float red, float green, float blue)
    : red_{red}
    , green_{green}
    , blue_{blue}
    {
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    Color(const Color& color)
    {
      memcpy(v_.data(), color.v_.data(), sizeof(Vec));
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    Color(Color&& color) noexcept
    : v_{color.v_}
    {
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    explicit Color(const Vec& v)
    {
      memcpy(v_.data(), v.data(), sizeof(Vec));
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
    explicit Color(Vec&& v) noexcept
    : v_{v}
    {
    }

    ~Color() = default;

    Color& operator=(const Color&) = delete;
    Color& operator=(Color&&) = default;

    float red() const
    {
      return red_;
    }

    float green() const
    {
      return green_;
    }

    float blue() const
    {
      return blue_;
    }

    bool operator==(const Color& rhs) const
    {
      return Approx(red_) == rhs.red_ && Approx(green_) == rhs.green_ && Approx(blue_) == rhs.blue_;
    }

    bool operator!=(const Color& rhs) const
    {
      return Approx(red_) != rhs.red_ || Approx(green_) != rhs.green_ || Approx(blue_) != rhs.blue_;
    }

    operator const float*() const
    {
      return v_.data();
    }

    friend Color operator+(const Color& lhs, const Color& rhs)
    {
      return Color{lhs.red_ + rhs.red_, lhs.green_ + rhs.green_, lhs.blue_ + rhs.blue_};
    }

    friend Color operator-(const Color& lhs, const Color& rhs)
    {
      return Color{lhs.red_ - rhs.red_, lhs.green_ - rhs.green_, lhs.blue_ - rhs.blue_};
    }

    friend Color operator*(const Color& lhs, const Color& rhs)
    {
      return Color{lhs.red_ * rhs.red_, lhs.green_ * rhs.green_, lhs.blue_ * rhs.blue_};
    }

    friend Color operator*(const Color& lhs, float scale)
    {
      return Color{lhs.red_ * scale, lhs.green_ * scale, lhs.blue_ * scale};
    }

    Color normalize() const
    {
      return Color{normalize_internal(red_), normalize_internal(green_), normalize_internal(blue_)};
    }

    friend std::ostream& operator<<(std::ostream& stream, const Color& color)
    {
      stream << "red: " << color[0] << " green: " << color[1] << " blue: " << color[2];
      return stream;
    }

  private:
    inline static float normalize_internal(float f)
    {
      if (f > 1.0f) {
        return 1.0f;
      }
      if (f < 0.0f) {
        return 0.0f;
      }
      return f;
    }

    union {
      struct {
        float red_;
        float green_;
        float blue_;
      };
      Vec v_;
    };
  };
}
