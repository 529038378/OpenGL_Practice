#!/bin/bash

filename=$1
mkdir $filename
cp ./Template/Makefile ./$filename/
cp ./Template/template.cpp ./$filename/${filename}.cpp
sed -i "1s/$/&${filename}/g" ./$filename/Makefile 

