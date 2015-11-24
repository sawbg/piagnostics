GCC = g++-4.9
CPPFLAGS = -g -std=gnu++14 -fpermissive -lftdi

piag:
	$(GCC) src/main.cpp -o bin/piag -lwiringPi -lwiringPiDev -lpthread $(CPPFLAGS)
	
test:
	$(GCC) src/test.cpp -o bin/test $(CPPFLAGS)

docs:
	mv doc/doxygen.config .
	rm -r doc
	mkdir doc
	mv doxygen.config doc/
	doxygen doc/doxygen.config

count:
	wc -l src/*  # works assuming there's no subdirectories
