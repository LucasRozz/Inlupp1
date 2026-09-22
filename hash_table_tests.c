#include <CUnit/Basic.h>
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "common.h"



int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

// These are example test functions. You should replace them with
// functions of your own.
void test1(void) {
  CU_ASSERT(42);
}

void test2(void) {
  CU_ASSERT_EQUAL(1 + 1, 2);
}

void test_create_destroy()
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create();
   CU_ASSERT_PTR_NOT_NULL(ht);
   ioopm_hash_table_destroy(ht);
}

void test_insert_once()
{
  // create new hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *key = "abc";
  int value = 123;

  // check that key is not in ht
  elem_t result = int_elem(0);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 0);

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, value);

  // destroy hash table
  ioopm_hash_table_destroy(ht);
}

void test_update_key()
{
  // create new hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *key = "abcd";
  int value = 1212;

  // check that key is not in ht
  elem_t result = int_elem(0);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 0);

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, value);
  value = 134;
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, value);


  // destroy hash table
  ioopm_hash_table_destroy(ht);
}

void test_insert_multiple()
{
  // create new hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *keys[]   = {"a", "b", "c", "d", "e"};
  int values[]   = {1, 2, 3, 4, 5};

  // insert every key-value pair
  for (size_t i = 0; i < 5; i++)
  {
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }

  // check that every key can be looked up and returns its own value
  elem_t result;
  for (size_t i = 0; i < 5; i++)
  {
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, keys[i], &result));
    CU_ASSERT_EQUAL(result.i, values[i]);
  }

  // a key that was never inserted should not be found
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, "fel", &result));

  // destroy hash table
  ioopm_hash_table_destroy(ht);
}

void test_remove()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *key1 = "x";
  char *key2 = "y";
  elem_t result;

  ioopm_hash_table_insert(ht, key1, 10);
  ioopm_hash_table_insert(ht, key2, 20);

  // remove an existing key: should succeed and return its value
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key1, &result));
  CU_ASSERT_EQUAL(result.i, 10);

  // it should no longer be found afterwards
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key1, &result));

  // the other key should be untouched
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result.i, 20);

  // removing a key that doesn't exist should just return false
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, "not_a_key", &result));

  ioopm_hash_table_destroy(ht);
}

void test_entry_remove(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *keys[]   = {"A*", "B-", "C0", "d", "e"};
  int values[]   = {1, 2, 3, 4, 5};

  // insert every key-value pair
  for (size_t i = 0; i < 5; i++)
  {
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }
  elem_t result;
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, keys[1], &result));
  CU_ASSERT_EQUAL(result.i, 2);
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, keys[1], &result));
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, keys[2], &result));
  CU_ASSERT_EQUAL(result.i, 3);

  ioopm_hash_table_destroy(ht);
}

void test_has_key_1(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *key= "hej";
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, key));

  ioopm_hash_table_destroy(ht);
}

void test_has_key_2(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *keys[]= {"hej", "då"};
  int values[] = {1, 2};
  
  ioopm_hash_table_insert(ht, keys[0], values[0]);
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, keys[0]));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, keys[1]));

  ioopm_hash_table_destroy(ht);
}



void test_has_key_3(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *keys[]= {"hej", "då", "va", "Buh"};
  int values[] = {1, 2, 3, 4};
  ioopm_hash_table_insert(ht, keys[0], values[0]);
  ioopm_hash_table_insert(ht, keys[1], values[1]);
  ioopm_hash_table_insert(ht, keys[2], values[2]);
  
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, keys[0]));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, keys[1]));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, keys[2]));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, keys[3]));

  ioopm_hash_table_destroy(ht);
}

void test_has_key_4(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *key = "hej";
  int value = 67;
  elem_t result;

  ioopm_hash_table_insert(ht, key, value);
  ioopm_hash_table_remove(ht, key, &result);
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, key));
  CU_ASSERT_EQUAL(result.i, 67);

  ioopm_hash_table_destroy(ht);
}

void test_has_key_5(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *keys[]= {"hej", "då", "va"};
  int values[] = {1, 2, 3};
  elem_t result;

  ioopm_hash_table_insert(ht, keys[0], values[0]);
  ioopm_hash_table_insert(ht, keys[1], values[1]);
  ioopm_hash_table_insert(ht, keys[2], values[2]);

  ioopm_hash_table_remove(ht, keys[1], &result);
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, keys[0]));

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, keys[2]));

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, keys[1]));

  ioopm_hash_table_destroy(ht);
}

void hash_table_empty_size_test(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
}

void hash_table_single_size_test(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *key = "hej";
  int value = 1;
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
}

void hash_table_multi_size_test(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *keys[]= {"hej", "då", "va", "buh", "apa"};
  int values[] = {1, 2, 3, 4, 5};
  for(size_t i = 0; i < 5; i++){
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 5);
}

void hash_table_remove_size_test(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *keys[]= {"hej", "då", "va"};
  int values[] = {1, 2, 3};
  elem_t result;
  for(size_t i = 0; i < 3; i++){
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }
  ioopm_hash_table_remove(ht, keys[1], &result);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 2);
}

void hash_table_remove_to_empty_test(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *keys[]= {"hej", "då", "va"};
  int values[] = {1, 2, 3};
  elem_t result;
  for(size_t i = 0; i < 3; i++){
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }
  ioopm_hash_table_remove(ht, keys[0], &result);
  ioopm_hash_table_remove(ht, keys[1], &result);
  ioopm_hash_table_remove(ht, keys[2], &result);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
    
  ioopm_hash_table_destroy(ht);
}

void test_iterator_empty_table(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  
  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_destroy(ht);
  ioopm_hash_table_iterator_destroy(it);
}

void test_iterator_single_table(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *key = "hej";
  int value = 1;
  ioopm_hash_table_insert(ht, key, value);
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it).i, 1);
  CU_ASSERT_STRING_EQUAL(ioopm_hash_table_iterator_current_key(it), key);

  ioopm_hash_table_iterator_advance(it);
  
  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_destroy(ht);
  ioopm_hash_table_iterator_destroy(it);  
}

void test_iterator_several_entries()
{
  char *keys[3] = {"abc", "qwe", "asd"};
  int values[3] = {0, 1, 2};

  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  for (size_t i = 0; i != 3; ++i)
  {
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }

  int iteration_count = 0;

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    iteration_count++;
    ioopm_hash_table_iterator_advance(it);
  }
  ioopm_hash_table_destroy(ht);
  ioopm_hash_table_iterator_destroy(it);
  CU_ASSERT_EQUAL(iteration_count, 3);
}


int main() {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      // If the test suite could not be added, tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if (
  (CU_add_test(my_test_suite, "creation and destroy test", test_create_destroy) == NULL)  ||
  (CU_add_test(my_test_suite, "test insert once", test_insert_once) == NULL)              ||
  (CU_add_test(my_test_suite, "test insert two times", test_update_key) == NULL)          ||
  (CU_add_test(my_test_suite, "test insert multiple keys", test_insert_multiple) == NULL) ||
  (CU_add_test(my_test_suite, "test entry remove", test_entry_remove) == NULL)                        ||
  (CU_add_test(my_test_suite, "test remove 1", test_has_key_1) == NULL)                        ||
  (CU_add_test(my_test_suite, "test remove 2", test_has_key_2) == NULL)                        ||
  (CU_add_test(my_test_suite, "test remove 3", test_has_key_3) == NULL)                        ||
  (CU_add_test(my_test_suite, "test remove 4", test_has_key_4) == NULL)                        ||
  (CU_add_test(my_test_suite, "test remove 5", test_has_key_5) == NULL)                        ||
  (CU_add_test(my_test_suite, "hash_table_remove_to_empty_test", hash_table_remove_to_empty_test) == NULL)                        ||
  (CU_add_test(my_test_suite, "ITERATOR TEST EMPTY", test_iterator_empty_table) == NULL)                        ||
  (CU_add_test(my_test_suite, "ITERATOR TEST SINGLE", test_iterator_single_table) == NULL)                        ||
  (CU_add_test(my_test_suite, "ITERATOR TEST SEVERAL", test_iterator_several_entries) == NULL)                        ||
  0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
    }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_NORMAL);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}
