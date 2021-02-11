//
//  color_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 23.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/color.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("create colors", "[color]")
{
  SECTION("create color")
  {
    sunray::Color c{-0.5f, 0.4f, 1.7f};

    CHECK(c.red() == Approx(-0.5));
    CHECK(c.green() == Approx(0.4));
    CHECK(c.blue() == Approx(1.7));
  }
  SECTION("create color from color")
  {
    sunray::Color c{-0.5f, 0.4f, 1.7f};
    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    sunray::Color c1{c};

    CHECK(c1.red() == Approx(-0.5));
    CHECK(c1.green() == Approx(0.4));
    CHECK(c1.blue() == Approx(1.7));
  }
  SECTION("create color from array")
  {
    sunray::Color::Vec v{{4.3f, -4.2f, 3.1f}};
    sunray::Color c{v};

    CHECK(c.red() == Approx(4.3));
    CHECK(c.green() == Approx(-4.2));
    CHECK(c.blue() == Approx(3.1));
  }
  SECTION("create color from array move")
  {
    sunray::Color c{{4.3f, -4.2f, 3.1f}};

    CHECK(c.red() == Approx(4.3));
    CHECK(c.green() == Approx(-4.2));
    CHECK(c.blue() == Approx(3.1));
  }
}

TEST_CASE("stream colors", "[color]")
{
  SECTION("stream")
  {
    sunray::Color c{4.3f, -4.2f, 3.1f};
    std::stringstream ss;
    ss << c;
    CHECK("red: 4.3 green: -4.2 blue: 3.1" == ss.str());
  }
}

TEST_CASE("color components", "[color]")
{
  SECTION("stream")
  {
    sunray::Color c{4.3f, -4.2f, 3.1f};
    CHECK(c.red() == Approx(4.3));
    CHECK(c.green() == Approx(-4.2));
    CHECK(c.blue() == Approx(3.1));
  }
}

TEST_CASE("compare colors", "[color]")
{
  sunray::Color c1{4.3f, -4.2f, 3.1f};
  sunray::Color c2{4.3f, -4.2f, 3.1f};
  sunray::Color c3{-4.3f, 4.2f, -3.1f};

  SECTION("compare equal")
  {
    CHECK(c1 == c1);
    CHECK(c1 == c2);
  }
  SECTION("compare not equal")
  {
    CHECK(c1 != c3);
    CHECK(c2 != c3);
  }
}

TEST_CASE("copy colors", "[color]")
{
  SECTION("copy")
  {
    sunray::Color c1{4.3f, -4.2f, 3.1f};
    sunray::Color c2{c1};
    CHECK(c1 == c2);
    sunray::Color c3{std::move(c2)};
    CHECK(c1 == c3);
  }
}

TEST_CASE("add colors", "[color]")
{
  SECTION("add color")
  {
    sunray::Color c1(0.9f, 0.6f, 0.75f);
    sunray::Color c2(0.7f, 0.1f, 0.25f);
    CHECK(c1 + c2 == sunray::Color(1.6f, 0.7f, 1.0f));
  }
}

TEST_CASE("subtract colors", "[color]")
{
  SECTION("subtract color")
  {
    sunray::Color c1(0.9f, 0.6f, 0.75f);
    sunray::Color c2(0.7f, 0.1f, 0.25f);
    CHECK(c1 - c2 == sunray::Color(0.2f, 0.5f, 0.5f));
  }
}

TEST_CASE("mulitply colors", "[color]")
{
  SECTION("mulitply color by scalar")
  {
    sunray::Color c(0.2f, 0.3f, 0.4f);

    CHECK(c * 2 == sunray::Color(0.4f, 0.6f, 0.8f));
  }
  SECTION("mulitply color or hadamard product")
  {
    sunray::Color c1(1, 0.2f, 0.4f);
    sunray::Color c2(0.9f, 1, 0.1f);
    CHECK(c1 * c2 == sunray::Color(0.9f, 0.2f, 0.04f));
  }
}

TEST_CASE("normalize colors", "[color]")
{
  SECTION("normalize color")
  {
    CHECK(sunray::Color(0.2f, 0, 0.5f) == sunray::Color(0.2f, -0.5f, 0.5f).normalize());
    CHECK(sunray::Color(1.0f, 0, 1.0f) == sunray::Color(1.2f, -1.5f, 1.5f).normalize());
  }
}
