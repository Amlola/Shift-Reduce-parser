set(PARSER_LIB_SOURCES lexer_helper.cpp
                       parser.cpp
                       utils.cpp)

set(PARSER_SOURCES main.cpp)

list(TRANSFORM PARSER_LIB_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
list(TRANSFORM PARSER_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

target_sources(parser-lib PRIVATE ${PARSER_LIB_SOURCES} ${FLEX_Lexer_OUTPUTS})
target_sources(parser PRIVATE ${PARSER_SOURCES})
 