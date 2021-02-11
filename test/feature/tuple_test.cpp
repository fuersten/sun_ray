//
//  tuple_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 21.12.19.
//  Copyright © 2019 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/feature/tuple.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("create tuples", "[tuple]")
{
  SECTION("create empty tuple")
  {
    sunray::Tuple tuple;
    CHECK(tuple[0] == Approx(0.0));
    CHECK(tuple[1] == Approx(0.0));
    CHECK(tuple[2] == Approx(0.0));
    CHECK(tuple[3] == Approx(0.0));
  }
  SECTION("create tuple")
  {
    sunray::Tuple tuple{4.3, -4.2, 3.1, 1.0};
    CHECK(tuple[0] == Approx(4.3));
    CHECK(tuple[1] == Approx(-4.2));
    CHECK(tuple[2] == Approx(3.1));
    CHECK(tuple[3] == Approx(1.0));
  }
  SECTION("create tuple from array")
  {
    sunray::Tuple::Vec v{{4.3, -4.2, 3.1, 1.0}};
    sunray::Tuple tuple{v};
    CHECK(tuple[0] == Approx(4.3));
    CHECK(tuple[1] == Approx(-4.2));
    CHECK(tuple[2] == Approx(3.1));
    CHECK(tuple[3] == Approx(1.0));
  }
}

TEST_CASE("stream tuples", "[tuple]")
{
  SECTION("stream")
  {
    sunray::Tuple tuple{4.3, -4.2, 3.1, 1.0};
    std::stringstream ss;
    ss << tuple;
    CHECK("x: 4.3 y: -4.2 z: 3.1 w: 1" == ss.str());
  }
}

TEST_CASE("tuple components", "[tuple]")
{
  SECTION("stream")
  {
    sunray::Tuple tuple{4.3, -4.2, 3.1, 1.0};
    CHECK(Approx(4.3) == tuple.x());
    CHECK(Approx(-4.2) == tuple.y());
    CHECK(Approx(3.1) == tuple.z());
    CHECK(Approx(1.0) == tuple.w());
  }
}

TEST_CASE("assign tuples", "[tuple]")
{
  SECTION("assign")
  {
    sunray::Tuple tuple{4.3, -4.2, 3.1, 1.0};
    sunray::Tuple tuple2;
    tuple2 = tuple;
    CHECK(tuple == tuple2);
  }
  SECTION("assign move")
  {
    sunray::Tuple tuple{4.3, -4.2, 3.1, 1.0};
    sunray::Tuple tuple2;
    tuple2 = sunray::Tuple{4.3, -4.2, 3.1, 1.0};
    CHECK(tuple == tuple2);
  }
}

TEST_CASE("check tuples", "[tuple]")
{
  SECTION("check point")
  {
    sunray::Point point{4.3, -4.2, 3.1, 1.0};
    CHECK(sunray::is_point(point) == true);
    CHECK(sunray::is_vector(point) == false);
  }
  SECTION("check vector")
  {
    sunray::Vector vector{4.3, -4.2, 3.1, 0.0};
    CHECK(sunray::is_point(vector) == false);
    CHECK(sunray::is_vector(vector) == true);
  }
}

TEST_CASE("compare tuples", "[tuple]")
{
  sunray::Tuple tuple{4.3, -4.2, 3.1, 1.0};
  sunray::Tuple tuple2{4.3, -4.2, 3.1, 1.0};
  sunray::Tuple tuple3{-4.3, 4.2, -3.1, 1.0};

  SECTION("compare equal")
  {
    CHECK(tuple == tuple);
    CHECK(tuple == tuple2);
  }
  SECTION("compare not equal")
  {
    CHECK(tuple != tuple3);
    CHECK(tuple2 != tuple3);
  }
}

TEST_CASE("copy tuples", "[tuple]")
{
  SECTION("copy")
  {
    sunray::Tuple tuple{4.3, -4.2, 3.1, 1.0};
    sunray::Tuple tuple2{tuple};
    CHECK(tuple == tuple2);
    sunray::Tuple tuple3{std::move(tuple2)};
    CHECK(tuple == tuple3);
  }
  SECTION("move")
  {
    sunray::Tuple::Vec v{{4.3, -4.2, 3.1, 1.0}};
    // NOLINTNEXTLINE(performance-move-const-arg)
    sunray::Tuple tuple{std::move(v)};
    CHECK(tuple[0] == Approx(4.3));
    CHECK(tuple[1] == Approx(-4.2));
    CHECK(tuple[2] == Approx(3.1));
    CHECK(tuple[3] == Approx(1.0));
  }
}

TEST_CASE("negate tuples", "[tuple]")
{
  SECTION("negate tuple")
  {
    sunray::Tuple tuple(1, -2, 3, -4);
    CHECK(tuple.negate() == sunray::Tuple(-1, 2, -3, 4));
    CHECK(-tuple == sunray::Tuple(-1, 2, -3, 4));
  }
  SECTION("negate operator")
  {
    auto vector = sunray::create_vector(-2, 3, 1);
    CHECK(-vector == sunray::create_vector(2, -3, -1));
  }
  SECTION("negate")
  {
    auto vector = sunray::create_vector(-2, 3, 1);
    CHECK(vector.negate() == sunray::create_vector(2, -3, -1));
  }
}

TEST_CASE("add tuples", "[tuple]")
{
  SECTION("add tuple")
  {
    sunray::Tuple tuple(3, -2, 5, 1);
    sunray::Tuple tuple2(-2, 3, 1, 0);
    CHECK(tuple + tuple2 == sunray::Tuple(1, 1, 6, 1));
  }
  SECTION("add point to vector")
  {
    auto point = sunray::create_point(3, -2, 5);
    auto vector = sunray::create_vector(-2, 3, 1);

    auto t = point + vector;
    CHECK(sunray::is_point(t));
    CHECK(t == sunray::create_point(1, 1, 6));
  }
  SECTION("add vector to vector")
  {
    auto vector1 = sunray::create_vector(-2, 3, 1);
    auto vector2 = sunray::create_vector(3, -2, 5);

    auto t = vector1 + vector2;
    CHECK(sunray::is_vector(t));
    CHECK(t == sunray::create_vector(1, 1, 6));
    auto t1 = vector2 + vector1;
    CHECK(sunray::is_vector(t1));
    CHECK(t1 == sunray::create_vector(1, 1, 6));
  }
}

TEST_CASE("substract tuples", "[tuple]")
{
  SECTION("substract tuple")
  {
    sunray::Tuple tuple(3, 2, 1, 1);
    sunray::Tuple tuple2(5, 6, 7, 1);
    CHECK(tuple - tuple2 == sunray::Tuple(-2, -4, -6, 0));
  }
  SECTION("substract point from point")
  {
    auto point1 = sunray::create_point(3, 2, 1);
    auto point2 = sunray::create_point(5, 6, 7);

    auto t = point1 - point2;
    CHECK(sunray::is_vector(t));
    CHECK(t == sunray::create_vector(-2, -4, -6));
  }
  SECTION("substract vector from point")
  {
    auto point = sunray::create_point(3, 2, 1);
    auto vector = sunray::create_vector(5, 6, 7);

    auto t = point - vector;
    CHECK(sunray::is_point(t));
    CHECK(t == sunray::create_point(-2, -4, -6));
  }
  SECTION("substract vector from vector")
  {
    auto vector1 = sunray::create_vector(3, 2, 1);
    auto vector2 = sunray::create_vector(5, 6, 7);

    auto t = vector1 - vector2;
    CHECK(sunray::is_vector(t));
    CHECK(t == sunray::create_vector(-2, -4, -6));
  }
}

TEST_CASE("multiply tuples", "[tuple]")
{
  SECTION("multiply tuple")
  {
    sunray::Tuple tuple(1, -2, 3, -4);
    CHECK(tuple * 3.5f == sunray::Tuple(3.5, -7, 10.5, -14));
    CHECK(tuple * 0.5 == sunray::Tuple(0.5, -1, 1.5, -2));
  }
  SECTION("multiply vector")
  {
    auto vector = sunray::create_vector(1, -2, 3);
    CHECK(vector * 3.5f == sunray::create_vector(3.5, -7, 10.5));
    CHECK(vector * 0.5 == sunray::create_vector(0.5, -1, 1.5));
  }
}

TEST_CASE("devide tuples", "[tuple]")
{
  SECTION("devide tuple")
  {
    sunray::Tuple tuple(1, -2, 3, -4);
    CHECK(tuple / 2.0f == sunray::Tuple(0.5, -1, 1.5, -2));
  }
  SECTION("devide tuple by zero")
  {
    sunray::Tuple tuple(1, -2, 3, -4);
    CHECK_THROWS_AS(tuple / 0.0, std::invalid_argument);
  }
  SECTION("devide vector")
  {
    auto vector = sunray::create_vector(1, -2, 3);
    CHECK(vector / 2.0f == sunray::create_vector(0.5, -1, 1.5));
  }
}

TEST_CASE("vector magnitude", "[tuple]")
{
  SECTION("magnitude")
  {
    CHECK(Approx(5.47723) == sunray::Tuple(1, 2, 3, 4).magnitude());
  }
  SECTION("unit vector")
  {
    CHECK(Approx(1.0) == sunray::create_vector(1, 0, 0).magnitude());
    CHECK(Approx(1.0) == sunray::create_vector(0, 1, 0).magnitude());
    CHECK(Approx(1.0) == sunray::create_vector(0, 0, 1).magnitude());
  }
  SECTION("magnitude")
  {
    CHECK(Approx(sunray::sqrt(14)) == sunray::create_vector(1, 2, 3).magnitude());
    CHECK(Approx(sunray::sqrt(14)) == sunray::create_vector(-1, -2, -3).magnitude());
  }
}

TEST_CASE("vector normalize", "[tuple]")
{
  SECTION("normalize vector")
  {
    CHECK(sunray::create_vector(1, 0, 0) == sunray::create_vector(4, 0, 0).normalize());
    CHECK(sunray::create_vector(0, 1, 0) == sunray::create_vector(0, 4, 0).normalize());
    CHECK(sunray::create_vector(0, 0, 1) == sunray::create_vector(0, 0, 4).normalize());
    auto tmp = sunray::sqrt(14);
    CHECK(sunray::create_vector(1 / tmp, 2 / tmp, 3 / tmp) == sunray::create_vector(1, 2, 3).normalize());
    CHECK(Approx(1.0) == sunray::create_vector(1, 2, 3).normalize().magnitude());
  }
  SECTION("normalize tuple")
  {
    CHECK(sunray::Tuple(1, 0, 0, 0) == sunray::Tuple(4, 0, 0, 0).normalize());
    CHECK(sunray::Tuple(0, 1, 0, 0) == sunray::Tuple(0, 4, 0, 0).normalize());
    CHECK(sunray::Tuple(0, 0, 1, 0) == sunray::Tuple(0, 0, 4, 0).normalize());
    CHECK(sunray::Tuple(0, 0, 0, 1) == sunray::Tuple(0, 0, 0, 4).normalize());
  }
}

TEST_CASE("scalar product", "[tuple]")
{
  SECTION("scalar product tuple")
  {
    auto t1 = sunray::Tuple(1, 2, 3, 4);
    auto t2 = sunray::Tuple(2, 3, 4, 5);
    CHECK(Approx(40.0) == t1.scalarProduct(t2));
  }
  SECTION("scalar product vector")
  {
    auto t1 = sunray::create_vector(1, 2, 3);
    auto t2 = sunray::create_vector(2, 3, 4);
    CHECK(Approx(20.0) == t1.scalarProduct(t2));
  }
}

TEST_CASE("cross product", "[tuple]")
{
  SECTION("cross product vector")
  {
    auto t1 = sunray::create_vector(1, 2, 3);
    auto t2 = sunray::create_vector(2, 3, 4);
    CHECK(sunray::create_vector(-1, 2, -1) == t1.crossProduct(t2));
    CHECK(sunray::create_vector(1, -2, 1) == t2.crossProduct(t1));
  }
}

TEST_CASE("reflect vector", "[tuple]")
{
  SECTION("refelect vector at 45°")
  {
    auto v = sunray::create_vector(1, -1, 0);
    auto n = sunray::create_vector(0, 1, 0);
    CHECK(v.reflect(n) == sunray::create_vector(1, 1, 0));
  }
  SECTION("refelect vector off slanted surface")
  {
    auto v = sunray::create_vector(0, -1, 0);
    auto n = sunray::create_vector(sunray::sqrt(2) / 2, sunray::sqrt(2) / 2, 0);
    CHECK(v.reflect(n) == sunray::create_vector(1, 0, 0));
  }
}

TEST_CASE("tuple factory", "[tuple factory]")
{
  SECTION("create point")
  {
    auto point = sunray::create_point(-4.2, 4.3, 1.3);
    CHECK(point[0] == Approx(-4.2));
    CHECK(point[1] == Approx(4.3));
    CHECK(point[2] == Approx(1.3));
    CHECK(point[3] == Approx(1.0));
  }
  SECTION("create vector")
  {
    auto vector = sunray::create_vector(-4.2, 4.3, 1.3);
    CHECK(vector[0] == Approx(-4.2));
    CHECK(vector[1] == Approx(4.3));
    CHECK(vector[2] == Approx(1.3));
    CHECK(vector[3] == Approx(0.0));
  }
}
