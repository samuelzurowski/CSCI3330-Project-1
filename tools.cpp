//
//  tools.cpp
//  ComputerOrg1
//
//  Created by Samuel Zurowski on 3/2/19.
//  Copyright © 2019 Samuel Zurowski. All rights reserved.
//

#include "tools.hpp"
void fatal(string error) {
    cout << "Error: " << error << " \nAborting Program..." << endl;
    exit(0);
}
