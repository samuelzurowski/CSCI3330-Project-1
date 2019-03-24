//
//  main.cpp
//  ComputerOrg1
//
//  Created by Samuel Zurowski on 3/1/19.
//  Copyright © 2019 Samuel Zurowski. All rights reserved.
//

#include "ParseFile.hpp"

int main(int argc, char *argv[]) {
    if (argc == 3) { // check if its has correct # args
        cout << "Encoding the file: " << argv[1] << endl;
        ParseFile p(argv[1], argv[2]);
        p.encodeToFile();
        cout << "Encoding sucessful, going to the file: " << argv[2] << endl;
        return 1;
    }

    cout << "[+] Project #1 Computer Organization - Samuel H. Zurowski" << endl;
    cout << "Usage: " << argv[0] << " <inputfile> <outputfile>" << endl;
    return 0;
}
