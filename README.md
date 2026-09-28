# Hashtable, linked lists and frequency count

## Running the tests:

    Hash_table_test: 
        To run the tests, you first need to compile the files, you do this by typing "make hash_table_tests". To run the tests and check for memory leaks you need to type "make memtest_hash_table". You dont need to 

    Linked_list_test: 
        To run the tests, you first need to compile the files, you do this by typing "make compile_linked_list". To run the tests and check for memory leaks you need to type "make tests_linked_list".
    
    Linked_list_iterator_test:
        To run the tests, you first need to compile the files, you do this by typing "make compile_iter_test". To run the tests and check for memory leaks you need to type "make tests_iter".


## Running freq_count

    freq_count.c:
        To run the file: gcc -Wall -Wextra freq_count.c hashtable.c -o freq_count.o
        Followed by: ./freq_count.o filename1 filename2 ...
        Note: filename1, filename2 are your own textfiles.
        
    
## Design descicions

    -We have followed the given instructions by representing all variables with the union "elem_t", a union defined in common.h.

    -Neither the linked list nor the hash table takes ownership of the data, this is done through allocating and freeing in separate function calls.

    -Currently, the number of buckets is fixed to 17. This is marked with a //dodge in the code since its a known simplification.

    -We have replaced values which represent indexes or sizes of a linked list and hash table from  int to size_t.

    -When we try to advance the iterator beyond the end of a linked list or hash table, its treated as a programming error and enforced with an assert, the program will not continue working after this.

    -When using ioopm_list_insert, if a bad index is used, it silently doesnt do anything.

    -When inserting a value in a hash table to an already existing key, it does not replace the stored key, instead it replaces the value. This means that no new allocation of memory is done.


## Documentation

    To avoid having set types for keys and values we've implemented the values with unions which increases the generalisation. By letting the user create their own function to check equality and hash function, this further increases the generalisation.

    An example of handling an error is when a new element is inserted in a list using ioopm_list_insert. If a bad index is used, nothing is done and the function doesnt return anything, however, this is currently not reported to the user. In other functions such as ioopm_list_remove and ioopm_list_get, if the index isn't valid, false is returned.


 # Test Coverage

    How to run coverage:
    By running "make coverage_all", all tests will run and the coverage for each file will be presented.

        To check the coverage only the hash table tests you can run "make coverage_hash_table_tests". This will work
        before compiling it beforehand since make recognizes its dependencies isnt created, so it will compile.

        To check coverage for the linked list iterator tests, you need to run "make coverage_iter_tests"

        To check coverage for the hash table tests, you need to run "make coverage_hash_table_tests"
    
    Coverage per .c file:

        hash_table_tests.c:
        Lines executed: 100.00% of 97
        Branches executed: 100.00% of 24

        linked_list_test.c
        Lines executed:70.67% of 150
        Branches executed:73.68% of 38

        linked_list_iterator_test.c
        Lines executed: 72.00% of 150
        Branches executed: 84.21% of 38
        
        To obtain coverage from the tests we used gcov.




    
