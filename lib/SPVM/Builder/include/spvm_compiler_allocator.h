#ifndef SPVM_COMPILER_ALLOCATOR_H
#define SPVM_COMPILER_ALLOCATOR_H

#include "spvm_typedecl.h"

// Parser information
struct spvm_compiler_allocator {
  // Blocks
  SPVM_LIST* blocks;

  // Lists
  SPVM_LIST* lists;
  
  // Hashes
  SPVM_LIST* hashes;
  
  // Constant pools
  SPVM_LIST* constant_pools;
  
  // This is all memory blocks allocated by the SPVM compiler and runtime.
  int32_t memory_blocks_count;
};

void* SPVM_COMPILER_ALLOCATOR_safe_malloc_zero_tmp_no_managed(size_t byte_size);
void SPVM_COMPILER_ALLOCATOR_free_tmp_no_managed(void* block);

void* SPVM_COMPILER_ALLOCATOR_safe_malloc_zero_tmp(SPVM_COMPILER* compiler, int32_t byte_size);
void SPVM_COMPILER_ALLOCATOR_free_tmp(SPVM_COMPILER* compiler, void* block);

SPVM_COMPILER_ALLOCATOR* SPVM_COMPILER_ALLOCATOR_new();
void SPVM_COMPILER_ALLOCATOR_init(SPVM_COMPILER* compiler);

SPVM_LIST* SPVM_COMPILER_ALLOCATOR_alloc_list(SPVM_COMPILER* compiler, int32_t capacity);
SPVM_HASH* SPVM_COMPILER_ALLOCATOR_alloc_hash(SPVM_COMPILER* compiler, int32_t capacity);
void* SPVM_COMPILER_ALLOCATOR_safe_malloc_zero(SPVM_COMPILER* compiler, int32_t size);
const char* SPVM_COMPILER_ALLOCATOR_alloc_format_string(SPVM_COMPILER* compiler, const char* message_template, ...);

SPVM_VMCODE* SPVM_COMPILER_ALLOCATOR_alloc_vmcode(SPVM_COMPILER* compiler);

void SPVM_COMPILER_ALLOCATOR_free(SPVM_COMPILER* compiler);

#endif
