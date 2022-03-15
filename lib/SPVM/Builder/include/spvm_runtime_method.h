#ifndef SPVM_RUNTIME_METHOD_H
#define SPVM_RUNTIME_METHOD_H

#include "spvm_typedecl.h"

// Runtime method information
struct spvm_runtime_method {
  void* precompile_address;
  void* native_address;
  const char* name;
  const char* signature;
  SPVM_LIST* arg_type_ids;
  int32_t opcodes_base;
  int32_t opcodes_length;
  int32_t id;
  int32_t name_id;
  int32_t signature_id;
  int32_t class_id;
  int32_t flag;
  int32_t call_stack_byte_vars_legnth;
  int32_t call_stack_short_vars_legnth;
  int32_t call_stack_int_vars_legnth;
  int32_t call_stack_long_vars_legnth;
  int32_t call_stack_float_vars_legnth;
  int32_t call_stack_double_vars_legnth;
  int32_t call_stack_object_vars_legnth;
  int32_t call_stack_ref_vars_legnth;
  int32_t mortal_stack_length;
  int32_t return_type_id;
  int8_t is_class_method;
  int8_t is_init;
};

#endif
