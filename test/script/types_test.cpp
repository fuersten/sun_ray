//
//  types_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 28.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/objects/color.h>
#include <sun_ray/script/types.h>

#include <sstream>

#include <catch2/catch.hpp>

using namespace std::string_literals;


TEST_CASE("binary operator type", "[operator]")
{
  SECTION("check operator code")
  {
    {
      std::stringstream ss;
      ss << sunray::script::BinaryOperator::ADD;
      CHECK(ss.str() == "+");
    }
    {
      std::stringstream ss;
      ss << sunray::script::BinaryOperator::SUB;
      CHECK(ss.str() == "-");
    }
    {
      std::stringstream ss;
      ss << sunray::script::BinaryOperator::MUL;
      CHECK(ss.str() == "*");
    }
    {
      std::stringstream ss;
      ss << sunray::script::BinaryOperator::DIV;
      CHECK(ss.str() == "/");
    }
  }
}

TEST_CASE("unary operator type", "[operator]")
{
  SECTION("check unary operator code")
  {
    std::stringstream ss;
    ss << sunray::script::UnaryOperator::MINUS;
    CHECK(ss.str() == "-");
  }
}

TEST_CASE("conditional operator type", "[operator]")
{
  SECTION("check conditional operator code")
  {
    {
      std::stringstream ss;
      ss << sunray::script::ConditionalOperator::EQ;
      CHECK(ss.str() == "==");
    }
    {
      std::stringstream ss;
      ss << sunray::script::ConditionalOperator::GE;
      CHECK(ss.str() == ">=");
    }
    {
      std::stringstream ss;
      ss << sunray::script::ConditionalOperator::GT;
      CHECK(ss.str() == ">");
    }
    {
      std::stringstream ss;
      ss << sunray::script::ConditionalOperator::LE;
      CHECK(ss.str() == "<=");
    }
    {
      std::stringstream ss;
      ss << sunray::script::ConditionalOperator::LT;
      CHECK(ss.str() == "<");
    }
  }
}

TEST_CASE("logical operator type", "[operator]")
{
  SECTION("check logical operator code")
  {
    {
      std::stringstream ss;
      ss << sunray::script::LogicalOperator::AND;
      CHECK(ss.str() == "and");
    }
    {
      std::stringstream ss;
      ss << sunray::script::LogicalOperator::OR;
      CHECK(ss.str() == "or");
    }
  }
}

TEST_CASE("to_string variant", "[variant]")
{
  SECTION("to string variant double")
  {
    CHECK(sunray::script::to_string(sunray::script::Variant{4711.0}) == "4711.00");
  }
  SECTION("to string variant bool")
  {
    CHECK(sunray::script::to_string(sunray::script::Variant{true}) == "true");
    CHECK(sunray::script::to_string(sunray::script::Variant{false}) == "false");
  }
  SECTION("to string variant string")
  {
    CHECK(sunray::script::to_string(sunray::script::Variant{"my string"s}) == "my string");
  }
  SECTION("to string variant class")
  {
    auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
    auto color{std::make_shared<sunray::script::Color>(color_meta_class, 0.1, 0.647, 0.5)};
    CHECK(sunray::script::to_string(sunray::script::Variant{color}) == "<Color r: 0.1 g: 0.647 b: 0.5>");
  }
}

TEST_CASE("is type variant", "[variant]")
{
  SECTION("is variant double")
  {
    CHECK_FALSE(sunray::script::is_string(sunray::script::Variant{4711.0}));
    CHECK_FALSE(sunray::script::is_class(sunray::script::Variant{4711.0}));
    CHECK_FALSE(sunray::script::is_bool(sunray::script::Variant{4711.0}));
    CHECK(sunray::script::is_double(sunray::script::Variant{4711.0}));
  }
  SECTION("is variant bool")
  {
    CHECK_FALSE(sunray::script::is_string(sunray::script::Variant{true}));
    CHECK_FALSE(sunray::script::is_class(sunray::script::Variant{true}));
    CHECK_FALSE(sunray::script::is_double(sunray::script::Variant{true}));
    CHECK(sunray::script::is_bool(sunray::script::Variant{true}));
  }
  SECTION("is variant string")
  {
    CHECK_FALSE(sunray::script::is_double(sunray::script::Variant{"my string"s}));
    CHECK_FALSE(sunray::script::is_class(sunray::script::Variant{"my string"s}));
    CHECK_FALSE(sunray::script::is_bool(sunray::script::Variant{"my string"s}));
    CHECK(sunray::script::is_string(sunray::script::Variant{"my string"s}));
  }
  SECTION("is variant class")
  {
    auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
    auto color{std::make_shared<sunray::script::Color>(color_meta_class)};
    CHECK_FALSE(sunray::script::is_double(sunray::script::Variant{color}));
    CHECK_FALSE(sunray::script::is_string(sunray::script::Variant{color}));
    CHECK_FALSE(sunray::script::is_bool(sunray::script::Variant{color}));
    CHECK(sunray::script::is_class(sunray::script::Variant{color}));
  }
}

TEST_CASE("as type variant", "[variant]")
{
  SECTION("variant as double")
  {
    auto v = sunray::script::Variant{4711.0};
    CHECK(sunray::script::as_double(v) == Approx(4711.0));
    double& d = sunray::script::as_double(v);
    d = 8.15;
    CHECK(sunray::script::as_double(v) == Approx(8.15));

    CHECK_THROWS_AS(sunray::script::as_string(v), std::exception);
    CHECK_THROWS_AS(sunray::script::as_class(v), std::exception);
    CHECK_THROWS_AS(sunray::script::as_bool(v), std::exception);
  }
  SECTION("variant as bool")
  {
    auto v = sunray::script::Variant{true};
    CHECK(sunray::script::as_bool(v) == true);
    bool& b = sunray::script::as_bool(v);
    b = false;
    CHECK(sunray::script::as_bool(v) == false);

    CHECK_THROWS_AS(sunray::script::as_string(v), std::exception);
    CHECK_THROWS_AS(sunray::script::as_class(v), std::exception);
    CHECK_THROWS_AS(sunray::script::as_double(v), std::exception);
  }
  SECTION("variant as string")
  {
    auto v = sunray::script::Variant{"my string"s};
    CHECK(sunray::script::as_string(v) == "my string");
    std::string& s = sunray::script::as_string(v);
    s = "Hello, world!";
    CHECK(sunray::script::as_string(v) == "Hello, world!");

    CHECK_THROWS_AS(sunray::script::as_double(v), std::exception);
    CHECK_THROWS_AS(sunray::script::as_class(v), std::exception);
    CHECK_THROWS_AS(sunray::script::as_bool(v), std::exception);
  }
  SECTION("variant as class")
  {
    auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
    auto color{std::make_shared<sunray::script::Color>(color_meta_class)};
    auto v = sunray::script::Variant{color};
    CHECK(sunray::script::as_class(v) == color);

    CHECK_THROWS_AS(sunray::script::as_double(v), std::exception);
    CHECK_THROWS_AS(sunray::script::as_string(v), std::exception);
    CHECK_THROWS_AS(sunray::script::as_bool(v), std::exception);
  }
}

TEST_CASE("type from variant", "[variant]")
{
  SECTION("type from string")
  {
    sunray::script::Variant v{"my string"s};
    sunray::script::TypePtr type = sunray::script::type_from_variant(v);
    REQUIRE(type);
    REQUIRE(type->is_basic_type());
    auto basic_type = std::dynamic_pointer_cast<const sunray::script::BasicType>(type);
    REQUIRE(basic_type);
    CHECK_FALSE(type->is_class_type());
    CHECK(basic_type->type() == sunray::script::PODType::STRING);
    CHECK(basic_type->as_string() == "string");
  }
  SECTION("type from double")
  {
    sunray::script::Variant v{4711.0};
    sunray::script::TypePtr type = sunray::script::type_from_variant(v);
    REQUIRE(type);
    REQUIRE(type->is_basic_type());
    auto basic_type = std::dynamic_pointer_cast<const sunray::script::BasicType>(type);
    REQUIRE(basic_type);
    CHECK_FALSE(type->is_class_type());
    CHECK(basic_type->type() == sunray::script::PODType::DOUBLE);
    CHECK(basic_type->as_string() == "double");
  }
  SECTION("type from bool")
  {
    sunray::script::Variant v{true};
    sunray::script::TypePtr type = sunray::script::type_from_variant(v);
    REQUIRE(type);
    REQUIRE(type->is_basic_type());
    auto basic_type = std::dynamic_pointer_cast<const sunray::script::BasicType>(type);
    REQUIRE(basic_type);
    CHECK_FALSE(type->is_class_type());
    CHECK(basic_type->type() == sunray::script::PODType::BOOLEAN);
    CHECK(basic_type->as_string() == "bool");
  }
  SECTION("type from class")
  {
    auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
    auto color{std::make_shared<sunray::script::Color>(color_meta_class)};
    auto v = sunray::script::Variant{color};

    sunray::script::TypePtr type = sunray::script::type_from_variant(v);
    REQUIRE(type);
    CHECK_FALSE(type->is_basic_type());
    CHECK(type->is_class_type());
    auto class_type = std::dynamic_pointer_cast<const sunray::script::ClassType>(type);
    REQUIRE(class_type);
    CHECK(class_type->type() == "Color");
    CHECK(class_type->as_string() == "Color");
  }
}

TEST_CASE("unwrap variants", "[unwrap]")
{
  SECTION("unwrap double")
  {
    sunray::script::Variant v{47.11};
    CHECK(sunray::script::unwrap<double>::as_type(v) == Approx(47.11));
  }
  SECTION("unwrap bool")
  {
    sunray::script::Variant v{true};
    CHECK(sunray::script::unwrap<bool>::as_type(v) == true);
  }
  SECTION("unwrap string")
  {
    sunray::script::Variant v{"hutzli"s};
    CHECK(sunray::script::unwrap<std::string>::as_type(v) == "hutzli");
  }
  SECTION("unwrap const string")
  {
    sunray::script::Variant v{"hutzli"s};
    CHECK(sunray::script::unwrap<const std::string&>::as_type(v) == "hutzli");
  }
  SECTION("unwrap variant")
  {
    sunray::script::Variant v{"hutzli"s};
    CHECK(sunray::script::unwrap<sunray::script::Variant>::as_type(v) == sunray::script::Variant{"hutzli"s});
  }
  SECTION("unwrap const variant")
  {
    sunray::script::Variant v{"hutzli"s};
    CHECK(sunray::script::unwrap<const sunray::script::Variant&>::as_type(v) == sunray::script::Variant{"hutzli"s});
  }
  SECTION("unwrap char")
  {
    sunray::script::Variant v{"hutzli"s};
    CHECK(sunray::script::unwrap<const char*>::as_type(v) == "hutzli");
  }
  SECTION("unwrap class")
  {
    auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
    auto color{std::make_shared<sunray::script::Color>(color_meta_class)};
    auto v = sunray::script::Variant{color};

    CHECK(sunray::script::unwrap<sunray::script::MutableClassPtr&>::as_type(v) == color);
  }
  SECTION("unwrap const class")
  {
    auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
    auto color{std::make_shared<sunray::script::Color>(color_meta_class)};
    auto v = sunray::script::Variant{color};

    CHECK(sunray::script::unwrap<const sunray::script::MutableClassPtr&>::as_type(v) == color);
  }
  SECTION("unwrap unkown")
  {
    struct unkown {
    };
    sunray::script::Variant v{"hutzli"s};
    CHECK_THROWS(sunray::script::unwrap<unkown>::as_type(v));
  }
}

TEST_CASE("unwrap variant type", "[unwrap]")
{
  SECTION("unwrap double")
  {
    sunray::script::Variant v{47.11};
    CHECK(sunray::script::unwrap<double>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<std::string>::is_type(v));
  }
  SECTION("unwrap bool")
  {
    sunray::script::Variant v{true};
    CHECK(sunray::script::unwrap<bool>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<std::string>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<double>::is_type(v));
  }
  SECTION("unwrap string")
  {
    sunray::script::Variant v{"hutzli"s};
    CHECK(sunray::script::unwrap<std::string>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<double>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<bool>::is_type(v));
  }
  SECTION("unwrap const string")
  {
    sunray::script::Variant v{"hutzli"s};
    CHECK(sunray::script::unwrap<const std::string&>::is_type(v));
    CHECK(sunray::script::unwrap<std::string>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<double>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<bool>::is_type(v));
  }
  SECTION("unwrap variant")
  {
    sunray::script::Variant v{"hutzli"s};
    CHECK(sunray::script::unwrap<sunray::script::Variant>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<double>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<bool>::is_type(v));
  }
  SECTION("unwrap const variant")
  {
    sunray::script::Variant v{"hutzli"s};
    CHECK(sunray::script::unwrap<const sunray::script::Variant&>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<double>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<bool>::is_type(v));
  }
  SECTION("unwrap char")
  {
    sunray::script::Variant v{"hutzli"s};
    CHECK(sunray::script::unwrap<const char*>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<bool>::is_type(v));
  }
  SECTION("unwrap class")
  {
    auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
    auto color{std::make_shared<sunray::script::Color>(color_meta_class)};
    auto v = sunray::script::Variant{color};

    CHECK(sunray::script::unwrap<sunray::script::MutableClassPtr&>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<bool>::is_type(v));
  }
  SECTION("unwrap const class")
  {
    auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
    auto color{std::make_shared<sunray::script::Color>(color_meta_class)};
    auto v = sunray::script::Variant{color};

    CHECK(sunray::script::unwrap<const sunray::script::MutableClassPtr&>::is_type(v));
    CHECK_FALSE(sunray::script::unwrap<bool>::is_type(v));
  }
  SECTION("unwrap unkown")
  {
    struct unkown {
    };
    sunray::script::Variant v{"hutzli"s};
    CHECK_THROWS(sunray::script::unwrap<unkown>::is_type(v));
  }
}
