#include <string>
#include "lexer.hpp"

namespace LexicalAnalyzer {
Lexer::Lexer(std::ifstream& input_file) {

    switch_streams(&input_file, nullptr);

    Tokens type;
    while ((type = static_cast<Tokens>(yylex())) != Tokens::END) {
        if (type == Tokens::ID) {
            tokens.emplace_back(LexemeType::IDENTIFICATOR, std::string(yytext));
        } else if (type == Tokens::NUM) {
            tokens.emplace_back(LexemeType::NUMBER, std::stoi(yytext));
        } else {
            tokens.emplace_back(LexemeType::OPERATOR, type);
        }
    }
}

Lexer::Lexeme Lexer::GetNextToken(std::size_t current_index) const {

    if (current_index < tokens.size()) {
        return tokens[current_index];
    }
    return Lexeme(LexemeType::OPERATOR, Tokens::END);
}

const std::vector<Lexer::Lexeme>& Lexer::GetTokens() const {

    return tokens;
}
}