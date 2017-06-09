#include "spvm_type_component_array.h"
#include "spvm.h"
#include "spvm_parser_allocator.h"
#include "spvm_parser.h"

SPVM_TYPE_COMPONENT_ARRAY* SPVM_TYPE_COMPONENT_ARRAY_new(SPVM* spvm) {
  return SPVM_PARSER_ALLOCATOR_alloc_memory_pool(spvm, spvm->parser->allocator, sizeof(SPVM_TYPE_COMPONENT_ARRAY));
}
