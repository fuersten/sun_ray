//
//  pattern_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 05.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/checker_pattern.h>
#include <sun_ray/feature/gradient_pattern.h>
#include <sun_ray/feature/ring_pattern.h>
#include <sun_ray/feature/sphere.h>
#include <sun_ray/feature/stripe_pattern.h>

#include <catch2/catch.hpp>

namespace
{
  sunray::Color black{0, 0, 0};
  sunray::Color white{1, 1, 1};

  class TestPattern : public sunray::Pattern
  {
  public:
    TestPattern()
    {
    }

    TestPattern(sunray::Matrix44 transformation)
    : Pattern{transformation}
    {
    }

    sunray::Color pattern_at(const sunray::Point& point) const override
    {
      return sunray::Color{static_cast<float>(point.x()), static_cast<float>(point.y()), static_cast<float>(point.z())};
    }

    std::string to_string() const override
    {
      std::stringstream ss;
      return ss.str();
    }
  };

}

TEST_CASE("create pattern", "[pattern]")
{
  SECTION("default pattern")
  {
    TestPattern pattern;
    CHECK(pattern.transformation() == sunray::Matrix44::identity());
    CHECK(pattern.inverse_transformation() == sunray::Matrix44::identity().inverse());
  }
  SECTION("pattern with transformation")
  {
    TestPattern pattern{sunray::Matrix44::translation(1, 2, 3)};
    CHECK(pattern.transformation() == sunray::Matrix44::translation(1, 2, 3));
    CHECK(pattern.inverse_transformation() == sunray::Matrix44::translation(1, 2, 3).inverse());
  }
}

TEST_CASE("striped pattern", "[pattern]")
{
  sunray::StripePattern pattern(white, black);

  SECTION("striped pattern constant in y")
  {
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 0)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(0, 1, 0)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(0, 2, 0)) == white);
  }
  SECTION("striped pattern constant in z")
  {
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 0)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 1)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 2)) == white);
  }
  SECTION("striped pattern constant in x")
  {
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 0)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(0.9, 0, 0)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(1, 0, 0)) == black);
    CHECK(pattern.pattern_at(sunray::create_point(-0.1, 0, 0)) == black);
    CHECK(pattern.pattern_at(sunray::create_point(-1, 0, 0)) == black);
    CHECK(pattern.pattern_at(sunray::create_point(-1.1, 0, 0)) == white);
  }
}

TEST_CASE("stream striped pattern", "[pattern]")
{
  sunray::StripePattern pattern(white, black);

  SECTION("stream")
  {
    std::stringstream ss;
    ss << pattern;
    CHECK(ss.str() == "stripe color a: red: 1 green: 1 blue: 1 color b: red: 0 green: 0 blue: 0");
  }
}

TEST_CASE("gradient pattern", "[pattern]")
{
  sunray::GradientPattern pattern(white, black);

  SECTION("gradient pattern interpolated between colors")
  {
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 0)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(0.25, 0, 0)) == sunray::Color{0.75, 0.75, 0.75});
    CHECK(pattern.pattern_at(sunray::create_point(0.5, 0, 0)) == sunray::Color{0.5, 0.5, 0.5});
    CHECK(pattern.pattern_at(sunray::create_point(0.75, 0, 0)) == sunray::Color{0.25, 0.25, 0.25});
  }
}

TEST_CASE("stream gradient pattern", "[pattern]")
{
  sunray::GradientPattern pattern(white, black);

  SECTION("stream")
  {
    std::stringstream ss;
    ss << pattern;
    CHECK(ss.str() == "gradient color a: red: 1 green: 1 blue: 1 color b: red: 0 green: 0 blue: 0");
  }
}

TEST_CASE("ring pattern", "[pattern]")
{
  sunray::RingPattern pattern(white, black);

  SECTION("ring pattern interpolated between colors")
  {
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 0)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(1, 0, 0)) == black);
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 1)) == black);
    CHECK(pattern.pattern_at(sunray::create_point(0.708, 0, 0.708)) == black);
  }
}

TEST_CASE("stream ring pattern", "[pattern]")
{
  sunray::RingPattern pattern(white, black);

  SECTION("stream")
  {
    std::stringstream ss;
    ss << pattern;
    CHECK(ss.str() == "ring color a: red: 1 green: 1 blue: 1 color b: red: 0 green: 0 blue: 0");
  }
}

TEST_CASE("checker pattern", "[pattern]")
{
  sunray::CheckerPattern pattern(white, black);

  SECTION("checker pattern repeats in x")
  {
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 0)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(0.99, 0, 0)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(1.01, 0, 0)) == black);
  }
  SECTION("checker pattern repeats in y")
  {
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 0)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(0, 0.99, 0)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(0, 1.01, 0)) == black);
  }
  SECTION("checker pattern repeats in z")
  {
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 0)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 0.99)) == white);
    CHECK(pattern.pattern_at(sunray::create_point(0, 0, 1.01)) == black);
  }
}

TEST_CASE("stream checker pattern", "[pattern]")
{
  sunray::CheckerPattern pattern(white, black);

  SECTION("stream")
  {
    std::stringstream ss;
    ss << pattern;
    CHECK(ss.str() == "checker color a: red: 1 green: 1 blue: 1 color b: red: 0 green: 0 blue: 0");
  }
}

TEST_CASE("apply stripe pattern", "[pattern]")
{
  SECTION("stripes with an object transformation")
  {
    auto sphere = sunray::Sphere::make_sphere(sunray::Matrix44::scaling(2, 2, 2));
    sunray::StripePattern pattern{white, black};
    CHECK(sunray::apply_pattern(pattern, *sphere, sunray::create_point(1.5, 0, 0)) == white);
  }
  SECTION("stripes with a pattern transformation")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::StripePattern pattern{white, black, sunray::Matrix44::scaling(2, 2, 2)};
    CHECK(sunray::apply_pattern(pattern, *sphere, sunray::create_point(1.5, 0, 0)) == white);
  }
  SECTION("stripes with a pattern and object transformation")
  {
    auto sphere = sunray::Sphere::make_sphere(sunray::Matrix44::scaling(2, 2, 2));
    sunray::StripePattern pattern{white, black, sunray::Matrix44::translation(0.5, 0, 0)};
    CHECK(sunray::apply_pattern(pattern, *sphere, sunray::create_point(2.5, 0, 0)) == white);
  }
}

TEST_CASE("apply pattern", "[pattern]")
{
  SECTION("pattern with an object transformation")
  {
    auto sphere = sunray::Sphere::make_sphere(sunray::Matrix44::scaling(2, 2, 2));
    CHECK(sunray::apply_pattern(TestPattern{}, *sphere, sunray::create_point(2, 3, 4)) == sunray::Color{1, 1.5, 2});
  }
  SECTION("pattern with a pattern transformation")
  {
    auto sphere = sunray::Sphere::make_sphere();
    CHECK(sunray::apply_pattern(TestPattern{sunray::Matrix44::scaling(2, 2, 2)}, *sphere, sunray::create_point(2, 3, 4)) ==
          sunray::Color{1, 1.5, 2});
  }
  SECTION("pattern with a pattern and object transformation")
  {
    auto sphere = sunray::Sphere::make_sphere(sunray::Matrix44::scaling(2, 2, 2));
    CHECK(sunray::apply_pattern(TestPattern{sunray::Matrix44::translation(0.5, 1, 1.5)}, *sphere,
                                sunray::create_point(2.5, 3, 3.5)) == sunray::Color{0.75, 0.5, 0.25});
  }
}
