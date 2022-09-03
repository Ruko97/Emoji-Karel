CC = g++
CFLAGS = -g -w -Wall
INCLUDE = -I.

lexer.o: lexer.hpp lexer.cpp
	$(CC) $(CFLAGS) -c lexer.cpp

codegen.o: ast.hpp codegen.cpp lexer.o
	$(CC) $(CFLAGS) -c codegen.cpp

dump.o: ast.hpp dump.cpp lexer.o
	$(CC) $(CFLAGS) -c dump.cpp

parser.o: parser.hpp parser.cpp lexer.o
	$(CC) $(CFLAGS) -c parser.cpp

instructions.o: instructions.cpp world.hpp
	$(CC) $(CFLAGS) -c instructions.cpp

world.o: world.cpp world.hpp
	$(CC) $(CFLAGS) -c world.cpp

test_lexer: lexer.o test/lexer/getTokTest/test.cpp
	$(CC) $(CFLAGS) $^ -o test/lexer/getTokTest/test.out $(INCLUDE)

test_parser: lexer.o dump.o codegen.o parser.o ast.hpp util.hpp test/parser/test.cpp
	$(CC) $(CFLAGS) $^ -o test/parser/test.out $(INCLUDE)

test_codegen: lexer.o dump.o codegen.o parser.o ast.hpp util.hpp test/codegen/test.cpp
	$(CC) $(CFLAGS) $^ -o test/codegen/test.out $(INCLUDE)

showTokens.out: lexer.o dump.o codegen.o parser.o ast.hpp test/lexer/showTokens.cpp
	$(CC) $(CFLAGS) $^ -o test/lexer/showTokens.out $(INCLUDE)

