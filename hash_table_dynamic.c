#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "hash_table.h"
#include "hash_table_iterator.h"
#include "common.h"

#define No_Buckets 17
struct entry
{
  elem_t key;    // holds the key
  elem_t value;  // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

// Check for achievements (M39) and (O44) for using double pointers
struct hash_table
{
  size_t size;
  size_t capacity;
  ioopm_hash_function *hash_fn;
  ioopm_eq_function *key_eq_fn;
  entry_t **buckets;
  double load;
};

size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381};

struct hash_table_iterator
{
  ioopm_hash_table_t *ht;
  size_t current_bucket;
  entry_t *current_entry;
};

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn)
{
  ioopm_hash_table_t *new = calloc(1, sizeof(ioopm_hash_table_size));
  new->hash_fn = hash_fn;
  new->key_eq_fn = key_eq_fn;
  return new;
}


static void entry_destroy(entry_t *entry_remove)
{
  free(entry_remove);
}

static void iter_remove_all_entry(entry_t *entry_to_remove)
{
  entry_t *current = entry_to_remove;
  while (current != NULL)
  {
    entry_t *next = current->next;
    entry_destroy(current);
    current = next;
  }
}

/*static void recursive_remove_all_entry(entry_t *entry_to_remove)
{
  if (entry_to_remove == NULL)
  {
    return;
  }
  else
  {
    entry_t *next = entry_to_remove->next;
    entry_destroy(entry_to_remove);
    recursive_remove_all_entry(next);
  }
}
*/

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  for (size_t i = 0; i < No_Buckets; i++)
  {
    entry_t *entry_to_remove = ht->(buckets[i]).next;     //tror de är sentinel nod grej med .next
    iter_remove_all_entry(entry_to_remove);
  }
  free(ht);
  return;
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *new = calloc(sizeof(entry_t), 1);
  new->key = key;
  new->value = value;
  new->next = next;
  return new;
}

/*static size_t string_knr_hash(const char *str)
{
  size_t result = 0;
  while (*str != '\0')
  {
    result = result * 31 + ((unsigned char)*str);
    str++;
  }
  return result;
}*/

static size_t hash_function(ioopm_hash_table_t *ht, elem_t key)
{
  return ht->hash_fn(key);
}

/*static entry_t *find_previous_entry(ioopm_hash_table_t *ht, elem_t key)
{
  size_t bucket = hash_function(ht, key) % No_Buckets;
  entry_t *previous = &ht->buckets[bucket];
  while (previous->next != NULL && !ht->key_eq_fn(previous->next->key, key))
  {
    previous = previous->next;
  }
  return previous;
}*/

static entry_t **find_previous_entry(ioopm_hash_table_t *ht, elem_t key){
  size_t bucket = ht->hash_fn(key) % ht->capacity;
  entry_t **previous = &ht->buckets[bucket];
  if(*previous != NULL && !ht->key_eq_fn((*previous)->next->key, key)){
    previous = &(*previous)->next;
  }
  return previous;
}

void bigger_hashtable(ioopm_hash_table_t *ht)
{
  

  //skala upp och rehasha
}


void smaller_hashtable(ioopm_hash_table_t *ht)
{
  //stubb

  //skala ner och rehasha
}


void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value_2_insert)
{
  entry_t **previous = find_previous_entry(ht, key);

  if ((*previous) != NULL)  
  {
    (*previous)->value = value_2_insert;
    return;
  }
    if((double)ht->size + 1 / ht->capacity > ht->load)
    {
      bigger_hashtable(ht);
      previous = find_previous_entry(ht, key);
    }
    *previous = entry_create(key, value_2_insert, NULL);
    ht->size ++;
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
  // look for an entry with the key we want
  entry_t *previous = find_previous_entry(ht, key);
  // if the key exists, return the value, otherwise, indicate that the lookup failed
  if (previous->next != NULL)
  {
    *result = previous->next->value;
    return true;
  }
  else
  {
    return false;
  }
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
  entry_t **previous = find_previous_entry(ht, key);
  entry_t *target = (*previous)->next;
  if (target == NULL)
  {
    return false;
  }
  else
  {
    *result = target->value;
    (*previous)->next = target->next;
    entry_destroy(target);
    ht->size -= 1;
    return true;
  }
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  elem_t result;
  return ioopm_hash_table_lookup(ht, key, &result);
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  return ht->size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return ioopm_hash_table_size(ht) > 0 ? false : true;
}







static void advance_iterator_state(ioopm_hash_table_iterator_t *it)
{
  // advance to the next entry in the bucket
  it->current_entry = it->current_entry->next;
  // if it was null advance to the next bucket
  if (it->current_entry == NULL)
  {
    it->current_bucket += 1;
    // if the next bucket existed, update the current entry
    if (it->current_bucket != No_Buckets)
    {
      it->current_entry = &it->ht->buckets[it->current_bucket];
    }
  }
}

static void skip_sentinel_nodes(ioopm_hash_table_iterator_t *it)
{
  while (it->current_bucket != No_Buckets &&
         it->current_entry == &it->ht->buckets[it->current_bucket])
  {
    advance_iterator_state(it);
  }
}

ioopm_hash_table_iterator_t *ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_iterator_t *it = malloc(sizeof(ioopm_hash_table_iterator_t));
  it->ht = ht;
  it->current_bucket = 0;
  it->current_entry = &ht->buckets[0];
  skip_sentinel_nodes(it);
  return it;
}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it)
{
  free(it);
}

bool ioopm_hash_table_iterator_at_end(ioopm_hash_table_iterator_t *it)
{
  return it->current_bucket == No_Buckets;
}

void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it)
{
  assert(!ioopm_hash_table_iterator_at_end(it) && "iterator at end when advancing");
  advance_iterator_state(it);
  skip_sentinel_nodes(it);
}

elem_t ioopm_hash_table_iterator_current_key(ioopm_hash_table_iterator_t *it)
{
  return it->current_entry->key;
}

elem_t ioopm_hash_table_iterator_current_value(ioopm_hash_table_iterator_t *it)
{
  return it->current_entry->value;
}
