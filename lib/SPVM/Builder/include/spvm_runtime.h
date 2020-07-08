#ifndef SPVM_RUNTIME_H
#define SPVM_RUNTIME_H

#include "spvm_base.h"
#include "spvm_native.h"

struct SPVM_runtime {
  SPVM_OPCODE* opcodes;
  
  char* string_pool;
  int32_t string_pool_length;

  // Package variables
  SPVM_VALUE* package_vars_heap;
  
  // C function addresses(native or precompile)
  void** sub_cfunc_addresses;
  
  // Memory blocks count
  int32_t memory_blocks_count;
  
  SPVM_COMPILER* compiler;
};

#endif
