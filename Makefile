vpath %.c src

notebooc: notebooc.c
	$(CC) ./src/notebooc.c -o ./bin/notebooc -Wall -Wextra -pedantic -std=c99