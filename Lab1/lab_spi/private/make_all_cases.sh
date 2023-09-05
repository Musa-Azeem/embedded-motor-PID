#!/bin/sh

# Copyright 2021 Jason Bakos, Philip Conrad, Charles Daniels
#
# Part of the University of South Carolina CSCE317 course materials. Used by
# instructors for test case generators. Do not redistribute.

# This file will generate into the test_cases folder all of the test cases
# for the SPI lab. Any name collisions will be overwritten.


cd "$(dirname "$0")/.."
mkdir -p test_cases

set -u
set -e
set -x

for i in $(seq 30) ; do
	name=$(printf "part1_%03d" $i)
	d="test_cases/$name"
	if [ -e "$d" ] ; then
		rm -rf "$d"
	fi
	mkdir -p "$d"
	python3 private/gentestcase_fixed.py \
		--input "$d/input.txt" \
		--output "$d/output.txt" \
		--cpol 0 \
		--cpha 0 \
		--stream_probability 0.0
	echo "0.72" > "$d/weight.txt"
	echo "part 1" > "$d/category.txt"

	name=$(printf "part2_%03d" $i)
	d="test_cases/$name"
	if [ -e "$d" ] ; then
		rm -rf "$d"
	fi
	mkdir -p "$d"
	python3 private/gentestcase_fixed.py \
		--input "$d/input.txt" \
		--output "$d/output.txt" \
		--cpol 0 \
		--cpha 0 \
		--stream_probability 0.50
	echo "0.16" > "$d/weight.txt"
	echo "part 2" > "$d/category.txt"

	name=$(printf "part3_%03d" $i)
	d="test_cases/$name"
	if [ -e "$d" ] ; then
		rm -rf "$d"
	fi
	mkdir -p "$d"
	cpol=1
	cpha=1
	if [ $i -lt 10 ] ; then
		cpol=0
		cpha=1
	elif [ $i -lt 20 ] ; then
		cpol=1
		cpha=0
	fi

	python3 private/gentestcase_fixed.py \
		--input "$d/input.txt" \
		--output "$d/output.txt" \
		--cpol $cpol \
		--cpha $cpha \
		--stream_probability 0.30
	echo "0.12" > "$d/weight.txt"
	echo "part 3" > "$d/category.txt"
	echo "In this test case, CPOL=$cpol and CPHA=$cpha" >> "$d/description.txt"

done
