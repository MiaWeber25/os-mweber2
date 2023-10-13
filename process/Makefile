all : fork2 

clean :
	/bin/rm -rf fork2 

fork : fork2.c
	$(CC) -g -o $@ $^

test : fork2 
	./fork2 9
