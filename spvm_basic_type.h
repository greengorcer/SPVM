#ifndef SPVM_BASIC_TYPE_H
#define SPVM_BASIC_TYPE_H

#include <stdio.h>

#include "spvm_base.h"

enum {
  SPVM_BASIC_TYPE_C_ID_UNKNOWN,
  SPVM_BASIC_TYPE_C_ID_VOID,
  SPVM_BASIC_TYPE_C_ID_UNDEF,
  SPVM_BASIC_TYPE_C_ID_BYTE,
  SPVM_BASIC_TYPE_C_ID_SHORT,
  SPVM_BASIC_TYPE_C_ID_INT,
  SPVM_BASIC_TYPE_C_ID_LONG,
  SPVM_BASIC_TYPE_C_ID_FLOAT,
  SPVM_BASIC_TYPE_C_ID_DOUBLE,
  SPVM_BASIC_TYPE_C_ID_ANY_OBJECT,
  SPVM_BASIC_TYPE_C_ID_STRING,
};

extern const char* const SPVM_BASIC_TYPE_C_ID_NAMES[];

struct SPVM_basic_type {
  const char* name;
  int32_t id;
};

SPVM_BASIC_TYPE* SPVM_BASIC_TYPE_new(SPVM_COMPILER* compiler);

#endif
