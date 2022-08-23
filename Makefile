CC = g++
CFLAGS = -g -w -Wall

lexer.o: lexer.hpp lexer.cpp
	$(CC) $(CFLAGS) -c lexer.cpp

parser.o: parser.hpp parser.cpp ast.hpp lexer.o
	$(CC) $(CFLAGS) -c parser.cpp

