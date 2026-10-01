#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "common.h"

// Growth thresholds with load factor 0.75 and the primes table
// 17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381:
//   17 -> 31 at entry 13,   31 -> 67 at entry 24,   67 -> 127 at entry 51, ...
//   8191 -> 16381 at entry 6144
// The next growth (past 16381) would happen at entry 12286, so every
// test here stays well below that (max 5000 entries).

static bool eq_function(elem_t a, elem_t b)
{
  return strcmp(a.s, b.s) == 0;
}

static size_t hash_function(elem_t key)
{
  const char *str = key.s;
  size_t result = 0;
  while (*str != '\0')
  {
    result = result * 31 + ((unsigned char)*str);
    str++;
  }
  return result;
}

// Builds n distinct keys "key0", "key1", ... on the heap.
// The hash table does not own its keys, so free them with free_keys
// AFTER the table has been destroyed.
static char **make_keys(size_t n)
{
  char **keys = calloc(n, sizeof(char *));
  for (size_t i = 0; i < n; i++)
  {
    keys[i] = malloc(32);
    snprintf(keys[i], 32, "key%zu", i);
  }
  return keys;
}

static void free_keys(char **keys, size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    free(keys[i]);
  }
  free(keys);
}

static void insert_keys(ioopm_hash_table_t *ht, char **keys, size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    ioopm_hash_table_insert(ht, string_elem(keys[i]), int_elem((int)i));
  }
}

// Every key must be found, with its own value.
static void assert_all_present(ioopm_hash_table_t *ht, char **keys, size_t n)
{
  elem_t result;
  for (size_t i = 0; i < n; i++)
  {
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(keys[i]), &result));
    CU_ASSERT_EQUAL(result.i, (int)i);
  }
}

// Insert one key at a time across the first growth (entry 13) and check
// after EVERY insert that nothing has been lost. Off-by-one bugs live here.
void test_grow_at_threshold(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function, eq_function);
  size_t n = 15;
  char **keys = make_keys(n);

  for (size_t i = 0; i < n; i++)
  {
    ioopm_hash_table_insert(ht, string_elem(keys[i]), int_elem((int)i));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), i + 1);
    assert_all_present(ht, keys, i + 1);
  }

  ioopm_hash_table_destroy(ht);
  free_keys(keys, n);
}

// Several growths in a row: 17 -> 31 -> 67 -> 127 -> 257.
// Checks everything after each insert, so a growth that loses or
// misplaces an entry is caught right away.
void test_grow_several_times_step_by_step(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function, eq_function);
  size_t n = 150;
  char **keys = make_keys(n);

  for (size_t i = 0; i < n; i++)
  {
    ioopm_hash_table_insert(ht, string_elem(keys[i]), int_elem((int)i));
    assert_all_present(ht, keys, i + 1);
  }
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), n);

  ioopm_hash_table_destroy(ht);
  free_keys(keys, n);
}

// Many entries: grows up to 8191 buckets (but not past the primes table).
void test_grow_many_entries(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function, eq_function);
  size_t n = 5000;
  char **keys = make_keys(n);

  insert_keys(ht, keys, n);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), n);
  assert_all_present(ht, keys, n);
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("not_a_key")));

  ioopm_hash_table_destroy(ht);
  free_keys(keys, n);
}

// Updating existing keys after growth: values change, size does not,
// and no extra growth or duplicate entries.
void test_update_after_grow(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function, eq_function);
  size_t n = 200;
  char **keys = make_keys(n);
  elem_t result;

  insert_keys(ht, keys, n);
  for (size_t i = 0; i < n; i++)
  {
    ioopm_hash_table_insert(ht, string_elem(keys[i]), int_elem((int)i * 10));
  }

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), n);
  for (size_t i = 0; i < n; i++)
  {
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(keys[i]), &result));
    CU_ASSERT_EQUAL(result.i, (int)i * 10);
  }

  ioopm_hash_table_destroy(ht);
  free_keys(keys, n);
}

// Remove everything after growth, then check the table still works.
void test_remove_all_after_grow(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function, eq_function);
  size_t n = 500;
  char **keys = make_keys(n);
  elem_t result;

  insert_keys(ht, keys, n);
  for (size_t i = 0; i < n; i++)
  {
    CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem(keys[i]), &result));
    CU_ASSERT_EQUAL(result.i, (int)i);
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem(keys[i])));
  }
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

  // the grown (now empty) table must still accept new entries
  insert_keys(ht, keys, n);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), n);
  assert_all_present(ht, keys, n);

  ioopm_hash_table_destroy(ht);
  free_keys(keys, n);
}

// Remove every other key while the table is growing.
void test_mixed_insert_remove(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function, eq_function);
  size_t n = 600;
  char **keys = make_keys(n);
  elem_t result;

  for (size_t i = 0; i < n; i++)
  {
    ioopm_hash_table_insert(ht, string_elem(keys[i]), int_elem((int)i));
    if (i % 2 == 1)
    {
      CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem(keys[i - 1]), &result));
    }
  }

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), n / 2);
  for (size_t i = 0; i < n; i++)
  {
    bool should_exist = (i % 2 == 1);
    CU_ASSERT_EQUAL(ioopm_hash_table_has_key(ht, string_elem(keys[i])), should_exist);
  }

  ioopm_hash_table_destroy(ht);
  free_keys(keys, n);
}

// The iterator must visit every entry exactly once after growth.
void test_iterator_after_grow(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function, eq_function);
  size_t n = 300;
  char **keys = make_keys(n);
  int *seen = calloc(n, sizeof(int));

  insert_keys(ht, keys, n);

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  size_t count = 0;
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    int value = ioopm_hash_table_iterator_current_value(it).i;
    CU_ASSERT_TRUE(value >= 0 && value < (int)n);
    if (value >= 0 && value < (int)n)
    {
      seen[value]++;
    }
    count++;
    ioopm_hash_table_iterator_advance(it);
  }
  ioopm_hash_table_iterator_destroy(it);

  CU_ASSERT_EQUAL(count, n);
  for (size_t i = 0; i < n; i++)
  {
    CU_ASSERT_EQUAL(seen[i], 1); // every entry seen exactly once
  }

  free(seen);
  ioopm_hash_table_destroy(ht);
  free_keys(keys, n);
}

// Keys whose hashes are multiples of 17 all land in bucket 0 before
// the first growth. After growth they must spread out and still be found.
// (Single-character keys: hash = the character code.)
void test_colliding_keys_survive_grow(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(hash_function, eq_function);
  // '"' = 34, 'D' = 68, 'U' = 85, 'f' = 102, 'w' = 119: all ≡ 0 (mod 17)
  char *colliding[] = {"\"", "D", "U", "f", "w"};
  size_t n_other = 20;
  char **others = make_keys(n_other);
  elem_t result;

  for (int i = 0; i < 5; i++)
  {
    ioopm_hash_table_insert(ht, string_elem(colliding[i]), int_elem(100 + i));
  }
  insert_keys(ht, others, n_other); // forces at least one growth

  for (int i = 0; i < 5; i++)
  {
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(colliding[i]), &result));
    CU_ASSERT_EQUAL(result.i, 100 + i);
  }
  assert_all_present(ht, others, n_other);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 5 + n_other);

  ioopm_hash_table_destroy(ht);
  free_keys(others, n_other);
}

int main(void)
{
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  CU_pSuite suite = CU_add_suite("Dynamic resizing", NULL, NULL);
  if (suite == NULL)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (
      (CU_add_test(suite, "grow at load factor threshold", test_grow_at_threshold) == NULL) ||
      (CU_add_test(suite, "grow several times, step by step", test_grow_several_times_step_by_step) == NULL) ||
      (CU_add_test(suite, "grow with many entries", test_grow_many_entries) == NULL) ||
      (CU_add_test(suite, "update after grow", test_update_after_grow) == NULL) ||
      (CU_add_test(suite, "remove all after grow", test_remove_all_after_grow) == NULL) ||
      (CU_add_test(suite, "mixed insert/remove while growing", test_mixed_insert_remove) == NULL) ||
      (CU_add_test(suite, "iterator after grow", test_iterator_after_grow) == NULL) ||
      (CU_add_test(suite, "colliding keys survive grow", test_colliding_keys_survive_grow) == NULL) ||
      0)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
