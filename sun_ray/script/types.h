//
//  types.h
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
#include <sun_ray/script/meta_class_fwd.h>

#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>


namespace sunray
{
  namespace script
  {
    using Variant = std::variant<std::string, bool, double, MutableClassPtr>;

    template<class... Ts>
    struct overload : Ts... {
      using Ts::operator()...;
    };
    template<class... Ts>
    overload(Ts...) -> overload<Ts...>;

    template<typename = void>
    std::string to_string(const Variant& v)
    {
      return std::visit(overload{[](const std::string& s) {
                                   return s;
                                 },
                                 [](const bool& b) {
                                   std::stringstream ss;
                                   ss << std::boolalpha << b;
                                   return ss.str();
                                 },
                                 [](const double& d) {
                                   std::stringstream ss;
                                   ss << std::fixed << std::showpoint << std::setprecision(2) << d;
                                   return ss.str();
                                 },
                                 [](const MutableClassPtr& c) {
                                   return fmt::format("<{}>", c->to_string());
                                 }},
                        v);
    }

    inline bool is_double(const Variant& v)
    {
      return v.index() == 2;
    }

    inline bool is_string(const Variant& v)
    {
      return v.index() == 0;
    }

    inline bool is_class(const Variant& v)
    {
      return v.index() == 3;
    }

    inline bool is_bool(const Variant& v)
    {
      return v.index() == 1;
    }

    inline double as_double(const Variant& v)
    {
      return std::get<2>(v);
    }

    inline std::string as_string(const Variant& v)
    {
      return std::get<0>(v);
    }

    inline double& as_double(Variant& v)
    {
      return std::get<2>(v);
    }

    inline std::string& as_string(Variant& v)
    {
      return std::get<0>(v);
    }

    inline MutableClassPtr as_class(const Variant& v)
    {
      return std::get<3>(v);
    }

    inline bool as_bool(const Variant& v)
    {
      return std::get<1>(v);
    }

    inline bool& as_bool(Variant& v)
    {
      return std::get<1>(v);
    }

    template<int v>
    struct int2type {
      enum { value = v };
    };


    template<typename T>
    struct unwrap {
      static T as_type(const Variant&)
      {
        throw std::runtime_error{"unkown type"};
      }
      static bool is_type(const Variant&)
      {
        throw std::runtime_error{"unkown type"};
      }
    };

    template<>
    struct unwrap<double> {
      static double as_type(const Variant& v)
      {
        return as_double(v);
      }
      static bool is_type(const Variant& v)
      {
        return v.index() == 2;
      }
    };

    template<>
    struct unwrap<const std::string&> {
      static std::string as_type(const Variant& v)
      {
        return as_string(v);
      }
      static bool is_type(const Variant& v)
      {
        return v.index() == 0;
      }
    };

    template<>
    struct unwrap<std::string> {
      static std::string as_type(const Variant& v)
      {
        return as_string(v);
      }
      static bool is_type(const Variant& v)
      {
        return v.index() == 0;
      }
    };

    template<>
    struct unwrap<bool> {
      static bool as_type(const Variant& v)
      {
        return as_bool(v);
      }
      static bool is_type(const Variant& v)
      {
        return v.index() == 1;
      }
    };

    template<>
    struct unwrap<const char*> {
      static std::string as_type(const Variant& v)
      {
        return as_string(v);
      }
      static bool is_type(const Variant& v)
      {
        return v.index() == 0;
      }
    };

    template<>
    struct unwrap<Variant> {
      static Variant as_type(const Variant& v)
      {
        return v;
      }
      static bool is_type(const Variant&)
      {
        return true;
      }
    };

    template<>
    struct unwrap<const Variant&> {
      static Variant as_type(const Variant& v)
      {
        return v;
      }
      static bool is_type(const Variant&)
      {
        return true;
      }
    };

    template<>
    struct unwrap<MutableClassPtr&> {
      static MutableClassPtr as_type(const Variant& v)
      {
        return as_class(v);
      }
      static bool is_type(const Variant& v)
      {
        return v.index() == 3;
      }
    };

    template<>
    struct unwrap<const MutableClassPtr&> {
      static MutableClassPtr as_type(const Variant& v)
      {
        return as_class(v);
      }
      static bool is_type(const Variant& v)
      {
        return v.index() == 3;
      }
    };


    enum class BinaryOperator { ADD, SUB, MUL, DIV };

    template<class CharT, class Traits>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const BinaryOperator& op)
    {
      switch (op) {
        case sunray::script::BinaryOperator::ADD:
          os << "+";
          break;
        case sunray::script::BinaryOperator::SUB:
          os << "-";
          break;
        case sunray::script::BinaryOperator::MUL:
          os << "*";
          break;
        case sunray::script::BinaryOperator::DIV:
          os << "/";
          break;
      }
      return os;
    }


    enum class LogicalOperator { AND, OR };

    template<class CharT, class Traits>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const LogicalOperator& op)
    {
      switch (op) {
        case sunray::script::LogicalOperator::AND:
          os << "and";
          break;
        case sunray::script::LogicalOperator::OR:
          os << "or";
          break;
      }
      return os;
    }


    enum class ConditionalOperator { GT, GE, LT, LE, EQ, NEQ };

    template<class CharT, class Traits>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ConditionalOperator& op)
    {
      switch (op) {
        case sunray::script::ConditionalOperator::GT:
          os << ">";
          break;
        case sunray::script::ConditionalOperator::LT:
          os << "<";
          break;
        case sunray::script::ConditionalOperator::LE:
          os << "<=";
          break;
        case sunray::script::ConditionalOperator::GE:
          os << ">=";
          break;
        case sunray::script::ConditionalOperator::EQ:
          os << "==";
          break;
        case sunray::script::ConditionalOperator::NEQ:
          os << "<>";
          break;
      }
      return os;
    }


    enum class UnaryOperator { MINUS, NOT };

    template<class CharT, class Traits>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const UnaryOperator& op)
    {
      switch (op) {
        case sunray::script::UnaryOperator::MINUS:
          os << "-";
          break;
        case sunray::script::UnaryOperator::NOT:
          os << "not";
          break;
      }
      return os;
    }

    enum class PODType { BOOLEAN, DOUBLE, STRING };


    class Type;
    using TypePtr = std::shared_ptr<const Type>;

    class Type
    {
    public:
      virtual ~Type() = default;

      virtual bool is_basic_type() const = 0;

      virtual bool is_class_type() const = 0;

      virtual std::string as_string() const = 0;

    protected:
      Type() = default;
    };


    class BasicType : public Type
    {
    public:
      explicit BasicType(PODType type) noexcept
      : type_{type}
      {
      }

      bool is_basic_type() const override
      {
        return true;
      }

      bool is_class_type() const override
      {
        return false;
      }

      std::string as_string() const override
      {
        switch (type_) {
          case PODType::BOOLEAN:
            return "bool";
          case PODType::DOUBLE:
            return "double";
          case PODType::STRING:
            return "string";
        }
        throw std::runtime_error{fmt::format("unknown pod type '{}'", type_)};
      }

      PODType type() const
      {
        return type_;
      }

      static TypePtr make_type(sunray::script::PODType type)
      {
        return std::make_shared<sunray::script::BasicType>(type);
      }

    private:
      PODType type_;
    };


    class ClassType : public Type
    {
    public:
      explicit ClassType(std::string type) noexcept
      : type_{std::move(type)}
      {
      }

      bool is_basic_type() const override
      {
        return false;
      }

      bool is_class_type() const override
      {
        return true;
      }

      std::string as_string() const override
      {
        return type_;
      }

      const std::string& type() const
      {
        return type_;
      }

    private:
      std::string type_;
    };

    inline TypePtr type_from_variant(const Variant& v)
    {
      if (is_string(v)) {
        return BasicType::make_type(PODType::STRING);
      }
      if (is_double(v)) {
        return BasicType::make_type(PODType::DOUBLE);
      }
      if (is_bool(v)) {
        return BasicType::make_type(PODType::BOOLEAN);
      }
      if (is_class(v)) {
        return std::make_shared<ClassType>(as_class(v)->meta_class()->name());
      }
      throw std::runtime_error{"Cannot create type due to unkown variant type"};
    }
  }
}
