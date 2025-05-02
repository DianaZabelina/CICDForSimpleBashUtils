#!/bin/bash

cd src/cat
cat_output=$(bash s21_cat_test.sh)
cat_fail=$(echo "$cat_output" | grep "FAIL:" | awk '{print $2}')
cat_success=$(echo "$cat_output" | grep "SUCCESS:" | awk '{print $2}')
echo -e "s21_cat_test_result\nFAIL $cat_fail, SUCCESS $cat_success"
cd ../grep
grep_output=$(bash s21_grep_test.sh)
grep_fail=$(echo "$grep_output" | grep "FAIL:" | awk '{print $2}')
grep_success=$(echo "$grep_output" | grep "SUCCESS:" | awk '{print $2}')
echo -e "s21_grep_test_result\nFAIL $grep_fail, SUCCESS $grep_success"
if [ "$cat_fail" -ne 0 ] || [ "$grep_fail" -ne 0 ]; then exit 1; fi