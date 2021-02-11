//
//  disk_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 19.04.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/disk.h>
#include <sun_ray/feature/transformation.h>

#include <catch2/catch.hpp>


TEST_CASE("create disk", "[disk]")
{
  sunray::Material default_material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f};

  SECTION("create default disk")
  {
    auto disk = sunray::Disk::make_disk();
    CHECK(disk->origin() == sunray::create_point(0, 0, 0));
    CHECK(disk->transformation() == sunray::Matrix44::identity());
    CHECK(disk->inverse_transformation() == sunray::Matrix44::identity().inverse());
    CHECK(disk->material() == default_material);
  }
  SECTION("create sphere with material and transformation")
  {
    auto disk = sunray::Disk::make_disk(default_material, sunray::Matrix44::translation(2, 3, 4));
    CHECK(disk->origin() == sunray::create_point(0, 0, 0));
    CHECK(disk->transformation() == sunray::Matrix44::translation(2, 3, 4));
    CHECK(disk->inverse_transformation() == disk->transformation().inverse());
    CHECK(disk->material() == default_material);
  }
}

TEST_CASE("disk normal", "[disk]")
{
  SECTION("disk normal")
  {
    auto disk = sunray::Disk::make_disk();
    CHECK(disk->normal_at(sunray::create_point(0, 0, 0)) == sunray::create_vector(0, 1, 0));
    CHECK(disk->normal_at(sunray::create_point(10, 0, -10)) == sunray::create_vector(0, 1, 0));
    CHECK(disk->normal_at(sunray::create_point(-5, 0, 150)) == sunray::create_vector(0, 1, 0));
  }
}

TEST_CASE("disk intersections", "[disk]")
{
  SECTION("intersect parallel")
  {
    auto disk = sunray::Disk::make_disk();
    auto ray = sunray::Ray{sunray::create_point(0, 10, 0), sunray::create_vector(0, 0, 1)};

    sunray::Intersections intersections;
    CHECK_FALSE(disk->is_intersected_by(ray, intersections));
  }
  SECTION("intersect coplanar")
  {
    auto disk = sunray::Disk::make_disk();
    auto ray = sunray::Ray{sunray::create_point(0, 0, 0), sunray::create_vector(0, 0, 1)};

    sunray::Intersections intersections;
    CHECK_FALSE(disk->is_intersected_by(ray, intersections));
  }
  SECTION("intersect from above")
  {
    auto disk = sunray::Disk::make_disk();
    auto ray = sunray::Ray{sunray::create_point(0, 1, 0), sunray::create_vector(0, -1, 0)};

    sunray::Intersections intersections;
    CHECK(disk->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 1);
    CHECK(intersections.intersections()[0].time() == Approx(1));
    CHECK(disk.get() == intersections.intersections()[0].object());
  }
  SECTION("intersect from below")
  {
    auto disk = sunray::Disk::make_disk();
    auto ray = sunray::Ray{sunray::create_point(0, -1, 0), sunray::create_vector(0, 1, 0)};

    sunray::Intersections intersections;
    CHECK(disk->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 1);
    CHECK(intersections.intersections()[0].time() == Approx(1));
    CHECK(disk.get() == intersections.intersections()[0].object());
  }
}
