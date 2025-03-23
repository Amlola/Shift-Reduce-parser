#include <array>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <optional>
#include <string>
#include <algorithm>
#include "utils.hpp"

namespace SyntaxAnalyzer {
#ifdef PRINT_TABLE
    using namespace TablePrinter;
#endif
using namespace LexicalAnalyzer;

void LR_Parser::Parse(Lexer& lexer) {

    stack.push(0);

    #ifdef PRINT_TABLE
        PrintTitle();
    #endif

    while (true) {
        Lexer::Lexeme token = lexer.GetNextToken(current_token_index);

        int condition = stack.top();
        TerminalsType type = GetTerminalType(token); 

        const auto& cur_action_it = action_table.find({condition, type});

        if (cur_action_it == action_table.end()) {
            throw std::runtime_error{"Syntax error"};
        }

        const auto& action = cur_action_it->second;

        if (action.action == ActionType::SHIFT) {
            Shift(action, lexer);
            current_token_index++;
        } else if (action.action == ActionType::ACCEPT) {
            #ifdef PRINT_TABLE
                PrintTableRow(stack, symbols, GetSubstringFromTokenIndex(current_token_index, lexer.GetTokens()), action.action);
            #endif
            break;
        } else { 
            Reduce(action, lexer);
        }      
    }
}

void LR_Parser::Shift(const Action& action, const Lexer& lexer) {

    #ifdef PRINT_TABLE 
        std::string input = GetSubstringFromTokenIndex(current_token_index, lexer.GetTokens());
        
        PrintTableRow(stack, symbols, input, action.action);

        symbols.emplace_back(GetCurrentSymbol(lexer.GetTokens()[current_token_index]));
    #endif

    stack.push(action.next_cond);
}

void LR_Parser::Reduce(const Action& action, const Lexer& lexer) {

    #ifdef PRINT_TABLE
        PrintTableRow(stack, symbols, GetSubstringFromTokenIndex(current_token_index, lexer.GetTokens()), action.action);

        GetSymbolsAfterReduce(action.action, symbols);
    #endif

    auto prod = GetProduction(action.action);
    
    for (std::size_t stack_id_pop = 0; stack_id_pop < prod.second; stack_id_pop++) {
        stack.pop();
    }

    const auto& new_condition = goto_table.find({stack.top(), prod.first});

    if (new_condition == goto_table.end()) {
        throw std::runtime_error{"Syntax error"};
    }

    stack.push(new_condition->second);
}
}