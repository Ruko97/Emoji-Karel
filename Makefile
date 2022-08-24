CC = g++
CFLAGS = -g -w -Wall
INCLUDE = -I.

lexer.o: lexer.hpp lexer.cpp
	$(CC) $(CFLAGS) -c lexer.cpp

ast.o: ast.hpp ast.cpp lexer.o
	$(CC) $(CFLAGS) -c ast.cpp

parser.o: parser.hpp parser.cpp lexer.o ast.o
	$(CC) $(CFLAGS) -c parser.cpp

test_lexer: lexer.o test/lexer/getTokTest/test.cpp 
	$(CC) $(CFLAGS) $^ -o test/lexer/getTokTest/test.out $(INCLUDE)

test_parser: lexer.o ast.o parser.o util.hpp test/parser/test.cpp
	${CC} $(CFLAGS) $^ -o test/parser/test.out $(INCLUDE)

showTokens.out: lexer.o ast.o parser.o test/lexer/showTokens.cpp
	$(CC) $(CFLAGS) $^ -o test/lexer/showTokens.out $(INCLUDE)

