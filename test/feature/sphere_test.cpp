//
//  sphere_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 05.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/sphere.h>
#include <sun_ray/feature/transformation.h>

#include <catch2/catch.hpp>


TEST_CASE("create sphere", "[sphere]")
{
  sunray::Material default_material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f};

  SECTION("create default sphere")
  {
    auto sphere = sunray::Sphere::make_sphere();
    CHECK(sphere->origin() == sunray::create_point(0, 0, 0));
    CHECK(sphere->transformation() == sunray::Matrix44::identity());
    CHECK(sphere->inverse_transformation() == sunray::Matrix44::identity().inverse());
    CHECK(sphere->material() == default_material);
  }
  SECTION("create sphere with transformation")
  {
    auto sphere = sunray::Sphere::make_sphere(sunray::Matrix44::translation(2, 3, 4));
    CHECK(sphere->origin() == sunray::create_point(0, 0, 0));
    CHECK(sphere->transformation() == sunray::Matrix44::translation(2, 3, 4));
    CHECK(sphere->inverse_transformation() == sphere->transformation().inverse());
    CHECK(sphere->material() == default_material);
  }
  SECTION("create sphere with material")
  {
    auto sphere = sunray::Sphere::make_sphere(default_material);
    CHECK(sphere->origin() == sunray::create_point(0, 0, 0));
    CHECK(sphere->transformation() == sunray::Matrix44::identity());
    CHECK(sphere->material() == default_material);
  }
  SECTION("create sphere with material and transformation")
  {
    auto sphere = sunray::Sphere::make_sphere(default_material, sunray::Matrix44::translation(2, 3, 4));
    CHECK(sphere->origin() == sunray::create_point(0, 0, 0));
    CHECK(sphere->transformation() == sunray::Matrix44::translation(2, 3, 4));
    CHECK(sphere->inverse_transformation() == sphere->transformation().inverse());
    CHECK(sphere->material() == default_material);
  }
}

TEST_CASE("sphere normal", "[sphere]")
{
  SECTION("sphere normal x axis")
  {
    auto sphere = sunray::Sphere::make_sphere();
    CHECK(sphere->normal_at(sunray::create_point(1, 0, 0)) == sunray::create_vector(1, 0, 0));
  }
  SECTION("sphere normal y axis")
  {
    auto sphere = sunray::Sphere::make_sphere();
    CHECK(sphere->normal_at(sunray::create_point(0, 1, 0)) == sunray::create_vector(0, 1, 0));
  }
  SECTION("sphere normal z axis")
  {
    auto sphere = sunray::Sphere::make_sphere();
    CHECK(sphere->normal_at(sunray::create_point(0, 0, 1)) == sunray::create_vector(0, 0, 1));
  }
  SECTION("sphere normal nonaxial")
  {
    auto sphere = sunray::Sphere::make_sphere();
    CHECK(sphere->normal_at(sunray::create_point(sunray::sqrt(3) / 3, sunray::sqrt(3) / 3, sunray::sqrt(3) / 3)) ==
          sunray::create_vector(sunray::sqrt(3) / 3, sunray::sqrt(3) / 3, sunray::sqrt(3) / 3));
  }
  SECTION("sphere normal is normalized")
  {
    auto sphere = sunray::Sphere::make_sphere();
    auto n = sphere->normal_at(sunray::create_point(sunray::sqrt(3) / 3, sunray::sqrt(3) / 3, sunray::sqrt(3) / 3));
    CHECK(n == n.normalize());
  }
}

TEST_CASE("sphere normal translated", "[sphere]")
{
  SECTION("sphere normal translated")
  {
    auto sphere = sunray::Sphere::make_sphere(sunray::Matrix44::translation(0, 1, 0));
    auto n = sphere->normal_at(sunray::create_point(0, 1.70711, -0.70711));
    CHECK(n == sunray::create_vector(0, 0.70711, -0.70711));
  }
  SECTION("sphere normal rotated")
  {
    sunray::Transformation trans;
    trans.rotate_z(sunray::PI / 5).scale(1, 0.5, 1);
    auto sphere = sunray::Sphere::make_sphere(trans.matrix());
    auto n = sphere->normal_at(sunray::create_point(0, sunray::sqrt(2) / 2, -sunray::sqrt(2) / 2));
    CHECK(n == sunray::create_vector(0, 0.97014, -0.242554));
  }
}
