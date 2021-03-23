//
//  token.h
//  sun_ray
//
//  BSD 3-Clause License
//  Copyright (c) 2020, Lars-Christian Fürstenberg
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//  this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its
//  contributors may be used to endorse or promote products derived from this
//  software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//

#pragma once

#include <sun_ray/script/location.h>
#include <sun_ray/script/types.h>


namespace sunray
{
  namespace script
  {
    enum class TokenCode {
      UNDEFINED,
      EOI,
      CONST_LITERAL,
      ASSIGNMENT,
      PLUS,
      MINUS,
      MUL,
      DIV,
      DOT,
      LPAREN,
      RPAREN,
      LSQUARE,
      RSQUARE,
      IDENTIFIER,
      COMMA,
      TYPENAME,
      LT,
      LE,
      GT,
      GE,
      EQ,
      IF,
      END,
      WHILE,
      AND,
      OR
    };

    template<class CharT, class Traits>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const TokenCode& tok)
    {
      switch (tok) {
        case sunray::script::TokenCode::UNDEFINED:
          os << "unexpected character sequence";
          break;
        case sunray::script::TokenCode::EOI:
          os << "eoi";
          break;
        case sunray::script::TokenCode::CONST_LITERAL:
          os << "float";
          break;
        case sunray::script::TokenCode::ASSIGNMENT:
          os << "assignment";
          break;
        case sunray::script::TokenCode::PLUS:
          os << "plus";
          break;
        case sunray::script::TokenCode::MINUS:
          os << "minus";
          break;
        case sunray::script::TokenCode::MUL:
          os << "mult";
          break;
        case sunray::script::TokenCode::DIV:
          os << "div";
          break;
        case sunray::script::TokenCode::DOT:
          os << "dot";
          break;
        case sunray::script::TokenCode::LPAREN:
          os << "left paren";
          break;
        case sunray::script::TokenCode::RPAREN:
          os << "right paren";
          break;
        case sunray::script::TokenCode::LSQUARE:
          os << "left square";
          break;
        case sunray::script::TokenCode::RSQUARE:
          os << "right square";
          break;
        case sunray::script::TokenCode::IDENTIFIER:
          os << "identifier";
          break;
        case sunray::script::TokenCode::COMMA:
          os << "comma";
          break;
        case sunray::script::TokenCode::TYPENAME:
          os << "typename";
          break;
        case sunray::script::TokenCode::LT:
          os << "less than";
          break;
        case sunray::script::TokenCode::GT:
          os << "greater than";
          break;
        case sunray::script::TokenCode::LE:
          os << "less equal";
          break;
        case sunray::script::TokenCode::GE:
          os << "greater equal";
          break;
        case sunray::script::TokenCode::EQ:
          os << "equal";
          break;
        case sunray::script::TokenCode::IF:
          os << "if";
          break;
        case sunray::script::TokenCode::END:
          os << "end";
          break;
        case sunray::script::TokenCode::WHILE:
          os << "while";
          break;
        case sunray::script::TokenCode::AND:
          os << "and";
          break;
        case sunray::script::TokenCode::OR:
          os << "or";
          break;
      }
      return os;
    }


    struct Token {
      Token() = default;

      Token(TokenCode code)
      : code_{code}
      {
      }

      Token(const Token&) = delete;
      Token(Token&&) = default;
      Token& operator=(const Token&) = delete;
      Token& operator=(Token&&) = default;

      bool operator==(const TokenCode rhs) const
      {
        return code_ == rhs;
      }

      bool operator!=(const TokenCode rhs) const
      {
        return code_ != rhs;
      }

      TokenCode code_{TokenCode::UNDEFINED};
      Variant value_;
      std::string identifier_;
      Location location_;
    };
  }
}
