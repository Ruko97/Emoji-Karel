CC = g++
CFLAGS = -g -w -Wall

lexer.o: lexer.hpp lexer.cpp
	$(CC) $(CFLAGS) -c lexer.cpp

parser.o: parser.hpp parser.cpp ast.hpp lexer.o
	$(CC) $(CFLAGS) -c parser.cpp

test_lexer: lexer.o test/lexer/getTokTest/test.cpp 
	$(CC) $(CFLAGS) $^ -o test/lexer/getTokTest/test.out -I.

test_parser: lexer.o parser.o util.hpp test/parser/test.cpp
	${CC} $(CFLAGS) $^ -o test/parser/test.out -I.

