//
//  cylinder_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 25.04.21.
//  Copyright © 2021 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/cylinder.h>
#include <sun_ray/feature/transformation.h>

#include <catch2/catch.hpp>


TEST_CASE("create cylinder", "[cylinder]")
{
  sunray::Material default_material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f};

  SECTION("create default cylinder")
  {
    auto cylinder = sunray::Cylinder::make_cylinder();
    CHECK(cylinder->origin() == sunray::create_point(0, 0, 0));
    CHECK(cylinder->transformation() == sunray::Matrix44::identity());
    CHECK(cylinder->inverse_transformation() == sunray::Matrix44::identity().inverse());
    CHECK(cylinder->material() == default_material);
  }
  SECTION("create cylinder with material and transformation")
  {
    auto cylinder = sunray::Cylinder::make_cylinder(default_material, sunray::Matrix44::translation(2, 3, 4));
    CHECK(cylinder->origin() == sunray::create_point(0, 0, 0));
    CHECK(cylinder->transformation() == sunray::Matrix44::translation(2, 3, 4));
    CHECK(cylinder->inverse_transformation() == cylinder->transformation().inverse());
    CHECK(cylinder->material() == default_material);
  }
}

TEST_CASE("cylinder normal", "[cylinder]")
{
  SECTION("cylinder normal")
  {
    auto cylinder = sunray::Cylinder::make_cylinder();
    CHECK(cylinder->normal_at(sunray::create_point(1, 0, 0)) == sunray::create_vector(1, 0, 0));
    CHECK(cylinder->normal_at(sunray::create_point(0, 5, -1)) == sunray::create_vector(0, 0, -1));
    CHECK(cylinder->normal_at(sunray::create_point(0, -2, 1)) == sunray::create_vector(0, 0, 1));
    CHECK(cylinder->normal_at(sunray::create_point(-1, 1, 0)) == sunray::create_vector(-1, 0, 0));
  }
}

TEST_CASE("a ray strikes a cylinder", "[cylinder]")
{
  auto cylinder = sunray::Cylinder::make_cylinder();

  SECTION("cylinder tangent")
  {
    auto ray = sunray::Ray{sunray::create_point(1, 0, -5), sunray::create_vector(0, 0, 1).normalize()};

    sunray::Intersections intersections;
    CHECK(cylinder->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].time() == Approx(5));
    CHECK(intersections.intersections()[1].time() == Approx(5));
  }
  SECTION("perpendicular")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1).normalize()};

    sunray::Intersections intersections;
    CHECK(cylinder->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].time() == Approx(4));
    CHECK(intersections.intersections()[1].time() == Approx(6));
  }
  SECTION("skewed ray")
  {
    auto ray = sunray::Ray{sunray::create_point(0.5, 0, -5), sunray::create_vector(0.1, 1, 1).normalize()};

    sunray::Intersections intersections;
    CHECK(cylinder->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].time() == Approx(6.80798));
    CHECK(intersections.intersections()[1].time() == Approx(7.08872));
  }
}

TEST_CASE("cylinder intersection misses", "[cylinder]")
{
  auto cylinder = sunray::Cylinder::make_cylinder();

  SECTION("side misses")
  {
    sunray::Intersections intersections;
    CHECK_FALSE(cylinder->is_intersected_by(
      sunray::Ray{sunray::create_point(1, 0, 0), sunray::create_vector(0, 1, 0).normalize()}, intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(cylinder->is_intersected_by(
      sunray::Ray{sunray::create_point(0, 0, 0), sunray::create_vector(0, 1, 0).normalize()}, intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(cylinder->is_intersected_by(
      sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(1, 1, 1).normalize()}, intersections));
    CHECK(intersections.intersections().empty());
  }
}

TEST_CASE("truncated cylinder", "[cylinder]")
{
  auto cylinder = sunray::Cylinder::make_cylinder(2, 1);

  SECTION("misses")
  {
    sunray::Intersections intersections;
    CHECK_FALSE(cylinder->is_intersected_by(
      sunray::Ray{sunray::create_point(0, 1.5, 0), sunray::create_vector(0.1, 1, 0).normalize()}, intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(cylinder->is_intersected_by(
      sunray::Ray{sunray::create_point(0, 3, -5), sunray::create_vector(0, 0, 1).normalize()}, intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(cylinder->is_intersected_by(
      sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1).normalize()}, intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(cylinder->is_intersected_by(
      sunray::Ray{sunray::create_point(0, 2, -5), sunray::create_vector(0, 0, 1).normalize()}, intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(cylinder->is_intersected_by(
      sunray::Ray{sunray::create_point(0, 1, -5), sunray::create_vector(0, 0, 1).normalize()}, intersections));
    CHECK(intersections.intersections().empty());
  }
}
