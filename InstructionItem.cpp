//
//  InstructionItem.cpp
//  ComputerOrg1
//
//  Created by Samuel Zurowski on 3/4/19.
//  Copyright © 2019 Samuel Zurowski. All rights reserved.
//

#include "InstructionItem.hpp"
// NOTE: excluding branch & jump
bool InstructionItem::isIType(string line) {
    if(line.find(",") != string::npos && ( // check if i type
       line.at(line.find_first_of(',') - 1) == ')' ||
       line.at(line.size() - 1) == ')'             ||
       count(line.begin(), line.end(), ',') == 2
                                          )) {
        return true; // if isIType
    } return false; // not
}
InstructionItem::InstructionItem(string s) {
    line = s;
    regex rgx("^(([a-zA-Z]+):)?([a-zA-Z]+)\\s(.+)");
    /* ^(([a-zA-Z]+):)? start string group by group by any letter one or more
     *                  close, matching : close group 0 or 1.
     * ([a-zA-Z]+)      group any letter one or more close group
     * \\s(.+)          whitespace character match anything one or more.
     */
    smatch matches;
    if(regex_search(s, matches, rgx)) {
        if(matches[2].length() > 0) label = matches[2].str();
        
        if(matches[3].length() > 0 && matches[4].length() > 0) {
            op = matches[3].str(); // op code
            params = matches[4].str(); // the operands
            
            if(r.find(op) != r.end()) // r type
                type = R_TYPE;
            else if(m.at(op) == 2 || m.at(op) == 3) // J type
                type = J_TYPE;
            else if(isIType(line) || isBranch(op)) // i type
                type = I_TYPE;
        }
    }
}

bool InstructionItem::isBranch(string opcode) {
    if(m.at(op) == 4 || m.at(op) == 5) return true;
    return false;
}

const map<string, int> InstructionItem::m = { // map of op codes
    {"j", 2},      {"jal", 3},    {"beqz", 4},   {"bnez", 5},
    {"addi", 8},   {"addui", 9},  {"subi", 10},  {"subui", 11},
    {"andi", 12},  {"ori", 13},   {"xori", 14},  {"lhi", 15},
    {"jr", 18},    {"jalr", 19},  {"seqi", 24},  {"snei", 25},
    {"slti", 26},  {"sgti", 27},  {"slei", 28},  {"sgei", 29},
    {"lb", 32},    {"lh", 33},    {"lw", 35},    {"lbu", 36},
    {"lhu", 37},   {"sb", 40},    {"sh", 41},    {"sw", 43},
    {"sequi", 48}, {"sneui", 49}, {"sltui", 50}, {"sgtui", 52},
    {"sgeui", 53}, {"slli", 54},  {"srli", 55},  {"srai", 56}
};


const map<string, int> InstructionItem::r = { // r types
    {"sll",   4}, {"srl",   6}, {"sra", 7},   {"sltu", 18},
    {"sgtu", 19}, {"sleu", 20}, {"sgeu", 21}, {"add", 32},
    {"addu", 33}, {"sub",  34}, {"subu", 35}, {"and", 36},
    {"or",   37}, {"xor",  38}, {"seq", 40},  {"sne", 41},
    {"slt",  42}, {"seq",  40}, {"sne", 41},  {"slt", 42},
    {"sgt",  43}, {"sle",  44}, {"sge", 45}
};

