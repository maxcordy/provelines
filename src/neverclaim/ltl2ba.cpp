/*
 * ltl2ba.hpp
 *
 * Created on 2013-07-31
 */

#include "ltl2ba.hpp"

#include "util/Assert.hpp"

#include <cstdlib>

using std::string;

namespace neverclaim {

bool ltl2ba(const string & ltlProperty, const string & filePath) {

    string command("ltl2ba -f \"!(");
    command.append(ltlProperty);
    command.append(")\" > ");
    command.append(filePath);

    int result = system(command.c_str());

    return result == 0;
}

} // namespace neverclaim
