#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <variant>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <functional>
#include "lexer.hpp"

namespace ParseTree {
class Node {

    public:
        enum class NonTerminalsType {
            E, 
            T, 
            F,
        };
        
        using DataType = std::variant<LexicalAnalyzer::Lexer::Lexeme, NonTerminalsType>;

        explicit Node(const DataType& data) : data(data) {}
        explicit Node(DataType&& data) : data(std::move(data)) {}

        const DataType& GetData() const noexcept {

            return data;
        }

        DataType& GetData() noexcept {

            return data;
        }

        void AddChild(std::vector<std::unique_ptr<Node>>&& new_children) {

            children = std::move(new_children);
        }

        const std::vector<std::unique_ptr<Node>>& GetChildren() const noexcept {

            return children;
        }

    private:
        DataType data;
        std::vector<std::unique_ptr<Node>> children;
    };


class TreePrinter {

    public:
        void PrintTree(const Node& node, const std::string& file_output_tree_name) const {

            std::ostringstream oss;
            GetPreOrder(node, oss);

            std::ofstream out(file_output_tree_name);
            if (!out) {
                throw std::runtime_error("Can't open text output file with tree");
                return;
            }

            out << oss.str() << "\n";

            out.close();
        }
    
        void GenerateDotFile(const Node& node, const std::string& filename) const {

            std::ofstream dot_file(filename);
            if (dot_file.is_open()) {
                dot_file << "digraph Tree {\n";
    
                std::unordered_map<const Node*, std::size_t> node_ids;
                std::size_t counter{};
    
                GenerateDotFile(node, dot_file, node_ids, counter);
    
                dot_file << "}\n";
            } else {
                std::ostringstream error_message;
                error_message << "Can't open file: " << filename;
                throw std::invalid_argument(error_message.str());
            }
        }
    
    private:
        void GetPreOrder(const Node& node, std::ostringstream& oss) const {

            oss << NodeDataToString(node.GetData()) << " ";
    
            for (const auto& child : node.GetChildren()) {
                GetPreOrder(*child, oss);
            }
        }
    
        void GenerateDotFile(const Node& node, std::ofstream& dot_file,
                            std::unordered_map<const Node*, std::size_t>& node_ids,
                            std::size_t& counter) const {

            if (node_ids.find(&node) == node_ids.end()) {
                node_ids[&node] = counter++;
            }
    
            const std::size_t& node_id = node_ids[&node];
    
            dot_file << "  " << node_id << " [label=\"" << NodeDataToString(node.GetData()) << "\"];\n";
    
            for (const auto& child : node.GetChildren()) {
                if (node_ids.find(child.get()) == node_ids.end()) {
                    node_ids[child.get()] = counter++;
                }
    
                const std::size_t& child_id = node_ids[child.get()];
    
                dot_file << "  " << node_id << " -> " << child_id << ";\n";
    
                GenerateDotFile(*child, dot_file, node_ids, counter);
            }
        }
    
        static std::string NodeDataToString(const Node::DataType& data) {

            if (std::holds_alternative<Node::NonTerminalsType>(data)) {
                switch (std::get<Node::NonTerminalsType>(data)) {
                    case Node::NonTerminalsType::E: return "E";
                    case Node::NonTerminalsType::T: return "T";
                    case Node::NonTerminalsType::F: return "F";
                    default: break;
                }
            } else {
                return LexicalAnalyzer::GetCurrentSymbol(std::get<LexicalAnalyzer::Lexer::Lexeme>(data));
            }

            return "";
        }
    };
}