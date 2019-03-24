//
//  ParseFile.hpp
//  ComputerOrg1
//
//  Created by Samuel Zurowski on 3/1/19.
//  Copyright © 2019 Samuel Zurowski. All rights reserved.
//
#pragma once

#include "InstructionItem.hpp"
class ParseFile{
private:
    vector<InstructionItem> lines;
    map<int, string> labels;
    string binCode(int num, int length);
    int findLocation(string value);
    unsigned long encodeLine(int lineNum);
    string output;
public:
    ~ParseFile() = default;
    ParseFile(string, string);
    void encodeToFile();
};
