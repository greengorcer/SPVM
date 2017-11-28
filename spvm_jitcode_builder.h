#ifndef SPVM_JITCODE_BUILDER_H
#define SPVM_JITCODE_BUILDER_H

#include "spvm_base.h"

// Constant macro
#define SPVM_JITCODE_BUILDER_OBJECT_HEADER_BYTE_SIZE sizeof(SPVM_OBJECT)
#define SPVM_JITCODE_BUILDER_OBJECT_LENGTH_BYTE_OFFSET ((int32_t)offsetof(SPVM_OBJECT, length))
#define SPVM_JITCODE_BUILDER_OBJECT_REF_COUNT_BYTE_OFFSET ((int32_t)offsetof(SPVM_OBJECT, ref_count))
#define SPVM_JITCODE_BUILDER_RUNTIME_EXCEPTION_BYTE_OFFSET ((int32_t)offsetof(SPVM_RUNTIME, exception))
#define SPVM_JITCODE_BUILDER_TYPE_CODE_VOID (SPVM_TYPE_C_CODE_VOID)
#define SPVM_JITCODE_BUILDER_TYPE_CODE_BYTE (SPVM_TYPE_C_CODE_BYTE)
#define SPVM_JITCODE_BUILDER_TYPE_CODE_SHORT (SPVM_TYPE_C_CODE_SHORT)
#define SPVM_JITCODE_BUILDER_TYPE_CODE_INT (SPVM_TYPE_C_CODE_INT)
#define SPVM_JITCODE_BUILDER_TYPE_CODE_LONG (SPVM_TYPE_C_CODE_LONG)
#define SPVM_JITCODE_BUILDER_TYPE_CODE_FLOAT (SPVM_TYPE_C_CODE_FLOAT)
#define SPVM_JITCODE_BUILDER_TYPE_CODE_DOUBLE (SPVM_TYPE_C_CODE_DOUBLE)

// Inline macro function
#define SPVM_JITCODE_BUILDER_INLINE_INC_REF_COUNT(object) ((*(int32_t*)((intptr_t)object + SPVM_JITCODE_BUILDER_OBJECT_REF_COUNT_BYTE_OFFSET))++)
#define SPVM_JITCODE_BUILDER_INLINE_DEC_REF_COUNT_ONLY(object) ((*(int32_t*)((intptr_t)object + SPVM_JITCODE_BUILDER_OBJECT_REF_COUNT_BYTE_OFFSET))--)
#define SPVM_JITCODE_BUILDER_INLINE_GET_EXCEPTION() (*(SPVM_API_OBJECT**)((intptr_t)SPVM_JITCODE_BUILDER_RUNTIME + SPVM_JITCODE_BUILDER_RUNTIME_EXCEPTION_BYTE_OFFSET))
#define SPVM_JITCODE_BUILDER_INLINE_SET_EXCEPTION_NULL() \
  do { \
    if ((*(SPVM_API_OBJECT**)((intptr_t)SPVM_JITCODE_BUILDER_RUNTIME + SPVM_JITCODE_BUILDER_RUNTIME_EXCEPTION_BYTE_OFFSET)) != NULL) { \
      api->dec_ref_count(api, (*(SPVM_API_OBJECT**)((intptr_t)SPVM_JITCODE_BUILDER_RUNTIME + SPVM_JITCODE_BUILDER_RUNTIME_EXCEPTION_BYTE_OFFSET))); \
    } \
    (*(SPVM_API_OBJECT**)((intptr_t)SPVM_JITCODE_BUILDER_RUNTIME + SPVM_JITCODE_BUILDER_RUNTIME_EXCEPTION_BYTE_OFFSET)) = NULL; \
  } \
  while (0) \

#define SPVM_JITCODE_BUILDER_INLINE_ISWEAK(object) ((intptr_t)object & 1)


void SPVM_JITCODE_BUILDER_build_jitcode(SPVM_COMPILER* compiler);

#endif
