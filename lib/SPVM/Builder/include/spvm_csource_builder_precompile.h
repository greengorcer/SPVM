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


void SPVM_CSOURCE_BUILDER_PRECOMPILE_build_head(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_build_method_declaration(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, const char* class_name, const char* method_name);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_build_method_implementation(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, const char* class_name, const char* method_name);
void SPVM_CSOURCE_BUILDER_PRECOMPILE_build_class_csource(SPVM_COMPILER* compiler, SPVM_STRING_BUFFER* string_buffer, const char* class_name);

#endif
