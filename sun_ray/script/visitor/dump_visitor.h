//
//  dump_visitor.h
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

#include <sun_ray/script/ast.h>
#include <sun_ray/script/types.h>

#include <iomanip>
#include <ostream>


namespace sunray
{
  namespace script
  {
    class DumpVisitor : public Visitor
    {
    public:
      DumpVisitor(std::ostream& stream)
      : stream_(stream)
      {
      }

      void visit(const Statements& statements)
      {
        for (const auto& statement : statements) {
          stream_ << indent();
          statement->accept(*this);
          stream_ << "\n";
        }
      }

      void visit(const BinaryExpression& node) override
      {
        stream_ << "(";
        node.lhs().accept(*this);
        stream_ << " " << node.op() << " ";
        node.rhs().accept(*this);
        stream_ << ")";
      }

      void visit(const LogicalExpression& node) override
      {
        node.lhs().accept(*this);
        stream_ << " " << node.op() << " ";
        node.rhs().accept(*this);
      }

      void visit(const ConditionalExpression& node) override
      {
        node.lhs().accept(*this);
        stream_ << " " << node.op() << " ";
        node.rhs().accept(*this);
      }

      void visit(const SimpleConditionalExpression& node) override
      {
        node.rhs().accept(*this);
      }

      void visit(const UnaryExpression& node) override
      {
        stream_ << node.op() << " ";
        node.rhs().accept(*this);
      }

      void visit(const Identifier& node) override
      {
        stream_ << node.identifier();
      }

      void visit(const Literal& node) override
      {
        if (is_string(node.value())) {
          stream_ << std::quoted(to_string(node.value()), '\'');
        } else {
          stream_ << to_string(node.value());
        }
      }

      void visit(const Assignment& node) override
      {
        node.lhs().accept(*this);
        stream_ << " = ";
        node.expression().accept(*this);
      }

      void visit(const IfCondition& node) override
      {
        stream_ << "if (";
        node.condition().accept(*this);
        stream_ << ")\n";
        ++indent_;
        for (const auto& statement : node.then_body()) {
          stream_ << indent();
          statement->accept(*this);
          stream_ << "\n";
        }
        --indent_;
        stream_ << indent() << "end";
      }

      void visit(const While& node) override
      {
        stream_ << "while (";
        node.condition().accept(*this);
        stream_ << ")\n";
        ++indent_;
        for (const auto& statement : node.body()) {
          stream_ << indent();
          statement->accept(*this);
          stream_ << "\n";
        }
        --indent_;
        stream_ << indent() << "end";
      }

      void visit(const FunctionCall& node) override
      {
        stream_ << node.identifier() << "(";
        bool first{true};
        for (const auto& param : node.parameter()) {
          if (first) {
            first = false;
          } else {
            stream_ << ", ";
          }
          param->accept(*this);
        }
        stream_ << ")";
      }

      void visit(const Property& node) override
      {
        node.lhs().accept(*this);
        stream_ << "." << node.identifier();
      }

      void visit(const MethodCall& node) override
      {
        node.lhs().accept(*this);
        stream_ << "." << node.identifier() << "(";
        bool first{true};
        for (const auto& param : node.parameter()) {
          if (first) {
            first = false;
          } else {
            stream_ << ", ";
          }
          param->accept(*this);
        }
        stream_ << ")";
      }

      void visit(const Constructor& node) override
      {
        stream_ << node.type_name() << "(";
        bool first{true};
        for (const auto& param : node.parameter()) {
          if (first) {
            first = false;
          } else {
            stream_ << ", ";
          }
          param->accept(*this);
        }
        stream_ << ")";
      }

    private:
      std::string indent() const
      {
        return std::string(2 * indent_, ' ');
      }
      std::ostream& stream_;
      uint32_t indent_{0};
    };
  }
}
