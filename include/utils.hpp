#pragma once

#include "parser.hpp"
#include <string>

#ifdef PRINT_TABLE
namespace TablePrinter {
constexpr std::size_t width = 30;

void PrintTitle(std::ofstream& out);

void PrintStack(const std::stack<int>& stack, std::ofstream& out);

void PrintSymbols(const std::vector<std::string>& symbols, std::ofstream& out);

std::string GetStringAction(SyntaxAnalyzer::LR_Parser::ActionType action);

void PrintTableRow(std::ofstream& out, const std::stack<int>& stack, const std::vector<std::string>& symbols, 
    const std::string& input, SyntaxAnalyzer::LR_Parser::ActionType action);
}
#endif

namespace SyntaxAnalyzer {
LR_Parser::TerminalsType GetTerminalType(const LexicalAnalyzer::Lexer::Lexeme& lexeme);

LR_Parser::Production GetProduction(LR_Parser::ActionType action);

#ifdef PRINT_TABLE
void GetSymbolsAfterReduce(LR_Parser::ActionType action, std::vector<std::string>& symbols);
#endif
}

namespace LexicalAnalyzer {
std::string GetSubstringFromTokenIndex(std::size_t token_index, const std::vector<Lexer::Lexeme>& tokens);

std::string GetCurrentSymbol(std::size_t token_index, const std::vector<Lexer::Lexeme>& tokens);
}