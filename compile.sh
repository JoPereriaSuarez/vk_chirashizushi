#!/usr/bin/bash

bin_dir="$PWD/bin/"
src_dir="$PWD/src/"
header_dir="$PWD/headers/"
args=()
for file in $(find "$src_dir" -type f -name '*c')
do
    filename=$(basename "$file" .c)
    args+=("$bin_dir$filename".o)
    gcc -g -c -I "$src_dir" "$file" -o "$bin_dir$filename".o
done

gcc -g -fsanitize=address "${args[@]}" main.c -o chirashizushi.out -lSDL2 -lvulkan 