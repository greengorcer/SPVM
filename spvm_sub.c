#include <stdlib.h>

#include "spvm_sub.h"

#include "spvm_compiler_allocator.h"
#include "spvm_compiler.h"

SPVM_SUB* SPVM_SUB_new(SPVM_COMPILER* compiler) {
  SPVM_SUB* sub = SPVM_COMPILER_ALLOCATOR_alloc_memory_pool(compiler, compiler->allocator, sizeof(SPVM_SUB));
  
  sub->op_args = SPVM_COMPILER_ALLOCATOR_alloc_array(compiler, compiler->allocator, 0);
  sub->op_mys = SPVM_COMPILER_ALLOCATOR_alloc_array(compiler, compiler->allocator, 0);
  
  return sub;
}
