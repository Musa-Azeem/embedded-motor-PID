#!/bin/sh

set -e
set -u
set -x

cd "$(dirname "$0")/.."

# tidy up to reduce tarball sizes
make -C solution_c clean
make -C code_c clean
rm -rf ./utils/python_utils/__pycache__ ./grader/__pycache__ ./*/__pycache__

NAME=lab_2023sp_spi
CWD="$(pwd)"

# remove any existing stuff just to be safe
rm -f README.pdf template_c_$NAME.tar.gz template_python_$NAME.tar.gz solution_c_$NAME.tar.gz solution_python_$NAME.tar.gz


# make the lab sheet
./private/compile_labsheet.sh

# template_c.tar.gz
TMP="$(mktemp -d)"
LAB="$TMP/$NAME/"
mkdir -p "$LAB"
cp -r ./code_c "$LAB/code"
cp -r ./utils "$LAB/utils"
cp -r ./grader "$LAB/grader"
cp -r ./test_cases "$LAB/test_cases"
cp README.pdf "$LAB"
cp README.md "$LAB"
cp grade.sh "$LAB"
cd "$TMP"
tar cvfz ./template_c_$NAME.tar.gz "$NAME"
cd "$CWD"
mv "$TMP/template_c_$NAME.tar.gz" ./
rm -rf "$TMP"

# template_python.tar.gz
TMP="$(mktemp -d)"
LAB="$TMP/$NAME/"
mkdir -p "$LAB"
cp -r ./code_python "$LAB/code"
cp -r ./utils "$LAB/utils"
cp -r ./grader "$LAB/grader"
cp -r ./test_cases "$LAB/test_cases"
cp README.pdf "$LAB"
cp README.md "$LAB"
cp grade.sh "$LAB"
cd "$TMP"
tar cvfz ./template_python_$NAME.tar.gz "$NAME"
cd "$CWD"
mv "$TMP/template_python_$NAME.tar.gz" ./
rm -rf "$TMP"

# solution_c.tar.gz
TMP="$(mktemp -d)"
LAB="$TMP/$NAME/"
mkdir -p "$LAB"
cp -r ./solution_c "$LAB/code"
cp -r ./utils "$LAB/utils"
cp -r ./grader "$LAB/grader"
cp -r ./test_cases "$LAB/test_cases"
cp README.pdf "$LAB"
cp README.md "$LAB"
cp grade.sh "$LAB"
cd "$TMP"
tar cvfz ./solution_c_$NAME.tar.gz "$NAME"
cd "$CWD"
mv "$TMP/solution_c_$NAME.tar.gz" ./
rm -rf "$TMP"

# solution_python.tar.gz
TMP="$(mktemp -d)"
LAB="$TMP/$NAME/"
mkdir -p "$LAB"
cp -r ./solution_python "$LAB/code"
cp -r ./utils "$LAB/utils"
cp -r ./grader "$LAB/grader"
cp -r ./test_cases "$LAB/test_cases"
cp README.pdf "$LAB"
cp README.md "$LAB"
cp grade.sh "$LAB"
cd "$TMP"
tar cvfz ./solution_python_$NAME.tar.gz "$NAME"
cd "$CWD"
mv "$TMP/solution_python_$NAME.tar.gz" ./
rm -rf "$TMP"
