/*
 * files.cpp
 *
 * Created on 2013-07-26
 */

#include "files.hpp"

#include <fstream>
#include <iostream>
#include <string>

using std::ifstream;
using std::ios;
using std::ofstream;
using std::string;

namespace util {

void copyFile(const string & src, const string & dest) {
    ifstream in(src.c_str(), ios::binary | ios::in);
    ofstream out(dest.c_str(), ios::binary | ios::out);
    out << in.rdbuf();
    in.close();
    out.close();
}

} // namespace util
