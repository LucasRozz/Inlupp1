tests: linked_list_tests.c linked_list.c linked_list.h
	gcc -g -Wall -Wextra -Wpedantic linked_list.c linked_list_tests.c -o linked_list_tests -lcunit

memtest: linked_list_tests
	valgrind --leak-check=full ./linked_list_tests