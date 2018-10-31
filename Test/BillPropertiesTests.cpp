//
// Created by bfrasure on 10/30/18.
//

#include "catch.hpp"

#include "../src/BillProperties.h"
#include <iostream>
#include <zconf.h>


#include <stdio.h>  /* defines FILENAME_MAX */
// #define WINDOWS  /* uncomment this line to use it for windows.*/
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>

using namespace std;

std::string GetCurrentWorkingDir( void ) {
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    return current_working_dir;
}


TEST_CASE("Successfully load a test properties file", "[current]"){

    cout << "Directory: " << GetCurrentWorkingDir() << endl;
    BillProperties properties = BillProperties("Test/test.properties");
    cout << properties.at("numShapes") << endl;
    cout << properties.at("dt") << endl;
}