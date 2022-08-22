CC = g++
CFLAGS = -g- w -Wall

lexer.o: lexer.h lexer.cpp
	$(CC) $(CFLAGS) -c lexer.cpp
