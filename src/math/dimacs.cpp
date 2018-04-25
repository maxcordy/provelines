/*
 * dimacs.cpp
 *
 * Created on July 26, 2013
 */

#include "dimacs.hpp"

#include "BoolFct.hpp"
#include "BoolFctFactory.hpp"
#include "BoolVar.hpp"

#include "util/Assert.hpp"

#include <glog/logging.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using std::auto_ptr;
using std::istream;
using std::logic_error;
using std::string;

namespace math {

BoolFct * parseBoolFct(istream & stream, const BoolFctFactoryPtr & factory) {
    // Eating comments and declaration
    bool found = false;
    while (!stream.eof() && !found) {
        string line;
        std::getline(stream, line);
        if (line[0] == 'p') {
            found = true;
        }
    }
    if (!found) {
        throw logic_error("Wrong dimacs file");
    }
    // Parsing boolean function
    int id;
    auto_ptr<BoolFct> literal;
    auto_ptr<BoolFct> disjunction;
    BoolFct * result = factory->getTrue();
    stream >> id;
    while (!stream.eof()) {
        disjunction.reset(factory->getFalse());
        while (id != 0) {
            if (id > 0) {
                literal.reset(factory->create(BoolVar::makeBoolVar(id)));
                disjunction->disjunction(*literal);
            } else {
                id = -id;
                literal.reset(factory->create(BoolVar::makeBoolVar(id)));
                literal->negation();
                disjunction->disjunction(*literal);
            }
            stream >> id;
        }
        result->conjunction(*disjunction);
        stream >> id;
    }
    return result;
}

} // namespace math
