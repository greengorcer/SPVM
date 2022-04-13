#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdarg.h>

#include "spvm_list.h"
#include "spvm_hash.h"

#include "spvm_allocator.h"

#include "spvm_opcode_array.h"
#include "spvm_opcode.h"
#include "spvm_class.h"
#include "spvm_method.h"
#include "spvm_type.h"
#include "spvm_weaken_backref.h"
#include "spvm_string_buffer.h"
#include "spvm_api.h"
#include "spvm_object.h"
#include "spvm_native.h"

#include "spvm_runtime_basic_type.h"
#include "spvm_runtime_class.h"
#include "spvm_runtime_class_var.h"
#include "spvm_runtime_field.h"
#include "spvm_runtime.h"
#include "spvm_runtime_method.h"
#include "spvm_runtime_constant_string.h"
#include "spvm_runtime_type.h"
#include "spvm_precompile.h"
#include "spvm_api_compiler.h"
#include "spvm_api_string_buffer.h"
#include "spvm_api_allocator.h"
#include "spvm_api_runtime.h"
#include "spvm_api_precompile.h"




















































SPVM_ENV* SPVM_API_new_env_raw() {

  // Env Allocator
  SPVM_ENV_ALLOCATOR* env_allocator = SPVM_API_ALLOCATOR_new_env();

  // Env String Buffer
  SPVM_ENV_STRING_BUFFER* env_string_buffer = SPVM_API_STRING_BUFFER_new_env();

  // Env Compiler
  SPVM_ENV_COMPILER* env_compiler = SPVM_API_COMPILER_new_env();

  // Env Precompile
  void* env_precompile_init[]  = {
    SPVM_API_PRECOMPILE_new_precompile,
    SPVM_API_PRECOMPILE_free_precompile,
    SPVM_API_PRECOMPILE_set_runtime,
    SPVM_API_PRECOMPILE_get_runtime,
    SPVM_API_PRECOMPILE_create_precompile_source,
  };
  SPVM_ENV_PRECOMPILE* env_precompile = calloc(1, sizeof(env_precompile_init));
  memcpy(env_precompile, env_precompile_init, sizeof(env_precompile_init));

  // Env Runtime
  void* env_runtime_init[]  = {
    SPVM_API_RUNTIME_new_runtime,
    SPVM_API_RUNTIME_free_runtime,
    SPVM_API_RUNTIME_prepare,
    SPVM_API_RUNTIME_get_opcodes,
    SPVM_API_RUNTIME_get_opcodes_length,
    SPVM_API_RUNTIME_get_constant_string_id,
    SPVM_API_RUNTIME_get_constant_string_value,
    SPVM_API_RUNTIME_get_name,
    SPVM_API_RUNTIME_get_basic_type_id_by_name,
    SPVM_API_RUNTIME_get_basic_type_name_id,
    SPVM_API_RUNTIME_get_basic_type_class_id,
    SPVM_API_RUNTIME_get_basic_type_category,
    SPVM_API_RUNTIME_get_type_basic_type_id,
    SPVM_API_RUNTIME_get_type_dimension,
    SPVM_API_RUNTIME_get_type_width,
    SPVM_API_RUNTIME_get_type_is_object,
    SPVM_API_RUNTIME_get_type_is_ref,
    SPVM_API_RUNTIME_get_class_id_by_name,
    SPVM_API_RUNTIME_get_class_name_id,
    SPVM_API_RUNTIME_get_class_module_file_id,
    SPVM_API_RUNTIME_get_class_is_anon,
    SPVM_API_RUNTIME_get_class_fields_base_id,
    SPVM_API_RUNTIME_get_class_fields_length,
    SPVM_API_RUNTIME_get_class_methods_base_id,
    SPVM_API_RUNTIME_get_class_methods_length,
    SPVM_API_RUNTIME_get_class_class_vars_base_id,
    SPVM_API_RUNTIME_get_class_class_vars_length,
    SPVM_API_RUNTIME_get_class_anon_methods_base_id,
    SPVM_API_RUNTIME_get_class_anon_methods_length,
    SPVM_API_RUNTIME_get_class_var_id_by_index,
    SPVM_API_RUNTIME_get_class_var_id_by_name,
    SPVM_API_RUNTIME_get_class_var_name_id,
    SPVM_API_RUNTIME_get_class_var_signature_id,
    SPVM_API_RUNTIME_get_class_var_class_id,
    SPVM_API_RUNTIME_get_field_id_by_index,
    SPVM_API_RUNTIME_get_field_id_by_name,
    SPVM_API_RUNTIME_get_field_name_id,
    SPVM_API_RUNTIME_get_field_type_id,
    SPVM_API_RUNTIME_get_field_signature_id,
    SPVM_API_RUNTIME_get_field_class_id,
    SPVM_API_RUNTIME_get_method_id_by_index,
    SPVM_API_RUNTIME_get_method_id_by_name,
    SPVM_API_RUNTIME_get_method_name_id,
    SPVM_API_RUNTIME_get_method_signature_id,
    SPVM_API_RUNTIME_get_method_return_type_id,
    SPVM_API_RUNTIME_get_method_class_id,
    SPVM_API_RUNTIME_get_method_is_class_method,
    SPVM_API_RUNTIME_get_method_is_anon,
    SPVM_API_RUNTIME_get_method_has_precompile_flag,
    SPVM_API_RUNTIME_get_method_call_stack_byte_vars_length,
    SPVM_API_RUNTIME_get_method_call_stack_short_vars_length,
    SPVM_API_RUNTIME_get_method_call_stack_int_vars_length,
    SPVM_API_RUNTIME_get_method_call_stack_long_vars_length,
    SPVM_API_RUNTIME_get_method_call_stack_float_vars_length,
    SPVM_API_RUNTIME_get_method_call_stack_double_vars_length,
    SPVM_API_RUNTIME_get_method_call_stack_object_vars_length,
    SPVM_API_RUNTIME_get_method_call_stack_ref_vars_length,
    SPVM_API_RUNTIME_get_method_mortal_stack_length,
    SPVM_API_RUNTIME_get_method_opcodes_base_id,
    SPVM_API_RUNTIME_get_method_opcodes_length,
    SPVM_API_RUNTIME_get_method_args_base_id,
    SPVM_API_RUNTIME_get_method_args_length,
    SPVM_API_RUNTIME_get_arg_type_id,
    SPVM_API_RUNTIME_get_anon_method_method_id,
    SPVM_API_RUNTIME_get_native_method_address,
    SPVM_API_RUNTIME_set_native_method_address,
    SPVM_API_RUNTIME_get_precompile_method_address,
    SPVM_API_RUNTIME_set_precompile_method_address,
    (void*)(intptr_t)sizeof(SPVM_OBJECT), // object_header_byte_size
    (void*)(intptr_t)offsetof(SPVM_OBJECT, weaken_backref_head), // weaken_backref_head
    (void*)(intptr_t)offsetof(SPVM_OBJECT, ref_count), // object_ref_count_offset
    (void*)(intptr_t)offsetof(SPVM_OBJECT, basic_type_id), // object_basic_type_id_offset
    (void*)(intptr_t)offsetof(SPVM_OBJECT, type_dimension), // object_type_dimension_offset
    (void*)(intptr_t)offsetof(SPVM_OBJECT, flag), // object_flag_offset
    (void*)(intptr_t)offsetof(SPVM_OBJECT, length), // object_length_offset
  };
  SPVM_ENV_RUNTIME* env_runtime = calloc(1, sizeof(env_runtime_init));
  memcpy(env_runtime, env_runtime_init, sizeof(env_runtime_init));

  // Env API
  void* env_api_init[]  = {
    env_allocator,
    env_string_buffer,
    env_compiler,
    env_precompile,
    env_runtime,
  };
  SPVM_ENV_API* env_api = calloc(1, sizeof(env_api_init));
  memcpy(env_api, env_api_init, sizeof(env_api_init));

  // The impelements of Native APIs
  void* env_init[]  = {
    NULL, // class_vars_heap
    (void*)(intptr_t)sizeof(SPVM_OBJECT), // object_header_byte_size
    (void*)(intptr_t)offsetof(SPVM_OBJECT, weaken_backref_head), // weaken_backref_head
    (void*)(intptr_t)offsetof(SPVM_OBJECT, ref_count), // object_ref_count_offset
    (void*)(intptr_t)offsetof(SPVM_OBJECT, basic_type_id), // object_basic_type_id_offset
    (void*)(intptr_t)offsetof(SPVM_OBJECT, type_dimension), // object_type_dimension_offset
    (void*)NULL, // object_type_category_offset(unused)
    (void*)(intptr_t)offsetof(SPVM_OBJECT, flag), // object_flag_offset
    (void*)(intptr_t)offsetof(SPVM_OBJECT, length), // object_length_offset
    (void*)(intptr_t)SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE_OBJECT, // byte_object_basic_type_id
    (void*)(intptr_t)SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT_OBJECT, // short_object_basic_type_id
    (void*)(intptr_t)SPVM_NATIVE_C_BASIC_TYPE_ID_INT_OBJECT, // int_object_basic_type_id
    (void*)(intptr_t)SPVM_NATIVE_C_BASIC_TYPE_ID_LONG_OBJECT,  // long_object_basic_type_id
    (void*)(intptr_t)SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT_OBJECT, // float_object_basic_type_id
    (void*)(intptr_t)SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE_OBJECT, // double_object_basic_type_id
    NULL, // runtime
    NULL, // exception_object
    NULL, // native_mortal_stack
    NULL, // native_mortal_stack_top
    NULL, // native_mortal_stack_capacity
    SPVM_API_get_basic_type_id,
    SPVM_API_get_field_id,
    SPVM_API_get_field_offset,
    SPVM_API_get_class_var_id,
    SPVM_API_get_class_method_id,
    SPVM_API_get_instance_method_id,
    SPVM_API_new_object_raw,
    SPVM_API_new_object,
    SPVM_API_new_byte_array_raw,
    SPVM_API_new_byte_array,
    SPVM_API_new_short_array_raw,
    SPVM_API_new_short_array,
    SPVM_API_new_int_array_raw,
    SPVM_API_new_int_array,
    SPVM_API_new_long_array_raw,
    SPVM_API_new_long_array,
    SPVM_API_new_float_array_raw,
    SPVM_API_new_float_array,
    SPVM_API_new_double_array_raw,
    SPVM_API_new_double_array,
    SPVM_API_new_object_array_raw,
    SPVM_API_new_object_array,
    SPVM_API_new_muldim_array_raw,
    SPVM_API_new_muldim_array,
    SPVM_API_new_mulnum_array_raw,
    SPVM_API_new_mulnum_array,
    SPVM_API_new_string_nolen_raw,
    SPVM_API_new_string_nolen,
    SPVM_API_new_string_raw,
    SPVM_API_new_string,
    SPVM_API_new_pointer_raw,
    SPVM_API_new_pointer,
    SPVM_API_concat_raw,
    SPVM_API_concat,
    SPVM_API_new_stack_trace_raw,
    SPVM_API_new_stack_trace,
    SPVM_API_length,
    SPVM_API_get_elems_byte,
    SPVM_API_get_elems_short,
    SPVM_API_get_elems_int,
    SPVM_API_get_elems_long,
    SPVM_API_get_elems_float,
    SPVM_API_get_elems_double,
    SPVM_API_get_elem_object,
    SPVM_API_set_elem_object,
    SPVM_API_get_field_byte,
    SPVM_API_get_field_short,
    SPVM_API_get_field_int,
    SPVM_API_get_field_long,
    SPVM_API_get_field_float,
    SPVM_API_get_field_double,
    SPVM_API_get_field_object,
    SPVM_API_set_field_byte,
    SPVM_API_set_field_short,
    SPVM_API_set_field_int,
    SPVM_API_set_field_long,
    SPVM_API_set_field_float,
    SPVM_API_set_field_double,
    SPVM_API_set_field_object,
    SPVM_API_get_class_var_byte,
    SPVM_API_get_class_var_short,
    SPVM_API_get_class_var_int,
    SPVM_API_get_class_var_long,
    SPVM_API_get_class_var_float,
    SPVM_API_get_class_var_double,
    SPVM_API_get_class_var_object,
    SPVM_API_set_class_var_byte,
    SPVM_API_set_class_var_short,
    SPVM_API_set_class_var_int,
    SPVM_API_set_class_var_long,
    SPVM_API_set_class_var_float,
    SPVM_API_set_class_var_double,
    SPVM_API_set_class_var_object,
    SPVM_API_pointer,
    SPVM_API_set_pointer,
    SPVM_API_call_spvm_method,
    SPVM_API_exception,
    SPVM_API_set_exception,
    SPVM_API_ref_count,
    SPVM_API_inc_ref_count,
    SPVM_API_dec_ref_count,
    SPVM_API_enter_scope,
    SPVM_API_push_mortal,
    SPVM_API_leave_scope,
    SPVM_API_remove_mortal,
    SPVM_API_is_type,
    SPVM_API_has_callback,
    SPVM_API_get_object_basic_type_id,
    SPVM_API_get_object_type_dimension,
    SPVM_API_weaken,
    SPVM_API_isweak,
    SPVM_API_unweaken,
    SPVM_API_alloc_memory_block_zero,
    SPVM_API_free_memory_block,
    SPVM_API_get_memory_blocks_count,
    SPVM_API_get_type_name_raw,
    SPVM_API_get_type_name,
    SPVM_API_new_env,
    SPVM_API_free_env,
    NULL, // memory_blocks_count
    SPVM_API_get_chars,
    SPVM_API_die,
    SPVM_API_new_object_by_name,
    SPVM_API_new_pointer_by_name,
    SPVM_API_set_field_byte_by_name,
    SPVM_API_set_field_short_by_name,
    SPVM_API_set_field_int_by_name,
    SPVM_API_set_field_long_by_name,
    SPVM_API_set_field_float_by_name,
    SPVM_API_set_field_double_by_name,
    SPVM_API_set_field_object_by_name,
    SPVM_API_get_field_byte_by_name,
    SPVM_API_get_field_short_by_name,
    SPVM_API_get_field_int_by_name,
    SPVM_API_get_field_long_by_name,
    SPVM_API_get_field_float_by_name,
    SPVM_API_get_field_double_by_name,
    SPVM_API_get_field_object_by_name,
    SPVM_API_set_class_var_byte_by_name,
    SPVM_API_set_class_var_short_by_name,
    SPVM_API_set_class_var_int_by_name,
    SPVM_API_set_class_var_long_by_name,
    SPVM_API_set_class_var_float_by_name,
    SPVM_API_set_class_var_double_by_name,
    SPVM_API_set_class_var_object_by_name,
    SPVM_API_get_class_var_byte_by_name,
    SPVM_API_get_class_var_short_by_name,
    SPVM_API_get_class_var_int_by_name,
    SPVM_API_get_class_var_long_by_name,
    SPVM_API_get_class_var_float_by_name,
    SPVM_API_get_class_var_double_by_name,
    SPVM_API_get_class_var_object_by_name,
    SPVM_API_call_class_method_by_name,
    SPVM_API_call_instance_method_by_name,
    SPVM_API_get_field_string_chars_by_name,
    (void*)(intptr_t)SPVM_NATIVE_C_BASIC_TYPE_ID_ANY_OBJECT, // any_object_basic_type_id
    SPVM_API_dump_raw,
    SPVM_API_dump,
    SPVM_API_call_spvm_method, // call_class_method
    SPVM_API_call_spvm_method, // call_instance_method
    SPVM_API_get_instance_method_id_static,
    SPVM_API_get_bool_object_value,
    (void*)(intptr_t)SPVM_NATIVE_C_BASIC_TYPE_ID_STRING, // string_basic_type_id
    SPVM_API_make_read_only,
    SPVM_API_is_read_only,
    SPVM_API_is_array,
    SPVM_API_is_string,
    SPVM_API_is_numeric_array,
    SPVM_API_is_mulnum_array,
    SPVM_API_get_elem_byte_size,
    SPVM_API_new_array_proto_raw,
    SPVM_API_new_array_proto,
    SPVM_API_copy_raw,
    SPVM_API_copy,
    SPVM_API_shorten,
    SPVM_API_has_interface,
    NULL, // no_symbol_cache_flag
    SPVM_API_set_no_symbol_cache_flag,
    SPVM_API_get_no_symbol_cache_flag,
    SPVM_API_print,
    SPVM_API_print_stderr,
    SPVM_API_new_env_raw,
    SPVM_API_free_env_raw,
    SPVM_API_init_env,
    SPVM_API_call_init_blocks,
    SPVM_API_cleanup_global_vars,
    SPVM_API_is_object_array,
    SPVM_API_can_assign_array_element,
    env_api,
    SPVM_API_free_env_prepared,
  };
  
  SPVM_ENV* env = calloc(1, sizeof(env_init));
  if (env == NULL) {
    return NULL;
  }
  memcpy(env, env_init, sizeof(env_init));

  return env;
}

int32_t SPVM_API_init_env(SPVM_ENV* env) {
  
  SPVM_RUNTIME* runtime = env->runtime;

  // Mortal stack
  int32_t native_mortal_stack_capacity = 1;
  void* native_mortal_stack = calloc(sizeof(SPVM_OBJECT*), native_mortal_stack_capacity);
  if (native_mortal_stack == NULL) {
    return 1;
  }

  // Initialize Class Variables
  void* class_vars_heap = calloc(sizeof(SPVM_VALUE), ((int64_t)runtime->class_vars_length + 1));
  if (class_vars_heap == NULL) {
    return 2;
  }
  
  env->native_mortal_stack_capacity = (void*)(intptr_t)native_mortal_stack_capacity;
  env->native_mortal_stack = native_mortal_stack;
  env->class_vars_heap = class_vars_heap;

  // Adjust alignment SPVM_VALUE
  int32_t object_header_byte_size = sizeof(SPVM_OBJECT);
  if (object_header_byte_size % sizeof(SPVM_VALUE) != 0) {
    object_header_byte_size += (sizeof(SPVM_VALUE) - object_header_byte_size % sizeof(SPVM_VALUE));
  }
  assert(object_header_byte_size % sizeof(SPVM_VALUE) == 0);
  
  // Object header byte size
  env->object_header_byte_size = (void*)(intptr_t)object_header_byte_size;
  
  return 0;
}

void SPVM_API_make_read_only(SPVM_ENV* env, SPVM_OBJECT* string) {
  if (string && (string->basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_STRING && string->type_dimension == 0)) {
    string->flag |= SPVM_OBJECT_C_FLAG_IS_READ_ONLY;
  }
}

int32_t SPVM_API_is_read_only(SPVM_ENV* env, SPVM_OBJECT* string) {
  
  int32_t is_read_only;
  if (string) {
    if (string->flag & SPVM_OBJECT_C_FLAG_IS_READ_ONLY) {
      is_read_only = 1;
    }
    else {
      is_read_only = 0;
    }
  }
  else {
    is_read_only = 0;
  }
  
  return is_read_only;
}

SPVM_OBJECT* SPVM_API_dump(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  SPVM_OBJECT* str = SPVM_API_dump_raw(env, object);
  
  SPVM_API_push_mortal(env, str);

  return str;
}

SPVM_OBJECT* SPVM_API_dump_raw(SPVM_ENV* env, SPVM_OBJECT* object) {

  SPVM_RUNTIME* runtime = env->runtime;
  
  int32_t depth = 0;
  SPVM_STRING_BUFFER* string_buffer = SPVM_STRING_BUFFER_new(runtime->allocator, 255, SPVM_ALLOCATOR_C_ALLOC_TYPE_TMP);
  SPVM_HASH* address_symtable = SPVM_HASH_new(runtime->allocator, 255, SPVM_ALLOCATOR_C_ALLOC_TYPE_TMP);
  
  SPVM_API_dump_recursive(env, object, &depth, string_buffer, address_symtable);
  
  int32_t string_buffer_length = string_buffer->length;
  
  SPVM_OBJECT* dump = SPVM_API_new_string_raw(env, string_buffer->value, string_buffer->length);
  
  SPVM_HASH_free(address_symtable);
  address_symtable = NULL;
  
  SPVM_STRING_BUFFER_free(string_buffer);
  string_buffer = NULL;
  
  return dump;
}

void SPVM_API_dump_recursive(SPVM_ENV* env, SPVM_OBJECT* object, int32_t* depth, SPVM_STRING_BUFFER* string_buffer, SPVM_HASH* address_symtable) {
  
  SPVM_RUNTIME* runtime = env->runtime;
  
  char tmp_buffer[256];
  
  SPVM_OBJECT* dump;
  if (object == NULL) {
    SPVM_STRING_BUFFER_add(string_buffer, "undef");
  }
  else {
    int32_t basic_type_id = object->basic_type_id;
    int32_t type_dimension = object->type_dimension;
    
    SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, basic_type_id);
    const char* basic_type_name = SPVM_API_RUNTIME_get_basic_type_name(env->runtime, basic_type->id);
    
    if (SPVM_API_is_string(env, object)) {
      const char* chars = env->get_chars(env, object);
      int32_t chars_length  = env->length(env, object);
      SPVM_STRING_BUFFER_add(string_buffer, "\"");
      SPVM_STRING_BUFFER_add_len(string_buffer, (char*)chars, chars_length);
      SPVM_STRING_BUFFER_add(string_buffer, "\"");
    }
    else if (type_dimension > 0) {
      int32_t array_length = object->length;
      int32_t element_type_dimension = type_dimension - 1;

      SPVM_STRING_BUFFER_add(string_buffer, "[\n");
            
      for (int32_t array_index = 0; array_index < array_length; array_index++) {
    
        for (int32_t depth_index = 0; depth_index < *depth + 1; depth_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
        }

        if (SPVM_API_is_mulnum_array(env, object)) {

          SPVM_STRING_BUFFER_add(string_buffer, "{\n");

          SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, basic_type_id);
          SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id);
          int32_t fields_length = class->fields_length;
          
          for (int32_t field_index = 0; field_index < fields_length; field_index++) {
            for (int32_t depth_index = 0; depth_index < *depth + 2; depth_index++) {
              SPVM_STRING_BUFFER_add(string_buffer, "  ");
            }
            
            SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, class->fields_base_id + field_index);
            
            SPVM_RUNTIME_TYPE* field_type =SPVM_API_RUNTIME_get_type(env->runtime, field->type_id);

            int32_t field_basic_type_id = field_type->basic_type_id;

            const char* field_name = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, field->name_id, NULL);
            SPVM_STRING_BUFFER_add(string_buffer, field_name);
            SPVM_STRING_BUFFER_add(string_buffer, " => ");

            switch (field_basic_type_id) {
              case SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE: {
                int8_t* element = &((int8_t*)((intptr_t)object + env->object_header_byte_size))[array_index * fields_length];
                sprintf(tmp_buffer, "%d", element[field_index]);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT: {
                int16_t* element = &((int16_t*)((intptr_t)object + env->object_header_byte_size))[array_index * fields_length];
                sprintf(tmp_buffer, "%d", element[field_index]);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_ID_INT: {
                int32_t* element = &((int32_t*)((intptr_t)object + env->object_header_byte_size))[array_index * fields_length];
                sprintf(tmp_buffer, "%d", element[field_index]);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_ID_LONG: {
                int64_t* element = &((int64_t*)((intptr_t)object + env->object_header_byte_size))[array_index * fields_length];
                sprintf(tmp_buffer, "%lld", (long long int)element[field_index]);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT: {
                float* element = &((float*)((intptr_t)object + env->object_header_byte_size))[array_index * fields_length];
                sprintf(tmp_buffer, "%g", element[field_index]);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE: {
                double* element = &((double*)((intptr_t)object + env->object_header_byte_size))[array_index * fields_length];
                sprintf(tmp_buffer, "%g", element[field_index]);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
            }
            
            if (field_index == fields_length - 1) {
              SPVM_STRING_BUFFER_add(string_buffer, "\n");
            }
            else {
              SPVM_STRING_BUFFER_add(string_buffer, ",\n");
            }
          }

          SPVM_STRING_BUFFER_add(string_buffer, "  }");
        }
        else if (SPVM_API_is_numeric_array(env, object)) {
          switch (basic_type_id) {
            case SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE: {
              int8_t element = ((int8_t*)((intptr_t)object + env->object_header_byte_size))[array_index];
              sprintf(tmp_buffer, "%d", element);
              SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
              break;
            }
            case SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT: {
              int16_t element = ((int16_t*)((intptr_t)object + env->object_header_byte_size))[array_index];
              sprintf(tmp_buffer, "%d", element);
              SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
              break;
            }
            case SPVM_NATIVE_C_BASIC_TYPE_ID_INT: {
              int32_t element = ((int32_t*)((intptr_t)object + env->object_header_byte_size))[array_index];
              sprintf(tmp_buffer, "%d", element);
              SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
              break;
            }
            case SPVM_NATIVE_C_BASIC_TYPE_ID_LONG: {
              int64_t element = ((int64_t*)((intptr_t)object + env->object_header_byte_size))[array_index];
              sprintf(tmp_buffer, "%lld", (long long int)element);
              SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
              break;
            }
            case SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT: {
              float element = ((float*)((intptr_t)object + env->object_header_byte_size))[array_index];
              sprintf(tmp_buffer, "%g", element);
              SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
              break;
            }
            case SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE: {
              double element = ((double*)((intptr_t)object + env->object_header_byte_size))[array_index];
              sprintf(tmp_buffer, "%g", element);
              SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
              break;
            }
          }
        }
        else if (SPVM_API_is_object_array(env, object)) {
          SPVM_OBJECT* element = (((SPVM_OBJECT**)((intptr_t)object + env->object_header_byte_size))[array_index]);
          (*depth)++;
          SPVM_API_dump_recursive(env, element, depth, string_buffer, address_symtable);
          (*depth)--;
        }
        else {
          assert(0);
        }
        
        if (array_index == array_length - 1) {
          SPVM_STRING_BUFFER_add(string_buffer, "\n");
        }
        else {
          SPVM_STRING_BUFFER_add(string_buffer, ",\n");
        }
      }

      for (int32_t depth_index = 0; depth_index < *depth; depth_index++) {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
      }
      SPVM_STRING_BUFFER_add(string_buffer, "]");

      SPVM_STRING_BUFFER_add(string_buffer, " : ");
      SPVM_STRING_BUFFER_add(string_buffer, basic_type_name);
      for (int32_t type_dimension_index = 0; type_dimension_index < type_dimension; type_dimension_index++) {
        SPVM_STRING_BUFFER_add(string_buffer, "[]");
      }
      
      // If the object is weaken, this get the real address
      sprintf(tmp_buffer, "(%p)", (void*)((intptr_t)object & ~1));
      SPVM_STRING_BUFFER_add(string_buffer, tmp_buffer);
    }
    else {

      // If the object is weaken, this get the real address
      sprintf(tmp_buffer, "%p", (void*)((intptr_t)object & ~1));
      int32_t exists = (int32_t)(intptr_t)SPVM_HASH_get(address_symtable, tmp_buffer, strlen(tmp_buffer));
      if (exists) {
        // If the object is weaken, this get the real address
        sprintf(tmp_buffer, "REUSE_OBJECT(%p)", (void*)((intptr_t)object & ~1));
        SPVM_STRING_BUFFER_add(string_buffer, tmp_buffer);
      }
      else {
        SPVM_HASH_set(address_symtable, tmp_buffer, strlen(tmp_buffer), (void*)(intptr_t)1);
        
        SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, basic_type_id);
        SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id);
        assert(class);

        SPVM_STRING_BUFFER_add(string_buffer, "{\n");
        
        // Free object fields
        int32_t fields_length = class->fields_length;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          for (int32_t depth_index = 0; depth_index < *depth + 1; depth_index++) {
            SPVM_STRING_BUFFER_add(string_buffer, "  ");
          }

          SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, class->fields_base_id + field_index);
          
          SPVM_RUNTIME_TYPE* field_type =SPVM_API_RUNTIME_get_type(env->runtime, field->type_id);
          
          int32_t field_basic_type_id = field_type->basic_type_id;
          int32_t field_type_dimension = field_type->dimension;
          int32_t field_offset = field->offset;
          const char* field_name = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, field->name_id, NULL);
          
          SPVM_STRING_BUFFER_add(string_buffer, field_name);
          SPVM_STRING_BUFFER_add(string_buffer, " => ");
          if (field_type_dimension == 0 && field_basic_type_id >= SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE && field_basic_type_id <= SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE) {
            switch (field_basic_type_id) {
              case SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE: {
                int8_t field_value = *(int8_t*)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
                sprintf(tmp_buffer, "%d", field_value);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT: {
                int16_t field_value = *(int16_t*)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
                sprintf(tmp_buffer, "%d", field_value);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_ID_INT: {
                int32_t field_value = *(int32_t*)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
                sprintf(tmp_buffer, "%d", field_value);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_ID_LONG: {
                int64_t field_value = *(int64_t*)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
                sprintf(tmp_buffer, "%lld", (long long int)field_value);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT: {
                float field_value = *(float*)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
                sprintf(tmp_buffer, "%g", field_value);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE: {
                double field_value = *(double*)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
                sprintf(tmp_buffer, "%g", field_value);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              default : {
                assert(0);
              }
            }
          }
          else  {
            SPVM_OBJECT* field_value = *(SPVM_OBJECT**)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
            (*depth)++;
            SPVM_API_dump_recursive(env, field_value, depth, string_buffer, address_symtable);
            (*depth)--;
          }
          
          if (field_index == fields_length - 1) {
            SPVM_STRING_BUFFER_add(string_buffer, "\n");
          }
          else {
            SPVM_STRING_BUFFER_add(string_buffer, ",\n");
          }
        }

        for (int32_t depth_index = 0; depth_index < *depth; depth_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
        }
        SPVM_STRING_BUFFER_add(string_buffer, "}");

        SPVM_STRING_BUFFER_add(string_buffer, " : ");
        SPVM_STRING_BUFFER_add(string_buffer, basic_type_name);
        sprintf(tmp_buffer, "(%p)", object);
        SPVM_STRING_BUFFER_add(string_buffer, tmp_buffer);
      }
    }
  }
}

const char* SPVM_API_get_field_string_chars_by_name(SPVM_ENV* env, SPVM_OBJECT* obj, const char* class_name, const char* field_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;

  int32_t id = env->get_field_id(env, class_name, field_name, "string");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, type:string", class_name, field_name, file, line);
    return NULL;
  };
  SPVM_OBJECT* value = env->get_field_object(env, obj, id);
  if (value == NULL) {
    return NULL;
  }
  else {
    const char* chars = env->get_chars(env, value);
    return chars;
  }
}

int32_t SPVM_API_call_class_method_by_name(SPVM_ENV* env, const char* class_name, const char* method_name, const char* signature, SPVM_VALUE* stack, const char* file, int32_t line) {
  
  int32_t method_id = env->get_class_method_id(env, class_name, method_name, signature);
  if (method_id < 0) {
    env->die(env, "Method not found, class name:%s, sub name:%s, signature:%s", class_name, method_name, signature, file, line);
    return 1;
  }
  int32_t e = env->call_class_method(env, method_id, stack);
  if (e) {
    const char* message = env->get_chars(env, env->get_exception(env));
    env->die(env, "%s", message, file, line);
    return e;
  }
  
  return 0;
}

int32_t SPVM_API_call_instance_method_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* method_name, const char* signature, SPVM_VALUE* stack, const char* file, int32_t line) {
  
  if (object == NULL) {
    env->die(env, "Object must not be NULL", file, line);
    return 1;
  };

  int32_t method_id = env->get_instance_method_id(env, object, method_name, signature);
  if (method_id < 0) {
    env->die(env, "Method not found, object:%p, sub name:%s, signature:%s", object, method_name, signature, file, line);
    return 1;
  };
  int32_t e = env->call_instance_method(env, method_id, stack);
  
  if (e) {
    const char* message = env->get_chars(env, env->get_exception(env));
    env->die(env, "%s", message, file, line);
    return e;
  }
  
  return 0;
}

void* SPVM_API_new_object_by_name(SPVM_ENV* env, const char* class_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_basic_type_id(env, class_name);
  if (id < 0) {
    env->die(env, "Class \"%s\" not found", class_name, file, line);
    *exception_flag = 1;
    return NULL;
  };
  
  void* object = env->new_object(env, id);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_pointer_by_name(SPVM_ENV* env, const char* class_name, void* pointer, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_basic_type_id(env, class_name);
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "class \"%s\" not found", class_name, file, line);
    return NULL;
  };
  SPVM_OBJECT* object = env->new_pointer(env, id, pointer);
  return object;
}

void SPVM_API_set_field_byte_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, int8_t value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, "byte");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:byte", class_name, field_name, file, line);
    return;
  }
  env->set_field_byte(env, object, id, value);
}

void SPVM_API_set_field_short_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, int16_t value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, "short");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:short", class_name, field_name, file, line);
    return;
  };
  env->set_field_short(env, object, id, value);
}

void SPVM_API_set_field_int_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, int32_t value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, "int");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:int", class_name, field_name, file, line);
    return;
  };
  env->set_field_int(env, object, id, value);
}

void SPVM_API_set_field_long_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, int64_t value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, "long");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:long", class_name, field_name, file, line);
    return;
  };
  env->set_field_long(env, object, id, value);
}

void SPVM_API_set_field_float_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, float value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, "float");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:float", class_name, field_name, file, line);
    return;
  };
  env->set_field_float(env, object, id, value);
}

void SPVM_API_set_field_double_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, double value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, "double");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:double", class_name, field_name, file, line);
    return;
  };
  env->set_field_double(env, object, id, value);
}

void SPVM_API_set_field_object_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, const char* signature, SPVM_OBJECT* value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, signature);
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:%s", class_name, field_name, signature, file, line);
    return;
  };
  env->set_field_object(env, object, id, value);
}

int8_t SPVM_API_get_field_byte_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, "byte");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:byte", class_name, field_name, file, line);
    return 0;
  };
  int8_t value = env->get_field_byte(env, object, id);
  return value;
}

int16_t SPVM_API_get_field_short_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, "short");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:short", class_name, field_name, file, line);
    return 0;
  };
  int16_t value = env->get_field_short(env, object, id);
  return value;
}

int32_t SPVM_API_get_field_int_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, "int");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:int", class_name, field_name, file, line);
    return 0;
  };
  int32_t value = env->get_field_int(env, object, id);
  return value;
}

int64_t SPVM_API_get_field_long_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, "long");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:long", class_name, field_name, file, line);
    return 0;
  };
  int64_t value = env->get_field_long(env, object, id);
  return value;
}

float SPVM_API_get_field_float_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, "float");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:float", class_name, field_name, file, line);
    return 0;
  };
  float value = env->get_field_float(env, object, id);
  return value;
}

double SPVM_API_get_field_double_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, "double");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:double", class_name, field_name, file, line);
    return 0;
  };
  double value = env->get_field_double(env, object, id);
  return value;
}

SPVM_OBJECT* SPVM_API_get_field_object_by_name(SPVM_ENV* env, SPVM_OBJECT* object, const char* class_name, const char* field_name, const char* signature, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_field_id(env, class_name, field_name, signature);
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "field not found, class name:%s, field name:%s, signature:%s", class_name, field_name, signature, file, line);
    return NULL;
  };
  SPVM_OBJECT* value = env->get_field_object(env, object, id);
  return value;
}

void SPVM_API_set_class_var_byte_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, int8_t value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, "byte");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:byte", class_name, class_var_name, file, line);
    return;
  };
  env->set_class_var_byte(env, id, value);
}

void SPVM_API_set_class_var_short_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, int16_t value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, "short");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:byte", class_name, class_var_name, file, line);
    return;
  };
  env->set_class_var_short(env, id, value);
}

void SPVM_API_set_class_var_int_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, int32_t value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, "int");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:byte", class_name, class_var_name, file, line);
    return;
  };
  env->set_class_var_int(env, id, value);
}

void SPVM_API_set_class_var_long_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, int64_t value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, "long");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:long", class_name, class_var_name, file, line);
    return;
  };
  env->set_class_var_long(env, id, value);
}

void SPVM_API_set_class_var_float_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, float value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, "float");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:float", class_name, class_var_name, file, line);
    return;
  };
  env->set_class_var_float(env, id, value);
}

void SPVM_API_set_class_var_double_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, double value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, "double");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:double", class_name, class_var_name, file, line);
    return;
  };
  env->set_class_var_double(env, id, value);
}

void SPVM_API_set_class_var_object_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, const char* type_signature, SPVM_OBJECT* value, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, type_signature);
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:%s", class_name, class_var_name, type_signature, file, line);
    return;
  };
  env->set_class_var_object(env, id, value);
}

int8_t SPVM_API_get_class_var_byte_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, "byte");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:byte", class_name, class_var_name, file, line);
    return 0;
  };
  int8_t value = env->get_class_var_byte(env, id);
  return value;
}

int16_t SPVM_API_get_class_var_short_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, "short");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:short", class_name, class_var_name, file, line);
    return 0;
  };
  int16_t value = env->get_class_var_short(env, id);
  return value;
}

int32_t SPVM_API_get_class_var_int_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, "int");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:int", class_name, class_var_name, file, line);
    return 0;
  };
  int32_t value = env->get_class_var_int(env, id);
  return value;
}

int64_t SPVM_API_get_class_var_long_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, "long");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:long", class_name, class_var_name, file, line);
    return 0;
  };
  int64_t value = env->get_class_var_long(env, id);
  return value;
}

float SPVM_API_get_class_var_float_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, "float");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:float", class_name, class_var_name, file, line);
    return 0;
  };
  float value = env->get_class_var_float(env, id);
  return value;
}

double SPVM_API_get_class_var_double_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, "double");
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:double", class_name, class_var_name, file, line);
    return 0;
  };
  double value = env->get_class_var_double(env, id);
  return value;
}

SPVM_OBJECT* SPVM_API_get_class_var_object_by_name(SPVM_ENV* env, const char* class_name, const char* class_var_name, const char* type_signature, int32_t* exception_flag, const char* file, int32_t line) {
  *exception_flag = 0;
  
  int32_t id = env->get_class_var_id(env, class_name, class_var_name, type_signature);
  if (id < 0) {
    *exception_flag = 1;
    env->die(env, "Class variable not defined. Class name:%s, Variable name:%s, Type:%s", class_name, class_var_name, type_signature, file, line);
    return NULL;
  };
  SPVM_OBJECT* value = env->get_class_var_object(env, id);
  return value;
}

int32_t SPVM_API_die(SPVM_ENV* env, const char* message, ...) {
  
  va_list args;
  
  char* message_with_line = (char*)env->alloc_memory_block_zero(env, 512);
  int32_t message_length = strlen(message);
  if (message_length > 255) {
    message_length = 255;
  }
  memcpy(message_with_line, message, message_length);
  const char* file_line = " at %s line %d";
  memcpy(message_with_line + message_length, file_line, strlen(file_line));

  char* buffer = (char*)env->alloc_memory_block_zero(env, 512);
  va_start(args, message);
  vsnprintf(buffer, 511, message_with_line, args);
  va_end(args);
  
  void* exception = env->new_string_raw(env, buffer, strlen(buffer));
  
  env->free_memory_block(env, message_with_line);
  message_with_line = NULL;
  
  env->free_memory_block(env, buffer);
  buffer = NULL;
  
  env->set_exception(env, exception);
  
  return 1;
}

int32_t SPVM_API_remove_mortal(SPVM_ENV* env, int32_t original_mortal_stack_top, SPVM_OBJECT* remove_object) {
  (void)env;
  
  int32_t remove_count = 0;
  if (remove_object != NULL) {
    int32_t match_mortal_stack_index = -1;
    for (int32_t mortal_stack_index = original_mortal_stack_top; mortal_stack_index < (intptr_t)env->native_mortal_stack_top; mortal_stack_index++) {
      SPVM_OBJECT* object = ((SPVM_OBJECT**)(env->native_mortal_stack))[mortal_stack_index];
      
      if (remove_object == object) {
        remove_count++;
        match_mortal_stack_index = mortal_stack_index;
        SPVM_API_dec_ref_count(env, object);
      }
    }
    
    if (remove_count) {
      for (int32_t mortal_stack_index = match_mortal_stack_index; mortal_stack_index < (intptr_t)env->native_mortal_stack_top; mortal_stack_index++) {
        ((SPVM_OBJECT**)(env->native_mortal_stack))[mortal_stack_index] = ((SPVM_OBJECT**)(env->native_mortal_stack))[mortal_stack_index + 1];
      }
      env->native_mortal_stack_top = (void*)((intptr_t)env->native_mortal_stack_top - remove_count);
    }
  }
  return remove_count;
}

void SPVM_API_cleanup_global_vars(SPVM_ENV* env) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  assert(runtime);
  
  // Free exception
  SPVM_API_set_exception(env, NULL);
  
  // Free objects of class variables
  for (int32_t class_var_id = 0; class_var_id < runtime->class_vars_length; class_var_id++) {
    SPVM_RUNTIME_CLASS_VAR* class_var = SPVM_API_RUNTIME_get_class_var(env->runtime, class_var_id);

    int32_t class_var_type_is_object = SPVM_API_RUNTIME_get_type_is_object(env->runtime, class_var->type_id);
    if (class_var_type_is_object) {
      SPVM_OBJECT* object = *(void**)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id];
      if (object) {
        SPVM_API_dec_ref_count(env, object);
      }
    }
  }
}

void SPVM_API_free_env_raw(SPVM_ENV* env) {

  // Free class variables heap
  if (env->class_vars_heap != NULL) {
    free(env->class_vars_heap);
    env->class_vars_heap = NULL;
  }
  
  // Free mortal stack
  if (env->native_mortal_stack != NULL) {
    free(env->native_mortal_stack);
    env->native_mortal_stack = NULL;
  }
  
  // Free env api
  free(env->api->allocator);
  free(env->api->string_buffer);
  free(env->api->compiler);
  free(env->api->precompile);
  free(env->api->runtime);
  free(env->api);
  
  // Free env
  free(env);
  env = NULL;
}

int32_t SPVM_API_call_spvm_method(SPVM_ENV* env, int32_t method_id, SPVM_VALUE* stack) {
  (void)env;
  
  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Method
  SPVM_RUNTIME_METHOD* method = SPVM_API_RUNTIME_get_method(env->runtime, method_id);
  
  int32_t exception_flag;
  
  // Call native method
  if (method->flag & SPVM_METHOD_C_FLAG_NATIVE) {
    // Enter scope
    int32_t original_mortal_stack_top = SPVM_API_enter_scope(env);

    // Call native subrotuine
    int32_t (*native_address)(SPVM_ENV*, SPVM_VALUE*) = runtime->method_native_addresses[method->id];
    assert(native_address != NULL);
    exception_flag = (*native_address)(env, stack);
    
    int32_t method_return_type_is_object = SPVM_API_RUNTIME_get_type_is_object(env->runtime, method->return_type_id);
    
    // Increment ref count of return value
    if (!exception_flag) {
      if (method_return_type_is_object) {
        if (*(void**)&stack[0] != NULL) {
          SPVM_API_INC_REF_COUNT_ONLY(*(void**)&stack[0]);
        }
      }
    }

    // Leave scope
    SPVM_API_leave_scope(env, original_mortal_stack_top);

    // Decrement ref count of return value
    if (!exception_flag) {
      if (method_return_type_is_object) {
        if (*(void**)&stack[0] != NULL) {
          SPVM_API_DEC_REF_COUNT_ONLY(*(void**)&stack[0]);
        }
      }
    }
    
    // Set default exception message
    if (exception_flag && env->exception_object == NULL) {
      void* exception = env->new_string_nolen_raw(env, "Error");
      env->set_exception(env, exception);
    }
  }
  else {
    // Call precompiled method
    void* method_precompile_address = runtime->method_precompile_addresses[method->id];
    if (method_precompile_address) {
      int32_t (*precompile_address)(SPVM_ENV*, SPVM_VALUE*) = method_precompile_address;
      exception_flag = (*precompile_address)(env, stack);
    }
    // Call sub virtual machine
    else {
      exception_flag = SPVM_API_call_spvm_method_vm(env, method_id, stack);
    }
  }
  
  return exception_flag;
}

int32_t SPVM_API_call_spvm_method_vm(SPVM_ENV* env, int32_t method_id, SPVM_VALUE* stack) {
  (void)env;

  // Opcode relative index
  register int32_t opcode_rel_index = 0;
  
  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;

  // Runtime method
  SPVM_RUNTIME_METHOD* method = SPVM_API_RUNTIME_get_method(env->runtime, method_id);
  
  // Runtime class
  SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, method->class_id);

  // Operation codes
  SPVM_OPCODE* opcodes = runtime->opcodes;

  // Exception flag
  int32_t exception_flag = 0;

  // Operation code base
  int32_t method_opcodes_base_id = method->opcodes_base_id;

  // Call method argument stack top
  int32_t stack_index = 0;
  
  // Mortal stack
  uint16_t* mortal_stack = NULL;
  int32_t mortal_stack_top = 0;
  
  // object variables
  void** object_vars = NULL;
  
  // ref variables
  void** ref_vars = NULL;
  
  // double variables
  double* double_vars = NULL;
  
  // float variables
  float* float_vars = NULL;

  // long variables
  int64_t* long_vars = NULL;

  // int variables
  int32_t* int_vars = NULL;

  // short variables
  int16_t* short_vars = NULL;
  
  // byte variables
  int8_t* byte_vars = NULL;
  
  // Alloc variable memory
  // Allignment is 8. This is numeric type max byte size
  // Order 8, 4, 2, 1 numeric variable, and addrress variables
  char* call_stack = NULL;
  {
    // Numeric area byte size
    int32_t numeric_vars_byte_size = 0;
    numeric_vars_byte_size += method->call_stack_long_vars_length * 8;
    numeric_vars_byte_size += method->call_stack_double_vars_length * 8;
    numeric_vars_byte_size += method->call_stack_int_vars_length * 4;
    numeric_vars_byte_size += method->call_stack_float_vars_length * 4;
    numeric_vars_byte_size += method->call_stack_short_vars_length * 2;
    numeric_vars_byte_size += method->mortal_stack_length * 2;
    numeric_vars_byte_size += method->call_stack_byte_vars_length * 1;
    
    if (numeric_vars_byte_size % 8 != 0) {
      numeric_vars_byte_size += (8 - (numeric_vars_byte_size % 8));
    }
    
    // Address area byte size
    int32_t address_vars_byte_size = 0;
    address_vars_byte_size += method->call_stack_object_vars_length * sizeof(void*);
    address_vars_byte_size += method->call_stack_ref_vars_length * sizeof(void*);
    
    // Total area byte size
    int32_t total_vars_byte_size = numeric_vars_byte_size + address_vars_byte_size;
    
    call_stack = SPVM_API_alloc_memory_block_zero(env, total_vars_byte_size + 1);
    if (call_stack == NULL) {
      void* exception = env->new_string_nolen_raw(env, "Can't alloc call stack memory");
      env->set_exception(env, exception);
      exception_flag = 1;
      return exception_flag;
    }

    int32_t call_stack_offset = 0;
    
    // Double variables
    double_vars = (double*)&call_stack[call_stack_offset];
    call_stack_offset += method->call_stack_double_vars_length * 8;
    
    // Long varialbes
    long_vars = (int64_t*)&call_stack[call_stack_offset];
    call_stack_offset += method->call_stack_long_vars_length * 8;
    
    // Float variables
    float_vars = (float*)&call_stack[call_stack_offset];
    call_stack_offset += method->call_stack_float_vars_length * 4;
    
    // Int variables
    int_vars = (int32_t*)&call_stack[call_stack_offset];
    call_stack_offset += method->call_stack_int_vars_length * 4;

    // Short variables
    short_vars = (int16_t*)&call_stack[call_stack_offset];
    call_stack_offset += method->call_stack_short_vars_length * 2;

    // Mortal stack
    mortal_stack = (uint16_t*)&call_stack[call_stack_offset];
    call_stack_offset += method->mortal_stack_length * 2;
    
    // Byte variables
    byte_vars = (int8_t*)&call_stack[call_stack_offset];
    call_stack_offset += method->call_stack_byte_vars_length * 1;
    
    call_stack_offset = numeric_vars_byte_size;

    // Object variables
    object_vars = (void**)&call_stack[call_stack_offset];
    call_stack_offset += method->call_stack_object_vars_length * sizeof(void*);
    
    // Refernce variables
    ref_vars = (void**)&call_stack[call_stack_offset];
    call_stack_offset += method->call_stack_ref_vars_length * sizeof(void*);
  }

  // Buffer for string convertion
  // double need 17 digit
  // int64_t need 21 gidit (-9223372036854775808 + (null character))
  char tmp_buffer[256];
  
  int32_t object_header_byte_size = (intptr_t)env->object_header_byte_size;

  // Execute operation codes
  while (1) {
    SPVM_OPCODE* opcode = &(opcodes[method_opcodes_base_id + opcode_rel_index]);
    
    int32_t opcode_id = opcode->id;
    
    switch (opcode_id) {
      case SPVM_OPCODE_C_ID_GET_ARG_BYTE: {
        byte_vars[opcode->operand1] = *(int8_t*)&stack[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_SHORT: {
        short_vars[opcode->operand1] = *(int16_t*)&stack[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_INT: {
        int_vars[opcode->operand1] = *(int32_t*)&stack[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_LONG: {
        long_vars[opcode->operand1] = *(int64_t*)&stack[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_FLOAT: {
        float_vars[opcode->operand1] = *(float*)&stack[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_DOUBLE: {
        double_vars[opcode->operand1] = *(double*)&stack[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_OBJECT: {
        object_vars[opcode->operand1] = *(void**)&stack[opcode->operand2];
        void* object = *(void**)&object_vars[opcode->operand1];
        if (object != NULL) {
          SPVM_API_INC_REF_COUNT_ONLY(object);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_REF: {
        ref_vars[opcode->operand1] = *(void**)&stack[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_MULNUM_BYTE: {
        int32_t type_width = opcode->operand3;
        for (int32_t field_index = 0; field_index < type_width; field_index++) {
          byte_vars[opcode->operand1 + field_index] = *(int8_t*)&stack[opcode->operand2 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_MULNUM_SHORT: {
        int32_t type_width = opcode->operand3;
        for (int32_t field_index = 0; field_index < type_width; field_index++) {
          short_vars[opcode->operand1 + field_index] = *(int16_t*)&stack[opcode->operand2 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_MULNUM_INT: {
        int32_t type_width = opcode->operand3;
        for (int32_t field_index = 0; field_index < type_width; field_index++) {
          int_vars[opcode->operand1 + field_index] = *(int32_t*)&stack[opcode->operand2 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_MULNUM_LONG: {
        int32_t type_width = opcode->operand3;
        for (int32_t field_index = 0; field_index < type_width; field_index++) {
          long_vars[opcode->operand1 + field_index] = *(int64_t*)&stack[opcode->operand2 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_MULNUM_FLOAT: {
        int32_t type_width = opcode->operand3;
        for (int32_t field_index = 0; field_index < type_width; field_index++) {
          float_vars[opcode->operand1 + field_index] = *(float*)&stack[opcode->operand2 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_MULNUM_DOUBLE: {
        int32_t type_width = opcode->operand3;
        for (int32_t field_index = 0; field_index < type_width; field_index++) {
          double_vars[opcode->operand1 + field_index] = *(double*)&stack[opcode->operand2 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_INT: {
        int_vars[0] = int_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_LONG: {
        int_vars[0] = !!long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_FLOAT: {
        int_vars[0] = !!float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_DOUBLE: {
        int_vars[0] = !!double_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_BOOL_OBJECT: {
        int_vars[0] = !!env->get_bool_object_value(env, *(void**)&object_vars[opcode->operand1]);
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_OBJECT: {
        int_vars[0] = !!*(void**)&object_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_IS_UNDEF: {
        int_vars[0] = *(void**)&object_vars[opcode->operand1] == NULL;
        break;
      }
      case SPVM_OPCODE_C_ID_IS_NOT_UNDEF: {
        int_vars[0] = *(void**)&object_vars[opcode->operand1] != NULL;
        break;
      }
      case SPVM_OPCODE_C_ID_EQ_INT: {
        int_vars[0] = int_vars[opcode->operand1] == int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_EQ_LONG: {
        int_vars[0] = long_vars[opcode->operand1] == long_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_EQ_FLOAT: {
        int_vars[0] = float_vars[opcode->operand1] == float_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_EQ_DOUBLE: {
        int_vars[0] = double_vars[opcode->operand1] == double_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_EQ_OBJECT: {
        int_vars[0] = *(void**)&object_vars[opcode->operand1] == *(void**)&object_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_NE_INT: {
        int_vars[0] = int_vars[opcode->operand1] != int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_NE_LONG: {
        int_vars[0] = long_vars[opcode->operand1] != long_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_NE_FLOAT: {
        int_vars[0] = float_vars[opcode->operand1] != float_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_NE_DOUBLE: {
        int_vars[0] = double_vars[opcode->operand1] != double_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_NE_OBJECT: {
        int_vars[0] = *(void**)&object_vars[opcode->operand1] != *(void**)&object_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GT_INT: {
        int_vars[0] = int_vars[opcode->operand1] > int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GT_LONG: {
        int_vars[0] = long_vars[opcode->operand1] > long_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GT_FLOAT: {
        int_vars[0] = float_vars[opcode->operand1] > float_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GT_DOUBLE: {
        int_vars[0] = double_vars[opcode->operand1] > double_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GE_INT: {
        int_vars[0] = int_vars[opcode->operand1] >= int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GE_LONG: {
        int_vars[0] = long_vars[opcode->operand1] >= long_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GE_FLOAT: {
        int_vars[0] = float_vars[opcode->operand1] >= float_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_GE_DOUBLE: {
        int_vars[0] = double_vars[opcode->operand1] >= double_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_LT_INT: {
        int_vars[0] = int_vars[opcode->operand1] < int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_LT_LONG: {
        int_vars[0] = long_vars[opcode->operand1] < long_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_LT_FLOAT: {
        int_vars[0] = float_vars[opcode->operand1] < float_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_LT_DOUBLE: {
        int_vars[0] = double_vars[opcode->operand1] < double_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_LE_INT: {
        int_vars[0] = int_vars[opcode->operand1] <= int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_LE_LONG: {
        int_vars[0] = long_vars[opcode->operand1] <= long_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_LE_FLOAT: {
        int_vars[0] = float_vars[opcode->operand1] <= float_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_LE_DOUBLE: {
        int_vars[0] = double_vars[opcode->operand1] <= double_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_CMP_INT: {
        int_vars[0] = int_vars[opcode->operand1] > int_vars[opcode->operand2] ? 1
                    : int_vars[opcode->operand1] < int_vars[opcode->operand2] ? -1
                    : 0;
        break;
      }
      case SPVM_OPCODE_C_ID_CMP_LONG: {
        int_vars[0] = long_vars[opcode->operand1] > long_vars[opcode->operand2] ? 1
                    : long_vars[opcode->operand1] < long_vars[opcode->operand2] ? -1
                    : 0;
        break;
      }
      case SPVM_OPCODE_C_ID_CMP_FLOAT: {
        int_vars[0] = float_vars[opcode->operand1] > float_vars[opcode->operand2] ? 1
                    : float_vars[opcode->operand1] < float_vars[opcode->operand2] ? -1
                    : 0;
        break;
      }
      case SPVM_OPCODE_C_ID_CMP_DOUBLE: {
        int_vars[0] = double_vars[opcode->operand1] > double_vars[opcode->operand2] ? 1
                    : double_vars[opcode->operand1] < double_vars[opcode->operand2] ? -1
                    : 0;
        break;
      }
      case SPVM_OPCODE_C_ID_IS_TYPE: {
        void* object = *(void**)&object_vars[opcode->operand1];
        int32_t check_basic_type_id = opcode->operand2;
        int32_t check_type_dimension = opcode->operand3;

        if (object) {
          int_vars[0] = env->is_type(env, object, check_basic_type_id, check_type_dimension);
        }
        else {
          int_vars[0] = 0;
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_HAS_CALLBACK: {
        void* object = *(void**)&object_vars[opcode->operand1];
        int32_t callback_basic_type_id = opcode->operand2;
        
        if (object) {
          int_vars[0] = env->has_callback(env, object, callback_basic_type_id);
        }
        else {
          int_vars[0] = 0;
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_HAS_INTERFACE: {
        void* object = *(void**)&object_vars[opcode->operand1];
        int32_t callback_basic_type_id = opcode->operand2;
        
        if (object) {
          int_vars[0] = env->has_interface(env, object, callback_basic_type_id);
        }
        else {
          int_vars[0] = 0;
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_STRING_EQ:
      case SPVM_OPCODE_C_ID_STRING_NE:
      case SPVM_OPCODE_C_ID_STRING_GT:
      case SPVM_OPCODE_C_ID_STRING_GE:
      case SPVM_OPCODE_C_ID_STRING_LT:
      case SPVM_OPCODE_C_ID_STRING_LE:
      case SPVM_OPCODE_C_ID_STRING_CMP:
      {
        void* object1 = *(void**)&object_vars[opcode->operand1];
        void* object2 = *(void**)&object_vars[opcode->operand2];
        
        if (object1 == NULL && object2 == NULL) {
         switch (opcode_id) {
            case SPVM_OPCODE_C_ID_STRING_EQ: {
              int_vars[0] = 1;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_NE: {
              int_vars[0] = 0;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_GT: {
              int_vars[0] = 0;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_GE: {
              int_vars[0] = 1;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_LT: {
              int_vars[0] = 0;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_LE: {
              int_vars[0] = 1;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_CMP: {
              int_vars[0] = 0;
              break;
            }
          }
        }
        else if (object1 != NULL && object2 == NULL) {
          switch (opcode_id) {
            case SPVM_OPCODE_C_ID_STRING_EQ: {
              int_vars[0] = 0;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_NE: {
              int_vars[0] = 1;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_GT: {
              int_vars[0] = 1;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_GE: {
              int_vars[0] = 1;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_LT: {
              int_vars[0] = 0;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_LE: {
              int_vars[0] = 0;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_CMP: {
              int_vars[0] = 1;
              break;
            }
          }
        }
        else if (object1 == NULL && object2 != NULL) {
          switch (opcode_id) {
            case SPVM_OPCODE_C_ID_STRING_EQ: {
              int_vars[0] = 0;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_NE: {
              int_vars[0] = 1;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_GT: {
              int_vars[0] = 0;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_GE: {
              int_vars[0] = 0;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_LT: {
              int_vars[0] = 1;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_LE: {
              int_vars[0] = 1;
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_CMP: {
              int_vars[0] = -1;
              break;
            }
          }
        }
        else {
          int32_t length1 = *(int32_t*)((intptr_t)object1 + (intptr_t)env->object_length_offset);
          int32_t length2 = *(int32_t*)((intptr_t)object2 + (intptr_t)env->object_length_offset);
          
          const char* bytes1 = env->get_chars(env, object1);
          const char* bytes2 = env->get_chars(env, object2);
          
          int32_t short_string_length = length1 < length2 ? length1 : length2;
          int32_t retval = memcmp(bytes1, bytes2, short_string_length);
          int32_t cmp;
          if (retval) {
            cmp = retval < 0 ? -1 : 1;
          } else if (length1 == length2) {
            cmp = 0;
          } else {
            cmp = length1 < length2 ? -1 : 1;
          }
          
          switch (opcode_id) {
            case SPVM_OPCODE_C_ID_STRING_EQ: {
              int_vars[0] = (cmp == 0);
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_NE: {
              int_vars[0] = (cmp != 0);
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_GT: {
              int_vars[0] = (cmp == 1);
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_GE: {
              int_vars[0] = (cmp >= 0);
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_LT: {
              int_vars[0] = (cmp == -1);
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_LE: {
              int_vars[0] = (cmp <= 0);
              break;
            }
            case SPVM_OPCODE_C_ID_STRING_CMP: {
              int_vars[0] = cmp;
              break;
            }
          }
        }

        break;
      }
      case SPVM_OPCODE_C_ID_ADD_INT: {
        int_vars[opcode->operand0] = int_vars[opcode->operand1] + int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_ADD_LONG: {
        long_vars[opcode->operand0] = long_vars[opcode->operand1] + long_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_ADD_FLOAT: {
        float_vars[opcode->operand0] = float_vars[opcode->operand1] + float_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_ADD_DOUBLE: {
        double_vars[opcode->operand0] = double_vars[opcode->operand1] + double_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_SUBTRACT_INT: {
        int_vars[opcode->operand0] = int_vars[opcode->operand1] - int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_SUBTRACT_LONG: {
        long_vars[opcode->operand0] = long_vars[opcode->operand1] - long_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_SUBTRACT_FLOAT: {
        float_vars[opcode->operand0] = float_vars[opcode->operand1] - float_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_SUBTRACT_DOUBLE: {
        double_vars[opcode->operand0] = double_vars[opcode->operand1] - double_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_MULTIPLY_INT: {
        int_vars[opcode->operand0] = int_vars[opcode->operand1] * int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_MULTIPLY_LONG: {
        long_vars[opcode->operand0] = long_vars[opcode->operand1] * long_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_MULTIPLY_FLOAT: {
        float_vars[opcode->operand0] = float_vars[opcode->operand1] * float_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_MULTIPLY_DOUBLE: {
        double_vars[opcode->operand0] = double_vars[opcode->operand1] * double_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_DIVIDE_INT: {
        if (__builtin_expect(int_vars[opcode->operand2] == 0, 0)) {
          void* exception = env->new_string_nolen_raw(env, "0 division");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int_vars[opcode->operand0]
            = int_vars[opcode->operand1] / int_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_DIVIDE_LONG: {
        if (__builtin_expect(long_vars[opcode->operand2] == 0, 0)) {
          void* exception = env->new_string_nolen_raw(env, "0 division");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          long_vars[opcode->operand0]
            = long_vars[opcode->operand1] / long_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_DIVIDE_FLOAT: {
        float_vars[opcode->operand0]
          = float_vars[opcode->operand1] / float_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_DIVIDE_DOUBLE: {
        double_vars[opcode->operand0]
          = double_vars[opcode->operand1] / double_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_DIVIDE_UNSIGNED_INT: {
        if (__builtin_expect(int_vars[opcode->operand2] == 0, 0)) {
          void* exception = env->new_string_nolen_raw(env, "0 division");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int_vars[opcode->operand0]
            = (uint32_t)int_vars[opcode->operand1] / (uint32_t)int_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_DIVIDE_UNSIGNED_LONG: {
        if (__builtin_expect(long_vars[opcode->operand2] == 0, 0)) {
          void* exception = env->new_string_nolen_raw(env, "0 division");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          long_vars[opcode->operand0]
            = (uint64_t)long_vars[opcode->operand1] / (uint64_t)long_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_REMAINDER_INT: {
        if (__builtin_expect(int_vars[opcode->operand2] == 0, 0)) {
          void* exception = env->new_string_nolen_raw(env, "0 division");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int_vars[opcode->operand0] = int_vars[opcode->operand1] % int_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_REMAINDER_LONG: {
        if (__builtin_expect(long_vars[opcode->operand2] == 0, 0)) {
          void* exception = env->new_string_nolen_raw(env, "0 division");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          long_vars[opcode->operand0] = long_vars[opcode->operand1] % long_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_REMAINDER_UNSIGNED_INT: {
        if (__builtin_expect(int_vars[opcode->operand2] == 0, 0)) {
          void* exception = env->new_string_nolen_raw(env, "0 division");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int_vars[opcode->operand0] = (uint32_t)int_vars[opcode->operand1] % (uint32_t)int_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_REMAINDER_UNSIGNED_LONG: {
        if (__builtin_expect(long_vars[opcode->operand2] == 0, 0)) {
          void* exception = env->new_string_nolen_raw(env, "0 division");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          long_vars[opcode->operand0] = (uint64_t)long_vars[opcode->operand1] % (uint64_t)long_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_LEFT_SHIFT_INT: {
        int_vars[opcode->operand0]
          = int_vars[opcode->operand1] << int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_LEFT_SHIFT_LONG: {
        long_vars[opcode->operand0]
          = long_vars[opcode->operand1] << int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_RIGHT_ARITHMETIC_SHIFT_INT: {
        int_vars[opcode->operand0]
          = int_vars[opcode->operand1] >> int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_RIGHT_ARITHMETIC_SHIFT_LONG: {
        long_vars[opcode->operand0]
          = long_vars[opcode->operand1] >> int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_RIGHT_LOGICAL_SHIFT_INT: {
        int_vars[opcode->operand0]
          = (int32_t)((uint32_t)int_vars[opcode->operand1] >> int_vars[opcode->operand2]);
        break;
      }
      case SPVM_OPCODE_C_ID_RIGHT_LOGICAL_SHIFT_LONG: {
        long_vars[opcode->operand0]
          = (int64_t)((uint64_t)long_vars[opcode->operand1] >> int_vars[opcode->operand2]);
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_AND_INT: {
        int_vars[opcode->operand0]
          = int_vars[opcode->operand1] & int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_AND_LONG: {
        long_vars[opcode->operand0]
          = long_vars[opcode->operand1] & long_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_OR_INT: {
        int_vars[opcode->operand0]
          = int_vars[opcode->operand1] | int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_OR_LONG: {
        long_vars[opcode->operand0]
          = long_vars[opcode->operand1] | long_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_XOR_INT: {
        int_vars[opcode->operand0]
          = int_vars[opcode->operand1] ^ int_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_XOR_LONG: {
        long_vars[opcode->operand0]
          = long_vars[opcode->operand1] ^ long_vars[opcode->operand2];
        break;
      }
      case SPVM_OPCODE_C_ID_NEGATE_INT: {
        int_vars[opcode->operand0] = -int_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_NEGATE_LONG: {
        long_vars[opcode->operand0] = -long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_NEGATE_FLOAT: {
        float_vars[opcode->operand0] = -float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_NEGATE_DOUBLE: {
        double_vars[opcode->operand0] = -double_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_NOT_INT: {
        int_vars[opcode->operand0] = ~int_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_NOT_LONG: {
        long_vars[opcode->operand0] = ~long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_LONG: {
        long_vars[opcode->operand0] = (int64_t)int_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_FLOAT: {
        float_vars[opcode->operand0] = (float)int_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_DOUBLE: {
        double_vars[opcode->operand0] = (double)int_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_INT: {
        int_vars[opcode->operand0] = (int32_t)long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_FLOAT: {
        float_vars[opcode->operand0] = (float)long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_DOUBLE: {
        double_vars[opcode->operand0] = (double)long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_INT: {
        int_vars[opcode->operand0] = (int32_t)float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_LONG: {
        long_vars[opcode->operand0] = (int64_t)float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_DOUBLE: {
        double_vars[opcode->operand0] = (double)float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_INT: {
        int_vars[opcode->operand0] = (int32_t)double_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_LONG: {
        long_vars[opcode->operand0] = (int64_t)double_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_FLOAT: {
        float_vars[opcode->operand0] = (float)double_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_BYTE: {
        byte_vars[opcode->operand0] = (int8_t)(int_vars[opcode->operand1]);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_SHORT: {
        short_vars[opcode->operand0] = (int16_t)(int_vars[opcode->operand1]);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_INT: {
        int_vars[opcode->operand0] = (int32_t)byte_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_INT: {
        int_vars[opcode->operand0] = (int32_t)short_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_LONG: {
        long_vars[opcode->operand0] = (int64_t)byte_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_FLOAT: {
        float_vars[opcode->operand0] = (float)byte_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_DOUBLE: {
        double_vars[opcode->operand0] = (double)byte_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_BYTE: {
        byte_vars[opcode->operand0] = (int8_t)short_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_LONG: {
        long_vars[opcode->operand0] = (int64_t)short_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_FLOAT: {
        float_vars[opcode->operand0] = (float)short_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_DOUBLE: {
        double_vars[opcode->operand0] = (double)short_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_BYTE: {
        byte_vars[opcode->operand0] = (int8_t)long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_SHORT: {
        short_vars[opcode->operand0] = (int16_t)long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_BYTE: {
        byte_vars[opcode->operand0] = (int8_t)float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_SHORT: {
        short_vars[opcode->operand0] = (int16_t)float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_BYTE: {
        byte_vars[opcode->operand0] = (int8_t)double_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_SHORT: {
        short_vars[opcode->operand0] = (int16_t)double_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_SHORT: {
        short_vars[opcode->operand0] = (int16_t)byte_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_STRING: {
        sprintf(tmp_buffer, "%" PRId8, byte_vars[opcode->operand1]);
        int32_t string_length = strlen(tmp_buffer);
        void* string = env->new_string_raw(env, tmp_buffer, string_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_STRING_TO_BYTE_ARRAY: {
        void* src_string = object_vars[opcode->operand1];
        int32_t src_string_length = env->length(env, src_string);
        const char* src_string_data = env->get_chars(env, src_string);
        void* byte_array = env->new_byte_array_raw(env, src_string_length);
        int8_t* byte_array_data = env->get_elems_byte(env, byte_array);
        memcpy(byte_array_data, src_string_data, src_string_length);
        
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], byte_array);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_ARRAY_TO_STRING: {
        void* src_byte_array = object_vars[opcode->operand1];
        int32_t src_byte_array_length = env->length(env, src_byte_array);
        int8_t* src_byte_array_data = env->get_elems_byte(env, src_byte_array);
        void* string = env->new_string_raw(env, (const char*)src_byte_array_data, src_byte_array_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_STRING: {
        sprintf(tmp_buffer, "%" PRId16, short_vars[opcode->operand1]);
        int32_t string_length = strlen(tmp_buffer);
        void* string = env->new_string_raw(env, tmp_buffer, string_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_STRING: {
        sprintf(tmp_buffer, "%" PRId32, int_vars[opcode->operand1]);
        int32_t string_length = strlen(tmp_buffer);
        void* string = env->new_string_raw(env, tmp_buffer, string_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_STRING: {
        sprintf(tmp_buffer, "%" PRId64, long_vars[opcode->operand1]);
        int32_t string_length = strlen(tmp_buffer);
        void* string = env->new_string_raw(env, tmp_buffer, string_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_STRING: {
        sprintf(tmp_buffer, "%g", float_vars[opcode->operand1]);
        int32_t string_length = strlen(tmp_buffer);
        void* string = env->new_string_raw(env, tmp_buffer, string_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_STRING: {
        sprintf(tmp_buffer, "%g", double_vars[opcode->operand1]);
        int32_t string_length = strlen(tmp_buffer);
        void* string = env->new_string_raw(env, tmp_buffer, string_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_BYTE:
        byte_vars[opcode->operand0] = 0;
        break;
      case SPVM_OPCODE_C_ID_INIT_SHORT:
        short_vars[opcode->operand0] = 0;
        break;
      case SPVM_OPCODE_C_ID_INIT_INT:
        int_vars[opcode->operand0] = 0;
        break;
      case SPVM_OPCODE_C_ID_INIT_LONG: {
        long_vars[opcode->operand0] = 0;
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_FLOAT: {
        float_vars[opcode->operand0] = 0;
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_DOUBLE: {
        double_vars[opcode->operand0] = 0;
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_UNDEF:
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], NULL);
        break;
      case SPVM_OPCODE_C_ID_INIT_MULNUM_BYTE: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          byte_vars[opcode->operand0 + field_index] = 0;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_MULNUM_SHORT: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          short_vars[opcode->operand0 + field_index] = 0;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_MULNUM_INT: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          int_vars[opcode->operand0 + field_index] = 0;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_MULNUM_LONG: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          long_vars[opcode->operand0 + field_index] = 0;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_MULNUM_FLOAT: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          float_vars[opcode->operand0 + field_index] = 0;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_MULNUM_DOUBLE: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          double_vars[opcode->operand0 + field_index] = 0;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_CONSTANT_BYTE:
        byte_vars[opcode->operand0] = (int8_t)(uint8_t)opcode->operand1;
        break;
      case SPVM_OPCODE_C_ID_MOVE_CONSTANT_INT:
        int_vars[opcode->operand0] = (int32_t)opcode->operand1;
        break;
      case SPVM_OPCODE_C_ID_MOVE_CONSTANT_LONG: {
        long_vars[opcode->operand0] = *(int64_t*)&opcode->operand1;
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_CONSTANT_FLOAT: {
        SPVM_VALUE value;
        value.ival = (int32_t)opcode->operand1;
        float_vars[opcode->operand0] = value.fval;
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_CONSTANT_DOUBLE: {
        double_vars[opcode->operand0] = *(double*)&opcode->operand1;
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_BYTE: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            byte_vars[opcode->operand0] = ((int8_t*)((intptr_t)array + object_header_byte_size))[index];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_SHORT: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            short_vars[opcode->operand0] = ((int16_t*)((intptr_t)array + object_header_byte_size))[index];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_INT: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            int_vars[opcode->operand0] = ((int32_t*)((intptr_t)array + object_header_byte_size))[index];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_LONG: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            long_vars[opcode->operand0] = ((int64_t*)((intptr_t)array + object_header_byte_size))[index];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_FLOAT: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            float_vars[opcode->operand0] = ((float*)((intptr_t)array + object_header_byte_size))[index];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_DOUBLE: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            double_vars[opcode->operand0] = ((double*)((intptr_t)array + object_header_byte_size))[index];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_OBJECT: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            void* object = ((void**)((intptr_t)array + object_header_byte_size))[index];
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_BYTE: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            ((int8_t*)((intptr_t)array + object_header_byte_size))[index] = byte_vars[opcode->operand2];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_SHORT: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
           ((int16_t*)((intptr_t)array + object_header_byte_size))[index] = short_vars[opcode->operand2];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_INT: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            ((int32_t*)((intptr_t)array + object_header_byte_size))[index] = int_vars[opcode->operand2];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_LONG: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            ((int64_t*)((intptr_t)array + object_header_byte_size))[index] = long_vars[opcode->operand2];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_FLOAT: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            ((float*)((intptr_t)array + object_header_byte_size))[index] = float_vars[opcode->operand2];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_DOUBLE: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            ((double*)((intptr_t)array + object_header_byte_size))[index] = double_vars[opcode->operand2];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_OBJECT: {
        
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            void** element_address = &((void**)((intptr_t)array + object_header_byte_size))[index];
            SPVM_API_OBJECT_ASSIGN(element_address, *(void**)&object_vars[opcode->operand2]);
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_OBJECT_CHECK_TYPE: {
        
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            void** element_address = &((void**)((intptr_t)array + object_header_byte_size))[index];
            void* object = *(void**)&object_vars[opcode->operand2];
            int32_t can_assign = env->can_assign_array_element(env, array, object);
            if (can_assign) {
              SPVM_API_OBJECT_ASSIGN(element_address, object);
            }
            else {
              void* exception = env->new_string_nolen_raw(env, "Assigned element type is invalid");
              env->set_exception(env, exception);
              exception_flag = 1;
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_UNDEF: {
        
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            void* object_address = &((void**)((intptr_t)array + object_header_byte_size))[index];
            SPVM_API_OBJECT_ASSIGN(object_address, NULL);
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_BYTE: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        int32_t fields_length = opcode->operand3;
        
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            int32_t field_index;
            for (field_index = 0; field_index < fields_length; field_index++) {
              byte_vars[opcode->operand0 + field_index] = ((int8_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index];
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_SHORT: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        int32_t fields_length = opcode->operand3;
        
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            for (int32_t field_index = 0; field_index < fields_length; field_index++) {
              short_vars[opcode->operand0 + field_index] = ((int16_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index];
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_INT: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        int32_t fields_length = opcode->operand3;
        
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            for (int32_t field_index = 0; field_index < fields_length; field_index++) {
              int_vars[opcode->operand0 + field_index] = ((int32_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index];
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_LONG: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        int32_t fields_length = opcode->operand3;
        
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            int32_t field_index;
            for (field_index = 0; field_index < fields_length; field_index++) {
              long_vars[opcode->operand0 + field_index] = ((int64_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index];
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FLOAT: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        int32_t fields_length = opcode->operand3;
        
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            int32_t field_index;
            for (field_index = 0; field_index < fields_length; field_index++) {
              float_vars[opcode->operand0 + field_index] = ((float*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index];
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_DOUBLE: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        int32_t fields_length = opcode->operand3;
        
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            int32_t field_index;
            for (field_index = 0; field_index < fields_length; field_index++) {
              double_vars[opcode->operand0 + field_index] = ((double*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index];
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FIELD_BYTE: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            byte_vars[opcode->operand0] = ((int8_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FIELD_SHORT: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            short_vars[opcode->operand0] = ((int16_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FIELD_INT: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            int_vars[opcode->operand0] = ((int32_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FIELD_LONG: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            long_vars[opcode->operand0] = ((int64_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FIELD_FLOAT: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            float_vars[opcode->operand0] = ((float*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FIELD_DOUBLE: {
        void* array = *(void**)&object_vars[opcode->operand1];
        int32_t index = int_vars[opcode->operand2];
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        
        if (__builtin_expect(array == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            double_vars[opcode->operand0] = ((double*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_BYTE: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            int32_t field_index;
            for (field_index = 0; field_index < fields_length; field_index++) {
              ((int8_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index] = byte_vars[opcode->operand2 + field_index];
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_SHORT: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            int32_t field_index;
            for (field_index = 0; field_index < fields_length; field_index++) {
              ((int16_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index] = short_vars[opcode->operand2 + field_index];
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_INT: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            int32_t field_index;
            for (field_index = 0; field_index < fields_length; field_index++) {
              ((int32_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index] = int_vars[opcode->operand2 + field_index];
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_LONG: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            int32_t field_index;
            for (field_index = 0; field_index < fields_length; field_index++) {
              ((int64_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index] = long_vars[opcode->operand2 + field_index];
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FLOAT: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            int32_t field_index;
            for (field_index = 0; field_index < fields_length; field_index++) {
              ((float*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index] = float_vars[opcode->operand2 + field_index];
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_DOUBLE: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            int32_t field_index;
            for (field_index = 0; field_index < fields_length; field_index++) {
              ((double*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index] = double_vars[opcode->operand2 + field_index];
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FIELD_BYTE: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            ((int8_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index] = byte_vars[opcode->operand2];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FIELD_SHORT: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            ((int16_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index] = short_vars[opcode->operand2];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FIELD_INT: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            ((int32_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index] = int_vars[opcode->operand2];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FIELD_LONG: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            ((int64_t*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index] = long_vars[opcode->operand2];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FIELD_FLOAT: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            ((float*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index] = float_vars[opcode->operand2];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FIELD_DOUBLE: {
        void* array = *(void**)&object_vars[opcode->operand0];
        int32_t index = int_vars[opcode->operand1];
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        if (__builtin_expect(!array, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Array must not be undef");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) {
            void* exception = env->new_string_nolen_raw(env, "Index is out of range");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            ((double*)((intptr_t)array + object_header_byte_size))[fields_length * index + field_index] = double_vars[opcode->operand2];
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_DEREF_MULNUM_BYTE: {
        int8_t* value_ref = *(int8_t**)&ref_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          byte_vars[opcode->operand0 + field_index] = value_ref[field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_DEREF_MULNUM_SHORT: {
        int16_t* value_ref = *(int16_t**)&ref_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          short_vars[opcode->operand0 + field_index] = value_ref[field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_DEREF_MULNUM_INT: {
        int32_t* value_ref = *(int32_t**)&ref_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          int_vars[opcode->operand0 + field_index] = value_ref[field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_DEREF_MULNUM_LONG: {
        int64_t* value_ref = *(int64_t**)&ref_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          long_vars[opcode->operand0 + field_index] = value_ref[field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_DEREF_MULNUM_FLOAT: {
        float* value_ref = *(float**)&ref_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          float_vars[opcode->operand0 + field_index] = value_ref[field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_DEREF_MULNUM_DOUBLE: {
        double* value_ref = *(double**)&ref_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          double_vars[opcode->operand0 + field_index] = value_ref[field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DEREF_BYTE: {
        int8_t* value_ref = *(int8_t**)&ref_vars[opcode->operand1];
        int32_t field_index = opcode->operand2;
        byte_vars[opcode->operand0] = value_ref[field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DEREF_SHORT: {
        int16_t* value_ref = *(int16_t**)&ref_vars[opcode->operand1];
        int32_t field_index = opcode->operand2;
        short_vars[opcode->operand0] = value_ref[field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DEREF_INT: {
        int32_t* value_ref = *(int32_t**)&ref_vars[opcode->operand1];
        int32_t field_index = opcode->operand2;
        int_vars[opcode->operand0] = value_ref[field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DEREF_LONG: {
        int64_t* value_ref = *(int64_t**)&ref_vars[opcode->operand1];
        int32_t field_index = opcode->operand2;
        long_vars[opcode->operand0] = value_ref[field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DEREF_FLOAT: {
        float* value_ref = *(float**)&ref_vars[opcode->operand1];
        int32_t field_index = opcode->operand2;
        float_vars[opcode->operand0] = value_ref[field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DEREF_DOUBLE: {
        double* value_ref = *(double**)&ref_vars[opcode->operand1];
        int32_t field_index = opcode->operand2;
        double_vars[opcode->operand0] = value_ref[field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DEREF_BYTE: {
        int8_t* value_ref = *(int8_t**)&ref_vars[opcode->operand0];
        int32_t field_index = opcode->operand2;
        value_ref[field_index] = byte_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DEREF_SHORT: {
        int16_t* value_ref = *(int16_t**)&ref_vars[opcode->operand0];
        int32_t field_index = opcode->operand2;
        value_ref[field_index] = short_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DEREF_INT: {
        int32_t* value_ref = *(int32_t**)&ref_vars[opcode->operand0];
        int32_t field_index = opcode->operand2;
        value_ref[field_index] = int_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DEREF_LONG: {
        int64_t* value_ref = *(int64_t**)&ref_vars[opcode->operand0];
        int32_t field_index = opcode->operand2;
        value_ref[field_index] = long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DEREF_FLOAT: {
        float* value_ref = *(float**)&ref_vars[opcode->operand0];
        int32_t field_index = opcode->operand2;
        value_ref[field_index] = float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DEREF_DOUBLE: {
        double* value_ref = *(double**)&ref_vars[opcode->operand0];
        int32_t field_index = opcode->operand2;
        value_ref[field_index] = double_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_BYTE: {
        byte_vars[opcode->operand0] = byte_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_SHORT: {
        short_vars[opcode->operand0] = short_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_INT: {
        int_vars[opcode->operand0] = int_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_LONG: {
        long_vars[opcode->operand0] = long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_FLOAT: {
        float_vars[opcode->operand0] = float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_DOUBLE: {
        double_vars[opcode->operand0] = double_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_OBJECT: {
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], *(void**)&object_vars[opcode->operand1]);
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_OBJECT_CHECK_READ_ONLY: {
        void* string = *(void**)&object_vars[opcode->operand1];
        if (env->is_read_only(env, string)) {
          void* exception = env->new_string_nolen_raw(env, "Read-only strings can't be converted to mutable strings.");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_COPY: {
        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (object) {
          if (!(env->is_string(env, object) || env->is_numeric_array(env, object) || env->is_mulnum_array(env, object))) {
            void* exception = env->new_string_nolen_raw(env, "The operand of the copy operator must be a string type, a numeric type, or a multi numeric type");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            void* new_object_raw = env->copy_raw(env, object);
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], new_object_raw);
          }
        }
        else {
          SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], NULL);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_UNDEF: {
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], NULL);
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_REF: {
        *(void**)&ref_vars[opcode->operand0] = *(void**)&ref_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_MULNUM_BYTE: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          byte_vars[opcode->operand0 + field_index] = byte_vars[opcode->operand1 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_MULNUM_SHORT: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          short_vars[opcode->operand0 + field_index] = short_vars[opcode->operand1 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_MULNUM_INT: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          int_vars[opcode->operand0 + field_index] = int_vars[opcode->operand1 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_MULNUM_LONG: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          long_vars[opcode->operand0 + field_index] = long_vars[opcode->operand1 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_MULNUM_FLOAT: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          float_vars[opcode->operand0 + field_index] = float_vars[opcode->operand1 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_MULNUM_DOUBLE: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          double_vars[opcode->operand0 + field_index] = double_vars[opcode->operand1 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_BYTE: {
        int32_t field_index = opcode->operand2;
        byte_vars[opcode->operand0] = byte_vars[opcode->operand1 + field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_SHORT: {
        int32_t field_index = opcode->operand2;
        short_vars[opcode->operand0] = short_vars[opcode->operand1 + field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_INT: {
        int32_t field_index = opcode->operand2;
        int_vars[opcode->operand0] = int_vars[opcode->operand1 + field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_LONG: {
        int32_t field_index = opcode->operand2;
        long_vars[opcode->operand0] = long_vars[opcode->operand1 + field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_FLOAT: {
        int32_t field_index = opcode->operand2;
        float_vars[opcode->operand0] = float_vars[opcode->operand1 + field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DOUBLE: {
        int32_t field_index = opcode->operand2;
        double_vars[opcode->operand0] = double_vars[opcode->operand1 + field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_BYTE: {
        int32_t field_index = opcode->operand2;
        byte_vars[opcode->operand0 + field_index] = byte_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_SHORT: {
        int32_t field_index = opcode->operand2;
        short_vars[opcode->operand0 + field_index] = short_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_INT: {
        int32_t field_index = opcode->operand2;
        int_vars[opcode->operand0 + field_index] = int_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_LONG: {
        int32_t field_index = opcode->operand2;
        long_vars[opcode->operand0 + field_index] = long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_FLOAT: {
        int32_t field_index = opcode->operand2;
        float_vars[opcode->operand0 + field_index] = float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DOUBLE: {
        int32_t field_index = opcode->operand2;
        double_vars[opcode->operand0 + field_index] = double_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_MORTAL: {
        mortal_stack[mortal_stack_top] = opcode->operand0;
        mortal_stack_top++;
        
        break;
      }
      case SPVM_OPCODE_C_ID_LEAVE_SCOPE: {
        int32_t original_mortal_stack_top = opcode->operand0;
        int32_t mortal_stack_index;
        for (mortal_stack_index = original_mortal_stack_top; mortal_stack_index < mortal_stack_top; mortal_stack_index++) {
          int32_t var_index = mortal_stack[mortal_stack_index];
          void** object_address = (void**)&object_vars[var_index];
          
          if (*(void**)&object_vars[var_index] != NULL) {
            if (SPVM_API_GET_REF_COUNT(*object_address) > 1) { SPVM_API_DEC_REF_COUNT_ONLY(*object_address); }
            else { env->dec_ref_count(env, *object_address); }
          }
          
          *object_address = NULL;
        }
        
        mortal_stack_top = original_mortal_stack_top;
        
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_OBJECT: {
        int32_t basic_type_id = opcode->operand1;
        
        void* object = env->new_object_raw(env, basic_type_id);
        if (object == NULL) {
          void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for object");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          // Push object
          SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_BYTE_ARRAY: {
        int32_t length = int_vars[opcode->operand1];
        if (length >= 0) {
          void* object = env->new_byte_array_raw(env, length);
          if (object == NULL) {
            void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for byte array");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
          }
        }
        else {
          void* exception = env->new_string_nolen_raw(env, "Array length must be more than or equal to 0");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_SHORT_ARRAY: {
        
        // length
        int32_t length = int_vars[opcode->operand1];
        
        if (length >= 0) {
          void* object = env->new_short_array_raw(env, length);
          if (object == NULL) {
            void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for short array");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            // Set array
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
          }
        }
        else {
          void* exception = env->new_string_nolen_raw(env, "Array length must be more than or equal to 0");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_INT_ARRAY: {
        
        // length
        int32_t length = int_vars[opcode->operand1];
        
        if (length >= 0) {
          void* object = env->new_int_array_raw(env, length);
          if (object == NULL) {
            void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for int array");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            // Set array
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
          }
        }
        else {
          void* exception = env->new_string_nolen_raw(env, "Array length must be more than or equal to 0");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_LONG_ARRAY: {
        
        int32_t length = int_vars[opcode->operand1];
        if (length >= 0) {
          void* object = env->new_long_array_raw(env, length);
          if (object == NULL) {
            void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for long array");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
          }
        }
        else {
          void* exception = env->new_string_nolen_raw(env, "Array length must be more than or equal to 0");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_FLOAT_ARRAY: {
        
        int32_t length = int_vars[opcode->operand1];
        if (length >= 0) {
          void* object = env->new_float_array_raw(env, length);
          if (object == NULL) {
            void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for float array");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
          }
        }
        else {
          void* exception = env->new_string_nolen_raw(env, "Array length must be more than or equal to 0");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_DOUBLE_ARRAY: {
        
        int32_t length = int_vars[opcode->operand1];
        if (length >= 0) {
          void* object = env->new_double_array_raw(env, length);
          if (object == NULL) {
            void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for double array");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
          }
        }
        else {
          void* exception = env->new_string_nolen_raw(env, "Array length must be more than or equal to 0");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_OBJECT_ARRAY: {
        int32_t basic_type_id = opcode->operand1;
        
        int32_t length = int_vars[opcode->operand2];
        if (length >= 0) {
          void* object = env->new_object_array_raw(env, basic_type_id, length);
          if (object == NULL) {
            void* exception = env->new_string_nolen_raw(env, "Can't create the object array");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
          }
        }
        else {
          void* exception = env->new_string_nolen_raw(env, "Array length must be more than or equal to 0");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_MULTI_ARRAY: {
        int32_t basic_type_id = opcode->operand1;
        int32_t element_dimension = opcode->operand3;
        int32_t length = int_vars[opcode->operand2];
        
        if (length >= 0) {
          void* object = env->new_muldim_array_raw(env, basic_type_id, element_dimension, length);
          if (object == NULL) {
            void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for multi dimention array");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
          }
        }
        else {
          void* exception = env->new_string_nolen_raw(env, "Array length must be more than or equal to 0");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_MULNUM_ARRAY: {
        int32_t basic_type_id = opcode->operand1;
        
        int32_t length = int_vars[opcode->operand2];
        
        if (length >= 0) {
          void* object = env->new_mulnum_array_raw(env, basic_type_id, length);
          if (object == NULL) {
            void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for muti numeric array");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
          }
        }
        else {
          void* exception = env->new_string_nolen_raw(env, "Array length must be more than or equal to 0");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_STRING: {
        int32_t constant_string_id = opcode->operand1;
        int32_t constant_string_length;
        const char* constant_string = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, constant_string_id, &constant_string_length);
        void* string = env->new_string_raw(env, constant_string, constant_string_length);
        if (string == NULL) {
          void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for string");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          env->make_read_only(env, string);
          SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0] , string);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_ARRAY_LENGTH: {
        if (*(void**)&object_vars[opcode->operand1] == NULL) {
          void* exception = env->new_string_nolen_raw(env, "Can't get the array length of undef.");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int_vars[opcode->operand0] = *(int32_t*)((intptr_t)*(void**)&object_vars[opcode->operand1] + (intptr_t)env->object_length_offset);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_IS_READ_ONLY: {
        void* string = *(void**)&object_vars[opcode->operand1];
        int32_t is_read_only = env->is_read_only(env, string);
        int_vars[0] = is_read_only;
        break;
      }
      case SPVM_OPCODE_C_ID_CONCAT: {
        
        void* string1 = *(void**)&object_vars[opcode->operand1];
        void* string2 = *(void**)&object_vars[opcode->operand2];
        if (string1 == NULL) {
          void* exception = env->new_string_nolen_raw(env, "\".\" operater left value must be defined");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else if (string2 == NULL) {
          void* exception = env->new_string_nolen_raw(env, "\".\" operater right value must be defined");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          void* string3 = env->concat_raw(env, string1, string2);
          SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string3);
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_REFOP: {
        void* object = object_vars[opcode->operand1];
        if (object == NULL) {
          object_vars[opcode->operand0] = NULL;
        }
        else {
          void* type_name = env->get_type_name_raw(env, object);
          SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], type_name);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_DUMP: {
        void* object = object_vars[opcode->operand1];
        void* dump = env->dump_raw(env, object);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], dump);
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_STRING_LEN: {
        int32_t length = int_vars[opcode->operand1];
        if (length >= 0) {
          void* string = env->new_string_raw(env, NULL, length);
          if (string == NULL) {
            void* exception = env->new_string_nolen_raw(env, "The new_string_len operator can't allocate enough memory");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
          else {
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
          }
        }
        else {
          void* exception = env->new_string_nolen_raw(env, "The length of the new_string_len operator must be a positive number");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GOTO:
        opcode_rel_index = opcode->operand0;
        continue;
      case SPVM_OPCODE_C_ID_IF_EQ_ZERO: {
        if (int_vars[0] == 0) {
          opcode_rel_index = opcode->operand0;
          continue;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_IF_NE_ZERO: {
        if (int_vars[0]) {
          opcode_rel_index = opcode->operand0;
          continue;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_BYTE: {
        *(int8_t*)&stack[stack_index] = byte_vars[opcode->operand0];
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_SHORT: {
        *(int16_t*)&stack[stack_index] = short_vars[opcode->operand0];
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_INT: {
        *(int32_t*)&stack[stack_index] = int_vars[opcode->operand0];
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_LONG: {
        *(int64_t*)&stack[stack_index] = long_vars[opcode->operand0];
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_FLOAT: {
        *(float*)&stack[stack_index] = float_vars[opcode->operand0];
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_DOUBLE: {
        *(double*)&stack[stack_index] = double_vars[opcode->operand0];
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_OBJECT: {
        *(void**)&stack[stack_index] = *(void**)&object_vars[opcode->operand0];
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_REF: {
        *(void**)&stack[stack_index] = *(void**)&ref_vars[opcode->operand0];
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_UNDEF:
        *(void**)&stack[stack_index] = NULL;
        stack_index++;
        
        break;
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_BYTE: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(int8_t*)&stack[stack_index + field_index] = byte_vars[opcode->operand0 + field_index];
        }
        stack_index += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_SHORT: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(int16_t*)&stack[stack_index + field_index] = short_vars[opcode->operand0 + field_index];
        }
        stack_index += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_INT: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(int32_t*)&stack[stack_index + field_index] = int_vars[opcode->operand0 + field_index];
        }
        stack_index += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_LONG: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(int64_t*)&stack[stack_index + field_index] = long_vars[opcode->operand0 + field_index];
        }
        stack_index += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_FLOAT: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(float*)&stack[stack_index + field_index] = float_vars[opcode->operand0 + field_index];
        }
        stack_index += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_DOUBLE: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(double*)&stack[stack_index + field_index] = double_vars[opcode->operand0 + field_index];
        }
        stack_index += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_CHECK_OBJECT_TYPE: {
        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (object != NULL) {
          int32_t check_basic_type_id = opcode->operand2;
          int32_t check_type_dimension = opcode->operand3;
          
          int32_t object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);
          int32_t object_type_dimension = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);
          
          if (object_basic_type_id == check_basic_type_id && object_type_dimension == check_type_dimension) {
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], *(void**)&object_vars[opcode->operand1]);
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "Can't convert imcompatible object type in runtime.");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_CHECK_CALLBACK: {
        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (object != NULL) {
          int32_t check_basic_type_id = opcode->operand2;
          
          int32_t object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);
          int32_t object_type_dimension = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);
          
          if (env->has_callback(env, object, check_basic_type_id)) {
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], *(void**)&object_vars[opcode->operand1]);
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "Can't convert imcompatible callback type.");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_CHECK_INTERFACE: {
        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (object != NULL) {
          int32_t check_basic_type_id = opcode->operand2;
          
          int32_t object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);
          int32_t object_type_dimension = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);
          
          if (env->has_interface(env, object, check_basic_type_id)) {
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], *(void**)&object_vars[opcode->operand1]);
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "Can't convert imcompatible callback type.");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_CALL_CLASS_METHOD_BY_ID:
      case SPVM_OPCODE_C_ID_CALL_INSTANCE_METHOD_BY_ID:
      {
        int32_t call_method_id = opcode->operand1;
        stack_index = 0;
        exception_flag = env->call_spvm_method(env, call_method_id, stack);

        if (!exception_flag) {
          SPVM_RUNTIME_METHOD* call_spvm_method = SPVM_API_RUNTIME_get_method(env->runtime, call_method_id);
          SPVM_RUNTIME_TYPE* call_spvm_method_return_type =SPVM_API_RUNTIME_get_type(env->runtime, call_spvm_method->return_type_id);
          int32_t call_spvm_method_return_basic_type_id = call_spvm_method_return_type->basic_type_id;
          int32_t call_spvm_method_return_type_dimension = call_spvm_method_return_type->dimension;
          int32_t call_spvm_method_return_basic_type_category = SPVM_API_RUNTIME_get_basic_type_category(env->runtime, call_spvm_method_return_type->basic_type_id);
          if (call_spvm_method_return_type_dimension == 0) {
            switch (call_spvm_method_return_basic_type_category) {
              case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_VOID: {
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_NUMERIC: {
                switch (call_spvm_method_return_basic_type_id) {
                  case SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE: {
                    byte_vars[opcode->operand0] = *(int8_t*)&stack[0];
                    break;
                  }
                  case SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT: {
                    short_vars[opcode->operand0] = *(int16_t*)&stack[0];
                    break;
                  }
                  case SPVM_NATIVE_C_BASIC_TYPE_ID_INT: {
                    int_vars[opcode->operand0] = *(int32_t*)&stack[0];
                    break;
                  }
                  case SPVM_NATIVE_C_BASIC_TYPE_ID_LONG: {
                    long_vars[opcode->operand0] = *(int64_t*)&stack[0];
                    break;
                  }
                  case SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT: {
                    float_vars[opcode->operand0] = *(float*)&stack[0];
                    break;
                  }
                  case SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE: {
                    double_vars[opcode->operand0] = *(double*)&stack[0];
                    break;
                  }
                  default: {
                    assert(0);
                  }
                }
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_STRING:
              case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_CLASS:
              case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_INTERFACE:
              case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_CALLBACK:
              case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_ANY_OBJECT:
              {
                SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], stack[0].oval);
                break;
              }
              case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_MULNUM:
              {
                int32_t method_return_class_id = SPVM_API_RUNTIME_get_basic_type_class_id(env->runtime, call_spvm_method_return_basic_type_id);
                int32_t method_return_class_fields_length = SPVM_API_RUNTIME_get_class_fields_length(env->runtime, method_return_class_id);
                int32_t method_return_class_fields_base_id = SPVM_API_RUNTIME_get_class_fields_base_id(env->runtime, method_return_class_id);
                int32_t method_return_mulnum_field_id = method_return_class_fields_base_id;
                int32_t method_return_mulnum_field_type_id = SPVM_API_RUNTIME_get_field_type_id(env->runtime, method_return_mulnum_field_id);
                int32_t method_return_mulnum_field_type_basic_type_id = SPVM_API_RUNTIME_get_type_basic_type_id(env->runtime, method_return_mulnum_field_type_id);
                
                switch(method_return_mulnum_field_type_basic_type_id) {
                  case SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE: {
                    int32_t fields_length = opcode->operand3;
                    for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                      byte_vars[opcode->operand0 + field_index] = *(int8_t*)&stack[field_index];
                    }
                    break;
                  }
                  case SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT: {
                    int32_t fields_length = opcode->operand3;
                    for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                      short_vars[opcode->operand0 + field_index] = *(int16_t*)&stack[field_index];
                    }
                    break;
                  }
                  case SPVM_NATIVE_C_BASIC_TYPE_ID_INT: {
                    int32_t fields_length = opcode->operand3;
                    for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                      int_vars[opcode->operand0 + field_index] = *(int32_t*)&stack[field_index];
                    }
                    break;
                  }
                  case SPVM_NATIVE_C_BASIC_TYPE_ID_LONG: {
                    int32_t fields_length = opcode->operand3;
                    for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                      long_vars[opcode->operand0 + field_index] = *(int64_t*)&stack[field_index];
                    }
                    break;
                  }
                  case SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT: {
                    int32_t fields_length = opcode->operand3;
                    for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                      float_vars[opcode->operand0 + field_index] = *(float*)&stack[field_index];
                    }
                    break;
                  }
                  case SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE: {
                    int32_t fields_length = opcode->operand3;
                    for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                      double_vars[opcode->operand0 + field_index] = *(double*)&stack[field_index];
                    }
                    break;
                  }
                  default: {
                    assert(0);
                  }
                }
                break;
              }
              default: {
                assert(0);
              }
            }
          }
          else if (call_spvm_method_return_type_dimension > 0) {
            SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], stack[0].oval);
          }
          else {
            assert(0);
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_CALL_INSTANCE_METHOD_BY_SIGNATURE:
      {
        int32_t decl_method_id = opcode->operand1;
        SPVM_RUNTIME_METHOD* decl_method = SPVM_API_RUNTIME_get_method(env->runtime, decl_method_id);
        void* object = stack[0].oval;
        const char* decl_method_name = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, decl_method->name_id, NULL);
        const char* decl_method_signature = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, decl_method->signature_id, NULL);
        int32_t call_method_id = env->get_instance_method_id(env, object, decl_method_name, decl_method_signature);

        stack_index = 0;
        if (call_method_id < 0) {
          memset(tmp_buffer, sizeof(tmp_buffer), 0);
          SPVM_RUNTIME_CLASS* decl_method_class = SPVM_API_RUNTIME_get_class(env->runtime, decl_method->class_id);
          snprintf(tmp_buffer, 255, "Can't find the \"%s\" method with the signature \"%s\" that is declared in \"%s\"", decl_method_name, decl_method_signature, SPVM_API_RUNTIME_get_constant_string_value(env->runtime, decl_method_class->name_id, NULL));
          void* exception = env->new_string_nolen_raw(env, tmp_buffer);
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          exception_flag = env->call_spvm_method(env, call_method_id, stack);
          
          if (!exception_flag) {
            SPVM_RUNTIME_METHOD* call_spvm_method = SPVM_API_RUNTIME_get_method(env->runtime, call_method_id);
            SPVM_RUNTIME_TYPE* call_spvm_method_return_type =SPVM_API_RUNTIME_get_type(env->runtime, call_spvm_method->return_type_id);
            int32_t call_spvm_method_return_basic_type_id = call_spvm_method_return_type->basic_type_id;
            int32_t call_spvm_method_return_type_dimension = call_spvm_method_return_type->dimension;
            int32_t call_spvm_method_return_basic_type_category = SPVM_API_RUNTIME_get_basic_type_category(env->runtime, call_spvm_method_return_type->basic_type_id);
            if (call_spvm_method_return_type_dimension == 0) {
              switch (call_spvm_method_return_basic_type_category) {
                case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_VOID: {
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_NUMERIC: {
                  switch (call_spvm_method_return_basic_type_id) {
                    case SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE: {
                      byte_vars[opcode->operand0] = *(int8_t*)&stack[0];
                      break;
                    }
                    case SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT: {
                      short_vars[opcode->operand0] = *(int16_t*)&stack[0];
                      break;
                    }
                    case SPVM_NATIVE_C_BASIC_TYPE_ID_INT: {
                      int_vars[opcode->operand0] = *(int32_t*)&stack[0];
                      break;
                    }
                    case SPVM_NATIVE_C_BASIC_TYPE_ID_LONG: {
                      long_vars[opcode->operand0] = *(int64_t*)&stack[0];
                      break;
                    }
                    case SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT: {
                      float_vars[opcode->operand0] = *(float*)&stack[0];
                      break;
                    }
                    case SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE: {
                      double_vars[opcode->operand0] = *(double*)&stack[0];
                      break;
                    }
                    default: {
                      assert(0);
                    }
                  }
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_STRING:
                case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_CLASS:
                case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_INTERFACE:
                case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_CALLBACK:
                case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_ANY_OBJECT:
                {
                  SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], stack[0].oval);
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_MULNUM:
                {
                  int32_t method_return_class_id = SPVM_API_RUNTIME_get_basic_type_class_id(env->runtime, call_spvm_method_return_basic_type_id);
                  int32_t method_return_class_fields_length = SPVM_API_RUNTIME_get_class_fields_length(env->runtime, method_return_class_id);
                  int32_t method_return_class_fields_base_id = SPVM_API_RUNTIME_get_class_fields_base_id(env->runtime, method_return_class_id);
                  int32_t method_return_mulnum_field_id = method_return_class_fields_base_id;
                  int32_t method_return_mulnum_field_type_id = SPVM_API_RUNTIME_get_field_type_id(env->runtime, method_return_mulnum_field_id);
                  int32_t method_return_mulnum_field_type_basic_type_id = SPVM_API_RUNTIME_get_type_basic_type_id(env->runtime, method_return_mulnum_field_type_id);
                  
                  switch(method_return_mulnum_field_type_basic_type_id) {
                    case SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE: {
                      int32_t fields_length = opcode->operand3;
                      for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                        byte_vars[opcode->operand0 + field_index] = *(int8_t*)&stack[field_index];
                      }
                      break;
                    }
                    case SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT: {
                      int32_t fields_length = opcode->operand3;
                      for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                        short_vars[opcode->operand0 + field_index] = *(int16_t*)&stack[field_index];
                      }
                      break;
                    }
                    case SPVM_NATIVE_C_BASIC_TYPE_ID_INT: {
                      int32_t fields_length = opcode->operand3;
                      for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                        int_vars[opcode->operand0 + field_index] = *(int32_t*)&stack[field_index];
                      }
                      break;
                    }
                    case SPVM_NATIVE_C_BASIC_TYPE_ID_LONG: {
                      int32_t fields_length = opcode->operand3;
                      for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                        long_vars[opcode->operand0 + field_index] = *(int64_t*)&stack[field_index];
                      }
                      break;
                    }
                    case SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT: {
                      int32_t fields_length = opcode->operand3;
                      for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                        float_vars[opcode->operand0 + field_index] = *(float*)&stack[field_index];
                      }
                      break;
                    }
                    case SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE: {
                      int32_t fields_length = opcode->operand3;
                      for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                        double_vars[opcode->operand0 + field_index] = *(double*)&stack[field_index];
                      }
                      break;
                    }
                    default: {
                      assert(0);
                    }
                  }
                  break;
                }
                default: {
                  assert(0);
                }
              }
            }
            else if (call_spvm_method_return_type_dimension > 0) {
              SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], stack[0].oval);
            }
            else {
              assert(0);
            }
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_IF_EXCEPTION_CATCH: {
        if (exception_flag) {
          exception_flag = 0;
          
          int32_t method_index = opcode->operand1;
          int32_t method_id = class->methods_base_id + method_index;
          SPVM_RUNTIME_METHOD* method = SPVM_API_RUNTIME_get_method(env->runtime, method_id);
          int32_t line = opcode->operand2;
          
          const char* method_name = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, method->name_id, NULL);
          SPVM_RUNTIME_CLASS* method_class = SPVM_API_RUNTIME_get_class(env->runtime, method->class_id);
          const char* class_name = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, method_class->name_id, NULL);
          const char* file = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, method_class->module_file_id, NULL);
          
          // Exception stack trace
          env->set_exception(env, env->new_stack_trace_raw(env, env->get_exception(env), class_name, method_name, file, line));
          opcode_rel_index = opcode->operand0;
          continue;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_IF_EXCEPTION_RETURN: {
        
        if (exception_flag) {
          int32_t method_index = opcode->operand1;
          int32_t method_id = class->methods_base_id + method_index;
          SPVM_RUNTIME_METHOD* method = SPVM_API_RUNTIME_get_method(env->runtime, method_id);
          int32_t line = opcode->operand2;
          
          const char* method_name = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, method->name_id, NULL);
          SPVM_RUNTIME_CLASS* method_class = SPVM_API_RUNTIME_get_class(env->runtime, method->class_id);
          const char* class_name = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, method_class->name_id, NULL);
          const char* file = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, class->module_file_id, NULL);

          // Exception stack trace
          env->set_exception(env, env->new_stack_trace_raw(env, env->get_exception(env), class_name, method_name, file, line));
          opcode_rel_index = opcode->operand0;
          continue;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DIE_FLAG_TRUE: {
        exception_flag = 1;
        break;
      }
      case SPVM_OPCODE_C_ID_PRINT: {
        void* object = object_vars[opcode->operand0];
        if (object) {
          const char* bytes = env->get_chars(env, object);
          int32_t string_length = env->length(env, object);
          
          if (string_length > 0) {
            size_t ret = fwrite(bytes, 1, string_length, stdout);
          }
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_MAKE_READ_ONLY: {
        void* string = object_vars[opcode->operand0];
        env->make_read_only(env, string);
        break;
      }
      case SPVM_OPCODE_C_ID_WARN: {
        int32_t line = opcode->operand1;
        
        const char* file = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, class->module_file_id, NULL);
        
        void* object = object_vars[opcode->operand0];
        
        int32_t empty_or_undef = 0;
        if (object) {
          const char* bytes = env->get_chars(env, object);
          int32_t string_length = env->length(env, object);

          if (string_length > 0) {
            size_t ret = fwrite(bytes, 1, string_length, stderr);
            // Add line and file information if last character is not '\n'
            int32_t add_line_file;
            if (bytes[string_length - 1] != '\n') {
              fprintf(stderr, " at %s line %d\n", file, line);
            }
          }
          else {
            empty_or_undef = 1;
          }
        }
        else {
          empty_or_undef = 1;
        }
        
        if (empty_or_undef) {
          fprintf(stderr, "Warning: something's wrong at %s line %d\n", file, line);
        }
        
        fflush(stderr);
        
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_VOID:
      {
        opcode_rel_index = opcode->operand1;
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_BYTE:
      {
        *(int8_t*)&stack[0] = byte_vars[opcode->operand0];
        
        opcode_rel_index = opcode->operand1;
        
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_SHORT:
      {
        *(int16_t*)&stack[0] = short_vars[opcode->operand0];
        
        opcode_rel_index = opcode->operand1;
        
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_INT:
      {
        *(int32_t*)&stack[0] = int_vars[opcode->operand0];
        
        opcode_rel_index = opcode->operand1;
        
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_LONG:
      {
        *(int64_t*)&stack[0] = long_vars[opcode->operand0];
        
        opcode_rel_index = opcode->operand1;
        
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_FLOAT:
      {
        *(float*)&stack[0] = float_vars[opcode->operand0];
        
        opcode_rel_index = opcode->operand1;
        
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_DOUBLE:
      {
        *(double*)&stack[0] = double_vars[opcode->operand0];
        
        opcode_rel_index = opcode->operand1;
        
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_OBJECT:
      {
        *(void**)&stack[0] = *(void**)&object_vars[opcode->operand0];
        // Increment ref count of return value not to release by leave scope
        if (*(void**)&stack[0] != NULL) {
          SPVM_API_INC_REF_COUNT_ONLY(*(void**)&stack[0]);
        }
        opcode_rel_index = opcode->operand1;
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_UNDEF:
      {
        *(void**)&stack[0] = NULL;
        opcode_rel_index = opcode->operand1;
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_MULNUM_BYTE:
      {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(int8_t*)&stack[field_index] = byte_vars[opcode->operand0 + field_index];
        }
        opcode_rel_index = opcode->operand1;
        
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_MULNUM_SHORT:
      {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(int16_t*)&stack[field_index] = short_vars[opcode->operand0 + field_index];
        }
        opcode_rel_index = opcode->operand1;
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_MULNUM_INT:
      {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(int32_t*)&stack[field_index] = int_vars[opcode->operand0 + field_index];
        }
        opcode_rel_index = opcode->operand1;
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_MULNUM_LONG:
      {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(int64_t*)&stack[field_index] = long_vars[opcode->operand0 + field_index];
        }
        opcode_rel_index = opcode->operand1;
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_MULNUM_FLOAT:
      {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(float*)&stack[field_index] = float_vars[opcode->operand0 + field_index];
        }
        opcode_rel_index = opcode->operand1;
        continue;
      }
      case SPVM_OPCODE_C_ID_RETURN_MULNUM_DOUBLE:
      {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(double*)&stack[field_index] = double_vars[opcode->operand0 + field_index];
        }
        opcode_rel_index = opcode->operand1;
        continue;
      }
      case SPVM_OPCODE_C_ID_LOOKUP_SWITCH: {
        
        int32_t switch_id = opcode->operand1;
        
        // Default branch
        int32_t default_opcode_rel_index = opcode->operand2;
        
        // Cases length
        int32_t case_infos_length = opcode->operand3;

        if (case_infos_length > 0) {
          // min
          SPVM_OPCODE* opcode_case_info_min = &(opcodes[method_opcodes_base_id + opcode_rel_index + 1 + 0]);
          int32_t min = opcode_case_info_min->operand1;
          
          // max
          SPVM_OPCODE* opcode_case_info_max = &(opcodes[method_opcodes_base_id + opcode_rel_index + 1 + case_infos_length - 1]);
          int32_t max = opcode_case_info_max->operand1;
          
          if (int_vars[opcode->operand0] >= min && int_vars[opcode->operand0] <= max) {
            // 2 opcode_rel_index searching
            int32_t cur_min_pos = 0;
            int32_t cur_max_pos = case_infos_length - 1;
            
            while (1) {
              if (cur_max_pos < cur_min_pos) {
                opcode_rel_index = default_opcode_rel_index;
                break;
              }
              int32_t cur_half_pos = cur_min_pos + (cur_max_pos - cur_min_pos) / 2;
              SPVM_OPCODE* opcode_case_cur_half = &(opcodes[method_opcodes_base_id + opcode_rel_index + 1 + cur_half_pos]);
              int32_t cur_half = opcode_case_cur_half->operand1;
              
              if (int_vars[opcode->operand0] > cur_half) {
                cur_min_pos = cur_half_pos + 1;
              }
              else if (int_vars[opcode->operand0] < cur_half) {
                cur_max_pos = cur_half_pos - 1;
              }
              else {
                opcode_rel_index = opcode_case_cur_half->operand2;
                break;
              }
            }
          }
          else {
            opcode_rel_index = default_opcode_rel_index;
          }
        }
        else {
          opcode_rel_index = default_opcode_rel_index;
        }
        
        continue;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_BYTE: {
        int32_t field_id = opcode->operand2;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of getting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          byte_vars[opcode->operand0] = *(int8_t*)((intptr_t)object + object_header_byte_size + field_offset);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_SHORT: {
        int32_t field_id = opcode->operand2;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];

        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of getting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          short_vars[opcode->operand0] = *(int16_t*)((intptr_t)object + object_header_byte_size + field_offset);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_INT: {
        int32_t field_id = opcode->operand2;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of getting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int_vars[opcode->operand0] = *(int32_t*)((intptr_t)object + object_header_byte_size + field_offset);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_LONG: {
        int32_t field_id = opcode->operand2;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of getting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          long_vars[opcode->operand0] = *(int64_t*)((intptr_t)object + object_header_byte_size + field_offset);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_FLOAT: {
        int32_t field_id = opcode->operand2;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of getting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          float_vars[opcode->operand0] = *(float*)((intptr_t)object + object_header_byte_size + field_offset);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_DOUBLE: {
        int32_t field_id = opcode->operand2;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of getting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          double_vars[opcode->operand0] = *(double*)((intptr_t)object + object_header_byte_size + field_offset);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_OBJECT: {
        int32_t field_id = opcode->operand2;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of getting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          void* get_field_object = *(void**)((intptr_t)object + object_header_byte_size + field_offset);
          SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], get_field_object);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_BYTE: {
        void* object = *(void**)&object_vars[opcode->operand0];
        int32_t field_id = opcode->operand1;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of setting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          *(int8_t*)((intptr_t)object + object_header_byte_size + field_offset) = byte_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_SHORT: {
        void* object = *(void**)&object_vars[opcode->operand0];
        int32_t field_id = opcode->operand1;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of setting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          *(int16_t*)((intptr_t)object + object_header_byte_size + field_offset) = short_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_INT: {
        void* object = *(void**)&object_vars[opcode->operand0];
        int32_t field_id = opcode->operand1;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of setting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          *(int32_t*)((intptr_t)object + object_header_byte_size + field_offset) = int_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_LONG: {
        void* object = *(void**)&object_vars[opcode->operand0];
        int32_t field_id = opcode->operand1;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of setting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          *(int64_t*)((intptr_t)object + object_header_byte_size + field_offset) = long_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_FLOAT: {
        void* object = *(void**)&object_vars[opcode->operand0];
        int32_t field_id = opcode->operand1;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of setting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          *(float*)((intptr_t)object + object_header_byte_size + field_offset) = float_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_DOUBLE: {
        void* object = *(void**)&object_vars[opcode->operand0];
        int32_t field_id = opcode->operand1;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of setting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          *(double*)((intptr_t)object + object_header_byte_size + field_offset) = double_vars[opcode->operand2];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_OBJECT: {
        void* object = *(void**)&object_vars[opcode->operand0];
        int32_t field_id = opcode->operand1;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;

        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of setting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          void* get_field_object_address = (void**)((intptr_t)object + object_header_byte_size + field_offset);
          SPVM_API_OBJECT_ASSIGN(get_field_object_address, *(void**)&object_vars[opcode->operand2]);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_UNDEF: {
        void* object = *(void**)&object_vars[opcode->operand0];
        int32_t field_id = opcode->operand1;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;

        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocants of setting fields must not be undefined values");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          void* get_field_object_address = (void**)((intptr_t)object + object_header_byte_size + field_offset);
          SPVM_API_OBJECT_ASSIGN(get_field_object_address, NULL);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_WEAKEN_FIELD: {
        int32_t field_id = opcode->operand1;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;
        void* object = *(void**)&object_vars[opcode->operand0];
        if (object == NULL) {
          SPVM_OBJECT* exception = env->new_string_nolen_raw(env, "Object to weaken an object field must not be undefined.");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          void** get_field_object_address = (void**)((intptr_t)object + object_header_byte_size + field_offset);
          int32_t status = env->weaken(env, get_field_object_address);
          if (status != 0) {
            void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for weaken back reference");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_UNWEAKEN_FIELD: {
        int32_t field_id = opcode->operand1;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;
        void* object = *(void**)&object_vars[opcode->operand0];
        if (object == NULL) {
          SPVM_OBJECT* exception = env->new_string_nolen_raw(env, "Object to unweaken an object field must not be undefined.");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          void** get_field_object_address = (void**)((intptr_t)object + object_header_byte_size + field_offset);
          env->unweaken(env, get_field_object_address);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_ISWEAK_FIELD: {
        int32_t field_id = opcode->operand2;
        SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
        int32_t field_offset = field->offset;
        void* object = *(void**)&object_vars[opcode->operand1];
        if (object == NULL) {
          SPVM_OBJECT* exception = env->new_string_nolen_raw(env, "Object to isweak an object field must not be undefined.");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          void** get_field_object_address = (void**)((intptr_t)object + object_header_byte_size + field_offset);
          int_vars[0] = env->isweak(env, get_field_object_address);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_HAS_IMPLEMENT: {
        int32_t implement_method_id = opcode->operand2;
        SPVM_RUNTIME_METHOD* implement_method = SPVM_API_RUNTIME_get_method(env->runtime, implement_method_id);
        const char* implement_method_name = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, implement_method->name_id, NULL);
        
        int32_t interface_basic_type_id = opcode->operand3;
        SPVM_RUNTIME_BASIC_TYPE* interface_basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, interface_basic_type_id);
        SPVM_RUNTIME_CLASS* interface_class = SPVM_API_RUNTIME_get_class(env->runtime, interface_basic_type->class_id);
        SPVM_RUNTIME_METHOD* interface_method = SPVM_API_get_runtime_method_from_runtime_class(env, interface_class->id, implement_method_name);
        const char* implement_method_signature = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, implement_method->signature_id, NULL);
        
        void* object = *(void**)&object_vars[opcode->operand1];
        
        int32_t call_method_id = env->get_instance_method_id(env, object, implement_method_name, implement_method_signature);
        
        int_vars[0] = call_method_id >= 0;

        break;
      }
      case SPVM_OPCODE_C_ID_REFCNT: {
        void* object = object_vars[opcode->operand1];
        if (object == NULL) {
          int_vars[opcode->operand0] = 0;
        }
        else {
          int_vars[opcode->operand0] = env->get_ref_count(env, object);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_REF_BYTE:
        *(void**)&ref_vars[opcode->operand0] = &byte_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_REF_SHORT:
        *(void**)&ref_vars[opcode->operand0] = &short_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_REF_INT:
        *(void**)&ref_vars[opcode->operand0] = &int_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_REF_LONG:
        *(void**)&ref_vars[opcode->operand0] = &long_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_REF_FLOAT:
        *(void**)&ref_vars[opcode->operand0] = &float_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_REF_DOUBLE:
        *(void**)&ref_vars[opcode->operand0] = &double_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_GET_DEREF_BYTE: {
        byte_vars[opcode->operand0] = *(int8_t*)*(void**)&ref_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_DEREF_SHORT: {
        short_vars[opcode->operand0] = *(int16_t*)*(void**)&ref_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_DEREF_INT: {
        int_vars[opcode->operand0] = *(int32_t*)*(void**)&ref_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_DEREF_LONG: {
        long_vars[opcode->operand0] = *(int64_t*)*(void**)&ref_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_DEREF_FLOAT: {
        float_vars[opcode->operand0] = *(float*)*(void**)&ref_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_DEREF_DOUBLE: {
        double_vars[opcode->operand0] = *(double*)*(void**)&ref_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DEREF_BYTE: {
        *(int8_t*)*(void**)&ref_vars[opcode->operand0] = byte_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DEREF_SHORT: {
        *(int16_t*)*(void**)&ref_vars[opcode->operand0] = short_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DEREF_INT: {
        *(int32_t*)*(void**)&ref_vars[opcode->operand0] = int_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DEREF_LONG: {
        *(int64_t*)*(void**)&ref_vars[opcode->operand0] = long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DEREF_FLOAT: {
        *(float*)*(void**)&ref_vars[opcode->operand0] = float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DEREF_DOUBLE: {
        *(double*)*(void**)&ref_vars[opcode->operand0] = double_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_BYTE: {
        int32_t class_var_id = opcode->operand1;
        
        byte_vars[opcode->operand0] = *(int8_t*)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id];
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_SHORT: {
        int32_t class_var_id = opcode->operand1;
        
        short_vars[opcode->operand0] = *(int16_t*)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id];
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_INT: {
        
        int32_t class_var_id = opcode->operand1;
        
        int_vars[opcode->operand0] = *(int32_t*)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id];
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_LONG: {
        int32_t class_var_id = opcode->operand1;
        
        long_vars[opcode->operand0] = *(int64_t*)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id];
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_FLOAT: {
        int32_t class_var_id = opcode->operand1;
        
        float_vars[opcode->operand0] = *(float*)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id];
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_DOUBLE: {
        int32_t class_var_id = opcode->operand1;
        
        double_vars[opcode->operand0] = *(double*)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id];
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_OBJECT: {
        int32_t class_var_id = opcode->operand1;
        
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], *(void**)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id]);
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_BYTE: {
        int32_t class_var_id = opcode->operand0;
        
        *(int8_t*)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id] = byte_vars[opcode->operand1];
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_SHORT: {
        int32_t class_var_id = opcode->operand0;
        
        *(int16_t*)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id] = short_vars[opcode->operand1];
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_INT: {
        int32_t class_var_id = opcode->operand0;
        
        *(int32_t*)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id] = int_vars[opcode->operand1];
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_LONG: {
        int32_t class_var_id = opcode->operand0;
        
        *(int64_t*)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id] = long_vars[opcode->operand1];
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_FLOAT: {
        int32_t class_var_id = opcode->operand0;
        
        *(float*)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id] = float_vars[opcode->operand1];
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_DOUBLE: {
        int32_t class_var_id = opcode->operand0;
        
        *(double*)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id] = double_vars[opcode->operand1];
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_OBJECT: {
        int32_t class_var_id = opcode->operand0;
        
        SPVM_API_OBJECT_ASSIGN((void**)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id], *(void**)&object_vars[opcode->operand1]);
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_UNDEF: {
        int32_t class_var_id = opcode->operand0;
        
        SPVM_API_OBJECT_ASSIGN((void**)&((SPVM_VALUE*)env->class_vars_heap)[class_var_id], NULL);
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_EXCEPTION_VAR: {
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], env->get_exception(env));
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_EXCEPTION_VAR: {
        
        env->set_exception(env, *(void**)&object_vars[opcode->operand0]);
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_EXCEPTION_VAR_UNDEF: {
        
        env->set_exception(env, NULL);
        
        break;
      }
      case SPVM_OPCODE_C_ID_END_METHOD: {
        goto label_END_METHOD;
      }
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_BYTE_OBJECT: {
        int8_t value = byte_vars[opcode->operand1];
        int32_t basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE_OBJECT;

        void* object = env->new_object_raw(env, basic_type_id);
        SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
        *(int8_t*)&fields[0] = value;
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
        
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_SHORT_OBJECT: {
        int16_t value = short_vars[opcode->operand1];
        int32_t basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT_OBJECT;
        void* object = env->new_object_raw(env, basic_type_id);
        SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
        *(int16_t*)&fields[0] = value;
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
        
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_INT_OBJECT: {
        int32_t value = int_vars[opcode->operand1];
        int32_t basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_INT_OBJECT;
        void* object = env->new_object_raw(env, basic_type_id);
        SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
        *(int32_t*)&fields[0] = value;
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
        
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_LONG_OBJECT: {
        int64_t value = long_vars[opcode->operand1];
        int32_t basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_LONG_OBJECT;
        void* object = env->new_object_raw(env, basic_type_id);
        SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
        *(int64_t*)&fields[0] = value;
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
        
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_FLOAT_OBJECT: {
        float value = float_vars[opcode->operand1];
        int32_t basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT_OBJECT;
        void* object = env->new_object_raw(env, basic_type_id);
        SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
        *(float*)&fields[0] = value;
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
        
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_DOUBLE_OBJECT: {
        double value = double_vars[opcode->operand1];
        int32_t basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE_OBJECT;
        void* object = env->new_object_raw(env, basic_type_id);
        SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
        *(double*)&fields[0] = value;
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
        
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_OBJECT_TO_BYTE: {
        void* object = *(void**)&object_vars[opcode->operand1];
        if (object == NULL) {
          void* exception = env->new_string_nolen_raw(env, "Can't convert undef value.");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int32_t object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);
          int32_t object_type_dimension = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);
          if (object_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE_OBJECT && object_type_dimension == 0) {
            SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
            byte_vars[opcode->operand0] = *(int8_t*)&fields[0];
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "The source type must be Byte.");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_OBJECT_TO_SHORT: {
        void* object = *(void**)&object_vars[opcode->operand1];
        if (object == NULL) {
          void* exception = env->new_string_nolen_raw(env, "Can't convert undef value.");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int32_t object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);
          int32_t object_type_dimension = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);
          if (object_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT_OBJECT && object_type_dimension == 0) {
            SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
            short_vars[opcode->operand0] = *(int16_t*)&fields[0];
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "The source type must be Short.");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_INT_OBJECT_TO_INT: {
        void* object = *(void**)&object_vars[opcode->operand1];
        if (object == NULL) {
          void* exception = env->new_string_nolen_raw(env, "Can't convert undef value.");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int32_t object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);
          int32_t object_type_dimension = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);
          if (object_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_INT_OBJECT && object_type_dimension == 0) {
            SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
            int_vars[opcode->operand0] = *(int32_t*)&fields[0];
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "The source type must be Int.");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_LONG_OBJECT_TO_LONG: {
        void* object = *(void**)&object_vars[opcode->operand1];
        if (object == NULL) {
          void* exception = env->new_string_nolen_raw(env, "Can't convert undef value.");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int32_t object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);
          int32_t object_type_dimension = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);
          if (object_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_LONG_OBJECT && object_type_dimension == 0) {
            SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
            long_vars[opcode->operand0] = *(int64_t*)&fields[0];
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "The source type must be Long.");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
        }
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_OBJECT_TO_FLOAT: {
        void* object = *(void**)&object_vars[opcode->operand1];
        if (object == NULL) {
          void* exception = env->new_string_nolen_raw(env, "Can't convert undef value.");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int32_t object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);
          int32_t object_type_dimension = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);
          if (object_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT_OBJECT && object_type_dimension == 0) {
            SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
            float_vars[opcode->operand0] = *(float*)&fields[0];
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "The source type must be Float.");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_OBJECT_TO_DOUBLE: {
        void* object = *(void**)&object_vars[opcode->operand1];
        if (object == NULL) {
          void* exception = env->new_string_nolen_raw(env, "Can't convert undef value.");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int32_t object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);
          int32_t object_type_dimension = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);
          if (object_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE_OBJECT && object_type_dimension == 0) {
            SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
            double_vars[opcode->operand0] = *(double*)&fields[0];
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "The source type must be Double.");
            env->set_exception(env, exception);
            exception_flag = 1;
          }
        }
        
        break;
      }
    }
    opcode_rel_index++;
  }

  label_END_METHOD:
  
  // Decrement ref count of return value
  if (!exception_flag) {
    int32_t method_return_type_is_object = SPVM_API_RUNTIME_get_type_is_object(env->runtime, method->return_type_id);
    if (method_return_type_is_object) {
      if (*(void**)&stack[0] != NULL) {
        SPVM_API_DEC_REF_COUNT_ONLY(*(void**)&stack[0]);
      }
    }
  }
  
  SPVM_API_free_memory_block(env, call_stack);
  call_stack = NULL;
  
  return exception_flag;
}

int32_t SPVM_API_is_type(SPVM_ENV* env, SPVM_OBJECT* object, int32_t basic_type_id, int32_t type_dimension) {
  
  // Object must be not null
  assert(object);
  
  if (object->basic_type_id == basic_type_id && object->type_dimension == type_dimension) {
    return 1;
  }
  else {
    return 0;
  }
}

int32_t SPVM_API_is_array(SPVM_ENV* env, SPVM_OBJECT* object) {
  
  int32_t is_array;
  if (object) {
    is_array = object->type_dimension > 0;
  }
  else {
    is_array = 0;
  }
  
  return is_array;
}

int32_t SPVM_API_is_string(SPVM_ENV* env, SPVM_OBJECT* object) {
  
  int32_t is_string;
  if (object) {
    is_string = (object->basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_STRING && object->type_dimension == 0);
  }
  else {
    is_string = 0;
  }
  
  return is_string;
}

int32_t SPVM_API_is_numeric_array(SPVM_ENV* env, SPVM_OBJECT* object) {
  
  int32_t is_numeric_array;
  if (object) {
    int32_t object_type_dimension = object->type_dimension;
    if (object_type_dimension == 0) {
      is_numeric_array = 0;
    }
    else if (object_type_dimension == 1) {
      int32_t object_basic_type_id = object->basic_type_id;
      int32_t object_basic_type_category = SPVM_API_RUNTIME_get_basic_type_category(env->runtime, object_basic_type_id);
      switch (object_basic_type_category) {
        case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_NUMERIC:
        {
          is_numeric_array = 1;
          break;
        }
        default: {
          is_numeric_array = 0;
        }
      }
    }
    else if (object_type_dimension > 1) {
      is_numeric_array = 0;
    }
    else {
      assert(0);
    }
  }
  else {
    is_numeric_array = 0;
  }

  return is_numeric_array;
}


int32_t SPVM_API_is_object_array(SPVM_ENV* env, SPVM_OBJECT* object) {
  
  int32_t is_object_array;
  if (object) {
    int32_t object_type_dimension = object->type_dimension;
    if (object_type_dimension == 0) {
      is_object_array = 0;
    }
    else if (object_type_dimension == 1) {
      int32_t object_basic_type_id = object->basic_type_id;
      int32_t object_basic_type_category = SPVM_API_RUNTIME_get_basic_type_category(env->runtime, object_basic_type_id);
      
      switch (object_basic_type_category) {
        case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_STRING:
        case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_CLASS:
        case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_INTERFACE:
        case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_CALLBACK:
        case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_ANY_OBJECT:
        {
          is_object_array = 1;
          break;
        }
        default: {
          is_object_array = 0;
        }
      }
    }
    else if (object_type_dimension > 1) {
      is_object_array = 1;
    }
    else {
      assert(0);
    }
  }
  else {
    is_object_array = 0;
  }

  return is_object_array;
}

int32_t SPVM_API_is_mulnum_array(SPVM_ENV* env, SPVM_OBJECT* object) {

  int32_t is_mulnum_array;
  if (object) {
    int32_t object_type_dimension = object->type_dimension;
    if (object_type_dimension == 0) {
      is_mulnum_array = 0;
    }
    else if (object_type_dimension == 1) {
      int32_t object_basic_type_id = object->basic_type_id;
      int32_t object_basic_type_category = SPVM_API_RUNTIME_get_basic_type_category(env->runtime, object_basic_type_id);
      switch (object_basic_type_category) {
        case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_MULNUM:
        {
          is_mulnum_array = 1;
          break;
        }
        default: {
          is_mulnum_array = 0;
        }
      }
    }
    else if (object_type_dimension > 1) {
      is_mulnum_array = 0;
    }
    else {
      assert(0);
    }
  }
  else {
    is_mulnum_array = 0;
  }

  return is_mulnum_array;
}

int32_t SPVM_API_get_elem_byte_size(SPVM_ENV* env, SPVM_OBJECT* array) {
  
  SPVM_RUNTIME* runtime = env->runtime;
  
  int32_t elem_byte_size;
  if (array) {
    if (SPVM_API_is_string(env, array)) {
      elem_byte_size = 1;
    }
    else if (SPVM_API_is_object_array(env, array)) {
      elem_byte_size = sizeof(void*);
    }
    else if (SPVM_API_is_numeric_array(env, array)) {
      int32_t basic_type_id = array->basic_type_id;
      int32_t type_dimension = array->type_dimension;
      assert(type_dimension == 1);
      
      SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, basic_type_id);
      if (basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE) {
        elem_byte_size = 1;
      }
      else if (basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT) {
        elem_byte_size = 2;
      }
      else if (basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_INT || basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT) {
        elem_byte_size = 4;
      }
      else if (basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_LONG || basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE) {
        elem_byte_size = 8;
      }
      else {
        assert(0);
      }
    }
    else if (SPVM_API_is_mulnum_array(env, array)) {
      int32_t basic_type_id = array->basic_type_id;
      int32_t type_dimension = array->type_dimension;
      assert(type_dimension == 1);
      
      SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, basic_type_id);
      assert(basic_type->class_id > -1);
      SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id);
      
      int32_t width = class->fields_length;
      
      SPVM_RUNTIME_FIELD* first_field = SPVM_API_RUNTIME_get_field(env->runtime, class->fields_base_id + 0);
      int32_t first_field_type_id = first_field->type_id;
      assert(first_field_type_id > -1);
      SPVM_RUNTIME_TYPE* first_field_type =SPVM_API_RUNTIME_get_type(env->runtime, first_field_type_id);
      
      int32_t field_basic_type_id = first_field_type->basic_type_id;
      
      if (field_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE) {
        elem_byte_size = 1 * width;
      }
      else if (field_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT) {
        elem_byte_size = 2 * width;
      }
      else if (field_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_INT || field_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT) {
        elem_byte_size = 4 * width;
      }
      else if (field_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_LONG || field_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE) {
        elem_byte_size = 8 * width;
      }
      else {
        assert(0);
      }
    }
  }
  else {
    elem_byte_size = 0;
  }
  
  return elem_byte_size;
}

int32_t SPVM_API_has_callback(SPVM_ENV* env, SPVM_OBJECT* object, int32_t callback_basic_type_id) {
  (void)env;

  SPVM_RUNTIME* runtime = env->runtime;

  // Object must be not null
  assert(object);
  
  int32_t object_basic_type_id = object->basic_type_id;
  int32_t object_type_dimension = object->type_dimension;
  
  int32_t has_callback = SPVM_API_RUNTIME_has_callback_by_id(env->runtime, object_basic_type_id, object_type_dimension, callback_basic_type_id, 0);
  
  return has_callback;
}

int32_t SPVM_API_has_interface(SPVM_ENV* env, SPVM_OBJECT* object, int32_t interface_basic_type_id) {
  (void)env;

  SPVM_RUNTIME* runtime = env->runtime;

  // Object must be not null
  assert(object);
  
  int32_t object_basic_type_id = object->basic_type_id;
  int32_t object_type_dimension = object->type_dimension;
  
  int32_t has_interface = SPVM_API_RUNTIME_has_interface_by_id(env->runtime, object_basic_type_id, object_type_dimension, interface_basic_type_id, 0);
  
  return has_interface;
}

int32_t SPVM_API_enter_scope(SPVM_ENV* env) {
  (void)env;
  
  int32_t mortal_stack_top = (intptr_t)env->native_mortal_stack_top;
  
  return mortal_stack_top;
}

int32_t SPVM_API_push_mortal(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;

  if (object != NULL) {
    // Extend mortal stack
    if (env->native_mortal_stack_top >= env->native_mortal_stack_capacity) {
      int32_t new_mortal_stack_capacity = (intptr_t)env->native_mortal_stack_capacity * 2;
      SPVM_OBJECT** new_mortal_stack = SPVM_API_alloc_memory_block_zero(env, sizeof(void*) * new_mortal_stack_capacity);
      if (new_mortal_stack == NULL) {
        return 1;
      }
      memcpy(new_mortal_stack, env->native_mortal_stack, sizeof(void*) * (intptr_t)env->native_mortal_stack_capacity);
      env->native_mortal_stack_capacity = (void*)(intptr_t)new_mortal_stack_capacity;
      SPVM_API_free_memory_block(env, env->native_mortal_stack);
      env->native_mortal_stack = NULL;
      env->native_mortal_stack = new_mortal_stack;
    }
    
    ((SPVM_OBJECT**)(env->native_mortal_stack))[(intptr_t)env->native_mortal_stack_top] = object;
    env->native_mortal_stack_top = (void*)((intptr_t)env->native_mortal_stack_top + 1);
    
    object->ref_count++;
  }
  
  return 0;
}

SPVM_OBJECT* SPVM_API_get_type_name_raw(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  assert(object);
  
  SPVM_RUNTIME* runtime = env->runtime;
  
  int32_t basic_type_id = object->basic_type_id;
  int32_t type_dimension = object->type_dimension;
  
  SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, basic_type_id);
  const char* basic_type_name = SPVM_API_RUNTIME_get_basic_type_name(env->runtime, basic_type->id);
  
  int32_t length = 0;
  
  
  // Basic type
  length += strlen(basic_type_name);
  
  //[]
  length += type_dimension * 2;
  
  int32_t scope_id = env->enter_scope(env);
  void* type_name_byte_array = env->new_byte_array(env, length + 1);
  
  
  char* cur = SPVM_API_alloc_memory_block_zero(env, length + 1);
  
  int32_t cur_index = 0;
  sprintf((char*)cur, "%s", basic_type_name);
  cur_index += strlen(basic_type_name);
  
  int32_t dim_index;
  for (dim_index = 0; dim_index < type_dimension; dim_index++) {
    sprintf((char*)(cur + cur_index), "[]");
    cur_index += 2;
  }
  
  void* sv_type_name = env->new_string_raw(env, (const char*)cur, length);
  
  SPVM_API_free_memory_block(env, cur);
  
  env->leave_scope(env, scope_id);
  
  return sv_type_name;
}

SPVM_OBJECT* SPVM_API_get_type_name(SPVM_ENV* env, SPVM_OBJECT* object_in) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_get_type_name_raw(env, object_in);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

void SPVM_API_leave_scope(SPVM_ENV* env, int32_t original_mortal_stack_top) {
  (void)env;
  
  int32_t mortal_stack_index;
  for (mortal_stack_index = original_mortal_stack_top; mortal_stack_index < (intptr_t)env->native_mortal_stack_top; mortal_stack_index++) {
    SPVM_OBJECT* object = ((SPVM_OBJECT**)(env->native_mortal_stack))[mortal_stack_index];
    
    if (object != NULL) {
      if (object->ref_count > 1) {
        object->ref_count--;
      }
      else {
        SPVM_API_dec_ref_count(env, object);
      }
    }
    
    ((SPVM_OBJECT**)(env->native_mortal_stack))[mortal_stack_index] = NULL;
  }
  
  env->native_mortal_stack_top = (void*)(intptr_t)original_mortal_stack_top;
}

SPVM_OBJECT* SPVM_API_new_stack_trace_raw(SPVM_ENV* env, SPVM_OBJECT* exception, const char* class_name, const char* method_name, const char* file, int32_t line) {
  
  // stack trace symbols
  const char* new_line_part = "\n    ";
  const char* arrow_part = "->";
  const char* at_part = " at ";

  // Exception
  const char* exception_bytes = env->get_chars(env, exception);
  int32_t exception_length = env->length(env, exception);
  
  // Total string length
  int32_t total_length = 0;
  total_length += exception_length;
  total_length += strlen(new_line_part);
  total_length += strlen(class_name);
  total_length += strlen(arrow_part);
  total_length += strlen(method_name);
  total_length += strlen(at_part);
  total_length += strlen(file);

  const char* line_part = " line ";
  char line_str[20];
  
  sprintf(line_str, "%" PRId32, line);
  total_length += strlen(line_part);
  total_length += strlen(line_str);
  
  // Create exception message
  void* new_exception = env->new_string_raw(env, NULL, total_length);
  const char* new_exception_bytes = env->get_chars(env, new_exception);
  
  memcpy(
    (void*)(new_exception_bytes),
    (void*)(exception_bytes),
    exception_length
  );

  sprintf(
    (char*)new_exception_bytes + exception_length,
    "%s%s%s%s%s%s%s%" PRId32,
    new_line_part,
    class_name,
    arrow_part,
    method_name,
    at_part,
    file,
    line_part,
    line
  );
  
  return new_exception;
}

SPVM_OBJECT* SPVM_API_new_stack_trace(SPVM_ENV* env, SPVM_OBJECT* exception, const char* class_name, const char* method_name, const char* file, int32_t line) {
  (void)env;
  
  SPVM_OBJECT* str = SPVM_API_new_stack_trace_raw(env, exception, class_name, method_name, file, line);
  
  SPVM_API_push_mortal(env, str);
  
  return str;
}

void SPVM_API_fprint(SPVM_ENV* env, FILE* fh, SPVM_OBJECT* string) {
  (void)env;
  
  if (string == NULL) {
    return;
  }
  
  const char* bytes = env->get_chars(env, string);
  int32_t string_length = env->length(env, string);
  
  {
    int32_t i;
    for (i = 0; i < string_length; i++) {
      putc((char)bytes[i], fh);
    }
  }
}

void SPVM_API_print(SPVM_ENV* env, SPVM_OBJECT* string) {
  (void)env;
  
  SPVM_API_fprint(env, stdout, string);
}

void SPVM_API_print_stderr(SPVM_ENV* env, SPVM_OBJECT* string) {
  (void)env;
  
  SPVM_API_fprint(env, stderr, string);
}

SPVM_OBJECT* SPVM_API_concat_raw(SPVM_ENV* env, SPVM_OBJECT* string1, SPVM_OBJECT* string2) {
  (void)env;

  int32_t string1_length = SPVM_API_length(env, string1);
  int32_t string2_length = SPVM_API_length(env, string2);
  
  int32_t string3_length = string1_length + string2_length;
  SPVM_OBJECT* string3 = SPVM_API_new_byte_array_raw(env, string3_length);

  string3->basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_STRING;
  string3->type_dimension = 0;

  const char* string1_bytes = SPVM_API_get_chars(env, string1);
  const char* string2_bytes = SPVM_API_get_chars(env, string2);
  char* string3_bytes = (char*)SPVM_API_get_chars(env, string3);
  
  if (string1_length > 0) {
    memcpy(string3_bytes, string1_bytes, string1_length);
  }
  if (string2_length) {
    memcpy(string3_bytes + string1_length, string2_bytes, string2_length);
  }
  
  return string3;
}

SPVM_OBJECT* SPVM_API_concat(SPVM_ENV* env, SPVM_OBJECT* string1, SPVM_OBJECT* string2) {
  (void)env;
  
  SPVM_OBJECT* str = SPVM_API_concat_raw(env, string1, string2);
  
  SPVM_API_push_mortal(env, str);
  
  return str;
}

int32_t SPVM_API_get_memory_blocks_count(SPVM_ENV* env) {
  (void)env;
  
  SPVM_RUNTIME* runtime = env->runtime;
  
  SPVM_ALLOCATOR* allocator = runtime->allocator;
  
  int32_t memory_blocks_count = SPVM_ALLOCATOR_get_memory_blocks_count(allocator);
  
  return memory_blocks_count;
}

void SPVM_API_free_weaken_back_refs(SPVM_ENV* env, SPVM_WEAKEN_BACKREF* weaken_backref_head) {
  (void)env;
  
  SPVM_WEAKEN_BACKREF* weaken_backref_head_cur = weaken_backref_head;
  SPVM_WEAKEN_BACKREF* weaken_backref_head_next = NULL;
  while (weaken_backref_head_cur != NULL){
    *(weaken_backref_head_cur->object_address) = NULL;
    weaken_backref_head_next = weaken_backref_head_cur->next;
    SPVM_API_free_memory_block(env, weaken_backref_head_cur);
    weaken_backref_head_cur = NULL;
    weaken_backref_head_cur = weaken_backref_head_next;
  }
}

int32_t SPVM_API_isweak(SPVM_ENV* env, SPVM_OBJECT** object_address) {
  (void)env;
  
  int32_t isweak = (intptr_t)*object_address & 1;
  
  return isweak;
}

int32_t SPVM_API_weaken(SPVM_ENV* env, SPVM_OBJECT** object_address) {
  (void)env;
  
  assert(object_address);

  if (*object_address == NULL) {
    return 0;
  }
  
  if (SPVM_API_isweak(env, object_address)) {
    return 0;
  }
  
  SPVM_OBJECT* object = *object_address;
  
  // Decrelement reference count
  if (object->ref_count == 1) {
    // If reference count is 1, the object is freeed without weaken
    SPVM_API_dec_ref_count(env, *object_address);
    *object_address = NULL;
    return 0;
  }
  else {
    object->ref_count--;
  }

  // Create weaken_backref_head
  if (object->weaken_backref_head == NULL) {
    SPVM_WEAKEN_BACKREF* new_weaken_backref = SPVM_API_alloc_memory_block_zero(env, sizeof(SPVM_WEAKEN_BACKREF));
    if (new_weaken_backref == NULL) {
      return 1;
    }
    new_weaken_backref->object_address = object_address;
    object->weaken_backref_head = new_weaken_backref;
  }
  // Add weaken_back_ref
  else {
    SPVM_WEAKEN_BACKREF* weaken_backref_next = object->weaken_backref_head;

    SPVM_WEAKEN_BACKREF* new_weaken_backref = SPVM_API_alloc_memory_block_zero(env, sizeof(SPVM_WEAKEN_BACKREF));
    if (new_weaken_backref) {
      return 1;
    }
    new_weaken_backref->object_address = object_address;
    
    while (weaken_backref_next->next != NULL){
      weaken_backref_next = weaken_backref_next->next;
    }
    weaken_backref_next->next = new_weaken_backref;
  }
  
  // Weaken is implemented by tag pointer.
  // If pointer most right bit is 1, object is weaken.
  *object_address = (SPVM_OBJECT*)((intptr_t)*object_address | 1);
  
  return 0;
}

void SPVM_API_unweaken(SPVM_ENV* env, SPVM_OBJECT** object_address) {
  (void)env;

  assert(object_address);
  
  if (*object_address == NULL) {
    return;
  }
  
  if (!SPVM_API_isweak(env, object_address)) {
    return;
  }
  
  // Unweaken
  *object_address = (SPVM_OBJECT*)((intptr_t)*object_address & ~(intptr_t)1);
  
  SPVM_OBJECT* object = *object_address;
  
  // Increment reference count
  object->ref_count++;

  // Remove weaken back ref
  SPVM_WEAKEN_BACKREF** weaken_backref_next_address = &object->weaken_backref_head;
  assert(*weaken_backref_next_address);
  
  int32_t pass_one = 0;
  while (*weaken_backref_next_address != NULL){
    if ((*weaken_backref_next_address)->object_address == object_address) {
      pass_one++;
      SPVM_WEAKEN_BACKREF* tmp = (*weaken_backref_next_address)->next;
      SPVM_API_free_memory_block(env, *weaken_backref_next_address);
      *weaken_backref_next_address = NULL;
      *weaken_backref_next_address = tmp;
      break;
    }
    *weaken_backref_next_address = (*weaken_backref_next_address)->next;
  }
  assert(pass_one == 1);
}

int32_t SPVM_API_set_exception(SPVM_ENV* env, SPVM_OBJECT* exception) {
  if (env->exception_object != NULL) {
    SPVM_API_dec_ref_count(env, (SPVM_OBJECT*)env->exception_object);
  }
  
  SPVM_API_OBJECT_ASSIGN(&env->exception_object, exception);
  
  if (env->exception_object != NULL) {
    ((SPVM_OBJECT*)env->exception_object)->ref_count++;
  }
  
  return 1;
}

SPVM_OBJECT* SPVM_API_exception(SPVM_ENV* env) {
  (void)env;
  
  return env->exception_object;
}

SPVM_OBJECT* SPVM_API_new_byte_array(SPVM_ENV* env, int32_t length) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_byte_array_raw(env, length);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_short_array(SPVM_ENV* env, int32_t length) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_short_array_raw(env, length);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_int_array(SPVM_ENV* env, int32_t length) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_int_array_raw(env, length);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_long_array(SPVM_ENV* env, int32_t length) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_long_array_raw(env, length);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_float_array(SPVM_ENV* env, int32_t length) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_float_array_raw(env, length);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_double_array(SPVM_ENV* env, int32_t length) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_double_array_raw(env, length);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_object_array(SPVM_ENV* env, int32_t basic_type_id, int32_t length) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_object_array_raw(env, basic_type_id, length);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_muldim_array(SPVM_ENV* env, int32_t basic_type_id, int32_t element_dimension, int32_t length) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_muldim_array_raw(env, basic_type_id, element_dimension, length);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_mulnum_array(SPVM_ENV* env, int32_t basic_type_id, int32_t length) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_mulnum_array_raw(env, basic_type_id, length);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_object(SPVM_ENV* env, int32_t basic_type_id) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_object_raw(env, basic_type_id);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_pointer(SPVM_ENV* env, int32_t basic_type_id, void* pointer) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_pointer_raw(env, basic_type_id, pointer);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_string_nolen_raw(SPVM_ENV* env, const char* bytes) {
  (void)env;
  
  int32_t length = strlen((char*)bytes);
  
  SPVM_OBJECT* object = SPVM_API_new_byte_array_raw(env, length);
  
  object->basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_STRING;
  object->type_dimension = 0;
  
  if (bytes != NULL && length > 0) {
    memcpy((void*)((intptr_t)object + env->object_header_byte_size), (char*)bytes, length);
  }
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_string_nolen(SPVM_ENV* env, const char* bytes) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_string_nolen_raw(env, bytes);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_string_raw(SPVM_ENV* env, const char* bytes, int32_t length) {
  (void)env;

  SPVM_OBJECT* object = SPVM_API_new_byte_array_raw(env, length);
  
  object->basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_STRING;
  object->type_dimension = 0;

  if (bytes != NULL && length > 0) {
    memcpy((void*)((intptr_t)object + env->object_header_byte_size), (char*)bytes, length);
  }

  return object;
}

int32_t SPVM_API_get_filed_first_int(SPVM_ENV* env, SPVM_OBJECT* object) {

  int32_t value = *(int32_t*)((intptr_t)object + env->object_header_byte_size);
  
  return value;
}

int32_t SPVM_API_get_bool_object_value(SPVM_ENV* env, SPVM_OBJECT* bool_object) {
  (void)env;

  int32_t value = SPVM_API_get_filed_first_int(env, bool_object);
  
  return value;
}

SPVM_OBJECT* SPVM_API_new_string(SPVM_ENV* env, const char* bytes, int32_t length) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_string_raw(env, bytes, length);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_byte_array_raw(SPVM_ENV* env, int32_t length) {
  (void)env;
  
  // If lenght is less than 0, return NULL.
  if (length < 0) {
    return NULL;
  }
  
  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + sizeof(int8_t) * ((int64_t)length + 1);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }

  object->type_dimension = 1;
  object->basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE;
  object->length = length;
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_short_array_raw(SPVM_ENV* env, int32_t length) {
  (void)env;
  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + sizeof(int16_t) * ((int64_t)length + 1);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }
  
  object->type_dimension = 1;
  object->basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT;
  
  // Set array length
  object->length = length;

  return object;
}

SPVM_OBJECT* SPVM_API_new_int_array_raw(SPVM_ENV* env, int32_t length) {
  (void)env;

  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + sizeof(int32_t) * ((int64_t)length + 1);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }
  
  object->type_dimension = 1;
  object->basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_INT;

  // Set array length
  object->length = length;

  return object;
}

SPVM_OBJECT* SPVM_API_new_long_array_raw(SPVM_ENV* env, int32_t length) {
  (void)env;

  if (length < 0) {
    return NULL;
  }
  
  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + sizeof(int64_t) * ((int64_t)length + 1);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }
  
  object->type_dimension = 1;
  object->basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_LONG;

  // Set array length
  object->length = length;

  return object;
}

SPVM_OBJECT* SPVM_API_new_float_array_raw(SPVM_ENV* env, int32_t length) {
  (void)env;

  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + sizeof(float) * ((int64_t)length + 1);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }
  
  object->type_dimension = 1;
  object->basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT;

  // Set array length
  object->length = length;

  return object;
}

SPVM_OBJECT* SPVM_API_new_double_array_raw(SPVM_ENV* env, int32_t length) {
  (void)env;

  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + sizeof(double) * ((int64_t)length + 1);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }
  
  object->type_dimension = 1;
  object->basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE;
  
  // Set array length
  object->length = length;

  return object;
}

SPVM_OBJECT* SPVM_API_new_object_array_raw(SPVM_ENV* env, int32_t basic_type_id, int32_t length) {
  (void)env;
  
  SPVM_RUNTIME* runtime = env->runtime;

  SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, basic_type_id);
  
  if (!basic_type) {
    return NULL;
  }
  
  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + sizeof(void*) * ((int64_t)length + 1);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }

  for (int32_t index = 0; index < length; index++) {
    SPVM_OBJECT* get_field_object = ((SPVM_OBJECT**)((intptr_t)object + env->object_header_byte_size))[index];
  }

  object->basic_type_id = basic_type->id;
  object->type_dimension = 1;

  // Set array length
  object->length = length;
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_muldim_array_raw(SPVM_ENV* env, int32_t basic_type_id, int32_t element_dimension, int32_t length) {
  (void)env;
  
  if (element_dimension < 1) {
    return NULL;
  }
  else if (basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_ANY_OBJECT) {
    return NULL;
  }
  
  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + sizeof(void*) * ((int64_t)length + 1);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }
  
  object->basic_type_id = basic_type_id;
  object->type_dimension = element_dimension + 1;
  
  // Set array length
  object->length = length;
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_mulnum_array_raw(SPVM_ENV* env, int32_t basic_type_id, int32_t length) {
  (void)env;

  SPVM_RUNTIME* runtime = env->runtime;

  // valut_t array dimension must be 1
  SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, basic_type_id);
  const char* basic_type_name = SPVM_API_RUNTIME_get_basic_type_name(env->runtime, basic_type->id);
  
  // Class
  SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id);
  int32_t fields_length = class->fields_length;
  SPVM_RUNTIME_FIELD* field_first = SPVM_API_RUNTIME_get_field(env->runtime, class->fields_base_id + 0);

  SPVM_RUNTIME_TYPE* first_field_type =SPVM_API_RUNTIME_get_type(env->runtime, field_first->type_id);

  int32_t field_basic_type_id = first_field_type->basic_type_id;

  int32_t unit_size;
  if (field_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE) {
    unit_size = sizeof(int8_t);
  }
  else if (field_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT) {
    unit_size = sizeof(int16_t);
  }
  else if (field_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_INT) {
    unit_size = sizeof(int32_t);
  }
  else if (field_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_LONG) {
    unit_size = sizeof(int64_t);
  }
  else if (field_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT) {
    unit_size = sizeof(float);
  }
  else if (field_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE) {
    unit_size = sizeof(double);
  }
  else {
    assert(0);
  }

  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + unit_size * fields_length * ((int64_t)length + 1);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }

  object->basic_type_id = basic_type->id;
  object->type_dimension = 1;

  // Set array length
  object->length = length;

  return object;
}

SPVM_OBJECT* SPVM_API_new_object_raw(SPVM_ENV* env, int32_t basic_type_id) {
  (void)env;
  
  SPVM_RUNTIME* runtime = env->runtime;
  
  SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, basic_type_id);
  
  SPVM_RUNTIME_CLASS* class;
  if (!SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id)) {
    class = NULL;
  }
  else {
    class = SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id);
  }
  if (!class) {
    return NULL;
  }

  // Alloc body length + 1
  int32_t fields_length = class->fields_length;

  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + sizeof(SPVM_VALUE) * ((int64_t)fields_length + 1);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }
  
  object->basic_type_id = basic_type->id;
  object->type_dimension = 0;

  object->length = fields_length;

  // Has destructor
  if (class->method_destructor_id >= 0) {
    object->flag |= SPVM_OBJECT_C_FLAG_HAS_DESTRUCTOR;
  }
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_pointer_raw(SPVM_ENV* env, int32_t basic_type_id, void* pointer) {
  (void)env;
  
  SPVM_RUNTIME* runtime = env->runtime;
  
  SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, basic_type_id);

  SPVM_RUNTIME_CLASS* class;
  if (!SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id)) {
    class = NULL;
  }
  else {
    class = SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id);
  }
  if (!class) {
    return NULL;
  }

  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + sizeof(void*);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }
  
  *(void**)((intptr_t)object + (intptr_t)env->object_header_byte_size) = pointer;

  object->basic_type_id = basic_type->id;
  object->type_dimension = 0;

  object->length = 0;

  // Has destructor
  if (SPVM_API_RUNTIME_get_method(env->runtime, class->method_destructor_id)) {
    object->flag |= SPVM_OBJECT_C_FLAG_HAS_DESTRUCTOR;
  }
  
  return object;
}

int32_t SPVM_API_get_object_basic_type_id(SPVM_ENV* env, SPVM_OBJECT* object) {
  return object->basic_type_id;
}

int32_t SPVM_API_get_object_type_dimension(SPVM_ENV* env, SPVM_OBJECT* object) {
  return object->type_dimension;
}

int32_t SPVM_API_length(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  int32_t length = object->length;
  
  return length;
}

int8_t* SPVM_API_get_elems_byte(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;

  return (int8_t*)((intptr_t)object + env->object_header_byte_size);
}

const char* SPVM_API_get_chars(SPVM_ENV* env, SPVM_OBJECT* string) {
  (void)env;

  return (const char*)((intptr_t)string + env->object_header_byte_size);
}

int16_t* SPVM_API_get_elems_short(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  return (int16_t*)((intptr_t)object + env->object_header_byte_size);
}

int32_t* SPVM_API_get_elems_int(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  return (int32_t*)((intptr_t)object + env->object_header_byte_size);
}

int64_t* SPVM_API_get_elems_long(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  return (int64_t*)((intptr_t)object + env->object_header_byte_size);
}

float* SPVM_API_get_elems_float(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  return (float*)((intptr_t)object + env->object_header_byte_size);
}

double* SPVM_API_get_elems_double(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  return (double*)((intptr_t)object + env->object_header_byte_size);
}

SPVM_OBJECT* SPVM_API_get_elem_object(SPVM_ENV* env, SPVM_OBJECT* array, int32_t index) {
  (void)env;
  
  SPVM_OBJECT* object_maybe_weaken = ((SPVM_OBJECT**)((intptr_t)array + env->object_header_byte_size))[index];
  SPVM_OBJECT* object = SPVM_API_GET_OBJECT_NO_WEAKEN_ADDRESS(object_maybe_weaken);
  
  return object;
}

void SPVM_API_set_elem_object(SPVM_ENV* env, SPVM_OBJECT* array, int32_t index, SPVM_OBJECT* oval) {
  (void)env;
  
  void* object_address = &((void**)((intptr_t)array + env->object_header_byte_size))[index];
  
  SPVM_API_OBJECT_ASSIGN(object_address, oval);
}

void* SPVM_API_pointer(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  return *(void**)((intptr_t)object + (intptr_t)env->object_header_byte_size);
}

void SPVM_API_set_pointer(SPVM_ENV* env, SPVM_OBJECT* object, void* ptr) {
  (void)env;
  
  *(void**)((intptr_t)object + (intptr_t)env->object_header_byte_size) = ptr;
}

void SPVM_API_dec_ref_count(SPVM_ENV* env, SPVM_OBJECT* object) {
  
  assert(object != NULL);
  assert(object->ref_count > 0);
  
  // Not weakened
  assert((((intptr_t)object) & 1) == 0);
  
  // If reference count is zero, free address.
  if (object->ref_count == 1) {
    // Free object array
    if (SPVM_API_is_object_array(env, object)) {
      int32_t length = object->length;
      for (int32_t index = 0; index < length; index++) {
        SPVM_OBJECT** get_field_object_address = &(((SPVM_OBJECT**)((intptr_t)object + env->object_header_byte_size))[index]);

        if (*get_field_object_address != NULL) {
          SPVM_API_dec_ref_count(env, *get_field_object_address);
        }
      }
    }
    // Free object
    else {
      int32_t object_basic_type_id = object->basic_type_id;
      int32_t object_basic_type_category = SPVM_API_RUNTIME_get_basic_type_category(env->runtime, object_basic_type_id);
      if (object_basic_type_category == SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_CLASS) {
        // Class
        SPVM_RUNTIME* runtime = env->runtime;
        SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, object->basic_type_id);
        SPVM_RUNTIME_CLASS* class;
        if (!SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id)) {
          class = NULL;
        }
        else {
          class = SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id);
        }
        
        int32_t is_pointer = 0;
        if (class) {
          if (class->flag & SPVM_CLASS_C_FLAG_POINTER) {
            is_pointer = 1;
          }
        }
        
        // Call destructor
        if (object->flag & SPVM_OBJECT_C_FLAG_HAS_DESTRUCTOR) {
          SPVM_VALUE args[1];
          args[0].oval = object;
          int32_t exception_flag = SPVM_API_call_spvm_method(env, class->method_destructor_id, args);
          
          // Exception in destructor is changed to warning
          if (exception_flag) {
            void* exception = env->get_exception(env);
            const char* exception_chars = env->get_chars(env, exception);
            fprintf(stderr, "(in cleanup) %s\n", exception_chars);
          }
          
          assert(object->ref_count > 0);
        }
        
        // Free object fields
        int32_t object_fields_offset = class->object_fields_offset;
        int32_t object_fields_length = class->object_fields_length;
        for (int32_t index = 0; index < object_fields_length; index++) {
          SPVM_OBJECT** get_field_object_address = &(((SPVM_OBJECT**)((intptr_t)object + (intptr_t)env->object_header_byte_size + object_fields_offset))[index]);
          if (*get_field_object_address != NULL) {
            // If object is weak, unweaken
            if (SPVM_API_isweak(env, get_field_object_address)) {
              SPVM_API_unweaken(env, get_field_object_address);
            }
            
            SPVM_API_dec_ref_count(env, *get_field_object_address);
          }
        }
      }
    }
    
    // Free weak back refenreces
    if (object->weaken_backref_head != NULL) {
      SPVM_API_free_weaken_back_refs(env, object->weaken_backref_head);
      object->weaken_backref_head = NULL;
    }
    
    // Decrement reference count
    object->ref_count--;
  
    // Free object
    SPVM_API_free_memory_block(env, object);
    object = NULL;
  }
  else {
    // Decrement reference count
    object->ref_count--;
  }
}

void SPVM_API_inc_ref_count(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  if (object != NULL) {
    assert(object->ref_count >= 0);
    // Increment reference count
    object->ref_count++;
  }
}

int32_t SPVM_API_get_basic_type_id(SPVM_ENV* env, const char* basic_type_name) {
  (void)env;

  if (basic_type_name == NULL) {
    return -1;
  }

  SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type_by_name(env->runtime, basic_type_name);
  if (basic_type) {
    int32_t basic_type_id = basic_type->id;
    return basic_type_id;
  }
  else {
    return -1;
  }
}

int32_t SPVM_API_ref_count(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  return object->ref_count;
}

int32_t SPVM_API_get_field_offset(SPVM_ENV* env, int32_t field_id) {
  (void)env;
  
  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
  
  return field->offset;
}

int32_t SPVM_API_get_field_id(SPVM_ENV* env, const char* class_name, const char* field_name, const char* signature) {
  (void)env;
  
  // Basic type
  SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type_by_name(env->runtime, class_name);
  if (!basic_type) {
    return -1;
  }
  
  if (!SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id)) {
    return -1;
  }
  
  // Class
  SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id);
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_get_runtime_field_from_runtime_class(env, class->id, field_name);
  if (!field) {
    return -1;
  }

  // Signature
  const char* field_signature = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, field->signature_id, NULL);
  if (strcmp(signature, field_signature) != 0) {
    return -1;
  }
  
  int32_t field_id = field->id;
  
  return field_id;
}

int32_t SPVM_API_get_class_var_id(SPVM_ENV* env, const char* class_name, const char* class_var_name, const char* signature) {
  (void)env;
  
  // Basic type
  SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type_by_name(env->runtime, class_name);
  
  // Class name
  SPVM_RUNTIME_CLASS* class;
  if (!basic_type) {
    return -1;
  }
  else if (!SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id)) {
    return -1;
  }
  else {
    class = SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id);
  }

  // Class variable name
  SPVM_RUNTIME_CLASS_VAR* class_var = SPVM_API_get_runtime_class_var_from_runtime_class(env, class->id, class_var_name);
  if (!class_var) {
    return -1;
  }
  
  const char* class_var_signature = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, class_var->signature_id, NULL);
  
  // Signature
  if (strcmp(signature, class_var_signature) != 0) {
    return -1;
  }
 
  return class_var->id;
}

SPVM_RUNTIME_METHOD* SPVM_API_get_runtime_method_from_runtime_class(SPVM_ENV* env, int32_t class_id, const char* search_method_name) {
  
  SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, class_id);
  
  SPVM_RUNTIME_METHOD* found_method = NULL;
  if (class->methods_length > 0) {
    for (int32_t method_id = class->methods_base_id; method_id <  class->methods_base_id + class->methods_length; method_id++) {
      SPVM_RUNTIME_METHOD* method = SPVM_API_RUNTIME_get_method(env->runtime, method_id);
      const char* method_name = SPVM_API_RUNTIME_get_name(env->runtime, method->name_id);
      if (strcmp(method_name, search_method_name) == 0) {
        found_method = method;
        break;
      }
    }
  }
  
  return found_method;
}

SPVM_RUNTIME_FIELD* SPVM_API_get_runtime_field_from_runtime_class(SPVM_ENV* env, int32_t class_id, const char* search_field_name) {
  
  SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, class_id);
  
  SPVM_RUNTIME_FIELD* found_field = NULL;
  if (class->fields_length > 0) {
    for (int32_t field_id = class->fields_base_id; field_id <  class->fields_base_id + class->fields_length; field_id++) {
      SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
      const char* field_name = SPVM_API_RUNTIME_get_name(env->runtime, field->name_id);
      if (strcmp(field_name, search_field_name) == 0) {
        found_field = field;
        break;
      }
    }
  }
  
  return found_field;
}

SPVM_RUNTIME_CLASS_VAR* SPVM_API_get_runtime_class_var_from_runtime_class(SPVM_ENV* env, int32_t class_id, const char* search_class_var_name) {
  
  SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, class_id);
  
  SPVM_RUNTIME_CLASS_VAR* found_class_var = NULL;
  if (class->class_vars_length > 0) {
    for (int32_t class_var_id = class->class_vars_base_id; class_var_id <  class->class_vars_base_id + class->class_vars_length; class_var_id++) {
      SPVM_RUNTIME_CLASS_VAR* class_var = SPVM_API_RUNTIME_get_class_var(env->runtime, class_var_id);
      const char* class_var_name = SPVM_API_RUNTIME_get_name(env->runtime, class_var->name_id);
      if (strcmp(class_var_name, search_class_var_name) == 0) {
        found_class_var = class_var;
        break;
      }
    }
  }
  
  return found_class_var;
}

SPVM_RUNTIME_FIELD* SPVM_API_get_runtime_field_from_index(SPVM_ENV* env, int32_t class_id, int32_t field_index) {
  
  SPVM_RUNTIME* runtime = env->runtime;
  
  SPVM_RUNTIME_FIELD* field = NULL;
  SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, class_id);
  if (class) {
    int32_t field_id = class->fields_base_id + field_index;
    field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
  }
  
  return field;
}

SPVM_RUNTIME_METHOD* SPVM_API_get_runtime_method_from_index(SPVM_ENV* env, int32_t class_id, int32_t method_index) {
  
  SPVM_RUNTIME* runtime = env->runtime;
  
  SPVM_RUNTIME_METHOD* method = NULL;
  SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, class_id);
  if (class) {
    int32_t method_id = class->methods_base_id +  method_index;
    method = SPVM_API_RUNTIME_get_method(env->runtime, method_id);
  }
  
  return method;
}

int32_t SPVM_API_get_class_method_id(SPVM_ENV* env, const char* class_name, const char* method_name, const char* signature) {
  (void)env;
  
  // Method ID
  int32_t method_id = -1;
  
  // Basic type
  SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type_by_name(env->runtime, class_name);
  if (basic_type) {
    
    // Class
    SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id);
    if (class) {

      // Method
      SPVM_RUNTIME_METHOD* method = SPVM_API_get_runtime_method_from_runtime_class(env, class->id, method_name);
      
      if (method) {
        // Class method
        if (method->is_class_method) {
          // Signature
          const char* method_signature = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, method->signature_id, NULL);
          if (strcmp(signature, method_signature) == 0) {
            method_id = method->id;
          }
        }
      }
    }
  }
  
  return method_id;
}

int32_t SPVM_API_get_instance_method_id_static(SPVM_ENV* env, const char* class_name, const char* method_name, const char* signature) {
  (void)env;
  
  // Method ID
  int32_t method_id = -1;
  
  // Basic type
  SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type_by_name(env->runtime, class_name);
  if (basic_type) {
    
    // Class
    SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id);
    if (class) {

      // Method
      SPVM_RUNTIME_METHOD* method = SPVM_API_get_runtime_method_from_runtime_class(env, class->id, method_name);
      if (method) {
        // Instance method
        if (!method->is_class_method) {
          // Signature
          const char* method_signature = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, method->signature_id, NULL);
          if (strcmp(signature, method_signature) == 0) {
            method_id = method->id;
          }
        }
      }
    }
  }
  
  return method_id;
}


int32_t SPVM_API_get_instance_method_id(SPVM_ENV* env, SPVM_OBJECT* object, const char* method_name, const char* signature) {
  (void)env;
  
  // Method ID
  int32_t method_id = -1;
  
  // Compiler
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Basic type
  SPVM_RUNTIME_BASIC_TYPE* basic_type = SPVM_API_RUNTIME_get_basic_type(env->runtime, object->basic_type_id);
  
  // Class
  SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, basic_type->class_id);
  if (class) {
    // Method
    SPVM_RUNTIME_METHOD* method = NULL;
    
    // Anon instance method
    if (class->flag & SPVM_CLASS_C_FLAG_ANON_METHOD_CLASS) {
      // Method name
      int32_t method_id = class->methods_base_id;
      method = SPVM_API_RUNTIME_get_method(env->runtime, method_id);
    }
    // Normal instance method
    else {
      method = SPVM_API_get_runtime_method_from_runtime_class(env, class->id, method_name);
    }
    if (method) {
      // Instance method
      if (!method->is_class_method) {
        // Signature
        const char* method_signature = SPVM_API_RUNTIME_get_constant_string_value(env->runtime, method->signature_id, NULL);
        if (strcmp(signature, method_signature) == 0) {
          method_id = method->id;
        }
      }
    }
  }

  return method_id;
}

int8_t SPVM_API_get_field_byte(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
  
  // Get field value
  int8_t value = *(int8_t*)((intptr_t)object + env->object_header_byte_size + field->offset);

  return value;
}

int16_t SPVM_API_get_field_short(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
  
  // Get field value
  int16_t value = *(int16_t*)((intptr_t)object + env->object_header_byte_size + field->offset);
  
  return value;
}

int32_t SPVM_API_get_field_int(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);

  // Get field value
  int32_t value = *(int32_t*)((intptr_t)object + env->object_header_byte_size + field->offset);
  
  return value;
}

int64_t SPVM_API_get_field_long(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);

  // Get field value
  int64_t value = *(int64_t*)((intptr_t)object + env->object_header_byte_size + field->offset);
  
  return value;
}

float SPVM_API_get_field_float(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);

  // Get field value
  float value = *(float*)((intptr_t)object + env->object_header_byte_size + field->offset);
  
  return value;
}

double SPVM_API_get_field_double(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);

  // Get field value
  double value = *(double*)((intptr_t)object + env->object_header_byte_size + field->offset);
  
  return value;
}

SPVM_OBJECT* SPVM_API_get_field_object(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
  
  // Get field value
  SPVM_OBJECT* value_maybe_weaken = *(SPVM_OBJECT**)((intptr_t)object + env->object_header_byte_size + field->offset);
  SPVM_OBJECT* value = SPVM_API_GET_OBJECT_NO_WEAKEN_ADDRESS(value_maybe_weaken);
  
  return value;
}

void SPVM_API_set_field_byte(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, int8_t value) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);

  // Get field value
  *(int8_t*)((intptr_t)object + env->object_header_byte_size + field->offset) = value;
}

void SPVM_API_set_field_short(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, int16_t value) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);

  // Get field value
  *(int16_t*)((intptr_t)object + env->object_header_byte_size + field->offset) = value;
}

void SPVM_API_set_field_int(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, int32_t value) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);

  // Get field value
  *(int32_t*)((intptr_t)object + env->object_header_byte_size + field->offset) = value;
}

void SPVM_API_set_field_long(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, int64_t value) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);

  // Get field value
  *(int64_t*)((intptr_t)object + env->object_header_byte_size + field->offset) = value;
}

void SPVM_API_set_field_float(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, float value) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);

  // Get field value
  *(float*)((intptr_t)object + env->object_header_byte_size + field->offset) = value;
}

void SPVM_API_set_field_double(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, double value) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);

  // Get field value
  *(double*)((intptr_t)object + env->object_header_byte_size + field->offset) = value;
}

void SPVM_API_set_field_object(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, SPVM_OBJECT* value) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Field
  SPVM_RUNTIME_FIELD* field = SPVM_API_RUNTIME_get_field(env->runtime, field_id);
  
  // Get field value
  void* get_field_object_address = (void**)((intptr_t)object + env->object_header_byte_size + field->offset);

  SPVM_API_OBJECT_ASSIGN(get_field_object_address, value);
}

void* SPVM_API_alloc_memory_block_zero(SPVM_ENV* env, size_t byte_size) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  assert(byte_size > 0);

  if ((uint64_t)byte_size > (uint64_t)SIZE_MAX) {
    return NULL;
  }
  
  void* block = SPVM_ALLOCATOR_alloc_memory_block_tmp(runtime->allocator, (size_t)byte_size);
  
#ifdef SPVM_DEBUG_ALLOC_MEMORY_COUNT
    SPVM_ALLOCATOR* allocator = runtime->allocator;
    fprintf(stderr, "[ALLOC_MEMORY %p (All:%d, Tmp:%d, Perm: %d]\n", block,
      (int32_t)(intptr_t)env->get_memory_blocks_count(env), allocator->memory_blocks_count_tmp, allocator->memory_blocks_count_permanent);
#endif

  return block;
}

void SPVM_API_free_memory_block(SPVM_ENV* env, void* block) {

  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;

  if (block) {
    SPVM_ALLOCATOR_free_memory_block_tmp(runtime->allocator, block);
#ifdef SPVM_DEBUG_ALLOC_MEMORY_COUNT
    SPVM_ALLOCATOR* allocator = runtime->allocator;
    fprintf(stderr, "[FREE_MEMORY %p (All:%d, Tmp:%d, Perm: %d]\n", block,
      (int32_t)(intptr_t)env->get_memory_blocks_count(env), allocator->memory_blocks_count_tmp, allocator->memory_blocks_count_permanent);
#endif
  }
}

int8_t SPVM_API_get_class_var_byte(SPVM_ENV* env, int32_t packagke_var_id) {

  // Get field value
  int8_t value = ((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].bval;

  return value;
}

int16_t SPVM_API_get_class_var_short(SPVM_ENV* env, int32_t packagke_var_id) {

  // Get field value
  int16_t value = ((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].sval;

  return value;
}

int32_t SPVM_API_get_class_var_int(SPVM_ENV* env, int32_t packagke_var_id) {

  // Get field value
  int32_t value = ((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].ival;

  return value;
}

int64_t SPVM_API_get_class_var_long(SPVM_ENV* env, int32_t packagke_var_id) {

  // Get field value
  int64_t value = ((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].lval;

  return value;
}

float SPVM_API_get_class_var_float(SPVM_ENV* env, int32_t packagke_var_id) {

  // Get field value
  float value = ((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].fval;

  return value;
}

double SPVM_API_get_class_var_double(SPVM_ENV* env, int32_t packagke_var_id) {

  // Get field value
  double value = ((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].dval;

  return value;
}

SPVM_OBJECT* SPVM_API_get_class_var_object(SPVM_ENV* env, int32_t packagke_var_id) {

  // Get field value
  SPVM_OBJECT* value_maybe_weaken = (SPVM_OBJECT*)((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].oval;
  SPVM_OBJECT* value = SPVM_API_GET_OBJECT_NO_WEAKEN_ADDRESS(value_maybe_weaken);
  
  return value;
}

void SPVM_API_set_class_var_byte(SPVM_ENV* env, int32_t packagke_var_id, int8_t value) {
  
  // Get field value
  ((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].bval = value;
}

void SPVM_API_set_class_var_short(SPVM_ENV* env, int32_t packagke_var_id, int16_t value) {

  // Get field value
  ((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].sval = value;
}

void SPVM_API_set_class_var_int(SPVM_ENV* env, int32_t packagke_var_id, int32_t value) {

  // Get field value
  ((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].ival = value;
}

void SPVM_API_set_class_var_long(SPVM_ENV* env, int32_t packagke_var_id, int64_t value) {

  // Get field value
  ((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].lval = value;
}

void SPVM_API_set_class_var_float(SPVM_ENV* env, int32_t packagke_var_id, float value) {

  // Get field value
  ((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].fval = value;
}

void SPVM_API_set_class_var_double(SPVM_ENV* env, int32_t packagke_var_id, double value) {

  // Get field value
  ((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].dval = value;
}

void SPVM_API_set_class_var_object(SPVM_ENV* env, int32_t packagke_var_id, SPVM_OBJECT* value) {

  // Get field value
  void* get_field_object_address = &((SPVM_VALUE*)(env->class_vars_heap))[packagke_var_id].oval;
  SPVM_API_OBJECT_ASSIGN(get_field_object_address, value);
}


SPVM_OBJECT* SPVM_API_new_array_proto_raw(SPVM_ENV* env, SPVM_OBJECT* array, int32_t length) {

  if (array == NULL) {
    return NULL;
  }
  
  if (length < 0) {
    return NULL;
  }
  
  int32_t element_byte_size = env->get_elem_byte_size(env, array);
  

  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + element_byte_size * ((int64_t)length + 1);
  
  // Create object
  SPVM_OBJECT* new_array = SPVM_API_alloc_memory_block_zero(env, (size_t)alloc_byte_size);
  if (!new_array) {
    return NULL;
  }

  new_array->basic_type_id = array->basic_type_id;
  new_array->type_dimension = array->type_dimension;
  new_array->length = length;
  
  return new_array;
}

SPVM_OBJECT* SPVM_API_new_array_proto(SPVM_ENV* env, SPVM_OBJECT* array, int32_t length) {
  (void)env;
  
  SPVM_OBJECT* object = SPVM_API_new_array_proto_raw(env, array, length);
  
  SPVM_API_push_mortal(env, object);
  
  return object;
}

SPVM_OBJECT* SPVM_API_copy_raw(SPVM_ENV* env, SPVM_OBJECT* object) {
  if (!object) {
    return NULL;
  }
  
  SPVM_OBJECT* new_object;
  
  int32_t length = object->length;
  
  if (env->is_string(env, object)) {
    new_object = env->new_string_raw(env, NULL, length);
    
    const char* object_chars = env->get_chars(env, object);
    char* new_object_chars = (char*)env->get_chars(env, new_object);
    
    memcpy(new_object_chars, object_chars, length);
  }
  else if (env->is_numeric_array(env, object) || env->is_mulnum_array(env, object)) {
    new_object = env->new_array_proto_raw(env, object, length);
    
    const char* object_bytes = (const char*)env->get_elems_byte(env, object);
    char* new_object_bytes = (char*)env->get_elems_byte(env, new_object);
    
    int32_t element_byte_size = env->get_elem_byte_size(env, object);
    
    memcpy(new_object_bytes, object_bytes, element_byte_size * length);
  }
  else {
    new_object = NULL;
  }
  
  return new_object;
}

SPVM_OBJECT* SPVM_API_copy(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  SPVM_OBJECT* new_object = SPVM_API_copy_raw(env, object);
  
  SPVM_API_push_mortal(env, new_object);
  
  return new_object;
}

void SPVM_API_shorten(SPVM_ENV* env, SPVM_OBJECT* string, int32_t new_length) {
  (void)env;
  
  if (string != NULL) {
    if (env->is_string(env, string)) {
      if (!env->is_read_only(env, string)) {
        int32_t length = string->length;
        
        if (new_length > length) {
          new_length = length;
        }
        else if (new_length < 0) {
          new_length = 0;
        }
        
        string->length = new_length;
        char* chars = (char*)env->get_chars(env, string);
        if (new_length > length) {
          memset(chars + new_length, 0, new_length - length);
        }
      }
    }
  }
}

void SPVM_API_set_no_symbol_cache_flag(SPVM_ENV* env, int32_t flag) {
  (void)env;

  env->no_symbol_cache_flag = (void*)(intptr_t)flag;
}

int32_t SPVM_API_get_no_symbol_cache_flag(SPVM_ENV* env) {
  (void)env;
  
  return (int32_t)(intptr_t)env->no_symbol_cache_flag;
}

void SPVM_API_call_init_blocks(SPVM_ENV* env) {
  (void)env;
  
  // Runtime
  SPVM_RUNTIME* runtime = env->runtime;
  
  // Call INIT blocks
  int32_t classes_length = runtime->classes_length;
  SPVM_VALUE stack[256];
  for (int32_t class_id = 0; class_id < classes_length; class_id++) {
    
    SPVM_RUNTIME_CLASS* class = SPVM_API_RUNTIME_get_class(env->runtime, class_id);
    
    if (class->has_init_block) {
      SPVM_RUNTIME_METHOD* init_method = SPVM_API_get_runtime_method_from_runtime_class(env, class->id, "INIT");
      assert(init_method);
      env->call_spvm_method(env, init_method->id, stack);
    }
  }
}

SPVM_ENV* SPVM_API_new_env(SPVM_ENV* env) {
  (void)env;
  
  // New raw env
  SPVM_ENV* new_env = SPVM_API_new_env_raw(NULL);
  
  // Set the runtime
  new_env->runtime = env->runtime;
  
  // Initialize env
  new_env->init_env(new_env);
  
  // Call init blocks
  new_env->call_init_blocks(new_env);
  
  return new_env;
}

void SPVM_API_free_env(SPVM_ENV* env) {
  (void)env;
  
  env->cleanup_global_vars(env);
  
  SPVM_RUNTIME* runtime = env->runtime;
  SPVM_ALLOCATOR* allocator = runtime->allocator;
  SPVM_ALLOCATOR_free(allocator);
  runtime->allocator = NULL;
  allocator= NULL;
  
  env->free_env_raw(env);
}

void SPVM_API_free_env_prepared(SPVM_ENV* env) {

  // Cleanup global variables
  env->cleanup_global_vars(env);

  // Free runtime
  SPVM_API_RUNTIME_free_runtime(env->runtime);
  env->runtime = NULL;
  
  // Free env
  env->free_env_raw(env);
}

int32_t SPVM_API_can_assign_array_element(SPVM_ENV* env, SPVM_OBJECT* array, SPVM_OBJECT* element) {
  
  int32_t can_assign;
  
  assert(array);
  
  if (element == NULL) {
    can_assign = 1;
  }
  else {
    int32_t array_basic_type_id = array->basic_type_id;
    int32_t array_type_dimension = array->type_dimension;
    int32_t array_basic_type_category = SPVM_API_RUNTIME_get_basic_type_category(env->runtime, array_basic_type_id);

    int32_t element_basic_type_id = element->basic_type_id;
    int32_t element_type_dimension = element->type_dimension;
    
    if (array_type_dimension == 0) {
      can_assign = 0;
    }
    else if (array_type_dimension == 1) {
      if (array_basic_type_id == element_basic_type_id && array_type_dimension == element_type_dimension + 1) {
        can_assign = 1;
      }
      else {
        switch (array_basic_type_category) {
          case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_ANY_OBJECT:
          {
            can_assign = 1;
            break;
          }
          case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_INTERFACE:
          {
            can_assign = SPVM_API_RUNTIME_has_interface_by_id(env->runtime, array_basic_type_id, array_type_dimension - 1, element_basic_type_id, element_type_dimension);
            break;
          }
          case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_CALLBACK:
          {
            can_assign = SPVM_API_RUNTIME_has_callback_by_id(env->runtime, array_basic_type_id, array_type_dimension - 1, element_basic_type_id, element_type_dimension);
            break;
          }
          default: {
            can_assign = 0;
          }
        }
      }
    }
    else if (array_type_dimension > 1) {
      if (array_basic_type_id == element_basic_type_id && array_type_dimension == element_type_dimension + 1) {
        can_assign = 1;
      }
      else {
        switch (array_basic_type_category) {
          case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_INTERFACE:
          {
            can_assign = SPVM_API_RUNTIME_has_interface_by_id(env->runtime, array_basic_type_id, array_type_dimension - 1, element_basic_type_id, element_type_dimension);
            break;
          }
          case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_CALLBACK:
          {
            can_assign = SPVM_API_RUNTIME_has_callback_by_id(env->runtime, array_basic_type_id, array_type_dimension - 1, element_basic_type_id, element_type_dimension);
            break;
          }
          default: {
            can_assign = 0;
          }
        }
      }
    }
  }
  
  return can_assign;
}

