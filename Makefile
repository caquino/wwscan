all:	
	cd libping-1.14 && ./configure && make install
	cd src && $(MAKE) all

clean:
	cd libping-1.14 && make clean
	cd src && make clean
