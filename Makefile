CFLAGS = -g -Wall -Wextra -Wpedantic
COV = --coverage

.PHONY: all memtest_linked_list memtest_iter memtest_hash_table clean coverage_all coverage_linked_list_tests coverage_iter_test coverage_hash_table_testS

all:linked_list_tests iter_tests hash_table_tests

linked_list.o:               linked_list.h list_iterator.h common.h
hash_table.o:                hash_table.h hash_table_iterator.h common.h
linked_list_tests.o:         linked_list.h common.h
linked_list_iterator_test.o: linked_list.h list_iterator.h common.h
hash_table_tests.o:          hash_table.h hash_table_iterator.h common.h
hash_table_dynamic.o:		 hash_table.h hash_table_iterator.h common.h

freq_count: freq_count.c hash_table.o
	gcc $(CFLAGS) $^ -o $@ -pg

linked_list_tests: linked_list_tests.o linked_list.o
	gcc $(CFLAGS) $(COV) $^ -o $@ -lcunit

iter_tests: linked_list.o linked_list_iterator_test.o
	gcc $(CFLAGS) $(COV) $^ -o $@ -lcunit

hash_table_tests: hash_table.o hash_table_tests.o
	gcc $(CFLAGS) $(COV) $^ -o $@ -lcunit

hash_table_dynamic_tests: hash_table_dynamic.o hash_table_tests.o
	gcc $(CFLAGS) $(COV) $^ -o $@ -lcunit

memtest_linked_list: linked_list_tests
	valgrind --leak-check=full ./linked_list_tests

memtest_iter: iter_tests
	valgrind --leak-check=full ./iter_tests

memtest_hash_table: hash_table_tests
	valgrind --leak-check=full ./hash_table_tests

.PHONY: 

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
	rm -f *.o *.gcno *.gcda *.gcov linked_list_tests iter_tests hash_table_tests freq_count a.out gmon.out