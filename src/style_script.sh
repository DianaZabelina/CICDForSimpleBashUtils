#!/bin/bash

output=$(clang-format -style=Google -n src/cat/s21_cat.c src/grep/s21_grep.c 2>&1)
if [[ -n $output ]]; then
  echo "code style check failed"
  exit 1
else
  echo "code style check passed"
fi