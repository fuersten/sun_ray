//
//  diagnostic_message_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 28.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/diagnostic_message_handler.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("stream message level", "[diagnostic message level]")
{
  SECTION("stream error")
  {
    sunray::script::DiagnosticMessageLevel lvl{sunray::script::DiagnosticMessageLevel::ERROR_LVL};
    std::stringstream ss;
    ss << lvl;
    CHECK(ss.str() == "ERROR");
  }
  SECTION("stream warning")
  {
    sunray::script::DiagnosticMessageLevel lvl{sunray::script::DiagnosticMessageLevel::WARNING_LVL};
    std::stringstream ss;
    ss << lvl;
    CHECK(ss.str() == "WARNING");
  }
  SECTION("stream none")
  {
    sunray::script::DiagnosticMessageLevel lvl{sunray::script::DiagnosticMessageLevel::NONE};
    std::stringstream ss;
    ss << lvl;
    CHECK(ss.str() == "NONE");
  }
}


TEST_CASE("handle messages", "[diagnostic message handler]")
{
  SECTION("add error")
  {
    sunray::script::DiagnosticMessageHandler handler;
    handler.add_error("E0815", "This is the error message", sunray::script::Location(5, 17));
    CHECK(handler.has_error());
    CHECK_FALSE(handler.has_warning());
  }
  SECTION("add warning")
  {
    sunray::script::DiagnosticMessageHandler handler;
    handler.add_warning("W4711", "This is the warning message", sunray::script::Location(3, 5));
    CHECK_FALSE(handler.has_error());
    CHECK(handler.has_warning());
  }
  SECTION("reset")
  {
    sunray::script::DiagnosticMessageHandler handler;
    handler.add_warning("W4711", "This is the warning message", sunray::script::Location(3, 5));
    handler.add_error("E0815", "This is the error message", sunray::script::Location(5, 17));
    CHECK(handler.has_error());
    CHECK(handler.has_warning());
    handler.reset();
    CHECK_FALSE(handler.has_error());
    CHECK_FALSE(handler.has_warning());
  }
  SECTION("output")
  {
    sunray::script::DiagnosticMessageHandler handler;
    handler.add_warning("W4711", "This is the warning message", sunray::script::Location(3, 5));
    handler.add_error("E0815", "This is the error message", sunray::script::Location(5, 17));

    std::stringstream ss;
    handler.output_messages(ss);

    const std::string expected =
      "WARNING [W4711]: This is the warning message [3:5]\nERROR [E0815]: This is the error message [5:17]\n";
    CHECK(expected == ss.str());
  }
}
