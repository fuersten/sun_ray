//
//  build_in_function.h
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

#include <sun_ray/feature/math_helper.h>
#include <sun_ray/script/function_registry.h>

#include <date/date.h>
#include <random>


namespace sunray
{
  namespace script
  {
    class BuildInFunctions
    {
    public:
      BuildInFunctions(FunctionRegistry& registry, std::ostream& out)
      {
        out_ = &out;
        registry.add_function("now", now);

        registry.add_function("ceil", (double (*)(double))(::ceil));
        registry.add_function("floor", (double (*)(double))(::floor));
        registry.add_function("trunc", (double (*)(double))(::trunc));
        registry.add_function("round", (double (*)(double))(::round));

        registry.add_function("sin", (double (*)(double))(::sin));
        registry.add_function("cos", (double (*)(double))(::cos));
        registry.add_function("tan", (double (*)(double))(::tan));
        registry.add_function("asin", (double (*)(double))(::asin));
        registry.add_function("acos", (double (*)(double))(::acos));
        registry.add_function("atan", (double (*)(double))(::atan));

        registry.add_function("log", (double (*)(double))(::log));
        registry.add_function("exp", (double (*)(double))(::exp));
        registry.add_function("sqrt", (double (*)(double))(::sqrt));
        registry.add_function("pow", (double (*)(double, double))(::pow));

        registry.add_function("abs", (double (*)(double))(::fabs));
        registry.add_function("mod", (double (*)(double, double))(::fmod));
        registry.add_function("max", (double (*)(double, double))(::fmax));
        registry.add_function("min", (double (*)(double, double))(::fmin));

        registry.add_function("print", (double (*)(Variant))(print));
        registry.add_function("print", (double (*)(const std::string&, Variant))(print));
        registry.add_function("print", (double (*)(const std::string&, Variant, Variant))(print));
        registry.add_function("print", (double (*)(const std::string&, Variant, Variant, Variant))(print));
        registry.add_function("print", (double (*)(const std::string&, Variant, Variant, Variant, Variant))(print));

        registry.add_function("println", (double (*)(Variant))(println));
        registry.add_function("println", (double (*)(const std::string&, Variant))(println));
        registry.add_function("println", (double (*)(const std::string&, Variant, Variant))(println));
        registry.add_function("println", (double (*)(const std::string&, Variant, Variant, Variant))(println));
        registry.add_function("println", (double (*)(const std::string&, Variant, Variant, Variant, Variant))(println));

        registry.add_function("str", (std::string(*)(Variant))(str));
        registry.add_function("format", (std::string(*)(const std::string&, Variant))(format));
        registry.add_function("random", (double (*)())(random));
        registry.add_function("seed", (double (*)(double))(seed));
        registry.add_function("deg_to_rad", (double (*)(double))(deg_to_rad));
      }

      ~BuildInFunctions() = default;

      BuildInFunctions(const BuildInFunctions&) = delete;
      BuildInFunctions(BuildInFunctions&&) = delete;
      BuildInFunctions& operator=(const BuildInFunctions&) = delete;
      BuildInFunctions& operator=(BuildInFunctions&&) = delete;

    private:
      static double seed(double seed)
      {
        gen_.seed(static_cast<uint32_t>(seed));
        return 0.0;
      }

      static double random()
      {
        return dis_(gen_);
      }

      static std::string now()
      {
        auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
        return date::format("%FT%T", now);
      }

      static std::string str(Variant v)
      {
        return to_string(v);
      }

      static std::string format(const std::string& fmt, Variant v)
      {
        if (is_double(v)) {
          return fmt::format(fmt, as_double(v));
        } else if (is_bool(v)) {
          return fmt::format(fmt, as_bool(v));
        } else if (is_string(v)) {
          return fmt::format(fmt, as_string(v));
        }

        return fmt::format(fmt, to_string(v));
      }

      static double print(Variant v)
      {
        *out_ << to_string(v);
        return 0;
      }

      static double print(const std::string& fmt, Variant v0)
      {
        *out_ << fmt::format(fmt, to_string(v0));
        return 0;
      }

      static double print(const std::string& fmt, Variant v0, Variant v1)
      {
        *out_ << fmt::format(fmt, to_string(v0), to_string(v1));
        return 0;
      }

      static double print(const std::string& fmt, Variant v0, Variant v1, Variant v2)
      {
        *out_ << fmt::format(fmt, to_string(v0), to_string(v1), to_string(v2));
        return 0;
      }

      static double print(const std::string& fmt, Variant v0, Variant v1, Variant v2, Variant v3)
      {
        *out_ << fmt::format(fmt, to_string(v0), to_string(v1), to_string(v2), to_string(v3));
        return 0;
      }

      static double println(Variant v)
      {
        *out_ << to_string(v) << std::endl;
        return 0;
      }

      static double println(const std::string& fmt, Variant v0)
      {
        *out_ << fmt::format(fmt, to_string(v0)) << std::endl;
        return 0;
      }

      static double println(const std::string& fmt, Variant v0, Variant v1)
      {
        *out_ << fmt::format(fmt, to_string(v0), to_string(v1)) << std::endl;
        return 0;
      }

      static double println(const std::string& fmt, Variant v0, Variant v1, Variant v2)
      {
        *out_ << fmt::format(fmt, to_string(v0), to_string(v1), to_string(v2)) << std::endl;
        return 0;
      }

      static double println(const std::string& fmt, Variant v0, Variant v1, Variant v2, Variant v3)
      {
        *out_ << fmt::format(fmt, to_string(v0), to_string(v1), to_string(v2), to_string(v3)) << std::endl;
        return 0;
      }

      inline static std::ostream* out_ = nullptr;
      inline static std::mt19937 gen_{3127717181};
      inline static std::uniform_real_distribution<> dis_{-1.0, 1.0};
    };
  }
}
