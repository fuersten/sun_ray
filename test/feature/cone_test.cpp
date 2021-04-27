//
//  cone_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 27.04.21.
//  Copyright © 2021 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/cone.h>
#include <sun_ray/feature/transformation.h>

#include <catch2/catch.hpp>


TEST_CASE("create cone", "[cone]")
{
  sunray::Material default_material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f};

  SECTION("create default cone")
  {
    auto cone = sunray::Cone::make_cone();
    CHECK(cone->origin() == sunray::create_point(0, 0, 0));
    CHECK(cone->transformation() == sunray::Matrix44::identity());
    CHECK(cone->inverse_transformation() == sunray::Matrix44::identity().inverse());
    CHECK(cone->material() == default_material);
  }
  SECTION("create cone with material and transformation")
  {
    auto cone = sunray::Cone::make_cone(default_material, sunray::Matrix44::translation(2, 3, 4));
    CHECK(cone->origin() == sunray::create_point(0, 0, 0));
    CHECK(cone->transformation() == sunray::Matrix44::translation(2, 3, 4));
    CHECK(cone->inverse_transformation() == cone->transformation().inverse());
    CHECK(cone->material() == default_material);
  }
}

TEST_CASE("cone normal", "[cone]")
{
  SECTION("cone normal")
  {
    auto cone = sunray::Cone::make_cone();
    CHECK(cone->normal_at(sunray::create_point(0, 0, 0)) == sunray::create_vector(0, 0, 0).normalize());
    CHECK(cone->normal_at(sunray::create_point(1, 1, 1)) == sunray::create_vector(1, -sqrt(2), 1).normalize());
    CHECK(cone->normal_at(sunray::create_point(-1, -1, 0)) == sunray::create_vector(-1, 1, 0).normalize());
  }
}

TEST_CASE("a ray strikes a cone", "[cone]")
{
  auto cone = sunray::Cone::make_cone();

  SECTION("intersect")
  {
    {
      auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1).normalize()};

      sunray::Intersections intersections;
      CHECK(cone->is_intersected_by(ray, intersections));
      REQUIRE(intersections.intersections().size() == 2);
      CHECK(intersections.intersections()[0].time() == Approx(5));
      CHECK(intersections.intersections()[1].time() == Approx(5));
    }
    {
      auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(1, 1, 1).normalize()};

      sunray::Intersections intersections;
      CHECK(cone->is_intersected_by(ray, intersections));
      REQUIRE(intersections.intersections().size() == 2);
      CHECK(intersections.intersections()[0].time() == Approx(8.66025));
      CHECK(intersections.intersections()[1].time() == Approx(8.66025));
    }
    {
      auto ray = sunray::Ray{sunray::create_point(1, 1, -5), sunray::create_vector(-0.5, -1, 1).normalize()};

      sunray::Intersections intersections;
      CHECK(cone->is_intersected_by(ray, intersections));
      REQUIRE(intersections.intersections().size() == 2);
      CHECK(intersections.intersections()[0].time() == Approx(4.550055));
      CHECK(intersections.intersections()[1].time() == Approx(49.44994));
    }
  }
  SECTION("intersect parallel to one cone")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, -1), sunray::create_vector(0, 1, 1).normalize()};

    sunray::Intersections intersections;
    CHECK(cone->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 1);
    CHECK(intersections.intersections()[0].time() == Approx(0.35355));
  }
}

TEST_CASE("truncated cone", "[cone]")
{
  auto cone = sunray::Cone::make_cone(2, 1);
  sunray::Intersections intersections;

  SECTION("misses")
  {
    CHECK_FALSE(cone->is_intersected_by(
      sunray::Ray{sunray::create_point(0, 1.5, 0), sunray::create_vector(0.1, 1, 0).normalize()}, intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(cone->is_intersected_by(sunray::Ray{sunray::create_point(0, 3, -5), sunray::create_vector(0, 0, 1).normalize()},
                                        intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(cone->is_intersected_by(sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1).normalize()},
                                        intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(cone->is_intersected_by(sunray::Ray{sunray::create_point(0, 2, -5), sunray::create_vector(0, 0, 1).normalize()},
                                        intersections));
    CHECK(intersections.intersections().empty());

    CHECK_FALSE(cone->is_intersected_by(sunray::Ray{sunray::create_point(0, 1, -5), sunray::create_vector(0, 0, 1).normalize()},
                                        intersections));
    CHECK(intersections.intersections().empty());
  }
  SECTION("hit")
  {
    CHECK(cone->is_intersected_by(sunray::Ray{sunray::create_point(0, 1.5, -5), sunray::create_vector(0, 0, 1).normalize()},
                                  intersections));
    CHECK(intersections.intersections().size() == 2);
  }
}

TEST_CASE("caped cone", "[cone]")
{
  auto cone = sunray::Cone::make_cone(0.5, -0.5, true);
  sunray::Intersections intersections;

  SECTION("hits")
  {
    CHECK(cone->is_intersected_by(sunray::Ray{sunray::create_point(0, 0, -0.25), sunray::create_vector(0, 1, 1).normalize()},
                                  intersections));
    CHECK(intersections.intersections().size() == 2);

    intersections.clear();
    CHECK(cone->is_intersected_by(sunray::Ray{sunray::create_point(0, 0, -0.25), sunray::create_vector(0, 1, 0).normalize()},
                                  intersections));
    CHECK(intersections.intersections().size() == 4);
  }
  SECTION("miss")
  {
    CHECK_FALSE(cone->is_intersected_by(sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 1, 0).normalize()},
                                        intersections));
    CHECK(intersections.intersections().size() == 0);
  }
}
