//
//  stack_machine_visitor.h
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
#include <sun_ray/script/meta_class.h>
#include <sun_ray/script/stack_machine.h>


namespace sunray
{
  namespace script
  {
    class StackMachineVisitor : public Visitor
    {
    public:
      StackMachineVisitor(FunctionRegistry& function_registry, MetaClassRegistry& meta_class_registry)
      : function_registry_{function_registry}
      , meta_class_registry_{meta_class_registry}
      {
      }

      ~StackMachineVisitor() = default;

      StackMachine machine()
      {
        return std::move(sm_);
      }

      void visit(const Statements& statements)
      {
        for (const auto& statement : statements) {
          statement->accept(*this);
          drop_unused_return(statement);
        }
      }

    private:
      void drop_unused_return(const StatementPtr& statement)
      {
        const auto expression_stmt = dynamic_cast<const ExpressionStatement*>(statement.get());
        if (expression_stmt) {
          if (dynamic_cast<const FunctionCall*>(&expression_stmt->expression()) ||
              dynamic_cast<const MethodCall*>(&expression_stmt->expression()) ||
              dynamic_cast<const Property*>(&expression_stmt->expression())) {
            sm_.add_instruction(Instruction(OpCode::DROP));
          }
        }
      }

      void visit(const BinaryExpression& node) override
      {
        node.lhs().accept(*this);
        node.rhs().accept(*this);
        Instruction inst(OpCode::ADD);
        switch (node.op()) {
          case BinaryOperator::ADD:
            inst.code_ = OpCode::ADD;
            break;
          case BinaryOperator::SUB:
            inst.code_ = OpCode::SUB;
            break;
          case BinaryOperator::MUL:
            inst.code_ = OpCode::MUL;
            break;
          case BinaryOperator::DIV:
            inst.code_ = OpCode::DIV;
            break;
        }
        sm_.add_instruction(inst);
      }

      void visit(const LogicalExpression& node) override
      {
        switch (node.op()) {
          case LogicalOperator::AND:
            node.lhs().accept(*this);
            node.rhs().accept(*this);
            sm_.add_instruction(Instruction{OpCode::AND});
            break;
          case LogicalOperator::OR:
            node.lhs().accept(*this);
            node.rhs().accept(*this);
            sm_.add_instruction(Instruction{OpCode::OR});
            break;
        }
      }

      void visit(const ConditionalExpression& node) override
      {
        switch (node.op()) {
          case ConditionalOperator::GT:
            node.lhs().accept(*this);
            node.rhs().accept(*this);
            sm_.add_instruction(Instruction{OpCode::LT});
            break;
          case ConditionalOperator::GE:
            node.lhs().accept(*this);
            node.rhs().accept(*this);
            sm_.add_instruction(Instruction{OpCode::LE});
            break;
          case ConditionalOperator::LT:
            node.rhs().accept(*this);
            node.lhs().accept(*this);
            sm_.add_instruction(Instruction{OpCode::LT});
            break;
          case ConditionalOperator::LE:
            node.rhs().accept(*this);
            node.lhs().accept(*this);
            sm_.add_instruction(Instruction{OpCode::LE});
            break;
          case ConditionalOperator::EQ:
            node.lhs().accept(*this);
            node.rhs().accept(*this);
            sm_.add_instruction(Instruction{OpCode::EQ});
            break;
        }
      }

      void visit(const UnaryExpression& node) override
      {
        node.rhs().accept(*this);
        sm_.add_instruction(Instruction(OpCode::NEG));
      }

      void visit(const Identifier& node) override
      {
        if (!is_assignment_) {
          sm_.add_instruction(Instruction(OpCode::PUSHVAR, get_index_for_identifier(node.identifier())));
        } else {
          sm_.add_instruction(Instruction(OpCode::STOREVAR, get_index_for_identifier(node.identifier())));
        }
      }

      void visit(const Literal& node) override
      {
        sm_.add_instruction(Instruction(OpCode::PUSH, node.value()));
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
        node.condition().accept(*this);
        Label label{"l" + std::to_string(label_counter_++)};
        sm_.add_instruction(Instruction{OpCode::BNE, label});
        for (const auto& statement : node.then_body()) {
          statement->accept(*this);
        }
        sm_.add_instruction(Instruction(OpCode::NOP));
        sm_.add_label(label);
      }

      void visit(const While& node) override
      {
        Label label1{"l" + std::to_string(label_counter_++)};
        Label label2{"l" + std::to_string(label_counter_++)};

        sm_.add_instruction(Instruction(OpCode::NOP));
        sm_.add_label(label1);
        node.condition().accept(*this);
        sm_.add_instruction(Instruction{OpCode::BNE, label2});
        visit(node.body());
        sm_.add_instruction(Instruction{OpCode::JMP, label1});
        sm_.add_instruction(Instruction(OpCode::NOP));
        sm_.add_label(label2);
      }

      void visit(const FunctionCall& node) override
      {
        auto mangeled_name = NameMangler::mangle(node.identifier(), static_cast<uint8_t>(node.parameter().size()));
        auto idx = function_registry_.index_for_function(mangeled_name);
        if (idx < 0) {
          throw std::runtime_error{fmt::format("Cannot find function {} in registry ({})", node.identifier(), mangeled_name)};
        }
        uint8_t count{0};
        for (auto it = node.parameter().rbegin(); it != node.parameter().rend(); ++it) {
          (*it)->accept(*this);
          ++count;
        }
        sm_.add_instruction(Instruction(OpCode::PUSH, Variant{static_cast<double>(count)}));
        sm_.add_instruction(Instruction(OpCode::PUSH, Variant{static_cast<double>(idx)}));
        sm_.add_instruction(Instruction(OpCode::FUNC));
      }

      void visit(const Property& node) override
      {
        bool was_assignment = is_assignment_;
        is_assignment_ = false;
        node.lhs().accept(*this);
        is_assignment_ = was_assignment;

        Variant count{0.0};
        std::string prefix = "get";
        if (is_assignment_) {
          prefix = "set";
          count = 1.0;
        }

        auto name = fmt::format("{}_{}", prefix, node.identifier());
        sm_.add_instruction(Instruction(OpCode::PUSH, name));
        sm_.add_instruction(Instruction(OpCode::PUSH, count));
        sm_.add_instruction(Instruction(OpCode::METHOD));
      }

      void visit(const MethodCall& node) override
      {
        uint8_t count{0};
        for (auto it = node.parameter().rbegin(); it != node.parameter().rend(); ++it) {
          (*it)->accept(*this);
          ++count;
        }
        node.lhs().accept(*this);
        sm_.add_instruction(Instruction(OpCode::PUSH, node.identifier()));
        sm_.add_instruction(Instruction(OpCode::PUSH, Variant{static_cast<double>(count)}));
        sm_.add_instruction(Instruction(OpCode::METHOD));
      }

      void visit(const Constructor& node) override
      {
        auto meta_class = meta_class_registry_.find(node.type_name());
        if (!meta_class) {
          throw std::runtime_error{fmt::format("Cannot find class {} in class registry", node.type_name())};
        }
        auto mangeled_name = NameMangler::mangle(meta_class->name() + "_constructor", 1);
        auto idx = function_registry_.index_for_function(mangeled_name);
        if (idx < 0) {
          throw std::runtime_error{fmt::format("Cannot find method {} in registry ({})", node.type_name(), mangeled_name)};
        }
        uint8_t count{0};
        for (auto it = node.parameter().rbegin(); it != node.parameter().rend(); ++it) {
          (*it)->accept(*this);
          ++count;
        }
        sm_.add_instruction(Instruction(OpCode::PUSH, Variant{static_cast<double>(count)}));
        sm_.add_instruction(Instruction(OpCode::PUSH, Variant{static_cast<double>(idx)}));
        sm_.add_instruction(Instruction(OpCode::FUNC));
      }

      uint32_t get_index_for_identifier(const std::string& identifier)
      {
        auto it = id_mapping_.find(identifier);
        if (it != id_mapping_.end()) {
          return it->second;
        }

        id_mapping_.emplace(std::make_pair(identifier, id_counter_));
        return id_counter_++;
      }

      bool is_assignment_{false};
      FunctionRegistry& function_registry_;
      MetaClassRegistry& meta_class_registry_;
      StackMachine sm_{function_registry_};
      uint32_t label_counter_{1};
      std::map<std::string, uint32_t> id_mapping_;
      uint32_t id_counter_{0};
    };
  }
}
