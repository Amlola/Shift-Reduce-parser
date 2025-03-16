%option c++ yylineno noyywrap

%{
    #include "lexer.hpp"
%}

NUMBER   [1-9][0-9]*|0
VAR      [a-zA-Z_][a-zA-Z0-9_]*
WS       [ \t\r\n]+

%%

{NUMBER}   { return static_cast<int>(LexicalAnalyzer::Lexer::Tokens::NUM); }
{VAR}      { return static_cast<int>(LexicalAnalyzer::Lexer::Tokens::ID);  }

"+"        { return static_cast<int>(LexicalAnalyzer::Lexer::Tokens::ADD); }
"-"        { return static_cast<int>(LexicalAnalyzer::Lexer::Tokens::SUB); }

"*"        { return static_cast<int>(LexicalAnalyzer::Lexer::Tokens::MUL); }
"/"        { return static_cast<int>(LexicalAnalyzer::Lexer::Tokens::DIV); }

"("        { return static_cast<int>(LexicalAnalyzer::Lexer::Tokens::LBRACKET); }
")"        { return static_cast<int>(LexicalAnalyzer::Lexer::Tokens::RBRACKET); }

{WS}       { }
.          { printf("Unknown symbol: %s\n", yytext); }

%%