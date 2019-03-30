//
//  ParseFile.cpp
//  ComputerOrg1
//
//  Created by Samuel Zurowski on 3/1/19.
//  Copyright © 2019 Samuel Zurowski. All rights reserved.
//

#include "ParseFile.hpp"

ParseFile::ParseFile(string inputFile, string outputFile) {
    output = outputFile;
    ifstream ifs(inputFile);
    if(!ifs.is_open()) fatal("File is not opened");
    
    string line;
    while(getline(ifs, line)) {
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        InstructionItem item(line);
        lines.push_back(item);
    } ifs.close();
}

int ParseFile::findLocation(string value) {
    for(int i = 0; i < lines.size(); i++) // if string not found in label
        if(lines[i].label == value) return i;
    fatal("Label Not Found");
    return 0;
}

unsigned long ParseFile::encodeLine(int lineNum) {
    InstructionItem item = lines.at(lineNum);
    string rs1, imm, rs2, rd, func, label, val;
    
    if(item.type == InstructionItem::R_TYPE) { // ADD r2,r3,r4
        func = item.op;
        smatch matches;
        regex reg("^[^,\\d]+([\\d]+),[^,\\d]+([\\d]+),[^,\\d]+([\\d]+)");
        if(regex_search(item.params, matches, reg)) {
        /* ^[^,\\d]+ start string not in range of comma or digit, one or more
         * ([\\d]+), group in range of digitals one or more then comma.
         * [^,\\d]+  not in range of comma or digit one or more.
         */
            rs1 = matches[2].str();
            rs2 = matches[3].str();
            rd = matches[1].str();

            val = binCode(stoi(rs1), 5) + binCode(stoi(rs2), 5) +
                binCode(stoi(rd), 5) + binCode(InstructionItem::r.at(func), 11);
            return stoul(val, 0, 2);
        }
    }
    else if(item.type == InstructionItem::J_TYPE) { //J or JAL label
        int key = findLocation(item.params);
        int offset = (key * 4) - (lineNum * 4 + 4);
        val = binCode(InstructionItem::m.at(item.op), 6) + binCode(offset, 26);
        return stoul(val, 0, 2);
    }
    else if (item.type == InstructionItem::I_TYPE){ // I TYPE
        if (item.isBranch(item.op)) { // branch instr
            //for bra
            unsigned long comma = item.params.find_first_of(',');
            string jumpInstr = string(&item.params[comma + 1], &item.params[item.params.size()]);
            
            int key = findLocation(jumpInstr);
            int offset = (key * 4) - ((lineNum * 4) + 4);
            rs1 = string(&item.params[1], &item.params[comma]);
            val = binCode(InstructionItem::m.at(item.op), 6) + binCode(stoi(rs1), 5) +
            binCode(0, 5) + binCode(offset, 16);

            return stoul(val, 0, 2);
            
        } else if(item.line.find(",") != string::npos) {
        /*
        * ^([^\\d]* start string group not in range zero or more
        * ([\\d]+) group matches one or more in range digit end group
        * [^\\d,]+ not in range of digit or comma of one or more
        * )? end group zero or one
        * ([^\\d]* start group not digit zero or more
        * ([\\d]+) group(matches one or more in range digit end group x2
        */
            smatch matches; // used for string matches
            regex exp("^([^-\\d]*([-\\d]+)[^-\\d,]+)?([^-\\d]*([-\\d]+))");
            int i = 0;
            bool isGroup3 = count(item.line.begin(), item.line.end(), ',') == 2;
            string lineStr = item.params;
            
            while (regex_search(lineStr, matches, exp)) {
                if(i < 2) {
                    if (matches[2].length() > 0) {
                        imm = matches[2].str();
                        rs1 = matches[4].str();
                    }
                    else {
                        if(imm.length() > 0) {
                            rs2 = matches[4].str();
                            break;
                        } else if(isGroup3) {
                            if(rs2.length() > 0) rs1 = matches[4].str();
                            else rs2 = matches[4].str();
                            
                        } else rs2 = matches[4].str();
                    }
                } else imm = matches[4].str();
                lineStr = matches.suffix(); // get next part of the string
                i++; // increment the counter
            }
        }
    } else if(item.op == "lhi") { // lhi
        regex ex("^([^\\d]*([\\d]+)[^\\d,]+)?([^\\d]*([\\d]+)),#([-\\d]+)");
        smatch match;
        if(regex_search(item.params, match, ex)) {
            rs1 = "0";
            imm = match[5].str();
            rs2 = match[4].str();
            val = binCode(InstructionItem::m.at(item.op), 6) +
            binCode(stoi(rs1), 5) + binCode(stoi(rs2), 5) +
            binCode(stoi(imm), 16);
            
            return stoul(val, 0, 2);
        }
    } else { // if op JR or JALR no commas
        rs1 = string(&item.params[1], &item.params[item.params.size()]);
        
        val = binCode(InstructionItem::m.at(item.op), 6) +
            binCode(stoi(rs1), 5) + binCode(0, 5) + binCode(0, 16);
        return stoul(val, 0, 2);
    } val = binCode(InstructionItem::m.at(item.op), 6) +
          binCode(stoi(rs1), 5) + binCode(stoi(rs2), 5) +
          binCode(stoi(imm), 16);
    
    return stoul(val, 0, 2);
}

void ParseFile::encodeToFile() {
    ofstream ofs(output);
//    if (!ofs.is_open()) fatal("Output file could not be opened");
    
    for(int i = 0; i < lines.size(); i++) {
        string z;
        stringstream ss;
        ss << hex << uppercase << encodeLine(i); // push the data into stream
        string f = ss.str();
        // to ensure there are enough zeros on the front
        if(f.length() < 8) z = string(8 - f.length(), '0');
        ofs << lines.at(i).line << ": " <<  "0x" << z << f << endl;
    }
    ofs.close();
}

string ParseFile::binCode(int n, int length) {
    string r;
    bool isNeg = false;
    if(n < 0) {
        stringstream s;
        s << bitset<26>(n);
        r = s.str();
        r = string(&r[r.length()- length], &r[r.length()]);
        return r;
    }
    while(n > 0) {
        r = (n % 2 == 0 ? '0' : '1') + r;  //to check if 1 or 0
        n /= 2; // shift the bit over 1 (can't use >> due to negative)
    }
    if(r.length() < length && !isNeg) r = string(length - r.length(), '0') + r;
    else r = string(length - r.length(), '1') + r;
    return r;
}
