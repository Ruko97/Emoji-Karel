CC = g++
CFLAGS = -g -w -Wall

lexer.o: lexer.hpp lexer.cpp
	$(CC) $(CFLAGS) -c lexer.cpp

ast.o: ast.hpp
	$(CC) $(CFLAGS) -c ast.hpp
