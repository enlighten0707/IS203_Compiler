./cgen test/test.seal
cd test
gcc test.seal -no-pie -o main
./main
