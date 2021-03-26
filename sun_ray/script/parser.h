//
//  parser.h
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
#include <sun_ray/script/diagnostic_message_handler.h>
#include <sun_ray/script/scanner.h>


namespace sunray
{
  namespace script
  {
    class ParseError : public std::runtime_error
    {
    public:
      ParseError()
      : std::runtime_error("Parse error")
      {
      }

      ParseError(std::string error)
      : std::runtime_error(std::move(error))
      {
      }
    };


    class Parser
    {
    public:
      Parser(DiagnosticMessageHandler& diagnostic_messages)
      : diagnostic_messages_(diagnostic_messages)
      {
      }

      Parser(const Parser&) = delete;
      Parser& operator=(const Parser&) = delete;
      Parser(Parser&&) = delete;
      Parser& operator=(Parser&&) = delete;

      Statements parse(std::istream& input)
      {
        diagnostic_messages_.reset();

        Statements statements;
        scanner_.reset(new Scanner{input});

        if (parse_next().code_ != TokenCode::EOI) {
          statements = parse_statements();
        }

        if (statements.empty()) {
          diagnostic_messages_.add_error("E001", "expected at least one statement", current_token_.location_);
        }

        return statements;
      }

    private:
      Token& parse_next()
      {
        if (current_token_ == TokenCode::EOI) {
          diagnostic_messages_.add_error("E003", "premature end of input", current_token_.location_);
          throw ParseError{};
        }
        current_token_ = scanner_->get_next_token();
        return current_token_;
      }

      bool expect(std::initializer_list<TokenCode> tokens)
      {
        for (const auto& tok : tokens) {
          if (current_token_ == tok) {
            parse_next();
            return true;
          }
        }

        return false;
      }

      void expect_throw(std::initializer_list<TokenCode> tokens)
      {
        if (!expect(tokens)) {
          std::vector<TokenCode> v;
          v.insert(v.end(), tokens.begin(), tokens.end());
          std::stringstream message;
          message << "didn't expect " << current_token_.code_ << ", but one of ";
          message << join(v, ",");

          diagnostic_messages_.add_error("E006", message.str(), current_token_.location_);
          throw ParseError{};
        }
      }

      bool can_expect(std::initializer_list<TokenCode> tokens)
      {
        if (std::any_of(tokens.begin(), tokens.end(),
                        std::bind(std::equal_to<>(), current_token_.code_, std::placeholders::_1))) {
          return true;
        }

        return false;
      }

      Statements parse_statements()
      {
        Statements statements;

        while (current_token_ != TokenCode::EOI) {
          try {
            auto statement = parse_statement();
            if (statement) {
              statements.emplace_back(std::move(statement));
            }
          } catch (const ParseError&) {
            // TODO: find next sync point
            return statements;
          }
        }

        return statements;
      }

      StatementPtr parse_statement()
      {
        StatementPtr statement;
        if (can_expect({TokenCode::IF})) {
          statement = parse_if();
        } else if (can_expect({TokenCode::WHILE})) {
          statement = parse_while();
        } else {
          auto exp = parse_logical_expression();
          if (can_expect({TokenCode::ASSIGNMENT})) {
            parse_next();
            statement = std::make_unique<Assignment>(exp->location(), std::move(exp), parse_logical_expression());
          } else {
            statement = std::make_unique<ExpressionStatement>(exp->location(), std::move(exp));
          }
        }
        return statement;
      }

      StatementPtr parse_if()
      {
        expect_throw({TokenCode::IF});
        expect_throw({TokenCode::LPAREN});
        auto condition = parse_logical_expression();
        expect_throw({TokenCode::RPAREN});
        Statements body;
        while (!can_expect({TokenCode::END})) {
          body.emplace_back(parse_statement());
        }
        expect_throw({TokenCode::END});
        if (!dynamic_cast<const RelationalExpression*>(condition.get())) {
          std::stringstream message;
          message << "expect a relational expression as if condition";
          diagnostic_messages_.add_error("E009", message.str(), current_token_.location_);
          throw ParseError{};
        }
        auto real_condition = std::unique_ptr<RelationalExpression>(dynamic_cast<RelationalExpression*>(condition.release()));
        return std::make_unique<IfCondition>(real_condition->location(), std::move(real_condition), std::move(body));
      }

      StatementPtr parse_while()
      {
        expect_throw({TokenCode::WHILE});
        expect_throw({TokenCode::LPAREN});
        auto condition = parse_logical_expression();
        expect_throw({TokenCode::RPAREN});
        Statements body;
        while (!can_expect({TokenCode::END})) {
          body.emplace_back(parse_statement());
        }
        expect_throw({TokenCode::END});
        if (!dynamic_cast<const RelationalExpression*>(condition.get())) {
          std::stringstream message;
          message << "expect a relational expression as while condition";
          diagnostic_messages_.add_error("E010", message.str(), current_token_.location_);
          throw ParseError{};
        }
        auto real_condition = std::unique_ptr<RelationalExpression>(dynamic_cast<RelationalExpression*>(condition.release()));
        return std::make_unique<While>(real_condition->location(), std::move(real_condition), std::move(body));
      }

      ExpressionPtr parse_logical_expression()
      {
        auto node = parse_conditional_expression();
        if (can_expect({TokenCode::AND, TokenCode::OR})) {
          LogicalOperator op;
          if (current_token_.code_ == TokenCode::AND) {
            op = LogicalOperator::AND;
          } else if (current_token_.code_ == TokenCode::OR) {
            op = LogicalOperator::OR;
          } else {
            std::stringstream message;
            message << "expect a logical operator (and,or)";
            diagnostic_messages_.add_error("E012", message.str(), current_token_.location_);
            throw ParseError{};
          }
          parse_next();
          node = std::make_unique<LogicalExpression>(node->location(), op, std::move(node), parse_conditional_expression());
        }

        return node;
      }

      ExpressionPtr parse_conditional_expression()
      {
        auto node = parse_additive();
        if (can_expect({TokenCode::GT, TokenCode::LT, TokenCode::GE, TokenCode::LE, TokenCode::EQ})) {
          ConditionalOperator op;
          if (current_token_.code_ == TokenCode::GT) {
            op = ConditionalOperator::GT;
          } else if (current_token_.code_ == TokenCode::LT) {
            op = ConditionalOperator::LT;
          } else if (current_token_.code_ == TokenCode::GE) {
            op = ConditionalOperator::GE;
          } else if (current_token_.code_ == TokenCode::LE) {
            op = ConditionalOperator::LE;
          } else if (current_token_.code_ == TokenCode::EQ) {
            op = ConditionalOperator::EQ;
          } else {
            std::stringstream message;
            message << "expect a conditional operator (<,>,<=,>=,==)";
            diagnostic_messages_.add_error("E011", message.str(), current_token_.location_);
            throw ParseError{};
          }
          parse_next();
          node = std::make_unique<ConditionalExpression>(node->location(), op, std::move(node), parse_additive());
        }

        return node;
      }

      ExpressionPtr parse_additive()
      {
        auto node = parse_term();

        while (can_expect({TokenCode::PLUS, TokenCode::MINUS})) {
          BinaryOperator op = current_token_.code_ == TokenCode::PLUS ? BinaryOperator::ADD : BinaryOperator::SUB;
          parse_next();
          node = std::make_unique<BinaryExpression>(node->location(), op, std::move(node), parse_term());
        }

        return node;
      }

      ExpressionPtr parse_term()
      {
        auto node = parse_factor();

        while (can_expect({TokenCode::DIV, TokenCode::MUL})) {
          BinaryOperator op = current_token_.code_ == TokenCode::DIV ? BinaryOperator::DIV : BinaryOperator::MUL;
          parse_next();
          node = std::make_unique<BinaryExpression>(node->location(), op, std::move(node), parse_factor());
        }

        return node;
      }

      ExpressionPtr parse_factor()
      {
        return parse_unary();
      }

      ExpressionPtr parse_unary()
      {
        Location loc{current_token_.location_};
        if (can_expect({TokenCode::MINUS})) {
          parse_next();
          return std::make_unique<UnaryExpression>(loc, UnaryOperator::MINUS, parse_unary());
        } else if (can_expect({TokenCode::NOT})) {
          parse_next();
          return std::make_unique<UnaryExpression>(loc, UnaryOperator::NOT, parse_unary());
        }
        return parse_postfix();
      }

      std::vector<ExpressionPtr> parameter()
      {
        expect_throw({TokenCode::LPAREN});
        std::vector<ExpressionPtr> parameter;
        bool first{true};
        while (current_token_.code_ != TokenCode::RPAREN) {
          if (first) {
            first = false;
          } else {
            expect_throw({TokenCode::COMMA});
          }
          parameter.emplace_back(parse_conditional_expression());
        }
        expect_throw({TokenCode::RPAREN});

        return parameter;
      }

      ExpressionPtr parse_postfix()
      {
        auto node = parse_primary();

        if (can_expect({TokenCode::LPAREN})) {
          Location loc{node->location()};
          auto identifier = dynamic_cast<Identifier*>(node.get());
          if (!identifier) {
            std::stringstream message;
            message << "expect identifier as function call, property, or method call";
            diagnostic_messages_.add_error("E008", message.str(), current_token_.location_);
            throw ParseError{};
          }
          return std::make_unique<FunctionCall>(loc, std::move(identifier->identifier()), parameter());
        }

        while (can_expect({TokenCode::DOT})) {
          Location loc{node->location()};
          expect_throw({TokenCode::DOT});
          auto identifier = current_token_.identifier_;
          expect_throw({TokenCode::IDENTIFIER});

          if (can_expect({TokenCode::LPAREN})) {
            node = std::make_unique<MethodCall>(loc, std::move(node), std::move(identifier), parameter());
          } else {
            node = std::make_unique<Property>(loc, std::move(node), std::move(identifier));
          }
        }

        if (!node) {
          std::stringstream message;
          message << "expect identifier, constant, function call, property, or method call";
          diagnostic_messages_.add_error("E007", message.str(), current_token_.location_);
          throw ParseError{};
        }

        return node;
      }

      ExpressionPtr parse_primary()
      {
        Location loc{current_token_.location_};
        ExpressionPtr node;

        if (can_expect({TokenCode::LSQUARE})) {
          expect_throw({TokenCode::LSQUARE});
          std::vector<ExpressionPtr> parameter;
          parameter.emplace_back(parse_conditional_expression());
          expect_throw({TokenCode::COMMA});
          parameter.emplace_back(parse_conditional_expression());
          expect_throw({TokenCode::COMMA});
          parameter.emplace_back(parse_conditional_expression());
          expect_throw({TokenCode::RSQUARE});
          return std::make_unique<Constructor>(loc, "Vector", std::move(parameter));
        } else if (can_expect({TokenCode::LPAREN})) {
          expect_throw({TokenCode::LPAREN});
          node = parse_conditional_expression();
          bool is_point{false};
          std::vector<ExpressionPtr> parameter;
          if (can_expect({TokenCode::COMMA})) {
            is_point = true;
            parameter.emplace_back(std::move(node));
            parse_next();
            parameter.emplace_back(parse_conditional_expression());
            expect_throw({TokenCode::COMMA});
            parameter.emplace_back(parse_conditional_expression());
          }
          expect({TokenCode::RPAREN});
          if (is_point) {
            return std::make_unique<Constructor>(loc, "Point", std::move(parameter));
          }
        } else if (can_expect({TokenCode::CONST_LITERAL})) {
          node = std::make_unique<Literal>(loc, current_token_.value_);
          parse_next();
        } else if (can_expect({TokenCode::IDENTIFIER})) {
          node = std::make_unique<Identifier>(loc, current_token_.identifier_);
          parse_next();
        } else if (can_expect({TokenCode::TYPENAME})) {
          auto identifier = current_token_.identifier_;
          parse_next();
          return std::make_unique<Constructor>(loc, std::move(identifier), parameter());
        } else {
          diagnostic_messages_.add_error("E004", "expected primary expression", loc);
          throw ParseError{};
        }

        return node;
      }

      std::unique_ptr<Scanner> scanner_;
      Token current_token_;
      DiagnosticMessageHandler& diagnostic_messages_;
    };
  }
}
