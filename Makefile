CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic -Wno-gnu-binary-literal -fprofile-arcs -ftest-coverage
LIBS = s21_decimal.a
TESTS = check_s21_decimal
C_FILES := $(wildcard *.c)
OBJ_FILES := $(C_FILES:.c=.o)

ifeq ($(OS), Darwin)
	TEST_LIBS = -lcheck -lrt -pthread -lsubunit -L/usr/lib/x86_64-linux-gnu/libsubunit.so -L/usr/lib/x86_64-linux-gnu/libcheck.a -lm
else
	TEST_LIBS = -lcheck -L /usr/bin/
endif



all: clean $(LIBS) gcov_report

clean:
	rm -rf  *.o *.gcda *.gcno *.gcov *.a *.info coverage_report check_s21_decimal a.out

test: $(TESTS) 
	./$(TESTS) 
s21_decimal.a: $(OBJ_FILES)
		ar rcs $@ $< $(OBJ_FILES)

s21_decimal.o: $(C_FILES) s21_decimal.h
	$(CC) $(CFLAGS) $(C_FILES) -fprofile-arcs -ftest-coverage -c $<

check_s21_decimal.o: check_s21_decimal_maks.c s21_decimal.h
	$(CC) $(CFLAGS)  -c $<

check_s21_decimal: check_s21_decimal_maks.o s21_decimal.a
	$(CC) $(CFLAGS)  -o $@ $^ $(TEST_LIBS)


gcov_report: CFLAGS += -fprofile-arcs -ftest-coverage
gcov_report: test
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report
	rm -f *.gcda *.gcno *.gcov
	open ./coverage_report/index.html