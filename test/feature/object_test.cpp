//
//  object_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 03.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/object.h>
#include <sun_ray/feature/transformation.h>

#include <catch2/catch.hpp>


namespace
{
  class TestObject : public sunray::Object
  {
  public:
    TestObject() = default;

    explicit TestObject(sunray::Material material)
    : Object(std::move(material))
    {
    }

    explicit TestObject(sunray::Matrix44 transformation)
    : Object(std::move(transformation))
    {
    }

    TestObject(sunray::Material material, sunray::Matrix44 transformation)
    : Object(std::move(material), std::move(transformation))
    {
    }

    const sunray::Ray& ray() const
    {
      return *ray_;
    }

  private:
    bool do_intersected_by(const sunray::Ray& ray, sunray::Intersections& intersections) const override
    {
      (void)intersections;
      ray_.reset(new sunray::Ray(ray.origin(), ray.direction()));
      return true;
    }

    sunray::Vector do_normal_at(const sunray::Point& point) const override
    {
      return sunray::create_vector(point.x(), point.y(), point.z());
    }

    mutable std::unique_ptr<sunray::Ray> ray_;
  };
}


TEST_CASE("create object", "[object]")
{
  sunray::Material default_material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f};

  SECTION("create default object")
  {
    TestObject object;
    CHECK(object.origin() == sunray::create_point(0, 0, 0));
    CHECK(object.transformation() == sunray::Matrix44::identity());
    CHECK(object.inverse_transformation() == sunray::Matrix44::identity().inverse());
    CHECK(object.material() == default_material);
  }
  SECTION("create object with transformation")
  {
    TestObject object(sunray::Matrix44::translation(2, 3, 4));
    CHECK(object.origin() == sunray::create_point(0, 0, 0));
    CHECK(object.transformation() == sunray::Matrix44::translation(2, 3, 4));
    CHECK(object.inverse_transformation() == object.transformation().inverse());
    CHECK(object.material() == default_material);
  }
  SECTION("create object with material")
  {
    TestObject object(default_material);
    CHECK(object.origin() == sunray::create_point(0, 0, 0));
    CHECK(object.transformation() == sunray::Matrix44::identity());
    CHECK(object.material() == default_material);
  }
  SECTION("create object with material and transformation")
  {
    TestObject object(default_material, sunray::Matrix44::translation(2, 3, 4));
    CHECK(object.origin() == sunray::create_point(0, 0, 0));
    CHECK(object.transformation() == sunray::Matrix44::translation(2, 3, 4));
    CHECK(object.inverse_transformation() == object.transformation().inverse());
    CHECK(object.material() == default_material);
  }
}

TEST_CASE("object intersect method", "[object]")
{
  SECTION("intersect scaled shape with a ray")
  {
    TestObject object(sunray::Matrix44::scaling(2, 2, 2));
    sunray::Ray ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};
    sunray::Intersections intersections;
    object.is_intersected_by(ray, intersections);
    CHECK(object.ray() == sunray::Ray{sunray::create_point(0, 0, -2.5), sunray::create_vector(0, 0, 0.5)});
  }
  SECTION("intersect translated shape with a ray")
  {
    TestObject object(sunray::Matrix44::translation(5, 0, 0));
    sunray::Ray ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};
    sunray::Intersections intersections;
    object.is_intersected_by(ray, intersections);
    CHECK(object.ray() == sunray::Ray{sunray::create_point(-5, 0, -5), sunray::create_vector(0, 0, 1)});
  }
}

TEST_CASE("object normal method", "[object]")
{
  SECTION("test normal translated")
  {
    TestObject object(sunray::Matrix44::translation(0, 1, 0));
    CHECK(object.normal_at(sunray::create_point(0, 1.70711, -0.70711)) == sunray::create_vector(0, 0.70711, -0.70711));
  }
  SECTION("test normal transformed")
  {
    sunray::Transformation trans;
    trans.rotate_z(sunray::PI / 5).scale(1, 0.5, 1);
    TestObject object(trans.matrix());
    CHECK(object.normal_at(sunray::create_point(0, sunray::sqrt(2) / 2, -sunray::sqrt(2) / 2)) ==
          sunray::create_vector(0, 0.97014, -0.24254));
  }
}
