//
//  ast.h
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

#include <sun_ray/script/token.h>
#include <sun_ray/script/types.h>

#include <memory>
#include <vector>


namespace sunray
{
  namespace script
  {
    class Node;
    class Statement;
    class Expression;
    class RelationalExpression;
    class LogicalExpression;
    class ConditionalExpression;
    class SimpleConditionalExpression;
    class BinaryExpression;
    class UnaryExpression;
    class Identifier;
    class Literal;
    class Assignment;
    class IfCondition;
    class While;
    class FunctionCall;
    class Property;
    class MethodCall;
    class Constructor;

    typedef std::unique_ptr<Node> NodePtr;
    typedef std::vector<NodePtr> Nodes;
    typedef std::unique_ptr<Statement> StatementPtr;
    typedef std::vector<StatementPtr> Statements;
    typedef std::unique_ptr<Expression> ExpressionPtr;
    typedef std::unique_ptr<RelationalExpression> RelationalExpressionPtr;


    class Visitor
    {
    public:
      virtual ~Visitor()
      {
      }

      Visitor(const Visitor&) = delete;
      Visitor& operator=(const Visitor&) = delete;
      Visitor(Visitor&&) = delete;
      Visitor& operator=(Visitor&&) = delete;

      virtual void visit(const BinaryExpression& node) = 0;
      virtual void visit(const LogicalExpression& node) = 0;
      virtual void visit(const ConditionalExpression& node) = 0;
      virtual void visit(const SimpleConditionalExpression& node) = 0;
      virtual void visit(const UnaryExpression& node) = 0;
      virtual void visit(const Identifier& node) = 0;
      virtual void visit(const Literal& node) = 0;
      virtual void visit(const Assignment& node) = 0;
      virtual void visit(const IfCondition& node) = 0;
      virtual void visit(const While& node) = 0;
      virtual void visit(const FunctionCall& node) = 0;
      virtual void visit(const Property& node) = 0;
      virtual void visit(const MethodCall& node) = 0;
      virtual void visit(const Constructor& node) = 0;

    protected:
      Visitor() = default;
    };


    class Node
    {
    public:
      Node(const Node&) = delete;
      Node& operator=(const Node&) = delete;
      Node(Node&&) = delete;
      Node& operator=(Node&&) = delete;

      virtual ~Node()
      {
      }

      virtual void accept(Visitor& visitor) const = 0;

      const Location& location() const
      {
        return loc_;
      }

    protected:
      Node(const Location& loc)
      : loc_(loc)
      {
      }

      Location loc_;
    };


    class Statement : public Node
    {
    public:
    protected:
      Statement(const Location& loc)
      : Node(loc)
      {
      }
    };


    class Expression : public Node
    {
    protected:
      Expression(const Location& loc)
      : Node(loc)
      {
      }
    };


    class ExpressionStatement : public Statement
    {
    public:
      ExpressionStatement(const Location& loc, ExpressionPtr exp)
      : Statement(loc)
      , exp_{std::move(exp)}
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        exp_->accept(visitor);
      }

      const Expression& expression() const
      {
        return *exp_;
      }

    private:
      ExpressionPtr exp_;
    };


    class Identifier : public Expression
    {
    public:
      explicit Identifier(const Location& loc, const std::string& identifier)
      : Expression(loc)
      , identifier_(identifier)
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      const std::string& identifier() const
      {
        return identifier_;
      }

    private:
      std::string identifier_;
    };


    class Literal : public Expression
    {
    public:
      explicit Literal(const Location& loc, Variant v)
      : Expression(loc)
      , value_(std::move(v))
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      Variant value() const
      {
        return value_;
      }

    private:
      Variant value_;
    };


    class Constructor : public Expression
    {
    public:
      Constructor(const Location& loc, std::string type_name, std::vector<ExpressionPtr> parameter)
      : Expression(loc)
      , type_name_{std::move(type_name)}
      , parameter_{std::move(parameter)}
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      const std::string& type_name() const
      {
        return type_name_;
      }

      const std::vector<ExpressionPtr>& parameter() const
      {
        return parameter_;
      }

    private:
      std::string type_name_;
      std::vector<ExpressionPtr> parameter_;
    };


    class FunctionCall : public Expression
    {
    public:
      FunctionCall(const Location& loc, std::string identifier, std::vector<ExpressionPtr> parameter)
      : Expression(loc)
      , identifier_{std::move(identifier)}
      , parameter_{std::move(parameter)}
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      const std::string& identifier() const
      {
        return identifier_;
      }

      const std::vector<ExpressionPtr>& parameter() const
      {
        return parameter_;
      }

    private:
      std::string identifier_;
      std::vector<ExpressionPtr> parameter_;
    };


    class Property : public Expression
    {
    public:
      Property(const Location& loc, ExpressionPtr lhs, std::string identifier)
      : Expression(loc)
      , lhs_{std::move(lhs)}
      , identifier_{std::move(identifier)}
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      const Expression& lhs() const
      {
        return *lhs_;
      }

      const std::string& identifier() const
      {
        return identifier_;
      }

    private:
      ExpressionPtr lhs_;
      std::string identifier_;
    };


    class MethodCall : public Expression
    {
    public:
      MethodCall(const Location& loc, ExpressionPtr lhs, std::string identifier, std::vector<ExpressionPtr> parameter)
      : Expression(loc)
      , lhs_{std::move(lhs)}
      , identifier_{std::move(identifier)}
      , parameter_{std::move(parameter)}
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      const Expression& lhs() const
      {
        return *lhs_;
      }

      const std::string& identifier() const
      {
        return identifier_;
      }

      const std::vector<ExpressionPtr>& parameter() const
      {
        return parameter_;
      }

    private:
      ExpressionPtr lhs_;
      std::string identifier_;
      std::vector<ExpressionPtr> parameter_;
    };


    class Assignment : public Statement
    {
    public:
      explicit Assignment(const Location& loc, ExpressionPtr lhs, ExpressionPtr exp)
      : Statement(loc)
      , lhs_(std::move(lhs))
      , expression_(std::move(exp))
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      const Expression& lhs() const
      {
        return *lhs_;
      }

      const Expression& expression() const
      {
        return *expression_;
      }

    private:
      ExpressionPtr lhs_;
      ExpressionPtr expression_;
    };


    class IfCondition : public Statement
    {
    public:
      explicit IfCondition(const Location& loc, RelationalExpressionPtr condition, Statements then_body)
      : Statement(loc)
      , condition_(std::move(condition))
      , then_body_(std::move(then_body))
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      const RelationalExpression& condition() const
      {
        return *condition_;
      }

      const Statements& then_body() const
      {
        return then_body_;
      }

    private:
      RelationalExpressionPtr condition_;
      Statements then_body_;
    };


    class While : public Statement
    {
    public:
      explicit While(const Location& loc, RelationalExpressionPtr condition, Statements body)
      : Statement(loc)
      , condition_(std::move(condition))
      , body_(std::move(body))
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      const RelationalExpression& condition() const
      {
        return *condition_;
      }

      const Statements& body() const
      {
        return body_;
      }

    private:
      RelationalExpressionPtr condition_;
      Statements body_;
    };


    class BinaryExpression : public Expression
    {
    public:
      explicit BinaryExpression(const Location& loc, BinaryOperator op, ExpressionPtr expLhs, ExpressionPtr expRhs)
      : Expression(loc)
      , operator_(op)
      , lhs_(std::move(expLhs))
      , rhs_(std::move(expRhs))
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      BinaryOperator op() const
      {
        return operator_;
      }

      const Expression& lhs() const
      {
        return *lhs_;
      }

      const Expression& rhs() const
      {
        return *rhs_;
      }

    private:
      BinaryOperator operator_;
      ExpressionPtr lhs_;
      ExpressionPtr rhs_;
    };


    class UnaryExpression : public Expression
    {
    public:
      explicit UnaryExpression(const Location& loc, UnaryOperator op, ExpressionPtr expRhs)
      : Expression(loc)
      , operator_(op)
      , rhs_(std::move(expRhs))
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      UnaryOperator op() const
      {
        return operator_;
      }

      const Expression& rhs() const
      {
        return *rhs_;
      }

    private:
      UnaryOperator operator_;
      ExpressionPtr rhs_;
    };


    class RelationalExpression : public Expression
    {
    protected:
      RelationalExpression(const Location& loc)
      : Expression(loc)
      {
      }
    };


    class SimpleConditionalExpression : public RelationalExpression
    {
    public:
      explicit SimpleConditionalExpression(const Location& loc, ExpressionPtr exp)
      : RelationalExpression(loc)
      , exp_(std::move(exp))
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      const Expression& rhs() const
      {
        return *exp_;
      }

    private:
      ExpressionPtr exp_;
    };


    class LogicalExpression : public RelationalExpression
    {
    public:
      explicit LogicalExpression(const Location& loc, LogicalOperator op, ExpressionPtr expLhs, ExpressionPtr expRhs)
      : RelationalExpression(loc)
      , operator_(op)
      , lhs_(std::move(expLhs))
      , rhs_(std::move(expRhs))
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      LogicalOperator op() const
      {
        return operator_;
      }

      const Expression& lhs() const
      {
        return *lhs_;
      }

      const Expression& rhs() const
      {
        return *rhs_;
      }

    private:
      LogicalOperator operator_;
      ExpressionPtr lhs_;
      ExpressionPtr rhs_;
    };


    class ConditionalExpression : public RelationalExpression
    {
    public:
      explicit ConditionalExpression(const Location& loc, ConditionalOperator op, ExpressionPtr expLhs, ExpressionPtr expRhs)
      : RelationalExpression(loc)
      , operator_(op)
      , lhs_(std::move(expLhs))
      , rhs_(std::move(expRhs))
      {
      }

      virtual void accept(Visitor& visitor) const
      {
        visitor.visit(*this);
      }

      ConditionalOperator op() const
      {
        return operator_;
      }

      const Expression& lhs() const
      {
        return *lhs_;
      }

      const Expression& rhs() const
      {
        return *rhs_;
      }

    private:
      ConditionalOperator operator_;
      ExpressionPtr lhs_;
      ExpressionPtr rhs_;
    };
  }
}
