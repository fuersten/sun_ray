//
//  ring_pattern.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 05.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/pattern.h>


namespace sunray
{
  class RingPattern : public Pattern
  {
  public:
    RingPattern(Color a, Color b)
    : a_{std::move(a)}
    , b_{std::move(b)}
    {
    }

    RingPattern(Color a, Color b, Matrix44 transformation)
    : Pattern{transformation}
    , a_{std::move(a)}
    , b_{std::move(b)}
    {
    }

    Color pattern_at(const Point& point) const override
    {
      auto square = pow<2>(point.x()) + pow<2>(point.z());
      if (static_cast<int>(floor(sqrt(square))) % 2 == 0) {
        return a_;
      }
      return b_;
    }

    std::string to_string() const override
    {
      std::stringstream ss;
      ss << "ring color a: " << a_ << " color b: " << b_;
      return ss.str();
    }

  private:
    Color a_;
    Color b_;
  };
}
