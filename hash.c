#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#pragma once


/**
* @file hash_table.h
* @author Lowe Lindin, Lucas Stenros
* @date 2026-09-14
* @brief Simple hash table that maps string keys to integer values.
*
* Here typically goes a more extensive explanation of what the header
* defines. Doxygens tags are words preceeded by either a backslash @\
* or by an at symbol @@.
*
*/

typedef struct entry entry_t;

struct entry
{
  char *key;    // holds the key
  int value;    // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

typedef struct hash_table ioopm_hash_table_t;

/// @brief Create a new hash table
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(void);

ioopm_hash_table_t *ioopm_hash_table_create(void) {
    entry_t *buckets[17];
 return NULL;
}

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
    free(ht);
 return;
}

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, char *key, int value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return the value mapped to by key (FIXME: what if the key does not exist?)
int ioopm_hash_table_lookup(ioopm_hash_table_t *ht, char *key);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the value mapped to by key (FIXME: what if the key does not exist?)
int ioopm_hash_table_remove(ioopm_hash_table_t *ht, char *key);





