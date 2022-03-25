#ifndef SPVM_USE_H
#define SPVM_USE_H

#include "spvm_typedecl.h"

struct spvm_use {
  SPVM_OP* op_use;
  SPVM_OP* op_type;
  const char* class_name;
  const char* class_alias_name;
  int32_t is_require;
  const char* file;
};

SPVM_USE* SPVM_USE_new(SPVM_COMPILER* compiler);

#endif
