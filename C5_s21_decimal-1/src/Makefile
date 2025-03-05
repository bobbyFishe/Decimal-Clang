FLAGS=-Wall -Wextra -Werror -std=c11 

SOURCES= ${wildcard *.c}
OBJECTS= ${patsubst %.c, %.o, $(SOURCES)}

all: s21_decimal

push:
	@git add .
	@git commit -m "psuhed make"
	@git push origin develop

python:
	@cd tests && python3 4tests.py

s21_decimal: ${OBJECTS}
	@ar rcs s21_decimal.a ${OBJECTS}
	@ranlib s21_decimal.a
	@rm -r *.o

checkmk:
	@checkmk test.check > test.c
	@checkmk clean_mode=1 test.check > test.c

build_tst:s21_decimal
	@gcc ${FLAGS} -o test test.c tests/for_test.c s21_decimal.a -lcheck -lsubunit -lrt -lpthread -lm

test: checkmk build_tst python
	@./test

leaks: checkmk build_tst python
	@valgrind --leak-check=full --show-leak-kinds=all ./test


gcov_report: s21_decimal checkmk build_tst python
	@NEW_SOURCES="$$(ls *.c)"; \
	gcc --coverage $$NEW_SOURCES tests/for_test.c s21_decimal.a -L. -o test -lcheck -lsubunit -lrt -lpthread -lm
	@./test
	@rm -f test-for_test* test-test*
	@lcov --directory . --capture -o coverage.info --rc lcov_branch_coverage=1
	@genhtml --output-directory report --legend --branch-coverage coverage.info
	@open report/index.html
	@rm -rf *.gcda *.gcno test test.c coverage.info


clean:
	@rm -rf coverage.info *.gcda *.gcno *.o s21_decimal.a test report/ test.c tests/new.txt tests/sub,add,div*

style:
	@clang-format -style=Google -i *.c *.h
