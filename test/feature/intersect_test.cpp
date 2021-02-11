//
//  intersect_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 05.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/sphere.h>

#include <catch2/catch.hpp>


TEST_CASE("create intersection", "[intersection]")
{
  SECTION("create intersection")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersection i1{6.5, sphere.get()};

    CHECK(i1.time() == Approx(6.5));
    CHECK(i1.object() == sphere.get());
  }
  SECTION("compare less intersections")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersection i1{6.5, sphere.get()};
    sunray::Intersection i2{7.5, sphere.get()};
    CHECK(i1 < i2);
    CHECK_FALSE(i2 < i1);
  }
  SECTION("compare equal intersections")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersection i1{6.5, sphere.get()};
    sunray::Intersection i2{7.5, sphere.get()};
    CHECK_FALSE(i1 == i2);
    CHECK(i2 == i2);
    sunray::Intersection i3{7.5, sphere.get()};
    CHECK(i2 == i3);

    auto sphere2 = sunray::Sphere::make_sphere();
    sunray::Intersection i4{6.5, sphere.get()};
    sunray::Intersection i5{6.5, sphere2.get()};
    CHECK_FALSE(i4 == i5);
    CHECK(i1 == i4);
  }
}

TEST_CASE("assign intersection", "[intersection]")
{
  SECTION("assign intersection")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersection i1{6.5, sphere.get()};
    sunray::Intersection i2{3.5, sphere.get()};
    i2 = i1;

    CHECK(i2.time() == Approx(6.5));
    CHECK(i2.object() == sphere.get());
  }
  SECTION("move intersection")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersection i1{6.5, sphere.get()};
    sunray::Intersection i2{3.5, sphere.get()};
    i2 = std::move(i1);

    CHECK(i2.time() == Approx(6.5));
    CHECK(i2.object() == sphere.get());
  }
}

TEST_CASE("compare intersections", "[intersection]")
{
  SECTION("less intersection")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersection i1{6.5, sphere.get()};
    sunray::Intersection i2{3.5, sphere.get()};

    CHECK(i2 < i1);
    CHECK_FALSE(i1 < i2);
  }
}


TEST_CASE("intersect sphere", "[intersect]")
{
  SECTION("intersect sphere at two points")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersections intersections;
    CHECK(sphere->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].object() == sphere.get());
    CHECK(intersections.intersections()[0].time() == Approx(4.0));
    CHECK(intersections.intersections()[1].object() == sphere.get());
    CHECK(intersections.intersections()[1].time() == Approx(6.0));
  }
  SECTION("intersect sphere at tangent")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 1, -5), sunray::create_vector(0, 0, 1)};
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersections intersections;
    CHECK(sphere->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].object() == sphere.get());
    CHECK(intersections.intersections()[0].time() == Approx(5.0));
    CHECK(intersections.intersections()[1].object() == sphere.get());
    CHECK(intersections.intersections()[1].time() == Approx(5.0));
  }
  SECTION("ray misses sphere")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 2, -5), sunray::create_vector(0, 0, 1)};
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersections intersections;
    CHECK_FALSE(sphere->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().empty());
  }
  SECTION("intersect sphere inside sphere")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, 0), sunray::create_vector(0, 0, 1)};
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersections intersections;
    CHECK(sphere->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].object() == sphere.get());
    CHECK(intersections.intersections()[0].time() == Approx(-1.0));
    CHECK(intersections.intersections()[1].object() == sphere.get());
    CHECK(intersections.intersections()[1].time() == Approx(1.0));
  }
  SECTION("sphere behind ray")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, 5), sunray::create_vector(0, 0, 1)};
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersections intersections;
    CHECK(sphere->is_intersected_by(ray, intersections));
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].object() == sphere.get());
    CHECK(intersections.intersections()[0].time() == Approx(-6.0));
    CHECK(intersections.intersections()[1].object() == sphere.get());
    CHECK(intersections.intersections()[1].time() == Approx(-4.0));
  }
}

TEST_CASE("intersect object", "[intersect]")
{
  SECTION("intersect")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};
    auto sphere = sunray::Sphere::make_sphere();
    auto intersections = sunray::intersect(ray, *sphere);
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].object() == sphere.get());
    CHECK(intersections.intersections()[0].time() == Approx(4.0));
    CHECK(intersections.intersections()[1].object() == sphere.get());
    CHECK(intersections.intersections()[1].time() == Approx(6.0));
  }
}

TEST_CASE("object hit", "[intersect]")
{
  SECTION("all positive t")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{1, sphere.get()});
    intersections.add(sunray::Intersection{2, sphere.get()});
    auto intersection = intersections.hit();
    REQUIRE(intersection);
    CHECK(intersection->time() == Approx(1.0));
  }
  SECTION("some negative t")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{-1, sphere.get()});
    intersections.add(sunray::Intersection{1, sphere.get()});
    auto intersection = intersections.hit();
    REQUIRE(intersection);
    CHECK(intersection->time() == Approx(1.0));
  }
  SECTION("all negative t")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{-1, sphere.get()});
    intersections.add(sunray::Intersection{-2, sphere.get()});
    auto intersection = intersections.hit();
    CHECK_FALSE(intersection);
  }
  SECTION("lowest non negative t")
  {
    auto sphere = sunray::Sphere::make_sphere();
    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{5, sphere.get()});
    intersections.add(sunray::Intersection{7, sphere.get()});
    intersections.add(sunray::Intersection{-3, sphere.get()});
    intersections.add(sunray::Intersection{2, sphere.get()});
    auto intersection = intersections.hit();
    REQUIRE(intersection);
    CHECK(intersection->time() == Approx(2.0));
  }
}

TEST_CASE("intersect object with transformed ray", "[intersect]")
{
  SECTION("intersect scaled transformed")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};
    auto sphere = sunray::Sphere::make_sphere(sunray::Matrix44::scaling(2, 2, 2));
    auto intersections = sunray::intersect(ray, *sphere);
    REQUIRE(intersections.intersections().size() == 2);
    CHECK(intersections.intersections()[0].object() == sphere.get());
    CHECK(intersections.intersections()[0].time() == Approx(3.0));
    CHECK(intersections.intersections()[1].object() == sphere.get());
    CHECK(intersections.intersections()[1].time() == Approx(7.0));
  }
  SECTION("intersect translated transformed")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};
    auto sphere = sunray::Sphere::make_sphere(sunray::Matrix44::translation(5, 0, 0));
    auto intersections = sunray::intersect(ray, *sphere);
    CHECK(intersections.intersections().empty());
  }
}
