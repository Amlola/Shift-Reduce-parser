#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <variant>
#include "lexer.hpp"
#include "parse_tree.hpp"

namespace SyntaxAnalyzer {
class LR_Parser final {

    public:     
        LR_Parser() = default;
        void Parse(LexicalAnalyzer::Lexer& lexer);

        const ParseTree::Node& GetRoot() const;

        enum class ActionType {
            ACCEPT,
            REDUCE_E_TO_E_ADD_T,
            REDUCE_E_TO_E_SUB_T,
            REDUCE_E_TO_T,
            REDUCE_T_TO_T_MUL_F,
            REDUCE_T_TO_T_DIV_F,
            REDUCE_T_TO_F,
            REDUCE_F_TO_BRACKET_E,
            REDUCE_F_TO_ID,
            REDUCE_F_TO_NUM,
            SHIFT,
            ERROR,
        };
    
    private:
        
        struct Condition final {
            int cur_cond;
            LexicalAnalyzer::Lexer::Tokens token;

            bool operator==(const Condition& other) const {
                return cur_cond == other.cur_cond && token == other.token;
            }
        };

        struct ConditionHash final {
            std::size_t operator()(const Condition& cond) const {
                std::size_t hash1 = std::hash<int>{}(cond.cur_cond);
                std::size_t hash2 = std::hash<std::underlying_type_t<LexicalAnalyzer::Lexer::Tokens>>{}(static_cast<std::underlying_type_t<LexicalAnalyzer::Lexer::Tokens>>(cond.token));
                return hash1 ^ (hash2 << 1);
            }
        };

        struct Action final {
            int next_cond;
            ActionType action;
        };

        struct GotoCondition final {
            int cur_cond;
            ParseTree::Node::NonTerminalsType term;

            bool operator==(const GotoCondition& other) const {
                return cur_cond == other.cur_cond && term == other.term;
            }
        };

        struct GotoConditionHash final {
            std::size_t operator()(const GotoCondition& cond) const {
                std::size_t hash1 = std::hash<int>{}(cond.cur_cond);
                std::size_t hash2 = std::hash<std::underlying_type_t<ParseTree::Node::NonTerminalsType>>{}(static_cast<std::underlying_type_t<ParseTree::Node::NonTerminalsType>>(cond.term));
                return hash1 ^ (hash2 << 1);
            }
        };

        std::vector<std::string> symbols;
        std::stack<int> stack;

        std::stack<std::unique_ptr<ParseTree::Node>> parse_tree_stack;

        std::size_t current_token_index = 0;

        void Shift(const Action& action, const LexicalAnalyzer::Lexer& lexer);
        void Reduce(const Action& action, const LexicalAnalyzer::Lexer& lexer);

        const std::unordered_map<Condition, Action, ConditionHash> action_table = {

            {{.cur_cond = 0, .token  = LexicalAnalyzer::Lexer::Tokens::LBRACKET}, {.next_cond =  4, .action = ActionType::SHIFT}},
            {{.cur_cond = 0, .token  = LexicalAnalyzer::Lexer::Tokens::NUM},      {.next_cond = 12, .action = ActionType::SHIFT}},
            {{.cur_cond = 0, .token  = LexicalAnalyzer::Lexer::Tokens::ID},       {.next_cond =  5, .action = ActionType::SHIFT}},

            {{.cur_cond = 1, .token  = LexicalAnalyzer::Lexer::Tokens::ADD},      {.next_cond =  6, .action = ActionType::SHIFT}},
            {{.cur_cond = 1, .token  = LexicalAnalyzer::Lexer::Tokens::SUB},      {.next_cond = 13, .action = ActionType::SHIFT}},
            {{.cur_cond = 1, .token  = LexicalAnalyzer::Lexer::Tokens::END},      {.next_cond =  0, .action = ActionType::ACCEPT}},

            {{.cur_cond = 2, .token  = LexicalAnalyzer::Lexer::Tokens::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_T}},
            {{.cur_cond = 2, .token  = LexicalAnalyzer::Lexer::Tokens::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_T}},
            {{.cur_cond = 2, .token  = LexicalAnalyzer::Lexer::Tokens::MUL},      {.next_cond =  7, .action = ActionType::SHIFT}},
            {{.cur_cond = 2, .token  = LexicalAnalyzer::Lexer::Tokens::DIV},      {.next_cond = 15, .action = ActionType::SHIFT}},
            {{.cur_cond = 2, .token  = LexicalAnalyzer::Lexer::Tokens::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_E_TO_T}},
            {{.cur_cond = 2, .token  = LexicalAnalyzer::Lexer::Tokens::END},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_T}},

            {{.cur_cond = 3, .token  = LexicalAnalyzer::Lexer::Tokens::ADD},      {.next_cond =  2, .action = ActionType::REDUCE_T_TO_F}},
            {{.cur_cond = 3, .token  = LexicalAnalyzer::Lexer::Tokens::SUB},      {.next_cond =  2, .action = ActionType::REDUCE_T_TO_F}},
            {{.cur_cond = 3, .token  = LexicalAnalyzer::Lexer::Tokens::MUL},      {.next_cond =  2, .action = ActionType::REDUCE_T_TO_F}},
            {{.cur_cond = 3, .token  = LexicalAnalyzer::Lexer::Tokens::DIV},      {.next_cond =  2, .action = ActionType::REDUCE_T_TO_F}},
            {{.cur_cond = 3, .token  = LexicalAnalyzer::Lexer::Tokens::RBRACKET}, {.next_cond =  2, .action = ActionType::REDUCE_T_TO_F}},
            {{.cur_cond = 3, .token  = LexicalAnalyzer::Lexer::Tokens::END},      {.next_cond =  2, .action = ActionType::REDUCE_T_TO_F}},

            {{.cur_cond = 4, .token  = LexicalAnalyzer::Lexer::Tokens::LBRACKET}, {.next_cond =  4, .action = ActionType::SHIFT}},
            {{.cur_cond = 4, .token  = LexicalAnalyzer::Lexer::Tokens::NUM},      {.next_cond = 12, .action = ActionType::SHIFT}},
            {{.cur_cond = 4, .token  = LexicalAnalyzer::Lexer::Tokens::ID},       {.next_cond =  5, .action = ActionType::SHIFT}},

            {{.cur_cond = 5, .token  = LexicalAnalyzer::Lexer::Tokens::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_ID}},
            {{.cur_cond = 5, .token  = LexicalAnalyzer::Lexer::Tokens::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_ID}},
            {{.cur_cond = 5, .token  = LexicalAnalyzer::Lexer::Tokens::MUL},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_ID}},
            {{.cur_cond = 5, .token  = LexicalAnalyzer::Lexer::Tokens::DIV},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_ID}},
            {{.cur_cond = 5, .token  = LexicalAnalyzer::Lexer::Tokens::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_F_TO_ID}},
            {{.cur_cond = 5, .token  = LexicalAnalyzer::Lexer::Tokens::END},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_ID}},
            
            {{.cur_cond = 6, .token  = LexicalAnalyzer::Lexer::Tokens::LBRACKET}, {.next_cond =  4, .action = ActionType::SHIFT}},
            {{.cur_cond = 6, .token  = LexicalAnalyzer::Lexer::Tokens::NUM},      {.next_cond = 12, .action = ActionType::SHIFT}},
            {{.cur_cond = 6, .token  = LexicalAnalyzer::Lexer::Tokens::ID},       {.next_cond =  5, .action = ActionType::SHIFT}},

            {{.cur_cond = 7, .token  = LexicalAnalyzer::Lexer::Tokens::LBRACKET}, {.next_cond =  4, .action = ActionType::SHIFT}},
            {{.cur_cond = 7, .token  = LexicalAnalyzer::Lexer::Tokens::NUM},      {.next_cond = 12, .action = ActionType::SHIFT}},
            {{.cur_cond = 7, .token  = LexicalAnalyzer::Lexer::Tokens::ID},       {.next_cond =  5, .action = ActionType::SHIFT}},

            {{.cur_cond = 8, .token  = LexicalAnalyzer::Lexer::Tokens::ADD},      {.next_cond =  6, .action = ActionType::SHIFT}},
            {{.cur_cond = 8, .token  = LexicalAnalyzer::Lexer::Tokens::SUB},      {.next_cond = 13, .action = ActionType::SHIFT}},
            {{.cur_cond = 8, .token  = LexicalAnalyzer::Lexer::Tokens::RBRACKET}, {.next_cond = 11, .action = ActionType::SHIFT}},

            {{.cur_cond = 9, .token  = LexicalAnalyzer::Lexer::Tokens::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_ADD_T}},
            {{.cur_cond = 9, .token  = LexicalAnalyzer::Lexer::Tokens::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_ADD_T}},
            {{.cur_cond = 9, .token  = LexicalAnalyzer::Lexer::Tokens::MUL},      {.next_cond =  7, .action = ActionType::SHIFT}},
            {{.cur_cond = 9, .token  = LexicalAnalyzer::Lexer::Tokens::DIV},      {.next_cond = 15, .action = ActionType::SHIFT}},
            {{.cur_cond = 9, .token  = LexicalAnalyzer::Lexer::Tokens::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_ADD_T}},
            {{.cur_cond = 9, .token  = LexicalAnalyzer::Lexer::Tokens::END},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_ADD_T}},

            {{.cur_cond = 10, .token = LexicalAnalyzer::Lexer::Tokens::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_MUL_F}},
            {{.cur_cond = 10, .token = LexicalAnalyzer::Lexer::Tokens::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_MUL_F}},
            {{.cur_cond = 10, .token = LexicalAnalyzer::Lexer::Tokens::MUL},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_MUL_F}},
            {{.cur_cond = 10, .token = LexicalAnalyzer::Lexer::Tokens::DIV},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_MUL_F}},
            {{.cur_cond = 10, .token = LexicalAnalyzer::Lexer::Tokens::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_MUL_F}},
            {{.cur_cond = 10, .token = LexicalAnalyzer::Lexer::Tokens::END},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_MUL_F}},

            {{.cur_cond = 11, .token = LexicalAnalyzer::Lexer::Tokens::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_BRACKET_E}},
            {{.cur_cond = 11, .token = LexicalAnalyzer::Lexer::Tokens::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_BRACKET_E}},
            {{.cur_cond = 11, .token = LexicalAnalyzer::Lexer::Tokens::MUL},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_BRACKET_E}},
            {{.cur_cond = 11, .token = LexicalAnalyzer::Lexer::Tokens::DIV},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_BRACKET_E}},
            {{.cur_cond = 11, .token = LexicalAnalyzer::Lexer::Tokens::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_F_TO_BRACKET_E}},
            {{.cur_cond = 11, .token = LexicalAnalyzer::Lexer::Tokens::END},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_BRACKET_E}},

            {{.cur_cond = 12, .token = LexicalAnalyzer::Lexer::Tokens::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_NUM}},
            {{.cur_cond = 12, .token = LexicalAnalyzer::Lexer::Tokens::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_NUM}},
            {{.cur_cond = 12, .token = LexicalAnalyzer::Lexer::Tokens::MUL},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_NUM}},
            {{.cur_cond = 12, .token = LexicalAnalyzer::Lexer::Tokens::DIV},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_NUM}},
            {{.cur_cond = 12, .token = LexicalAnalyzer::Lexer::Tokens::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_F_TO_NUM}},
            {{.cur_cond = 12, .token = LexicalAnalyzer::Lexer::Tokens::END},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_NUM}},

            {{.cur_cond = 13, .token = LexicalAnalyzer::Lexer::Tokens::LBRACKET}, {.next_cond =  4, .action = ActionType::SHIFT}},
            {{.cur_cond = 13, .token = LexicalAnalyzer::Lexer::Tokens::NUM},      {.next_cond = 12, .action = ActionType::SHIFT}},
            {{.cur_cond = 13, .token = LexicalAnalyzer::Lexer::Tokens::ID},       {.next_cond =  5, .action = ActionType::SHIFT}},

            {{.cur_cond = 14, .token = LexicalAnalyzer::Lexer::Tokens::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_SUB_T}},
            {{.cur_cond = 14, .token = LexicalAnalyzer::Lexer::Tokens::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_SUB_T}},
            {{.cur_cond = 14, .token = LexicalAnalyzer::Lexer::Tokens::MUL},      {.next_cond =  7, .action = ActionType::SHIFT}},
            {{.cur_cond = 14, .token = LexicalAnalyzer::Lexer::Tokens::DIV},      {.next_cond = 15, .action = ActionType::SHIFT}},
            {{.cur_cond = 14, .token = LexicalAnalyzer::Lexer::Tokens::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_SUB_T}},
            {{.cur_cond = 14, .token = LexicalAnalyzer::Lexer::Tokens::END},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_SUB_T}},

            {{.cur_cond = 15, .token = LexicalAnalyzer::Lexer::Tokens::LBRACKET}, {.next_cond =  4, .action = ActionType::SHIFT}},
            {{.cur_cond = 15, .token = LexicalAnalyzer::Lexer::Tokens::NUM},      {.next_cond = 12, .action = ActionType::SHIFT}},
            {{.cur_cond = 15, .token = LexicalAnalyzer::Lexer::Tokens::ID},       {.next_cond =  5, .action = ActionType::SHIFT}},

            {{.cur_cond = 16, .token = LexicalAnalyzer::Lexer::Tokens::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_DIV_F}},
            {{.cur_cond = 16, .token = LexicalAnalyzer::Lexer::Tokens::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_DIV_F}},
            {{.cur_cond = 16, .token = LexicalAnalyzer::Lexer::Tokens::MUL},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_DIV_F}},
            {{.cur_cond = 16, .token = LexicalAnalyzer::Lexer::Tokens::DIV},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_DIV_F}},
            {{.cur_cond = 16, .token = LexicalAnalyzer::Lexer::Tokens::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_DIV_F}},
            {{.cur_cond = 16, .token = LexicalAnalyzer::Lexer::Tokens::END},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_DIV_F}},
        };

        const std::unordered_map<GotoCondition, int, GotoConditionHash> goto_table = {

            {{.cur_cond = 0,  .term = ParseTree::Node::NonTerminalsType::E},  1},
            {{.cur_cond = 0,  .term = ParseTree::Node::NonTerminalsType::T},  2},
            {{.cur_cond = 0,  .term = ParseTree::Node::NonTerminalsType::F},  3},

            {{.cur_cond = 4,  .term = ParseTree::Node::NonTerminalsType::E},  8},
            {{.cur_cond = 4,  .term = ParseTree::Node::NonTerminalsType::T},  2},
            {{.cur_cond = 4,  .term = ParseTree::Node::NonTerminalsType::F},  3},

            {{.cur_cond = 6,  .term = ParseTree::Node::NonTerminalsType::T},  9},
            {{.cur_cond = 6,  .term = ParseTree::Node::NonTerminalsType::F},  3},

            {{.cur_cond = 7,  .term = ParseTree::Node::NonTerminalsType::F}, 10},

            {{.cur_cond = 13, .term = ParseTree::Node::NonTerminalsType::T}, 14},
            {{.cur_cond = 13, .term = ParseTree::Node::NonTerminalsType::F},  3},

            {{.cur_cond = 15, .term = ParseTree::Node::NonTerminalsType::F}, 16},
        };
    };
}
    