cmake_minimum_required(VERSION 2.6)

# Options
option(test "Enable unit testing." ON)
option(cgtest "Precompiled Google Test." OFF) # not used anymore, see below
option(coverage "Enable coverage report." OFF)
option(integration "Enable integration testing." OFF)

# Project configuration
project(ProVeLines)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include) if you want your own
# include/ directory then you can do include_directories(${COMMON_INCLUDES})
# in other cmakelists.txt files

# Version number
set(PVL_VERSION_MAJOR 2)
set(PVL_VERSION_MINOR 0)
set(PVL_VERSION ${PVL_VERSION_MAJOR}.${PVL_VERSION_MINOR})

#find_package(Subversion)
#    if(Subversion_FOUND)
#      # Use the FindSubversion.cmake module to get the svn rev and append
#      # that to the patch version.
#      Subversion_WC_INFO(${PROJECT_SOURCE_DIR} Project)
#      message("Current revision is ${Project_WC_REVISION}")
#      set (PVL_REVISION ${Project_WC_REVISION})
#endif(Subversion_FOUND)

configure_file (
    "${PROJECT_SOURCE_DIR}/Version.hpp.in"
    "${PROJECT_BINARY_DIR}/Version.hpp"
)


################################
# Normal Libraries & Executables
################################

# Adds include directories to the build
include_directories(
    ${PROJECT_BINARY_DIR}   # for version.hpp
    $ENV{CUDD_HOME}/include
    src
)

# Specifies directories in which the linker will look for libraries
# Note: the command will apply only to targets created after it is called
link_directories(
    $ENV{CUDD_HOME}/obj
    $ENV{CUDD_HOME}/cudd
    $ENV{CUDD_HOME}/mtr
    $ENV{CUDD_HOME}/st
    $ENV{CUDD_HOME}/epd
    $ENV{CUDD_HOME}/util
)

# Generation of the list of the source files
file(
    GLOB_RECURSE  # Searches for files recursively in the given folders
    source_files  # Variable holding the list of files
    src/*
)

add_library(provelines_lib ${source_files})
add_executable(provelines src/main.cpp)
# key idea: SEPARATE OUT your main() function into its own file so it can be
# its own executable separating out main() means you can add this library to
# be used elsewhere

target_link_libraries(provelines
    provelines_lib           # ProVeLines
    glog                     # Google Logging
    obj cudd mtr st epd util # CUDD
    json                     # JsonCpp
)


################################
# Testing
################################

if (test OR integration)

message(STATUS "Enabling testing")

# Adds another build target, which is test for Makefile generators
enable_testing()

# GTest is now on the library path along with GMock.
# The commented code below remains for documentation since it took me a while
# to figure it out :-)

# if (cgtest)
#     # Adds the Google Test library
#     include_directories($ENV{GTEST_HOME}/include)
#     link_directories($ENV{GTEST_HOME}/build)
# else()
#     # This adds another subdirectory, which has project(gtest)
#     add_subdirectory(lib/gtest-1.6.0)

#     # Include the gtest library
#     # gtest_SOURCE_DIR is available due to project(gtest) above
#     include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
# endif() # cgtest

endif() # test OR integration

################################
# Unit Tests
################################

if (test)

message(STATUS "Enabling unit testing")

# Generation of the list of the test files
file(
  GLOB_RECURSE
  test_files
  test/*
)
include_directories(src)

add_executable(runUnitTests ${test_files})

target_link_libraries(runUnitTests
    gmock                    # Google Mock & Google Test
    pthread                  # needed by gtest if precompiled
    provelines_lib           # extra linking for the project
    glog                     # Google Logging
    obj cudd mtr st epd util # CUDD
    json                     # JsonCpp
)

# This is so you can do 'make test' to see all your tests run, instead of
# manually running the executable runUnitTests to see those specific tests.
add_test(
    NAME UnitTests
    COMMAND runUnitTests --gtest_output=xml:gtest-result.xml
)

endif() # test

################################
# Integration Tests
################################

if (integration)

message(STATUS "Enabling integration testing")

# Generation of the list of the integration tests files
file(
  GLOB_RECURSE
  integration_files
  integration/*
)
include_directories(src)

add_executable(runIntegrationTests ${integration_files})

target_link_libraries(runIntegrationTests
    gmock                    # Google Mock & Google Test
    pthread                  # needed by gtest if precompiled
    provelines_lib           # extra linking for the project
    glog                     # Google Logging
    obj cudd mtr st epd util # CUDD
    json                     # JsonCpp
)

add_test(
    NAME IntegrationTests
    COMMAND runIntegrationTests --gtest_output=xml:integration-result.xml
)

endif() # integration


################################
# Code analysis
################################

# cppcheck target
add_custom_command(
    OUTPUT cppcheck-result.xml
    COMMAND cppcheck -v --enable=all --xml --check-config -I../src ../src 2> cppcheck-result.xml
)
add_custom_target(cppcheck DEPENDS cppcheck-result.xml)

# cppncss target
add_custom_command(
    OUTPUT cppncss-result.xml
    COMMAND cppncss -r -v -x -k -f=cppncss-result.xml ../src/
)
add_custom_target(cppncss DEPENDS cppncss-result.xml)

# valgrind target
add_custom_command(
    OUTPUT valgrind-result.xml
    COMMAND valgrind --xml=yes --xml-file=valgrind-result.xml bin/Debug/runUnitTests
)
add_custom_target(valgrind DEPENDS valgrind-result.xml)


################################
# Coverage
################################

if (coverage)

message(STATUS "Enabling coverage report")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(COVERAGE_FILE "${CMAKE_BINARY_DIR}/coverage.xml")
set(COVERAGE_COMMAND_FILE "${CMAKE_BINARY_DIR}/coverage.cmake")

# gcovr cannot write directly to a file so the execution needs to
# be wrapped in a cmake file that generates the file output
file(
    WRITE ${COVERAGE_COMMAND_FILE}
    "SET(ENV{LANG} en)\n"
)
file(
    APPEND ${COVERAGE_COMMAND_FILE}
    "EXECUTE_PROCESS(COMMAND gcovr -x -r \"${CMAKE_SOURCE_DIR}\" OUTPUT_FILE \"${COVERAGE_FILE}\" WORKING_DIRECTORY \"${CMAKE_BINARY_DIR}\")\n"
)

add_custom_command(
    OUTPUT ${COVERAGE_FILE}
    COMMAND ${CMAKE_COMMAND}
    ARGS -P ${COVERAGE_COMMAND_FILE}
    COMMENT "Generating coverage report"
    VERBATIM
)

add_custom_target(coverage DEPENDS ${COVERAGE_FILE})

endif() # coverage
