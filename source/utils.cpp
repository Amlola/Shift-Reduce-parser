#include <cstddef>
#include <iomanip>
#include "utils.hpp"

#ifdef PRINT_TABLE
namespace TablePrinter {
using namespace SyntaxAnalyzer;

void PrintTitle() {

    std::cout << std::setw(width) << std::left << "Stack"
              << std::setw(width) << std::left << "Symbols"
              << std::setw(width) << std::left << "Input"
              << std::setw(width) << std::left << "Action\n";
    std::cout << "\n";
}

void PrintStack(const std::stack<int>& stack) {
    
    std::stack<int> temp_stack = stack;
    std::vector<int> elements;

    while (!temp_stack.empty()) {
        elements.push_back(temp_stack.top());
        temp_stack.pop();
    }

    std::string stack_content;
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        stack_content += std::to_string(*it) + " ";
    }

    std::cout << std::setw(width) << std::left << stack_content;
}


void PrintSymbols(const std::vector<std::string>& symbols) {

    std::string symbols_content;

    for (const auto& symb : symbols) {
        symbols_content += symb;
    }

    std::cout << std::setw(width) << std::left << "$" + symbols_content;
}

std::string GetStringAction(LR_Parser::ActionType action) {

    switch (action) {
        case LR_Parser::ActionType::ACCEPT:                return "Accept";
        case LR_Parser::ActionType::REDUCE_E_TO_E_ADD_T:   return "Reduce E->E+T";
        case LR_Parser::ActionType::REDUCE_E_TO_E_SUB_T:   return "Reduce E->E-T";
        case LR_Parser::ActionType::REDUCE_E_TO_T:         return "Reduce E->T";
        case LR_Parser::ActionType::REDUCE_T_TO_T_MUL_F:   return "Reduce T->T*F";
        case LR_Parser::ActionType::REDUCE_T_TO_T_DIV_F:   return "Reduce T->T/F";
        case LR_Parser::ActionType::REDUCE_T_TO_F:         return "Reduce T->F";
        case LR_Parser::ActionType::REDUCE_F_TO_BRACKET_E: return "Reduce F->(E)";
        case LR_Parser::ActionType::REDUCE_F_TO_ID:        return "Reduce F->id";
        case LR_Parser::ActionType::REDUCE_F_TO_NUM:       return "Reduce F->num";
        case LR_Parser::ActionType::SHIFT:                 return "Shift";
        case LR_Parser::ActionType::ERROR:                 return "Error";
        default:
            throw std::runtime_error("Unknown action");
    }
}

void PrintTableRow(const std::stack<int>& stack, const std::vector<std::string>& symbols,
                const std::string& input, LR_Parser::ActionType action) {
    
    std::cout << std::string(width * 4, '-') << "\n";
    PrintStack(stack);
    PrintSymbols(symbols);
    std::cout << std::setw(width) << std::left << input;
    std::cout << std::setw(width) << std::left << GetStringAction(action) << "\n";
}
}
#endif

namespace SyntaxAnalyzer {
using namespace LexicalAnalyzer;

LR_Parser::TerminalsType GetTerminalType(const Lexer::Lexeme& lexeme) {

    switch (lexeme.type) {
        case Lexer::LexemeType::IDENTIFICATOR:
            return LR_Parser::TerminalsType::ID;

        case Lexer::LexemeType::NUMBER:
            return LR_Parser::TerminalsType::NUMBER;

        case Lexer::LexemeType::OPERATOR: {
            auto token = std::get<Lexer::Tokens>(lexeme.token);
            switch (token) {
                case Lexer::Tokens::ADD:
                    return LR_Parser::TerminalsType::ADD;
                case Lexer::Tokens::SUB:
                    return LR_Parser::TerminalsType::SUB;
                case Lexer::Tokens::MUL:
                    return LR_Parser::TerminalsType::MUL;
                case Lexer::Tokens::DIV:
                    return LR_Parser::TerminalsType::DIV;
                case Lexer::Tokens::LBRACKET:
                    return LR_Parser::TerminalsType::LBRACKET;
                case Lexer::Tokens::RBRACKET:
                    return LR_Parser::TerminalsType::RBRACKET;
                case Lexer::Tokens::END:
                    return LR_Parser::TerminalsType::END;
                default:
                    throw std::runtime_error("Unknown operator token");
            }
        }

        default:
            throw std::runtime_error("Unknown token type");
    }
}

std::pair<LR_Parser::TerminalsType, std::size_t> GetProduction(LR_Parser::ActionType action) {
    switch (action) {
        case LR_Parser::ActionType::REDUCE_E_TO_E_ADD_T:
        case LR_Parser::ActionType::REDUCE_E_TO_E_SUB_T:
            return {LR_Parser::TerminalsType::E, 3};

        case LR_Parser::ActionType::REDUCE_E_TO_T:
            return {LR_Parser::TerminalsType::E, 1};

        case LR_Parser::ActionType::REDUCE_T_TO_T_DIV_F:
        case LR_Parser::ActionType::REDUCE_T_TO_T_MUL_F:
            return {LR_Parser::TerminalsType::T, 3};

        case LR_Parser::ActionType::REDUCE_T_TO_F:
            return {LR_Parser::TerminalsType::T, 1};

        case LR_Parser::ActionType::REDUCE_F_TO_ID:
        case LR_Parser::ActionType::REDUCE_F_TO_NUM:
            return {LR_Parser::TerminalsType::F, 1};

        case LR_Parser::ActionType::REDUCE_F_TO_BRACKET_E:
            return {LR_Parser::TerminalsType::F, 3};

        default:
            throw std::runtime_error("Unknown action");
    }
}

#ifdef PRINT_TABLE
void GetSymbolsAfterReduce(LR_Parser::ActionType action, std::vector<std::string>& symbols) {
    switch (action) {
        case LR_Parser::ActionType::REDUCE_E_TO_E_ADD_T:
            symbols.erase(symbols.end() - 3, symbols.end());
            symbols.push_back("E");
            break;

        case LR_Parser::ActionType::REDUCE_E_TO_E_SUB_T:
            symbols.erase(symbols.end() - 3, symbols.end());
            symbols.push_back("E");
            break;

        case LR_Parser::ActionType::REDUCE_E_TO_T:
            symbols.pop_back();
            symbols.push_back("E");
            break;

        case LR_Parser::ActionType::REDUCE_T_TO_T_MUL_F:
            symbols.erase(symbols.end() - 3, symbols.end());
            symbols.push_back("T");
            break;

        case LR_Parser::ActionType::REDUCE_T_TO_T_DIV_F:
            symbols.erase(symbols.end() - 3, symbols.end());
            symbols.push_back("T");
            break;

        case LR_Parser::ActionType::REDUCE_T_TO_F:
            symbols.pop_back();
            symbols.push_back("T");
            break;

        case LR_Parser::ActionType::REDUCE_F_TO_BRACKET_E:
            symbols.erase(symbols.end() - 3, symbols.end());
            symbols.push_back("F");
            break;

        case LR_Parser::ActionType::REDUCE_F_TO_ID:
            symbols.pop_back();
            symbols.push_back("F");
            break;

        case LR_Parser::ActionType::REDUCE_F_TO_NUM:
            symbols.pop_back();
            symbols.push_back("F");
            break;

        default:
            break;
    }
}
#endif
}

#ifdef PRINT_TABLE
namespace LexicalAnalyzer {
std::string GetSubstringFromTokenIndex(std::size_t token_index, const std::vector<Lexer::Lexeme>& tokens) {

    if (token_index >= tokens.size()) {
        return "$";
    }

    std::string result;
    std::for_each(tokens.begin() + token_index, tokens.end(), [&result](const auto& token) {
        result += GetCurrentSymbol(token);
    });

    return result + "$";
}

std::string GetCurrentSymbol(const Lexer::Lexeme& lexeme) {

    switch (lexeme.type) {
        case Lexer::LexemeType::IDENTIFICATOR:
            return std::get<std::string>(lexeme.token);
            break;
        case Lexer::LexemeType::NUMBER:
            return std::to_string(std::get<int>(lexeme.token));
            break;
        case Lexer::LexemeType::OPERATOR:
            switch (std::get<Lexer::Tokens>(lexeme.token)) {
                case Lexer::Tokens::ADD:      return "+"; break;
                case Lexer::Tokens::SUB:      return "-"; break;
                case Lexer::Tokens::MUL:      return "*"; break;
                case Lexer::Tokens::DIV:      return "/"; break;
                case Lexer::Tokens::LBRACKET: return "("; break;
                case Lexer::Tokens::RBRACKET: return ")"; break;
                default: break;
            }
            break;
    }

    return "";
}
}
#endif