#include "spvm_switch_info.h"

#include "spvm_parser_allocator.h"
#include "spvm_parser.h"

SPVM_SWITCH_INFO* SPVM_SWITCH_INFO_new(SPVM_PARSER* parser) {
  return SPVM_PARSER_ALLOCATOR_alloc_memory_pool(parser, parser->allocator, sizeof(SPVM_SWITCH_INFO));
}
