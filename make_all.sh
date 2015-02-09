#! /bin/bash

for d in */; do echo "$d" && cd "$d" && make && cd ..; done

