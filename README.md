Producer-Consumer Simulation
============================

This is a simulation of the [producer-consumer problem](http://http://en.wikipedia.org/wiki/Producer-consumer_problem) for a C/C++ in UNIX class (CS 3376).

v1 has no locking and demonstrates what happens.
v2 demonstrates the problem with semaphores and 1 second sleep time for consumers.
v3 demonstrates the problem with semaphores and a random sleep time for consumers.

*Build*:
	g++ v1.cpp -o v1
	g++ v2.cpp -o v2
	g++ v3.cpp -o v3
