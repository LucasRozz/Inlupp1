CFLAGS = -g -Wall -Wextra -Wpedantic
COV = --coverage

.PHONY: all memtest_linked_list memtest_iter memtest_hash_table clean coverage_all coverage_linked_list_tests coverage_iter_test coverage_hash_table_test

all:linked_list_tests iter_tests hash_table_tests

linked_list.o:               Headers/linked_list.h Headers/list_iterator.h Headers/common.h
hash_table.o:                Headers/hash_table.h Headers/hash_table_iterator.h Headers/common.h
linked_list_tests.o:         Headers/linked_list.h Headers/common.h
linked_list_iterator_test.o: Headers/linked_list.h Headers/list_iterator.h Headers/common.h
hash_table_tests.o:          Headers/hash_table.h Headers/hash_table_iterator.h Headers/common.h

freq_count: freq_count.c hash_table.o
	gcc $(CFLAGS) $^ -o $@ -pg

linked_list_tests: linked_list_tests.o linked_list.o
	gcc $(CFLAGS) $(COV) $^ -o $@ -lcunit

iter_tests: linked_list.o linked_list_iterator_test.o
	gcc $(CFLAGS) $(COV) $^ -o $@ -lcunit

hash_table_tests: hash_table.o hash_table_tests.o
	gcc $(CFLAGS) $(COV) $^ -o $@ -lcunit
	

memtest_linked_list: linked_list_tests
	valgrind --leak-check=full ./linked_list_tests

memtest_iter: iter_tests
	valgrind --leak-check=full ./iter_tests

memtest_hash_table: hash_table_tests
	valgrind --leak-check=full ./hash_table_tests

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

freq_count_profile: hash_table.o freq_count.c
	gcc $(CFLAGS) $^ -o freq_count_profile -pg

all_profiles: profile_1k profile_10k profile_16k profile_small

profile_1k: freq_count_profile
	./freq_count_profile Words/1k-long-words.txt
	gprof ./freq_count_profile gmon.out > Profiles/1k_test.txt

profile_10k: freq_count_profile
	./freq_count_profile Words/10k-words.txt
	gprof ./freq_count_profile gmon.out > Profiles/10k_test.txt
	
profile_16k: freq_count_profile
	./freq_count_profile Words/16k-words.txt
	gprof ./freq_count_profile gmon.out > Profiles/16k_test.txt

profile_small: freq_count_profile
	./freq_count_profile Words/small.txt
	gprof ./freq_count_profile gmon.out > Profiles/small.txt

clean:
	rm -f *.o *.gcno *.gcda *.gcov linked_list_tests iter_tests hash_table_tests freq_count a.out gmon.out freq_count_profile