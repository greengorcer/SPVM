#ifndef SPVM_CSOURCE_BUILDER_PRECOMPILE_H
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_H

#include "spvm_typedecl.h"

enum {
  SPVM_CSOURCE_BUILDER_PRECOMPILE_C_CTYPE_ID_BYTE,
  SPVM_CSOURCE_BUILDER_PRECOMPILE_C_CTYPE_ID_SHORT,
  SPVM_CSOURCE_BUILDER_PRECOMPILE_C_CTYPE_ID_INT,
  SPVM_CSOURCE_BUILDER_PRECOMPILE_C_CTYPE_ID_LONG,
  SPVM_CSOURCE_BUILDER_PRECOMPILE_C_CTYPE_ID_FLOAT,
  SPVM_CSOURCE_BUILDER_PRECOMPILE_C_CTYPE_ID_DOUBLE,
  SPVM_CSOURCE_BUILDER_PRECOMPILE_C_CTYPE_ID_BOOL_OBJECT,
  SPVM_CSOURCE_BUILDER_PRECOMPILE_C_CTYPE_ID_OBJECT,
  SPVM_CSOURCE_BUILDER_PRECOMPILE_C_CTYPE_ID_REF,
};

// Constant macro
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_OBJECT_HEADER_BYTE_SIZE sizeof(SPVM_OBJECT)
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_OBJECT_LENGTH_BYTE_OFFSET ((int32_t)offsetof(SPVM_OBJECT, length))
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_OBJECT_REF_COUNT_BYTE_OFFSET ((int32_t)offsetof(SPVM_OBJECT, ref_count))
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_RUNTIME_EXCEPTION_BYTE_OFFSET ((int32_t)offsetof(SPVM_RUNTIME, exception))
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_TYPE_CODE_VOID (SPVM_BASIC_TYPE_C_ID_VOID)
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_TYPE_CODE_BYTE (SPVM_BASIC_TYPE_C_ID_BYTE)
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_TYPE_CODE_SHORT (SPVM_BASIC_TYPE_C_ID_SHORT)
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_TYPE_CODE_INT (SPVM_BASIC_TYPE_C_ID_INT)
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_TYPE_CODE_LONG (SPVM_BASIC_TYPE_C_ID_LONG)
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_TYPE_CODE_FLOAT (SPVM_BASIC_TYPE_C_ID_FLOAT)
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_TYPE_CODE_DOUBLE (SPVM_BASIC_TYPE_C_ID_DOUBLE)

// Inline macro function
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_INLINE_INC_REF_COUNT(object) ((*(int32_t*)((intptr_t)object + SPVM_CSOURCE_BUILDER_PRECOMPILE_OBJECT_REF_COUNT_BYTE_OFFSET))++)
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_INLINE_DEC_REF_COUNT_ONLY(object) ((*(int32_t*)((intptr_t)object + SPVM_CSOURCE_BUILDER_PRECOMPILE_OBJECT_REF_COUNT_BYTE_OFFSET))--)
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_INLINE_GET_EXCEPTION() (*(void**)((intptr_t)SPVM_CSOURCE_BUILDER_PRECOMPILE_RUNTIME + SPVM_CSOURCE_BUILDER_PRECOMPILE_RUNTIME_EXCEPTION_BYTE_OFFSET))
#define SPVM_CSOURCE_BUILDER_PRECOMPILE_INLINE_SET_EXCEPTION_NULL() \
  do { \
    if ((*(void**)((intptr_t)SPVM_CSOURCE_BUILDER_PRECOMPILE_RUNTIME + SPVM_CSOURCE_BUILDER_PRECOMPILE_RUNTIME_EXCEPTION_BYTE_OFFSET)) != NULL) { \
      env->dec_ref_count(env, (*(void**)((intptr_t)SPVM_CSOURCE_BUILDER_PRECOMPILE_RUNTIME + SPVM_CSOURCE_BUILDER_PRECOMPILE_RUNTIME_EXCEPTION_BYTE_OFFSET))); \
    } \
    (*(void**)((intptr_t)SPVM_CSOURCE_BUILDER_PRECOMPILE_RUNTIME + SPVM_CSOURCE_BUILDER_PRECOMPILE_RUNTIME_EXCEPTION_BYTE_OFFSET)) = NULL; \
  } \
  while (0) \

#define SPVM_CSOURCE_BUILDER_PRECOMPILE_INLINE_ISWEAK(object) ((intptr_t)object & 1)


void SPVM_CSOURCE_BUILDER_PRECOMPILE_build_class_csource(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, const char* class_name);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_build_head(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_build_method_declaration(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, const char* class_name, const char* method_name);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_build_method_implementation(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, const char* class_name, const char* method_name);

const char* SPVM_CSOURCE_BUILDER_PRECOMPILE_get_ctype_name(SPVM_COMPILER* compiler, int32_t ctype_id);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_var(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_var_value(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t index, int32_t field_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_operand(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t var_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_operand_deref(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t var_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_stack(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t var_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_operand_value(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t var_index, int32_t field_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_bool(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_numeric_eq(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_numeric_ne(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_numeric_gt(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_numeric_ge(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_numeric_lt(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_numeric_le(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_numeric_cmp(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_add(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_subtract(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_multiply(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_divide_integral(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_divide_unsigned_int(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_divide_unsigned_long(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_divide_floating_point(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_remainder_integral(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_remainder_unsigned_int(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_remainder_unsigned_long(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_left_shift(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_right_logical_shift(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_right_arithmetic_shift(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_bit_and(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_bit_or(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_bit_xor(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_negate(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_plus(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_complement(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_inc(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t number);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_convert(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t out_ctype_id, int32_t in_ctype_id, int32_t out_index, int32_t in_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_array_fetch(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t out_index, int32_t array_index, int32_t index_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_array_store(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t array_index, int32_t index_index, int32_t in_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_mulnum_array_fetch(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t out_index, int32_t array_index, int32_t index_index, int32_t fields_length);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_mulnum_array_field_fetch(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t out_index, int32_t array_index, int32_t index_index, int32_t fields_length, int32_t field_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_mulnum_array_store(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t array_index, int32_t index_index, int32_t in_index, int32_t fields_length);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_mulnum_array_field_store(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t array_index, int32_t index_index, int32_t in_index, int32_t fields_length, int32_t field_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_mulnum_deref(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t out_var_id, int32_t ref_var_id, int32_t fields_length);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_mulnum_deref_get_field(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t out_var_id, int32_t ref_var_id, int32_t field_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_mulnum_deref_set_field(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t ref_var_id, int32_t in_var_id, int32_t field_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_move(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_get_deref(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_set_deref(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in_index);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_get_field(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, SPVM_CLASS* class, int32_t field_ctype_id, int32_t out_index, int32_t object_index, SPVM_FIELD* field);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_add_set_field(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, SPVM_CLASS* class, int32_t field_ctype_id, int32_t object_index, SPVM_FIELD* field, int32_t in_index);


#endif
