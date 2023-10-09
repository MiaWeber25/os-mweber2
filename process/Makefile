all : newFork hello

clean :
	/bin/rm -rf newFork hello

fork : newFork.c
	$(CC) -g -o $@ $^

hello : hello.cpp
	$(CXX) -g -o $@ $^

test : newFork 
	./newFork 10
