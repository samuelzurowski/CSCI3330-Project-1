//
//  InstructionItem.hpp
//  ComputerOrg1
//
//  Created by Samuel Zurowski on 3/4/19.
//  Copyright © 2019 Samuel Zurowski. All rights reserved.
//
#pragma once

#include "tools.hpp"
class InstructionItem {
public:
    static const map<string, int> m;
    static const map<string, int> r;
    bool isBranch(string opcode);
    string label;
    string op;
    string params;
    string line;
    enum instructionType {I_TYPE, R_TYPE, J_TYPE, INVALID};
    instructionType type = INVALID;
    InstructionItem(string s);
    bool isIType(string s);
};
