//
//  world_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 18.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/plane.h>
#include <sun_ray/feature/sphere.h>
#include <sun_ray/feature/transformation.h>
#include <sun_ray/feature/world.h>

#include <sstream>

#include <catch2/catch.hpp>


namespace
{
  sunray::World default_world()
  {
    sunray::World world;
    auto light = std::make_shared<sunray::PointLight>(sunray::create_point(-10, 10, -10), sunray::Color(1, 1, 1));
    world.add_light(light);

    auto s1 =
      sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 0.1f, 0.7f, 0.2f, 200.0f, 0.0f, 0.0f, 1.0f});

    sunray::Transformation trans;
    trans.scale(0.5, 0.5, 0.5);
    auto s2 = sunray::Sphere::make_sphere(trans.matrix());

    world.add_object(s1);
    world.add_object(s2);

    return world;
  }

  sunray::Color black{0, 0, 0};
  sunray::Color white{1, 1, 1};

  class TestPattern : public sunray::Pattern
  {
  public:
    TestPattern()
    {
    }

    TestPattern(sunray::Matrix44 transformation)
    : Pattern{transformation}
    {
    }

    sunray::Color pattern_at(const sunray::Point& point) const override
    {
      return sunray::Color{static_cast<float>(point.x()), static_cast<float>(point.y()), static_cast<float>(point.z())};
    }

    std::string to_string() const override
    {
      std::stringstream ss;
      return ss.str();
    }
  };
}


TEST_CASE("create world", "[world]")
{
  sunray::World world;
  auto light = std::make_shared<sunray::PointLight>(sunray::create_point(-10, 10, -10), sunray::Color(1, 1, 1));
  world.add_light(light);

  auto s1 =
    sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 0.1f, 0.7f, 0.2f, 200.0f, 0.0f, 0.0f, 1.0f});

  sunray::Transformation trans;
  trans.scale(0.5, 0.5, 0.5);
  auto s2 = sunray::Sphere::make_sphere(trans.matrix());

  world.add_object(s1);
  world.add_object(s2);

  SECTION("contains object")
  {
    CHECK(world.contains(s1));
    CHECK(world.contains(s2));
  }
  SECTION("does not contain object")
  {
    auto s = sunray::Sphere::make_sphere(trans.matrix());
    CHECK_FALSE(world.contains(s));
  }
}

TEST_CASE("intersect world", "[world]")
{
  sunray::World world = default_world();

  SECTION("intersect world")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};
    sunray::Intersections intersections;
    world.intersect(ray, intersections);
    REQUIRE(intersections.intersections().size() == 4);
    CHECK(intersections.intersections()[0].time() == Approx(4));
    CHECK(intersections.intersections()[1].time() == Approx(4.5));
    CHECK(intersections.intersections()[2].time() == Approx(5.5));
    CHECK(intersections.intersections()[3].time() == Approx(6));
  }
  SECTION("intersect world from inside")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, 0), sunray::create_vector(0, 0, 1)};
    sunray::Intersections intersections;
    world.intersect(ray, intersections);
    REQUIRE(intersections.intersections().size() == 4);
    CHECK(intersections.intersections()[0].time() == Approx(-1));
    CHECK(intersections.intersections()[1].time() == Approx(-0.5));
    CHECK(intersections.intersections()[2].time() == Approx(0.5));
    CHECK(intersections.intersections()[3].time() == Approx(1));
  }
  SECTION("shade intersection hit")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};
    sunray::Intersections intersections;
    world.intersect(ray, intersections);
    sunray::IntersectionState state{intersections.intersections()[0], ray, intersections};
    CHECK(world.shade_hit(state, 1) == sunray::Color{0.38066f, 0.47583f, 0.2855f});
  }
  SECTION("color if ray misses")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 1, 0)};
    sunray::Intersections intersections;
    CHECK(world.color_at(ray, intersections) == black);
  }
  SECTION("color if ray hits")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};
    sunray::Intersections intersections;
    CHECK(world.color_at(ray, intersections) == sunray::Color{0.38066f, 0.47583f, 0.2855f});
  }
}

TEST_CASE("intersect world from inside", "[world]")
{
  sunray::World world;
  auto light = std::make_shared<sunray::PointLight>(sunray::create_point(0, 0.25, 0), sunray::Color(1, 1, 1));
  world.add_light(light);

  auto s1 =
    sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 0.1f, 0.7f, 0.2f, 200.0f, 0.0f, 0.0f, 1.0f});

  sunray::Transformation trans;
  trans.scale(0.5, 0.5, 0.5);
  auto s2 = sunray::Sphere::make_sphere(trans.matrix());

  world.add_object(s1);
  world.add_object(s2);

  SECTION("shade intersection hit from inside")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, 0), sunray::create_vector(0, 0, 1)};
    sunray::Intersections intersections;
    world.intersect(ray, intersections);

    REQUIRE(intersections.intersections().size() == 4);
    CHECK(intersections.intersections()[2].time() == Approx(0.5));
    sunray::IntersectionState state{intersections.intersections()[2], ray, intersections};
    CHECK(world.shade_hit(state, 1) == sunray::Color{0.90498f, 0.90498f, 0.90498f});
  }
}

TEST_CASE("shade shadows", "[world]")
{
  sunray::World world;
  auto light = std::make_shared<sunray::PointLight>(sunray::create_point(0, 0, -10), sunray::Color(1, 1, 1));
  world.add_light(light);

  auto s1 = sunray::Sphere::make_sphere();

  sunray::Transformation trans;
  trans.translate(0, 0, 10);
  auto s2 = sunray::Sphere::make_sphere(trans.matrix());

  world.add_object(s1);
  world.add_object(s2);

  SECTION("shade intersections in shadow")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, 5), sunray::create_vector(0, 0, 1)};
    sunray::Intersections intersections;
    world.intersect(ray, intersections);

    REQUIRE(intersections.intersections().size() == 4);
    CHECK(intersections.intersections()[2].time() == Approx(4));
    sunray::IntersectionState state{intersections.intersections()[2], ray, intersections};
    CHECK(world.shade_hit(state, 1) == sunray::Color{0.1f, 0.1f, 0.1f});
  }
}

TEST_CASE("shade object disabled shadows", "[world]")
{
  sunray::World world;
  auto light = std::make_shared<sunray::PointLight>(sunray::create_point(0, 0, -10), sunray::Color(1, 1, 1));
  world.add_light(light);

  auto s1 = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 1, 0.7f, 0.2f, 200.0f, 0.0f, 0.0f, 1.0f},
                                        sunray::Matrix44::identity());

  sunray::Transformation trans;
  trans.translate(0, 0, 10);
  auto s2 = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 1, 0.7f, 0.2f, 200.0f, 0.0f, 0.0f, 1.0f},
                                        trans.matrix(), false);

  world.add_object(s1);
  world.add_object(s2);

  SECTION("shade intersections in shadow with object disabled shadows")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, 5), sunray::create_vector(0, 0, 1)};
    sunray::Intersections intersections;
    world.intersect(ray, intersections);

    REQUIRE(intersections.intersections().size() == 4);
    CHECK(intersections.intersections()[2].time() == Approx(4));
    sunray::IntersectionState state{intersections.intersections()[2], ray, intersections};
    CHECK(world.shade_hit(state, 1) == sunray::Color{0.8f, 1, 0.6f});
  }
}

TEST_CASE("intersect world behind", "[world]")
{
  sunray::World world;
  auto light = std::make_shared<sunray::PointLight>(sunray::create_point(-10, 10, -10), sunray::Color(1, 1, 1));
  world.add_light(light);

  auto s1 = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 1, 0.7f, 0.2f, 200.0f, 0.0f, 0.0f, 1.0f});

  sunray::Transformation trans;
  trans.scale(0.5, 0.5, 0.5);
  auto s2 = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{1, 1, 1}, 1, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f},
                                        trans.matrix());

  world.add_object(s1);
  world.add_object(s2);

  SECTION("color if hit behind ray")
  {
    auto ray = sunray::Ray{sunray::create_point(0, 0, 0.75), sunray::create_vector(0, 0, -1)};
    sunray::Intersections intersections;
    CHECK(world.color_at(ray, intersections) == s2->material().color());
  }
}

TEST_CASE("shadow test", "[world]")
{
  sunray::World world = default_world();
  auto light = std::make_shared<sunray::PointLight>(sunray::create_point(-10, 10, -10), sunray::Color(1, 1, 1));

  SECTION("nothing collinear with point and light")
  {
    auto point = sunray::create_point(0, 10, 0);
    CHECK_FALSE(world.is_shadowed(*light, point));
  }
  SECTION("object between point and light")
  {
    auto point = sunray::create_point(10, -10, 10);
    CHECK(world.is_shadowed(*light, point));
  }
  SECTION("object behind the light")
  {
    auto point = sunray::create_point(-20, 20, -20);
    CHECK_FALSE(world.is_shadowed(*light, point));
  }
  SECTION("point between object and the light")
  {
    auto point = sunray::create_point(-2, 2, -2);
    CHECK_FALSE(world.is_shadowed(*light, point));
  }
  SECTION("object between point and light with disabled shadows")
  {
    sunray::RenderContext context;
    context.shadows_ = false;
    world.context(context);

    auto point = sunray::create_point(10, -10, 10);
    CHECK_FALSE(world.is_shadowed(*light, point));
  }
}

TEST_CASE("reflection test", "[world]")
{
  sunray::World world;
  auto light = std::make_shared<sunray::PointLight>(sunray::create_point(-10, 10, -10), sunray::Color(1, 1, 1));
  world.add_light(light);

  auto s1 =
    sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 0.1f, 0.7f, 0.2f, 200.0f, 0.0f, 0.0f, 1.0f});

  sunray::Transformation trans;
  trans.scale(0.5, 0.5, 0.5);
  auto s2 = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f},
                                        trans.matrix());

  world.add_object(s1);
  world.add_object(s2);
  auto ray = sunray::Ray{sunray::create_point(0, 0, 0), sunray::create_vector(0, 0, 1)};

  SECTION("reflected color for nonreflective material")
  {
    sunray::Intersection i1{1, s2.get()};
    sunray::IntersectionState state{i1, ray, sunray::Intersections{}};
    auto color = world.reflect_color(state, 1);
    CHECK(color == black);
  }
  SECTION("reflected color for a refelctive material")
  {
    sunray::Transformation trans1;
    trans1.translate(0, -1, 0);
    auto plane = sunray::Plane::make_plane(sunray::Material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.5f, 0.0f, 1.0f},
                                           trans1.matrix());
    world.add_object(plane);
    auto ray1 = sunray::Ray{sunray::create_point(0, 0, -3), sunray::create_vector(0, -sqrt(2) / 2, sqrt(2) / 2)};
    sunray::Intersection i1{sqrt(2), plane.get()};
    sunray::IntersectionState state{i1, ray1, sunray::Intersections{}};
    auto color = world.reflect_color(state, 1);
    CHECK(color == sunray::Color{0.19032f, 0.2379f, 0.14274f});
  }
}

TEST_CASE("recursive reflection test", "[world]")
{
  sunray::World world;
  auto light = std::make_shared<sunray::PointLight>(sunray::create_point(0, 0, 0), sunray::Color(1, 1, 1));
  world.add_light(light);

  auto ray = sunray::Ray{sunray::create_point(0, 0, 0), sunray::create_vector(0, 1, 0)};

  SECTION("reflected color for mutually refelctive materials")
  {
    sunray::Transformation trans;
    trans.translate(0, -1, 0);
    auto lower_plane = sunray::Plane::make_plane(
      sunray::Material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 1, 0.0f, 1.0f}, trans.matrix());
    world.add_object(lower_plane);

    sunray::Transformation trans1;
    trans1.translate(0, 1, 0);
    auto upper_plane = sunray::Plane::make_plane(
      sunray::Material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 1, 0.0f, 1.0f}, trans1.matrix());
    world.add_object(upper_plane);

    sunray::Intersections intersections;
    world.color_at(ray, intersections);
  }
}

TEST_CASE("limit maximum recursive depth", "[world]")
{
  sunray::World world = default_world();
  sunray::Transformation trans;
  trans.translate(0, -1, 0);
  auto plane = sunray::Plane::make_plane(sunray::Material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.5f, 0.0f, 1.0f},
                                         trans.matrix());
  world.add_object(plane);
  auto ray = sunray::Ray{sunray::create_point(0, 0, -3), sunray::create_vector(0, -sqrt(2) / 2, sqrt(2) / 2)};

  SECTION("intersect world")
  {
    sunray::Intersection i1{sqrt(2), plane.get()};
    sunray::IntersectionState state{i1, ray, sunray::Intersections{}};
    auto color = world.reflect_color(state, 0);
    CHECK(color == black);
  }
}

TEST_CASE("refraction color test", "[world]")
{
  sunray::World world;
  auto light = std::make_shared<sunray::PointLight>(sunray::create_point(-10, 10, -10), sunray::Color(1, 1, 1));
  world.add_light(light);

  auto s1 =
    sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 0.1f, 0.7f, 0.2f, 200.0f, 0.0f, 0.0f, 1.0f});

  world.add_object(s1);
  auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};

  SECTION("refracted color with opaque surface")
  {
    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{4, s1.get()});
    intersections.add(sunray::Intersection{6, s1.get()});
    sunray::IntersectionState state{intersections.intersections()[0], ray, intersections};
    CHECK(world.refracted_color(state, 5) == black);
  }
}

TEST_CASE("refraction test with maximun depth", "[world]")
{
  sunray::World world;
  auto light = std::make_shared<sunray::PointLight>(sunray::create_point(-10, 10, -10), sunray::Color(1, 1, 1));
  world.add_light(light);

  auto s1 =
    sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 0.1f, 0.7f, 0.2f, 200.0f, 0.0f, 1.0f, 1.5f});

  world.add_object(s1);
  auto ray = sunray::Ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};

  SECTION("refracted color with maximun depth")
  {
    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{4, s1.get()});
    intersections.add(sunray::Intersection{6, s1.get()});
    sunray::IntersectionState state{intersections.intersections()[0], ray, intersections};
    CHECK(world.refracted_color(state, 0) == black);
  }
}

TEST_CASE("refraction test with total reflection", "[world]")
{
  sunray::World world;
  auto light = std::make_shared<sunray::PointLight>(sunray::create_point(-10, 10, -10), sunray::Color(1, 1, 1));
  world.add_light(light);

  auto s1 =
    sunray::Sphere::make_sphere(sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 0.1f, 0.7f, 0.2f, 200.0f, 0.0f, 1.0f, 1.5f});

  world.add_object(s1);
  auto ray = sunray::Ray{sunray::create_point(0, 0, sunray::sqrt(2) / 2), sunray::create_vector(0, 1, 0)};

  SECTION("refraction test with total reflection")
  {
    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{-sunray::sqrt(2) / 2, s1.get()});
    intersections.add(sunray::Intersection{sunray::sqrt(2) / 2, s1.get()});
    sunray::IntersectionState state{intersections.intersections()[1], ray, intersections};
    CHECK(world.refracted_color(state, 5) == black);
  }
}

TEST_CASE("refraction test with reflection", "[world]")
{
  sunray::World world;
  auto light = std::make_shared<sunray::PointLight>(sunray::create_point(-10, 10, -10), sunray::Color(1, 1, 1));
  world.add_light(light);

  auto material = sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 1, 0.7f, 0.2f, 200.0f, 0.0f, 0.0f, 1.0f};
  material.pattern(std::make_shared<TestPattern>());
  auto s1 = sunray::Sphere::make_sphere(material);

  sunray::Transformation trans;
  trans.scale(0.5, 0.5, 0.5);
  auto s2 = sunray::Sphere::make_sphere(
    sunray::Material{sunray::Color{0.8f, 1, 0.6f}, 0.1f, 0.7f, 0.2f, 200.0f, 0.0f, 1.0f, 1.5f}, trans.matrix());

  world.add_object(s1);
  world.add_object(s2);

  auto ray = sunray::Ray{sunray::create_point(0, 0, 0.1), sunray::create_vector(0, 1, 0)};

  SECTION("refracted color with refracted ray")
  {
    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{-0.9899, s1.get()});
    intersections.add(sunray::Intersection{-0.4899, s2.get()});
    intersections.add(sunray::Intersection{0.4899, s2.get()});
    intersections.add(sunray::Intersection{0.9899, s1.get()});
    sunray::IntersectionState state{intersections.intersections()[2], ray, intersections};
    CHECK(world.refracted_color(state, 5) == sunray::Color{0, 0.99888f, 0.04725f});
  }
}

TEST_CASE("reflection and transparent test", "[world]")
{
  sunray::World world = default_world();

  sunray::Transformation trans;
  trans.translate(0, -3.5, -0.5);
  auto ball = sunray::Sphere::make_sphere(sunray::Material{sunray::Color{1, 0, 0}, 0.5f, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f},
                                          trans.matrix());

  world.add_object(ball);
  auto ray = sunray::Ray{sunray::create_point(0, 0, -3), sunray::create_vector(0, -sqrt(2) / 2, sqrt(2) / 2)};

  SECTION("reflected color for a transparent material")
  {
    trans.clear();
    trans.translate(0, -1, 0);
    auto floor = sunray::Plane::make_plane(sunray::Material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.0f, 0.5f, 1.5f},
                                           trans.matrix());
    world.add_object(floor);

    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{sqrt(2), floor.get()});
    sunray::IntersectionState state{intersections.intersections()[0], ray, intersections};
    auto color = world.shade_hit(state, 5);
    CHECK(color == sunray::Color{0.93642f, 0.68642f, 0.68642f});
  }
  SECTION("reflected color for a refelctive and transparent material")
  {
    trans.clear();
    trans.translate(0, -1, 0);
    auto floor = sunray::Plane::make_plane(sunray::Material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.5f, 0.5f, 1.5f},
                                           trans.matrix());
    world.add_object(floor);

    sunray::Intersections intersections;
    intersections.add(sunray::Intersection{sqrt(2), floor.get()});
    sunray::IntersectionState state{intersections.intersections()[0], ray, intersections};
    auto color = world.shade_hit(state, 5);
    CHECK(color == sunray::Color{0.93391f, 0.69643f, 0.69243f});
  }
}
