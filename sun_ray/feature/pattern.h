//
//  pattern.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 05.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/color.h>
#include <sun_ray/feature/matrix.h>
#include <sun_ray/feature/tuple.h>

#include <memory>
#include <ostream>
#include <sstream>


namespace sunray
{
  class Pattern;
  using PatternPtr = std::shared_ptr<const Pattern>;

  class Pattern
  {
  public:
    virtual ~Pattern() = default;

    Pattern(const Pattern&) = default;
    Pattern(Pattern&&) = default;
    Pattern& operator=(const Pattern&) = delete;
    Pattern& operator=(Pattern&&) = delete;

    inline const auto& inverse_transformation() const
    {
      return inverse_transformation_;
    }

    inline const auto& transformation() const
    {
      return transformation_;
    }

    virtual Color pattern_at(const Point& point) const = 0;

    friend std::ostream& operator<<(std::ostream& stream, const Pattern& pattern)
    {
      stream << pattern.to_string();
      return stream;
    }

    virtual std::string to_string() const = 0;

  protected:
    Pattern() = default;

    Pattern(Matrix44 transformation)
    : transformation_{transformation}
    , inverse_transformation_{transformation_.inverse()}
    {
    }

  private:
    Matrix44 transformation_{Matrix44::identity()};
    const Matrix44 inverse_transformation_{Matrix44::identity().inverse()};
  };
}
