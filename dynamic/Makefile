CFLAGS = -g -Wall -Wextra -Wpedantic
COV = --coverage

.PHONY: all memtest_linked_list memtest_iter memtest_hash_table clean coverage_all coverage_linked_list_tests coverage_iter_test coverage_hash_table_testS


hash_table_dynamic.o:		 hash_table.h hash_table_iterator.h common.h


hash_table_dynamic_tests: hash_table_dynamic.o hash_table_tests.o
	gcc $(CFLAGS) $(COV) $^ -o $@ -lcunit

clean:
	rm -f *.o *.gcno *.gcda *.gcov linked_list_tests iter_tests hash_table_tests freq_count a.out gmon.out hash_table_dynamic_tests