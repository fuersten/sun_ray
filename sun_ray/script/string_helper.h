//
//  string_helper.h
//  sun_ray
//
//  Created by Lars-Christian Fürstenberg on 22.01.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <iterator>
#include <sstream>
#include <string>


namespace sunray
{
  namespace script
  {
    template<class T, class Traits, class Alloc>
    inline std::basic_string<T, Traits, Alloc> trim_right(const std::basic_string<T, Traits, Alloc>& s)
    {
      size_t endpos = s.find_last_not_of(" \t");
      if (std::string::npos != endpos && endpos != s.length() - 1) {
        return s.substr(0, endpos + 1);
      }
      return s;
    }


    template<class T, class Traits, class Alloc>
    inline std::basic_string<T, Traits, Alloc> trim_left(const std::basic_string<T, Traits, Alloc>& s)
    {
      size_t startpos = s.find_first_not_of(" \t");
      if (std::string::npos != startpos && startpos != 0) {
        return s.substr(startpos);
      }
      return s;
    }


    template<class T, class Traits, class Alloc>
    inline std::basic_string<T, Traits, Alloc> trim(const std::basic_string<T, Traits, Alloc>& s)
    {
      return trim_left(trim_right(s));
    }


    template<typename T>
    std::string join(const T& elems, const char* const separator)
    {
      switch (elems.size()) {
        case 0:
          return "";
        case 1: {
          std::ostringstream os;
          os << elems[0];
          return os.str();
        }
        default:
          std::ostringstream os;
          std::copy(elems.begin(), elems.end() - 1, std::ostream_iterator<typename T::value_type>(os, separator));
          os << *elems.rbegin();
          return os.str();
      }
    }
  }
}
