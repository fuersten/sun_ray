//
//  color.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 25.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/color.h>

#include <string>
#include <vector>


namespace sunray
{
  class Canvas
  {
  public:
    using Vec = std::vector<float>;

    Canvas(uint32_t width, uint32_t height)
    : width_{width}
    , height_{height}
    {
      pixels_.resize(height_ * width_ * 3, 0.0f);
    }

    Canvas(const Canvas& canvas)
    : width_{canvas.width_}
    , height_{canvas.height_}
    {
      pixels_.resize(height_ * width_ * 3, 0.0f);
      std::copy(canvas.pixels_.begin(), canvas.pixels_.end(), pixels_.begin());
    }

    Canvas(Canvas&& canvas) noexcept
    : width_{canvas.width_}
    , height_{canvas.height_}
    , pixels_{std::move(canvas.pixels_)}
    {
    }

    ~Canvas() = default;

    Canvas& operator=(const Canvas&) = delete;
    Canvas& operator=(Canvas&&) = delete;

    uint32_t height() const
    {
      return height_;
    }

    uint32_t width() const
    {
      return width_;
    }

    Color pixel_at(uint32_t x, uint32_t y) const
    {
      check_ranges(x, y);
      const auto offset = x + x + x + y * (width_ * 3);
      return Color{pixels_[offset], pixels_[offset + 1], pixels_[offset + 2]};
    }

    void pixel_at(uint32_t x, uint32_t y, const Color& color)
    {
      check_ranges(x, y);
      const auto offset = x + x + x + y * (width_ * 3);
      pixels_[offset] = color.red();
      pixels_[offset + 1] = color.green();
      pixels_[offset + 2] = color.blue();
    }

    friend std::ostream& operator<<(std::ostream& stream, const Canvas& canvas)
    {
      stream << "width: " << canvas.width_ << " height: " << canvas.height_;
      return stream;
    }

  private:
    void check_ranges(uint32_t x, uint32_t y) const
    {
      if (x > (width_ - 1)) {
        throw std::out_of_range{std::string("x is out of range (") + std::to_string(x) + ">" + std::to_string(width_)};
      }
      if (y > (height_ - 1)) {
        throw std::out_of_range{std::string("y is out of range (") + std::to_string(y) + ">" + std::to_string(height_)};
      }
    }

    uint32_t width_{0};
    uint32_t height_{0};
    Vec pixels_;
  };
}
