//
//  light.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 12.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/color.h>
#include <sun_ray/feature/tuple.h>

#include <memory>
#include <ostream>


namespace sunray
{
  class PointLight;
  using LightPtr = std::shared_ptr<const PointLight>;

  class PointLight
  {
  public:
    PointLight(Point position, Color intensity)
    : position_{std::move(position)}
    , intensity_{std::move(intensity)}
    {
    }

    ~PointLight() = default;

    PointLight(const PointLight&) = default;
    PointLight(PointLight&&) = delete;
    PointLight& operator=(const PointLight&) = delete;
    PointLight& operator=(PointLight&&) = delete;

    inline const Point& position() const
    {
      return position_;
    }

    inline const Color& intensity() const
    {
      return intensity_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const PointLight& light)
    {
      stream << "position: " << light.position() << " intensity: " << light.intensity();
      return stream;
    }

  private:
    Point position_;
    Color intensity_;
  };
}
