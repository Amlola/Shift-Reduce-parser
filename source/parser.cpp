#include <array>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <optional>
#include <string>
#include "utils.hpp"

namespace SyntaxAnalyzer {
#ifdef PRINT_TABLE
    using namespace TablePrinter;
#endif
using namespace LexicalAnalyzer;

LR_Parser::LR_Parser(const std::string& file_name) {

    #ifdef PRINT_TABLE
        table_file.open(file_name);

        if (!table_file.is_open()) {
            throw std::invalid_argument("Can't open file");
        }
    #else
        throw std::runtime_error("You can't get file with table without -DPRINT_TABLE option");
    #endif
}

void LR_Parser::Parse(Lexer& lexer) {

    stack.push(0);

    #ifdef PRINT_TABLE
        PrintTitle(table_file);
    #endif

    while (true) {
        Lexer::Lexeme token = lexer.GetNextToken(current_token_index);

        int condition = stack.top();
        TerminalsType type = GetTerminalType(token); 

        const auto& cur_action_it = action_table.find({condition, type});

        if (cur_action_it == action_table.end()) {
            #ifdef PRINT_TABLE
                table_file.close();
            #endif
            throw std::runtime_error{"Syntax error"};
        }

        const auto& action = cur_action_it->second;

        if (action.action == ActionType::SHIFT) {
            Shift(action, lexer);
            current_token_index++;
        } else if (action.action == ActionType::ACCEPT) {
            #ifdef PRINT_TABLE
                PrintTableRow(table_file, stack, symbols, GetSubstringFromTokenIndex(current_token_index, lexer.GetTokens()), action.action);
            #endif
            break;
        } else { 
            Reduce(action, lexer);
        }      
    }

    #ifdef PRINT_TABLE
        table_file.close();
    #endif
}

void LR_Parser::Shift(const Action& action, const Lexer& lexer) {

    #ifdef PRINT_TABLE 
        std::string input = GetSubstringFromTokenIndex(current_token_index, lexer.GetTokens());
        
        PrintTableRow(table_file, stack, symbols, input, action.action);

        symbols.emplace_back(GetCurrentSymbol(current_token_index, lexer.GetTokens()));
    #endif

    stack.push(action.next_cond);
}

void LR_Parser::Reduce(const Action& action, const Lexer& lexer) {

    #ifdef PRINT_TABLE
        PrintTableRow(table_file, stack, symbols, GetSubstringFromTokenIndex(current_token_index, lexer.GetTokens()), action.action);

        GetSymbolsAfterReduce(action.action, symbols);
    #endif

    Production prod = GetProduction(action.action);
    
    for (std::size_t stack_id_pop = 0; stack_id_pop < prod.len; stack_id_pop++) {
        stack.pop();
    }

    const auto& new_condition = goto_table.find({stack.top(), prod.header});

    if (new_condition == goto_table.end()) {
        #ifdef PRINT_TABLE
            table_file.close();
        #endif
        throw std::runtime_error{"Syntax error"};
    }

    stack.push(new_condition->second);
}
}