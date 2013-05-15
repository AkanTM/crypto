crypto
======

oss-3 benchmark crypto Whirlpool hashing algorithm

Use ./build.sh to compile all 3 programs

Use benchmark.py to test Whirlpool from botan, libgcrypt and crypto++
Test files were generated random from /dev/urandom
Tested each program with each file 100 times.
Mean results (time and peak rss) can be seen in file "results.txt":

Please keep in mind that only the actual hash time was considered, 
the file reading and the hash structure updating times were ignored.
