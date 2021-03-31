//
//  dump_visitor_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 31.03.21.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/build_in_functions.h>
#include <sun_ray/script/parser.h>
#include <sun_ray/script/visitor/dump_visitor.h>

#include <sstream>

#include <catch2/catch.hpp>

namespace
{
  std::string dump_script(const std::string& input)
  {
    sunray::script::DiagnosticMessageHandler diagnostic_messages;
    sunray::script::Parser parser{diagnostic_messages};

    std::istringstream is{input};
    auto statements = parser.parse(is);
    REQUIRE(!diagnostic_messages.has_error());
    REQUIRE(!statements.empty());

    std::stringstream output;
    sunray::script::DumpVisitor visitor{output};
    visitor.visit(statements);
    return output.str();
  }
}

TEST_CASE("dump visitor", "[dump visitor]")
{
  SECTION("dump simple script")
  {
    auto script = dump_script(R"(
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

      println('n => {} = {}', n, res)
)");

    auto expected = R"(n = 90.00
res = 0.00
if (n == 0.00)
  res = 0.00
end
if (n == 1.00)
  res = 1.00
end
if (n > 1.00)
  a = 1.00
  b = 1.00
  count = 2.00
  res = 1.00
  while (count < n)
    res = (a + b)
    a = b
    b = res
    count = (count + 1.00)
  end
end
println('n => {} = {}', n, res)
)";

    CHECK(script == expected);
  }
  SECTION("dump class script")
  {
    auto script = dump_script(R"(
      material = Material()
      material.ambient = 0.3
      material.diffuse = 0.5
      material.specular = 0.5
      material.shininess = 100
      material.set_reflective(0.3)
      material.pattern = CheckerPattern(Color(1, 0.75, 0.35), Color(0.35, 0.75, 1)))");

    auto expected = R"(material = Material()
material.ambient = 0.30
material.diffuse = 0.50
material.specular = 0.50
material.shininess = 100.00
material.set_reflective(0.30)
material.pattern = CheckerPattern(Color(1.00, 0.75, 0.35), Color(0.35, 0.75, 1.00))
)";
    CHECK(script == expected);
  }
  SECTION("dump condition script")
  {
    auto script = dump_script(R"(
      n = 0
      if(true or false) n = 1 end
      if(not true) n = 2 end)");

    auto expected = R"(n = 0.00
if (true or false)
  n = 1.00
end
if (not true)
  n = 2.00
end
)";
    CHECK(script == expected);
  }
}
