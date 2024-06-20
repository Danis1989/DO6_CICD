#!/bin/bash

if [ -f "clang_output.txt" ];
then
	if [ -s "clang_output.txt" ];
	then
		cat "clang_output.txt"
		echo -e "\033[0;31mCode is not clang-formatted\033[0m"
		exit 1
    else 
		echo -e "\033[0;32mCode is clang-formatted\033[0m"
	fi
else
	echo "clang_output.txt file doesn't exist"
	exit 1
fils