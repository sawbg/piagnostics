GCC = g++-4.9
CPPFLAGS = -g -std=gnu++14 -fpermissive -lftdi

piag:
	$(GCC) src/main.cpp -o bin/piag -lwiringPi -lpthread $(CPPFLAGS)
	doxygen doc/doxygen.config

test:
	$(GCC) src/test.cpp -o bin/test $(CPPFLAGS)

docs:
#	cp doc/doxygen.config temp
#	rm doc/* -r
#	cp temp doc/doxygen.config
	doxygen doc/doxygen.config
