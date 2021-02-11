//
//  dot_dump_visitor.h
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


namespace sunray
{
  namespace script
  {
    class DotNode
    {
    public:
      explicit DotNode(const std::string& id, const std::string& label)
      : id_(id)
      , label_(label)
      {
      }

      std::string id_;
      std::string label_;
    };

    typedef std::shared_ptr<DotNode> DotNodePtr;


    class DotNodeConnection
    {
    public:
      explicit DotNodeConnection(DotNodePtr lhs, const std::string& lTag, DotNodePtr rhs, const std::string& rTag)
      : lhs_(lhs)
      , lhs_tag_(lTag)
      , rhs_(rhs)
      , rhs_tag_(rTag)
      {
      }

      DotNodePtr lhs_;
      std::string lhs_tag_;
      DotNodePtr rhs_;
      std::string rhs_tag_;
    };

    typedef std::vector<DotNodePtr> DotNodes;
    typedef std::vector<DotNodeConnection> DotNodeConnections;


    class DotDumpVisitor : public Visitor
    {
    public:
      explicit DotDumpVisitor(const std::string& unit_name)
      : unit_name_(unit_name)
      {
      }

      void visit(const Nodes& nodes)
      {
        std::stringstream ss;
        ss << "[shape=record,label=\"{program|" << unit_name_ << "}\"]";
        std::string label = ss.str();
        DotNodePtr program = std::make_shared<DotNode>("program", label);
        nodes_.emplace_back(program);

        make_block_node(nodes.size(), nodes);

        DotNodeConnection connection(program, "", current_dot_node_, "");
        connections_.emplace_back(connection);
      }

      void visit(const BinaryExpression& node) override
      {
        std::stringstream nss;
        nss << "[shape=record,label=\"{binary|{<f0>lhs|<f1>op|<f2>rhs}}\"]";
        std::stringstream nid;
        nid << "binary_" << next_index();
        DotNodePtr dot_node = std::make_shared<DotNode>(nid.str(), nss.str());
        nodes_.emplace_back(dot_node);

        {
          node.lhs().accept(*this);
          DotNodeConnection connection(dot_node, "f0", current_dot_node_, "");
          connections_.emplace_back(connection);
        }

        {
          std::stringstream ss;
          ss << "[shape=record,label=\"" << node.op() << "\"]";
          std::stringstream id;
          id << "op_" << next_index();
          DotNodePtr opNode = std::make_shared<DotNode>(id.str(), ss.str());
          nodes_.emplace_back(opNode);
          DotNodeConnection connection(dot_node, "f1", opNode, "");
          connections_.emplace_back(connection);
        }

        {
          node.rhs().accept(*this);
          DotNodeConnection connection(dot_node, "f2", current_dot_node_, "");
          connections_.emplace_back(connection);
        }

        current_dot_node_ = dot_node;
      }

      void visit(const UnaryExpression& node) override
      {
        std::stringstream nss;
        nss << "[shape=record,label=\"{unary|{<f0>op|<f1>exp}}\"]";
        std::stringstream nid;
        nid << "unary_" << next_index();
        DotNodePtr dot_node = std::make_shared<DotNode>(nid.str(), nss.str());
        nodes_.emplace_back(dot_node);

        {
          std::stringstream ss;
          ss << "[shape=record,label=\"" << node.op();
          ss << "\"]";
          std::stringstream id;
          id << "op_" << next_index();
          DotNodePtr opNode = std::make_shared<DotNode>(id.str(), ss.str());
          nodes_.emplace_back(opNode);
          DotNodeConnection connection(dot_node, "f0", opNode, "");
          connections_.emplace_back(connection);
        }
        node.rhs().accept(*this);
        DotNodeConnection connection(dot_node, "f1", current_dot_node_, "");
        connections_.emplace_back(connection);
        current_dot_node_ = dot_node;
      }

      void visit(const Identifier& node) override
      {
        std::stringstream ss;
        ss << "[shape=record,label=\"{identifier|{";
        ss << node.identifier();
        ss << "}}\"]";
        std::stringstream id;
        id << "identifier_" << next_index();
        DotNodePtr dot_node = std::make_shared<DotNode>(id.str(), ss.str());
        nodes_.emplace_back(dot_node);
        current_dot_node_ = dot_node;
      }

      void visit(const Literal& node) override
      {
        std::stringstream ss;
        ss << "[shape=record,label=\"{value|{";

        if (is_double(node.value())) {
          ss << "float"
             << "|" << to_string(node.value()) << "}}\"]";
        } else {
          ss << "string"
             << "|" << to_string(node.value()) << "}}\"]";
        }
        std::stringstream id;
        id << "literal_" << next_index();
        DotNodePtr dot_node = std::make_shared<DotNode>(id.str(), ss.str());
        nodes_.emplace_back(dot_node);
        current_dot_node_ = dot_node;
      }

      void visit(const Assignment& node) override
      {
        const auto& identifier = dynamic_cast<const sunray::script::Identifier&>(node.lhs());

        std::stringstream ss;
        ss << "[shape=record,label=\"{var|{";
        ss << identifier.identifier();
        ss << "|"
           << "float";
        ss << "}}\"]";
        std::stringstream id;
        id << "var_" << next_index();
        DotNodePtr var = std::make_shared<DotNode>(id.str(), ss.str());
        nodes_.emplace_back(var);

        node.expression().accept(*this);

        DotNodeConnection conn(var, "", current_dot_node_, "");
        connections_.emplace_back(conn);

        current_dot_node_ = var;
      }

      void visit(const FunctionCall& node) override
      {
        (void)node;
      }

      void dump_nodes(std::ostream& out) const
      {
        DotNodePtr program;
        for (const auto& node : nodes_) {
          if (node->id_ == "program") {
            program = node;
          }
        }

        if (!program) {
          throw std::runtime_error{"no program node found"};
          return;
        }

        out << "digraph exp {\n";
        out << "  layout=dot;\n";
        out << "  ranksep=0.75;\n";
        out << "  nodesep=0.45;\n";
        out << "  node[shape=record, height=\"0.3\", width=\"0.3\"];\n";
        for (const auto& node : nodes_) {
          out << "  ";
          out << node->id_ << " " << node->label_ << ";\n";
        }
        out << "\n";
        for (const auto& connection : connections_) {
          out << "  ";
          out << connection.lhs_->id_;
          if (!connection.lhs_tag_.empty()) {
            out << ":" << connection.lhs_tag_;
          }
          out << " -> " << connection.rhs_->id_;
          if (!connection.rhs_tag_.empty()) {
            out << ":" << connection.rhs_tag_;
          }
          out << ";\n";
        }
        out << "}\n";
      }

    private:
      template<typename T>
      void make_block_node(size_t node_size, const T& collection)
      {
        std::stringstream ss;
        ss << "[shape=record,label=\"<f0>-\\>|";
        for (size_t n = 0; n < node_size; ++n) {
          ss << "<f" << n + 1 << ">";
          if (n < node_size - 1) {
            ss << "|";
          }
        }
        ss << "\"]";
        std::stringstream id;
        id << "block_" << next_index();
        DotNodePtr dot_node = std::make_shared<DotNode>(id.str(), ss.str());
        nodes_.emplace_back(dot_node);

        size_t n = 0;
        for (const auto& node : collection) {
          if (node) {
            std::stringstream sst;
            sst << "f" << n + 1;
            node->accept(*this);
            DotNodeConnection connection(dot_node, sst.str(), current_dot_node_, "");
            connections_.emplace_back(connection);
            ++n;
          }
        }
        current_dot_node_ = dot_node;
      }

      size_t next_index()
      {
        return ++counter_;
      }

      std::string unit_name_;
      size_t counter_;
      DotNodes nodes_;
      mutable DotNodeConnections connections_;
      DotNodePtr current_dot_node_;
    };
  }
}
