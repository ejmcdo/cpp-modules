/*
* Module that makes file manipulation that much easier.
*/

#pragma once
#include <iostream>
#include <fstream>

/*
* fileCont - Takes a file with name fn and returns the contents as a string.
*/
std::string fileCont(std::string fn) {
    std::ifstream file(fn, std::ios::binary);
    std::string final;
    int sub;
    while (file) {
        sub = file.get();
        if (sub >= 0)
            final += sub;
    }
    file.close();
    return final;
}

/*
* fileWrite - Takes a file with name fn and writes string c into it.
*/
void fileWrite(std::string fn, std::string c) {
    std::ofstream file(fn, std::ios::binary);
    file << c;
    file.close();
}