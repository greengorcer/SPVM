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

#include "spvm_compiler.h"
#include "spvm_op.h"

#include "spvm_opcode_array.h"
#include "spvm_opcode.h"
#include "spvm_class.h"
#include "spvm_basic_type.h"
#include "spvm_class_var.h"
#include "spvm_field.h"
#include "spvm_method.h"
#include "spvm_type.h"
#include "spvm_my.h"
#include "spvm_constant.h"
#include "spvm_weaken_backref.h"
#include "spvm_switch_info.h"
#include "spvm_case_info.h"
#include "spvm_limit.h"
#include "spvm_string_buffer.h"
#include "spvm_api.h"
#include "spvm_object.h"
#include "spvm_native.h"






























































SPVM_ENV* SPVM_API_create_env(SPVM_COMPILER* compiler) {

  // Native APIs. If a element is added, must increment env_length variable.
  void* env_init[]  = {
    NULL, // class_vars_heap
    (void*)(intptr_t)sizeof(SPVM_OBJECT), // object_header_byte_size
    (void*)(intptr_t)offsetof(SPVM_OBJECT, weaken_backref_head), // weaken_backref_head
    (void*)(intptr_t)offsetof(SPVM_OBJECT, ref_count), // object_ref_count_offset
    (void*)(intptr_t)offsetof(SPVM_OBJECT, basic_type_id), // object_basic_type_id_offset
    (void*)(intptr_t)offsetof(SPVM_OBJECT, type_dimension), // object_type_dimension_offset
    (void*)(intptr_t)offsetof(SPVM_OBJECT, type_category), // object_type_category_offset
    (void*)(intptr_t)offsetof(SPVM_OBJECT, flag), // object_flag_offset
    (void*)(intptr_t)offsetof(SPVM_OBJECT, length), // object_length_offset
    (void*)(intptr_t)SPVM_BASIC_TYPE_C_ID_BYTE_OBJECT, // byte_object_basic_type_id
    (void*)(intptr_t)SPVM_BASIC_TYPE_C_ID_SHORT_OBJECT, // short_object_basic_type_id
    (void*)(intptr_t)SPVM_BASIC_TYPE_C_ID_INT_OBJECT, // int_object_basic_type_id
    (void*)(intptr_t)SPVM_BASIC_TYPE_C_ID_LONG_OBJECT,  // long_object_basic_type_id
    (void*)(intptr_t)SPVM_BASIC_TYPE_C_ID_FLOAT_OBJECT, // float_object_basic_type_id
    (void*)(intptr_t)SPVM_BASIC_TYPE_C_ID_DOUBLE_OBJECT, // double_object_basic_type_id
    compiler,
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
    SPVM_API_object_basic_type_id,
    SPVM_API_object_type_dimension,
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
    (void*)(intptr_t)SPVM_BASIC_TYPE_C_ID_ANY_OBJECT, // any_object_basic_type_id
    SPVM_API_dump_raw,
    SPVM_API_dump,
    SPVM_API_call_spvm_method, // call_class_method
    SPVM_API_call_spvm_method, // call_instance_method
  };
  
  SPVM_ENV* env = calloc(sizeof(env_init), 1);
  if (env == NULL) {
    return NULL;
  }
  memcpy(env, env_init, sizeof(env_init));

  // Mortal stack
  int32_t native_mortal_stack_capacity = 1;
  void* native_mortal_stack = SPVM_API_alloc_memory_block_zero(env, sizeof(SPVM_OBJECT*) * native_mortal_stack_capacity);
  if (native_mortal_stack == NULL) {
    return NULL;
  }

  // Initialize Class Variables
  void* class_vars_heap = SPVM_API_alloc_memory_block_zero(env, sizeof(SPVM_VALUE) * ((int64_t)compiler->class_vars->length + 1));
  if (class_vars_heap == NULL) {
    return NULL;
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
  
  return env;
}

SPVM_OBJECT* SPVM_API_dump(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  SPVM_OBJECT* str = SPVM_API_dump_raw(env, object);
  
  SPVM_API_push_mortal(env, str);

  return str;
}

SPVM_OBJECT* SPVM_API_dump_raw(SPVM_ENV* env, SPVM_OBJECT* object) {
  
  int32_t depth = 0;
  SPVM_STRING_BUFFER* string_buffer = SPVM_STRING_BUFFER_new(255);
  SPVM_HASH* address_symtable = SPVM_HASH_new(255);
  
  SPVM_API_dump_recursive(env, object, &depth, string_buffer, address_symtable);
  
  int32_t string_buffer_length = string_buffer->length;
  
  SPVM_OBJECT* dump = SPVM_API_new_string_raw(env, string_buffer->buffer, string_buffer->length);
  
  SPVM_HASH_free(address_symtable);
  SPVM_STRING_BUFFER_free(string_buffer);
  
  return dump;
}

void SPVM_API_dump_recursive(SPVM_ENV* env, SPVM_OBJECT* object, int32_t* depth, SPVM_STRING_BUFFER* string_buffer, SPVM_HASH* address_symtable) {
  
  SPVM_COMPILER* compiler = (SPVM_COMPILER*)env->compiler;
  
  char tmp_buffer[256];
  
  SPVM_OBJECT* dump;
  if (object == NULL) {
    SPVM_STRING_BUFFER_add(string_buffer, "undef");
  }
  else {
    int32_t basic_type_id = object->basic_type_id;
    int32_t type_dimension = object->type_dimension;
    
    SPVM_BASIC_TYPE* basic_type = SPVM_LIST_fetch(compiler->basic_types, basic_type_id);
    const char* basic_type_name = basic_type->name;
    
    if (SPVM_TYPE_is_array_type(compiler, basic_type_id, type_dimension, 0)) {
      int32_t array_length = object->length;
      int32_t element_type_dimension = type_dimension - 1;

      SPVM_STRING_BUFFER_add(string_buffer, "[\n");
            
      for (int32_t array_index = 0; array_index < array_length; array_index++) {
    
        for (int32_t depth_index = 0; depth_index < *depth + 1; depth_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
        }

        if (SPVM_TYPE_is_multi_numeric_type(compiler, basic_type_id, element_type_dimension, 0)) {

          SPVM_STRING_BUFFER_add(string_buffer, "{\n");

          SPVM_BASIC_TYPE* basic_type = SPVM_LIST_fetch(compiler->basic_types, basic_type_id);
          SPVM_CLASS* class = basic_type->class;
          SPVM_FIELD* first_field = SPVM_LIST_fetch(class->fields, 0);
          int32_t fields_length = class->fields->length;
          
          for (int32_t field_index = 0; field_index < fields_length; field_index++) {
            for (int32_t depth_index = 0; depth_index < *depth + 2; depth_index++) {
              SPVM_STRING_BUFFER_add(string_buffer, "  ");
            }
            
            SPVM_FIELD* field = SPVM_LIST_fetch(class->fields, field_index);
            int32_t field_basic_type_id = field->type->basic_type->id;

            const char* field_name = field->name;
            SPVM_STRING_BUFFER_add(string_buffer, field_name);
            SPVM_STRING_BUFFER_add(string_buffer, " => ");

            switch (field_basic_type_id) {
              case SPVM_BASIC_TYPE_C_ID_BYTE: {
                int8_t* element = &((int8_t*)((intptr_t)object + env->object_header_byte_size))[array_index * fields_length];
                sprintf(tmp_buffer, "%d", element[field_index]);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_BASIC_TYPE_C_ID_SHORT: {
                int16_t* element = &((int16_t*)((intptr_t)object + env->object_header_byte_size))[array_index * fields_length];
                sprintf(tmp_buffer, "%d", element[field_index]);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_BASIC_TYPE_C_ID_INT: {
                int32_t* element = &((int32_t*)((intptr_t)object + env->object_header_byte_size))[array_index * fields_length];
                sprintf(tmp_buffer, "%d", element[field_index]);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_BASIC_TYPE_C_ID_LONG: {
                int64_t* element = &((int64_t*)((intptr_t)object + env->object_header_byte_size))[array_index * fields_length];
                sprintf(tmp_buffer, "%lld", (long long int)element[field_index]);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_BASIC_TYPE_C_ID_FLOAT: {
                float* element = &((float*)((intptr_t)object + env->object_header_byte_size))[array_index * fields_length];
                sprintf(tmp_buffer, "%g", element[field_index]);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_BASIC_TYPE_C_ID_DOUBLE: {
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
        else if (SPVM_TYPE_is_numeric_type(compiler, basic_type_id, element_type_dimension, 0)) {
          switch (basic_type_id) {
            case SPVM_BASIC_TYPE_C_ID_BYTE: {
              int8_t element = ((int8_t*)((intptr_t)object + env->object_header_byte_size))[array_index];
              sprintf(tmp_buffer, "%d", element);
              SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
              break;
            }
            case SPVM_BASIC_TYPE_C_ID_SHORT: {
              int16_t element = ((int16_t*)((intptr_t)object + env->object_header_byte_size))[array_index];
              sprintf(tmp_buffer, "%d", element);
              SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
              break;
            }
            case SPVM_BASIC_TYPE_C_ID_INT: {
              int32_t element = ((int32_t*)((intptr_t)object + env->object_header_byte_size))[array_index];
              sprintf(tmp_buffer, "%d", element);
              SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
              break;
            }
            case SPVM_BASIC_TYPE_C_ID_LONG: {
              int64_t element = ((int64_t*)((intptr_t)object + env->object_header_byte_size))[array_index];
              sprintf(tmp_buffer, "%lld", (long long int)element);
              SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
              break;
            }
            case SPVM_BASIC_TYPE_C_ID_FLOAT: {
              float element = ((float*)((intptr_t)object + env->object_header_byte_size))[array_index];
              sprintf(tmp_buffer, "%g", element);
              SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
              break;
            }
            case SPVM_BASIC_TYPE_C_ID_DOUBLE: {
              double element = ((double*)((intptr_t)object + env->object_header_byte_size))[array_index];
              sprintf(tmp_buffer, "%g", element);
              SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
              break;
            }
          }
        }
        else if (SPVM_TYPE_is_object_type(compiler, basic_type_id, element_type_dimension, 0)) {
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
      
      sprintf(tmp_buffer, "(%p)", object);
      SPVM_STRING_BUFFER_add(string_buffer, tmp_buffer);
    }
    else if (SPVM_TYPE_is_string_type(compiler, basic_type_id, 0, 0)) {
      const char* chars = env->get_chars(env, object);
      int32_t chars_length  = env->length(env, object);
      SPVM_STRING_BUFFER_add(string_buffer, "\"");
      SPVM_STRING_BUFFER_add_len(string_buffer, (char*)chars, chars_length);
      SPVM_STRING_BUFFER_add(string_buffer, "\"");
    }
    else if(SPVM_TYPE_is_module_type(compiler, basic_type_id, 0, 0)) {

      sprintf(tmp_buffer, "%p", object);
      int32_t exists = (int32_t)(intptr_t)SPVM_HASH_fetch(address_symtable, tmp_buffer, strlen(tmp_buffer));
      if (exists) {
        sprintf(tmp_buffer, "REUSE_OBJECT(%p)", object);
        SPVM_STRING_BUFFER_add(string_buffer, tmp_buffer);
      }
      else {
        SPVM_HASH_insert(address_symtable, tmp_buffer, strlen(tmp_buffer), (void*)(intptr_t)1);
        
        SPVM_BASIC_TYPE* basic_type = SPVM_LIST_fetch(compiler->basic_types, basic_type_id);
        SPVM_CLASS* class = basic_type->class;
        assert(class);

        SPVM_STRING_BUFFER_add(string_buffer, "{\n");
        
        // Free object fields
        SPVM_LIST* fields = class->fields;
        int32_t fields_length = fields->length;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          for (int32_t depth_index = 0; depth_index < *depth + 1; depth_index++) {
            SPVM_STRING_BUFFER_add(string_buffer, "  ");
          }

          SPVM_FIELD* field = SPVM_LIST_fetch(fields, field_index);
          
          int32_t field_basic_type_id = field->type->basic_type->id;
          int32_t field_type_dimension = field->type->dimension;
          int32_t field_offset = field->offset;
          const char* field_name = field->name;
          
          SPVM_STRING_BUFFER_add(string_buffer, field_name);
          SPVM_STRING_BUFFER_add(string_buffer, " => ");
          if (SPVM_TYPE_is_numeric_type(compiler, field_basic_type_id, field_type_dimension, 0)) {
            switch (field_basic_type_id) {
              case SPVM_BASIC_TYPE_C_ID_BYTE: {
                int8_t field_value = *(int8_t*)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
                sprintf(tmp_buffer, "%d", field_value);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_BASIC_TYPE_C_ID_SHORT: {
                int16_t field_value = *(int16_t*)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
                sprintf(tmp_buffer, "%d", field_value);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_BASIC_TYPE_C_ID_INT: {
                int32_t field_value = *(int32_t*)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
                sprintf(tmp_buffer, "%d", field_value);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_BASIC_TYPE_C_ID_LONG: {
                int64_t field_value = *(int64_t*)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
                sprintf(tmp_buffer, "%lld", (long long int)field_value);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_BASIC_TYPE_C_ID_FLOAT: {
                float field_value = *(float*)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
                sprintf(tmp_buffer, "%g", field_value);
                SPVM_STRING_BUFFER_add(string_buffer, (const char*)tmp_buffer);
                break;
              }
              case SPVM_BASIC_TYPE_C_ID_DOUBLE: {
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
          else if (SPVM_TYPE_is_object_type(compiler, field_basic_type_id, field_type_dimension, 0)) {
            SPVM_OBJECT* field_value = *(SPVM_OBJECT**)((intptr_t)object + (intptr_t)env->object_header_byte_size + field_offset);
            (*depth)++;
            SPVM_API_dump_recursive(env, field_value, depth, string_buffer, address_symtable);
            (*depth)--;
          }
          else {
            assert(0);
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
    else {
      assert(0);
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
  env->free_memory_block(env, buffer);
  
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

SPVM_ENV* SPVM_API_new_env(SPVM_ENV* env) {
  return SPVM_API_create_env(env->compiler);
}

void SPVM_API_free_env(SPVM_ENV* env) {
  // Free exception
  SPVM_API_set_exception(env, NULL);

  // Free class variables heap
  SPVM_API_free_memory_block(env, env->class_vars_heap);
  
  // Free mortal stack
  SPVM_API_free_memory_block(env, env->native_mortal_stack);

  free(env);
}

void SPVM_API_call_init_blocks(SPVM_ENV* env) {
  (void)env;
  
  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Call INIT blocks
  int32_t classes_length = compiler->classes->length;
  SPVM_VALUE stack[SPVM_LIMIT_C_METHOD_ARGS_MAX_COUNT];
  for (int32_t class_id = 0; class_id < classes_length; class_id++) {
    
    SPVM_CLASS* class = SPVM_LIST_fetch(compiler->classes, class_id);
    
    if (class->op_init_method) {
      SPVM_METHOD* init_method = class->op_init_method->uv.method;
      env->call_spvm_method(env, init_method->id, stack);
    }
  }
}

int32_t SPVM_API_call_spvm_method(SPVM_ENV* env, int32_t method_id, SPVM_VALUE* stack) {
  (void)env;
  
  // Runtime
  SPVM_COMPILER* compiler = env->compiler;

  // Method
  SPVM_METHOD* method = SPVM_LIST_fetch(compiler->methods, method_id);
  
  int32_t exception_flag;
  
  // Call native sub
  if (method->flag & SPVM_METHOD_C_FLAG_NATIVE) {
    // Enter scope
    int32_t original_mortal_stack_top = SPVM_API_enter_scope(env);

    // Call native subrotuine
    int32_t (*native_address)(SPVM_ENV*, SPVM_VALUE*) = method->native_address;
    assert(native_address != NULL);
    exception_flag = (*native_address)(env, stack);
    
    // Increment ref count of return value
    if (!exception_flag) {
      switch (method->return_type_category) {
        case SPVM_TYPE_C_TYPE_CATEGORY_ANY_OBJECT:
        case SPVM_TYPE_C_TYPE_CATEGORY_CLASS:
        case SPVM_TYPE_C_TYPE_CATEGORY_NUMERIC_ARRAY:
        case SPVM_TYPE_C_TYPE_CATEGORY_MULNUM_ARRAY:
        case SPVM_TYPE_C_TYPE_CATEGORY_OBJECT_ARRAY:
        case SPVM_TYPE_C_TYPE_CATEGORY_STRING:
        {
          if (*(void**)&stack[0] != NULL) {
            SPVM_API_INC_REF_COUNT_ONLY(*(void**)&stack[0]);
          }
        }
      }
    }

    // Leave scope
    SPVM_API_leave_scope(env, original_mortal_stack_top);

    // Decrement ref count of return value
    if (!exception_flag) {
      switch (method->return_type_category) {
        case SPVM_TYPE_C_TYPE_CATEGORY_ANY_OBJECT:
        case SPVM_TYPE_C_TYPE_CATEGORY_CLASS:
        case SPVM_TYPE_C_TYPE_CATEGORY_NUMERIC_ARRAY:
        case SPVM_TYPE_C_TYPE_CATEGORY_MULNUM_ARRAY:
        case SPVM_TYPE_C_TYPE_CATEGORY_OBJECT_ARRAY:
        case SPVM_TYPE_C_TYPE_CATEGORY_STRING:
        {
          if (*(void**)&stack[0] != NULL) {
            SPVM_API_DEC_REF_COUNT_ONLY(*(void**)&stack[0]);
          }
        }
      }
    }
    
    // Set default exception message
    if (exception_flag && env->exception_object == NULL) {
      void* exception = env->new_string_nolen_raw(env, "Error");
      env->set_exception(env, exception);
    }
  }
  // Call precompiled sub
  else if (method->flag & SPVM_METHOD_C_FLAG_PRECOMPILE) {
    int32_t (*precompile_address)(SPVM_ENV*, SPVM_VALUE*) = method->precompile_address;
    exception_flag = (*precompile_address)(env, stack);
  }
  // Call sub virtual machine
  else {
    exception_flag = SPVM_API_call_spvm_method_vm(env, method_id, stack);
  }
  
  return exception_flag;
}

int32_t SPVM_API_call_spvm_method_vm(SPVM_ENV* env, int32_t method_id, SPVM_VALUE* stack) {
  (void)env;

  // Opcode relative index
  register int32_t opcode_rel_index = 0;
  
  // Runtime
  SPVM_COMPILER* compiler = env->compiler;

  // Runtime method
  SPVM_METHOD* method = SPVM_LIST_fetch(compiler->methods, method_id);
  
  // Runtime class
  SPVM_CLASS* class = method->class;

  // Operation codes
  SPVM_OPCODE* opcodes = compiler->opcode_array->values;

  // Exception flag
  int32_t exception_flag = 0;

  // Operation code base
  int32_t method_opcodes_base = method->opcodes_base;

  // Call method argument stack top
  int32_t call_spvm_method_arg_stack_top = 0;
  
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
    numeric_vars_byte_size += method->long_vars_alloc_length * 8;
    numeric_vars_byte_size += method->double_vars_alloc_length * 8;
    numeric_vars_byte_size += method->int_vars_alloc_length * 4;
    numeric_vars_byte_size += method->float_vars_alloc_length * 4;
    numeric_vars_byte_size += method->short_vars_alloc_length * 2;
    numeric_vars_byte_size += method->mortal_stack_length * 2;
    numeric_vars_byte_size += method->byte_vars_alloc_length * 1;
    
    if (numeric_vars_byte_size % 8 != 0) {
      numeric_vars_byte_size += (8 - (numeric_vars_byte_size % 8));
    }
    
    // Address area byte size
    int32_t address_vars_byte_size = 0;
    address_vars_byte_size += method->object_vars_alloc_length * sizeof(void*);
    address_vars_byte_size += method->ref_vars_alloc_length * sizeof(void*);
    
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
    call_stack_offset += method->double_vars_alloc_length * 8;
    
    // Long varialbes
    long_vars = (int64_t*)&call_stack[call_stack_offset];
    call_stack_offset += method->long_vars_alloc_length * 8;
    
    // Float variables
    float_vars = (float*)&call_stack[call_stack_offset];
    call_stack_offset += method->float_vars_alloc_length * 4;
    
    // Int variables
    int_vars = (int32_t*)&call_stack[call_stack_offset];
    call_stack_offset += method->int_vars_alloc_length * 4;

    // Short variables
    short_vars = (int16_t*)&call_stack[call_stack_offset];
    call_stack_offset += method->short_vars_alloc_length * 2;

    // Mortal stack
    mortal_stack = (uint16_t*)&call_stack[call_stack_offset];
    call_stack_offset += method->mortal_stack_length * 2;
    
    // Byte variables
    byte_vars = (int8_t*)&call_stack[call_stack_offset];
    call_stack_offset += method->byte_vars_alloc_length * 1;
    
    call_stack_offset = numeric_vars_byte_size;

    // Object variables
    object_vars = (void**)&call_stack[call_stack_offset];
    call_stack_offset += method->object_vars_alloc_length * sizeof(void*);
    
    // Refernce variables
    ref_vars = (void**)&call_stack[call_stack_offset];
    call_stack_offset += method->ref_vars_alloc_length * sizeof(void*);
  }

  // Buffer for string convertion
  // double need 17 digit
  // int64_t need 21 gidit (-9223372036854775808 + (null character))
  char string_convert_buffer[21];
  
  // Copy arguments to variables
  {
    int32_t stack_index = 0;
    for (int32_t arg_index = 0; arg_index < method->args->length; arg_index++) {
      SPVM_MY* arg = SPVM_LIST_fetch(method->args, arg_index);
      
      int32_t type_width = arg->type_width;
      switch (arg->type_category) {
        case SPVM_TYPE_C_TYPE_CATEGORY_BYTE: {
          byte_vars[arg->mem_id] = *(int8_t*)&stack[stack_index];
          stack_index++;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_SHORT: {
          short_vars[arg->mem_id] = *(int16_t*)&stack[stack_index];
          stack_index++;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_INT: {
          int_vars[arg->mem_id] = *(int32_t*)&stack[stack_index];
          stack_index++;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_LONG: {
          long_vars[arg->mem_id] = *(int64_t*)&stack[stack_index];
          stack_index++;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_FLOAT: {
          float_vars[arg->mem_id] = *(float*)&stack[stack_index];
          stack_index++;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_DOUBLE: {
          double_vars[arg->mem_id] = *(double*)&stack[stack_index];
          stack_index++;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_MULNUM_BYTE: {
          for (int32_t field_index = 0; field_index < type_width; field_index++) {
            byte_vars[arg->mem_id + field_index] = *(int8_t*)&stack[stack_index + field_index];
          }
          stack_index += type_width;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_MULNUM_SHORT: {
          for (int32_t field_index = 0; field_index < type_width; field_index++) {
            short_vars[arg->mem_id + field_index] = *(int16_t*)&stack[stack_index + field_index];
          }
          stack_index += type_width;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_MULNUM_INT: {
          for (int32_t field_index = 0; field_index < type_width; field_index++) {
            int_vars[arg->mem_id + field_index] = *(int32_t*)&stack[stack_index + field_index];
          }
          stack_index += type_width;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_MULNUM_LONG: {
          for (int32_t field_index = 0; field_index < type_width; field_index++) {
            long_vars[arg->mem_id + field_index] = *(int64_t*)&stack[stack_index + field_index];
          }
          stack_index += type_width;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_MULNUM_FLOAT: {
          for (int32_t field_index = 0; field_index < type_width; field_index++) {
            float_vars[arg->mem_id + field_index] = *(float*)&stack[stack_index + field_index];
          }
          stack_index += type_width;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_MULNUM_DOUBLE: {
          for (int32_t field_index = 0; field_index < type_width; field_index++) {
            double_vars[arg->mem_id + field_index] = *(double*)&stack[stack_index + field_index];
          }
          stack_index += type_width;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_ANY_OBJECT:
        case SPVM_TYPE_C_TYPE_CATEGORY_CLASS:
        case SPVM_TYPE_C_TYPE_CATEGORY_NUMERIC_ARRAY:
        case SPVM_TYPE_C_TYPE_CATEGORY_MULNUM_ARRAY:
        case SPVM_TYPE_C_TYPE_CATEGORY_OBJECT_ARRAY:
        case SPVM_TYPE_C_TYPE_CATEGORY_STRING:
        {
          object_vars[arg->mem_id] = *(void**)&stack[stack_index];

          // If arg is object, increment reference count
          void* object = *(void**)&object_vars[arg->mem_id];
          if (object != NULL) {
            SPVM_API_INC_REF_COUNT_ONLY(object);
          }
          stack_index++;
          break;
        }
        case SPVM_TYPE_C_TYPE_CATEGORY_REF_BYTE:
        case SPVM_TYPE_C_TYPE_CATEGORY_REF_SHORT:
        case SPVM_TYPE_C_TYPE_CATEGORY_REF_INT:
        case SPVM_TYPE_C_TYPE_CATEGORY_REF_LONG:
        case SPVM_TYPE_C_TYPE_CATEGORY_REF_FLOAT:
        case SPVM_TYPE_C_TYPE_CATEGORY_REF_DOUBLE:
        case SPVM_TYPE_C_TYPE_CATEGORY_REF_MULNUM_BYTE:
        case SPVM_TYPE_C_TYPE_CATEGORY_REF_MULNUM_SHORT:
        case SPVM_TYPE_C_TYPE_CATEGORY_REF_MULNUM_INT:
        case SPVM_TYPE_C_TYPE_CATEGORY_REF_MULNUM_LONG:
        case SPVM_TYPE_C_TYPE_CATEGORY_REF_MULNUM_FLOAT:
        case SPVM_TYPE_C_TYPE_CATEGORY_REF_MULNUM_DOUBLE:
        {
          ref_vars[arg->mem_id] = *(void**)&stack[stack_index];
          stack_index++;
          break;
        }
        default:
          assert(0);
      }
    }
  }

  int32_t object_header_byte_size = (intptr_t)env->object_header_byte_size;

  // Execute operation codes
  while (1) {
    SPVM_OPCODE* opcode = &(opcodes[method_opcodes_base + opcode_rel_index]);
    
    int32_t opcode_id = opcode->id;
    
    switch (opcode_id) {
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
          
          int8_t* bytes1 = env->get_elems_byte(env, object1);
          int8_t* bytes2 = env->get_elems_byte(env, object2);
          
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
      case SPVM_OPCODE_C_ID_ADD_INT:
        int_vars[opcode->operand0] = int_vars[opcode->operand1] + int_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_ADD_LONG:
        long_vars[opcode->operand0] = long_vars[opcode->operand1] + long_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_ADD_FLOAT:
        float_vars[opcode->operand0] = float_vars[opcode->operand1] + float_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_ADD_DOUBLE:
        double_vars[opcode->operand0] = double_vars[opcode->operand1] + double_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_SUBTRACT_INT:
        int_vars[opcode->operand0] = int_vars[opcode->operand1] - int_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_SUBTRACT_LONG:
        long_vars[opcode->operand0] = long_vars[opcode->operand1] - long_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_SUBTRACT_FLOAT:
        float_vars[opcode->operand0] = float_vars[opcode->operand1] - float_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_SUBTRACT_DOUBLE:
        double_vars[opcode->operand0] = double_vars[opcode->operand1] - double_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_MULTIPLY_INT:
        int_vars[opcode->operand0] = int_vars[opcode->operand1] * int_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_MULTIPLY_LONG:
        long_vars[opcode->operand0] = long_vars[opcode->operand1] * long_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_MULTIPLY_FLOAT:
        float_vars[opcode->operand0] = float_vars[opcode->operand1] * float_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_MULTIPLY_DOUBLE:
        double_vars[opcode->operand0] = double_vars[opcode->operand1] * double_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_DIVIDE_INT:
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
      case SPVM_OPCODE_C_ID_DIVIDE_LONG:
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
      case SPVM_OPCODE_C_ID_DIVIDE_FLOAT:
        float_vars[opcode->operand0]
          = float_vars[opcode->operand1] / float_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_DIVIDE_DOUBLE:
        double_vars[opcode->operand0]
          = double_vars[opcode->operand1] / double_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_REMAINDER_INT:
        if (__builtin_expect(int_vars[opcode->operand2] == 0, 0)) {
          void* exception = env->new_string_nolen_raw(env, "0 division");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int_vars[opcode->operand0] = int_vars[opcode->operand1] % int_vars[opcode->operand2];
        }
        break;
      case SPVM_OPCODE_C_ID_REMAINDER_LONG:
        if (__builtin_expect(long_vars[opcode->operand2] == 0, 0)) {
          void* exception = env->new_string_nolen_raw(env, "0 division");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          long_vars[opcode->operand0] = long_vars[opcode->operand1] % long_vars[opcode->operand2];
        }
        break;
      case SPVM_OPCODE_C_ID_LEFT_SHIFT_INT:
        int_vars[opcode->operand0]
          = int_vars[opcode->operand1] << int_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_LEFT_SHIFT_LONG:
        long_vars[opcode->operand0]
          = long_vars[opcode->operand1] << int_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_RIGHT_ARITHMETIC_SHIFT_INT:
        int_vars[opcode->operand0]
          = int_vars[opcode->operand1] >> int_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_RIGHT_ARITHMETIC_SHIFT_LONG:
        long_vars[opcode->operand0]
          = long_vars[opcode->operand1] >> int_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_RIGHT_LOGICAL_SHIFT_INT:
        int_vars[opcode->operand0]
          = (int32_t)((uint32_t)int_vars[opcode->operand1] >> int_vars[opcode->operand2]);
        break;
      case SPVM_OPCODE_C_ID_RIGHT_LOGICAL_SHIFT_LONG:
        long_vars[opcode->operand0]
          = (int64_t)((uint64_t)long_vars[opcode->operand1] >> int_vars[opcode->operand2]);
        break;
      case SPVM_OPCODE_C_ID_BIT_AND_INT:
        int_vars[opcode->operand0]
          = int_vars[opcode->operand1] & int_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_BIT_AND_LONG:
        long_vars[opcode->operand0]
          = long_vars[opcode->operand1] & long_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_BIT_OR_INT:
        int_vars[opcode->operand0]
          = int_vars[opcode->operand1] | int_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_BIT_OR_LONG:
        long_vars[opcode->operand0]
          = long_vars[opcode->operand1] | long_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_BIT_XOR_INT:
        int_vars[opcode->operand0]
          = int_vars[opcode->operand1] ^ int_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_BIT_XOR_LONG:
        long_vars[opcode->operand0]
          = long_vars[opcode->operand1] ^ long_vars[opcode->operand2];
        break;
      case SPVM_OPCODE_C_ID_NEGATE_INT:
        int_vars[opcode->operand0] = -int_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_NEGATE_LONG:
        long_vars[opcode->operand0] = -long_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_NEGATE_FLOAT:
        float_vars[opcode->operand0] = -float_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_NEGATE_DOUBLE:
        double_vars[opcode->operand0] = -double_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_BIT_NOT_INT:
        int_vars[opcode->operand0] = ~int_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_BIT_NOT_LONG:
        long_vars[opcode->operand0] = ~long_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_LONG:
        long_vars[opcode->operand0] = (int64_t)int_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_FLOAT:
        float_vars[opcode->operand0] = (float)int_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_DOUBLE:
        double_vars[opcode->operand0] = (double)int_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_INT:
        int_vars[opcode->operand0] = (int32_t)long_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_FLOAT:
        float_vars[opcode->operand0] = (float)long_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_DOUBLE:
        double_vars[opcode->operand0] = (double)long_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_INT:
        int_vars[opcode->operand0] = (int32_t)float_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_LONG:
        long_vars[opcode->operand0] = (int64_t)float_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_DOUBLE:
        double_vars[opcode->operand0] = (double)float_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_INT:
        int_vars[opcode->operand0] = (int32_t)double_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_LONG:
        long_vars[opcode->operand0] = (int64_t)double_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_FLOAT:
        float_vars[opcode->operand0] = (float)double_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_BYTE:
        byte_vars[opcode->operand0] = (int8_t)(int_vars[opcode->operand1]);
        break;
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_SHORT:
        short_vars[opcode->operand0] = (int16_t)(int_vars[opcode->operand1]);
        break;
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_INT:
        int_vars[opcode->operand0] = (int32_t)byte_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_INT:
        int_vars[opcode->operand0] = (int32_t)short_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_LONG:
        long_vars[opcode->operand0] = (int64_t)byte_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_FLOAT:
        float_vars[opcode->operand0] = (float)byte_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_DOUBLE:
        double_vars[opcode->operand0] = (double)byte_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_BYTE:
        byte_vars[opcode->operand0] = (int8_t)short_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_LONG:
        long_vars[opcode->operand0] = (int64_t)short_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_FLOAT:
        float_vars[opcode->operand0] = (float)short_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_DOUBLE:
        double_vars[opcode->operand0] = (double)short_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_BYTE:
        byte_vars[opcode->operand0] = (int8_t)long_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_SHORT:
        short_vars[opcode->operand0] = (int16_t)long_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_BYTE:
        byte_vars[opcode->operand0] = (int8_t)float_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_SHORT:
        short_vars[opcode->operand0] = (int16_t)float_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_BYTE:
        byte_vars[opcode->operand0] = (int8_t)double_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_SHORT:
        short_vars[opcode->operand0] = (int16_t)double_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_SHORT:
        short_vars[opcode->operand0] = (int16_t)byte_vars[opcode->operand1];
        break;
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_TO_STRING: {
        sprintf(string_convert_buffer, "%" PRId8, byte_vars[opcode->operand1]);
        int32_t string_length = strlen(string_convert_buffer);
        void* string = env->new_string_raw(env, string_convert_buffer, string_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_STRING_TO_BYTE_ARRAY:
      {
        void* src_string = object_vars[opcode->operand1];
        int32_t src_string_length = env->length(env, src_string);
        int8_t* src_string_data = env->get_elems_byte(env, src_string);
        void* byte_array = env->new_byte_array_raw(env, src_string_length);
        int8_t* byte_array_data = env->get_elems_byte(env, byte_array);
        memcpy(byte_array_data, src_string_data, src_string_length);
        
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], byte_array);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_BYTE_ARRAY_TO_STRING:
      {
        void* src_byte_array = object_vars[opcode->operand1];
        int32_t src_byte_array_length = env->length(env, src_byte_array);
        int8_t* src_byte_array_data = env->get_elems_byte(env, src_byte_array);
        void* string = env->new_string_raw(env, (const char*)src_byte_array_data, src_byte_array_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_STRING: {
        sprintf(string_convert_buffer, "%" PRId16, short_vars[opcode->operand1]);
        int32_t string_length = strlen(string_convert_buffer);
        void* string = env->new_string_raw(env, string_convert_buffer, string_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_STRING: {
        sprintf(string_convert_buffer, "%" PRId32, int_vars[opcode->operand1]);
        int32_t string_length = strlen(string_convert_buffer);
        void* string = env->new_string_raw(env, string_convert_buffer, string_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_STRING: {
        sprintf(string_convert_buffer, "%" PRId64, long_vars[opcode->operand1]);
        int32_t string_length = strlen(string_convert_buffer);
        void* string = env->new_string_raw(env, string_convert_buffer, string_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_STRING: {
        sprintf(string_convert_buffer, "%g", float_vars[opcode->operand1]);
        int32_t string_length = strlen(string_convert_buffer);
        void* string = env->new_string_raw(env, string_convert_buffer, string_length);
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], string);
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_STRING: {
        sprintf(string_convert_buffer, "%g", double_vars[opcode->operand1]);
        int32_t string_length = strlen(string_convert_buffer);
        void* string = env->new_string_raw(env, string_convert_buffer, string_length);
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
        int32_t constant_id = opcode->operand1;
        SPVM_CONSTANT* constant = class->info_constants->values[constant_id];
        long_vars[opcode->operand0] = constant->value.lval;
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_CONSTANT_FLOAT: {
        SPVM_VALUE value;
        value.ival = (int32_t)opcode->operand1;
        float_vars[opcode->operand0] = value.fval;
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_CONSTANT_DOUBLE: {
        int32_t constant_id = opcode->operand1;
        SPVM_CONSTANT* constant = class->info_constants->values[constant_id];
        double_vars[opcode->operand0] = constant->value.dval;
        break;
      }
      case SPVM_OPCODE_C_ID_ARRAY_FETCH_BYTE: {
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
      case SPVM_OPCODE_C_ID_ARRAY_FETCH_SHORT: {
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
      case SPVM_OPCODE_C_ID_ARRAY_FETCH_INT: {
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
      case SPVM_OPCODE_C_ID_ARRAY_FETCH_LONG: {
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
      case SPVM_OPCODE_C_ID_ARRAY_FETCH_FLOAT: {
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
      case SPVM_OPCODE_C_ID_ARRAY_FETCH_DOUBLE: {
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
      case SPVM_OPCODE_C_ID_ARRAY_FETCH_OBJECT: {
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
      case SPVM_OPCODE_C_ID_ARRAY_STORE_BYTE: {
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
      case SPVM_OPCODE_C_ID_ARRAY_STORE_SHORT: {
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
      case SPVM_OPCODE_C_ID_ARRAY_STORE_INT: {
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
      case SPVM_OPCODE_C_ID_ARRAY_STORE_LONG: {
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
      case SPVM_OPCODE_C_ID_ARRAY_STORE_FLOAT: {
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
      case SPVM_OPCODE_C_ID_ARRAY_STORE_DOUBLE: {
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
      case SPVM_OPCODE_C_ID_ARRAY_STORE_OBJECT: {
        
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
      case SPVM_OPCODE_C_ID_ARRAY_STORE_OBJECT_CHECK_TYPE: {
        
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
            int32_t is_valid;
            if (object == NULL) {
              is_valid = 1;
            }
            else {
              int32_t array_basic_type_id = *(int32_t*)((intptr_t)array + (intptr_t)env->object_basic_type_id_offset);
              int32_t array_type_dimension = *(uint8_t*)((intptr_t)array + (intptr_t)env->object_type_dimension_offset);
              int32_t element_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);
              int32_t element_type_dimension = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);
              if (array_basic_type_id == element_basic_type_id && array_type_dimension == element_type_dimension + 1) {
                is_valid = 1;
              }
              else if (array_basic_type_id == (intptr_t)env->any_object_basic_type_id && array_type_dimension == element_type_dimension + 1) {
                is_valid = 1;
              }
              else {
                is_valid = 0;
              }
            }
            if (is_valid) {
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
      case SPVM_OPCODE_C_ID_ARRAY_STORE_UNDEF: {
        
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FETCH_BYTE: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FETCH_SHORT: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FETCH_INT: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FETCH_LONG: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FETCH_FLOAT: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FETCH_DOUBLE: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FIELD_FETCH_BYTE: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FIELD_FETCH_SHORT: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FIELD_FETCH_INT: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FIELD_FETCH_LONG: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FIELD_FETCH_FLOAT: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FIELD_FETCH_DOUBLE: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_STORE_BYTE: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_STORE_SHORT: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_STORE_INT: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_STORE_LONG: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_STORE_FLOAT: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_STORE_DOUBLE: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FIELD_STORE_BYTE: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FIELD_STORE_SHORT: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FIELD_STORE_INT: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FIELD_STORE_LONG: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FIELD_STORE_FLOAT: {
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
      case SPVM_OPCODE_C_ID_MULNUM_ARRAY_FIELD_STORE_DOUBLE: {
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
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_BYTE: {
        int8_t* value_ref = *(int8_t**)&ref_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          byte_vars[opcode->operand0 + field_index] = value_ref[field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_SHORT: {
        int16_t* value_ref = *(int16_t**)&ref_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          short_vars[opcode->operand0 + field_index] = value_ref[field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_INT: {
        int32_t* value_ref = *(int32_t**)&ref_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          int_vars[opcode->operand0 + field_index] = value_ref[field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_LONG: {
        int64_t* value_ref = *(int64_t**)&ref_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          long_vars[opcode->operand0 + field_index] = value_ref[field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_FLOAT: {
        float* value_ref = *(float**)&ref_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          float_vars[opcode->operand0 + field_index] = value_ref[field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_DOUBLE: {
        double* value_ref = *(double**)&ref_vars[opcode->operand1];
        int32_t fields_length = opcode->operand3;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          double_vars[opcode->operand0 + field_index] = value_ref[field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_GET_FIELD_BYTE: {
        int8_t* value_ref = *(int8_t**)&ref_vars[opcode->operand1];
        int32_t field_index = opcode->operand2;
        byte_vars[opcode->operand0] = value_ref[field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_GET_FIELD_SHORT: {
        int16_t* value_ref = *(int16_t**)&ref_vars[opcode->operand1];
        int32_t field_index = opcode->operand2;
        short_vars[opcode->operand0] = value_ref[field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_GET_FIELD_INT: {
        int32_t* value_ref = *(int32_t**)&ref_vars[opcode->operand1];
        int32_t field_index = opcode->operand2;
        int_vars[opcode->operand0] = value_ref[field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_GET_FIELD_LONG: {
        int64_t* value_ref = *(int64_t**)&ref_vars[opcode->operand1];
        int32_t field_index = opcode->operand2;
        long_vars[opcode->operand0] = value_ref[field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_GET_FIELD_FLOAT: {
        float* value_ref = *(float**)&ref_vars[opcode->operand1];
        int32_t field_index = opcode->operand2;
        float_vars[opcode->operand0] = value_ref[field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_GET_FIELD_DOUBLE: {
        double* value_ref = *(double**)&ref_vars[opcode->operand1];
        int32_t field_index = opcode->operand2;
        double_vars[opcode->operand0] = value_ref[field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_SET_FIELD_BYTE: {
        int8_t* value_ref = *(int8_t**)&ref_vars[opcode->operand0];
        int32_t field_index = opcode->operand2;
        value_ref[field_index] = byte_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_SET_FIELD_SHORT: {
        int16_t* value_ref = *(int16_t**)&ref_vars[opcode->operand0];
        int32_t field_index = opcode->operand2;
        value_ref[field_index] = short_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_SET_FIELD_INT: {
        int32_t* value_ref = *(int32_t**)&ref_vars[opcode->operand0];
        int32_t field_index = opcode->operand2;
        value_ref[field_index] = int_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_SET_FIELD_LONG: {
        int64_t* value_ref = *(int64_t**)&ref_vars[opcode->operand0];
        int32_t field_index = opcode->operand2;
        value_ref[field_index] = long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_SET_FIELD_FLOAT: {
        float* value_ref = *(float**)&ref_vars[opcode->operand0];
        int32_t field_index = opcode->operand2;
        value_ref[field_index] = float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_DEREF_SET_FIELD_DOUBLE: {
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
      case SPVM_OPCODE_C_ID_MOVE_UNDEF: {
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], NULL);
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_REF: {
        *(void**)&ref_vars[opcode->operand0] = *(void**)&ref_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_MOVE_BYTE: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          byte_vars[opcode->operand0 + field_index] = byte_vars[opcode->operand1 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_MOVE_SHORT: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          short_vars[opcode->operand0 + field_index] = short_vars[opcode->operand1 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_MOVE_INT: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          int_vars[opcode->operand0 + field_index] = int_vars[opcode->operand1 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_MOVE_LONG: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          long_vars[opcode->operand0 + field_index] = long_vars[opcode->operand1 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_MOVE_FLOAT: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          float_vars[opcode->operand0 + field_index] = float_vars[opcode->operand1 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_MOVE_DOUBLE: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          double_vars[opcode->operand0 + field_index] = double_vars[opcode->operand1 + field_index];
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_GET_FIELD_BYTE: {
        int32_t field_index = opcode->operand2;
        byte_vars[opcode->operand0] = byte_vars[opcode->operand1 + field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_GET_FIELD_SHORT: {
        int32_t field_index = opcode->operand2;
        short_vars[opcode->operand0] = short_vars[opcode->operand1 + field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_GET_FIELD_INT: {
        int32_t field_index = opcode->operand2;
        int_vars[opcode->operand0] = int_vars[opcode->operand1 + field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_GET_FIELD_LONG: {
        int32_t field_index = opcode->operand2;
        long_vars[opcode->operand0] = long_vars[opcode->operand1 + field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_GET_FIELD_FLOAT: {
        int32_t field_index = opcode->operand2;
        float_vars[opcode->operand0] = float_vars[opcode->operand1 + field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_GET_FIELD_DOUBLE: {
        int32_t field_index = opcode->operand2;
        double_vars[opcode->operand0] = double_vars[opcode->operand1 + field_index];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_SET_FIELD_BYTE: {
        int32_t field_index = opcode->operand2;
        byte_vars[opcode->operand0 + field_index] = byte_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_SET_FIELD_SHORT: {
        int32_t field_index = opcode->operand2;
        short_vars[opcode->operand0 + field_index] = short_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_SET_FIELD_INT: {
        int32_t field_index = opcode->operand2;
        int_vars[opcode->operand0 + field_index] = int_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_SET_FIELD_LONG: {
        int32_t field_index = opcode->operand2;
        long_vars[opcode->operand0 + field_index] = long_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_SET_FIELD_FLOAT: {
        int32_t field_index = opcode->operand2;
        float_vars[opcode->operand0 + field_index] = float_vars[opcode->operand1];
        break;
      }
      case SPVM_OPCODE_C_ID_MULNUM_SET_FIELD_DOUBLE: {
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
            void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for object array");
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
        int32_t constant_id = opcode->operand1;
        SPVM_CONSTANT* constant = class->info_constants->values[constant_id];
        const char* string_value = constant->value.oval;
        
        void* string = env->new_string_raw(env, string_value, constant->string_length);
        if (string == NULL) {
          void* exception = env->new_string_nolen_raw(env, "Can't allocate memory for string");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0] , string);
        }
        break;
      }
      case SPVM_OPCODE_C_ID_ARRAY_LENGTH:
        if (*(void**)&object_vars[opcode->operand1] == NULL) {
          void* exception = env->new_string_nolen_raw(env, "Can't get array length of undef value.");
          env->set_exception(env, exception);
          exception_flag = 1;
        }
        else {
          int_vars[opcode->operand0] = *(int32_t*)((intptr_t)*(void**)&object_vars[opcode->operand1] + (intptr_t)env->object_length_offset);
        }
        break;
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
        *(int8_t*)&stack[call_spvm_method_arg_stack_top] = byte_vars[opcode->operand0];
        call_spvm_method_arg_stack_top++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_SHORT: {
        *(int16_t*)&stack[call_spvm_method_arg_stack_top] = short_vars[opcode->operand0];
        call_spvm_method_arg_stack_top++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_INT: {
        *(int32_t*)&stack[call_spvm_method_arg_stack_top] = int_vars[opcode->operand0];
        call_spvm_method_arg_stack_top++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_LONG: {
        *(int64_t*)&stack[call_spvm_method_arg_stack_top] = long_vars[opcode->operand0];
        call_spvm_method_arg_stack_top++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_FLOAT: {
        *(float*)&stack[call_spvm_method_arg_stack_top] = float_vars[opcode->operand0];
        call_spvm_method_arg_stack_top++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_DOUBLE: {
        *(double*)&stack[call_spvm_method_arg_stack_top] = double_vars[opcode->operand0];
        call_spvm_method_arg_stack_top++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_OBJECT: {
        *(void**)&stack[call_spvm_method_arg_stack_top] = *(void**)&object_vars[opcode->operand0];
        call_spvm_method_arg_stack_top++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_REF: {
        *(void**)&stack[call_spvm_method_arg_stack_top] = *(void**)&ref_vars[opcode->operand0];
        call_spvm_method_arg_stack_top++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_UNDEF:
        *(void**)&stack[call_spvm_method_arg_stack_top] = NULL;
        call_spvm_method_arg_stack_top++;
        
        break;
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_BYTE: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(int8_t*)&stack[call_spvm_method_arg_stack_top + field_index] = byte_vars[opcode->operand0 + field_index];
        }
        call_spvm_method_arg_stack_top += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_SHORT: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(int16_t*)&stack[call_spvm_method_arg_stack_top + field_index] = short_vars[opcode->operand0 + field_index];
        }
        call_spvm_method_arg_stack_top += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_INT: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(int32_t*)&stack[call_spvm_method_arg_stack_top + field_index] = int_vars[opcode->operand0 + field_index];
        }
        call_spvm_method_arg_stack_top += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_LONG: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(int64_t*)&stack[call_spvm_method_arg_stack_top + field_index] = long_vars[opcode->operand0 + field_index];
        }
        call_spvm_method_arg_stack_top += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_FLOAT: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(float*)&stack[call_spvm_method_arg_stack_top + field_index] = float_vars[opcode->operand0 + field_index];
        }
        call_spvm_method_arg_stack_top += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_DOUBLE: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          *(double*)&stack[call_spvm_method_arg_stack_top + field_index] = double_vars[opcode->operand0 + field_index];
        }
        call_spvm_method_arg_stack_top += fields_length;
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
      case SPVM_OPCODE_C_ID_CALL_CLASS_METHOD:
      case SPVM_OPCODE_C_ID_CALL_INSTANCE_METHOD:
      {
        int32_t call_method_id = opcode->operand1;
        SPVM_METHOD* call_spvm_method = SPVM_LIST_fetch(compiler->methods, call_method_id);
        call_spvm_method_arg_stack_top -= call_spvm_method->args_alloc_length;
        exception_flag = env->call_spvm_method(env, call_method_id, stack);
        
        switch (call_spvm_method->return_type_category_id) {
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_VOID: {
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_BYTE: {
            if (!exception_flag) {
              byte_vars[opcode->operand0] = *(int8_t*)&stack[0];
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_SHORT: {
            if (!exception_flag) {
              short_vars[opcode->operand0] = *(int16_t*)&stack[0];
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_INT: {
            if (!exception_flag) {
              int_vars[opcode->operand0] = *(int32_t*)&stack[0];
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_LONG: {
            if (!exception_flag) {
              long_vars[opcode->operand0] = *(int64_t*)&stack[0];
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_FLOAT: {
            if (!exception_flag) {
              float_vars[opcode->operand0] = *(float*)&stack[0];
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_DOUBLE: {
            if (!exception_flag) {
              double_vars[opcode->operand0] = *(double*)&stack[0];
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_OBJECT: {
            if (!exception_flag) {
              SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], stack[0].oval);
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_MULNUM_BYTE: {
            if (!exception_flag) {
              int32_t fields_length = opcode->operand3;
              for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                byte_vars[opcode->operand0 + field_index] = *(int8_t*)&stack[field_index];
              }
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_MULNUM_SHORT: {
            if (!exception_flag) {
              int32_t fields_length = opcode->operand3;
              for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                short_vars[opcode->operand0 + field_index] = *(int16_t*)&stack[field_index];
              }
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_MULNUM_INT: {
            if (!exception_flag) {
              int32_t fields_length = opcode->operand3;
              for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                int_vars[opcode->operand0 + field_index] = *(int32_t*)&stack[field_index];
              }
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_MULNUM_LONG: {
            if (!exception_flag) {
              int32_t fields_length = opcode->operand3;
              for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                long_vars[opcode->operand0 + field_index] = *(int64_t*)&stack[field_index];
              }
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_MULNUM_FLOAT: {
            if (!exception_flag) {
              int32_t fields_length = opcode->operand3;
              for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                float_vars[opcode->operand0 + field_index] = *(float*)&stack[field_index];
              }
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_MULNUM_DOUBLE: {
            if (!exception_flag) {
              int32_t fields_length = opcode->operand3;
              for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                double_vars[opcode->operand0 + field_index] = *(double*)&stack[field_index];
              }
            }
            break;
          }
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_CALL_CALLBACK_METHOD:
      {
        int32_t decl_method_id = opcode->operand1;
        SPVM_METHOD* decl_method = SPVM_LIST_fetch(compiler->methods, decl_method_id);
        void* object = *(void**)&object_vars[opcode->operand2];
        const char* decl_method_name = decl_method->name;
        const char* decl_method_signature = decl_method->signature;
        int32_t call_method_id = env->get_instance_method_id(env, object, decl_method_name, decl_method_signature);
        call_spvm_method_arg_stack_top -= decl_method->args_alloc_length;
        exception_flag = env->call_spvm_method(env, call_method_id, stack);
        
        switch (decl_method->return_type_category_id) {
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_VOID: {
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_BYTE: {
            if (!exception_flag) {
              byte_vars[opcode->operand0] = *(int8_t*)&stack[0];
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_SHORT: {
            if (!exception_flag) {
              short_vars[opcode->operand0] = *(int16_t*)&stack[0];
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_INT: {
            if (!exception_flag) {
              int_vars[opcode->operand0] = *(int32_t*)&stack[0];
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_LONG: {
            if (!exception_flag) {
              long_vars[opcode->operand0] = *(int64_t*)&stack[0];
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_FLOAT: {
            if (!exception_flag) {
              float_vars[opcode->operand0] = *(float*)&stack[0];
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_DOUBLE: {
            if (!exception_flag) {
              double_vars[opcode->operand0] = *(double*)&stack[0];
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_OBJECT: {
            if (!exception_flag) {
              SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], stack[0].oval);
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_MULNUM_BYTE: {
            if (!exception_flag) {
              int32_t fields_length = opcode->operand3;
              for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                byte_vars[opcode->operand0 + field_index] = *(int8_t*)&stack[field_index];
              }
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_MULNUM_SHORT: {
            if (!exception_flag) {
              int32_t fields_length = opcode->operand3;
              for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                short_vars[opcode->operand0 + field_index] = *(int16_t*)&stack[field_index];
              }
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_MULNUM_INT: {
            if (!exception_flag) {
              int32_t fields_length = opcode->operand3;
              for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                int_vars[opcode->operand0 + field_index] = *(int32_t*)&stack[field_index];
              }
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_MULNUM_LONG: {
            if (!exception_flag) {
              int32_t fields_length = opcode->operand3;
              for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                long_vars[opcode->operand0 + field_index] = *(int64_t*)&stack[field_index];
              }
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_MULNUM_FLOAT: {
            if (!exception_flag) {
              int32_t fields_length = opcode->operand3;
              for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                float_vars[opcode->operand0 + field_index] = *(float*)&stack[field_index];
              }
            }
            break;
          }
          case SPVM_METHOD_C_RETURN_TYPE_CATEGORY_ID_MULNUM_DOUBLE: {
            if (!exception_flag) {
              int32_t fields_length = opcode->operand3;
              for (int32_t field_index = 0; field_index < fields_length; field_index++) {
                double_vars[opcode->operand0 + field_index] = *(double*)&stack[field_index];
              }
            }
            break;
          }
        }
        
        break;
      }
      case SPVM_OPCODE_C_ID_IF_EXCEPTION_CATCH: {
        if (exception_flag) {
          exception_flag = 0;
          
          SPVM_METHOD* method = SPVM_LIST_fetch(class->methods, opcode->operand1);
          int32_t line = opcode->operand2;
          
          const char* method_name = method->name;
          SPVM_CLASS* method_class = method->class;
          const char* class_name = method_class->name;
          const char* file = method->class->module_file;
          
          // Exception stack trace
          env->set_exception(env, env->new_stack_trace_raw(env, env->get_exception(env), class_name, method_name, file, line));
          opcode_rel_index = opcode->operand0;
          continue;
        }
        break;
      }
      case SPVM_OPCODE_C_ID_IF_EXCEPTION_RETURN: {
        if (exception_flag) {
          SPVM_METHOD* method = SPVM_LIST_fetch(class->methods, opcode->operand1);
          int32_t line = opcode->operand2;
          
          const char* method_name = method->name;
          SPVM_CLASS* method_class = SPVM_LIST_fetch(compiler->classes, method->class->id);
          const char* class_name = method_class->name;
          const char* file = class->module_file;

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
      case SPVM_OPCODE_C_ID_WARN: {
        int32_t line = opcode->operand1;
        
        SPVM_CLASS* method_class = SPVM_LIST_fetch(compiler->classes, method->class->id);
        const char* file = class->module_file;
        
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
        
        SPVM_SWITCH_INFO* switch_info = class->info_switch_infos->values[switch_id];

        // Default branch
        int32_t default_opcode_rel_index = switch_info->default_opcode_rel_index;
        
        // Cases length
        int32_t case_infos_length = switch_info->case_infos->length;

        if (case_infos_length > 0) {
          // min
          SPVM_CASE_INFO* min_case_info = (SPVM_CASE_INFO*)switch_info->case_infos->values[0];
          int32_t min = min_case_info->constant->value.ival;
          
          // max
          SPVM_CASE_INFO* max_case_info = (SPVM_CASE_INFO*)switch_info->case_infos->values[case_infos_length - 1];
          int32_t max = max_case_info->constant->value.ival;
          
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
              SPVM_CASE_INFO* cur_half_case_info = (SPVM_CASE_INFO*)switch_info->case_infos->values[cur_half_pos];
              int32_t cur_half = cur_half_case_info->constant->value.ival;
              
              if (int_vars[opcode->operand0] > cur_half) {
                cur_min_pos = cur_half_pos + 1;
              }
              else if (int_vars[opcode->operand0] < cur_half) {
                cur_max_pos = cur_half_pos - 1;
              }
              else {
                opcode_rel_index = cur_half_case_info->opcode_rel_index;
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];

        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;

        void* object = *(void**)&object_vars[opcode->operand1];
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;
        
        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;

        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
        int32_t field_offset = field->offset;

        if (__builtin_expect(object == NULL, 0)) {
          void* exception = env->new_string_nolen_raw(env, "Invocant must be defined");
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
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
        SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
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
        int32_t basic_type_id = (intptr_t)(void*)env->byte_object_basic_type_id;

        void* object = env->new_object_raw(env, basic_type_id);
        SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
        *(int8_t*)&fields[0] = value;
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
        
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_SHORT_TO_SHORT_OBJECT: {
        int16_t value = short_vars[opcode->operand1];
        int32_t basic_type_id = (intptr_t)(void*)env->short_object_basic_type_id;
        void* object = env->new_object_raw(env, basic_type_id);
        SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
        *(int16_t*)&fields[0] = value;
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
        
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_INT_TO_INT_OBJECT: {
        int32_t value = int_vars[opcode->operand1];
        int32_t basic_type_id = (intptr_t)(void*)env->int_object_basic_type_id;
        void* object = env->new_object_raw(env, basic_type_id);
        SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
        *(int32_t*)&fields[0] = value;
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
        
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_LONG_TO_LONG_OBJECT: {
        int64_t value = long_vars[opcode->operand1];
        int32_t basic_type_id = (intptr_t)(void*)env->long_object_basic_type_id;
        void* object = env->new_object_raw(env, basic_type_id);
        SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
        *(int64_t*)&fields[0] = value;
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
        
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_FLOAT_TO_FLOAT_OBJECT: {
        float value = float_vars[opcode->operand1];
        int32_t basic_type_id = (intptr_t)(void*)env->float_object_basic_type_id;
        void* object = env->new_object_raw(env, basic_type_id);
        SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
        *(float*)&fields[0] = value;
        SPVM_API_OBJECT_ASSIGN((void**)&object_vars[opcode->operand0], object);
        
        break;
      }
      case SPVM_OPCODE_C_ID_CONVERT_DOUBLE_TO_DOUBLE_OBJECT: {
        double value = double_vars[opcode->operand1];
        int32_t basic_type_id = (intptr_t)(void*)env->double_object_basic_type_id;
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
          if (object_basic_type_id == (intptr_t)(void*)env->byte_object_basic_type_id && object_type_dimension == 0) {
            SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
            byte_vars[opcode->operand0] = *(int8_t*)&fields[0];
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "Can't convert imcompatible object type from Byte to byte.");
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
          if (object_basic_type_id == (intptr_t)(void*)env->short_object_basic_type_id && object_type_dimension == 0) {
            SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
            short_vars[opcode->operand0] = *(int16_t*)&fields[0];
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "Can't convert imcompatible object type from Short to short.");
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
          if (object_basic_type_id == (intptr_t)(void*)env->int_object_basic_type_id && object_type_dimension == 0) {
            SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
            int_vars[opcode->operand0] = *(int32_t*)&fields[0];
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "Can't convert imcompatible object type from Int to int.");
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
          if (object_basic_type_id == (intptr_t)(void*)env->long_object_basic_type_id && object_type_dimension == 0) {
            SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
            long_vars[opcode->operand0] = *(int64_t*)&fields[0];
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "Can't convert imcompatible object type from Long to long.");
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
          if (object_basic_type_id == (intptr_t)(void*)env->float_object_basic_type_id && object_type_dimension == 0) {
            SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
            float_vars[opcode->operand0] = *(float*)&fields[0];
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "Can't convert imcompatible object type from Float to float.");
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
          if (object_basic_type_id == (intptr_t)(void*)env->double_object_basic_type_id && object_type_dimension == 0) {
            SPVM_VALUE* fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);
            double_vars[opcode->operand0] = *(double*)&fields[0];
          }
          else {
            void* exception = env->new_string_nolen_raw(env, "Can't convert imcompatible object type from Double to double.");
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
    switch (method->return_type_category) {
      case SPVM_TYPE_C_TYPE_CATEGORY_ANY_OBJECT:
      case SPVM_TYPE_C_TYPE_CATEGORY_CLASS:
      case SPVM_TYPE_C_TYPE_CATEGORY_NUMERIC_ARRAY:
      case SPVM_TYPE_C_TYPE_CATEGORY_MULNUM_ARRAY:
      case SPVM_TYPE_C_TYPE_CATEGORY_OBJECT_ARRAY:
      case SPVM_TYPE_C_TYPE_CATEGORY_STRING:
      {
        if (*(void**)&stack[0] != NULL) {
          SPVM_API_DEC_REF_COUNT_ONLY(*(void**)&stack[0]);
        }
      }
    }
  }
  
  SPVM_API_free_memory_block(env, call_stack);
  
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

int32_t SPVM_API_has_callback(SPVM_ENV* env, SPVM_OBJECT* object, int32_t callback_basic_type_id) {
  (void)env;
  
  // Object must be not null
  assert(object);
  
  int32_t object_basic_type_id = object->basic_type_id;
  int32_t object_type_dimension = object->type_dimension;
  
  int32_t has_callback;
  if (object_type_dimension != 0) {
    has_callback = 0;
  }
  else {
    SPVM_COMPILER* compiler = env->compiler;

    SPVM_BASIC_TYPE* object_basic_type = SPVM_LIST_fetch(compiler->basic_types, object_basic_type_id);
    SPVM_BASIC_TYPE* callback_basic_type = SPVM_LIST_fetch(compiler->basic_types, callback_basic_type_id);

    SPVM_CLASS* object_class = SPVM_LIST_fetch(compiler->classes, object_basic_type->class->id);
    SPVM_CLASS* callback_class = SPVM_LIST_fetch(compiler->classes, callback_basic_type->class->id);
    
    SPVM_METHOD* method_callback = SPVM_LIST_fetch(callback_class->methods, 0);
    
    const char* method_callback_signature = method_callback->signature;
    if (object_class->flag & SPVM_CLASS_C_FLAG_ANON_METHOD_CLASS) {
      SPVM_METHOD* object_class_method = SPVM_LIST_fetch(object_class->methods, 0);
      
      if (strcmp(method_callback_signature, object_class_method->signature) == 0) {
        has_callback = 1;
      }
      else {
        has_callback = 0;
      }
    }
    else {
      const char* object_class_name = object_class->name;
      const char* method_callback_name = method_callback->name;
      int32_t method_id = SPVM_API_get_class_method_id(env, object_class_name, method_callback_name, method_callback_signature);
      if (method_id >= 0) {
        has_callback = 1;
      }
      else {
        has_callback = 0;
      }
    }
  }
  
  return has_callback;
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
  
  SPVM_COMPILER* compiler = env->compiler;
  
  int32_t basic_type_id = object->basic_type_id;
  int32_t type_dimension = object->type_dimension;
  
  SPVM_BASIC_TYPE* basic_type = SPVM_LIST_fetch(compiler->basic_types, basic_type_id);
  const char* basic_type_name = basic_type->name;
  
  int32_t length = 0;
  
  
  // Basic type
  length += strlen(basic_type_name);
  
  //[]
  length += type_dimension * 2;
  
  int32_t scope_id = env->enter_scope(env);
  void* type_name_byte_array = env->new_byte_array(env, length + 1);
  
  int8_t* cur = env->get_elems_byte(env, type_name_byte_array);
  
  int32_t cur_index = 0;
  sprintf((char*)cur, "%s", basic_type_name);
  cur_index += strlen(basic_type_name);
  
  int32_t dim_index;
  for (dim_index = 0; dim_index < type_dimension; dim_index++) {
    sprintf((char*)(cur + cur_index), "[]");
    cur_index += 2;
  }
  
  void* sv_type_name = env->new_string_raw(env, (const char*)cur, length);
  
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
  const char* new_line_part = "\n ";
  const char* arrow_part = "->";
  const char* at_part = " called at ";

  // Exception
  int8_t* exception_bytes = env->get_elems_byte(env, exception);
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
  int8_t* new_exception_bytes = env->get_elems_byte(env, new_exception);
  
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

void SPVM_API_print(SPVM_ENV* env, SPVM_OBJECT* string) {
  (void)env;
  
  int8_t* bytes = env->get_elems_byte(env, string);
  int32_t string_length = env->length(env, string);
  
  {
    int32_t i;
    for (i = 0; i < string_length; i++) {
      putchar((char)bytes[i]);
    }
  }
}

SPVM_OBJECT* SPVM_API_concat_raw(SPVM_ENV* env, SPVM_OBJECT* string1, SPVM_OBJECT* string2) {
  (void)env;

  int32_t string1_length = SPVM_API_length(env, string1);
  int32_t string2_length = SPVM_API_length(env, string2);
  
  int32_t string3_length = string1_length + string2_length;
  SPVM_OBJECT* string3 = SPVM_API_new_byte_array_raw(env, string3_length);

  string3->basic_type_id = SPVM_BASIC_TYPE_C_ID_STRING;
  string3->type_dimension = 0;
  string3->type_category = SPVM_TYPE_C_TYPE_CATEGORY_STRING;

  int8_t* string1_bytes = SPVM_API_get_elems_byte(env, string1);
  int8_t* string2_bytes = SPVM_API_get_elems_byte(env, string2);
  int8_t* string3_bytes = SPVM_API_get_elems_byte(env, string3);
  
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
  
  return (int32_t)(intptr_t)env->memory_blocks_count;
}

void SPVM_API_free_weaken_back_refs(SPVM_ENV* env, SPVM_WEAKEN_BACKREF* weaken_backref_head) {
  (void)env;
  
  SPVM_WEAKEN_BACKREF* temp = weaken_backref_head;
  SPVM_WEAKEN_BACKREF* swap = NULL;

  while(temp != NULL){
    swap = temp->next;
    *(temp->object_address) = NULL;
    SPVM_API_free_memory_block(env, temp);
    temp = swap;
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
  while ((*weaken_backref_next_address)->next != NULL){
    if ((*weaken_backref_next_address)->next->object_address == object_address) {
      SPVM_API_free_memory_block(env, (*weaken_backref_next_address)->next);
      *weaken_backref_next_address = (*weaken_backref_next_address)->next->next;
      break;
    }
    *weaken_backref_next_address = (*weaken_backref_next_address)->next;
  }
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
  
  object->basic_type_id = SPVM_BASIC_TYPE_C_ID_STRING;
  object->type_dimension = 0;
  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_STRING;
  
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
  
  object->basic_type_id = SPVM_BASIC_TYPE_C_ID_STRING;
  object->type_dimension = 0;
  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_STRING;

  if (bytes != NULL && length > 0) {
    memcpy((void*)((intptr_t)object + env->object_header_byte_size), (char*)bytes, length);
  }

  return object;
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
  object->basic_type_id = SPVM_BASIC_TYPE_C_ID_BYTE;
  object->length = length;
  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_NUMERIC_ARRAY;
  
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
  object->basic_type_id = SPVM_BASIC_TYPE_C_ID_SHORT;
  
  // Set array length
  object->length = length;

  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_NUMERIC_ARRAY;

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
  object->basic_type_id = SPVM_BASIC_TYPE_C_ID_INT;

  // Set array length
  object->length = length;

  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_NUMERIC_ARRAY;
  
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
  object->basic_type_id = SPVM_BASIC_TYPE_C_ID_LONG;

  // Set array length
  object->length = length;

  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_NUMERIC_ARRAY;
  
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
  object->basic_type_id = SPVM_BASIC_TYPE_C_ID_FLOAT;

  // Set array length
  object->length = length;

  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_NUMERIC_ARRAY;
  
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
  object->basic_type_id = SPVM_BASIC_TYPE_C_ID_DOUBLE;
  
  // Set array length
  object->length = length;

  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_NUMERIC_ARRAY;
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_object_array_raw(SPVM_ENV* env, int32_t basic_type_id, int32_t length) {
  (void)env;
  
  SPVM_COMPILER* compiler = env->compiler;

  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + sizeof(void*) * ((int64_t)length + 1);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }

  for (int32_t index = 0; index < length; index++) {
    SPVM_OBJECT* get_field_object = ((SPVM_OBJECT**)((intptr_t)object + env->object_header_byte_size))[index];
  }

  SPVM_BASIC_TYPE* basic_type = SPVM_LIST_fetch(compiler->basic_types, basic_type_id);

  object->basic_type_id = basic_type->id;
  object->type_dimension = 1;

  // Set array length
  object->length = length;
  
  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_OBJECT_ARRAY;
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_muldim_array_raw(SPVM_ENV* env, int32_t basic_type_id, int32_t element_dimension, int32_t length) {
  (void)env;
  
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
  
  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_OBJECT_ARRAY;
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_mulnum_array_raw(SPVM_ENV* env, int32_t basic_type_id, int32_t length) {
  (void)env;

  SPVM_COMPILER* compiler = env->compiler;

  // valut_t array dimension must be 1
  SPVM_BASIC_TYPE* basic_type = SPVM_LIST_fetch(compiler->basic_types, basic_type_id);
  const char* basic_type_name = basic_type->name;
  
  // Class
  SPVM_CLASS* class = basic_type->class;
  int32_t fields_length = class->fields->length;
  SPVM_FIELD* field_first = SPVM_LIST_fetch(class->fields, 0);
  int32_t field_basic_type_id = field_first->type->basic_type->id;

  int32_t unit_size;
  if (field_basic_type_id == SPVM_BASIC_TYPE_C_ID_BYTE) {
    unit_size = sizeof(int8_t);
  }
  else if (field_basic_type_id == SPVM_BASIC_TYPE_C_ID_SHORT) {
    unit_size = sizeof(int16_t);
  }
  else if (field_basic_type_id == SPVM_BASIC_TYPE_C_ID_INT) {
    unit_size = sizeof(int32_t);
  }
  else if (field_basic_type_id == SPVM_BASIC_TYPE_C_ID_LONG) {
    unit_size = sizeof(int64_t);
  }
  else if (field_basic_type_id == SPVM_BASIC_TYPE_C_ID_FLOAT) {
    unit_size = sizeof(float);
  }
  else if (field_basic_type_id == SPVM_BASIC_TYPE_C_ID_DOUBLE) {
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

  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_MULNUM_ARRAY;

  return object;
}

SPVM_OBJECT* SPVM_API_new_object_raw(SPVM_ENV* env, int32_t basic_type_id) {
  (void)env;
  
  SPVM_COMPILER* compiler = env->compiler;
  
  SPVM_BASIC_TYPE* basic_type = SPVM_LIST_fetch(compiler->basic_types, basic_type_id);
  
  SPVM_CLASS* class;
  if (!basic_type->class) {
    class = NULL;
  }
  else {
    class = basic_type->class;
  }
  if (!class) {
    return NULL;
  }

  // Alloc body length + 1
  int32_t fields_length = class->fields->length;

  int64_t alloc_byte_size = (intptr_t)env->object_header_byte_size + sizeof(SPVM_VALUE) * ((int64_t)fields_length + 1);
  
  // Create object
  SPVM_OBJECT* object = SPVM_API_alloc_memory_block_zero(env, alloc_byte_size);
  if (!object) {
    return NULL;
  }
  
  object->basic_type_id = basic_type->id;
  object->type_dimension = 0;

  object->length = fields_length;

  // Object type id
  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_CLASS;
  
  // Has destructor
  if (class->method_destructor) {
    object->flag |= SPVM_OBJECT_C_FLAG_HAS_DESTRUCTOR;
  }
  
  return object;
}

SPVM_OBJECT* SPVM_API_new_pointer_raw(SPVM_ENV* env, int32_t basic_type_id, void* pointer) {
  (void)env;
  
  SPVM_COMPILER* compiler = env->compiler;
  
  SPVM_BASIC_TYPE* basic_type = SPVM_LIST_fetch(compiler->basic_types, basic_type_id);

  SPVM_CLASS* class;
  if (!basic_type->class) {
    class = NULL;
  }
  else {
    class = basic_type->class;
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

  // Object type id
  object->type_category = SPVM_TYPE_C_TYPE_CATEGORY_CLASS;
  
  // Has destructor
  if (class->method_destructor) {
    object->flag |= SPVM_OBJECT_C_FLAG_HAS_DESTRUCTOR;
  }
  
  return object;
}

int32_t SPVM_API_object_type_dimension(SPVM_ENV* env, SPVM_OBJECT* object) {
  return object->type_dimension;
}

int32_t SPVM_API_object_basic_type_id(SPVM_ENV* env, SPVM_OBJECT* object) {
  return object->basic_type_id;
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
    // Free elements of object array
    if (object->type_category == SPVM_TYPE_C_TYPE_CATEGORY_OBJECT_ARRAY) {
      int32_t length = object->length;
      for (int32_t index = 0; index < length; index++) {
        SPVM_OBJECT** get_field_object_address = &(((SPVM_OBJECT**)((intptr_t)object + env->object_header_byte_size))[index]);

        if (*get_field_object_address != NULL) {
          SPVM_API_dec_ref_count(env, *get_field_object_address);
        }
      }
    }
    // Free class object
    else if (object->type_category == SPVM_TYPE_C_TYPE_CATEGORY_CLASS) {

      // Class
      SPVM_COMPILER* compiler = env->compiler;
      SPVM_BASIC_TYPE* basic_type = SPVM_LIST_fetch(compiler->basic_types, object->basic_type_id);
      SPVM_CLASS* class;
      if (!basic_type->class) {
        class = NULL;
      }
      else {
        class = basic_type->class;
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
        int32_t exception_flag = SPVM_API_call_spvm_method(env, class->method_destructor->id, args);
        
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
    
    // Free weak back refenreces
    if (object->weaken_backref_head != NULL) {
      SPVM_API_free_weaken_back_refs(env, object->weaken_backref_head);
      object->weaken_backref_head = NULL;
    }
    
    // Decrement reference count
    object->ref_count--;
  
    // Free object
    SPVM_API_free_memory_block(env, object);
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

int32_t SPVM_API_ref_count(SPVM_ENV* env, SPVM_OBJECT* object) {
  (void)env;
  
  return object->ref_count;
}

int32_t SPVM_API_get_field_offset(SPVM_ENV* env, int32_t field_id) {
  (void)env;
  
  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
  
  return field->offset;
}

SPVM_FIELD* SPVM_API_get_field(SPVM_ENV* env, SPVM_CLASS* class, const char* field_name) {
  // Runtime
  SPVM_COMPILER* compiler = env->compiler;

  SPVM_FIELD* field = SPVM_HASH_fetch(class->field_symtable, field_name, strlen(field_name));
  
  return field;
}

int32_t SPVM_API_get_field_id(SPVM_ENV* env, const char* class_name, const char* field_name, const char* signature) {
  (void)env;
  
  // Basic type
  SPVM_BASIC_TYPE* basic_type = SPVM_API_get_basic_type(env, class_name);
  if (!basic_type) {
    return -1;
  }
  
  if (!basic_type->class) {
    return -1;
  }
  
  // Class
  SPVM_CLASS* class = basic_type->class;
  
  // Field
  SPVM_FIELD* field = SPVM_API_get_field(env, class, field_name);
  if (!field) {
    return -1;
  }

  // Signature
  if (strcmp(signature, field->signature) != 0) {
    return -1;
  }
  
  int32_t field_id = field->id;
  
  return field_id;
}

int32_t SPVM_API_get_class_var_id(SPVM_ENV* env, const char* class_name, const char* class_var_name, const char* signature) {
  (void)env;
  
  // Basic type
  SPVM_BASIC_TYPE* basic_type = SPVM_API_get_basic_type(env, class_name);
  
  // Class name
  SPVM_CLASS* class;
  if (!basic_type) {
    return -1;
  }
  else if (!basic_type->class) {
    return -1;
  }
  else {
    class = basic_type->class;
  }

  // Class variable name
  SPVM_CLASS_VAR* class_var = SPVM_API_get_class_var(env, class, class_var_name);
  if (!class_var) {
    return -1;
  }
  
  // Signature
  if (strcmp(signature, class_var->signature) != 0) {
    return -1;
  }
 
  return class_var->id;
}

SPVM_METHOD* SPVM_API_get_method(SPVM_ENV* env, SPVM_CLASS* class, const char* method_name) {

  SPVM_METHOD* method = SPVM_HASH_fetch(class->method_symtable, method_name, strlen(method_name));
  
  return method;
}

int32_t SPVM_API_get_class_method_id(SPVM_ENV* env, const char* class_name, const char* method_name, const char* signature) {
  (void)env;
  
  // Method ID
  int32_t method_id = -1;
  
  // Basic type
  SPVM_BASIC_TYPE* basic_type = SPVM_API_get_basic_type(env, class_name);
  if (basic_type) {
    
    // Class
    SPVM_CLASS* class = basic_type->class;
    if (class) {

      // Method
      SPVM_METHOD* method = SPVM_API_get_method(env, class, method_name);
      if (method) {
        
        // Signature
        if (strcmp(signature, method->signature) == 0) {
          method_id = method->id;
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
  SPVM_COMPILER* compiler = env->compiler;
  
  // Basic type
  SPVM_BASIC_TYPE* basic_type = SPVM_LIST_fetch(compiler->basic_types, object->basic_type_id);
  
  // Class
  SPVM_CLASS* class = basic_type->class;
  if (class) {
    // Anon instance method
    if (class->flag & SPVM_CLASS_C_FLAG_ANON_METHOD_CLASS) {
      // Method name
      SPVM_METHOD* method = SPVM_LIST_fetch(class->methods, 0);
       
      // Signature
      if (strcmp(signature, method->signature) == 0) {
        method_id = method->id;
      }
    }
    // Normal instance method
    else {
      const char* class_name = class->name;
      method_id = SPVM_API_get_class_method_id(env, class_name, method_name, signature);
    }
  }

  return method_id;
}

SPVM_BASIC_TYPE* SPVM_API_get_basic_type(SPVM_ENV* env,  const char* basic_type_name) {
  // Runtime
  SPVM_COMPILER* compiler = env->compiler;

  SPVM_BASIC_TYPE* basic_type = (SPVM_BASIC_TYPE*)SPVM_HASH_fetch(compiler->basic_type_symtable, basic_type_name, strlen(basic_type_name));
  
  return basic_type;
}

int32_t SPVM_API_get_basic_type_id(SPVM_ENV* env, const char* basic_type_name) {
  (void)env;
  
  if (basic_type_name == NULL) {
    return -1;
  }
  
  SPVM_BASIC_TYPE* basic_type = SPVM_API_get_basic_type(env, basic_type_name);
  if (basic_type) {
    int32_t basic_type_id = basic_type->id;
    return basic_type_id;
  }
  else {
    return -1;
  }
}

int8_t SPVM_API_get_field_byte(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
  
  // Get field value
  int8_t value = *(int8_t*)((intptr_t)object + env->object_header_byte_size + field->offset);

  return value;
}

int16_t SPVM_API_get_field_short(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
  
  // Get field value
  int16_t value = *(int16_t*)((intptr_t)object + env->object_header_byte_size + field->offset);
  
  return value;
}

int32_t SPVM_API_get_field_int(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);

  // Get field value
  int32_t value = *(int32_t*)((intptr_t)object + env->object_header_byte_size + field->offset);
  
  return value;
}

int64_t SPVM_API_get_field_long(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);

  // Get field value
  int64_t value = *(int64_t*)((intptr_t)object + env->object_header_byte_size + field->offset);
  
  return value;
}

float SPVM_API_get_field_float(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);

  // Get field value
  float value = *(float*)((intptr_t)object + env->object_header_byte_size + field->offset);
  
  return value;
}

double SPVM_API_get_field_double(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);

  // Get field value
  double value = *(double*)((intptr_t)object + env->object_header_byte_size + field->offset);
  
  return value;
}

SPVM_OBJECT* SPVM_API_get_field_object(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
  
  // Get field value
  SPVM_OBJECT* value_maybe_weaken = *(SPVM_OBJECT**)((intptr_t)object + env->object_header_byte_size + field->offset);
  SPVM_OBJECT* value = SPVM_API_GET_OBJECT_NO_WEAKEN_ADDRESS(value_maybe_weaken);
  
  return value;
}

void SPVM_API_set_field_byte(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, int8_t value) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);

  // Get field value
  *(int8_t*)((intptr_t)object + env->object_header_byte_size + field->offset) = value;
}

void SPVM_API_set_field_short(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, int16_t value) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);

  // Get field value
  *(int16_t*)((intptr_t)object + env->object_header_byte_size + field->offset) = value;
}

void SPVM_API_set_field_int(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, int32_t value) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);

  // Get field value
  *(int32_t*)((intptr_t)object + env->object_header_byte_size + field->offset) = value;
}

void SPVM_API_set_field_long(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, int64_t value) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);

  // Get field value
  *(int64_t*)((intptr_t)object + env->object_header_byte_size + field->offset) = value;
}

void SPVM_API_set_field_float(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, float value) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);

  // Get field value
  *(float*)((intptr_t)object + env->object_header_byte_size + field->offset) = value;
}

void SPVM_API_set_field_double(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, double value) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);

  // Get field value
  *(double*)((intptr_t)object + env->object_header_byte_size + field->offset) = value;
}

void SPVM_API_set_field_object(SPVM_ENV* env, SPVM_OBJECT* object, int32_t field_id, SPVM_OBJECT* value) {

  // Runtime
  SPVM_COMPILER* compiler = env->compiler;
  
  // Field
  SPVM_FIELD* field = SPVM_LIST_fetch(compiler->fields, field_id);
  
  // Get field value
  void* get_field_object_address = (void**)((intptr_t)object + env->object_header_byte_size + field->offset);

  SPVM_API_OBJECT_ASSIGN(get_field_object_address, value);
}

void* SPVM_API_alloc_memory_block_zero(SPVM_ENV* env, int64_t byte_size) {
  
  assert(byte_size > 0);

  if ((uint64_t)byte_size > (uint64_t)SIZE_MAX) {
    return NULL;
  }
  
  void* block = calloc(1, (size_t)byte_size);
  
  if (block) {
    int32_t memory_blocks_count = (int32_t)(intptr_t)env->memory_blocks_count;
    memory_blocks_count++;
    env->memory_blocks_count = (void*)(intptr_t)memory_blocks_count;
  
#ifdef SPVM_DEBUG_ALLOC_MEMORY_COUNT
  fprintf(stderr, "[ALLOC_MEMORY %p %d]\n", block, (int32_t)(intptr_t)env->memory_blocks_count);
#endif
  }
  else {
    
  }

  return block;
}

void SPVM_API_free_memory_block(SPVM_ENV* env, void* block) {

  if (block) {
    int32_t memory_blocks_count = (int32_t)(intptr_t)env->memory_blocks_count;
    memory_blocks_count--;
    env->memory_blocks_count = (void*)(intptr_t)memory_blocks_count;
    
#ifdef SPVM_DEBUG_ALLOC_MEMORY_COUNT
    fprintf(stderr, "[FREE_MEMORY %p %d]\n", block, (int32_t)(intptr_t)env->memory_blocks_count);
#endif
    free(block);
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


// Private API
SPVM_CLASS_VAR* SPVM_API_get_class_var(SPVM_ENV* env, SPVM_CLASS* class, const char* class_var_name) {

  SPVM_CLASS_VAR* class_var = SPVM_HASH_fetch(class->class_var_symtable, class_var_name, strlen(class_var_name));
  
  return class_var;
}
