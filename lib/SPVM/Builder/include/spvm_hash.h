#ifndef SPVM_HASH_H
#define SPVM_HASH_H

#include "spvm_typedecl.h"
#include "spvm_native.h"

// Hash table
struct spvm_hash {
  SPVM_ALLOCATOR* allocator;
  int32_t* table;
  char* key_buffer;
  SPVM_HASH_ENTRY* entries;
  int32_t table_capacity;
  int32_t entries_capacity;
  int32_t entries_length;
  int32_t key_buffer_capacity;
  int32_t key_buffer_length;
  int8_t memory_block_type;
};

// Hash entry
struct spvm_hash_entry {
  void* value;
  int32_t next_index;
  int32_t key_index;
};

SPVM_HASH* SPVM_HASH_new(SPVM_ALLOCATOR* allocator, int32_t capacity, int32_t memory_block_type);

void SPVM_HASH_insert(SPVM_HASH* hash, const char* key, int32_t length, void* value);
void* SPVM_HASH_fetch(SPVM_HASH* hash, const char* key, int32_t length);
void* SPVM_HASH_fetch_with_exists(SPVM_HASH* hash, const char* key, int32_t length, int32_t* exists);

void SPVM_HASH_free(SPVM_HASH* hash);

int32_t SPVM_HASH_new_hash_entry(SPVM_HASH* hash, const char* key, int32_t length, void* value);
void SPVM_HASH_rehash(SPVM_HASH* hash, int32_t new_table_capacity);
void SPVM_HASH_insert_norehash(SPVM_HASH* hash, const char* key, int32_t length, void* value);
int32_t SPVM_HASH_calc_hash_value(const char* str, int32_t len);
void SPVM_HASH_maybe_extend_entries(SPVM_HASH* hash);

#endif
