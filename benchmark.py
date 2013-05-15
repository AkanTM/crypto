#!/usr/bin/env python

import sys
import subprocess


no_test = 100

files = ['test_1k', 'test_10k', 'test_100k', 'test_1M', 'test_10M']

execs = ['./botan_whirlpool', './gcrypt_whirlpool', './cryptopp_whirlpool']


g = open("results.txt", "w")


for exe in execs:
	print "Testing %s ..." %exe
	g.write(exe+"\n")

	for afile in files:

		total_time = 0
		total_memory = 0	

		for i in xrange(no_test):
			p = subprocess.Popen([exe, afile], stdout=subprocess.PIPE)
			p.wait()

			out = p.stdout.read()
			time = float ( out.split("\n")[1] )
			memory = long( out.split("\n")[2] )
			total_time +=time
			total_memory += memory

		print "Done %s with time: %f seconds and peak rss: %ld" %(afile, total_time / no_test, total_memory/no_test)

		g.write("\t%s: %f seconds; %ld peak rss\n" %(afile , total_time / no_test, total_memory/no_test))

	g.write("\n")


g.close()




