//
//  stream_reader_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 22.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/stream_reader.h>

#include <ios>
#include <iostream>

#include <catch2/catch.hpp>


TEST_CASE("read stream", "[stream reader]")
{
  SECTION("read one line input")
  {
    const std::string input = "# Comment\r";
    std::istringstream is{input};
    sunray::StreamReader reader{is};
    CHECK(reader.current_char() == '#');
    CHECK(reader.current_line_count() == 1);
    CHECK(reader.current_column_count() == 1);
    CHECK(reader.next_char() == ' ');
    CHECK(reader.current_column_count() == 2);
    CHECK(reader.next_char() == 'C');
    CHECK(reader.current_column_count() == 3);
    CHECK(reader.next_char() == 'o');
    CHECK(reader.current_column_count() == 4);
    CHECK_FALSE(reader.is_eol());
    CHECK_FALSE(reader.is_eoi());
    CHECK(reader.next_char() == 'm');
    CHECK(reader.next_char() == 'm');
    CHECK(reader.safe_next_char() == 'e');
    CHECK(reader.next_char() == 'n');
    CHECK(reader.safe_next_char() == 't');
    CHECK_FALSE(reader.is_eol());
    CHECK(reader.next_char() == '\r');
    CHECK(reader.current_line_count() == 1);
    CHECK(reader.current_column_count() == 10);
    CHECK(reader.is_eol());
    CHECK(reader.next_char());
    CHECK(reader.is_eoi());
    CHECK_THROWS_AS(reader.safe_next_char(), std::exception);
  }
  SECTION("read multi line input")
  {
    const std::string input = "- light:\r  at: [-10, 10, -10]\r  intensity: [1, 0.84, 0]\r";
    std::istringstream is{input};
    sunray::StreamReader reader{is};
    CHECK(reader.current_column_count() == 1);
    CHECK(reader.current_char() == '-');
    CHECK(reader.safe_next_char() == ' ');
    CHECK(reader.current_column_count() == 2);
    CHECK(reader.safe_next_char() == 'l');
    CHECK(reader.current_line_count() == 1);
    reader.read_until_eol();
    CHECK(reader.current_column_count() == 1);
    CHECK(reader.current_char() == ' ');
    CHECK(reader.safe_next_char() == ' ');
    CHECK(reader.safe_next_char() == 'a');
    CHECK(reader.current_line_count() == 2);
    reader.read_until_eol();
    CHECK(reader.current_char() == ' ');
    CHECK(reader.safe_next_char() == ' ');
    CHECK(reader.safe_next_char() == 'i');
    CHECK(reader.current_line_count() == 3);
    reader.read_until_eol();
    CHECK(reader.current_line_count() == 4);
    reader.next_char();
    CHECK(reader.is_eoi());
  }
  SECTION("eat whitespace")
  {
    const std::string input = "   \t no whitespace";
    std::istringstream is{input};
    sunray::StreamReader reader{is};
    CHECK(reader.eat_whitespace() == 'n');
    CHECK(reader.safe_next_char() == 'o');
    CHECK(reader.safe_next_char() == ' ');
    CHECK(reader.safe_next_char() == 'w');
  }
  SECTION("bad stream")
  {
    std::istringstream is;
    is.setstate(std::ios::failbit);
    CHECK_THROWS_AS(sunray::StreamReader{is}, std::exception);
  }
}
