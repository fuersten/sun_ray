//
//  validation_visitor.h
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
#include <sun_ray/script/symboltable.h>


namespace sunray
{
  namespace script
  {
    class ValidationVisitor : public Visitor
    {
    public:
      explicit ValidationVisitor(SymbolTable& sym_table, DiagnosticMessageHandler& diagnostic_messages)
      : sym_table_(sym_table)
      , diagnostic_messages_(diagnostic_messages)
      {
      }

      void visit(const Statements& statements)
      {
        for (const auto& statement : statements) {
          statement->accept(*this);
        }
      }

      void visit(const BinaryExpression& node) override
      {
        node.lhs().accept(*this);
        node.rhs().accept(*this);
      }

      void visit(const LogicalExpression& node) override
      {
        (void)node;
      }

      void visit(const ConditionalExpression& node) override
      {
        (void)node;
        // TODO
      }

      void visit(const UnaryExpression& node) override
      {
        node.rhs().accept(*this);
      }

      void visit(const Identifier& node) override
      {
        if (!is_assignment_ && !sym_table_.has_symbol(node.identifier())) {
          diagnostic_messages_.add_error("E002", "Undeclared identifier '" + node.identifier() + "'", node.location());
        } else if (is_assignment_) {
          sym_table_.add_symbol(node.identifier());
        }
      }

      void visit(const Literal& node) override
      {
        (void)node;
      }

      void visit(const Assignment& node) override
      {
        node.expression().accept(*this);
        is_assignment_ = true;
        node.lhs().accept(*this);
        is_assignment_ = false;
      }

      void visit(const IfCondition& node) override
      {
        (void)node;
      }

      void visit(const While& node) override
      {
        (void)node;
      }

      void visit(const FunctionCall& node) override
      {
        (void)node;
      }

      void visit(const Property& node) override
      {
        (void)node;
      }

      void visit(const MethodCall& node) override
      {
        (void)node;
      }

      void visit(const Constructor& node) override
      {
        (void)node;
      }

    private:
      SymbolTable& sym_table_;
      DiagnosticMessageHandler& diagnostic_messages_;
      bool is_assignment_{false};
    };
  }
}
