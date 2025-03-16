#include <cstdlib>
#include <iostream>
#include "utils.hpp"
    
int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <file_name>\n";
            return EXIT_FAILURE;
        }

        std::string file_name = argv[1];

        std::ifstream input_file(file_name);
        if (!input_file.is_open()) {
            std::cerr << "Can't open input file" << file_name << "\n";
            return EXIT_FAILURE;
        }

        LexicalAnalyzer::Lexer lexer(input_file);

        std::string table_file("examples/table1.txt");
        SyntaxAnalyzer::LR_Parser parser(table_file);

        parser.Parse(lexer);

        input_file.close();

    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << '\n';
        return EXIT_FAILURE;
    }
        
    return 0;
}