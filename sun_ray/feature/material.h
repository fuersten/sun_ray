//
//  material.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 12.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <sun_ray/feature/color.h>
#include <sun_ray/feature/light.h>
#include <sun_ray/feature/pattern.h>
#include <sun_ray/feature/tuple.h>

#include <ostream>


namespace sunray
{
  class Material
  {
  public:
    Material() = default;

    Material(Color color, float ambient, float diffuse, float specular, float shininess, float reflective, float transparency,
             float refractive_index)
    : color_{std::move(color)}
    , ambient_{ambient}
    , diffuse_{diffuse}
    , specular_{specular}
    , shininess_{shininess}
    , reflective_{reflective}
    , transparency_{transparency}
    , refractive_index_{refractive_index}
    {
    }

    ~Material() = default;

    Material(const Material&) = default;
    Material(Material&&) = default;
    Material& operator=(const Material&) = delete;
    Material& operator=(Material&&) = delete;

    friend bool operator==(const Material& lhs, const Material& rhs)
    {
      return lhs.color() == rhs.color() && Approx(lhs.ambient()) == rhs.ambient() && Approx(lhs.diffuse()) == rhs.diffuse() &&
             Approx(lhs.specular()) == rhs.specular() && Approx(lhs.shininess()) == rhs.shininess() &&
             Approx(lhs.reflective()) == rhs.reflective() && Approx(lhs.transparency()) == rhs.transparency() &&
             Approx(lhs.refractive_index()) == rhs.refractive_index();
    }

    void pattern(PatternPtr pattern)
    {
      pattern_ = pattern;
    }

    inline PatternPtr pattern() const
    {
      return pattern_;
    }

    inline const Color& color() const
    {
      return color_;
    }

    inline float ambient() const
    {
      return ambient_;
    }

    inline float diffuse() const
    {
      return diffuse_;
    }

    inline float specular() const
    {
      return specular_;
    }

    inline float shininess() const
    {
      return shininess_;
    }

    inline float reflective() const
    {
      return reflective_;
    }

    inline float transparency() const
    {
      return transparency_;
    }

    inline float refractive_index() const
    {
      return refractive_index_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Material& material)
    {
      stream << "color: " << material.color() << " ambient: " << material.ambient() << " diffuse: " << material.diffuse()
             << " specular: " << material.specular() << " shininess: " << material.shininess()
             << " reflective: " << material.reflective() << " transparency: " << material.transparency()
             << " refractive_index: " << material.refractive_index();
      if (material.pattern_) {
        stream << "pattern: " << *material.pattern_;
      }
      return stream;
    }

  private:
    const Color color_{1, 1, 1};
    const float ambient_{0.1f};
    const float diffuse_{0.9f};
    const float specular_{0.9f};
    const float shininess_{200.0f};
    const float reflective_{0.0f};
    const float transparency_{0.0f};
    const float refractive_index_{1.0};
    PatternPtr pattern_;
  };
}
