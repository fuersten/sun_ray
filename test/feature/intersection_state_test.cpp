//
//  intersection_state_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 18.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/intersection_state.h>
#include <sun_ray/feature/plane.h>
#include <sun_ray/feature/sphere.h>
#include <sun_ray/feature/transformation.h>

#include <catch2/catch.hpp>

namespace
{
  sunray::SpherePtr make_glass_sphere(const sunray::Matrix44& matrix)
  {
    return sunray::Sphere::make_sphere(sunray::Material{sunray::Color(1, 1, 1), 1, 0, 0, 200.0f, 0.0f, 1.0f, 1.5f}, matrix);
  }
  sunray::SpherePtr make_glass_sphere()
  {
    return sunray::Sphere::make_sphere(sunray::Material{sunray::Color(1, 1, 1), 1, 0, 0, 200.0f, 0.0f, 1.0f, 1.5f});
  }
}

TEST_CASE("create intersection state", "[intersection state]")
{
  SECTION("create intersection state for outside hit")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersection i1{4, sphere.get()};
    auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};

    sunray::IntersectionState state{i1, ray, sunray::Intersections{}};
    CHECK(state.intersection() == i1);
    CHECK_FALSE(state.is_inside());
    CHECK(state.point() == sunray::create_point(0, 0, -1));
    CHECK(state.eye() == sunray::create_vector(0, 0, -1));
    CHECK(state.normal() == sunray::create_vector(0, 0, -1));
  }
  SECTION("create intersection state for inside hit")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersection i1{1, sphere.get()};
    auto ray = sunray::Ray{sunray::create_point(0, 0, 0), sunray::create_vector(0, 0, 1)};

    sunray::IntersectionState state{i1, ray, sunray::Intersections{}};
    CHECK(state.intersection() == i1);
    CHECK(state.is_inside());
    CHECK(state.point() == sunray::create_point(0, 0, 1));
    CHECK(state.eye() == sunray::create_vector(0, 0, -1));
    CHECK(state.normal() == sunray::create_vector(0, 0, -1));
  }
  SECTION("hit offsets the point")
  {
    sunray::Transformation trans;
    trans.translate(0, 0, 1);
    auto sphere = sunray::Sphere::make_sphere(trans.matrix());
    sunray::Intersection i1{5, sphere.get()};
    auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};

    sunray::IntersectionState state{i1, ray, sunray::Intersections{}};
    CHECK(state.intersection() == i1);
    CHECK_FALSE(state.is_inside());
    CHECK(state.over_point().z() < -sunray::epsilon / 2);
  }
  SECTION("reflection vector")
  {
    auto plane = sunray::Plane::make_plane();
    auto ray = sunray::Ray{sunray::create_point(0, 1, -1), sunray::create_vector(0, -sqrt(2) / 2, sqrt(2) / 2)};
    sunray::Intersection i1{sqrt(2), plane.get()};

    sunray::IntersectionState state{i1, ray, sunray::Intersections{}};
    CHECK(state.intersection() == i1);
    CHECK(state.reflect() == sunray::create_vector(0, sqrt(2) / 2, sqrt(2) / 2));
  }
}

TEST_CASE("create intersection state for refractions", "[intersection state]")
{
  sunray::Transformation trans;
  trans.scale(2, 2, 2);
  auto sphere_a =
    sunray::Sphere::make_sphere(sunray::Material{sunray::Color(1, 1, 1), 1, 0, 0, 200.0f, 0.0f, 1.0f, 1.5f}, trans.matrix());
  trans.clear();
  trans.translate(0, 0, -0.25);
  auto sphere_b =
    sunray::Sphere::make_sphere(sunray::Material{sunray::Color(1, 1, 1), 1, 0, 0, 200.0f, 0.0f, 1.0f, 2.0f}, trans.matrix());
  trans.clear();
  trans.translate(0, 0, 0.25);
  auto sphere_c =
    sunray::Sphere::make_sphere(sunray::Material{sunray::Color(1, 1, 1), 1, 0, 0, 200.0f, 0.0f, 1.0f, 2.5f}, trans.matrix());

  sunray::Ray ray{sunray::create_point(0, 0, -4), sunray::create_vector(0, 0, 1)};

  SECTION("find n1 and n2 at intersections")
  {
    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{2, sphere_a.get()});
    intersections.add(sunray::Intersection{2.75, sphere_b.get()});
    intersections.add(sunray::Intersection{3.25, sphere_c.get()});
    intersections.add(sunray::Intersection{4.75, sphere_b.get()});
    intersections.add(sunray::Intersection{5.25, sphere_c.get()});
    intersections.add(sunray::Intersection{6, sphere_a.get()});

    sunray::IntersectionState state1{intersections.intersections()[0], ray, intersections};
    CHECK(state1.n1() == Approx(1.0));
    CHECK(state1.n2() == Approx(1.5));

    sunray::IntersectionState state2{intersections.intersections()[1], ray, intersections};
    CHECK(state2.n1() == Approx(1.5));
    CHECK(state2.n2() == Approx(2.0));

    sunray::IntersectionState state3{intersections.intersections()[2], ray, intersections};
    CHECK(state3.n1() == Approx(2.0));
    CHECK(state3.n2() == Approx(2.5));

    sunray::IntersectionState state4{intersections.intersections()[3], ray, intersections};
    CHECK(state4.n1() == Approx(2.5));
    CHECK(state4.n2() == Approx(2.5));

    sunray::IntersectionState state5{intersections.intersections()[4], ray, intersections};
    CHECK(state5.n1() == Approx(2.5));
    CHECK(state5.n2() == Approx(1.5));

    sunray::IntersectionState state6{intersections.intersections()[5], ray, intersections};
    CHECK(state6.n1() == Approx(1.5));
    CHECK(state6.n2() == Approx(1.0));
  }
}

TEST_CASE("calculate intersection state under point", "[intersection state]")
{
  SECTION("calculate under point")
  {
    sunray::Transformation trans;
    trans.translate(0, 0, 1);
    auto sphere = make_glass_sphere(trans.matrix());
    sunray::Ray ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};

    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{5, sphere.get()});
    sunray::IntersectionState state{intersections.intersections()[0], ray, intersections};
    CHECK(state.under_point().z() > sunray::epsilon / 2);
  }
}

TEST_CASE("calculate schlick", "[intersection state]")
{
  auto sphere = make_glass_sphere();

  SECTION("schlick under total internal reflection")
  {
    sunray::Ray ray{sunray::create_point(0, 0, sunray::sqrt(2) / 2), sunray::create_vector(0, 1, 0)};
    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{-sunray::sqrt(2) / 2, sphere.get()});
    intersections.add(sunray::Intersection{sunray::sqrt(2) / 2, sphere.get()});
    sunray::IntersectionState state{intersections.intersections()[1], ray, intersections};
    CHECK(state.schlick() == Approx(1.0));
  }
  SECTION("schlick with perpendicular viewing angle")
  {
    sunray::Ray ray{sunray::create_point(0, 0, 0), sunray::create_vector(0, 1, 0)};
    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{-1.0, sphere.get()});
    intersections.add(sunray::Intersection{1.0, sphere.get()});
    sunray::IntersectionState state{intersections.intersections()[1], ray, intersections};
    CHECK(state.schlick() == Approx(0.04));
  }
  SECTION("schlick with small angle and n2 > n1")
  {
    sunray::Ray ray{sunray::create_point(0, 0.99, -2), sunray::create_vector(0, 0, 1)};
    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{1.8589, sphere.get()});
    sunray::IntersectionState state{intersections.intersections()[0], ray, intersections};
    CHECK(state.schlick() == Approx(0.48873));
  }
}
