#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <variant>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

namespace LexicalAnalyzer {
class Lexer final : public yyFlexLexer {

    public:
        enum class LexemeType {
            IDENTIFICATOR,
            NUMBER,
            OPERATOR
        };

        enum class Tokens {
            END,
            ID,
            NUM,
            ADD,
            SUB,
            MUL,
            DIV,
            LBRACKET,
            RBRACKET,
        };

        struct Lexeme final {
            LexemeType type;

            std::variant<std::string, int, Tokens> token;

            Lexeme(LexemeType type, const std::string& str) : type(type), token(str) {}
            Lexeme(LexemeType type, int num) : type(type), token(num) {}
            Lexeme(LexemeType type, Tokens oper) : type(type), token(oper) {}
        };

        explicit Lexer(std::ifstream& input);

        Lexeme GetNextToken(std::size_t current_index) const;

        const std::vector<Lexer::Lexeme>& GetTokens() const;

    private:
        std::vector<Lexeme> tokens;
    };
}