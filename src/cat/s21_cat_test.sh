#!/bin/bash

declare -a flags=(
    "-b"
    "-e"
    "-n"
    "-s"
    "-t"
)

declare -a files=(
    "s21_cat_case_1.txt"
    "s21_cat_case_2.txt"
    "s21_cat_case_3.txt"
)

declare -a exotic=(
    "-b s21_cat_case1.txt -e"
    "-benst s21_cat_case3.txt"
)

check() {
    if ! [ -x "$(command -v ./s21_cat)" ]; then
        echo "s21_cat does not exist" >&2
        exit 1
    fi
}

compare() {
    str=$(echo $@ | sed "s/OPT/$options/")
    str=$(echo "$str" | sed -e "s/FILE/$file/g")
    ./s21_cat $1 $files >test/s21_result.log
    cat $1 $files >test/cat_result.log
    diff="$(diff -s test/s21_result.log test/cat_result.log)"
    ((count++))
    if [ "$diff" == "Files test/s21_result.log and test/cat_result.log are identical" ]; then
        ((success++))
        echo "s21_cat $str FUNCTIONALITY SUCCESS $count"
    else
        ((fail++))
        echo "s21_cat $str FUNCTIONALITY FAIL $count"
    fi
}

check
mkdir -p test

success=0
fail=0
count=0

for opt in "${flags[@]}"; do
    for file in "${files[@]}"; do
        compare $opt $file
    done
done

for ((a = 0; a < 5; a++)); do
    for ((b = a + 1; b < 5; b++)); do
        if [ ${flags[a]} != ${flags[b]} ]; then
            opts=${flags[a]}" "${flags[b]}
            for file in "${files[@]}"; do
                compare $opts $file
            done
        fi
    done
done

for ((a = 0; a < 5; a++)); do
    for ((b = a + 1; b < 5; b++)); do
        for ((c = b + 1; c < 5; c++)); do
            if [ ${flags[a]} != ${flags[b]} ] && [ ${flags[a]} != ${flags[c]} ] && [ ${flags[b]} != ${flags[c]} ]; then
                opts=${flags[a]}" "${flags[b]}" "${flags[c]}
                for file in "${files[@]}"; do
                    compare $opts $file
                done
            fi
        done
    done
done

for ((a = 0; a < 5; a++)); do
    for ((b = a + 1; b < 5; b++)); do
        for ((c = b + 1; c < 5; c++)); do
            for ((d = c + 1; d < 5; d++)); do
                if [ ${flags[a]} != ${flags[b]} ] && [ ${flags[a]} != ${flags[c]} ] &&
                    [ ${flags[a]} != ${flags[d]} ] && [ ${flags[b]} != ${flags[c]} ] &&
                    [ ${flags[b]} != ${flags[d]} ] && [ ${flags[c]} != ${flags[d]} ]; then
                    opts=${flags[a]}" "${flags[b]}" "${flags[c]}" "${flags[d]}
                    for file in "${files[@]}"; do
                        compare $opts $file
                    done
                fi
            done
        done
    done
done

for case in "${exotic[@]}"; do
    compare $case
done

rm -rf test

echo "FAIL: $fail"
echo "SUCCESS: $success"
echo "ALL: $count"
