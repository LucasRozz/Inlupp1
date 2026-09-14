#include <stdio.h>
#include <stdlib.h>

/// Define a data structure for holding a key and a value
struct entry
{
  char *key; /// A string
  int value;
};

/// make entry_t an alias for struct entry
typedef struct entry entry_t;

/// make map_t an alias for an array of 10 entry_t's
typedef entry_t map_t[10];

/// The starting point of any C program
int main(void)
{
  /// Create an array of 10 elements, of which we are going to use 3
  map_t map;
  int map_size = 3;

  /// Populate array just like in the Python example
  map[0] = (entry_t) { .key = "answers", .value = 42 };
  map[1] = (entry_t) { .key = "perfumes", .value = 4711 };
  map[2] = (entry_t) { .key = "primes_under_10", .value = 4 };

  /// Print the contents of the map
  puts("== Dump the map ==");
  for (int i = 0; i < map_size; ++i) /// ++i means i = i + 1
    {
      printf("key: %s => value: %d\n", map[i].key, map[i].value);
    }

  /// Print the keys
  puts("\n== Dump the keys ==");
  for (int i = 0; i < map_size; ++i)
    {
      printf("key: %s\n", map[i].key);
    }

  /// Delete the entry with index 0
  puts("\n== Delete the first entry ==");
  for (int i = 1; i < map_size; ++i)
    {
      map[i-1] = map[i];
    }
  map_size -= 1; /// means map_size = map_size - 1;

  /// Print the contents of the map again to see effect of change
  puts("\n== Dump the map ==");
  for (int i = 0; i < map_size; ++i)
    {
      printf("key: %s => value: %d\n", map[i].key, map[i].value);
    }

  return 0;
}


unsigned long string_sum_hash(const char *str)
{
  unsigned long result = 0;
  do
    {
      result += *str; /// *str is the ASCII value of
    }
  while (*++str != '\0');
  return result;
}

typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;

struct entry
{
  char *key;    // holds the key
  int value;    // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t *buckets[17];
};