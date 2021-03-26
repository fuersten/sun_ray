//
//  stack_machine_visitor_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 23.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/build_in_functions.h>
#include <sun_ray/script/objects/canvas.h>
#include <sun_ray/script/objects/color.h>
#include <sun_ray/script/parser.h>
#include <sun_ray/script/visitor/stack_machine_visitor.h>

#include <sstream>

#include <catch2/catch.hpp>

namespace
{
  sunray::script::Instructions get_instructions(const std::string& input)
  {
    std::stringstream output;
    sunray::script::DiagnosticMessageHandler diagnostic_messages;
    sunray::script::Parser parser{diagnostic_messages};
    sunray::script::FunctionRegistry function_registry;
    sunray::script::BuildInFunctions buildin_functions{function_registry, output};
    sunray::script::MetaClassRegistry meta_class_registry{function_registry};
    meta_class_registry.add_meta_class(std::make_shared<sunray::script::CanvasMetaClass>());
    meta_class_registry.add_meta_class(std::make_shared<sunray::script::ColorMetaClass>());
    sunray::script::StackMachineVisitor visitor{function_registry, meta_class_registry};

    std::istringstream is{input};

    auto statements = parser.parse(is);
    REQUIRE(!statements.empty());

    visitor.visit(statements);
    auto sm = visitor.machine();
    return sm.instructions();
  }
}

TEST_CASE("visit", "[stack machine visitor]")
{
  SECTION("drop function return value")
  {
    auto instructions = get_instructions("x = 1 print(x)");
    REQUIRE(instructions.size() == 7);
    CHECK(instructions[6].code_ == sunray::script::OpCode::DROP);
  }
  SECTION("while with method call")
  {
    auto instructions = get_instructions(
      "color = Color(1, 1, 1) canvas = Canvas(10, 10) n = 0 while(n < 10) canvas.set_pixel(n, 1, color) n = n + 1 end");
    REQUIRE(instructions.size() == 34);
  }
}

TEST_CASE("visit parts", "[stack machine visitor]")
{
  SECTION("literal")
  {
    auto instructions = get_instructions("42");
    REQUIRE(instructions.size() == 1);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(42));
  }
  SECTION("assignment")
  {
    auto instructions = get_instructions("x = 42");
    REQUIRE(instructions.size() == 2);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(42));
    CHECK(instructions[1].code_ == sunray::script::OpCode::STOREVAR);
    CHECK(instructions[1].index_.value() == 0);
  }
  SECTION("addition")
  {
    auto instructions = get_instructions("1 + 42");
    REQUIRE(instructions.size() == 3);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(1));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(42));
    CHECK(instructions[2].code_ == sunray::script::OpCode::ADD);
  }
  SECTION("subtration")
  {
    auto instructions = get_instructions("1 - 42");
    REQUIRE(instructions.size() == 3);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(1));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(42));
    CHECK(instructions[2].code_ == sunray::script::OpCode::SUB);
  }
  SECTION("multiplication")
  {
    auto instructions = get_instructions("1 * 42");
    REQUIRE(instructions.size() == 3);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(1));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(42));
    CHECK(instructions[2].code_ == sunray::script::OpCode::MUL);
  }
  SECTION("division")
  {
    auto instructions = get_instructions("1 / 42");
    REQUIRE(instructions.size() == 3);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(1));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(42));
    CHECK(instructions[2].code_ == sunray::script::OpCode::DIV);
  }
  SECTION("and")
  {
    auto instructions = get_instructions("true and false");
    REQUIRE(instructions.size() == 3);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_bool(instructions[0].value_));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK_FALSE(sunray::script::as_bool(instructions[1].value_));
    CHECK(instructions[2].code_ == sunray::script::OpCode::AND);
  }
  SECTION("or")
  {
    auto instructions = get_instructions("true or false");
    REQUIRE(instructions.size() == 3);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_bool(instructions[0].value_));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK_FALSE(sunray::script::as_bool(instructions[1].value_));
    CHECK(instructions[2].code_ == sunray::script::OpCode::OR);
  }
  SECTION("unary neg")
  {
    auto instructions = get_instructions("-4711");
    REQUIRE(instructions.size() == 2);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(4711));
    CHECK(instructions[1].code_ == sunray::script::OpCode::NEG);
  }
  SECTION("unary not")
  {
    auto instructions = get_instructions("not true");
    REQUIRE(instructions.size() == 2);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_bool(instructions[0].value_));
    CHECK(instructions[1].code_ == sunray::script::OpCode::NOT);
  }
  SECTION("less")
  {
    auto instructions = get_instructions("42 < 815");
    REQUIRE(instructions.size() == 3);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(815));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(42));
    CHECK(instructions[2].code_ == sunray::script::OpCode::LT);
  }
  SECTION("less equal")
  {
    auto instructions = get_instructions("42 <= 815");
    REQUIRE(instructions.size() == 3);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(815));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(42));
    CHECK(instructions[2].code_ == sunray::script::OpCode::LE);
  }
  SECTION("greater")
  {
    auto instructions = get_instructions("42 > 815");
    REQUIRE(instructions.size() == 3);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(42));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(815));
    CHECK(instructions[2].code_ == sunray::script::OpCode::LT);
  }
  SECTION("greater equal")
  {
    auto instructions = get_instructions("42 >= 815");
    REQUIRE(instructions.size() == 3);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(42));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(815));
    CHECK(instructions[2].code_ == sunray::script::OpCode::LE);
  }
  SECTION("equal")
  {
    auto instructions = get_instructions("42 == 815");
    REQUIRE(instructions.size() == 3);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(42));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(815));
    CHECK(instructions[2].code_ == sunray::script::OpCode::EQ);
  }
  SECTION("not equal")
  {
    auto instructions = get_instructions("42 <> 815");
    REQUIRE(instructions.size() == 3);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(42));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(815));
    CHECK(instructions[2].code_ == sunray::script::OpCode::NEQ);
  }
  SECTION("if")
  {
    auto instructions = get_instructions("if (42 < 815) 42 end");
    REQUIRE(instructions.size() == 6);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(815));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(42));
    CHECK(instructions[2].code_ == sunray::script::OpCode::LT);
    CHECK(instructions[3].code_ == sunray::script::OpCode::BNE);
    REQUIRE(instructions[3].label_);
    CHECK(instructions[3].label_->label() == "l1");
    CHECK(sunray::script::as_double(instructions[4].value_) == Approx(42));
    CHECK(instructions[5].code_ == sunray::script::OpCode::NOP);
  }
  SECTION("while")
  {
    auto instructions = get_instructions("while (42 < 815) 42 end");
    REQUIRE(instructions.size() == 8);
    CHECK(instructions[0].code_ == sunray::script::OpCode::NOP);
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(815));
    CHECK(instructions[2].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[2].value_) == Approx(42));
    CHECK(instructions[3].code_ == sunray::script::OpCode::LT);
    CHECK(instructions[4].code_ == sunray::script::OpCode::BNE);
    REQUIRE(instructions[4].label_);
    CHECK(instructions[4].label_->label() == "l2");
    CHECK(sunray::script::as_double(instructions[5].value_) == Approx(42));
    CHECK(instructions[6].code_ == sunray::script::OpCode::JMP);
    REQUIRE(instructions[6].label_);
    CHECK(instructions[6].label_->label() == "l1");
    CHECK(instructions[7].code_ == sunray::script::OpCode::NOP);
  }
  SECTION("func")
  {
    auto instructions = get_instructions("print('{}', 4711)");
    REQUIRE(instructions.size() == 6);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(4711));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_string(instructions[1].value_) == "{}");
    CHECK(instructions[2].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[2].value_) == Approx(2));
    CHECK(instructions[3].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[3].value_) == Approx(20));
    CHECK(instructions[4].code_ == sunray::script::OpCode::FUNC);
    CHECK(instructions[5].code_ == sunray::script::OpCode::DROP);
  }
  SECTION("constructor")
  {
    auto instructions = get_instructions("color = Color(1, 1, 1)");
    REQUIRE(instructions.size() == 7);
    CHECK(instructions[0].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[0].value_) == Approx(1));
    CHECK(instructions[1].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[1].value_) == Approx(1));
    CHECK(instructions[2].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[2].value_) == Approx(1));
    CHECK(instructions[3].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[3].value_) == Approx(3));
    CHECK(instructions[4].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[4].value_) == Approx(39));
    CHECK(instructions[5].code_ == sunray::script::OpCode::FUNC);
    CHECK(instructions[6].code_ == sunray::script::OpCode::STOREVAR);
    CHECK(instructions[6].index_.value() == 0);
  }
  SECTION("property")
  {
    auto instructions = get_instructions("color = Color(1, 1, 1) color.r = 0.5");
    REQUIRE(instructions.size() == 12);
    CHECK(instructions[7].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[7].value_) == Approx(0.5));
    CHECK(instructions[8].code_ == sunray::script::OpCode::PUSHVAR);
    CHECK(instructions[8].index_.value() == 0);
    CHECK(instructions[9].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_string(instructions[9].value_) == "set_r");
    CHECK(instructions[10].code_ == sunray::script::OpCode::PUSH);
    CHECK(sunray::script::as_double(instructions[10].value_) == Approx(1));
    CHECK(instructions[11].code_ == sunray::script::OpCode::METHOD);
  }
}

TEST_CASE("smv error", "[stack machine visitor]")
{
  SECTION("unknown function")
  {
    CHECK_THROWS_WITH(get_instructions("doit(4711)"), "Cannot find function 'doit' in registry (_SR4doit1)");
  }
  SECTION("unknown class")
  {
    CHECK_THROWS_WITH(get_instructions("UnknownClass(4711)"), "Cannot find class 'UnknownClass' in class registry");
  }
}
