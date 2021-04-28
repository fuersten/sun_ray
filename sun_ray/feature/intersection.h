//
//  intersection.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 05.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/math_helper.h>

#include <algorithm>
#include <memory>
#include <vector>


namespace sunray
{
  class Shape;

  class Intersection
  {
  public:
    Intersection(double t, const Shape* object)
    : t_{t}
    , object_{object}
    {
    }

    ~Intersection() = default;

    Intersection(const Intersection&) = default;
    Intersection(Intersection&&) = default;

    Intersection& operator=(const Intersection&) = default;
    Intersection& operator=(Intersection&&) = default;

    friend bool operator==(const Intersection& lhs, const Intersection& rhs)
    {
      return Approx(lhs.t_) == rhs.t_ && lhs.object_ == rhs.object_;
    }

    bool operator<(const Intersection& other) const
    {
      return t_ < other.t_;
    }

    inline double time() const
    {
      return t_;
    }

    inline const Shape* object() const
    {
      return object_;
    }

  private:
    double t_;
    const Shape* object_;
  };


  class Intersections
  {
  public:
    Intersections()
    {
      intersections_.reserve(20);
    }

    ~Intersections() = default;

    Intersections(const Intersections&) = delete;
    Intersections(Intersections&&) = default;
    Intersections& operator=(const Intersections&) = delete;
    Intersections& operator=(Intersections&&) = delete;

    void clear()
    {
      intersections_.clear();
    }

    void add(Intersection&& intersection)
    {
      intersections_.emplace_back(std::move(intersection));
    }

    const Intersection* hit() const
    {
      std::sort(intersections_.begin(), intersections_.end());
      auto it = std::find_if(intersections_.begin(), intersections_.end(), [](const auto& intersection) {
        return intersection.time() > 0;
      });
      if (it != intersections_.end()) {
        return &(*it);
      }
      return nullptr;
    }

    const std::vector<Intersection>& intersections() const
    {
      std::sort(intersections_.begin(), intersections_.end());
      return intersections_;
    }

  private:
    mutable std::vector<Intersection> intersections_;
  };
}
