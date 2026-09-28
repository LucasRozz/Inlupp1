CFLAGS = -g -Wall -Wextra -Wpedantic
COV = --coverage

.PHONY: all memtest_linked_list memtest_iter memtest_hash_table

all:linked_list_tests iter_tests hash_table_tests

linked_list_tests: linked_list_tests.c linked_list.c linked_list.h common.h
	gcc $(CFLAGS) $(COV) linked_list.c linked_list_tests.c -o $@ -lcunit

iter_tests: linked_list.c linked_list.h list_iterator.h linked_list_iterator_test.c
	gcc $(CFLAGS) $(COV) linked_list.c linked_list_iterator_test.c -o $@ -lcunit

hash_table_tests: hash_table.c hash_table.h hash_table_tests.c common.h hash_table_iterator.h 
	gcc $(CFLAGS) $(COV) hash_table.c hash_table_tests.c -o $@ -lcunit

memtest_linked_list: linked_list_tests
	valgrind --leak-check=full ./linked_list_tests

memtest_iter: iter_tests
	valgrind --leak-check=full ./iter_tests

memtest_hash_table: hash_table_tests
	valgrind --leak-check=full ./hash_table_tests

.PHONY: clean coverage_all coverage_linked_list_tests coverage_iter_test coverage_hash_table_test

coverage_all: coverage_linked_list_tests coverage_iter_tests coverage_hash_table_tests

coverage_linked_list_tests: linked_list_tests
	./linked_list_tests 
	gcov -b linked_list_tests-linked_list.gcno

coverage_iter_tests: iter_tests
	./iter_tests
	gcov -b iter_tests-linked_list.gcno

coverage_hash_table_tests: hash_table_tests
	./hash_table_tests
	gcov -b hash_table_tests-hash_table.gcno

clean:
	rm -f *.o *.gcno *.gcda *.gcov linked_list_tests iter_tests hash_table_tests freq_count a.out