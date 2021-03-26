//
//  language_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 26.03.21.
//  Copyright © 2021 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/build_in_functions.h>
#include <sun_ray/script/parser.h>
#include <sun_ray/script/visitor/stack_machine_visitor.h>

#include <sstream>

#include <catch2/catch.hpp>

namespace
{
  std::tuple<sunray::script::Variant, std::string> process(const std::string& input)
  {
    std::stringstream output;
    sunray::script::DiagnosticMessageHandler diagnostic_messages;
    sunray::script::Parser parser{diagnostic_messages};
    sunray::script::FunctionRegistry function_registry;
    sunray::script::BuildInFunctions buildin_functions{function_registry, output};
    sunray::script::MetaClassRegistry meta_class_registry{function_registry};
    sunray::script::StackMachineVisitor visitor{function_registry, meta_class_registry};

    std::istringstream is{input};

    auto statements = parser.parse(is);
    REQUIRE(!statements.empty());

    visitor.visit(statements);
    auto sm = visitor.machine();
    return std::make_tuple(sm.run(), output.str());
  }
}

TEST_CASE("language", "[language]")
{
  SECTION("simple")
  {
    auto [result, output] = process("x = 1 x");

    CHECK(sunray::script::is_double(result));
    CHECK(sunray::script::as_double(result) == Approx(1.0));
  }
  SECTION("complex")
  {
    const auto program = R"(
    n = 90
    res = 0

    if (n == 0)
      res = 0
    end
    if (n == 1)
      res = 1
    end

    if (n > 1)
      a = 1
      b = 1
      count = 2
      res = 1
      while(count < n)
        res = a + b
        a = b
        b = res
        count = count + 1
      end
    end
    res
    )";
    auto [result, output] = process(program);

    CHECK(sunray::script::is_double(result));
    CHECK(sunray::script::as_double(result) == Approx(2880067194370816000.0));
  }
  SECTION("if with equals")
  {
    const auto program = R"(
    n = false
    res = 0

    if (n <> true)
      res = 1
    end

    n = 42
    if (n == 42)
      res = 2
    end
    if (n <> 42)
      res = 3
    end
    res
    )";
    auto [result, output] = process(program);

    CHECK(sunray::script::is_double(result));
    CHECK(sunray::script::as_double(result) == Approx(2.0));
  }
}
