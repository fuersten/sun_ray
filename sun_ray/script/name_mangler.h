//
//  name_mangler.h
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


namespace sunray
{
  namespace script
  {
    struct NameMangler {
      static std::string mangle(const std::string& name)
      {
        if (name.empty()) {
          throw std::runtime_error{"cannot mangle empty name"};
        }
        return fmt::format("{}{}{}", preamble_, name.length(), name);
      }

      static std::string mangle(const std::string& name, uint8_t count)
      {
        return fmt::format("{}{}", mangle(name), count);
      }

      static std::string demangle(const std::string& mangeled_name)
      {
        auto pos = mangeled_name.find_first_not_of("0123456789", 3);
        if (pos == std::string::npos) {
          throw std::runtime_error{fmt::format("'{}' is not a mangeled name", mangeled_name)};
        }
        if (mangeled_name.substr(0, 3) != preamble_) {
          throw std::runtime_error{fmt::format("'{}' is not a mangeled name", mangeled_name)};
        }

        unsigned long len{0};
        try {
          len = std::stoul(mangeled_name.substr(3, pos - 3));
        } catch (const std::exception&) {
          throw std::runtime_error{fmt::format("'{}' is not a mangeled name", mangeled_name)};
        }
        if (len == 0) {
          throw std::runtime_error{fmt::format("'{}' is not a mangeled name", mangeled_name)};
        }
        return mangeled_name.substr(pos, len);
      }

    private:
      NameMangler() = default;
      NameMangler(const NameMangler&) = delete;
      NameMangler(NameMangler&&) = delete;
      NameMangler& operator=(const NameMangler&) = delete;
      NameMangler& operator=(NameMangler&&) = delete;

      static constexpr const char* preamble_ = "_SR";
    };
  }
}
