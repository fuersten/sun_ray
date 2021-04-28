//
//  measurement_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 03.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/shapes/measurement.h>

#include <sstream>
#include <thread>

#include <catch2/catch.hpp>


TEST_CASE("measurement construction", "[measurement]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto measurement_meta_class = std::make_shared<sunray::script::MeasurementMetaClass>();
  registry.add_meta_class(measurement_meta_class);

  SECTION("construct measurement")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Measurement_constructor", 1));
    REQUIRE(idx != -1);
    auto measurement = function_registry.call_function(static_cast<size_t>(idx), {});
    REQUIRE(sunray::script::is_class(measurement));
  }
  SECTION("construct measurement error")
  {
    auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Measurement_constructor", 1));
    REQUIRE(idx != -1);
    CHECK_THROWS(function_registry.call_function(static_cast<size_t>(idx), {0.0, -0.1, 0.0, 1.0}));
  }
}

TEST_CASE("measurement methods", "[measurement]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry registry{function_registry};
  auto measurement_meta_class = std::make_shared<sunray::script::MeasurementMetaClass>();
  registry.add_meta_class(measurement_meta_class);
  auto measurement{measurement_meta_class->construct()};
  auto idx = function_registry.index_for_function(sunray::script::NameMangler::mangle("Measurement_elapsed_time", 1));

  SECTION("elapsed time")
  {
    std::this_thread::sleep_for(std::chrono::milliseconds{20});
    auto res = function_registry.call_function(static_cast<size_t>(idx), {measurement});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) > 19.0);
    std::this_thread::sleep_for(std::chrono::milliseconds{5});
    res = function_registry.call_function(static_cast<size_t>(idx), {measurement});
    REQUIRE(sunray::script::is_double(res));
    CHECK(sunray::script::as_double(res) > 4.9);
    // We actually cannot guarantee this, especially if building in the CI
    // CHECK(sunray::script::as_double(res) < 19.0);
  }
}

TEST_CASE("measurement stream", "[measurement]")
{
  auto measurement_meta_class = std::make_shared<sunray::script::MeasurementMetaClass>();

  SECTION("to string")
  {
    auto measurement{measurement_meta_class->construct()};
    CHECK(measurement->to_string() == "Measurement");
  }
}
