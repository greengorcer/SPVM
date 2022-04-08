#ifndef SPVM_API_RUNTIME_H
#define SPVM_API_RUNTIME_H

#include "spvm_typedecl.h"
#include "spvm_native.h"

SPVM_RUNTIME* SPVM_API_RUNTIME_new_runtime();
void SPVM_API_RUNTIME_prepare(SPVM_RUNTIME* runtime);
SPVM_OPCODE* SPVM_API_RUNTIME_get_opcodes(SPVM_RUNTIME* runtime);
int32_t SPVM_API_RUNTIME_get_opcode_ids_length(SPVM_RUNTIME* runtime);
void SPVM_API_RUNTIME_free_runtime(SPVM_RUNTIME* runtime);

int32_t SPVM_API_RUNTIME_get_basic_type_id(SPVM_RUNTIME* runtime, const char* basic_type_name);
int32_t SPVM_API_RUNTIME_get_basic_type_category(SPVM_RUNTIME* runtime, int32_t basic_type_id);
const char* SPVM_API_RUNTIME_get_basic_type_name(SPVM_RUNTIME* runtime, int32_t basic_type_id);
int32_t SPVM_API_RUNTIME_get_basic_type_name_id(SPVM_RUNTIME* runtime, int32_t basic_type_id);
int32_t SPVM_API_RUNTIME_get_basic_type_class_id(SPVM_RUNTIME* runtime, int32_t basic_type_id);
SPVM_RUNTIME_BASIC_TYPE* SPVM_API_RUNTIME_get_basic_type_with_name(SPVM_RUNTIME* runtime,  const char* basic_type_name);
SPVM_RUNTIME_BASIC_TYPE* SPVM_API_RUNTIME_get_basic_type(SPVM_RUNTIME* runtime, int32_t basic_type_id);

#endif
