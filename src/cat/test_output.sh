#!/bin/bash

if [[ "$#" -ne 2 ]] 
then
    echo "use 2 arguments"
    exit 1
else  
    if [[ $1 == 0 ]]
    then
        echo -e "\033[0;32m CAT : leaks test : SUCCESS \033[0m"
    else
        echo -e "\033[0;31m CAT : leaks test : FAILURE \033[0m"
        exit 1
    fi
    if [[ $2 == 0 ]]
    then
        echo -e "\033[0;32m CAT : performance test : SUCCESS \033[0m"
    else
		echo -e "\033[0;31m CAT : performance test : FAILURE \033[0m"
        exit 1
    fi
fi
