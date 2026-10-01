CFLAGS = -g -Wall -Wextra -Wpedantic
COV = --coverage

.PHONY: all memtest_linked_list memtest_iter memtest_hash_table clean coverage_all coverage_linked_list_tests coverage_iter_test coverage_hash_table_dynamic_testS


hash_table_dynamic.o:		 hash_table.h hash_table_iterator.h common.h


hash_table_dynamic_tests: hash_table_dynamic.o hash_table_dynamic_tests.c
	gcc $(CFLAGS) $(COV) --coverage $^ -o $@ -lcunit

coverage: hash_table_dynamic_tests
	./hash_table_dynamic_tests
	gcov -b ./hash_table_dynamic_tests-hash_table_dynamic_tests.gcno

memtest: hash_table_dynamic_tests
	valgrind --leak-check=full ./hash_table_dynamic_tests

freq_count_dynamic_profile: hash_table_dynamic.o freq_count.c
	gcc $(CFLAGS) $^ -o freq_count_dynamic_profile -pg

all_profiles: profile_1k profile_10k profile_16k profile_small

profile_1k: freq_count_dynamic_profile
	./freq_count_dynamic_profile Words/1k-long-words.txt
	gprof ./freq_count_dynamic_profile gmon.out > Profiles/1k_dynamic_test.txt

profile_10k: freq_count_dynamic_profile
	./freq_count_dynamic_profile Words/10k-words.txt
	gprof ./freq_count_dynamic_profile gmon.out > Profiles/10k_dynamic_test.txt
	
profile_16k: freq_count_dynamic_profile
	./freq_count_dynamic_profile Words/16k-words.txt
	gprof ./freq_count_dynamic_profile gmon.out > Profiles/16k_dynamic_test.txt

profile_small: freq_count_dynamic_profile
	./freq_count_dynamic_profile Words/small.txt
	gprof ./freq_count_dynamic_profile gmon.out > Profiles/small_dynamic_test.txt

clean:
	rm -f *.o *.gcno *.gcda *.gcov *.txt linked_list_tests iter_tests hash_table_dynamic_tests freq_count a.out gmon.out hash_table_dynamic_tests freq_count_dynamic_profile