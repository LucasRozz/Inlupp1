#include <CUnit/Basic.h>
#include "linked_list.h"
#include "list_iterator.h"

int init_suite(void)
{
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void)
{
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

// These are example test functions. You should replace them with
// functions of your own.

void iterator_print_test(void){
    ioopm_list_t *a_list = ioopm_list_create();
    ioopm_list_append(a_list, 1);
    ioopm_list_append(a_list, 2);
    ioopm_list_append(a_list, 3);
    ioopm_list_append(a_list, 4);
    ioopm_list_append(a_list, 5);
    ioopm_list_iterator_t *it;
    for (it = ioopm_list_iterator_create(a_list);
     !ioopm_list_iterator_at_end(it);
     ioopm_list_iterator_advance(it))
{
  printf("%d\n", ioopm_list_iterator_current(it));
}
ioopm_list_iterator_destroy(it);
ioopm_list_destroy(a_list);
}

void simple_iterator_test(void){
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(iter));
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 1);
    ioopm_list_destroy(list);
    ioopm_list_iterator_destroy(iter);
}

void test_iterator_create_on_empty_list(void){
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    // No elements exist yet, so there should be nothing to give
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_single_element(void){
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 42);

    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(iter));
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 42);

    ioopm_list_iterator_advance(iter);
    // Past the only element now
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void test_iterator_full_traversal(void){
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 1);
    ioopm_list_append(list, 2);
    ioopm_list_append(list, 3);
    ioopm_list_append(list, 4);

    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);

    int expected[] = {1, 2, 3, 4};
    for(int i = 0; i < 4; i++){
        CU_ASSERT_FALSE(ioopm_list_iterator_at_end(iter));
        CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), expected[i]);
        ioopm_list_iterator_advance(iter);
    }

    // We've now advanced past the last element
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

// Reimplements ioopm_list_size using only the iterator interface —
// the "crude but working" test style the instructions suggest.
static int count_with_iterator(ioopm_list_t *list){
    int count = 0;
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    while(!ioopm_list_iterator_at_end(iter)){
        count++;
        ioopm_list_iterator_advance(iter);
    }
    ioopm_list_iterator_destroy(iter);
    return count;
}

void test_iterator_reimplements_size(void){
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 10);
    ioopm_list_append(list, 20);
    ioopm_list_append(list, 30);

    CU_ASSERT_EQUAL(count_with_iterator(list), ioopm_list_size(list));
    CU_ASSERT_EQUAL(count_with_iterator(list), 3);

    ioopm_list_destroy(list);
}

// Cross-checks the iterator's traversal order against ioopm_list_get,
// which independently confirms the iterator visits elements in the
// right order and the right number of times.
void test_iterator_matches_get(void){
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 5);
    ioopm_list_append(list, 15);
    ioopm_list_append(list, 25);

    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    int index = 0;
    while(!ioopm_list_iterator_at_end(iter)){
        int expected;
        CU_ASSERT_TRUE(ioopm_list_get(list, index, &expected));
        CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), expected);
        ioopm_list_iterator_advance(iter);
        index++;
    }
    CU_ASSERT_EQUAL(index, ioopm_list_size(list));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

void iterator_remove_test(void){
    ioopm_list_t *list = ioopm_list_create();
    ioopm_list_append(list, 5);
    ioopm_list_append(list, 15);
    ioopm_list_append(list, 25);
    int result;

    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_TRUE(ioopm_list_iterator_remove(iter, &result));
    CU_ASSERT_EQUAL(result, 15);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 25);
    ioopm_list_iterator_advance(iter);
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(iter));

    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}


void iterator_insert_test(void){
    ioopm_list_t *list = ioopm_list_create();
    
    ioopm_list_iterator_t *iter = ioopm_list_iterator_create(list);
    ioopm_list_iterator_insert(iter, 1);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 1);
    ioopm_list_iterator_insert(iter, 2);
    ioopm_list_iterator_insert(iter, 3);

    ioopm_list_iterator_advance(iter);
    CU_ASSERT_EQUAL(ioopm_list_iterator_current(iter), 2);


    ioopm_list_iterator_destroy(iter);
    ioopm_list_destroy(list);
}

int main()
{
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL)
  {
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
      (CU_add_test(my_test_suite, "A simple create and check value", simple_iterator_test) == NULL) ||
      (CU_add_test(my_test_suite, "test printing the list \n", iterator_print_test) == NULL) ||
      (CU_add_test(my_test_suite, " test_iterator_create_on_empty_list",  test_iterator_create_on_empty_list) == NULL) ||
      (CU_add_test(my_test_suite, "test_iterator_single_element", test_iterator_single_element) == NULL) ||
      (CU_add_test(my_test_suite, "test_iterator_full_traversal", test_iterator_full_traversal) == NULL) ||
      (CU_add_test(my_test_suite, "test_iterator_reimplements_size", test_iterator_reimplements_size) == NULL) ||
      (CU_add_test(my_test_suite, "test_iterator_matches_get", test_iterator_matches_get) == NULL) ||
      (CU_add_test(my_test_suite, "iterator_remove_test", iterator_remove_test) == NULL) ||
      (CU_add_test(my_test_suite, "iterator_insert_test", iterator_insert_test) == NULL) ||
      0)
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