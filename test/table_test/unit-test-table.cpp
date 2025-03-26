#include <cmath>
#include <cstddef>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "utils.hpp"

bool CompareFiles(const std::string& expected_file, const std::string& actual_file) {

    std::ifstream expected_stream(expected_file);
    std::ifstream actual_stream(actual_file);

    std::string expected_line;
    std::string actual_line;
    std::size_t line_number = 1;

    while (std::getline(expected_stream, expected_line) && std::getline(actual_stream, actual_line)) {
        if (expected_line != actual_line) {
            ADD_FAILURE() << "Mismatch at line " << line_number << ":\n"
                          << "Expected: " << expected_line << "\n"
                          << "Actual:   " << actual_line;
            return false;
        }
        line_number++;
    }

    return true;
}

void RunParserAndGenerateTable(const std::string& input_file_name, const std::string& table_file_name) {

    std::ifstream input_file(input_file_name);
    if (!input_file.is_open()) {
        throw std::runtime_error("Can't open file: " + input_file_name);
    }

    LexicalAnalyzer::Lexer lexer(input_file);
    SyntaxAnalyzer::LR_Parser parser;
    std::ofstream table_file(table_file_name);
    std::cout.rdbuf(table_file.rdbuf());
    parser.Parse(lexer);

    input_file.close();
}

TEST(ParserTest, FirstTableTest) {

    std::string expected_file = TEST_FILE_DIR "table_test/expected_tables/exp_table1.txt";
    std::string input_file_name = TEST_FILE_DIR "../examples/num1.txt";
    std::string table_file = TEST_FILE_DIR "table_test/received_tables/get_table1.txt";

    ASSERT_NO_THROW(RunParserAndGenerateTable(input_file_name, table_file));

    EXPECT_TRUE(CompareFiles(expected_file, table_file));
}

TEST(ParserTest, SecondTableTest) {

    std::string expected_file = TEST_FILE_DIR "table_test/expected_tables/exp_table2.txt";
    std::string input_file_name = TEST_FILE_DIR "../examples/num2.txt";
    std::string table_file = TEST_FILE_DIR "received_tables/get_table2.txt";

    ASSERT_NO_THROW(RunParserAndGenerateTable(input_file_name, table_file));

    EXPECT_TRUE(CompareFiles(expected_file, table_file));
}

TEST(ParserTest, ThirdTableTest) {

    std::string expected_file = TEST_FILE_DIR "expected_tables/exp_table3.txt";
    std::string input_file_name = TEST_FILE_DIR "../examples/num3.txt";
    std::string table_file = TEST_FILE_DIR "received_tables/get_table3.txt";

    ASSERT_NO_THROW(RunParserAndGenerateTable(input_file_name, table_file));

    EXPECT_TRUE(CompareFiles(expected_file, table_file));
}

TEST(ParserTest, FourthTableTest) {

    std::string expected_file = TEST_FILE_DIR "expected_tables/exp_table4.txt";
    std::string input_file_name = TEST_FILE_DIR "../examples/num4.txt";
    std::string table_file = TEST_FILE_DIR "received_tables/get_table4.txt";

    ASSERT_NO_THROW(RunParserAndGenerateTable(input_file_name, table_file));

    EXPECT_TRUE(CompareFiles(expected_file, table_file));
}