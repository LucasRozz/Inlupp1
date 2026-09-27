### Hashtable and linked lists

# Running the tests:

    Hash_table_test: 
        To run the tests, you first need to compile the files, you do this by typing "make compile_hash_test". To run the tests and check for memory leaks you need to type "make tests_hash_table".

    Linked_list_test: 
        To run the tests, you first need to compile the files, you do this by typing "make compile_linked_list". To run the tests and check for memory leaks you need to type "make tests_linked_list".
    
    Linked_list_iterator_test:
        To run the tests, you first need to compile the files, you do this by typing "make compile_iter_test". To run the tests and check for memory leaks you need to type "make tests_iter".
    

# Design descicions
We have followed the given instructions by representing all variables with the union "elem_t", a union defined in common.h.
Neither the linked list nor the hash table takes ownership of the data, this is done through allocating and freeing in separate function calls.
Currently, the number of buckets is fixed to 17. This is marked with a //dodge in the code since its a known simplification.
We have replaced values which represent indexes or sizes of a linked list and hash table from int to size_t.
When we try to advance the iterator beyond the end of a linked list, its treated as a programming error and enforced with an assert, the program will not continue working after this.

# Running code

    freq_count.c:
        To run the file: gcc -Wall -Wextra freq_count.c -o freq_count.o
        Followed by: ./freq_count.o
        Description: freq_count.c takes as argument a textfile 
    
