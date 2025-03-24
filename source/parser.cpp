#include <array>
#include <cstddef>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <optional>
#include <string>
#include <algorithm>
#include "../include/utils.hpp"

namespace SyntaxAnalyzer {
// #ifdef PRINT_TABLE
    using namespace TablePrinter;
// #endif
using namespace LexicalAnalyzer;

void LR_Parser::Parse(Lexer& lexer) {

    stack.push(0);

    // #ifdef PRINT_TABLE
        PrintTitle();
    // #endif

    while (true) {
        Lexer::Lexeme token = lexer.GetNextToken(current_token_index);

        int condition = stack.top();

        Lexer::Tokens token_type = GetTokenType(token);

        const auto& cur_action_it = action_table.find({condition, token_type});

        if (cur_action_it == action_table.end()) {
            throw std::runtime_error{"Syntax error"};
        }

        const auto& action = cur_action_it->second;

        if (action.action == ActionType::SHIFT) {
            Shift(action, lexer);
            current_token_index++;
        } else if (action.action == ActionType::ACCEPT) {
            if (parse_tree_stack.size() != 1) {
                throw std::runtime_error{"Syntax error"};
            }
            // #ifdef PRINT_TABLE
                PrintTableRow(stack, symbols, GetSubstringFromTokenIndex(current_token_index, lexer.GetTokens()), action.action);
            // #endif
            break;
        } else { 
            Reduce(action, lexer);
        }      
    }
}

void LR_Parser::Shift(const Action& action, const Lexer& lexer) {

    const auto& tokens = lexer.GetTokens();
    const auto& token = tokens[current_token_index];

    // #ifdef PRINT_TABLE
        std::string input = GetSubstringFromTokenIndex(current_token_index, tokens);
        
        PrintTableRow(stack, symbols, input, action.action);

        symbols.emplace_back(GetCurrentSymbol(token));
    // #endif

    stack.push(action.next_cond);

    parse_tree_stack.push(std::make_unique<ParseTree::Node>(token));
}

void LR_Parser::Reduce(const Action& action, const Lexer& lexer) {

    // #ifdef PRINT_TABLE
        PrintTableRow(stack, symbols, GetSubstringFromTokenIndex(current_token_index, lexer.GetTokens()), action.action);

        GetSymbolsAfterReduce(action.action, symbols);
    // #endif

    std::vector<std::unique_ptr<ParseTree::Node>> children;

    const auto& prod = GetProduction(action.action);
    
    for (std::size_t stack_id_pop = 0; stack_id_pop < prod.second; stack_id_pop++) {
        children.push_back(std::move(parse_tree_stack.top()));
        parse_tree_stack.pop();
        stack.pop();
    }

    const auto& new_condition = goto_table.find({stack.top(), prod.first});

    if (new_condition == goto_table.end()) {
        throw std::runtime_error{"Syntax error"};
    }

    std::unique_ptr<ParseTree::Node> node = std::make_unique<ParseTree::Node>(prod.first);
    node->AddChild(std::move(children));
    parse_tree_stack.push(std::move(node));

    stack.push(new_condition->second);
}

const ParseTree::Node& LR_Parser::GetRoot() const {

    if (parse_tree_stack.empty()) {
        throw std::runtime_error("tree is empty");
    }
    return *parse_tree_stack.top();
}           
}