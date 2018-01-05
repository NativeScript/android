#! /bin/bash

if [ "$#" -lt "2" ]; then
    echo "No arguments supplied. Usage: $0 [path-to-jars-to-generate-metadata-for] [output-path]"
    exit
fi

if [ -d "bin" ]; then
    echo "bin directory found! In its current version the metadata generator uses a bin directory for a temporary metadata output location!"
    
    echo "Listing existing bin directory files"
    pwd
    ls -l $(pwd)/bin/*
    echo "Listing existing bin directory files. end"

    exit
fi

if [ ! -d $2 ]; then
    mkdir $2
fi

resolvedpath=$(readlink -f $0)
executabledir=$(dirname $resolvedpath)

mkdir bin

java -cp $executabledir/../classes com.telerik.metadata.Generator $1
if [ $? -ne 0 ]; then
    exit $?
fi

echo "Listing input files"
pwd
ls -l $1/*
echo "Listing input files. end"

echo "Listing output files"
pwd
ls -l $(pwd)/bin/*
echo "Listing output files. end"

mv bin/* $2

rmdir bin
