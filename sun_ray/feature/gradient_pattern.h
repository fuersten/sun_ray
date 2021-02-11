//
//  gradient_pattern.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 05.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/pattern.h>


namespace sunray
{
  class GradientPattern : public Pattern
  {
  public:
    GradientPattern(Color a, Color b)
    : a_{std::move(a)}
    , b_{std::move(b)}
    {
    }

    GradientPattern(Color a, Color b, Matrix44 transformation)
    : Pattern{transformation}
    , a_{std::move(a)}
    , b_{std::move(b)}
    {
    }

    Color pattern_at(const Point& point) const override
    {
      return a_ + distance_ * static_cast<float>(point.x() - floor(point.x()));
    }

    std::string to_string() const override
    {
      std::stringstream ss;
      ss << "gradient color a: " << a_ << " color b: " << b_;
      return ss.str();
    }

  private:
    Color a_;
    Color b_;
    Color distance_{b_ - a_};
  };
}
