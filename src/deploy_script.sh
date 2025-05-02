#!/bin/bash

src_dir="${CI_PROJECT_DIR}/src"
remote_user="adrienek"
remote_host="192.168.100.2"
remote_dir="/usr/local/bin"

scp "$src_dir/cat/s21_cat" "$remote_user@$remote_host:$remote_dir"

if [ $? -ne 0 ]; then
  echo "failed to copy s21_cat to remote host"
  exit 1
fi

scp "$src_dir/grep/s21_grep" "$remote_user@$remote_host:$remote_dir"

if [ $? -ne 0 ]; then
  echo "failed to copy s21_grep to remote host"
  exit 1
fi

ssh "$remote_user@$remote_host" ls -lah "$remote_dir"

if [ $? -ne 0 ]; then
  echo "failed to list files on remote host"
  exit 1
fi