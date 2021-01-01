# tan, tan, tan, tan, tan
./cgen test/tan.seal -o test/tan.s
cd test
gcc tan.s -no-pie -o main
./main
cd ../test-answer/
./tan
