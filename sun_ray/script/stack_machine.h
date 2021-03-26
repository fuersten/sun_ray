//
//  stack_machine.h
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
#include <sun_ray/script/stack.h>

#include <optional>
#include <ostream>
#include <vector>

namespace sunray
{
  namespace script
  {
    enum class OpCode {
      ADD,
      AND,
      BEQ,
      BNE,
      DIV,
      DROP,
      EQ,
      FUNC,
      JMP,
      LE,
      LT,
      METHOD,
      MUL,
      NEG,
      NOP,
      NOT,
      OR,
      PUSH,
      PUSHVAR,
      STOREVAR,
      SUB
    };

    template<class CharT, class Traits>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const OpCode& code)
    {
      switch (code) {
        case OpCode::ADD:
          os << "ADD";
          break;
        case OpCode::AND:
          os << "AND";
          break;
        case OpCode::BEQ:
          os << "BEQ";
          break;
        case OpCode::BNE:
          os << "BNE";
          break;
        case OpCode::DIV:
          os << "DIV";
          break;
        case OpCode::DROP:
          os << "DROP";
          break;
        case OpCode::EQ:
          os << "EQ";
          break;
        case OpCode::FUNC:
          os << "FUNC";
          break;
        case OpCode::JMP:
          os << "JMP";
          break;
        case OpCode::LE:
          os << "LE";
          break;
        case OpCode::LT:
          os << "LT";
          break;
        case OpCode::METHOD:
          os << "METHOD";
          break;
        case OpCode::MUL:
          os << "MUL";
          break;
        case OpCode::NEG:
          os << "NEG";
          break;
        case OpCode::NOP:
          os << "NOP";
          break;
        case OpCode::NOT:
          os << "NOT";
          break;
        case OpCode::OR:
          os << "OR";
          break;
        case OpCode::PUSH:
          os << "PUSH";
          break;
        case OpCode::PUSHVAR:
          os << "PUSHVAR";
          break;
        case OpCode::STOREVAR:
          os << "STOREVAR";
          break;
        case OpCode::SUB:
          os << "SUB";
          break;
      }

      return os;
    }


    class Label
    {
    public:
      explicit Label(std::string label)
      : label_{std::move(label)}
      , index_{std::numeric_limits<size_t>::max()}
      {
      }

      void set_index(size_t index)
      {
        index_ = index;
      }

      bool operator==(const Label& l) const
      {
        return label_ == l.label_;
      }

      const std::string& label() const
      {
        return label_;
      }

      size_t index() const
      {
        return index_;
      }

    private:
      std::string label_;
      size_t index_;
    };


    struct Instruction {
      explicit Instruction(OpCode op)
      : code_{op}
      {
      }

      explicit Instruction(OpCode op, Label label)
      : code_{op}
      , label_{std::move(label)}
      {
      }

      explicit Instruction(OpCode op, Variant val)
      : code_{op}
      , value_{std::move(val)}
      {
      }

      explicit Instruction(OpCode op, uint32_t index)
      : code_{op}
      , index_{index}
      {
      }

      ~Instruction() = default;

      Instruction(const Instruction&) = default;
      Instruction(Instruction&&) = default;
      Instruction& operator=(const Instruction&) = delete;
      Instruction& operator=(Instruction&&) = default;

      OpCode code_;
      Variant value_;
      std::optional<Label> label_;
      std::optional<uint32_t> index_;
    };

    typedef std::vector<Instruction> Instructions;


    class StackMachine
    {
    public:
      StackMachine(FunctionRegistry& registry)
      : function_registry_{registry}
      {
        variables_.resize(256);
      }

      ~StackMachine() = default;

      StackMachine(const StackMachine&) = delete;
      StackMachine(StackMachine&&) = default;
      StackMachine& operator=(const StackMachine&) = delete;
      StackMachine& operator=(StackMachine&&) = delete;

      void add_instruction(Instruction inst)
      {
        instructions_.emplace(instructions_.end(), std::move(inst));
      }

      void add_label(Label label)
      {
        auto it = std::find(labels_.begin(), labels_.end(), label);
        if (it != labels_.end()) {
          throw std::runtime_error{fmt::format("Label '{}' already set", label.label())};
        }

        label.set_index(instructions_.size() - 1);
        labels_.emplace_back(std::move(label));
      }

      Variant run()
      {
        reset();
        resolve_labels();

        size_t idx = 0;
        while (idx < instructions_.size()) {
          const auto& instruction = instructions_[idx];
          switch (instruction.code_) {
            case OpCode::STOREVAR: {
              variables_[instruction.index_.value()] = values_.top();
              values_.pop();
              break;
            }
            case OpCode::ADD: {
              std::visit(overload{[this](const std::string& val) {
                                    as_string(values_.top(1)) += val;
                                  },
                                  [](const bool&) {
                                    throw std::runtime_error{"Cannot add with bool"};
                                  },
                                  [this](const double& val) {
                                    as_double(values_.top(1)) += val;
                                  },
                                  [](const MutableClassPtr&) {
                                    throw std::runtime_error{"Cannot add with class"};
                                  }},
                         values_.top());
              values_.pop();
              break;
            }
            case OpCode::AND: {
              auto& top = values_.top(1);
              top = as_bool(values_.top()) && as_bool(top);
              values_.pop();
              break;
            }
            case OpCode::BEQ: {
              const auto& val = as_bool(values_.top());
              values_.pop();
              if (val) {
                idx = static_cast<size_t>(as_double(instruction.value_));
                if (idx > instructions_.size() - 1) {
                  throw std::runtime_error{fmt::format("jump index {} is out of range", idx)};
                }
                continue;
              }
              break;
            }
            case OpCode::BNE: {
              const auto& val = as_bool(values_.top());
              values_.pop();
              if (!val) {
                idx = static_cast<size_t>(as_double(instruction.value_));
                if (idx > instructions_.size() - 1) {
                  throw std::runtime_error{fmt::format("jump index {} is out of range", idx)};
                }
                continue;
              }
              break;
            }
            case OpCode::DROP: {
              values_.pop();
              break;
            }
            case OpCode::EQ: {
              auto& top = values_.top(1);
              top = Approx(as_double(values_.top())) == as_double(top);
              values_.pop();
              break;
            }
            case OpCode::FUNC: {
              const size_t fun_idx = static_cast<size_t>(as_double(values_.top()));
              const uint32_t count = static_cast<uint32_t>(as_double(values_.top(1)));
              std::vector<Variant> parameter;
              size_t pn = 2;
              for (uint32_t n = 0; n < count; ++n) {
                parameter.emplace_back(values_.top(pn));
                ++pn;
              }
              values_.pop(pn);
              values_.push(function_registry_.call_function(fun_idx, parameter));
              break;
            }
            case OpCode::JMP: {
              idx = static_cast<size_t>(as_double(instruction.value_));
              if (idx > instructions_.size() - 1) {
                throw std::runtime_error{fmt::format("jump index {} is out of range", idx)};
              }
              continue;
            }
            case OpCode::LE: {
              auto& top = values_.top(1);
              top = as_double(values_.top()) <= as_double(top);
              values_.pop();
              break;
            }
            case OpCode::LT: {
              auto& top = values_.top(1);
              top = as_double(values_.top()) < as_double(top);
              values_.pop();
              break;
            }
            case OpCode::METHOD: {
              const auto count = static_cast<uint8_t>(as_double(values_.top()));
              const auto name = as_string(values_.top(1));
              const auto c = as_class(values_.top(2));
              const auto mangled_name =
                NameMangler::mangle(fmt::format("{}_{}", c->meta_class()->name(), name), static_cast<uint8_t>(count + 1));
              const auto fun_idx = function_registry_.index_for_function(mangled_name);
              if (fun_idx < 0) {
                throw std::runtime_error{fmt::format("unkonw method {}_{}", c->meta_class()->name(), name)};
              }
              std::vector<Variant> parameter{c};
              size_t pn = 3;
              for (uint32_t n = 0; n < count; ++n) {
                parameter.emplace_back(values_.top(pn));
                ++pn;
              }
              values_.pop(pn);
              values_.push(function_registry_.call_function(static_cast<size_t>(fun_idx), parameter));
              break;
            }
            case OpCode::MUL: {
              as_double(values_.top(1)) *= as_double(values_.top());
              values_.pop();
              break;
            }
            case OpCode::DIV: {
              as_double(values_.top(1)) /= as_double(values_.top());
              values_.pop();
              break;
            }
            case OpCode::NEG: {
              values_.top() = -as_double(values_.top());
              break;
            }
            case OpCode::NOP: {
              break;
            }
            case OpCode::NOT: {
              values_.top() = !as_bool(values_.top());
              break;
            }
            case OpCode::OR: {
              auto& top = values_.top(1);
              top = as_bool(values_.top()) || as_bool(top);
              values_.pop();
              break;
            }
            case OpCode::PUSH: {
              auto val = instruction.value_;
              values_.push(std::move(val));
              break;
            }
            case OpCode::PUSHVAR: {
              values_.push(variables_[instruction.index_.value()]);
              break;
            }
            case OpCode::SUB: {
              as_double(values_.top(1)) -= as_double(values_.top());
              values_.pop();
              break;
            }
          }
          ++idx;
        }

        if (values_.empty()) {
          return Variant{};
        }
        return values_.next();
      }

      void dump_instructions(std::ostream& os) const
      {
        size_t count{0};
        for (const auto& instruction : instructions_) {
          print_label(os, count);
          os << instruction.code_ << " " << to_string(instruction.value_);
          if (instruction.label_) {
            os << " " << instruction.label_->label();
          }
          os << "\n";
          ++count;
        }
      }

      Instructions instructions() const
      {
        return instructions_;
      }

    private:
      void print_label(std::ostream& os, size_t count) const
      {
        auto it = std::find_if(labels_.begin(), labels_.end(), [&count](const auto& label) {
          return label.index() == count;
        });
        if (it != labels_.end()) {
          os << it->label() << ":\n";
        }
      }

      void resolve_labels()
      {
        for (auto& instruction : instructions_) {
          if (instruction.label_) {
            const auto& it = std::find(labels_.begin(), labels_.end(), instruction.label_);
            if (it == labels_.end()) {
              throw std::runtime_error{fmt::format("Label '{}' not found", instruction.label_->label())};
            }
            instruction.value_ = Variant{static_cast<double>(it->index())};
          }
        }
      }

      void reset()
      {
        while (!values_.empty()) {
          values_.pop();
        }
      }

      typedef Stack<Variant, 256> ValueStack;

      FunctionRegistry& function_registry_;
      Instructions instructions_;
      std::vector<Variant> variables_;
      ValueStack values_;
      std::vector<Label> labels_;
    };
  }
}
