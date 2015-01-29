all:
	gcc -Wall -shared -fPIC mpool.c mpool.h -o libmpool.so

test: all
	gcc test.c -Wl,-rpath,.,-rpath, -L. -lmpool -o test

clean:
	rm -f ./libmpool.so
	rm -f ./test
	
