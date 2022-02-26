#include "spvm_implement.h"
#include "spvm_allocator.h"
#include "spvm_compiler.h"

SPVM_IMPLEMENT* SPVM_IMPLEMENT_new(SPVM_COMPILER* compiler) {
  SPVM_IMPLEMENT* implement = SPVM_ALLOCATOR_new_block_compile_tmp(compiler, sizeof(SPVM_IMPLEMENT));
  
  return implement;
}
