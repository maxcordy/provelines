# ProVeLines
ProVeLines is a product line of verifier for software product lines. This repository contains a new version of the tool, coded in C++.

# Main Contributors
- Maxime Cordy (product owner, main developer)
- Andreas Classen (former product owner, developer)
- Marco Willemart (developer)
- Bruno Dawagne (developer)

# Project Dependencies

Google Mock 1.6.0
http://code.google.com/p/googlemock/

Google Logging 0.3.3
http://code.google.com/p/google-glog/

CUDD 2.5.0
http://vlsi.colorado.edu/~fabio/CUDD/

JsonCpp 0.5.0
http://jsoncpp.sourceforge.net/

LTL 2 BA
http://www.lsv.ens-cachan.fr/~gastin/ltl2ba/


# Library Path

Google Logging and JsonCpp should be available on the library path
as "glog" and "json" respectively.

To install a library on the path, the headers must be dropped off in
the /usr/local/include directory and the static or dynamic libraries
in the /usr/local/lib directory. Then the following command must be
issued: $ sudo ldconfig


# Executable Path

LTL 2 BA must be available on the executable path as "ltl2ba".

This can be achieved by created a symbolic link to the executable
itself. For example:
$ sudo ln -s /opt/ltl2ba-1.1/ltl2ba /usr/local/bin/ltl2ba


# Environment Variables

$CUDD_HOME : Absolute path to the CUDD directory.

$MINIMIZE_HOME : Absolute path to the Minimize directory.


# Google Mock & Google Test

Google Test (GTest) comes bundled with Google Mock (GMock). To
install both frameworks on the library path, performs the following
commands from the home directory of GMock.

$ autoreconf -fvi

$ cd make

$ make gmock.a

$ sudo cp gmock.a /usr/local/lib/libgmock.a

$ cd ..

$ sudo cp -r include/gmock /usr/local/include

$ sudo cp -r gtest/include/gtest /usr/local/include

$ sudo ldconfig
