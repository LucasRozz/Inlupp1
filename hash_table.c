#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "hash_table.h"
#include "hash_table_iterator.h"
#define No_Buckets 17

struct entry
{
  char *key;     // holds the key
  int value;     // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

//Check for achievements (M39) and (O44) for using double pointers

struct hash_table
{
  // DODGE: hard-coding number of buckets as 17.
  // NOTE: addressing this dodge is optional.
  entry_t buckets[No_Buckets];
  int size;
};

struct hash_table_iterator
{
  ioopm_hash_table_t *ht;
  int current_bucket;
  entry_t *current_entry;
};

ioopm_hash_table_t *ioopm_hash_table_create()
{
  /// Allocate zeroed-out space for a ioopm_hash_table_t = 17 pointers to entry_t's
  return calloc(1, sizeof(ioopm_hash_table_t));
}

static void entry_destroy(entry_t *entry_remove){
  free(entry_remove);
}

// M39 and O44 goal
static void iter_remove_all_entry(entry_t *entry_to_remove){
  entry_t *current = entry_to_remove;
  while(current != NULL){
    entry_t *next = current->next;
    entry_destroy(current);
    current = next;
  }
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
  // Todo: stub
  for(int i = 0; i < No_Buckets; i++){
    entry_t *entry_to_remove = ht->buckets[i].next;
    iter_remove_all_entry(entry_to_remove);
    }
    free (ht);
 return;
}

// used as an abstraction of the program(mål A!)
static entry_t *entry_create(char *key, int value, entry_t *next) {  
  entry_t *new = calloc(sizeof(entry_t), 1);
  new->key = key;
  new->value = value;
  new->next = next;
  return new;
}

static size_t string_knr_hash(const char *str)
{
  size_t result = 0;
  while (*str != '\0')
  {
    result = result * 31 + ((unsigned char) *str);
    str++;
  }
  return result;
}
static entry_t *find_previous_entry(ioopm_hash_table_t *ht, char *key) {
    size_t bucket = string_knr_hash(key) % No_Buckets;
    entry_t *previous = &ht->buckets[bucket];
    while(previous->next != NULL && strcmp(previous->next->key, key) != 0){
      previous = previous->next;
    }
  return previous;
}


void ioopm_hash_table_insert(ioopm_hash_table_t *ht, char *key, int value)
{
  // find previous entry, or the last entry if the key does not exist
  entry_t *previous = find_previous_entry(ht, key);

  // if the key exists, update the value, otherwise create a new entry
  if (previous->next != NULL)
  {
    previous->next->value = value;
  }
  else
  {
    previous->next = entry_create(key, value, NULL);
    ht->size += 1;
  }
}


bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, char *key, int *result)
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

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, char *key, int *result){
    entry_t *previous = find_previous_entry(ht, key);
    entry_t *target = previous->next;
    if(target == NULL){
      return false;
    }
    else{
    *result = target->value;
    previous->next = target->next;
    entry_destroy(target);
    ht->size -= 1;
    return true;
    }
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, char *key){
  int result;
  return ioopm_hash_table_lookup(ht, key, &result);
}

int ioopm_hash_table_size(ioopm_hash_table_t *ht){
  return ht->size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht){
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
    advance_iterator_state(it); // Cheat!
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

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it) {
  free(it);
}

bool ioopm_hash_table_iterator_at_end(ioopm_hash_table_iterator_t *it){
  //STUBB
  return it->current_bucket == No_Buckets;
}


void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it){
  //stubb
  assert(!ioopm_hash_table_iterator_at_end(it) && "iterator at end when advancing");
  advance_iterator_state(it);
  skip_sentinel_nodes(it);
}

char *ioopm_hash_table_iterator_current_key(ioopm_hash_table_iterator_t *it){
  //STUBB
  return it->current_entry->key;
}

int ioopm_hash_table_iterator_current_value(ioopm_hash_table_iterator_t *it){
  //stubb
  return it->current_entry->value;
}
