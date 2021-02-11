//
//  transformation_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 04.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/transformation.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("transform", "[transformation]")
{
  SECTION("transform sequence")
  {
    auto point = sunray::create_point(1, 0, 1);
    auto a = sunray::Matrix44::rotation_x(sunray::PI / 2.0);
    auto b = sunray::Matrix44::scaling(5, 5, 5);
    auto c = sunray::Matrix44::translation(10, 5, 7);
    auto p1 = a * point;
    CHECK(p1 == sunray::create_point(1, -1, 0));
    auto p2 = b * p1;
    CHECK(p2 == sunray::create_point(5, -5, 0));
    auto p3 = c * p2;
    CHECK(p3 == sunray::create_point(15, 0, 7));
  }
  SECTION("transform pipeline")
  {
    sunray::Transformation trans;
    trans.rotate_x(sunray::PI / 2.0).scale(5, 5, 5).translate(10, 5, 7);
    auto point = sunray::create_point(1, 0, 1);
    auto t = trans.matrix();
    CHECK(t * point == sunray::create_point(15, 0, 7));
  }
  SECTION("transform single pipeline")
  {
    sunray::Transformation trans;
    trans.identity();
    auto point = sunray::create_point(1, 0, 1);
    auto t = trans.matrix();
    CHECK(t * point == sunray::create_point(1, 0, 1));
  }
  SECTION("transform y rotate")
  {
    sunray::Transformation trans;
    trans.rotate_y(sunray::PI / 4);
    auto point = sunray::create_point(0, 0, 1);
    auto t = trans.matrix();
    CHECK(t * point == sunray::create_point(sunray::sqrt(2) / 2.0, 0, sunray::sqrt(2) / 2.0));
  }
  SECTION("transform z rotate")
  {
    sunray::Transformation trans;
    trans.rotate_z(sunray::PI / 4);
    auto point = sunray::create_point(0, 1, 0);
    auto t = trans.matrix();
    CHECK(t * point == sunray::create_point(-sunray::sqrt(2) / 2.0, sunray::sqrt(2) / 2.0, 0));
  }
  SECTION("transform shear")
  {
    sunray::Transformation trans;
    trans.shear(1, 0, 0, 0, 0, 0);
    auto point = sunray::create_point(2, 3, 4);
    auto t = trans.matrix();
    CHECK(t * point == sunray::create_point(5, 3, 4));
  }
}

TEST_CASE("error", "[transformation]")
{
  SECTION("transform no matrix")
  {
    sunray::Transformation trans;
    CHECK_THROWS_AS(trans.matrix(), std::runtime_error);
  }
}

TEST_CASE("view transform", "[view transformation]")
{
  SECTION("view transform default orientation")
  {
    auto t =
      sunray::view_transformation(sunray::create_point(0, 0, 0), sunray::create_point(0, 0, -1), sunray::create_vector(0, 1, 0));
    CHECK(t == sunray::Transformation().identity().matrix());
  }
  SECTION("view transform looking in positive z direction")
  {
    auto t =
      sunray::view_transformation(sunray::create_point(0, 0, 0), sunray::create_point(0, 0, 1), sunray::create_vector(0, 1, 0));
    CHECK(t == sunray::Transformation().scale(-1, 1, -1).matrix());
  }
  SECTION("view transform moves the world")
  {
    auto t =
      sunray::view_transformation(sunray::create_point(0, 0, 8), sunray::create_point(0, 0, 0), sunray::create_vector(0, 1, 0));
    CHECK(t == sunray::Transformation().translate(0, 0, -8).matrix());
  }
  SECTION("arbitrary view transform")
  {
    auto t =
      sunray::view_transformation(sunray::create_point(1, 3, 2), sunray::create_point(4, -2, 8), sunray::create_vector(1, 1, 0));
    // clang-format off
    CHECK(t == sunray::Matrix44{{ -0.50709, 0.50709,  0.67612, -2.36643,
                                   0.76772, 0.60609,  0.12122, -2.82843,
                                  -0.35857, 0.59761, -0.71714,  0.00000,
                                   0.00000, 0.00000,  0.00000,  1.00000 }});
    // clang-format on
  }
}
