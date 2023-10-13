all : wordCount 

clean :
	/bin/rm -rf wordCount 

fork : wordCount.c
	$(CC) -g -o -lpthread

test : wordCount 
	./wordCount
