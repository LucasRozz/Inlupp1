#include <CUnit/Basic.h>
#include "linked_list.h"

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
void test_create_list(void){
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_destroy(list);
}

void test_add_one_node(void){
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_append(list, 5);
  ioopm_list_destroy(list);
}

void test_add_and_get_head(void){
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_append(list, 1);
  ioopm_list_append(list, 2);
  ioopm_list_append(list, 3);
  CU_ASSERT_EQUAL(ioopm_list_head(list), 1);
  ioopm_list_destroy(list);
}


void test_head_and_last(void){
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_append(list, 5);
  ioopm_list_append(list, 60);
  ioopm_list_append(list, 34);
  CU_ASSERT_EQUAL(ioopm_list_head(list), 5);
  CU_ASSERT_EQUAL(ioopm_list_last(list), 34);
  ioopm_list_destroy(list);
}

void test_insert(void){
  ioopm_list_t *list = ioopm_list_create();
  ioopm_list_append(list, 3);
  ioopm_list_append(list, 4);
  ioopm_list_prepend(list, 2);
  ioopm_list_insert(list, 0, 1);
  ioopm_list_insert(list, 4, 5);
  CU_ASSERT_EQUAL(ioopm_list_head(list), 1);
  CU_ASSERT_EQUAL(ioopm_list_last(list), 5);
  ioopm_list_destroy(list);
}

void test_remove(void){
    ioopm_list_t *list = ioopm_list_create();
    int result = 0;
    ioopm_list_insert(list, 0, 5);
    ioopm_list_insert(list, 0, 4);
    ioopm_list_insert(list, 0, 3);
    ioopm_list_insert(list, 0, 2);
    ioopm_list_insert(list, 0, 1);

    CU_ASSERT_TRUE(ioopm_list_remove(list, 0, &result));
    CU_ASSERT_EQUAL(result, 1);
    CU_ASSERT_TRUE(ioopm_list_remove(list, 3, &result));
    CU_ASSERT_EQUAL(result, 5);
    ioopm_list_destroy(list);

}

void test_get(void){
  ioopm_list_t *list = ioopm_list_create();
    int result = 0;
    ioopm_list_insert(list, 0, 5);
    ioopm_list_insert(list, 1, 4);
    ioopm_list_insert(list, 2, 3);
    ioopm_list_insert(list, 3, 2);
    ioopm_list_insert(list, 4, 1);
    CU_ASSERT_TRUE(ioopm_list_get(list, 2, &result));
    CU_ASSERT_EQUAL(result, 3);
    ioopm_list_destroy(list);
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
    (CU_add_test(my_test_suite, "A simple create and destroy test", test_create_list) == NULL) ||
    (CU_add_test(my_test_suite, "test for adding a node and checking size", test_add_one_node) == NULL) ||
    (CU_add_test(my_test_suite, "test for adding and getting head from list", test_add_and_get_head) == NULL) ||
    (CU_add_test(my_test_suite, "test for getting head and last from list", test_add_and_get_head) == NULL) ||
    (CU_add_test(my_test_suite, "test insert head and last", test_insert) == NULL) ||
    (CU_add_test(my_test_suite, "test remove", test_remove) == NULL) ||
    (CU_add_test(my_test_suite, "test get", test_get) == NULL) ||
    0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
    }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
} 