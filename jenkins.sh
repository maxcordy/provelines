cd build
make test || true
make coverage
make cppcheck
make cppncss
make valgrind
cd ..
doxygen Doxyfile
