#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

typedef struct hash_table ioopm_hash_table_t;
typedef struct entry entry_t;

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
  entry_t buckets[17];
};

ioopm_hash_table_t *ioopm_hash_table_create()
{
  /// Allocate zeroed-out space for a ioopm_hash_table_t = 17 pointers to entry_t's
  return calloc(1, sizeof(ioopm_hash_table_t));
}

void entry_destroy(entry_t *entry_remove){
  free(entry_remove);
}

// M39 and O44 goals


void iter_entry_remove(entry_t *entry_remove){
  entry_t *current = entry_remove;
  while(current != NULL){
    entry_t *next = current->next;
    entry_destroy(current);
    current = next;
  }
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
  // Todo: stub
  for(int i = 0; i < 17; i++){
    entry_t *entry_to_remove = ht->buckets[i].next;
    iter_entry_remove(entry_to_remove);
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

entry_t *find_previous_entry(ioopm_hash_table_t *ht, char *key) {
    size_t bucket = string_knr_hash(key) % 17;
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
