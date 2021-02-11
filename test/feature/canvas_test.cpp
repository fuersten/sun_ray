//
//  tuple_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 25.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/canvas.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("create canvas", "[canvas]")
{
  SECTION("create canvas")
  {
    sunray::Canvas canvas{10, 20};

    CHECK(10 == canvas.width());
    CHECK(20 == canvas.height());

    const auto black = sunray::Color{0, 0, 0};
    auto only_black{true};
    for (uint32_t y = 0; y < 20; ++y) {
      for (uint32_t x = 0; x < 10; ++x) {
        if (canvas.pixel_at(x, y) != black) {
          only_black = false;
        }
      }
    }
    CHECK(only_black == true);
  }
}

TEST_CASE("stream canvas", "[canvas]")
{
  SECTION("stream canvas")
  {
    sunray::Canvas canvas{10, 20};
    std::stringstream ss;
    ss << canvas;
    CHECK("width: 10 height: 20" == ss.str());
  }
}

TEST_CASE("copy canvas", "[canvas]")
{
  SECTION("copy canvas")
  {
    sunray::Canvas canvas{10, 20};
    const auto black = sunray::Color{0, 0, 0};
    const auto red = sunray::Color{1.0f, 0, 0};

    CHECK(10 == canvas.width());
    CHECK(20 == canvas.height());
    canvas.pixel_at(0, 0, red);
    canvas.pixel_at(0, 10, red);
    canvas.pixel_at(9, 0, red);
    canvas.pixel_at(9, 19, red);

    sunray::Canvas canvas2{canvas};
    CHECK(10 == canvas2.width());
    CHECK(20 == canvas2.height());
    CHECK(canvas2.pixel_at(0, 0) == red);
    CHECK(canvas2.pixel_at(0, 10) == red);
    CHECK(canvas2.pixel_at(9, 0) == red);
    CHECK(canvas2.pixel_at(9, 19) == red);
    CHECK(canvas2.pixel_at(0, 1) == black);
    CHECK(canvas2.pixel_at(1, 10) == black);
    CHECK(canvas2.pixel_at(9, 1) == black);
    CHECK(canvas2.pixel_at(8, 19) == black);
  }
  SECTION("move canvas")
  {
    sunray::Canvas canvas{10, 20};
    const auto black = sunray::Color{0, 0, 0};
    const auto red = sunray::Color{1.0f, 0, 0};

    CHECK(10 == canvas.width());
    CHECK(20 == canvas.height());
    canvas.pixel_at(0, 0, red);
    canvas.pixel_at(0, 10, red);
    canvas.pixel_at(9, 0, red);
    canvas.pixel_at(9, 19, red);

    sunray::Canvas canvas2{std::move(canvas)};
    CHECK(10 == canvas2.width());
    CHECK(20 == canvas2.height());
    CHECK(canvas2.pixel_at(0, 0) == red);
    CHECK(canvas2.pixel_at(0, 10) == red);
    CHECK(canvas2.pixel_at(9, 0) == red);
    CHECK(canvas2.pixel_at(9, 19) == red);
    CHECK(canvas2.pixel_at(0, 1) == black);
    CHECK(canvas2.pixel_at(1, 10) == black);
    CHECK(canvas2.pixel_at(9, 1) == black);
    CHECK(canvas2.pixel_at(8, 19) == black);
  }
}

TEST_CASE("canvas pixels", "[canvas]")
{
  SECTION("canvas read pixel out of range")
  {
    sunray::Canvas canvas{10, 20};

    CHECK_THROWS_AS(canvas.pixel_at(10, 20), std::out_of_range);
    CHECK_THROWS_AS(canvas.pixel_at(9, 20), std::out_of_range);
    CHECK_THROWS_AS(canvas.pixel_at(11, 20), std::out_of_range);
    CHECK_THROWS_AS(canvas.pixel_at(9, 21), std::out_of_range);
  }
  SECTION("canvas write pixel out of range")
  {
    sunray::Canvas canvas{10, 20};

    const auto red = sunray::Color{1.0f, 0, 0};
    CHECK_THROWS_AS(canvas.pixel_at(10, 20, red), std::out_of_range);
    CHECK_THROWS_AS(canvas.pixel_at(9, 20, red), std::out_of_range);
    CHECK_THROWS_AS(canvas.pixel_at(11, 20, red), std::out_of_range);
    CHECK_THROWS_AS(canvas.pixel_at(9, 21, red), std::out_of_range);
  }
  SECTION("canvas write/read pixel")
  {
    sunray::Canvas canvas{10, 20};

    const auto black = sunray::Color{0, 0, 0};
    const auto red = sunray::Color{1.0f, 0, 0};
    const auto green = sunray::Color{0, 1.0f, 0};
    const auto blue = sunray::Color{0, 0, 1.0f};
    const auto orange = sunray::Color{1.0f, 0.647f, 0};

    canvas.pixel_at(0, 0, red);
    canvas.pixel_at(0, 10, orange);
    canvas.pixel_at(9, 0, green);
    canvas.pixel_at(9, 19, blue);

    for (uint32_t y = 0; y < 20; ++y) {
      for (uint32_t x = 0; x < 10; ++x) {
        if (x == 0 && y == 0) {
          CHECK(canvas.pixel_at(x, y) == red);
        } else if (x == 0 && y == 10) {
          CHECK(canvas.pixel_at(x, y) == orange);
        } else if (x == 9 && y == 0) {
          CHECK(canvas.pixel_at(x, y) == green);
        } else if (x == 9 && y == 19) {
          CHECK(canvas.pixel_at(x, y) == blue);
        } else {
          if (canvas.pixel_at(x, y) != black) {
            CHECK_FALSE(true);
          }
        }
      }
    }
  }
}
