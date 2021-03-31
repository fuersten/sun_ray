//
//  stack_machine_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 29.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/build_in_functions.h>
#include <sun_ray/script/objects/color.h>
#include <sun_ray/script/stack_machine.h>

#include <sstream>

#include <catch2/catch.hpp>

using namespace std::string_literals;


TEST_CASE("check opcode", "[stack machine opcode]")
{
  SECTION("check opcode code")
  {
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::ADD;
      CHECK(ss.str() == "ADD");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::AND;
      CHECK(ss.str() == "AND");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::BEQ;
      CHECK(ss.str() == "BEQ");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::BNE;
      CHECK(ss.str() == "BNE");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::DIV;
      CHECK(ss.str() == "DIV");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::DROP;
      CHECK(ss.str() == "DROP");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::EQ;
      CHECK(ss.str() == "EQ");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::FUNC;
      CHECK(ss.str() == "FUNC");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::JMP;
      CHECK(ss.str() == "JMP");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::LE;
      CHECK(ss.str() == "LE");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::LT;
      CHECK(ss.str() == "LT");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::METHOD;
      CHECK(ss.str() == "METHOD");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::MUL;
      CHECK(ss.str() == "MUL");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::NEG;
      CHECK(ss.str() == "NEG");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::NEQ;
      CHECK(ss.str() == "NEQ");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::NOP;
      CHECK(ss.str() == "NOP");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::NOT;
      CHECK(ss.str() == "NOT");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::OR;
      CHECK(ss.str() == "OR");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::PUSH;
      CHECK(ss.str() == "PUSH");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::PUSHVAR;
      CHECK(ss.str() == "PUSHVAR");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::STOREVAR;
      CHECK(ss.str() == "STOREVAR");
    }
    {
      std::stringstream ss;
      ss << sunray::script::OpCode::SUB;
      CHECK(ss.str() == "SUB");
    }
  }
}

TEST_CASE("label", "[stack machine label]")
{
  SECTION("create label")
  {
    sunray::script::Label label{"l1"};

    CHECK(label.label() == "l1");
    CHECK(label.index() == std::numeric_limits<size_t>::max());
  }
  SECTION("label set index")
  {
    sunray::script::Label label{"l1"};
    label.set_index(4711);

    CHECK(label.label() == "l1");
    CHECK(label.index() == 4711);
  }
  SECTION("label compare")
  {
    sunray::script::Label label1{"l1"};
    sunray::script::Label label2{"l1"};

    CHECK(label1 == label2);
    CHECK_FALSE(label1 == sunray::script::Label{"l2"});
    label2.set_index(42);
    CHECK(label1 == label2);
  }
}

TEST_CASE("create instruction", "[stack machine instruction]")
{
  SECTION("create instruction with opcode")
  {
    sunray::script::Instruction inst{sunray::script::OpCode::STOREVAR};

    CHECK(inst.code_ == sunray::script::OpCode::STOREVAR);
    CHECK(inst.value_.index() == 0);
  }
  SECTION("create instruction with opcode and variant")
  {
    sunray::script::Instruction inst{sunray::script::OpCode::PUSH, sunray::script::Variant{4711.0}};

    CHECK(inst.code_ == sunray::script::OpCode::PUSH);
    CHECK(inst.value_.index() == 2);
    CHECK(std::holds_alternative<double>(inst.value_));
    CHECK(std::get<double>(inst.value_) == Approx(4711.0));
  }
}

TEST_CASE("assign instruction", "[stack machine instruction]")
{
  SECTION("assign move instruction")
  {
    sunray::script::Instruction inst{sunray::script::OpCode::STOREVAR};
    sunray::script::Instruction inst1{sunray::script::OpCode::STOREVAR};
    // NOLINTNEXTLINE(performance-move-const-arg)
    inst1 = std::move(inst);
  }
}

TEST_CASE("stack machine operations", "[stack machine]")
{
  std::stringstream ss;
  sunray::script::FunctionRegistry function_registry;
  sunray::script::BuildInFunctions(function_registry, ss);
  sunray::script::MetaClassRegistry meta_class_registry{function_registry};
  auto color_meta_class = std::make_shared<sunray::script::ColorMetaClass>();
  meta_class_registry.add_meta_class(color_meta_class);
  sunray::script::StackMachine sm{function_registry};

  auto idx = static_cast<double>(function_registry.index_for_function(sunray::script::NameMangler::mangle("print", 3)));

  SECTION("empty machine")
  {
    sm.run();
  }
  SECTION("add two strings")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, "Hello,"s));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, " world!"s));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::ADD));
    auto res = sm.run();
    CHECK(sunray::script::as_string(res) == "Hello, world!");
  }
  SECTION("add two doubles")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{47.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{13.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::ADD));
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(60));
  }
  SECTION("subtract two doubles")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{47.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{13.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::SUB));
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(34));
  }
  SECTION("multiply two doubles")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{2.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{13.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::MUL));
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(26));
  }
  SECTION("devide two doubles")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{42.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{2.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::DIV));
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(21));
  }
  SECTION("negate double")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{42.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::NEG));
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(-42));
  }
  SECTION("assign double")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{42.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::STOREVAR, 0));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSHVAR, 0));
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(42));
  }
  SECTION("assign string")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, "42"s));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::STOREVAR, 0));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSHVAR, 0));
    auto res = sm.run();
    CHECK(sunray::script::as_string(res) == "42");
  }
  SECTION("complex arithmetic")
  {
    // x = 42
    // y = 2
    // z = ((x + y) * 10) / 2 = 220
    uint32_t x = 0;
    uint32_t y = 1;
    uint32_t z = 2;
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{42.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::STOREVAR, x));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{2.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::STOREVAR, y));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{10.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSHVAR, x));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSHVAR, y));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::ADD));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::MUL));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{2.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::DIV));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::STOREVAR, z));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSHVAR, z));
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(220));
  }
  SECTION("call function")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{"world"s}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{"Hello"s}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{"{}, {}!"s}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{3.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{idx}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::FUNC));
    sm.run();
    CHECK(ss.str() == "Hello, world!");
  }
  SECTION("drop")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, "world"s));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, "Hello"s));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::DROP));
    auto res = sm.run();
    CHECK(sunray::script::as_string(res) == "world");
  }
  SECTION("method call read")
  {
    auto color{std::make_shared<sunray::script::Color>(color_meta_class, 0.9, 1.0, 0.5)};

    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, color));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, "get_red"s));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{0.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::METHOD));
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(0.9));
  }
  SECTION("method call write")
  {
    auto color{std::make_shared<sunray::script::Color>(color_meta_class, 0.9, 1.0, 0.5)};

    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{0.5}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, color));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, "set_red"s));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{1.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::METHOD));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, color));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, "get_red"s));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{0.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::METHOD));
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(0.5));
  }
  SECTION("compare less than false")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{7.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{8.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::LT));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == false);
  }
  SECTION("compare less than true")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{8.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{7.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::LT));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == true);
  }
  SECTION("compare less equal greater")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{7.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{8.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::LE));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == false);
  }
  SECTION("compare less equal smaller")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{8.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{7.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::LE));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == true);
  }
  SECTION("compare less equal equal")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{8.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{8.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::LE));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == true);
  }
  SECTION("compare not equal number")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{8.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{7.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::NEQ));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == true);
  }
  SECTION("compare not equal boolean")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{true}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{false}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::NEQ));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == true);
  }
  SECTION("compare equal number")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{8.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{8.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::EQ));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == true);
  }
  SECTION("compare equal boolean")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{true}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{true}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::EQ));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == true);
  }
  SECTION("relational or true")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{true}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{false}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::OR));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == true);
  }
  SECTION("relational or false")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{false}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{false}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::OR));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == false);
  }
  SECTION("relational and false")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{true}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{false}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::AND));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == false);
  }
  SECTION("relational and true")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{true}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{true}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::AND));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == true);
  }
  SECTION("relational not true")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{true}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::NOT));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == false);
  }
  SECTION("relational not false")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{false}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::NOT));
    auto res = sm.run();
    REQUIRE(sunray::script::is_bool(res));
    CHECK(sunray::script::as_bool(res) == true);
  }
}

TEST_CASE("stack machine flow control", "[stack machine]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::StackMachine sm{function_registry};

  SECTION("labels")
  {
    uint32_t n = 0;
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{3.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::STOREVAR, n));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSHVAR, n));
    sm.add_label(sunray::script::Label{"loop"});
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{1.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::SUB));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::STOREVAR, n));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSHVAR, n));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{0.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::LT));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::BEQ, sunray::script::Label{"loop"}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSHVAR, n));
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(0.0));
  }
  SECTION("bne false")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{false}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::BNE, sunray::script::Label{"l1"}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{1.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::JMP, sunray::script::Label{"l2"}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{0.0}));
    sm.add_label(sunray::script::Label{"l1"});
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::NEG));
    sm.add_label(sunray::script::Label{"l2"});
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(0.0));
  }
  SECTION("bne true")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{true}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::BNE, sunray::script::Label{"l1"}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{0.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::JMP, sunray::script::Label{"l2"}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{1.0}));
    sm.add_label(sunray::script::Label{"l1"});
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::NEG));
    sm.add_label(sunray::script::Label{"l2"});
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(0.0));
  }
  SECTION("beq false")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{false}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::BEQ, sunray::script::Label{"l1"}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{1.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::JMP, sunray::script::Label{"l2"}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{0.0}));
    sm.add_label(sunray::script::Label{"l1"});
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::NEG));
    sm.add_label(sunray::script::Label{"l2"});
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(-1.0));
  }
  SECTION("beq true")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{true}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::BEQ, sunray::script::Label{"l1"}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{0.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::JMP, sunray::script::Label{"l2"}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{1.0}));
    sm.add_label(sunray::script::Label{"l1"});
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::NEG));
    sm.add_label(sunray::script::Label{"l2"});
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(-1.0));
  }
  SECTION("jmp")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::JMP, sunray::script::Label{"l1"}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{1.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{0.0}));
    sm.add_label(sunray::script::Label{"l1"});
    auto res = sm.run();
    CHECK(sunray::script::as_double(res) == Approx(0.0));
  }
}

TEST_CASE("stack machine runs", "[stack machine]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::StackMachine sm{function_registry};

  SECTION("multiple runs")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, "Hello, world!"s));
    {
      auto res = sm.run();
      CHECK(sunray::script::as_string(res) == "Hello, world!");
    }
    {
      auto res = sm.run();
      CHECK(sunray::script::as_string(res) == "Hello, world!");
    }
  }
}

TEST_CASE("stack machine operations error", "[stack machine]")
{
  sunray::script::FunctionRegistry function_registry;
  sunray::script::MetaClassRegistry meta_class_registry{function_registry};
  sunray::script::StackMachine sm{function_registry};

  SECTION("wrong variant type")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, "42"s));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{2.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::DIV));
    CHECK_THROWS_AS(sm.run(), std::bad_variant_access);
  }
  SECTION("too few operands")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::NEG));
    CHECK_THROWS_WITH(sm.run(), "stack is empty");
  }
  SECTION("another too few operands")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::MUL));
    CHECK_THROWS_WITH(sm.run(), "stack is empty");
  }
  SECTION("drop on empty stack")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::DROP));
    CHECK_THROWS_WITH(sm.run(), "stack is empty");
  }
  SECTION("add label multiple times")
  {
    sm.add_label(sunray::script::Label{"l1"});
    CHECK_THROWS_WITH(sm.add_label(sunray::script::Label{"l1"}), "Label 'l1' already set");
  }
  SECTION("add with bool")
  {
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{2.0}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::PUSH, sunray::script::Variant{true}));
    sm.add_instruction(sunray::script::Instruction(sunray::script::OpCode::ADD));
    CHECK_THROWS_WITH(sm.run(), "Cannot add with bool");
  }
}
