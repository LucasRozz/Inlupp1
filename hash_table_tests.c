#include <CUnit/Basic.h>
#include "hash_table.h"

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
  int result = 0;
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, 0);

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, value);

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
  int result = 0;
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, 0);

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, value);
  value = 134;
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, value);


  // destroy hash table
  ioopm_hash_table_destroy(ht);
}

void test_insert_multiple()
{
  // create new hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *keys[]   = {"a", "b", "c", "d", "e"};
  int values[]   = {1, 2, 3, 4, 5};
  int n = 5;

  // insert every key-value pair
  for (int i = 0; i < n; i++)
  {
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }

  // check that every key can be looked up and returns its own value
  int result;
  for (int i = 0; i < n; i++)
  {
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, keys[i], &result));
    CU_ASSERT_EQUAL(result, values[i]);
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
  int result;

  ioopm_hash_table_insert(ht, key1, 10);
  ioopm_hash_table_insert(ht, key2, 20);

  // remove an existing key: should succeed and return its value
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, key1, &result));
  CU_ASSERT_EQUAL(result, 10);

  // it should no longer be found afterwards
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key1, &result));

  // the other key should be untouched
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key2, &result));
  CU_ASSERT_EQUAL(result, 20);

  // removing a key that doesn't exist should just return false
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, "not_a_key", &result));

  ioopm_hash_table_destroy(ht);
}

void test_entry_remove(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
 

  char *keys[]   = {"A*", "B-", "C0", "d", "e"};
  int values[]   = {1, 2, 3, 4, 5};
  int n = 5;

  // insert every key-value pair
  for (int i = 0; i < n; i++)
  {
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }
  int result;
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, keys[1], &result));
  CU_ASSERT_EQUAL(result, 2);
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, keys[1], &result));
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, keys[2], &result));
  CU_ASSERT_EQUAL(result, 3);

  ioopm_hash_table_destroy(ht);
}

void test_has_key_1(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *key= "hej";
  int result;
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));

  ioopm_hash_table_destroy(ht);
}

void test_has_key_2(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *keys[]= {"hej", "då"};
  int values[] = {1, 2};
  int result;
  
  ioopm_hash_table_insert(ht, keys[0], values[0]);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, keys[0], &result));
  CU_ASSERT_EQUAL(result, values[0]);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, keys[1], &result));

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
  int result;

  ioopm_hash_table_insert(ht, key, value);
  ioopm_hash_table_remove(ht, key, &result);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, 67);

  ioopm_hash_table_destroy(ht);
}

void test_has_key_5(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *keys[]= {"hej", "då", "va"};
  int values[] = {1, 2, 3};
  int result;

  ioopm_hash_table_insert(ht, keys[0], values[0]);
  ioopm_hash_table_insert(ht, keys[1], values[1]);
  ioopm_hash_table_insert(ht, keys[2], values[2]);

  ioopm_hash_table_remove(ht, keys[1], &result);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, keys[0], &result));
  CU_ASSERT_EQUAL(result, values[0]);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, keys[2], &result));
  CU_ASSERT_EQUAL(result, values[2]);

  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, keys[1], &result));
  CU_ASSERT_EQUAL(result, values[2]);

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
  for(int i = 0; i < 5; i ++){
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 5);
}

void hash_table_remove_size_test(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  char *keys[]= {"hej", "då", "va"};
  int values[] = {1, 2, 3};
  int result;
  for(int i = 0; i < 4; i ++){
    ioopm_hash_table_insert(ht, keys[i], values[i]);
  }
  ioopm_hash_table_remove(ht, keys[1], &result);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 2);
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
  (CU_add_test(my_test_suite, "test remove", test_remove) == NULL)                        ||
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
