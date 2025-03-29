#include <cstdlib>
#include <iostream>
#include "utils.hpp"
    
int main() {
    
    try {
        std::string input;
        std::getline(std::cin, input);
        LexicalAnalyzer::Lexer lexer(input);

        SyntaxAnalyzer::LR_Parser parser;

        parser.Parse(lexer);

    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << '\n';
        return EXIT_FAILURE;
    }
        
    return 0;
}