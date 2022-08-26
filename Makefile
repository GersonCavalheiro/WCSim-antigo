NAME = main
SOURCES = $(wildcard *.cpp)
OBJS = $(SOURCES:.cpp=.o)
INCLUDES = $(wildcard *.h)
CC = g++
CFLAGS = -std=c++17 -g -DFATVM
%CFLAGS = -std=c++17 -g -DTINYVM
%CFLAGS = -std=c++17 -ffast-math -march=native
EXEC = main
	
main : $(OBJS)
	$(CC) -o $(NAME) $^ 

%.o : %.cpp $(INCLUDES)
	$(CC) -c $(CFLAGS) $< -o $@

git:
	rm -rf *.o $(NAME)
	git add .
	git commit -m "Backup"
	git push

clean:
	rm -rf *.o $(NAME)

run: $(EXEC)
	./$(EXEC) 4 < input/grande.dob
