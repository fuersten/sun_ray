//
//  parser_test.cpp
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 02.02.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#include <sun_ray/script/parser.h>
#include <sun_ray/script/visitor/dump_visitor.h>

#include <iostream>
#include <sstream>

#include <catch2/catch.hpp>


namespace
{
  const sunray::script::Identifier* get_identifier(const sunray::script::Assignment* node)
  {
    REQUIRE(node != nullptr);
    auto identifier = dynamic_cast<const sunray::script::Identifier*>(&(node->lhs()));
    REQUIRE(identifier);
    return identifier;
  }
  const sunray::script::Identifier* get_identifier(const sunray::script::Property* node)
  {
    REQUIRE(node != nullptr);
    auto identifier = dynamic_cast<const sunray::script::Identifier*>(&(node->lhs()));
    REQUIRE(identifier);
    return identifier;
  }
  const std::string& get_identifier(const sunray::script::Expression* node)
  {
    auto identifier = dynamic_cast<const sunray::script::Identifier*>(node);
    REQUIRE(identifier);
    return identifier->identifier();
  }
  const sunray::script::Expression* get_expression_from_statement(const sunray::script::Statement* statement)
  {
    const auto* exp = dynamic_cast<const sunray::script::ExpressionStatement*>(statement);
    REQUIRE(exp);
    return &exp->expression();
  }
}

TEST_CASE("parse assignment", "[parser]")
{
  sunray::script::DiagnosticMessageHandler diagnostic_messages;
  sunray::script::Parser parser{diagnostic_messages};

  SECTION("parse assignment from double")
  {
    const std::string input = "x = 42.0";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    auto node = dynamic_cast<const sunray::script::Assignment*>(nodes[0].get());
    const auto* identifier = get_identifier(node);
    CHECK(identifier->identifier() == "x");
    REQUIRE_NOTHROW(dynamic_cast<const sunray::script::Literal&>(node->expression()));
    const auto& exp_node = dynamic_cast<const sunray::script::Literal&>(node->expression());
    REQUIRE(sunray::script::is_double(exp_node.value()));
    CHECK(sunray::script::as_double(exp_node.value()) == Approx(42));
  }
  SECTION("parse assignment from string")
  {
    const std::string input = "s = 'hutzli'";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    auto node = dynamic_cast<const sunray::script::Assignment*>(nodes[0].get());
    const auto* identifier = get_identifier(node);
    CHECK(identifier->identifier() == "s");
    REQUIRE_NOTHROW(dynamic_cast<const sunray::script::Literal&>(node->expression()));
    const auto& exp_node = dynamic_cast<const sunray::script::Literal&>(node->expression());
    REQUIRE(sunray::script::is_string(exp_node.value()));
    CHECK(sunray::script::as_string(exp_node.value()) == "hutzli");
  }
  SECTION("parse assignment from function call")
  {
    const std::string input = "s = print('Hello, {}!', 'world')";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    auto node = dynamic_cast<const sunray::script::Assignment*>(nodes[0].get());
    const auto* identifier = get_identifier(node);
    CHECK(identifier->identifier() == "s");
    REQUIRE_NOTHROW(dynamic_cast<const sunray::script::FunctionCall&>(node->expression()));
    const auto& func_node = dynamic_cast<const sunray::script::FunctionCall&>(node->expression());
    CHECK(func_node.identifier() == "print");
    CHECK(func_node.parameter().size() == 2);
  }
  SECTION("parse assignment from typename")
  {
    const std::string input = "orange = Color(1, 0.647, 0)";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    auto node = dynamic_cast<const sunray::script::Assignment*>(nodes[0].get());
    const auto* identifier = get_identifier(node);
    CHECK(identifier->identifier() == "orange");
    REQUIRE_NOTHROW(dynamic_cast<const sunray::script::Constructor&>(node->expression()));
    const auto& constructor_node = dynamic_cast<const sunray::script::Constructor&>(node->expression());
    CHECK(constructor_node.type_name() == "Color");
    CHECK(constructor_node.parameter().size() == 3);
  }
  SECTION("parse assignment from property")
  {
    const std::string input = "color_red = orange.red";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    auto node = dynamic_cast<const sunray::script::Assignment*>(nodes[0].get());
    const auto* identifier = get_identifier(node);
    CHECK(identifier->identifier() == "color_red");
    REQUIRE_NOTHROW(dynamic_cast<const sunray::script::Property&>(node->expression()));
    const auto& property_node = dynamic_cast<const sunray::script::Property&>(node->expression());
    CHECK(property_node.identifier() == "red");
    identifier = get_identifier(&property_node);
    CHECK(identifier->identifier() == "orange");
  }
  SECTION("parse assignment from point")
  {
    const std::string input = "location = (1, 1.8, 0)";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    auto node = dynamic_cast<const sunray::script::Assignment*>(nodes[0].get());
    const auto* identifier = get_identifier(node);
    CHECK(identifier->identifier() == "location");
    auto constructor_node = dynamic_cast<const sunray::script::Constructor*>(&node->expression());
    REQUIRE(constructor_node != nullptr);
    CHECK(constructor_node->type_name() == "Point");
    CHECK(constructor_node->parameter().size() == 3);
  }
  SECTION("parse assignment from vector")
  {
    const std::string input = "direction = [1, 1.8, 0]";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    auto node = dynamic_cast<const sunray::script::Assignment*>(nodes[0].get());
    const auto* identifier = get_identifier(node);
    CHECK(identifier->identifier() == "direction");
    auto constructor_node = dynamic_cast<const sunray::script::Constructor*>(&node->expression());
    REQUIRE(constructor_node != nullptr);
    CHECK(constructor_node->type_name() == "Vector");
    CHECK(constructor_node->parameter().size() == 3);
  }
  SECTION("parse if")
  {
    const std::string input = "if (x > 2) x = 42 y = y + 1 end";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    auto node = dynamic_cast<const sunray::script::IfCondition*>(nodes[0].get());
    REQUIRE(node);
  }
  SECTION("parse while")
  {
    const std::string input = "while (x < 3) x = x + 1 y = 42 end";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    auto node = dynamic_cast<const sunray::script::While*>(nodes[0].get());
    REQUIRE(node);
  }
}

TEST_CASE("parse expressions", "[parser]")
{
  sunray::script::DiagnosticMessageHandler diagnostic_messages;
  sunray::script::Parser parser{diagnostic_messages};

  SECTION("parse logical and expression")
  {
    const std::string input = "x and true";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    const auto* node = dynamic_cast<const sunray::script::LogicalExpression*>(get_expression_from_statement(nodes[0].get()));
    REQUIRE(node != nullptr);
    CHECK_NOTHROW(dynamic_cast<const sunray::script::Identifier&>(node->lhs()));
    CHECK_NOTHROW(dynamic_cast<const sunray::script::Literal&>(node->rhs()));
    CHECK(node->op() == sunray::script::LogicalOperator::AND);
  }
  SECTION("parse logical or expression")
  {
    const std::string input = "false or true";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    const auto* node = dynamic_cast<const sunray::script::LogicalExpression*>(get_expression_from_statement(nodes[0].get()));
    REQUIRE(node != nullptr);
    CHECK_NOTHROW(dynamic_cast<const sunray::script::Literal&>(node->lhs()));
    CHECK_NOTHROW(dynamic_cast<const sunray::script::Literal&>(node->rhs()));
    CHECK(node->op() == sunray::script::LogicalOperator::OR);
  }
  SECTION("parse logical not expression")
  {
    const std::string input = "not true";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    const auto* node = dynamic_cast<const sunray::script::UnaryExpression*>(get_expression_from_statement(nodes[0].get()));
    REQUIRE(node != nullptr);
    CHECK_NOTHROW(dynamic_cast<const sunray::script::Literal&>(node->rhs()));
    CHECK(node->op() == sunray::script::UnaryOperator::NOT);
  }
  SECTION("parse neg expression")
  {
    const std::string input = "-4711";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    const auto* node = dynamic_cast<const sunray::script::UnaryExpression*>(get_expression_from_statement(nodes[0].get()));
    REQUIRE(node != nullptr);
    CHECK_NOTHROW(dynamic_cast<const sunray::script::Literal&>(node->rhs()));
    CHECK(node->op() == sunray::script::UnaryOperator::MINUS);
  }
  SECTION("parse greater equal expression")
  {
    const std::string input = "8 >= 9";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    const auto* node = dynamic_cast<const sunray::script::ConditionalExpression*>(get_expression_from_statement(nodes[0].get()));
    REQUIRE(node != nullptr);
    CHECK_NOTHROW(dynamic_cast<const sunray::script::Literal&>(node->lhs()));
    CHECK_NOTHROW(dynamic_cast<const sunray::script::Literal&>(node->rhs()));
    CHECK(node->op() == sunray::script::ConditionalOperator::GE);
  }
  SECTION("parse less equal expression")
  {
    const std::string input = "8 <= 9";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    const auto* node = dynamic_cast<const sunray::script::ConditionalExpression*>(get_expression_from_statement(nodes[0].get()));
    REQUIRE(node != nullptr);
    CHECK_NOTHROW(dynamic_cast<const sunray::script::Literal&>(node->lhs()));
    CHECK_NOTHROW(dynamic_cast<const sunray::script::Literal&>(node->rhs()));
    CHECK(node->op() == sunray::script::ConditionalOperator::LE);
  }
  SECTION("parse conditional expression")
  {
    const std::string input = "a + 1 > b - 3";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    const auto* node = dynamic_cast<const sunray::script::ConditionalExpression*>(get_expression_from_statement(nodes[0].get()));
    REQUIRE(node != nullptr);
    CHECK_NOTHROW(dynamic_cast<const sunray::script::BinaryExpression&>(node->lhs()));
    CHECK_NOTHROW(dynamic_cast<const sunray::script::BinaryExpression&>(node->rhs()));
    CHECK(node->op() == sunray::script::ConditionalOperator::GT);
  }
  SECTION("parse relational expression")
  {
    const std::string input = "x > 4 and y < 3";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    const auto* node = dynamic_cast<const sunray::script::LogicalExpression*>(get_expression_from_statement(nodes[0].get()));
    REQUIRE(node != nullptr);
    CHECK_NOTHROW(dynamic_cast<const sunray::script::ConditionalExpression&>(node->lhs()));
    CHECK_NOTHROW(dynamic_cast<const sunray::script::ConditionalExpression&>(node->rhs()));
    CHECK(node->op() == sunray::script::LogicalOperator::AND);
  }
  SECTION("parse function expression")
  {
    const std::string input = "print('No: {}', 47.11)";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    const auto* node = dynamic_cast<const sunray::script::FunctionCall*>(get_expression_from_statement(nodes[0].get()));
    REQUIRE(node != nullptr);
    CHECK(node->identifier() == "print");
    REQUIRE(node->parameter().size() == 2);
    const auto* exp_node = dynamic_cast<const sunray::script::Literal*>(node->parameter()[0].get());
    REQUIRE(exp_node);
    CHECK(sunray::script::is_string(exp_node->value()));
    CHECK(sunray::script::as_string(exp_node->value()) == "No: {}");
    exp_node = dynamic_cast<const sunray::script::Literal*>(node->parameter()[1].get());
    REQUIRE(exp_node);
    CHECK(sunray::script::is_double(exp_node->value()));
    CHECK(sunray::script::as_double(exp_node->value()) == Approx(47.11));
  }
  SECTION("parse method expression")
  {
    const std::string input = "red = Color(1, 0, 0)\ncanvas = Canvas(100, 50)\ncanvas.set_pixel(10, 20, red)";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 3);
    const auto* node = dynamic_cast<const sunray::script::Assignment*>(nodes[0].get());
    CHECK(get_identifier(node)->identifier() == "red");
    node = dynamic_cast<const sunray::script::Assignment*>(nodes[1].get());
    CHECK(get_identifier(node)->identifier() == "canvas");
    const auto method_node = dynamic_cast<const sunray::script::MethodCall*>(get_expression_from_statement(nodes[2].get()));
    REQUIRE(method_node != nullptr);
    CHECK(get_identifier(&method_node->lhs()) == "canvas");
    CHECK(method_node->identifier() == "set_pixel");
    REQUIRE(method_node->parameter().size() == 3);
    const auto* exp_node = dynamic_cast<const sunray::script::Literal*>(method_node->parameter()[0].get());
    REQUIRE(exp_node);
    CHECK(sunray::script::is_double(exp_node->value()));
    CHECK(sunray::script::as_double(exp_node->value()) == Approx(10));
    exp_node = dynamic_cast<const sunray::script::Literal*>(method_node->parameter()[1].get());
    REQUIRE(exp_node);
    CHECK(sunray::script::is_double(exp_node->value()));
    CHECK(sunray::script::as_double(exp_node->value()) == Approx(20));
    const auto* id_node = dynamic_cast<const sunray::script::Identifier*>(method_node->parameter()[2].get());
    REQUIRE(id_node);
    CHECK(id_node->identifier() == "red");
  }
  SECTION("parse method chain")
  {
    const std::string input = "Vector(1, 1.8, 0).normalize().multiply(11.25)";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    auto method_node = dynamic_cast<const sunray::script::MethodCall*>(get_expression_from_statement(nodes[0].get()));
    REQUIRE(method_node != nullptr);
    CHECK(method_node->identifier() == "multiply");
    REQUIRE(method_node->parameter().size() == 1);
    auto exp_node = dynamic_cast<const sunray::script::Literal*>(method_node->parameter()[0].get());
    REQUIRE(exp_node);
    CHECK(sunray::script::is_double(exp_node->value()));
    CHECK(sunray::script::as_double(exp_node->value()) == Approx(11.25));
    method_node = dynamic_cast<const sunray::script::MethodCall*>(&method_node->lhs());
    REQUIRE(method_node != nullptr);
    CHECK(method_node->identifier() == "normalize");
    REQUIRE(method_node->parameter().size() == 0);
    auto constructor_node = dynamic_cast<const sunray::script::Constructor*>(&method_node->lhs());
    REQUIRE(constructor_node != nullptr);
    CHECK(constructor_node->type_name() == "Vector");
    CHECK(constructor_node->parameter().size() == 3);
  }
  SECTION("parse point")
  {
    const std::string input = "(1, 1.8, 0)";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    auto constructor_node = dynamic_cast<const sunray::script::Constructor*>(get_expression_from_statement(nodes[0].get()));
    REQUIRE(constructor_node != nullptr);
    CHECK(constructor_node->type_name() == "Point");
    CHECK(constructor_node->parameter().size() == 3);
  }
  SECTION("parse vector")
  {
    const std::string input = "[1, 1.8, 0]";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 1);
    auto constructor_node = dynamic_cast<const sunray::script::Constructor*>(get_expression_from_statement(nodes[0].get()));
    REQUIRE(constructor_node != nullptr);
    CHECK(constructor_node->type_name() == "Vector");
    CHECK(constructor_node->parameter().size() == 3);
  }
}

TEST_CASE("parse simple programs", "[parser]")
{
  sunray::script::DiagnosticMessageHandler diagnostic_messages;
  sunray::script::Parser parser{diagnostic_messages};

  SECTION("parse simple calculation")
  {
    const std::string input = R"(
    x = 42
    y = 2
    z = ((x + y) * 10) / 2
    print('z = {}', z)
)";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    REQUIRE(nodes.size() == 4);

    auto node = dynamic_cast<const sunray::script::Assignment*>(nodes[0].get());
    const auto* identifier = get_identifier(node);
    CHECK(identifier->identifier() == "x");

    node = dynamic_cast<const sunray::script::Assignment*>(nodes[1].get());
    identifier = get_identifier(node);
    CHECK(identifier->identifier() == "y");

    node = dynamic_cast<const sunray::script::Assignment*>(nodes[2].get());
    identifier = get_identifier(node);
    CHECK(identifier->identifier() == "z");

    const auto* exp = dynamic_cast<const sunray::script::FunctionCall*>(get_expression_from_statement(nodes[3].get()));
    REQUIRE(exp != nullptr);
    CHECK(exp->identifier() == "print");
    REQUIRE(exp->parameter().size() == 2);
  }
}

TEST_CASE("parse errors", "[parser]")
{
  sunray::script::DiagnosticMessageHandler diagnostic_messages;
  sunray::script::Parser parser{diagnostic_messages};

  SECTION("parse empty input")
  {
    const std::string input = "";
    std::istringstream is{input};

    auto nodes = parser.parse(is);
    CHECK(nodes.empty());
  }
  SECTION("parse premature end of input")
  {
    const std::string input = "check = Color";
    std::istringstream is{input};

    parser.parse(is);
    CHECK(diagnostic_messages.has_error());
    std::stringstream ss;
    diagnostic_messages.output_messages(ss);
    CHECK(
      ss.str() ==
      "ERROR [E006]: didn't expect eoi, but one of left paren [1:14]\nERROR [E001]: expected at least one statement [1:14]\n");
  }
  SECTION("parse missing input")
  {
    const std::string input = "check =";
    std::istringstream is{input};

    parser.parse(is);
    CHECK(diagnostic_messages.has_error());
    std::stringstream ss;
    diagnostic_messages.output_messages(ss);
    CHECK(ss.str() == "ERROR [E004]: expected primary expression [1:9]\nERROR [E001]: expected at least one statement [1:9]\n");
  }
}
