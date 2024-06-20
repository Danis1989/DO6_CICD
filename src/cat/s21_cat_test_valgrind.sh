#!/bin/bash

SUCCESS_LEAK=0
SUCCESS_WORK=0
FAIL_LEAK=0
FAIL_WORK=0
COUNTER_LEAK=0
COUNTER_WORK=0
DIFF_RES=""

testing_leak()
{
    template=$(echo $@ | sed "s/VAR/$var/")
    valgrind --log-file="s21_cat_test_valgrind.log" --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./s21_cat $template > s21_cat_test_leak.log
    leak=$(grep ERROR s21_cat_test_valgrind.log)
    (( COUNTER_LEAK++ ))
    if [[ $leak == *"0 errors from 0 contexts"* ]]
    then
      (( SUCCESS_LEAK++ ))
        echo -en "\033[36m[fail_leak=$FAIL_LEAK|success_leak=$SUCCESS_LEAK|all_leak_tests=$COUNTER_LEAK]" 
        testing_work template
    else
      (( FAIL_LEAK++ ))
        echo -en "\033[0m\033[0m\033[31m[fail_leak=$FAIL_LEAK|success_leak=$SUCCESS_LEAK|all_leak_tests=$COUNTER_LEAK]" 
        testing_work template
    fi
    rm s21_cat_my_work.log
    rm s21_cat_orginal_work.log
    rm s21_cat_test_leak.log
    rm s21_cat_test_valgrind.log
}

testing_work()
{
    ./s21_cat $template > s21_cat_my_work.log
    cat $template > s21_cat_orginal_work.log
    DIFF_RES="$(diff -s s21_cat_my_work.log s21_cat_orginal_work.log)"
    (( COUNTER_WORK++ ))
    if [ "$DIFF_RES" == "Files s21_cat_my_work.log and s21_cat_orginal_work.log are identical" ]
    then
      (( SUCCESS_WORK++ ))
        echo -e "\033[34m[fail_work=$FAIL_WORK|success_work=$SUCCESS_WORK|all_work_tests=$COUNTER_WORK]\033[37m cat $template\n"
    else
      (( FAIL_WORK++ ))
        echo -e "\033[0m\033[0m\033[31m[fail_work=$FAIL_WORK|success_work=$SUCCESS_WORK|all_work_tests=$COUNTER_WORK]\033[37m cat $template\n"
    fi
}

for var1 in b e n s t v
do
    for i in "VAR cat_1.txt" "VAR cat_2.txt"
    do
        var="-$var1"
        testing_leak $i
    done
done

# for var1 in b e n s t v
# do
#     for var2 in b e n s t v
#     do
#         if [ $var1 != $var2 ]
#         then
#             for i in "VAR cat_1.txt" "VAR cat_2.txt" "VAR cat_3.txt" "VAR cat_4.txt" "VAR cat_5.txt" "VAR cat_6.txt"
#             do
#                 var="-$var1 -$var2"
#                 testing_leak $i
#             done
#         fi
#     done
# done

# for var1 in b e n s t v
# do
#    for var2 in b e n s t v
#    do
#        for var3 in b e n s t v
#        do
#            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
#            then
#                for i in "VAR cat_1.txt" "VAR cat_2.txt" "VAR cat_3.txt" "VAR cat_4.txt" "VAR cat_5.txt" "VAR cat_6.txt"
#                do
#                    var="-$var1 -$var2 -$var3"
#                    testing_leak $i
#                done
#            fi
#        done
#    done
# done

var="-b -e -n -s -t -v"
testing_leak "VAR cat_1.txt"

var="-"
testing_leak "test_1_cat.txt -n"

# echo -en "\033[31mFAIL_LEAK: $FAIL_LEAK\033[0m | "
# echo -e "\033[32mSUCCESS_LEAK: $SUCCESS_LEAK\033[0m"
# echo -en "\033[31mFAIL_WORK: $FAIL_WORK\033[0m | "
# echo -e "\033[32mSUCCESS_WORK: $SUCCESS_WORK\033[0m"
# echo "ALL: $COUNTER_WORK"

./test_output.sh $FAIL_LEAK $FAIL_WORK