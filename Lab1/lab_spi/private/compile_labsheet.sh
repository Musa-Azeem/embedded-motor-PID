#!/bin/sh

set -e
set -u

cd "$(dirname "$0")/.."

pandoc README.md -o README.pdf --from markdown --template "eisvogel" --listings --variable urlcolor=cyan
