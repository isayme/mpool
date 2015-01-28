all:
	gcc -Wall -o test test.c mpool.c mpool.h

clean:
	rm -f ./test
	
