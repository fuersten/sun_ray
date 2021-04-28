//
//  shape_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 03.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/shape.h>
#include <sun_ray/feature/transformation.h>

#include <catch2/catch.hpp>


namespace
{
  class TestShape : public sunray::Shape
  {
  public:
    TestShape() = default;

    explicit TestShape(sunray::Material material)
    : Shape(std::move(material))
    {
    }

    explicit TestShape(sunray::Matrix44 transformation)
    : Shape(std::move(transformation))
    {
    }

    TestShape(sunray::Material material, sunray::Matrix44 transformation)
    : Shape(std::move(material), std::move(transformation))
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


TEST_CASE("create shape", "[shape]")
{
  sunray::Material default_material{sunray::Color{1, 1, 1}, 0.1f, 0.9f, 0.9f, 200.0f, 0.0f, 0.0f, 1.0f};

  SECTION("create default shape")
  {
    TestShape shape;
    CHECK(shape.origin() == sunray::create_point(0, 0, 0));
    CHECK(shape.transformation() == sunray::Matrix44::identity());
    CHECK(shape.inverse_transformation() == sunray::Matrix44::identity().inverse());
    CHECK(shape.material() == default_material);
  }
  SECTION("create shape with transformation")
  {
    TestShape shape(sunray::Matrix44::translation(2, 3, 4));
    CHECK(shape.origin() == sunray::create_point(0, 0, 0));
    CHECK(shape.transformation() == sunray::Matrix44::translation(2, 3, 4));
    CHECK(shape.inverse_transformation() == shape.transformation().inverse());
    CHECK(shape.material() == default_material);
  }
  SECTION("create shape with material")
  {
    TestShape shape(default_material);
    CHECK(shape.origin() == sunray::create_point(0, 0, 0));
    CHECK(shape.transformation() == sunray::Matrix44::identity());
    CHECK(shape.material() == default_material);
  }
  SECTION("create shape with material and transformation")
  {
    TestShape shape(default_material, sunray::Matrix44::translation(2, 3, 4));
    CHECK(shape.origin() == sunray::create_point(0, 0, 0));
    CHECK(shape.transformation() == sunray::Matrix44::translation(2, 3, 4));
    CHECK(shape.inverse_transformation() == shape.transformation().inverse());
    CHECK(shape.material() == default_material);
  }
}

TEST_CASE("shape intersect method", "[shape]")
{
  SECTION("intersect scaled shape with a ray")
  {
    TestShape shape(sunray::Matrix44::scaling(2, 2, 2));
    sunray::Ray ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};
    sunray::Intersections intersections;
    shape.is_intersected_by(ray, intersections);
    CHECK(shape.ray() == sunray::Ray{sunray::create_point(0, 0, -2.5), sunray::create_vector(0, 0, 0.5)});
  }
  SECTION("intersect translated shape with a ray")
  {
    TestShape shape(sunray::Matrix44::translation(5, 0, 0));
    sunray::Ray ray{sunray::create_point(0, 0, -5), sunray::create_vector(0, 0, 1)};
    sunray::Intersections intersections;
    shape.is_intersected_by(ray, intersections);
    CHECK(shape.ray() == sunray::Ray{sunray::create_point(-5, 0, -5), sunray::create_vector(0, 0, 1)});
  }
}

TEST_CASE("shape normal method", "[shape]")
{
  SECTION("test normal translated")
  {
    TestShape shape(sunray::Matrix44::translation(0, 1, 0));
    CHECK(shape.normal_at(sunray::create_point(0, 1.70711, -0.70711)) == sunray::create_vector(0, 0.70711, -0.70711));
  }
  SECTION("test normal transformed")
  {
    sunray::Transformation trans;
    trans.rotate_z(sunray::PI / 5).scale(1, 0.5, 1);
    TestShape shape(trans.matrix());
    CHECK(shape.normal_at(sunray::create_point(0, sunray::sqrt(2) / 2, -sunray::sqrt(2) / 2)) ==
          sunray::create_vector(0, 0.97014, -0.24254));
  }
}
