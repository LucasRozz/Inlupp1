tests: linked_list_tests.c linked_list.c linked_list.h
	gcc -g -Wall -fprofile-arcs -ftest-coverage -Wextra -Wpedantic linked_list.c linked_list_tests.c -o linked_list_tests -lcunit

memtest: linked_list_tests
	valgrind --leak-check=full ./linked_list_tests

itertest: linked_list.c linked_list.h list_iterator.h linked_list_iterator_test.c
	gcc -g -Wall -Wextra -Wpedantic linked_list.c linked_list_iterator_test.c -o iter_test -lcunit

memtest_iter: iter_test
	valgrind --leak-check=full ./iter_test

hash_tests: hash_table.c hash_table.h hash_table_tests.c
	gcc -g -Wall -Wextra -Wpedantic hash_table.c hash_table_tests.c -o hash_test -lcunit

memtest_hash: 
	valgrind --leak-check=full ./hash_test