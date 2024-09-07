CC=gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
CHECK:=$(shell pkg-config --cflags --libs check)
Sanitize = -fsanitize=address -g

SRC=$(wildcard s21_*.c)

# OBJ=$(wildcard s21_*.o)

OBJ=$(patsubst %.c,%.o, ${SRC})

FILE_TEST=$(wildcard unit_test/s21_*.c)

all: clean s21_matrix.a

s21_matrix.a: $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	ar rc s21_matrix.a $(OBJ)
	ranlib s21_matrix.a

test: clean s21_matrix.a 
	$(CC) -o test $(CFLAGS) -g --coverage $(FILE_TEST) $(SRC) -lcheck  -lpthread -lm  -I /usr/local/include -L/usr/local/lib

gcov_report: test
	$(CC) --coverage $(CFLAGS) -lcheck -lm -lpthread $(FILE_TEST) $(SRC) -o test $(CHECK) -L. s21_matrix.a
	./test
	lcov -t "test" -o tests.info -c -d .
	genhtml -o coverage tests.info
	open coverage/index.html

clean:
	rm -f *.o *.gcda *.a *.gcno *.info
	rm -rf coverage test.dSYM
	rm -f test *.out

clang:
	clang-format --style=Google -i *.c *.h

.PHONY: all, clean, rebuild, gcov_report, s21_matrix.a
