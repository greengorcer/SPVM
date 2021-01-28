#ifndef SPVM_ARRAY_FIELD_ACCESS_H
#define SPVM_ARRAY_FIELD_ACCESS_H

#include "spvm_typedef.h"

struct SPVM_array_field_access {
  SPVM_FIELD* field;
};

SPVM_ARRAY_FIELD_ACCESS* SPVM_ARRAY_FIELD_ACCESS_new(SPVM_COMPILER* compiler);

#endif
