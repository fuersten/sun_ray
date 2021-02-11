//
//  scanner.h
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

#include <sun_ray/script/format_helper.h>
#include <sun_ray/script/stream_reader.h>
#include <sun_ray/script/string_helper.h>
#include <sun_ray/script/token.h>

#include <cctype>
#include <unordered_map>


namespace sunray
{
  namespace script
  {
    class Scanner
    {
    public:
      explicit Scanner(std::istream& input)
      : reader_{input}
      {
        keywords_["if"] = TokenCode::IF;
        keywords_["end"] = TokenCode::END;
        keywords_["while"] = TokenCode::WHILE;
        keywords_["and"] = TokenCode::AND;
        keywords_["or"] = TokenCode::OR;
      }

      Scanner(const Scanner&) = delete;
      Scanner& operator=(const Scanner&) = delete;
      Scanner(Scanner&&) = delete;
      Scanner& operator=(Scanner&&) = delete;

      Token get_next_token()
      {
        Token tok;

        reader_.eat_whitespace();
        tok.location_ = Location{reader_.current_line_count(), reader_.current_column_count()};

        if (reader_.is_eoi()) {
          tok.code_ = TokenCode::EOI;
        } else {
          switch (reader_.current_char()) {
            case '+':
              tok.code_ = TokenCode::PLUS;
              break;
            case '-':
              tok.code_ = TokenCode::MINUS;
              break;
            case '*':
              tok.code_ = TokenCode::MUL;
              break;
            case '#':
              reader_.read_until_eol();
              return get_next_token();
            case '/':
              tok.code_ = TokenCode::DIV;
              break;
            case '.':
              tok.code_ = TokenCode::DOT;
              break;
            case '=':
              tok.code_ = TokenCode::ASSIGNMENT;
              if (reader_.next_char() == '=') {
                tok.code_ = TokenCode::EQ;
              }
              reader_.next_char();
              return tok;
            case '(':
              tok.code_ = TokenCode::LPAREN;
              break;
            case ')':
              tok.code_ = TokenCode::RPAREN;
              break;
            case '[':
              tok.code_ = TokenCode::LSQUARE;
              break;
            case ']':
              tok.code_ = TokenCode::RSQUARE;
              break;
            case '<':
              tok.code_ = TokenCode::LT;
              if (reader_.next_char() == '=') {
                tok.code_ = TokenCode::LE;
              }
              reader_.next_char();
              return tok;
            case '>':
              tok.code_ = TokenCode::GT;
              if (reader_.next_char() == '=') {
                tok.code_ = TokenCode::GE;
              }
              reader_.next_char();
              return tok;
            case '\r':
              reader_.next_char();
              return get_next_token();
            case '\n':
              reader_.next_char();
              return get_next_token();
            case ',':
              tok.code_ = TokenCode::COMMA;
              break;
            default: {
              if (::isdigit(reader_.current_char())) {
                tok.value_ = parse_number();
                tok.code_ = TokenCode::CONST_LITERAL;
              } else if (::isalpha(reader_.current_char())) {
                if (::islower(reader_.current_char())) {
                  tok.code_ = TokenCode::IDENTIFIER;
                } else {
                  tok.code_ = TokenCode::TYPENAME;
                }
                tok.identifier_ = parse_identifier();
                if (tok.identifier_ == "true") {
                  tok.value_ = true;
                  tok.code_ = TokenCode::CONST_LITERAL;
                  tok.identifier_.clear();
                } else if (tok.identifier_ == "false") {
                  tok.value_ = false;
                  tok.code_ = TokenCode::CONST_LITERAL;
                  tok.identifier_.clear();
                } else if (keywords_.find(tok.identifier_) != keywords_.end()) {
                  tok.code_ = keywords_.find(tok.identifier_)->second;
                  tok.identifier_.clear();
                }
              } else if (reader_.current_char() == '\'') {
                tok.value_ = parse_quoted_string();
                tok.code_ = TokenCode::CONST_LITERAL;
              } else {
                throw std::runtime_error{fmt::format("scanning unknown char '{}'", reader_.current_char())};
              }
              return tok;
            }
          }

          reader_.next_char();
        }

        return tok;
      }

    private:
      double parse_number()
      {
        double value = 0.0;

        std::stringstream ss;
        if (reader_.current_char() == '0') {
          ss << reader_.current_char();
          reader_.next_char();
        } else {
          while (std::isdigit(reader_.current_char())) {
            ss << reader_.current_char();
            reader_.next_char();
          }
        }
        if (reader_.current_char() == '.') {
          ss << reader_.current_char();
          reader_.next_char();
          while (std::isdigit(reader_.current_char())) {
            ss << reader_.current_char();
            reader_.next_char();
          }
        }
        if (reader_.current_char() == 'e' || reader_.current_char() == 'E') {
          ss << reader_.current_char();
          reader_.next_char();
          if (reader_.current_char() == '+' || reader_.current_char() == '-') {
            ss << reader_.current_char();
            reader_.next_char();
          }
          while (std::isdigit(reader_.current_char())) {
            ss << reader_.current_char();
            reader_.next_char();
          }
        }
        value = std::stod(ss.str());
        return value;
      }

      std::string parse_identifier()
      {
        std::string s;
        while (::isalpha(reader_.current_char()) || ::isdigit(reader_.current_char()) || reader_.current_char() == '_') {
          s.append(1, reader_.current_char());
          reader_.next_char();
        }
        return trim(s);
      }

      std::string parse_quoted_string()
      {
        reader_.safe_next_char();
        std::string s;
        while (reader_.current_char() != '\'') {
          parse_next_string_char(s);
        }
        reader_.next_char();

        return s;
      }

      void parse_next_string_char(std::string& s)
      {
        if (reader_.current_char() == '\\') {
          switch (reader_.safe_next_char()) {
            case '\'':
              s.append(1, '\'');
              break;
            case '\\':
              s.append(1, '\\');
              break;
            case 'b':
              s.append(1, '\b');
              break;
            case 'f':
              s.append(1, '\f');
              break;
            case 't':
              s.append(1, '\t');
              break;
            case 'n':
              s.append(1, '\n');
              break;
            case 'r':
              s.append(1, '\r');
              break;
            default:
              s = reader_.current_char();
              throw std::runtime_error{"illegal escape sequence '\\" + s + "' found"};
          }
        } else {
          s.append(1, reader_.current_char());
        }
        reader_.next_char();
      }

      StreamReader reader_;
      std::unordered_map<std::string, TokenCode> keywords_;
    };
  }
}
