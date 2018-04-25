#!/bin/sh

input_dir=../integration/inputs
bin_dir=bin/Debug

cd build

# Integration tests
make test

# Memory leak detection
for input in `ls "${input_dir}"/*.*`
do
    filename=$(basename "$input")
    filename="${filename%.*}"
    echo Running memcheck with "${filename}"
    valgrind --xml=yes --xml-file=valgrind-"${filename}".xml\
        "${bin_dir}"/provelines "${input}"
done

# Profiling
for input in `ls "${input_dir}"/*.*`
do
    filename=$(basename "$input")
    filename="${filename%.*}"
    echo Running callgrind with "${filename}"
    valgrind --tool=callgrind --callgrind-out-file=callgrind."${filename}".out\
        "${bin_dir}"/provelines "${input}"
done
