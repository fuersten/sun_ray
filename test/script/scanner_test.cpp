//
//  scanner_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 02.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/scanner.h>

#include <iostream>
#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("scan primary", "[scanner]")
{
  SECTION("scan integer")
  {
    const std::string input = "42";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    REQUIRE(sunray::script::is_double(tok.value_));
    CHECK(sunray::script::as_double(tok.value_) == Approx(42));
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan float 0")
  {
    const std::string input = "0";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    REQUIRE(sunray::script::is_double(tok.value_));
    CHECK(sunray::script::as_double(tok.value_) == Approx(0));
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan float 0.0")
  {
    const std::string input = "0.0";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    REQUIRE(sunray::script::is_double(tok.value_));
    CHECK(sunray::script::as_double(tok.value_) == Approx(0));
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan float 0.67")
  {
    const std::string input = "0.67";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    REQUIRE(sunray::script::is_double(tok.value_));
    CHECK(sunray::script::as_double(tok.value_) == Approx(0.67));
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan float")
  {
    const std::string input = "47.11";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    REQUIRE(sunray::script::is_double(tok.value_));
    CHECK(sunray::script::as_double(tok.value_) == Approx(47.11));
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan float scientifical notation positive exponent")
  {
    const std::string input = "2.00600e+003";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    REQUIRE(sunray::script::is_double(tok.value_));
    CHECK(sunray::script::as_double(tok.value_) == Approx(2006.00));
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan float scientifical notation negative exponent")
  {
    const std::string input = "2.00600e-002";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    REQUIRE(sunray::script::is_double(tok.value_));
    CHECK(sunray::script::as_double(tok.value_) == Approx(0.02006));
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan identifier")
  {
    const std::string input = "hutzli";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::IDENTIFIER);
    CHECK(tok.identifier_ == "hutzli");
  }
  SECTION("scan identifier with special chars")
  {
    const std::string input = "x_2";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::IDENTIFIER);
    CHECK(tok.identifier_ == "x_2");
  }
  SECTION("scan typename")
  {
    const std::string input = "Color";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::TYPENAME);
    CHECK(tok.identifier_ == "Color");
  }
  SECTION("scan quoted string")
  {
    const std::string input = "'putzli'";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    REQUIRE(sunray::script::is_string(tok.value_));
    CHECK(sunray::script::as_string(tok.value_) == "putzli");
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan quoted string with escape characters")
  {
    const std::string input = R"('\'p\\ut\tz\bl\fi\n\r\'')";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    REQUIRE(sunray::script::is_string(tok.value_));
    CHECK(sunray::script::as_string(tok.value_) == "'p\\ut\tz\bl\fi\n\r'");
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan boolean true")
  {
    const std::string input = "true";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    REQUIRE(sunray::script::is_bool(tok.value_));
    CHECK(sunray::script::as_bool(tok.value_) == true);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan boolean false")
  {
    const std::string input = "false";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    REQUIRE(sunray::script::is_bool(tok.value_));
    CHECK(sunray::script::as_bool(tok.value_) == false);
    CHECK(tok.identifier_.empty());
  }
}

TEST_CASE("scan line", "[scanner]")
{
  SECTION("scan comment")
  {
    const std::string input = "# this is poo\r";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::EOI);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan endlines")
  {
    const std::string input = "\r\n\r";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::EOI);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan eoi")
  {
    const std::string input = "";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::EOI);
    CHECK(tok.identifier_.empty());
  }
}

TEST_CASE("scan operators", "[scanner]")
{
  SECTION("scan plus")
  {
    const std::string input = "+";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::PLUS);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan minus")
  {
    const std::string input = "-";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::MINUS);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan mul")
  {
    const std::string input = "*";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::MUL);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan div")
  {
    const std::string input = "/";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::DIV);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan assign")
  {
    const std::string input = "=";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::ASSIGNMENT);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan lparen")
  {
    const std::string input = "(";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::LPAREN);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan rparen")
  {
    const std::string input = ")";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::RPAREN);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan comma")
  {
    const std::string input = ",";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::COMMA);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan dot")
  {
    const std::string input = ".";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::DOT);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan less")
  {
    const std::string input = "<";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::LT);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan greater")
  {
    const std::string input = ">";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::GT);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan less equal")
  {
    const std::string input = "<=";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::LE);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan greater equal")
  {
    const std::string input = ">=";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::GE);
    CHECK(tok.identifier_.empty());
  }
  SECTION("scan equal")
  {
    const std::string input = "==";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::EQ);
    CHECK(tok.identifier_.empty());
  }
}

TEST_CASE("scan keywords", "[scanner]")
{
  SECTION("if")
  {
    const std::string input = "if";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::IF);
    CHECK(tok.identifier_.empty());
  }
  SECTION("end")
  {
    const std::string input = "end";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::END);
    CHECK(tok.identifier_.empty());
  }
  SECTION("while")
  {
    const std::string input = "while";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::WHILE);
    CHECK(tok.identifier_.empty());
  }
  SECTION("and")
  {
    const std::string input = "and";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::AND);
    CHECK(tok.identifier_.empty());
  }
  SECTION("or")
  {
    const std::string input = "or";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::OR);
    CHECK(tok.identifier_.empty());
  }
  SECTION("not")
  {
    const std::string input = "not";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::NOT);
    CHECK(tok.identifier_.empty());
  }
}

TEST_CASE("scan sequences", "[scanner]")
{
  SECTION("if end")
  {
    const std::string input = "if (x > 0) x = 1 end";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::IF);

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::LPAREN);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::IDENTIFIER);
    CHECK(tok.identifier_ == "x");

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::GT);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    CHECK(tok.identifier_.empty());
    REQUIRE(sunray::script::is_double(tok.value_));
    CHECK(sunray::script::as_double(tok.value_) == Approx(0));

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::RPAREN);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::IDENTIFIER);
    CHECK(tok.identifier_ == "x");

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::ASSIGNMENT);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    CHECK(tok.identifier_.empty());
    REQUIRE(sunray::script::is_double(tok.value_));
    CHECK(sunray::script::as_double(tok.value_) == Approx(1));

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::END);
    CHECK(tok.identifier_.empty());
  }
  SECTION("assign typename")
  {
    const std::string input = "orange = Color(1, 0.647, 0)";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::IDENTIFIER);
    CHECK(tok.identifier_ == "orange");

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::ASSIGNMENT);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::TYPENAME);
    CHECK(tok.identifier_ == "Color");

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::LPAREN);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::COMMA);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::COMMA);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::CONST_LITERAL);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::RPAREN);
    CHECK(tok.identifier_.empty());
  }
  SECTION("get property")
  {
    const std::string input = "red = orange.red";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    auto tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::IDENTIFIER);
    CHECK(tok.identifier_ == "red");

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::ASSIGNMENT);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::IDENTIFIER);
    CHECK(tok.identifier_ == "orange");

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::DOT);
    CHECK(tok.identifier_.empty());

    tok = scanner.get_next_token();
    CHECK(tok.code_ == sunray::script::TokenCode::IDENTIFIER);
    CHECK(tok.identifier_ == "red");
  }
}

TEST_CASE("scan error", "[scanner]")
{
  SECTION("scan error char")
  {
    const std::string input = "%";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    CHECK_THROWS_WITH(scanner.get_next_token(), "scanning unknown char '%'");
  }
  SECTION("scan error unkown escape")
  {
    const std::string input = "'test\\ztest'";
    std::istringstream is{input};
    sunray::script::Scanner scanner{is};

    CHECK_THROWS_WITH(scanner.get_next_token(), "illegal escape sequence '\\z' found");
  }
}
