#ifndef SPVM_IMPLEMENT_H
#define SPVM_IMPLEMENT_H

#include "spvm_typedecl.h"

struct spvm_compatible {
  SPVM_OP* op_type;
};

SPVM_IMPLEMENT* SPVM_IMPLEMENT_new(SPVM_COMPILER* compiler);

#endif
