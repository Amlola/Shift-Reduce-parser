#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "utils.hpp"

std::string RemoveSpaces(const std::string& str) {
    std::string result;
    std::copy_if(str.begin(), str.end(), std::back_inserter(result),
                 [](char sym) { return !static_cast<bool>(std::isspace(sym)); });
    return result;
}

bool CompareFiles(const std::string& expected_file, const std::string& actual_file) {

    std::ifstream expected_stream(expected_file);
    std::ifstream actual_stream(actual_file);

    if (!expected_stream.is_open()) {
        ADD_FAILURE() << "Failed to open expected file: " << expected_file;
        return false;
    }
    if (!actual_stream.is_open()) {
        ADD_FAILURE() << "Failed to open actual file: " << actual_file;
        return false;
    }

    std::string expected_line;
    std::string actual_line;

    if (!std::getline(expected_stream, expected_line)) {
        ADD_FAILURE() << "Expected file is empty: " << expected_file;
        return false;
    }
    if (!std::getline(actual_stream, actual_line)) {
        ADD_FAILURE() << "Actual file is empty: " << actual_file;
        return false;
    }

    std::string expected_no_spaces = RemoveSpaces(expected_line);
    std::string actual_no_spaces = RemoveSpaces(actual_line);

    if (expected_no_spaces != actual_no_spaces) {
        ADD_FAILURE() << "Mismatch:\n"
                      << "Expected: " << expected_line << "\n"
                      << "Actual:   " << actual_line;
        return false;
    }

    return true;
}

void RunParserAndGenerateTree(const std::string& input_file_name, const std::string& tree_out_file_name) {

    std::ifstream input_file(input_file_name);
    if (!input_file.is_open()) {
        throw std::runtime_error("Can't open file: " + input_file_name);
    }

    LexicalAnalyzer::Lexer lexer(input_file);
    SyntaxAnalyzer::LR_Parser parser;
    parser.Parse(lexer);

    ParseTree::TreePrinter printer;
    printer.PrintTree(parser.GetRoot(), tree_out_file_name);    

    input_file.close();
}

TEST(ParserTest, FirstTableTest) {

    std::string expected_file = "expected_tree/exp_tree1.txt";
    std::string input_file_name = "../examples/num1.txt";
    std::string tree_out_file = "received_tree/get_tree1.txt";

    ASSERT_NO_THROW(RunParserAndGenerateTree(input_file_name, tree_out_file));

    EXPECT_TRUE(CompareFiles(expected_file, tree_out_file));
}

TEST(ParserTest, SecondTableTest) {

    std::string expected_file = "expected_tree/exp_tree2.txt";
    std::string input_file_name = "../examples/num2.txt";
    std::string tree_out_file = "received_tree/get_tree2.txt";

    ASSERT_NO_THROW(RunParserAndGenerateTree(input_file_name, tree_out_file));

    EXPECT_TRUE(CompareFiles(expected_file, tree_out_file));
}

TEST(ParserTest, ThirdTableTest) {

    std::string expected_file = "expected_tree/exp_tree3.txt";
    std::string input_file_name = "../examples/num3.txt";
    std::string tree_out_file = "received_tree/get_tree3.txt";

    ASSERT_NO_THROW(RunParserAndGenerateTree(input_file_name, tree_out_file));

    EXPECT_TRUE(CompareFiles(expected_file, tree_out_file));
}

TEST(ParserTest, FourthTableTest) {

    std::string expected_file = "expected_tree/exp_tree3.txt";
    std::string input_file_name = "../examples/num3.txt";
    std::string tree_out_file = "received_tree/get_tree3.txt";

    ASSERT_NO_THROW(RunParserAndGenerateTree(input_file_name, tree_out_file));

    EXPECT_TRUE(CompareFiles(expected_file, tree_out_file));
}

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}