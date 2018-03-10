#ifndef SPVM_SWITCH_INFO_H
#define SPVM_SWITCH_INFO_H

#include "spvm_base.h"

enum {
  SPVM_SWITCH_INFO_C_ID_TABLE_SWITCH,
  SPVM_SWITCH_INFO_C_ID_LOOKUP_SWITCH,
};

// Parser information
struct SPVM_switch_info {
  SPVM_LIST* op_cases;
  SPVM_OP* op_default;
  int32_t max;
  int32_t min;
  int32_t id;
  int32_t opcode_index;
  int32_t default_opcode_index;
  SPVM_LIST* case_opcode_indexes;
};

SPVM_SWITCH_INFO* SPVM_SWITCH_INFO_new(SPVM_COMPILER* compiler);

#endif
