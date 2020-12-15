#!/bin/bash
script=$1
cp -f $script semant
echo "--------Test" $script "--------"
i=1
for filename in *.seal; do
    echo "------NO $i --- using" $filename "--------"
    ./semant $filename > tempfile
    diff tempfile answer/$filename.out > /dev/null
    if [ $? -eq 0 ] ; then
        echo -e "\033[32m passed \033[0m"
    else
        echo -e "\033[31m difference occured! \033[0m" 
    fi
    rm -f tempfile
    let 'i+=1'
done
rm -f semant