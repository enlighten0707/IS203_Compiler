#!/bin/bash

cd my-test
for filename in *.seal; do
    echo "--------Test using" $filename "--------"
    ../semant $filename > tempfile
    ../semant-ref $filename > tempfile2
    diff tempfile tempfile2 > /dev/null
    if [ $? -eq 0 ]; then
        echo "Passed"
    else
        echo NOT passed
    fi
done
rm -f tempfile
rm -f tempfile2
cd ..
