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

# Initial Profiling Results

    For each input, what are the top 3 functions?
    small.txt
        cmp_freq_words
        hash_function
        eq_function
    1k words
        eq_function
        hash_function
        process_word
    10k words
        eq_function
        hash_function
        process_word
    16k words
        eq_function_init
        hash_function
        cmp_freq_words
    
    This data was obtained using th gmon extension, to run it:   
        gprof options "file to run" gmon.out > "outfile"

    For each input, are the top 3 functions in your code (that you have written), or is it in library functions?
        Its only our functions in the top 3 most used.
    
    Are the top 3 functions in your code consistent across the inputs? Why? Why not?
        Functions like hash_function is consistent since every words needs to hash into their respective bucket no matter the frequency of the word. When a new word needs to be put in the table, it gets hashed 3 times and an already existing key gets hashed 2 times. This means that hash will get called more than process_word.
        Eq_function is consistently one of the most called functions, this is since we always need to compare the words when they're processed to compare to current keys in the hash table. This is repeated a lot when we have long linked list to iterate over.
        A function that differs across the inputs is the cmp_freq_words function which gets called more in the txt file with 16k words and the small.txt. In the small file, all words are different which means that when comparing the frequency, we have more words to compare. In the 16k file, we simply have more words to compare.
        Process_word gets called often in 1k and 10k files since every word needs to get processed, since we have words with higher frequency in these files, we dont need to compare the frequency that many times.

    Is there some kind of trend? (Possibly several)
        Textfiles with high frequency words will not call compare_freq_words as often and likewise hash_function will need to hach more if there are more low frequency words. Eq_function gets called the most times in every file except small, this is because the linked lists get quite long and for every comparison eq_function gets called. The small file doesn't have enough words for this to happen.
        
    Do the results correspond with your expectations?
        No, we expected process_words to be prominent and we didn't think eq_function would get called so many times. We realized that hash_function would get called often since we always need to hash the keys but we didn't think about the lengths of the linked lists and the amount of comparisons needed.
    
    Based on these results, do you see a way to make your program go faster?
        Yes, we need to improve the amount of buckets or making the amount dynamic so the linked lists get shorter and we don't need to check for equal as many times.
    