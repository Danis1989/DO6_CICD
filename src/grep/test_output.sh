#!/bin/bash

if [[ "$#" -ne 2 ]] 
then
    echo "use 2 arguments"
    exit 1
else  
    if [[ $1 == 0 ]]
    then
        echo -e "\033[0;32m GREP : leaks test : SUCCESS \033[0m"
    else
        echo -e "\033[0;31m GREP : leaks test : FAILURE \033[0m"
        exit 1
    fi
    if [[ $2 == 0 ]]
    then
        echo -e "\033[0;32m GREP : performance test : SUCCESS \033[0m"
    else
		echo -e "\033[0;31m GREP : performance test : FAILURE \033[0m"
        exit 1
    fi
fi