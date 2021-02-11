//
//  triangle_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 09.07.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/transformation.h>
#include <sun_ray/feature/triangle.h>

#include <catch2/catch.hpp>


TEST_CASE("create triangles", "[triangle]")
{
  sunray::Material default_material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f};

  SECTION("create triangle")
  {
    auto triangle = sunray::Triangle::make_triangle(sunray::create_point(0, 1, 0), sunray::create_point(-1, 0, 0),
                                                    sunray::create_point(1, 0, 0));
    CHECK(triangle->origin() == sunray::create_point(0, 0, 0));
    CHECK(triangle->transformation() == sunray::Matrix44::identity());
    CHECK(triangle->inverse_transformation() == sunray::Matrix44::identity().inverse());
    CHECK(triangle->material() == default_material);

    CHECK(triangle->p1() == sunray::create_point(0, 1, 0));
    CHECK(triangle->p2() == sunray::create_point(-1, 0, 0));
    CHECK(triangle->p3() == sunray::create_point(1, 0, 0));
    CHECK(triangle->e1() == sunray::create_vector(-1, -1, 0));
    CHECK(triangle->e2() == sunray::create_vector(1, -1, 0));
    CHECK(triangle->normal_at(sunray::create_point(0.0f, 0.5f, 0.0f)) == sunray::create_vector(0, 0, -1));
    CHECK(triangle->normal_at(sunray::create_point(-0.5f, 0.75f, 0.0f)) == sunray::create_vector(0, 0, -1));
    CHECK(triangle->normal_at(sunray::create_point(0.5f, 0.25f, 0.0f)) == sunray::create_vector(0, 0, -1));
  }
}

TEST_CASE("intersect triangles", "[triangle]")
{
  auto triangle =
    sunray::Triangle::make_triangle(sunray::create_point(0, 1, 0), sunray::create_point(-1, 0, 0), sunray::create_point(1, 0, 0));

  SECTION("intersect a ray parallel to triangle")
  {
    auto ray = sunray::Ray{sunray::create_point(0, -1, -2), sunray::create_vector(0, 1, 0)};

    sunray::Intersections intersections;
    CHECK_FALSE(triangle->is_intersected_by(ray, intersections));
    CHECK(intersections.intersections().empty());
  }
  SECTION("ray misses triangle edge p1 - p3")
  {
    auto ray = sunray::Ray{sunray::create_point(1, 1, -2), sunray::create_vector(0, 0, 1)};

    sunray::Intersections intersections;
    CHECK_FALSE(triangle->is_intersected_by(ray, intersections));
    CHECK(intersections.intersections().empty());
  }
  SECTION("ray misses triangle edge p1 - p2")
  {
    auto ray = sunray::Ray{sunray::create_point(-1, 1, -2), sunray::create_vector(0, 0, 1)};

    sunray::Intersections intersections;
    CHECK_FALSE(triangle->is_intersected_by(ray, intersections));
    CHECK(intersections.intersections().empty());
  }
  SECTION("ray misses triangle edge p2 - p3")
  {
    auto ray = sunray::Ray{sunray::create_point(0, -1, -2), sunray::create_vector(0, 0, 1)};

    sunray::Intersections intersections;
    CHECK_FALSE(triangle->is_intersected_by(ray, intersections));
    CHECK(intersections.intersections().empty());
  }
  SECTION("ray strikes triangle")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0.5, -2), sunray::create_vector(0, 0, 1)};

    sunray::Intersections intersections;
    CHECK(triangle->is_intersected_by(ray, intersections));
    REQUIRE(1 == intersections.intersections().size());
    CHECK(intersections.hit()->time() == Approx(2));
  }
}
