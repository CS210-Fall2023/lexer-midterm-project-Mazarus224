CC = gcc
FLAGS = -Wall -g
OFLAGS = -Wall -g -c

Lexer: main.o lexer.o
	$(CC) $(FLAGS) $< lexer.o -o $@

lexer.o: lexer.c Lex.h
	$(CC) $(OFLAGS) $<

main.o: main.c Lex.h
	$(CC) $(OFLAGS) $<  