//
//  stream_reader.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 22.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <istream>
#include <sstream>


namespace sunray
{
  class StreamReader
  {
  public:
    StreamReader(std::istream& stream)
    : stream_{stream}
    {
      next_char();
    }

    inline char current_char() const
    {
      return current_char_;
    }

    bool is_eol() const
    {
      return current_char_ == '\n' || current_char_ == '\r';
    }

    bool is_eoi() const
    {
      return current_char_ == EOF;
    }

    char next_char()
    {
      if (is_eol()) {
        column_count_ = 0;
        ++line_count_;
      }
      current_char_ = static_cast<char>(stream_.get());
      if (!stream_.good() && !stream_.eof()) {
        throw std::runtime_error{"unexpected end of input"};
      }
      ++column_count_;

      return current_char_;
    }

    char safe_next_char()
    {
      if (next_char() == EOF) {
        throw std::runtime_error{"unexpected end of input"};
      }
      return current_char_;
    }

    void read_until_eol()
    {
      while (safe_next_char() && !is_eol()) {
      }
      next_char();
    }

    char eat_whitespace()
    {
      while (current_char_ == ' ' || current_char_ == '\t') {
        next_char();
      }
      return current_char_;
    }

    uint32_t current_line_count() const
    {
      return line_count_;
    }

    uint32_t current_column_count() const
    {
      return column_count_;
    }

  private:
    std::istream& stream_;
    char current_char_{EOF};
    uint32_t line_count_{1};
    uint32_t column_count_{0};
  };
}
