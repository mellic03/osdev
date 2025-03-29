#!/bin/bash


# find build -name '*.cpp' -exec echo {} \;

./_build.sh kshell
./_build.sh makewin
./_build.sh sum

# for filename in src/*.cpp; do
#     echo $filename
#     # $(dirname "$0")/_build.sh $filename
# done

