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

namespace SyntaxAnalyzer {
class LR_Parser final {

    public:     
        LR_Parser() = default;
        explicit LR_Parser(const std::string& file_name);
        void Parse(LexicalAnalyzer::Lexer& lexer);

        enum class TerminalsType {
            END,
            NUMBER,
            ID,
            ADD,
            SUB,
            MUL,
            DIV,
            LBRACKET,
            RBRACKET,
            E, 
            T, 
            F,
        };

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

        struct Production final {
            TerminalsType header;
            std::size_t len;

            Production(TerminalsType header, std::size_t len) : header(header), len(len) {}
        };
    
    private:
        
        struct Condition final {
            int cur_cond;
            TerminalsType token;

            bool operator==(const Condition& other) const {
                return cur_cond == other.cur_cond && token == other.token;
            }
        };

        struct Action final {
            int next_cond;
            ActionType action;
        };

        struct ConditionHash final {
            std::size_t operator()(const Condition& cond) const {
                std::size_t hash1 = std::hash<std::size_t>{}(cond.cur_cond);
                std::size_t hash2 = std::hash<std::underlying_type_t<TerminalsType>>{}(static_cast<std::underlying_type_t<TerminalsType>>(cond.token));
                return hash1 ^ (hash2 << 1);
            }
        };

        std::vector<std::string> symbols;
        std::stack<int> stack;

        std::size_t current_token_index = 0;
        std::ofstream table_file;

        void Shift(const Action& action, const LexicalAnalyzer::Lexer& lexer);
        void Reduce(const Action& action, const LexicalAnalyzer::Lexer& lexer);

        const std::unordered_map<Condition, Action, ConditionHash> action_table = {

            {{.cur_cond = 0, .token  = TerminalsType::LBRACKET}, {.next_cond = 4,  .action = ActionType::SHIFT}},
            {{.cur_cond = 0, .token  = TerminalsType::NUMBER},   {.next_cond = 12, .action = ActionType::SHIFT}},
            {{.cur_cond = 0, .token  = TerminalsType::ID},       {.next_cond = 5,  .action = ActionType::SHIFT}},

            {{.cur_cond = 1, .token  = TerminalsType::ADD},      {.next_cond = 6,  .action = ActionType::SHIFT}},
            {{.cur_cond = 1, .token  = TerminalsType::SUB},      {.next_cond = 13, .action = ActionType::SHIFT}},
            {{.cur_cond = 1, .token  = TerminalsType::END},      {.next_cond = 0,  .action = ActionType::ACCEPT}},

            {{.cur_cond = 2, .token  = TerminalsType::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_T}},
            {{.cur_cond = 2, .token  = TerminalsType::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_T}},
            {{.cur_cond = 2, .token  = TerminalsType::MUL},      {.next_cond = 7,  .action = ActionType::SHIFT}},
            {{.cur_cond = 2, .token  = TerminalsType::DIV},      {.next_cond = 15, .action = ActionType::SHIFT}},
            {{.cur_cond = 2, .token  = TerminalsType::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_E_TO_T}},
            {{.cur_cond = 2, .token  = TerminalsType::END},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_T}},

            {{.cur_cond = 3, .token  = TerminalsType::ADD},      {.next_cond = 2,  .action = ActionType::REDUCE_T_TO_F}},
            {{.cur_cond = 3, .token  = TerminalsType::SUB},      {.next_cond = 2,  .action = ActionType::REDUCE_T_TO_F}},
            {{.cur_cond = 3, .token  = TerminalsType::MUL},      {.next_cond = 2,  .action = ActionType::REDUCE_T_TO_F}},
            {{.cur_cond = 3, .token  = TerminalsType::DIV},      {.next_cond = 2,  .action = ActionType::REDUCE_T_TO_F}},
            {{.cur_cond = 3, .token  = TerminalsType::RBRACKET}, {.next_cond = 2,  .action = ActionType::REDUCE_T_TO_F}},
            {{.cur_cond = 3, .token  = TerminalsType::END},      {.next_cond = 2,  .action = ActionType::REDUCE_T_TO_F}},

            {{.cur_cond = 4, .token  = TerminalsType::LBRACKET}, {.next_cond = 4,  .action = ActionType::SHIFT}},
            {{.cur_cond = 4, .token  = TerminalsType::NUMBER},   {.next_cond = 12, .action = ActionType::SHIFT}},
            {{.cur_cond = 4, .token  = TerminalsType::ID},       {.next_cond = 5,  .action = ActionType::SHIFT}},

            {{.cur_cond = 5, .token  = TerminalsType::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_ID}},
            {{.cur_cond = 5, .token  = TerminalsType::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_ID}},
            {{.cur_cond = 5, .token  = TerminalsType::MUL},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_ID}},
            {{.cur_cond = 5, .token  = TerminalsType::DIV},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_ID}},
            {{.cur_cond = 5, .token  = TerminalsType::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_F_TO_ID}},
            {{.cur_cond = 5, .token  = TerminalsType::END},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_ID}},
            
            {{.cur_cond = 6, .token  = TerminalsType::LBRACKET}, {.next_cond = 4,  .action = ActionType::SHIFT}},
            {{.cur_cond = 6, .token  = TerminalsType::NUMBER},   {.next_cond = 12, .action = ActionType::SHIFT}},
            {{.cur_cond = 6, .token  = TerminalsType::ID},       {.next_cond = 5,  .action = ActionType::SHIFT}},

            {{.cur_cond = 7, .token  = TerminalsType::LBRACKET}, {.next_cond = 4,  .action = ActionType::SHIFT}},
            {{.cur_cond = 7, .token  = TerminalsType::NUMBER},   {.next_cond = 12, .action = ActionType::SHIFT}},
            {{.cur_cond = 7, .token  = TerminalsType::ID},       {.next_cond = 5,  .action = ActionType::SHIFT}},

            {{.cur_cond = 8, .token  = TerminalsType::ADD},      {.next_cond = 6,  .action = ActionType::SHIFT}},
            {{.cur_cond = 8, .token  = TerminalsType::SUB},      {.next_cond = 13, .action = ActionType::SHIFT}},
            {{.cur_cond = 8, .token  = TerminalsType::RBRACKET}, {.next_cond = 11, .action = ActionType::SHIFT}},

            {{.cur_cond = 9, .token  = TerminalsType::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_ADD_T}},
            {{.cur_cond = 9, .token  = TerminalsType::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_ADD_T}},
            {{.cur_cond = 9, .token  = TerminalsType::MUL},      {.next_cond = 7,  .action = ActionType::SHIFT}},
            {{.cur_cond = 9, .token  = TerminalsType::DIV},      {.next_cond = 15, .action = ActionType::SHIFT}},
            {{.cur_cond = 9, .token  = TerminalsType::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_ADD_T}},
            {{.cur_cond = 9, .token  = TerminalsType::END},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_ADD_T}},

            {{.cur_cond = 10, .token = TerminalsType::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_MUL_F}},
            {{.cur_cond = 10, .token = TerminalsType::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_MUL_F}},
            {{.cur_cond = 10, .token = TerminalsType::MUL},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_MUL_F}},
            {{.cur_cond = 10, .token = TerminalsType::DIV},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_MUL_F}},
            {{.cur_cond = 10, .token = TerminalsType::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_MUL_F}},
            {{.cur_cond = 10, .token = TerminalsType::END},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_MUL_F}},

            {{.cur_cond = 11, .token = TerminalsType::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_BRACKET_E}},
            {{.cur_cond = 11, .token = TerminalsType::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_BRACKET_E}},
            {{.cur_cond = 11, .token = TerminalsType::MUL},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_BRACKET_E}},
            {{.cur_cond = 11, .token = TerminalsType::DIV},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_BRACKET_E}},
            {{.cur_cond = 11, .token = TerminalsType::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_F_TO_BRACKET_E}},
            {{.cur_cond = 11, .token = TerminalsType::END},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_BRACKET_E}},

            {{.cur_cond = 12, .token = TerminalsType::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_NUM}},
            {{.cur_cond = 12, .token = TerminalsType::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_NUM}},
            {{.cur_cond = 12, .token = TerminalsType::MUL},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_NUM}},
            {{.cur_cond = 12, .token = TerminalsType::DIV},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_NUM}},
            {{.cur_cond = 12, .token = TerminalsType::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_F_TO_NUM}},
            {{.cur_cond = 12, .token = TerminalsType::END},      {.next_cond = -1, .action = ActionType::REDUCE_F_TO_NUM}},

            {{.cur_cond = 13, .token = TerminalsType::LBRACKET}, {.next_cond = 4,  .action = ActionType::SHIFT}},
            {{.cur_cond = 13, .token = TerminalsType::NUMBER},   {.next_cond = 12, .action = ActionType::SHIFT}},
            {{.cur_cond = 13, .token = TerminalsType::ID},       {.next_cond = 5,  .action = ActionType::SHIFT}},

            {{.cur_cond = 14, .token = TerminalsType::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_SUB_T}},
            {{.cur_cond = 14, .token = TerminalsType::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_SUB_T}},
            {{.cur_cond = 14, .token = TerminalsType::MUL},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_SUB_T}},
            {{.cur_cond = 14, .token = TerminalsType::DIV},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_SUB_T}},
            {{.cur_cond = 14, .token = TerminalsType::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_SUB_T}},
            {{.cur_cond = 14, .token = TerminalsType::END},      {.next_cond = -1, .action = ActionType::REDUCE_E_TO_E_SUB_T}},

            {{.cur_cond = 15, .token = TerminalsType::LBRACKET}, {.next_cond = 4,  .action = ActionType::SHIFT}},
            {{.cur_cond = 15, .token = TerminalsType::NUMBER},   {.next_cond = 12, .action = ActionType::SHIFT}},
            {{.cur_cond = 15, .token = TerminalsType::ID},       {.next_cond = 5,  .action = ActionType::SHIFT}},

            {{.cur_cond = 16, .token = TerminalsType::ADD},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_DIV_F}},
            {{.cur_cond = 16, .token = TerminalsType::SUB},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_DIV_F}},
            {{.cur_cond = 16, .token = TerminalsType::MUL},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_DIV_F}},
            {{.cur_cond = 16, .token = TerminalsType::DIV},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_DIV_F}},
            {{.cur_cond = 16, .token = TerminalsType::RBRACKET}, {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_DIV_F}},
            {{.cur_cond = 16, .token = TerminalsType::END},      {.next_cond = -1, .action = ActionType::REDUCE_T_TO_T_DIV_F}},
        };

        const std::unordered_map<Condition, int, ConditionHash> goto_table = {

            {{.cur_cond = 0, .token  = TerminalsType::E},  1},
            {{.cur_cond = 0, .token  = TerminalsType::T},  2},
            {{.cur_cond = 0, .token  = TerminalsType::F},  3},

            {{.cur_cond = 4, .token  = TerminalsType::E},  8},
            {{.cur_cond = 4, .token  = TerminalsType::T},  2},
            {{.cur_cond = 4, .token  = TerminalsType::F},  3},

            {{.cur_cond = 6, .token  = TerminalsType::T},  9},
            {{.cur_cond = 6, .token  = TerminalsType::F},  3},

            {{.cur_cond = 7, .token  = TerminalsType::F}, 10},

            {{.cur_cond = 13, .token = TerminalsType::T}, 14},
            {{.cur_cond = 13, .token = TerminalsType::F},  3},

            {{.cur_cond = 15, .token = TerminalsType::F}, 16},
        };
    };
}
    