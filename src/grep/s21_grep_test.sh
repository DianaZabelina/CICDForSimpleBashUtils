#!/bin/bash

declare -a flags=(
    "-i"
    "-v"
    "-c"
    "-l"
    "-n"
    "-h"
)

declare -a patterns=(
    "hello"
    "[a-z]"
    "[0-9]"
)

declare -a files=(
    "s21_grep_case_1.txt"
    "s21_grep_case_2.txt"
)

check() {
    if ! [ -x "$(command -v ./s21_grep)" ]; then
        echo "s21_grep does not exist" >&2
        exit 1
    fi
}

compare() {
    str=$(echo $@ | sed "s/OPT/$options/")
    str=$(echo $str | sed -e "s/PATT/$patt/g")
    str=$(echo "$str" | sed -e "s/FILE/$file/g")
    ./s21_grep $1 $2 $patt $files >test/s21_result.log
    grep $1 $2 $patt $files >test/grep_result.log
    diff="$(diff -s test/s21_result.log test/grep_result.log)"
    ((count++))
    if [ "$diff" == "Files test/s21_result.log and test/grep_result.log are identical" ]; then
        ((success++))
        echo "s21_grep $str FUNCTIONALITY SUCCESS $count"
    else
        ((fail++))
        echo "s21_grep $str FUNCTIONALITY FAIL $count"
    fi
}

check
mkdir -p test

success=0
fail=0
count=0

for opt in "${flags[@]}"; do
    opts=$opt" -e"
    for patt in "${patterns[@]}"; do
        for file in "${files[@]}"; do
            compare $opts $patt $file
        done
    done
done

# for ((a = 0; a < 6; a++)); do
#     for ((b = a + 1; b < 6; b++)); do
#         if [ ${flags[a]} != ${flags[b]} ]; then
#             opts=${flags[a]}" "${flags[b]}" -e"
#             for patt in "${patterns[@]}"; do
#                 for file in "${files[@]}"; do
#                     compare $opts $patt $file
#                 done
#             done
#         fi
#     done
# done

# for ((a = 0; a < 6; a++)); do
#     for ((b = a + 1; b < 6; b++)); do
#         for ((c = b + 1; c < 6; c++)); do
#             if [ ${flags[a]} != ${flags[b]} ] && [ ${flags[a]} != ${flags[c]} ] && [ ${flags[b]} != ${flags[c]} ]; then
#                 opts=${flags[a]}" "${flags[b]}" "${flags[c]}" -e"
#                 for patt in "${patterns[@]}"; do
#                     for file in "${files[@]}"; do
#                         compare $opts $patt $file
#                     done
#                 done
#             fi
#         done
#     done
# done

# for ((a = 0; a < 6; a++)); do
#     for ((b = a + 1; b < 6; b++)); do
#         for ((c = b + 1; c < 6; c++)); do
#             for ((d = c + 1; d < 6; d++)); do
#                 if [ ${flags[a]} != ${flags[b]} ] && [ ${flags[a]} != ${flags[c]} ] &&
#                     [ ${flags[a]} != ${flags[d]} ] && [ ${flags[b]} != ${flags[c]} ] &&
#                     [ ${flags[b]} != ${flags[d]} ] && [ ${flags[c]} != ${flags[d]} ]; then
#                     opts=${flags[a]}" "${flags[b]}" "${flags[c]}" "${flags[d]}" -e"
#                     for patt in "${patterns[@]}"; do
#                         for file in "${files[@]}"; do
#                             compare $opts $patt $file
#                         done
#                     done
#                 fi
#             done
#         done
#     done
# done

for ((a = 0; a < 6; a++)); do
    for ((b = a + 1; b < 6; b++)); do
        for ((c = b + 1; c < 6; c++)); do
            for ((d = c + 1; d < 6; d++)); do
                for ((e = d + 1; e < 6; e++)); do
                    if [ ${flags[a]} != ${flags[b]} ] && [ ${flags[a]} != ${flags[c]} ] &&
                        [ ${flags[a]} != ${flags[d]} ] && [ ${flags[a]} != ${flags[e]} ] &&
                        [ ${flags[b]} != ${flags[c]} ] && [ ${flags[b]} != ${flags[d]} ] &&
                        [ ${flags[b]} != ${flags[e]} ] && [ ${flags[c]} != ${flags[d]} ] &&
                        [ ${flags[c]} != ${flags[e]} ] && [ ${flags[d]} != ${flags[e]} ]; then
                        opts=${flags[a]}" "${flags[b]}" "${flags[c]}" "${flags[d]}" "${flags[e]}" -e"
                        for patt in "${patterns[@]}"; do
                            for file in "${files[@]}"; do
                                compare $opts $patt $file
                            done
                        done
                    fi
                done
            done
        done
    done
done

opt="-e"
patt="hello"
file="not_exist.txt"
compare "" $opt $patt $file
compare "-s" $opt $patt $file

rm -rf test

echo "FAIL: $fail"
echo "SUCCESS: $success"
echo "ALL: $count"
