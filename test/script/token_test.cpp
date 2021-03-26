//
//  token_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 28.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/token.h>

#include <sstream>

#include <catch2/catch.hpp>


TEST_CASE("create token", "[token]")
{
  SECTION("create default token")
  {
    sunray::script::Token token;

    CHECK(token.code_ == sunray::script::TokenCode::UNDEFINED);
    CHECK(token.location_ == sunray::script::Location{});
    CHECK(std::get_if<double>(&token.value_) == nullptr);
    CHECK(token.identifier_.empty());
  }
  SECTION("create token")
  {
    sunray::script::Token token{sunray::script::TokenCode::IDENTIFIER};
    token.location_ = sunray::script::Location{5, 6};

    CHECK(token.code_ == sunray::script::TokenCode::IDENTIFIER);
    CHECK(token.location_ == sunray::script::Location{5, 6});
  }
}

TEST_CASE("token operator", "[token]")
{
  SECTION("compare token")
  {
    sunray::script::Token token{sunray::script::TokenCode::IDENTIFIER};

    CHECK(token == sunray::script::TokenCode::IDENTIFIER);
    CHECK_FALSE(token == sunray::script::TokenCode::ASSIGNMENT);

    CHECK_FALSE(token != sunray::script::TokenCode::IDENTIFIER);
    CHECK(token != sunray::script::TokenCode::ASSIGNMENT);
  }
}


TEST_CASE("check token code", "[token code]")
{
  SECTION("check token code")
  {
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::UNDEFINED;
      CHECK(ss.str() == "unexpected character sequence");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::EOI;
      CHECK(ss.str() == "eoi");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::CONST_LITERAL;
      CHECK(ss.str() == "float");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::ASSIGNMENT;
      CHECK(ss.str() == "assignment");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::PLUS;
      CHECK(ss.str() == "plus");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::MINUS;
      CHECK(ss.str() == "minus");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::MUL;
      CHECK(ss.str() == "mult");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::DIV;
      CHECK(ss.str() == "div");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::DOT;
      CHECK(ss.str() == "dot");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::LPAREN;
      CHECK(ss.str() == "left paren");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::RPAREN;
      CHECK(ss.str() == "right paren");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::LSQUARE;
      CHECK(ss.str() == "left square");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::RSQUARE;
      CHECK(ss.str() == "right square");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::IDENTIFIER;
      CHECK(ss.str() == "identifier");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::COMMA;
      CHECK(ss.str() == "comma");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::TYPENAME;
      CHECK(ss.str() == "typename");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::LT;
      CHECK(ss.str() == "less than");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::GT;
      CHECK(ss.str() == "greater than");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::LE;
      CHECK(ss.str() == "less equal");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::GE;
      CHECK(ss.str() == "greater equal");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::EQ;
      CHECK(ss.str() == "equal");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::NEQ;
      CHECK(ss.str() == "not equal");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::IF;
      CHECK(ss.str() == "if");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::END;
      CHECK(ss.str() == "end");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::WHILE;
      CHECK(ss.str() == "while");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::AND;
      CHECK(ss.str() == "and");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::OR;
      CHECK(ss.str() == "or");
    }
    {
      std::stringstream ss;
      ss << sunray::script::TokenCode::NOT;
      CHECK(ss.str() == "not");
    }
  }
}
