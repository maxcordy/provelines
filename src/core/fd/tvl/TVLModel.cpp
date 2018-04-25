/*
 * TVLModel.cpp
 *
 * Created on 2013-07--24
 */

#include "TVLModel.hpp"

#include "math/BoolVar.hpp"
#include "math/dimacs.hpp"

#include "core/features/BoolFeatureExp.hpp"

#include "util/Assert.hpp"

#include <glog/logging.h>

#include <stdexcept>
#include <fstream>

#include <cstdlib>
#include <cstring>

using core::features::FeatureExp;
using core::features::BoolFeatureExp;
using core::features::BoolFeatureExpFactory;

using math::BoolVar;
using math::BoolFct;
using math::BoolFctFactory;
using math::parseBoolFct;

using std::string;
using std::istream;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::logic_error;
using std::tr1::shared_ptr;

using std::endl;

namespace core {
namespace fd {
namespace tvl {

const char* TVLModel::WC_PATH =             "/tmp/pvl_workingcopy.tvl";

const char* TVLModel::CLAUSES_PATH =        "/tmp/pvl_clauses.tmp";

const char* TVLModel::MAPPING_PATH =        "/tmp/pvl_mapping.tmp";

const char* TVLModel::COMMAND =             "TVLParser.jar -dimacs";

const char* TVLModel::TVL_HOME_VAR=         "TVLHOME";

TVLModel::TVLModel(istream & stream,
        shared_ptr<BoolFctFactory> & fctFactory,
        shared_ptr<BoolFeatureExpFactory> & expFactory)

    : fctFactory(fctFactory), expFactory(expFactory)  {

    // Retrieving the TVL home value
    TVLhome = std::getenv(TVL_HOME_VAR);
    if (!TVLhome) {
        throw logic_error("TVLHOME environment variable is not settled");
    }

    // Creating the working copy of the TVL model
    ofstream out(WC_PATH, ios::out);
    if (!out.is_open()) {
        throw logic_error("Error while creating the working copy of the TVL model");
    }
    out << stream.rdbuf();
    out.close();

    // Generate mapping and dimacs files from the working copy
    generateDimacsFiles();

    checkRep();
}

TVLModel::TVLModel(istream & stream, const string & filter,
        shared_ptr<BoolFctFactory> & fctFactory,
        shared_ptr<BoolFeatureExpFactory> & expFactory)

    : fctFactory(fctFactory), expFactory(expFactory) {

    // Retrieving the TVLHOME value
    TVLhome = std::getenv(TVL_HOME_VAR);
    if (!TVLhome) {
        throw logic_error("TVLHOME environment variable not settled");
    }

    // Creating the working copy of the TVL model
    ofstream out(WC_PATH, ios::out);
    out << stream.rdbuf();
    out.close();

    // Adding filter to the working copy
    addFilter(filter);

    // Generate mapping and dimacs files from the working copy
    generateDimacsFiles();

    checkRep();
}

FeatureExp * TVLModel::toFeatureExp() const {
    createBoolVars();
    ifstream clauses(CLAUSES_PATH, ios::out);
    if (!clauses.is_open()) {
        throw logic_error("Error while reading the clauses file");
    }
    FeatureExp * result = expFactory->create(parseBoolFct(clauses, fctFactory));

    checkRep();
    return result;
}

void TVLModel::addFilter(const string & filter) {
    // 1. Find the root feature name
    ifstream in(WC_PATH, ios::in);
    if (in.is_open()) {
            throw logic_error("Error while reading the working copy of the TVL m");
    }
    string line;
    string rootName;
    char buffer[1024];
    bool found = false;
    while(!found && !in.eof()) {
        std::getline(in, line);
        if(std::sscanf(line.c_str(), "root %s {", buffer) == 1) {
            found = true;
            rootName = string(buffer);
        }
    }
    in.close();
    if(!found) {
        throw logic_error("Could not find the root feature of the TVL model");
    }
    // 2. Add the filter at the end of the file
    ofstream out(WC_PATH, ios::out | ios::app);
    if (!out.is_open()) {
        throw logic_error("Error while appending to the working copy of the TVL model");
    }
    out << endl << rootName << " { " << filter << " }" << endl;
    out.close();
}

void TVLModel::generateDimacsFiles() const {
    char command[1024] = "java -jar ";
    std::strcat(command, TVLhome);
    std::strcat(command, COMMAND);
    std::strcat(command, " ");
    std::strcat(command, MAPPING_PATH);
    std::strcat(command, " ");
    std::strcat(command, CLAUSES_PATH);
    std::strcat(command, " ");
    std::strcat(command, WC_PATH);
    int result = system(command);
    if (result != 0) {
        LOG(ERROR) << "Error using the command line " << command;
        throw logic_error("Error while parsing the TVL model");
    }
}

void TVLModel::createBoolVars() const {
    ifstream file(MAPPING_PATH, ios::in);
    if (!file.is_open()) {
        throw logic_error("Error while reading the mapping file");
    }
    string line;
    int id;
    char name[1024];
    while(!file.eof()) {
        std::getline(file, line);
        if(std::sscanf(line.c_str(), "%d %s", &id, name) == 2) {
            BoolVar::createBoolVar(string(name), id);
        }
    }
    file.close();
}

void TVLModel::checkRep() const {
    if(doCheckRep) {
        // Nothing here
    }
}

} // namespace tvl
} // namespace fd
} // namespace core
