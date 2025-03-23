#pragma once

#include "parser.hpp"
#include <string>

#ifdef PRINT_TABLE
namespace TablePrinter {
constexpr std::size_t width = 30;

void PrintTitle();

void PrintStack(const std::stack<int>& stack);

void PrintSymbols(const std::vector<std::string>& symbols);

std::string GetStringAction(SyntaxAnalyzer::LR_Parser::ActionType action);

void PrintTableRow(const std::stack<int>& stack, const std::vector<std::string>& symbols, 
    const std::string& input, SyntaxAnalyzer::LR_Parser::ActionType action);
}
#endif

namespace SyntaxAnalyzer {
LR_Parser::TerminalsType GetTerminalType(const LexicalAnalyzer::Lexer::Lexeme& lexeme);

std::pair<LR_Parser::TerminalsType, std::size_t> GetProduction(LR_Parser::ActionType action);

#ifdef PRINT_TABLE
void GetSymbolsAfterReduce(LR_Parser::ActionType action, std::vector<std::string>& symbols);
#endif
}

#ifdef PRINT_TABLE
namespace LexicalAnalyzer {
std::string GetSubstringFromTokenIndex(std::size_t token_index, const std::vector<Lexer::Lexeme>& tokens);

std::string GetCurrentSymbol(const Lexer::Lexeme& lexeme);
}
#endif