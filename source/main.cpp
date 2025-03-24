#include <cstdlib>
#include <iostream>
#include "utils.hpp"
    
int main(int argc, char** argv) {
    try {
        #ifdef DUMP_TREE
            if (argc != 3) {
                std::cerr << "Usage: " << argv[0] << " <input_file_name>" << " <dot_file>";
                return EXIT_FAILURE;
            }
        #else
            if (argc != 2) {
                std::cerr << "Usage: " << argv[0] << " <file_name>\n";
                return EXIT_FAILURE;
            }
        #endif

        std::ifstream input_file(argv[1]);
        if (!input_file.is_open()) {
            std::cerr << "Can't open input file" << argv[1] << "\n";
            return EXIT_FAILURE;
        }

        LexicalAnalyzer::Lexer lexer(input_file);

        SyntaxAnalyzer::LR_Parser parser;

        parser.Parse(lexer);

        #ifdef DUMP_TREE
            ParseTree::TreePrinter printer;
            printer.GenerateDotFile(parser.GetRoot(), argv[2]);
        #endif

        input_file.close();

    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << '\n';
        return EXIT_FAILURE;
    }
        
    return 0;
}