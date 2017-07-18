#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "spvm_hash.h"
#include "spvm_hash_entry.h"
#include "spvm_hash_func.h"
#include "spvm_util_allocator.h"

SPVM_HASH* SPVM_HASH_new(int32_t table_capacity) {
  
  assert(table_capacity >= 0);

  // Create hash
  SPVM_HASH* hash = SPVM_UTIL_ALLOCATOR_safe_malloc_i32_zero(1, sizeof(SPVM_HASH));

  // Default table capacity
  if (table_capacity == 0) {
    hash->table_capacity = 101;
  }
  else {
    hash->table_capacity = table_capacity;
  }
  
  // Initialize table
  hash->table = SPVM_UTIL_ALLOCATOR_safe_malloc_i32_zero(hash->table_capacity, sizeof(int32_t));
  memset(hash->table, -1, hash->table_capacity * sizeof(int32_t));
  
  // Initialize entries
  hash->entries_capacity = 255;
  hash->entries = SPVM_UTIL_ALLOCATOR_safe_malloc_i32_zero(hash->entries_capacity, sizeof(SPVM_HASH_ENTRY));
  hash->entries_length = 0;
  
  // Initialize key buffer
  hash->key_buffer_capacity = 0xFF;
  hash->key_buffer = SPVM_UTIL_ALLOCATOR_safe_malloc_i32_zero(hash->key_buffer_capacity, sizeof(char));
  hash->key_buffer_length = 0;
  
  return hash;
}

void SPVM_HASH_maybe_extend_entries(SPVM_HASH* hash) {
  
  assert(hash);
  
  int32_t entries_length = hash->entries_length;
  
  assert(entries_length >= 0);
  
  int32_t entries_capacity = hash->entries_capacity;
  
  if (entries_length >= entries_capacity) {
    int32_t new_entries_capacity = entries_capacity * 2;

    SPVM_HASH_ENTRY* new_entries = SPVM_UTIL_ALLOCATOR_safe_malloc_i32_zero(new_entries_capacity, sizeof(SPVM_HASH_ENTRY));
    memcpy(new_entries, hash->entries, entries_capacity * sizeof(SPVM_HASH_ENTRY));
    // Free BSD segfault bug
    // free(hash->entries);
    hash->entries = new_entries;
    
    hash->entries_capacity = new_entries_capacity;
  }
}

void SPVM_HASH_maybe_extend_key_buffer(SPVM_HASH* hash, int32_t length) {
  
  assert(hash);
  
  int32_t key_buffer_length = hash->key_buffer_length;
  
  assert(key_buffer_length >= 0);
  
  int32_t key_buffer_capacity = hash->key_buffer_capacity;
  
  if (key_buffer_length + length >= key_buffer_capacity) {
    int32_t new_key_buffer_capacity = key_buffer_capacity * 2;

    char* new_key_buffer = SPVM_UTIL_ALLOCATOR_safe_malloc_i32_zero(new_key_buffer_capacity, sizeof(SPVM_HASH_ENTRY));
    memcpy(new_key_buffer, hash->key_buffer, key_buffer_capacity);
    free(hash->key_buffer);
    hash->key_buffer = new_key_buffer;

    hash->key_buffer_capacity = new_key_buffer_capacity;
  }
}

void SPVM_HASH_free(SPVM_HASH* hash) {
  
  assert(hash);
  
  free(hash->table);
  free(hash->entries);
  free(hash->key_buffer);
  free(hash);
}

int32_t SPVM_HASH_new_hash_entry(SPVM_HASH* hash, const char* key, void* value) {
  
  assert(hash);
  assert(key);
  
  int32_t index = hash->entries_length;
  
  SPVM_HASH_maybe_extend_entries(hash);
  SPVM_HASH_ENTRY* hash_entry = &hash->entries[index];
  
  int32_t key_length = strlen(key);
  SPVM_HASH_maybe_extend_key_buffer(hash, key_length);
  
  hash_entry->key_index = hash->key_buffer_length;
  
  strncpy(&hash->key_buffer[hash->key_buffer_length], key, key_length);
  hash->key_buffer[hash->key_buffer_length + key_length] = '\0';
  
  hash->key_buffer_length += key_length + 1;
  
  *(void**)&hash_entry->value = value;
  hash_entry->next_index = -1;
  
  hash->entries_length++;
  
  return index;
}

void SPVM_HASH_rehash(SPVM_HASH* hash, int32_t new_table_capacity) {
  
  assert(hash);
  assert(new_table_capacity > 0);
  
  // Create new hash
  SPVM_HASH* new_hash = SPVM_HASH_new(new_table_capacity);
  
  // Rehash
  {
    int32_t i;
    for (i = 0; i < hash->entries_length; i++) {
      const char* key = &hash->key_buffer[hash->entries[i].key_index];
      
      assert(key);
      
      void* value = *(void**)(hash->entries[i].value);
      
      SPVM_HASH_insert_norehash(new_hash, key, strlen(key), value);
    }
  }
  
  // Replace hash fields
  free(hash->table);
  free(hash->entries);
  free(hash->key_buffer);
  hash->entries_length = new_hash->entries_length;
  hash->table_capacity = new_hash->table_capacity;
  hash->entries_capacity = new_hash->entries_capacity;
  hash->table = new_hash->table;
  hash->entries = new_hash->entries;
  
  hash->key_buffer_capacity = new_hash->key_buffer_capacity;
  hash->key_buffer_length = new_hash->key_buffer_length;
  hash->key_buffer = new_hash->key_buffer;
}

void SPVM_HASH_insert_norehash(SPVM_HASH* hash, const char* key, int32_t length, void* value) {
  
  assert(hash);
  assert(key);
  assert(length > 0);
  
  int32_t hash_value = SPVM_HASH_FUNC_calc_hash_for_index(key, length);
  int32_t table_index = hash_value % hash->table_capacity;
  
  assert(hash->table[table_index] >= -1);
  
  if (hash->table[table_index] != -1) {
    
    SPVM_HASH_ENTRY* entry = &hash->entries[hash->table[table_index]];
    while (1) {
      if (strncmp(key, &hash->key_buffer[entry->key_index], length) == 0) {
        *(void**)&entry->value = value;
        break;
      }
      else {
        if (entry->next_index != -1) {
          entry = &hash->entries[entry->next_index];
        }
        else {
          int32_t new_entry_index = SPVM_HASH_new_hash_entry(hash, key, value);
          entry = &hash->entries[new_entry_index];
          entry->next_index = new_entry_index;
          break;
        }
      }
    }
  }
  else {
    int32_t new_entry_index = SPVM_HASH_new_hash_entry(hash, key, value);
    hash->table[table_index] = new_entry_index;
  }
}

void SPVM_HASH_insert(SPVM_HASH* hash, const char* key, int32_t length, void* value) {
  
  assert(hash);
  assert(key);
  assert(length > 0);
  
  // Rehash
  if (hash->entries_length > hash->table_capacity * 0.75) {
    int32_t new_table_capacity = (hash->table_capacity * 2) + 1;
    
    SPVM_HASH_rehash(hash, new_table_capacity);
  }
  
  SPVM_HASH_insert_norehash(hash, key, length, value);
}

void* SPVM_HASH_search(SPVM_HASH* hash, const char* key, int32_t length) {
  
  assert(hash);
  assert(key);
  assert(length > 0);

  int32_t hash_value = SPVM_HASH_FUNC_calc_hash_for_index(key, length);
  int32_t table_index = hash_value % hash->table_capacity;
  
  SPVM_HASH_ENTRY* entry = NULL;
  if (hash->table[table_index] != -1) {
    entry = &hash->entries[hash->table[table_index]];
  }
  while (1) {
    if (entry) {
      
      if (strncmp(key, &hash->key_buffer[entry->key_index], length) == 0) {
        return *(void**)&entry->value;
      }
      else {
        if (entry->next_index == -1) {
          entry = NULL;
        }
        else {
          entry = &hash->entries[entry->next_index];
        }
      }
    }
    else {
      return NULL;
    }
  }
}
