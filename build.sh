#!/bin/bash


g++ -o botan_whirlpool botan.cpp `botan-config-1.10 --cflags --libs`
gcc -o gcrypt_whirlpool gcrypt.c `libgcrypt-config --cflags --libs`
g++ -g3 -ggdb -O0 -Wall -Wextra -Wno-unused -o cryptopp_whirlpool cryptopp_whirlpool.cpp -lcryptopp