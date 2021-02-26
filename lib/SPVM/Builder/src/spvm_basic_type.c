#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "spvm_compiler.h"
#include "spvm_basic_type.h"
#include "spvm_compiler_allocator.h"

const char* const* SPVM_BASIC_TYPE_C_ID_NAMES(void) {

  static const char* const id_names[] = {
    "unknown",
    "undef",
    "void",
    "byte",
    "short",
    "int",
    "long",
    "float",
    "double",
    "string",
    "object",
    "oarray",
    "SPVM::Byte",
    "SPVM::Short",
    "SPVM::Int",
    "SPVM::Long",
    "SPVM::Float",
    "SPVM::Double",
  };
  
  return id_names;
}

SPVM_BASIC_TYPE* SPVM_BASIC_TYPE_new(SPVM_COMPILER* compiler) {
  SPVM_BASIC_TYPE* basic_type = SPVM_COMPILER_ALLOCATOR_safe_malloc_zero(compiler, sizeof(SPVM_BASIC_TYPE));
  
  return basic_type;
}
