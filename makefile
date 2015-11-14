all:
	rm test
	g++ -g test.cpp -o test -std=gnu++14 -lftdi -fpermissive
