NAME = main
INPUTFILES = sipht_g cloud0 passwd_g network_g
SOURCES = $(wildcard *.cpp)
OBJS = $(SOURCES:.cpp=.o)
INCLUDES = $(wildcard *.h)
CC = g++
%CFLAGS = -std=c++17 -g -DFATVM
%CFLAGS = -std=c++17 -g -DTHINVM
CFLAGS = -g -std=c++17 -ffast-math -march=native
EXEC = main
	
main : $(OBJS)
	$(CC) -o $(NAME) $^ 

%.o : %.cpp $(INCLUDES) Makefile
	$(CC) -c $(CFLAGS) $< -o $@

git:
	rm -rf *.o $(NAME)
	git add .
	git commit -m "Antes de fazer Node : public BareMetal"
	git push

clean:
	rm -rf *.o $(NAME)

run: $(EXEC)
	./$(EXEC) $(INPUTFILES)
