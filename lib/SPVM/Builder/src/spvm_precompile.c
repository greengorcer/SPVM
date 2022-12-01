#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>

#include "spvm_precompile.h"

#include "spvm_allocator.h"
#include "spvm_native.h"
#include "spvm_api.h"
#include "spvm_api_runtime.h"
#include "spvm_string_buffer.h"
#include "spvm_opcode.h"

SPVM_PRECOMPILE* SPVM_PRECOMPILE_new() {
  SPVM_PRECOMPILE* precompile = SPVM_ALLOCATOR_alloc_memory_block_unmanaged(sizeof(SPVM_PRECOMPILE));
  
  return precompile;
}

void SPVM_PRECOMPILE_free(SPVM_PRECOMPILE* precompile) {
  
  SPVM_ALLOCATOR_free_memory_block_unmanaged(precompile);
  precompile = NULL;
}

void SPVM_PRECOMPILE_set_runtime(SPVM_PRECOMPILE* precompile, SPVM_RUNTIME* runtime) {
  
  precompile->runtime = runtime;
}

SPVM_RUNTIME* SPVM_PRECOMPILE_get_runtime(SPVM_PRECOMPILE* precompile) {
  
  return precompile->runtime;
}

void SPVM_PRECOMPILE_create_precompile_source(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, const char* class_name) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "// ");
  SPVM_STRING_BUFFER_add(string_buffer, class_name);
  SPVM_STRING_BUFFER_add(string_buffer, "\n");
  
  // Class
  int32_t class_id = SPVM_API_RUNTIME_get_class_id_by_name(runtime, class_name);
  int32_t class_is_anon = SPVM_API_RUNTIME_get_class_is_anon(runtime, class_id);
  int32_t class_methods_base_id = SPVM_API_RUNTIME_get_class_methods_base_id(runtime, class_id);
  int32_t class_methods_length = SPVM_API_RUNTIME_get_class_methods_length(runtime, class_id);
  
  // Constant strings
  if (!class_is_anon) {
    // Head part - include and define
    SPVM_PRECOMPILE_build_head(precompile, string_buffer);
    SPVM_STRING_BUFFER_add(string_buffer, "static const char* CURRENT_CLASS_NAME = \"");
    SPVM_STRING_BUFFER_add(string_buffer, class_name);
    SPVM_STRING_BUFFER_add(string_buffer, "\";\n");
  }
  
  // Method decrations
  {
    int32_t method_index;
    for (method_index = 0; method_index < class_methods_length; method_index++) {
      int32_t method_id = class_methods_base_id + method_index;

      int32_t method_name_id = SPVM_API_RUNTIME_get_method_name_id(runtime, method_id);
      const char* method_name = SPVM_API_RUNTIME_get_name(runtime, method_name_id);
      int32_t method_has_precompile_flag = SPVM_API_RUNTIME_get_method_is_precompile(runtime, method_id);

      if (method_has_precompile_flag) {
        SPVM_PRECOMPILE_build_method_declaration(precompile, string_buffer, class_name, method_name);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
      }
    }
  }
  
  // Method implementations
  {
    int32_t method_index;
    for (method_index = 0; method_index < class_methods_length; method_index++) {
      int32_t method_id = class_methods_base_id + method_index;
      int32_t method_name_id = SPVM_API_RUNTIME_get_method_name_id(runtime, method_id);
      const char* method_name = SPVM_API_RUNTIME_get_name(runtime, method_name_id);
      int32_t method_has_precompile_flag = SPVM_API_RUNTIME_get_method_is_precompile(runtime, method_id);
      if (method_has_precompile_flag) {
        SPVM_PRECOMPILE_build_method_implementation(precompile, string_buffer, class_name, method_name);
      }
    }
  }
  
  // If the class has anon methods, the anon methods is merged to this class
  int32_t class_anon_methods_length = SPVM_API_RUNTIME_get_class_anon_methods_length(runtime, class_id);
  if (class_anon_methods_length > 0) {
    int32_t class_anon_methods_base_id = SPVM_API_RUNTIME_get_class_anon_methods_base_id(runtime, class_id);
    for (int32_t anon_method_id = class_anon_methods_base_id; anon_method_id < class_anon_methods_length; anon_method_id++) {
      int32_t anon_method_method_id = SPVM_API_RUNTIME_get_anon_method_method_id(runtime, anon_method_id);
      int32_t anon_method_class_id = SPVM_API_RUNTIME_get_method_class_id(runtime, anon_method_method_id);
      int32_t anon_method_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, anon_method_class_id);
      const char* anon_method_class_name = SPVM_API_RUNTIME_get_name(runtime, anon_method_class_name_id);
      SPVM_PRECOMPILE_create_precompile_source(precompile, string_buffer, anon_method_class_name);
    }
  }

  SPVM_STRING_BUFFER_add(string_buffer, "\n");
}

void SPVM_PRECOMPILE_build_head(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  // Include headers and define macros
  SPVM_STRING_BUFFER_add(string_buffer,
    "#include \"spvm_native.h\"\n"
    "#include \"spvm_inline_api.h\"\n"
    "\n"
    "#include <stdlib.h>\n"
    "#include <string.h>\n"
    "#include <stdio.h>\n"
    "#include <inttypes.h>\n"
  );
}

void SPVM_PRECOMPILE_build_method_declaration(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, const char* class_name, const char* method_name) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  // Method declaration
  SPVM_STRING_BUFFER_add(string_buffer, "int32_t SPVMPRECOMPILE__");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)class_name);
  SPVM_STRING_BUFFER_add(string_buffer, (char*)"__");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)method_name);
  {
    int32_t index = string_buffer->length - (strlen(class_name) + 2 + strlen(method_name));
    
    while (index < string_buffer->length) {
      if (string_buffer->value[index] == ':') {
        string_buffer->value[index] = '_';
      }
      index++;
    }
  }
  SPVM_STRING_BUFFER_add(string_buffer, "(SPVM_ENV* env, SPVM_VALUE* stack)");
}

void SPVM_PRECOMPILE_build_method_implementation(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, const char* class_name, const char* method_name) {
  SPVM_RUNTIME* runtime = precompile->runtime;

  
  // Class
  int32_t class_id = SPVM_API_RUNTIME_get_class_id_by_name(runtime, class_name);
  int32_t class_is_anon = SPVM_API_RUNTIME_get_class_is_anon(runtime, class_id);
  
  // Method
  int32_t method_id = SPVM_API_RUNTIME_get_method_id_by_name(runtime, class_name, method_name);
  int32_t method_return_type_id = SPVM_API_RUNTIME_get_method_return_type_id(runtime, method_id);

  // Method declaration
  SPVM_PRECOMPILE_build_method_declaration(precompile, string_buffer, class_name, method_name);

  // Block start
  SPVM_STRING_BUFFER_add(string_buffer, " {\n");

  if (class_is_anon) {
    SPVM_STRING_BUFFER_add(string_buffer,"    const char* CURRENT_CLASS_NAME = \"");
    SPVM_STRING_BUFFER_add(string_buffer, class_name);
    SPVM_STRING_BUFFER_add(string_buffer, "\";\n");
  }

  // Current method name
  SPVM_STRING_BUFFER_add(string_buffer, "  const char* CURRENT_METHOD_NAME = \"");
  SPVM_STRING_BUFFER_add(string_buffer, method_name);
  SPVM_STRING_BUFFER_add(string_buffer, "\";\n");
  
  // Object header byte size
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t object_header_byte_size = (intptr_t)env->object_header_byte_size;\n");
  
  // Call method argument stack top
  int32_t stack_index = 0;

  // object variable declarations
  int32_t call_stack_object_vars_length = SPVM_API_RUNTIME_get_method_call_stack_object_vars_length(runtime, method_id);
  if (call_stack_object_vars_length > 0) {
    SPVM_STRING_BUFFER_add(string_buffer, "  void* object_vars[");
    SPVM_STRING_BUFFER_add_int(string_buffer, call_stack_object_vars_length);
    SPVM_STRING_BUFFER_add(string_buffer, "] = {0};\n");
  }

  // ref variable declarations
  int32_t call_stack_ref_vars_length = SPVM_API_RUNTIME_get_method_call_stack_ref_vars_length(runtime, method_id);
  if (call_stack_ref_vars_length > 0) {
    SPVM_STRING_BUFFER_add(string_buffer, "  void* ref_vars[");
    SPVM_STRING_BUFFER_add_int(string_buffer, call_stack_ref_vars_length);
    SPVM_STRING_BUFFER_add(string_buffer, "] = {0};\n");
  }

  // double variable declarations
  int32_t call_stack_double_vars_length = SPVM_API_RUNTIME_get_method_call_stack_double_vars_length(runtime, method_id);
  if (call_stack_double_vars_length > 0) {
    SPVM_STRING_BUFFER_add(string_buffer, "  double double_vars[");
    SPVM_STRING_BUFFER_add_int(string_buffer, call_stack_double_vars_length);
    SPVM_STRING_BUFFER_add(string_buffer, "];\n");
  }

  // float variable declarations
  int32_t call_stack_float_vars_length = SPVM_API_RUNTIME_get_method_call_stack_float_vars_length(runtime, method_id);
  if (call_stack_float_vars_length > 0) {
    SPVM_STRING_BUFFER_add(string_buffer, "  float float_vars[");
    SPVM_STRING_BUFFER_add_int(string_buffer, call_stack_float_vars_length);
    SPVM_STRING_BUFFER_add(string_buffer, "];\n");
  }

  // long variable declarations
  int32_t call_stack_long_vars_length = SPVM_API_RUNTIME_get_method_call_stack_long_vars_length(runtime, method_id);
  if (call_stack_long_vars_length > 0) {
    SPVM_STRING_BUFFER_add(string_buffer, "  int64_t long_vars[");
    SPVM_STRING_BUFFER_add_int(string_buffer, call_stack_long_vars_length);
    SPVM_STRING_BUFFER_add(string_buffer, "];\n");
  }

  // int variable declarations
  int32_t call_stack_int_vars_length = SPVM_API_RUNTIME_get_method_call_stack_int_vars_length(runtime, method_id);
  if (call_stack_int_vars_length > 0) {
    SPVM_STRING_BUFFER_add(string_buffer, "  int32_t int_vars[");
    SPVM_STRING_BUFFER_add_int(string_buffer, call_stack_int_vars_length);
    SPVM_STRING_BUFFER_add(string_buffer, "];\n");
  }

  // Exception
  // volatile attribute is not needed, but the environment "FreeBSD 9.1" and "gcc 4.2.1" seems to performe wrong optimisation
  // in double pointer logic. volatile attribute fixed the test "ref.t" "SPVM::TestCase::Ref->test_pass_mulnum_ref_byte".
  // SPVM_STRING_BUFFER_add(string_buffer, "  volatile int32_t error = 0;\n");
  
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t error = 0;\n");
  
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t error_code = 1;\n");

  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t eval_error = 0;\n");

  int32_t method_mortal_stack_length = SPVM_API_RUNTIME_get_method_mortal_stack_length(runtime, method_id);
  if (method_mortal_stack_length > 0) {
    SPVM_STRING_BUFFER_add(string_buffer, "  int32_t mortal_stack[");
    SPVM_STRING_BUFFER_add_int(string_buffer, method_mortal_stack_length);
    SPVM_STRING_BUFFER_add(string_buffer, "];\n");
    SPVM_STRING_BUFFER_add(string_buffer, "  int32_t mortal_stack_top = 0;\n");
  }
  
  // short variable declarations
  int32_t call_stack_short_vars_length = SPVM_API_RUNTIME_get_method_call_stack_short_vars_length(runtime, method_id);
  if (call_stack_short_vars_length > 0) {
    SPVM_STRING_BUFFER_add(string_buffer, "  int16_t short_vars[");
    SPVM_STRING_BUFFER_add_int(string_buffer, call_stack_short_vars_length);
    SPVM_STRING_BUFFER_add(string_buffer, "];\n");
  }

  // byte variable declarations
  int32_t call_stack_byte_vars_length = SPVM_API_RUNTIME_get_method_call_stack_byte_vars_length(runtime, method_id);
  if (call_stack_byte_vars_length > 0) {
    SPVM_STRING_BUFFER_add(string_buffer, "  int8_t byte_vars[");
    SPVM_STRING_BUFFER_add_int(string_buffer, call_stack_byte_vars_length);
    SPVM_STRING_BUFFER_add(string_buffer, "];\n");
  }

  // Convert string
  SPVM_STRING_BUFFER_add(string_buffer, "  char convert_string_buffer[21];\n");

  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t original_mortal_stack_top = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t line = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t arg_mem_id = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t stack_index = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* object = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* object1 = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* object2 = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* object_address = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* new_object_raw = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* array = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t index = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t call_method_args_stack_length = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t call_method_id = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t return_value = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* string = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* string1 = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* string2 = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* string3 = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* exception = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* type_name = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* get_field_object = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* dump = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* src_string = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* byte_arra = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void** element_address = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void** get_field_object_address = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* src_byte_array = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  void* byte_array = NULL;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t args_stack_index = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t elem_isa = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t access_basic_type_id = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t cast_type_dimension = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t cast_basic_type_id = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t dist_type_dimension = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t dist_basic_type_id = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t object_type_dimension_id = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t object_basic_type_id = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t runtime_assignability = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_VALUE tmp_constant;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t access_field_id = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int8_t* mulnum_ref_byte;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int16_t* mulnum_ref_short;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t* mulnum_ref_int;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int64_t* mulnum_ref_long;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  float* mulnum_ref_float;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  double* mulnum_ref_double;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t type_stack_length;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int8_t value_byte;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int16_t value_short;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t value_int;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int64_t value_long;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  float value_float;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  double value_double;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t basic_type_id;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_VALUE* fields;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t src_string_length;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int8_t* src_byte_array_data;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t src_byte_array_length;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int8_t* byte_array_data;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t string_length;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  char* src_string_data;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t access_class_id;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t tmp_error_code;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t empty_or_undef;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  char* bytes;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t access_class_var_id;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t is_read_only;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t length;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t length1;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t length2;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  char* bytes1;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  char* bytes2;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t short_string_length;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t retval;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t cmp;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t args_stack_length = env->get_args_stack_length(env, stack);\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int32_t cur_method_id = env->api->runtime->get_method_id_by_name(env->runtime, CURRENT_CLASS_NAME, CURRENT_METHOD_NAME);\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  int8_t* element_ptr_byte;\n");


  SPVM_OPCODE* opcodes = SPVM_API_RUNTIME_get_opcodes(runtime);
  int32_t method_opcodes_base_id = SPVM_API_RUNTIME_get_method_opcodes_base_id(runtime, method_id);
  int32_t opcodes_length = SPVM_API_RUNTIME_get_method_opcodes_length(runtime, method_id);
  int32_t opcode_index = 0;
  
  SPVM_OPCODE* opcode = NULL;

  while (opcode_index < opcodes_length) {

    // Line label
    SPVM_STRING_BUFFER_add(string_buffer, "L");
    SPVM_STRING_BUFFER_add_int(string_buffer, opcode_index);
    SPVM_STRING_BUFFER_add(string_buffer, ": ");
    
    opcode = &(opcodes[method_opcodes_base_id + opcode_index]);

    int32_t opcode_id = opcode->id;

    SPVM_STRING_BUFFER_add(string_buffer, "// ");
    SPVM_STRING_BUFFER_add(string_buffer, (char*)(SPVM_OPCODE_C_ID_NAMES())[opcode_id]);
    SPVM_STRING_BUFFER_add(string_buffer, "\n");
    
    switch (opcode_id) {

      case SPVM_OPCODE_C_ID_END_METHOD: {
        break;
      }
      case SPVM_OPCODE_C_ID_GOTO: {
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_IF_EQ_ZERO: {
        SPVM_STRING_BUFFER_add(string_buffer, "  if (");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " == 0) { goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, "; }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_IF_NE_ZERO: {
        SPVM_STRING_BUFFER_add(string_buffer, "  if (");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " != 0) { goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, "; }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_LOOKUP_SWITCH: {
        // Default branch
        int32_t default_opcode_rel_index = opcode->operand1;
        
        // Cases length
        int32_t case_infos_length = opcode->operand2;

        SPVM_STRING_BUFFER_add(string_buffer, "  switch(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ") {\n");
        for (int32_t case_index = 0; case_index < case_infos_length; case_index++) {
          SPVM_OPCODE* opcode_case_info = &(opcodes[method_opcodes_base_id + opcode_index + 1 + case_index]);

          int32_t match = opcode_case_info->operand1;
          int32_t opcode_rel_index = opcode_case_info->operand2;
          
          SPVM_STRING_BUFFER_add(string_buffer, "    case ");
          SPVM_STRING_BUFFER_add_int(string_buffer, match);
          SPVM_STRING_BUFFER_add(string_buffer, ": goto L");
          SPVM_STRING_BUFFER_add_int(string_buffer, opcode_rel_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        SPVM_STRING_BUFFER_add(string_buffer, "    default: goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, default_opcode_rel_index);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
        
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_CASE_INFO: {
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_MORTAL: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_PUSH_MORTAL(mortal_stack, mortal_stack_top, ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_LEAVE_SCOPE: {
        int32_t original_mortal_stack_top = opcode->operand0;
        if (method_mortal_stack_length > 0) {
          SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_LEAVE_SCOPE(env, stack, object_vars, mortal_stack, &mortal_stack_top, original_mortal_stack_top = ");
          SPVM_STRING_BUFFER_add_int(string_buffer, original_mortal_stack_top);
          SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        }
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_BYTE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_INIT_BYTE(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_SHORT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_INIT_SHORT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_INIT_INT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_INIT_LONG(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_FLOAT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_INIT_FLOAT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_DOUBLE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_INIT_DOUBLE(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_OBJECT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", NULL);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_MULNUM_BYTE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_INIT_MULNUM_BYTE(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_MULNUM_SHORT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_INIT_MULNUM_SHORT(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_MULNUM_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_INIT_MULNUM_INT(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_MULNUM_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_INIT_MULNUM_LONG(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_MULNUM_FLOAT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_INIT_MULNUM_FLOAT(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_INIT_MULNUM_DOUBLE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_INIT_MULNUM_DOUBLE(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_CONSTANT_BYTE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_STRING_BUFFER_add_byte(string_buffer, (int8_t)opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_CONSTANT_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, (int32_t)opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_CONSTANT_LONG: {
        int64_t long_value = *(int64_t*)&opcode->operand1;
        
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_STRING_BUFFER_add_long(string_buffer, long_value);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_CONSTANT_FLOAT: {
        SPVM_VALUE value;
        value.ival = (int32_t)opcode->operand1;
        
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = (tmp_constant.ival = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, value.ival);
        SPVM_STRING_BUFFER_add(string_buffer, ", tmp_constant.fval);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_CONSTANT_DOUBLE: {
        double double_value = *(double*)&opcode->operand1;
        SPVM_VALUE value;
        value.dval = double_value;
        
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = (tmp_constant.lval = ");
        SPVM_STRING_BUFFER_add_long(string_buffer, value.lval);
        SPVM_STRING_BUFFER_add(string_buffer, ", tmp_constant.dval);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_BYTE: {
        SPVM_PRECOMPILE_add_move(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_SHORT: {
        SPVM_PRECOMPILE_add_move(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_INT: {
        SPVM_PRECOMPILE_add_move(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_LONG: {
        SPVM_PRECOMPILE_add_move(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_FLOAT: {
        SPVM_PRECOMPILE_add_move(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_DOUBLE: {
        SPVM_PRECOMPILE_add_move(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_OBJECT: {
          SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
          SPVM_STRING_BUFFER_add(string_buffer, ", ");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
          SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_OBJECT_WITH_TYPE_CHECKING: {
        int32_t cast_basic_type = opcode->operand2;
        int32_t cast_type_dimension = opcode->operand3;

        int32_t cast_basic_type_name_id = SPVM_API_RUNTIME_get_basic_type_name_id(runtime, cast_basic_type);
        const char* cast_basic_type_name = SPVM_API_RUNTIME_get_name(runtime, cast_basic_type_name_id);
        
        SPVM_STRING_BUFFER_add(string_buffer, "  access_basic_type_id = env->get_basic_type_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)cast_basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "  if (access_basic_type_id < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The basic type \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)cast_basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "  env->set_exception(env, stack, exception);\n"
                                              "  error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    cast_basic_type_id = access_basic_type_id;\n"
                                              "    cast_type_dimension = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, cast_type_dimension);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "      object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    runtime_assignability = env->isa(env, stack, object, cast_basic_type_id, cast_type_dimension);\n"
                                              "    if (runtime_assignability) {\n"
                                              "      SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_VALUE_ASSIGN_NON_ASSIGNABLE_TYPE]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_OBJECT_CHECK_READ_ONLY: {
          SPVM_STRING_BUFFER_add(string_buffer, "  string = ");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                                "  if (env->is_read_only(env, stack, string)) {\n"
                                                "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ASSIGN_READ_ONLY_STRING_TO_MUTABLE_TYPE]);\n"
                                                "    env->set_exception(env, stack, exception);\n"
                                                "    error = 1;\n"
                                                "  }\n"
                                                "  else {\n"
                                                "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
          SPVM_STRING_BUFFER_add(string_buffer, ", string);\n"
                                                "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_REF: {
        SPVM_PRECOMPILE_add_move(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_ADD_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_ADD_INT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_ADD_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_ADD_LONG(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_ADD_FLOAT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_ADD_FLOAT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_ADD_DOUBLE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_ADD_DOUBLE(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SUBTRACT_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_SUBTRACT_INT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SUBTRACT_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_SUBTRACT_LONG(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SUBTRACT_FLOAT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_SUBTRACT_FLOAT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SUBTRACT_DOUBLE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_SUBTRACT_DOUBLE(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MULTIPLY_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_MULTIPLY_INT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MULTIPLY_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_MULTIPLY_LONG(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MULTIPLY_FLOAT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_MULTIPLY_FLOAT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MULTIPLY_DOUBLE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_MULTIPLY_DOUBLE(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_DIVIDE_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_DIVIDE_INT(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ", &error);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_DIVIDE_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_DIVIDE_LONG(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ", &error);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_DIVIDE_FLOAT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_DIVIDE_FLOAT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_DIVIDE_DOUBLE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_DIVIDE_DOUBLE(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_DIVIDE_UNSIGNED_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_DIVIDE_UNSIGNED_INT(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ", &error);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_DIVIDE_UNSIGNED_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_DIVIDE_UNSIGNED_LONG(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ", &error);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_REMAINDER_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_REMAINDER_INT(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ", &error);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_REMAINDER_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_REMAINDER_LONG(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ", &error);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_REMAINDER_UNSIGNED_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_REMAINDER_UNSIGNED_INT(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ", &error);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_REMAINDER_UNSIGNED_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_REMAINDER_UNSIGNED_LONG(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ", &error);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_LEFT_SHIFT_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_LEFT_SHIFT_INT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_LEFT_SHIFT_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_LEFT_SHIFT_LONG(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RIGHT_ARITHMETIC_SHIFT_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_RIGHT_ARITHMETIC_SHIFT_INT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RIGHT_ARITHMETIC_SHIFT_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_RIGHT_ARITHMETIC_SHIFT_LONG(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RIGHT_LOGICAL_SHIFT_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_RIGHT_LOGICAL_SHIFT_INT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RIGHT_LOGICAL_SHIFT_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_RIGHT_LOGICAL_SHIFT_LONG(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_AND_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_BIT_AND_INT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_AND_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_BIT_AND_LONG(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_OR_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_BIT_OR_INT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_OR_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_BIT_OR_LONG(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_XOR_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_BIT_XOR_INT(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_XOR_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_BIT_XOR_LONG(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ", ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_NOT_INT: {
        SPVM_PRECOMPILE_add_complement(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_BIT_NOT_LONG: {
        SPVM_PRECOMPILE_add_complement(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_NEGATE_INT: {
        SPVM_PRECOMPILE_add_negate(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_NEGATE_LONG: {
        SPVM_PRECOMPILE_add_negate(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_NEGATE_FLOAT: {
        SPVM_PRECOMPILE_add_negate(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_NEGATE_DOUBLE: {
        SPVM_PRECOMPILE_add_negate(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_CONCAT:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  string1 = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  string2 = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (string1 == NULL) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_CONCAT_LEFT_UNDEFINED]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else if (string2 == NULL) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_CONCAT_RIGHT_UNDEFINED]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    string3 = env->concat_raw(env, stack, string1, string2);\n"
                                              "    SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", string3);\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_CONVERSION_INT: {
        SPVM_PRECOMPILE_add_bool(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_CONVERSION_LONG: {
        SPVM_PRECOMPILE_add_bool(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_CONVERSION_FLOAT: {
        SPVM_PRECOMPILE_add_bool(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_CONVERSION_DOUBLE: {
        SPVM_PRECOMPILE_add_bool(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_CONVERSION_OBJECT: {
        SPVM_PRECOMPILE_add_bool(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_CONVERSION_REF: {
        SPVM_PRECOMPILE_add_bool(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_BOOL_CONVERSION_BOOL_OBJECT: {
        SPVM_PRECOMPILE_add_bool(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BOOL_CONVERSION_OBJECT, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_EQ_INT: {
        SPVM_PRECOMPILE_add_numeric_eq(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_EQ_LONG: {
        SPVM_PRECOMPILE_add_numeric_eq(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_EQ_FLOAT: {
        SPVM_PRECOMPILE_add_numeric_eq(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_EQ_DOUBLE: {
        SPVM_PRECOMPILE_add_numeric_eq(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_EQ_OBJECT: {
        SPVM_PRECOMPILE_add_numeric_eq(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_EQ_REF: {
        SPVM_PRECOMPILE_add_numeric_eq(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_NE_INT: {
        SPVM_PRECOMPILE_add_numeric_ne(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_NE_LONG: {
        SPVM_PRECOMPILE_add_numeric_ne(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_NE_FLOAT: {
        SPVM_PRECOMPILE_add_numeric_ne(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_NE_DOUBLE: {
        SPVM_PRECOMPILE_add_numeric_ne(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_NE_OBJECT: {
        SPVM_PRECOMPILE_add_numeric_ne(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_NE_REF: {
        SPVM_PRECOMPILE_add_numeric_ne(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GT_INT: {
        SPVM_PRECOMPILE_add_numeric_gt(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GT_LONG: {
        SPVM_PRECOMPILE_add_numeric_gt(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GT_FLOAT: {
        SPVM_PRECOMPILE_add_numeric_gt(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GT_DOUBLE: {
        SPVM_PRECOMPILE_add_numeric_gt(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GE_INT: {
        SPVM_PRECOMPILE_add_numeric_ge(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GE_LONG: {
        SPVM_PRECOMPILE_add_numeric_ge(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GE_FLOAT: {
        SPVM_PRECOMPILE_add_numeric_ge(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GE_DOUBLE: {
        SPVM_PRECOMPILE_add_numeric_ge(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_LT_INT: {
        SPVM_PRECOMPILE_add_numeric_lt(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_LT_LONG: {
        SPVM_PRECOMPILE_add_numeric_lt(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_LT_FLOAT: {
        SPVM_PRECOMPILE_add_numeric_lt(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_LT_DOUBLE: {
        SPVM_PRECOMPILE_add_numeric_lt(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_LE_INT: {
        SPVM_PRECOMPILE_add_numeric_le(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_LE_LONG: {
        SPVM_PRECOMPILE_add_numeric_le(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_LE_FLOAT: {
        SPVM_PRECOMPILE_add_numeric_le(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_LE_DOUBLE: {
        SPVM_PRECOMPILE_add_numeric_le(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_CMP_INT: {
        SPVM_PRECOMPILE_add_numeric_cmp(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_CMP_LONG: {
        SPVM_PRECOMPILE_add_numeric_cmp(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_CMP_FLOAT: {
        SPVM_PRECOMPILE_add_numeric_cmp(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_CMP_DOUBLE: {
        SPVM_PRECOMPILE_add_numeric_cmp(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_IS_UNDEF: {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, " == NULL;\n");
        break;
      }
      case SPVM_OPCODE_C_ID_IS_NOT_UNDEF: {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, " != NULL;\n");
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
        SPVM_STRING_BUFFER_add(string_buffer, "  object1 = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  object2 = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  if (object1 == NULL && object2 == NULL) {\n");
        SPVM_STRING_BUFFER_add(string_buffer, "      ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        switch (opcode_id) {
          case SPVM_OPCODE_C_ID_STRING_EQ: {
            SPVM_STRING_BUFFER_add(string_buffer, "1;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_NE: {
            SPVM_STRING_BUFFER_add(string_buffer, "0;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_GT: {
            SPVM_STRING_BUFFER_add(string_buffer, "0;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_GE: {
            SPVM_STRING_BUFFER_add(string_buffer, "1;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_LT: {
            SPVM_STRING_BUFFER_add(string_buffer, "0;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_LE: {
            SPVM_STRING_BUFFER_add(string_buffer, "1;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_CMP: {
            SPVM_STRING_BUFFER_add(string_buffer, "0;\n");
            break;
          }
        }
        SPVM_STRING_BUFFER_add(string_buffer, "  }\n"
                                              "  else if (object1 != NULL && object2 == NULL) {\n"
                                              "      ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        switch (opcode_id) {
          case SPVM_OPCODE_C_ID_STRING_EQ: {
            SPVM_STRING_BUFFER_add(string_buffer, "0;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_NE: {
            SPVM_STRING_BUFFER_add(string_buffer, "1;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_GT: {
            SPVM_STRING_BUFFER_add(string_buffer, "1;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_GE: {
            SPVM_STRING_BUFFER_add(string_buffer, "1;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_LT: {
            SPVM_STRING_BUFFER_add(string_buffer, "0;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_LE: {
            SPVM_STRING_BUFFER_add(string_buffer, "0;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_CMP: {
            SPVM_STRING_BUFFER_add(string_buffer, "1;\n");
            break;
          }
        }
        SPVM_STRING_BUFFER_add(string_buffer, "  }\n"
                                              "  else if (object1 == NULL && object2 != NULL) {\n"
                                              "    ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        switch (opcode_id) {
          case SPVM_OPCODE_C_ID_STRING_EQ: {
            SPVM_STRING_BUFFER_add(string_buffer, "0;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_NE: {
            SPVM_STRING_BUFFER_add(string_buffer, "1;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_GT: {
            SPVM_STRING_BUFFER_add(string_buffer, "0;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_GE: {
            SPVM_STRING_BUFFER_add(string_buffer, "0;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_LT: {
            SPVM_STRING_BUFFER_add(string_buffer, "1;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_LE: {
            SPVM_STRING_BUFFER_add(string_buffer, "1;\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_CMP: {
            SPVM_STRING_BUFFER_add(string_buffer, "-1;\n");
            break;
          }
        }
        SPVM_STRING_BUFFER_add(string_buffer, "  }\n"
                                              "  else {\n"
                                              "    length1 = *(int32_t*)((intptr_t)object1 + (intptr_t)env->object_length_offset);\n"
                                              "    length2 = *(int32_t*)((intptr_t)object2 + (intptr_t)env->object_length_offset);\n"
                                              "    bytes1 = (char*)env->get_chars(env, stack, object1);\n"
                                              "    bytes2 = (char*)env->get_chars(env, stack, object2);\n"
                                              "    short_string_length = length1 < length2 ? length1 : length2;\n"
                                              "    retval = memcmp(bytes1, bytes2, short_string_length);\n"
                                              "    if (retval) {\n"
                                              "      cmp = retval < 0 ? -1 : 1;\n"
                                              "    } else if (length1 == length2) {\n"
                                              "      cmp = 0;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      cmp = length1 < length2 ? -1 : 1;\n"
                                              "    }\n"
                                              "      ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        switch (opcode_id) {
          case SPVM_OPCODE_C_ID_STRING_EQ: {
            SPVM_STRING_BUFFER_add(string_buffer, "(cmp == 0);\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_NE: {
            SPVM_STRING_BUFFER_add(string_buffer, "(cmp != 0);\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_GT: {
            SPVM_STRING_BUFFER_add(string_buffer, "(cmp == 1);\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_GE: {
            SPVM_STRING_BUFFER_add(string_buffer, "(cmp >= 0);\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_LT: {
            SPVM_STRING_BUFFER_add(string_buffer, "(cmp == -1);\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_LE: {
            SPVM_STRING_BUFFER_add(string_buffer, "(cmp <= 0);\n");
            break;
          }
          case SPVM_OPCODE_C_ID_STRING_CMP: {
            SPVM_STRING_BUFFER_add(string_buffer, "cmp;\n");
            break;
          }
        }
        
        SPVM_STRING_BUFFER_add(string_buffer, "  }\n");

        break;
      }
      case SPVM_OPCODE_C_ID_NEW_OBJECT: {
        int32_t basic_type_id = opcode->operand1;
        int32_t basic_type_name_id = SPVM_API_RUNTIME_get_basic_type_name_id(runtime, basic_type_id);
        const char* basic_type_name = SPVM_API_RUNTIME_get_name(runtime, basic_type_name_id);

        SPVM_STRING_BUFFER_add(string_buffer, "    access_basic_type_id = env->get_basic_type_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "    if (access_basic_type_id < 0) {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, \"The basic type \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    if (!error) {\n"
                                              "      basic_type_id = access_basic_type_id;\n"
                                              "      object = env->new_object_raw(env, stack, basic_type_id);\n"
                                              "      if (object == NULL) {\n"
                                              "        exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "        env->set_exception(env, stack, exception);\n"
                                              "        error = 1;\n"
                                              "      }\n"
                                              "      else {\n"
                                              "        SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n"
                                              "      }\n"
                                              "    }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_OBJECT_ARRAY: {
        int32_t basic_type_id = opcode->operand1;
        int32_t basic_type_name_id = SPVM_API_RUNTIME_get_basic_type_name_id(runtime, basic_type_id);
        const char* basic_type_name = SPVM_API_RUNTIME_get_name(runtime, basic_type_name_id);

        SPVM_STRING_BUFFER_add(string_buffer, "    access_basic_type_id = env->get_basic_type_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n");
        SPVM_STRING_BUFFER_add(string_buffer, "    if (");
        SPVM_STRING_BUFFER_add(string_buffer, "access_basic_type_id");
        SPVM_STRING_BUFFER_add(string_buffer, " < 0) {\n");
        SPVM_STRING_BUFFER_add(string_buffer, "      exception = env->new_string_nolen_raw(env, stack, \"The basic type \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    if (!error) {\n"
                                              "      basic_type_id = "
                                              "access_basic_type_id"
                                              ";\n"
                                              "      length = *(int32_t*)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "      if (length >= 0) {\n"
                                              "        object = env->new_object_array_raw(env, stack, basic_type_id, length);\n"
                                              "        if (object == NULL) {\n"
                                              "          exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_NEW_ARRAY_FAILED]);\n"
                                              "          env->set_exception(env, stack, exception);\n"
                                              "          error = 1;\n"
                                              "        }\n"
                                              "        else {\n"
                                              "          SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, (void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n"
                                              "        }\n"
                                              "      }\n"
                                              "      else {\n"
                                              "        exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "        env->set_exception(env, stack, exception);\n"
                                              "        error = 1;\n"
                                              "      }\n"
                                              "    }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_MULDIM_ARRAY: {
        int32_t basic_type_id = opcode->operand1;
        int32_t basic_type_name_id = SPVM_API_RUNTIME_get_basic_type_name_id(runtime, basic_type_id);
        const char* basic_type_name = SPVM_API_RUNTIME_get_name(runtime, basic_type_name_id);
        int32_t element_dimension = opcode->operand3;
        
        SPVM_STRING_BUFFER_add(string_buffer, "    access_basic_type_id = env->get_basic_type_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "    if (access_basic_type_id < 0) {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, \"The basic type \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    if (!error) {\n"
                                              "      basic_type_id = "
                                              "access_basic_type_id"
                                              ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "      length = *(int32_t*)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "      if (length >= 0) {\n"
                                              "        object = env->new_muldim_array_raw(env, stack, basic_type_id, ");
        SPVM_STRING_BUFFER_add_int(string_buffer, element_dimension);
        SPVM_STRING_BUFFER_add(string_buffer, ", length);\n"
                                              "        if (object == NULL) {\n"
                                              "          exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_NEW_ARRAY_FAILED]);\n"
                                              "          env->set_exception(env, stack, exception);\n"
                                              "          error = 1;\n"
                                              "        }\n"
                                              "        else {\n"
                                              "          SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, (void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n"
                                              "        }\n"
                                              "      }\n"
                                              "      else {\n"
                                              "        exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "        env->set_exception(env, stack, exception);\n"
                                              "        error = 1;\n"
                                              "      }\n"
                                              "    }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_MULNUM_ARRAY: {
        int32_t basic_type_id = opcode->operand1;
        int32_t basic_type_name_id = SPVM_API_RUNTIME_get_basic_type_name_id(runtime, basic_type_id);
        const char* basic_type_name = SPVM_API_RUNTIME_get_name(runtime, basic_type_name_id);

        SPVM_STRING_BUFFER_add(string_buffer, "  {\n"
                                              "    access_basic_type_id = env->get_basic_type_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "    if (access_basic_type_id < 0) {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, \"The basic type \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    if (!error) {\n"
                                              "      basic_type_id = "
                                              "access_basic_type_id"
                                              ";\n"
                                              "      length = *(int32_t*)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "      if (length >= 0) {\n"
                                              "        object = env->new_mulnum_array_raw(env, stack, basic_type_id, length);\n"
                                              "        if (object == NULL) {\n"
                                              "          exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "          env->set_exception(env, stack, exception);\n"
                                              "          error = 1;\n"
                                              "        }\n"
                                              "        else {\n"
                                              "          SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, (void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n"
                                              "        }\n"
                                              "      }\n"
                                              "      else {\n"
                                              "        exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "        env->set_exception(env, stack, exception);\n"
                                              "        error = 1;\n"
                                              "      }\n"
                                              "    }\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_BYTE_ARRAY: {
        SPVM_STRING_BUFFER_add(string_buffer, "  {\n"
                                              "    length = *(int32_t*)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    if (length >= 0) {\n"
                                              "      object = env->new_byte_array_raw(env, stack, length);\n"
                                              "      if (object == NULL) {\n"
                                              "        exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "        env->set_exception(env, stack, exception);\n"
                                              "        error = 1;\n"
                                              "      }\n"
                                              "      else {\n"
                                              "        SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, (void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n"
                                              "      }\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_SHORT_ARRAY: {
         SPVM_STRING_BUFFER_add(string_buffer, "  length = *(int32_t*)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (length >= 0) {\n"
                                              "    object = env->new_short_array_raw(env, stack, length);\n"
                                              "    if (object == NULL) {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, (void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n"
                                              "    }\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_INT_ARRAY: {
         SPVM_STRING_BUFFER_add(string_buffer, "  length = *(int32_t*)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (length >= 0) {\n"
                                              "    object = env->new_int_array_raw(env, stack, length);\n"
                                              "    if (object == NULL) {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, (void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n"
                                              "    }\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_LONG_ARRAY: {
        SPVM_STRING_BUFFER_add(string_buffer, "  length = *(int32_t*)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (length >= 0) {\n"
                                              "    object = env->new_long_array_raw(env, stack, length);\n"
                                              "    if (object == NULL) {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, (void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n"
                                              "    }\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_FLOAT_ARRAY: {
        SPVM_STRING_BUFFER_add(string_buffer, "  length = *(int32_t*)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (length >= 0) {\n"
                                              "    object = env->new_float_array_raw(env, stack, length);\n"
                                              "    if (object == NULL) {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_NEW_ARRAY_FAILED]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, (void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n"
                                              "    }\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_DOUBLE_ARRAY: {
        SPVM_STRING_BUFFER_add(string_buffer, "  length = *(int32_t*)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (length >= 0) {\n"
                                              "    object = env->new_double_array_raw(env, stack, length);\n"
                                              "    if (object == NULL) {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, (void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n"
                                              "    }\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRRAY_LENGTH_SMALL]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_NEW_STRING: {
        int32_t constant_string_id = opcode->operand1;
        
        int32_t constant_string_length;
        const char* constant_string_value = SPVM_API_RUNTIME_get_constant_string_value(runtime, constant_string_id, &constant_string_length);

        SPVM_STRING_BUFFER_add(string_buffer,
          "  string = env->new_string_raw(env, stack, \""
        );
        for (int32_t i = 0; i < constant_string_length; i++) {
          SPVM_STRING_BUFFER_add_hex_char(string_buffer, constant_string_value[i]);
        }
        SPVM_STRING_BUFFER_add(string_buffer,
          "\", "
        );
        SPVM_STRING_BUFFER_add_int(string_buffer, constant_string_length);
        SPVM_STRING_BUFFER_add(string_buffer,
          ");\n"
          "  if (string == NULL) {\n"
          "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_NEW_STRING_FAILED]);\n"
          "    env->set_exception(env, stack, exception);\n"
          "    error = 1;\n"
          "  }\n"
          "  else {\n"
          "    env->make_read_only(env, stack, string);\n"
          "    SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &"
        );
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer,
          ", string);\n"
          "  }\n"
        );

        break;
      }
      case SPVM_OPCODE_C_ID_NEW_STRING_LEN: {
        SPVM_STRING_BUFFER_add(string_buffer, "    length = *(int32_t*)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    if (length >= 0) {\n"
                                              "      object = env->new_string_raw(env, stack, NULL, length);\n"
                                              "      if (object == NULL) {\n"
                                              "        exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_NEW_STRING_FAILED]);\n"
                                              "        env->set_exception(env, stack, exception);\n"
                                              "        error = 1;\n"
                                              "      }\n"
                                              "      else {\n"
                                              "        SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, (void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n"
                                              "      }\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_STRING_LENGTH_SMALL]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_IS_READ_ONLY: {
        SPVM_STRING_BUFFER_add(string_buffer, "  string = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  is_read_only = env->is_read_only(env, stack, string);\n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = is_read_only;\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MAKE_READ_ONLY: {
        SPVM_STRING_BUFFER_add(string_buffer, "  string = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  env->make_read_only(env, stack, string);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_BYTE: {
        SPVM_PRECOMPILE_add_array_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_SHORT: {
        SPVM_PRECOMPILE_add_array_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_INT: {
        SPVM_PRECOMPILE_add_array_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_LONG: {
        SPVM_PRECOMPILE_add_array_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_FLOAT: {
        SPVM_PRECOMPILE_add_array_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_DOUBLE: {
        SPVM_PRECOMPILE_add_array_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARRAY_ELEMENT_OBJECT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  array = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  index = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", SPVM_INLINE_API_GET_ARRAY_ELEMENT_OBJECT(env, stack, array, index, (int32_t*)&error, object_header_byte_size));\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_BYTE: {
        SPVM_PRECOMPILE_add_array_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_SHORT: {
        SPVM_PRECOMPILE_add_array_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_INT: {
        SPVM_PRECOMPILE_add_array_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_LONG: {
        SPVM_PRECOMPILE_add_array_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_FLOAT: {
        SPVM_PRECOMPILE_add_array_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_DOUBLE: {
        SPVM_PRECOMPILE_add_array_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_OBJECT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  array = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  index = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (__builtin_expect(array == NULL, 0)) { \n"
                                              "    env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_UNDEFINED]));\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else { \n"
                                              "    if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) { \n"
                                              "        env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_ACCESS_INDEX_OUT_OF_RANGE]));\n"
                                              "        error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      element_address = &((void**)((intptr_t)array + object_header_byte_size))[index];\n"
                                              "      SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, \n"
                                              "        element_address, "
                                              "      ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, "\n"
                                              "      );\n"
                                              "    }\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_OBJECT_CHECK_TYPE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  array = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  index = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (__builtin_expect(array == NULL, 0)) { \n"
                                              "    env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_UNDEFINED]));\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else { \n"
                                              "    if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) { \n"
                                              "        env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_ACCESS_INDEX_OUT_OF_RANGE]));\n"
                                              "        error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      element_address = &((void**)((intptr_t)array + object_header_byte_size))[index];\n"
                                              "      object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "      elem_isa = env->elem_isa(env, stack, array, object);\n"
                                              "      if (elem_isa) {\n"
                                              "        SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, element_address, object);\n"
                                              "      }\n"
                                              "      else {\n"
                                              "        exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ELEMENT_ASSIGN_NON_ASSIGNABLE_TYPE]);\n"
                                              "        env->set_exception(env, stack, exception);\n"
                                              "        error = 1;\n"
                                              "      }\n"
                                              "    }\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ARRAY_ELEMENT_UNDEF: {
        SPVM_STRING_BUFFER_add(string_buffer, "  array = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  index = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  if (__builtin_expect(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " == NULL, 0)) { \n"
                                              "    env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_UNDEFINED]));\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else { \n"
                                              "    if (__builtin_expect(");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, " < 0 || ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, "  >= *(int32_t*)((intptr_t)");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " + (intptr_t)env->object_length_offset), 0)) { \n"
                                              "      env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_ACCESS_INDEX_OUT_OF_RANGE]));\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      object_address = &((void**)((intptr_t)array + object_header_byte_size))[index];\n"
                                              "      SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, \n"
                                              "        object_address,\n"
                                              "        NULL"
                                              "      );\n"
                                              "    }\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_ARRAY_LENGTH: {
        SPVM_STRING_BUFFER_add(string_buffer, "  if (");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, " == NULL) {\n"
                                              "    env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_UNDEFINED]));\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = *(int32_t*)((intptr_t)");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, " + (intptr_t)env->object_length_offset);\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_BYTE: {
        int32_t field_id = opcode->operand2;
        SPVM_PRECOMPILE_add_get_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1, field_id);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_SHORT: {
        int32_t field_id = opcode->operand2;
        SPVM_PRECOMPILE_add_get_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1, field_id);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_INT: {
        int32_t field_id = opcode->operand2;
        SPVM_PRECOMPILE_add_get_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1, field_id);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_LONG: {
        int32_t field_id = opcode->operand2;
        SPVM_PRECOMPILE_add_get_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1, field_id);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_FLOAT: {
        int32_t field_id = opcode->operand2;
        SPVM_PRECOMPILE_add_get_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1, field_id);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_DOUBLE: {
        int32_t field_id = opcode->operand2;
        SPVM_PRECOMPILE_add_get_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1, field_id);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_FIELD_OBJECT: {
        int32_t field_id = opcode->operand2;
        
        int32_t field_class_id = SPVM_API_RUNTIME_get_field_class_id(runtime, field_id);
        int32_t field_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, field_class_id);
        const char* field_class_name = SPVM_API_RUNTIME_get_name(runtime, field_class_name_id);
        int32_t field_name_id = SPVM_API_RUNTIME_get_field_name_id(runtime, field_id);
        const char* field_name = SPVM_API_RUNTIME_get_name(runtime, field_name_id);

        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  access_field_id = env->get_field_id(env, object, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"

                                              "  if (access_field_id < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The field \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    int32_t access_field_offset"
                                              " = env->get_field_offset(env, access_field_id);\n"
                                              "    if (__builtin_expect(object == NULL, 0)) {\n"
                                              "      env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_FIELD_ACCESS_INVOCANT_UNDEFINED]));\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      get_field_object = *(void**)((intptr_t)object + object_header_byte_size + "
                                              "access_field_offset"
                                              ");\n"
                                              "      SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", get_field_object);\n"
                                              "    }\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_BYTE: {
        int32_t field_id = opcode->operand1;
        SPVM_PRECOMPILE_add_set_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, field_id, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_SHORT: {
        int32_t field_id = opcode->operand1;
        SPVM_PRECOMPILE_add_set_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, field_id, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_INT: {
        int32_t field_id = opcode->operand1;
        SPVM_PRECOMPILE_add_set_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, field_id, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_LONG: {
        int32_t field_id = opcode->operand1;
        SPVM_PRECOMPILE_add_set_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, field_id, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_FLOAT: {
        int32_t field_id = opcode->operand1;
        SPVM_PRECOMPILE_add_set_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, field_id, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_DOUBLE: {
        int32_t field_id = opcode->operand1;
        SPVM_PRECOMPILE_add_set_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, field_id, opcode->operand2);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_OBJECT:
      {
        int32_t field_id = opcode->operand1;
        
        int32_t field_class_id = SPVM_API_RUNTIME_get_field_class_id(runtime, field_id);
        int32_t field_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, field_class_id);
        const char* field_class_name = SPVM_API_RUNTIME_get_name(runtime, field_class_name_id);
        int32_t field_name_id = SPVM_API_RUNTIME_get_field_name_id(runtime, field_id);
        const char* field_name = SPVM_API_RUNTIME_get_name(runtime, field_name_id);

        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";");
        SPVM_STRING_BUFFER_add(string_buffer, "  access_field_id = env->get_field_id(env, object, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "  if (access_field_id < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The field \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    int32_t access_field_offset"
                                              " = env->get_field_offset(env, "
                                              "access_field_id"
                                              ");\n"
                                              "    if (__builtin_expect(object == NULL, 0)) {\n"
                                              "      env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_FIELD_ACCESS_INVOCANT_UNDEFINED]));\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      get_field_object_address = (void**)((intptr_t)object + object_header_byte_size + "
                                              "access_field_offset"
                                              ");\n"
                                              "      SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, "
                                              "get_field_object_address,");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand2);
        SPVM_STRING_BUFFER_add(string_buffer, "  );\n"
                                              "    }\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_FIELD_UNDEF:
      {
        int32_t field_id = opcode->operand1;
        
        int32_t field_class_id = SPVM_API_RUNTIME_get_field_class_id(runtime, field_id);
        int32_t field_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, field_class_id);
        const char* field_class_name = SPVM_API_RUNTIME_get_name(runtime, field_class_name_id);
        int32_t field_name_id = SPVM_API_RUNTIME_get_field_name_id(runtime, field_id);
        const char* field_name = SPVM_API_RUNTIME_get_name(runtime, field_name_id);

        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";");
        SPVM_STRING_BUFFER_add(string_buffer, "  access_field_id = env->get_field_id(env, object, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "  if (access_field_id < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The field \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    int32_t access_field_offset"
                                              " = env->get_field_offset(env, "
                                              "access_field_id"
                                              ");\n"
                                              "    if (__builtin_expect(object == NULL, 0)) {\n"
                                              "    env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_FIELD_ACCESS_INVOCANT_UNDEFINED]));\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      get_field_object_address = (void**)((intptr_t)object + object_header_byte_size + "
                                              "access_field_offset"
                                              ");\n"
                                              "      SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, get_field_object_address, NULL);"
                                              "    }\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_BYTE:
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_SHORT:
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_INT:
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_LONG:
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_FLOAT:
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_DOUBLE:
      {
        int32_t class_var_id = opcode->operand1;
        
        int32_t class_var_class_id = SPVM_API_RUNTIME_get_class_var_class_id(runtime, class_var_id);
        int32_t class_var_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, class_var_class_id);
        const char* class_var_class_name = SPVM_API_RUNTIME_get_name(runtime, class_var_class_name_id);
        int32_t class_var_name_id = SPVM_API_RUNTIME_get_class_var_name_id(runtime, class_var_id);
        const char* class_var_name = SPVM_API_RUNTIME_get_name(runtime, class_var_name_id);

        int32_t class_var_access_ctype_id;
        switch (opcode_id) {
          case SPVM_OPCODE_C_ID_GET_CLASS_VAR_BYTE:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_BYTE;
            break;
          case SPVM_OPCODE_C_ID_GET_CLASS_VAR_SHORT:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_SHORT;
            break;
          case SPVM_OPCODE_C_ID_GET_CLASS_VAR_INT:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_INT;
            break;
          case SPVM_OPCODE_C_ID_GET_CLASS_VAR_LONG:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_LONG;
            break;
          case SPVM_OPCODE_C_ID_GET_CLASS_VAR_FLOAT:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT;
            break;
          case SPVM_OPCODE_C_ID_GET_CLASS_VAR_DOUBLE:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE;
            break;
          default:
            assert(0);
        }
        
        SPVM_STRING_BUFFER_add(string_buffer, "  access_class_var_id = env->get_class_var_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\", \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "  if (access_class_var_id < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The class variable \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" in the class \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, class_var_access_ctype_id, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = *(");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, class_var_access_ctype_id));
        SPVM_STRING_BUFFER_add(string_buffer, "*)&((SPVM_VALUE*)env->class_vars_heap)[access_class_var_id];\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_CLASS_VAR_OBJECT: {
        int32_t class_var_id = opcode->operand1;
        
        int32_t class_var_class_id = SPVM_API_RUNTIME_get_class_var_class_id(runtime, class_var_id);
        int32_t class_var_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, class_var_class_id);
        const char* class_var_class_name = SPVM_API_RUNTIME_get_name(runtime, class_var_class_name_id);
        int32_t class_var_name_id = SPVM_API_RUNTIME_get_class_var_name_id(runtime, class_var_id);
        const char* class_var_name = SPVM_API_RUNTIME_get_name(runtime, class_var_name_id);

        SPVM_STRING_BUFFER_add(string_buffer, "  access_class_var_id = env->get_class_var_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\", \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "  if (access_class_var_id < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The class variable \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" in the class \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "      SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", *(void**)&((SPVM_VALUE*)env->class_vars_heap)[access_class_var_id]);\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_BYTE:
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_SHORT:
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_INT:
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_LONG:
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_FLOAT:
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_DOUBLE:
      {
        int32_t class_var_id = opcode->operand0;
        
        int32_t class_var_class_id = SPVM_API_RUNTIME_get_class_var_class_id(runtime, class_var_id);
        int32_t class_var_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, class_var_class_id);
        const char* class_var_class_name = SPVM_API_RUNTIME_get_name(runtime, class_var_class_name_id);
        int32_t class_var_name_id = SPVM_API_RUNTIME_get_class_var_name_id(runtime, class_var_id);
        const char* class_var_name = SPVM_API_RUNTIME_get_name(runtime, class_var_name_id);

        int32_t class_var_access_ctype_id;
        switch (opcode_id) {
          case SPVM_OPCODE_C_ID_SET_CLASS_VAR_BYTE:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_BYTE;
            break;
          case SPVM_OPCODE_C_ID_SET_CLASS_VAR_SHORT:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_SHORT;
            break;
          case SPVM_OPCODE_C_ID_SET_CLASS_VAR_INT:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_INT;
            break;
          case SPVM_OPCODE_C_ID_SET_CLASS_VAR_LONG:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_LONG;
            break;
          case SPVM_OPCODE_C_ID_SET_CLASS_VAR_FLOAT:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT;
            break;
          case SPVM_OPCODE_C_ID_SET_CLASS_VAR_DOUBLE:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE;
            break;
          default:
            class_var_access_ctype_id = SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT;
        }
        
        SPVM_STRING_BUFFER_add(string_buffer, "  access_class_var_id = env->get_class_var_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\", \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "  if (access_class_var_id < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The class variable \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" in the class \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    *(");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, class_var_access_ctype_id));
        SPVM_STRING_BUFFER_add(string_buffer, "*)&((SPVM_VALUE*)env->class_vars_heap)[access_class_var_id] = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, class_var_access_ctype_id, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_OBJECT: {
        int32_t class_var_id = opcode->operand0;
        
        int32_t class_var_class_id = SPVM_API_RUNTIME_get_class_var_class_id(runtime, class_var_id);
        int32_t class_var_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, class_var_class_id);
        const char* class_var_class_name = SPVM_API_RUNTIME_get_name(runtime, class_var_class_name_id);
        int32_t class_var_name_id = SPVM_API_RUNTIME_get_class_var_name_id(runtime, class_var_id);
        const char* class_var_name = SPVM_API_RUNTIME_get_name(runtime, class_var_name_id);

        SPVM_STRING_BUFFER_add(string_buffer, "  access_class_var_id = env->get_class_var_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\", \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "  if ("
                                              "access_class_var_id"
                                              " < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The class variable \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" in the class \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, (void**)&((SPVM_VALUE*)env->class_vars_heap)["
                                              "access_class_var_id"
                                              "],\n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ");"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_CLASS_VAR_UNDEF: {
        int32_t class_var_id = opcode->operand0;
        
        int32_t class_var_class_id = SPVM_API_RUNTIME_get_class_var_class_id(runtime, class_var_id);
        int32_t class_var_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, class_var_class_id);
        const char* class_var_class_name = SPVM_API_RUNTIME_get_name(runtime, class_var_class_name_id);
        int32_t class_var_name_id = SPVM_API_RUNTIME_get_class_var_name_id(runtime, class_var_id);
        const char* class_var_name = SPVM_API_RUNTIME_get_name(runtime, class_var_name_id);
        
        SPVM_STRING_BUFFER_add(string_buffer, "  access_class_var_id = env->get_class_var_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\", \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "  if (access_class_var_id < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The class variable \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" in the class \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_var_class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, (void**)&((SPVM_VALUE*)env->class_vars_heap)["
                                              "access_class_var_id"
                                              "], NULL);\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_EXCEPTION_VAR: {
        SPVM_STRING_BUFFER_add(string_buffer, "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", env->get_exception(env, stack));\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SET_EXCEPTION_VAR: {
        SPVM_STRING_BUFFER_add(string_buffer, "  env->set_exception(env, stack, ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SET_EXCEPTION_VAR_UNDEF: {
        SPVM_STRING_BUFFER_add(string_buffer, "  env->set_exception(env, stack, NULL);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_IF_EXCEPTION_CATCH: {
        int32_t line = opcode->operand2;
        
        SPVM_STRING_BUFFER_add(string_buffer, "  if (__builtin_expect(error, 0)) {\n"
                                              "    eval_error = error;\n"
                                              "    error = 0;\n"
                                              "    env->set_exception(env, stack, env->new_stack_trace_raw(env, stack, env->get_exception(env, stack), cur_method_id, line = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, line);
        SPVM_STRING_BUFFER_add(string_buffer,  "));\n"
                                              "    goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer,  opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_IF_EXCEPTION_RETURN: {
        int32_t line = opcode->operand2;
        
        SPVM_STRING_BUFFER_add(string_buffer, "  if (__builtin_expect(error, 0)) {\n"
                                              "    env->set_exception(env, stack, env->new_stack_trace_raw(env, stack, env->get_exception(env, stack), cur_method_id, line = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, line);
        SPVM_STRING_BUFFER_add(string_buffer,  "));\n"
                                              "    goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_ISA:
      {
        int32_t dist_basic_type = opcode->operand2;
        int32_t dist_type_dimension = opcode->operand3;
        int32_t basic_type_name_id = SPVM_API_RUNTIME_get_basic_type_name_id(runtime, dist_basic_type);
        const char* basic_type_name = SPVM_API_RUNTIME_get_name(runtime, basic_type_name_id);
        int32_t dimension = dist_type_dimension;
        
        SPVM_STRING_BUFFER_add(string_buffer, "  dist_basic_type_id = env->get_basic_type_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  if (dist_basic_type_id < 0) {\n");
        SPVM_STRING_BUFFER_add(string_buffer, "    exception = env->new_string_nolen_raw(env, stack, \"The basic type \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    int32_t dist_basic_type = "
                                              "dist_basic_type_id"
                                              ";\n"
                                              "    dist_type_dimension = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, dimension);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    if (object) {\n"
                                              "      object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);\n"
                                              "      int32_t object_type_dimension = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);\n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = env->isa(env, stack, object, dist_basic_type_id, dist_type_dimension);\n"
                                              "    }\n"
                                              "    else {\n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = 0;\n"
                                              "    }\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_IS_TYPE:
      {
        int32_t dist_basic_type = opcode->operand2;
        int32_t dist_type_dimension = opcode->operand3;
        int32_t basic_type_name_id = SPVM_API_RUNTIME_get_basic_type_name_id(runtime, dist_basic_type);
        const char* basic_type_name = SPVM_API_RUNTIME_get_name(runtime, basic_type_name_id);
        int32_t dimension = dist_type_dimension;
        
        SPVM_STRING_BUFFER_add(string_buffer, "  dist_basic_type_id = env->get_basic_type_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  if (dist_basic_type_id < 0) {\n");
        SPVM_STRING_BUFFER_add(string_buffer, "    exception = env->new_string_nolen_raw(env, stack, \"The basic type \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)basic_type_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    int32_t dist_basic_type = "
                                              "dist_basic_type_id"
                                              ";\n"
                                              "    dist_type_dimension = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, dimension);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    if (object) {\n"
                                              "      object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);\n"
                                              "      int32_t object_type_dimension = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);\n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = env->is_type(env, stack, object, dist_basic_type_id, dist_type_dimension);\n"
                                              "    }\n"
                                              "    else {\n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = 0;\n"
                                              "    }\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_HAS_IMPL: {
        int32_t implement_method_id = opcode->operand1;
        int32_t implement_method_name_id = SPVM_API_RUNTIME_get_method_name_id(runtime, implement_method_id);
        const char* implement_method_name = SPVM_API_RUNTIME_get_name(runtime, implement_method_name_id);

        int32_t interface_basic_type_id = opcode->operand2;
        int32_t interface_basic_type_class_id = SPVM_API_RUNTIME_get_basic_type_class_id(runtime, interface_basic_type_id);

        int32_t interface_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, interface_basic_type_class_id);
        const char* interface_name = SPVM_API_RUNTIME_get_name(runtime, interface_name_id);
        
        int32_t interface_method_id = SPVM_API_RUNTIME_get_method_id_by_name(runtime, interface_name, implement_method_name);

        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    call_method_id = env->get_instance_method_id(env, object, \"");
        SPVM_STRING_BUFFER_add(string_buffer, implement_method_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "    ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = call_method_id >= 0;\n");

        break;
      }
      case SPVM_OPCODE_C_ID_PRINT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (object) {\n"
                                              "    bytes = (char*)env->get_chars(env, stack, object);\n"
                                              "    string_length = env->length(env, stack, object);\n"
                                              "    if (string_length > 0) {\n"
                                              "      (void)fwrite(bytes, 1, string_length, stdout);\n"
                                              "    }\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_SAY: {
        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (object) {\n"
                                              "    bytes = (char*)env->get_chars(env, stack, object);\n"
                                              "    string_length = env->length(env, stack, object);\n"
                                              "    if (string_length > 0) {\n"
                                              "      (void)fwrite(bytes, 1, string_length, stdout);\n"
                                              "    }\n"
                                              "  }\n"
                                              "  fprintf(stdout, \"\\n\");\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_WARN: {
        int32_t line = opcode->operand1;
        
        int32_t module_rel_file_id = SPVM_API_RUNTIME_get_class_module_rel_file_id(runtime, class_id);
        int32_t module_dir_id = SPVM_API_RUNTIME_get_class_module_dir_id(runtime, class_id);
        const char* module_rel_file = SPVM_API_RUNTIME_get_constant_string_value(runtime, module_rel_file_id, NULL);
        const char* module_dir = NULL;
        const char* module_dir_sep;
        if (module_dir_id >= 0) {
          module_dir_sep = "/";
          module_dir = SPVM_API_RUNTIME_get_constant_string_value(runtime, module_dir_id, NULL);
        }
        else {
          module_dir_sep = "";
          module_dir = "";
        }
        
        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  empty_or_undef = 0;\n"
                                              "  if (object) {\n"
                                              "    bytes = (char*)env->get_chars(env, stack, object);\n"
                                              "    string_length = env->length(env, stack, object);\n"
                                              "    if (string_length > 0) {\n"
                                              "      (void)fwrite(bytes, 1, string_length, stderr);\n"
                                              "      if (bytes[string_length - 1] != '\\n') {\n"
                                              "        fprintf(stderr, \" at %s%s%s line %d\\n\", \"");
        SPVM_STRING_BUFFER_add(string_buffer, module_dir);
        SPVM_STRING_BUFFER_add(string_buffer, "\", \"");
        SPVM_STRING_BUFFER_add(string_buffer, module_dir_sep);
        SPVM_STRING_BUFFER_add(string_buffer, "\", \"");
        SPVM_STRING_BUFFER_add(string_buffer, module_rel_file);
        SPVM_STRING_BUFFER_add(string_buffer, "\", ");
        SPVM_STRING_BUFFER_add_int(string_buffer, line);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n"
                                              "      }\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      empty_or_undef = 1;\n"
                                              "    }\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    empty_or_undef = 1;\n"
                                              "  }\n"
                                              "  if (empty_or_undef) {\n"
                                              "    fprintf(stderr, \"Warning: something's wrong at %s%s%s line %d\\n\", \"");
        SPVM_STRING_BUFFER_add(string_buffer, module_dir);
        SPVM_STRING_BUFFER_add(string_buffer, "\", \"");
        SPVM_STRING_BUFFER_add(string_buffer, module_dir_sep);
        SPVM_STRING_BUFFER_add(string_buffer, "\", \"");
        SPVM_STRING_BUFFER_add(string_buffer, module_rel_file);
        SPVM_STRING_BUFFER_add(string_buffer, "\", ");
        SPVM_STRING_BUFFER_add_int(string_buffer, line);
        SPVM_STRING_BUFFER_add(string_buffer, ");\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ERROR_CODE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = error_code;\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ERROR_CODE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  tmp_error_code = ");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (tmp_error_code < 1) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ERROR_CODE_TOO_SMALL]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    error_code = tmp_error_code;");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = error_code;\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_CLEAR_EVAL_ERROR: {
        SPVM_STRING_BUFFER_add(string_buffer, "  eval_error = 0;\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_EVAL_ERROR: {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = eval_error;\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_CLASS_ID: {
        int32_t class_id = opcode->operand1;
        int32_t class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, class_id);
        const char* class_name = SPVM_API_RUNTIME_get_name(runtime, class_name_id);

        SPVM_STRING_BUFFER_add(string_buffer, "  access_class_id = env->get_class_id(env, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "  if (access_class_id < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The class \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = access_class_id;\n"
                                              "  }\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_SET_ERROR: {
        SPVM_STRING_BUFFER_add(string_buffer, "  error = error_code;\n");
        break;
      }
      case SPVM_OPCODE_C_ID_REFOP: {
        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (object == NULL) {\n"
                                              "    ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = NULL;\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    type_name = env->get_type_name_raw(env, stack, object);\n"
                                              "    SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", type_name);\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_DUMP: {
        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  dump = env->dump_raw(env, stack, object);\n"
                                              "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", dump);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_COPY: {
          SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                                "  if (object) {\n"
                                                "    if (!(env->is_string(env, stack, object) || env->is_numeric_array(env, stack, object) || env->is_mulnum_array(env, stack, object))) {\n"
                                                "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_COPY_OPERAND_INVALID]);\n"
                                                "      env->set_exception(env, stack, exception);\n"
                                                "      error = 1;\n"
                                                "    }\n"
                                                "    else {\n"
                                                "      new_object_raw = env->copy_raw(env, stack, object);\n"
                                                "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
          SPVM_STRING_BUFFER_add(string_buffer, ", new_object_raw);\n"
                                                "    }\n"
                                                "  }\n"
                                                "  else {\n"
                                                "    SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
          SPVM_STRING_BUFFER_add(string_buffer, ", NULL);\n"
                                                "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_REF_BYTE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  *(void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = &byte_vars[");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, "];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_REF_SHORT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  *(void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = &short_vars[");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, "];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_REF_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  *(void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = &int_vars[");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, "];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_REF_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  *(void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = &long_vars[");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, "];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_REF_FLOAT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  *(void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = &float_vars[");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, "];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_REF_DOUBLE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  *(void**)&");
        SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = &double_vars[");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, "];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_DEREF_BYTE: {
        SPVM_PRECOMPILE_add_get_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_DEREF_SHORT: {
        SPVM_PRECOMPILE_add_get_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_DEREF_INT: {
        SPVM_PRECOMPILE_add_get_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_DEREF_LONG: {
        SPVM_PRECOMPILE_add_get_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_DEREF_FLOAT: {
        SPVM_PRECOMPILE_add_get_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_DEREF_DOUBLE: {
        SPVM_PRECOMPILE_add_get_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DEREF_BYTE: {
        SPVM_PRECOMPILE_add_set_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DEREF_SHORT: {
        SPVM_PRECOMPILE_add_set_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DEREF_INT: {
        SPVM_PRECOMPILE_add_set_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DEREF_LONG: {
        SPVM_PRECOMPILE_add_set_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DEREF_FLOAT: {
        SPVM_PRECOMPILE_add_set_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_DEREF_DOUBLE: {
        SPVM_PRECOMPILE_add_set_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_BYTE: {
        int32_t field_index = opcode->operand2;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand1, field_index);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_SHORT: {
        int32_t field_index = opcode->operand2;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand1, field_index);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_INT: {
        int32_t field_index = opcode->operand2;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1, field_index);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_LONG: {
        int32_t field_index = opcode->operand2;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1, field_index);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_FLOAT: {
        int32_t field_index = opcode->operand2;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1, field_index);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DOUBLE: {
        int32_t field_index = opcode->operand2;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1, field_index);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_BYTE: {
        int32_t field_index = opcode->operand2;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, field_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_SHORT: {
        int32_t field_index = opcode->operand2;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, field_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_INT: {
        int32_t field_index = opcode->operand2;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, field_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_LONG: {
        int32_t field_index = opcode->operand2;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, field_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_FLOAT: {
        int32_t field_index = opcode->operand2;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, field_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DOUBLE: {
        int32_t field_index = opcode->operand2;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, field_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_MULNUM_BYTE: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand1, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_MULNUM_SHORT: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand1, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_MULNUM_INT: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_MULNUM_LONG: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_MULNUM_FLOAT: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        break;
      }
      case SPVM_OPCODE_C_ID_MOVE_MULNUM_DOUBLE: {
        int32_t fields_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_BYTE: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_array_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1, opcode->operand2, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_SHORT: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_array_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1, opcode->operand2, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_INT: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_array_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1, opcode->operand2, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_LONG: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_array_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1, opcode->operand2, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FLOAT: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_array_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1, opcode->operand2, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_DOUBLE: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_array_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1, opcode->operand2, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_BYTE: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_array_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1, opcode->operand2, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_SHORT: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_array_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1, opcode->operand2, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_INT: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_array_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1, opcode->operand2, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_LONG: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_array_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1, opcode->operand2, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FLOAT: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_array_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1, opcode->operand2, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_DOUBLE: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_array_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1, opcode->operand2, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FIELD_BYTE: {
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        SPVM_PRECOMPILE_add_mulnum_array_field_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1, opcode->operand2, fields_length, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FIELD_SHORT: {
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        SPVM_PRECOMPILE_add_mulnum_array_field_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1, opcode->operand2, fields_length, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FIELD_INT: {
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        SPVM_PRECOMPILE_add_mulnum_array_field_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1, opcode->operand2, fields_length, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FIELD_LONG: {
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        SPVM_PRECOMPILE_add_mulnum_array_field_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1, opcode->operand2, fields_length, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FIELD_FLOAT: {
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        SPVM_PRECOMPILE_add_mulnum_array_field_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1, opcode->operand2, fields_length, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_ARRAY_FIELD_DOUBLE: {
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        SPVM_PRECOMPILE_add_mulnum_array_field_get(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1, opcode->operand2, fields_length, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FIELD_BYTE: {
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        SPVM_PRECOMPILE_add_mulnum_array_field_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1, opcode->operand2, fields_length, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FIELD_SHORT: {
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        SPVM_PRECOMPILE_add_mulnum_array_field_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1, opcode->operand2, fields_length, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FIELD_INT: {
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        SPVM_PRECOMPILE_add_mulnum_array_field_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1, opcode->operand2, fields_length, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FIELD_LONG: {
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        SPVM_PRECOMPILE_add_mulnum_array_field_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1, opcode->operand2, fields_length, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FIELD_FLOAT: {
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        SPVM_PRECOMPILE_add_mulnum_array_field_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1, opcode->operand2, fields_length, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_ARRAY_FIELD_DOUBLE: {
        int32_t fields_length = (opcode->operand3 & 0xFF) + 1;
        int32_t field_index = opcode->operand3 >> 8;
        SPVM_PRECOMPILE_add_mulnum_array_field_set(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1, opcode->operand2, fields_length, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_DEREF_MULNUM_BYTE: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_DEREF_MULNUM_SHORT: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_DEREF_MULNUM_INT: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_DEREF_MULNUM_LONG: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_DEREF_MULNUM_FLOAT: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_DEREF_MULNUM_DOUBLE: {
        int32_t fields_length = opcode->operand3;
        SPVM_PRECOMPILE_add_mulnum_deref(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1, fields_length);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DEREF_BYTE: {
        int32_t field_index = opcode->operand2;
        SPVM_PRECOMPILE_add_mulnum_deref_get_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DEREF_SHORT: {
        int32_t field_index = opcode->operand2;
        SPVM_PRECOMPILE_add_mulnum_deref_get_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DEREF_INT: {
        int32_t field_index = opcode->operand2;
        SPVM_PRECOMPILE_add_mulnum_deref_get_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DEREF_LONG: {
        int32_t field_index = opcode->operand2;
        SPVM_PRECOMPILE_add_mulnum_deref_get_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DEREF_FLOAT: {
        int32_t field_index = opcode->operand2;
        SPVM_PRECOMPILE_add_mulnum_deref_get_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_GET_MULNUM_FIELD_DEREF_DOUBLE: {
        int32_t field_index = opcode->operand2;
        SPVM_PRECOMPILE_add_mulnum_deref_get_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DEREF_BYTE: {
        int32_t field_index = opcode->operand2;
        SPVM_PRECOMPILE_add_mulnum_deref_set_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DEREF_SHORT: {
        int32_t field_index = opcode->operand2;
        SPVM_PRECOMPILE_add_mulnum_deref_set_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DEREF_INT: {
        int32_t field_index = opcode->operand2;
        SPVM_PRECOMPILE_add_mulnum_deref_set_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DEREF_LONG: {
        int32_t field_index = opcode->operand2;
        SPVM_PRECOMPILE_add_mulnum_deref_set_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DEREF_FLOAT: {
        int32_t field_index = opcode->operand2;
        SPVM_PRECOMPILE_add_mulnum_deref_set_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_SET_MULNUM_FIELD_DEREF_DOUBLE: {
        int32_t field_index = opcode->operand2;
        SPVM_PRECOMPILE_add_mulnum_deref_set_field(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1, field_index);
        break;
      }
      case SPVM_OPCODE_C_ID_WEAKEN_FIELD: {
        int32_t field_id = opcode->operand1;
        
        int32_t field_class_id = SPVM_API_RUNTIME_get_field_class_id(runtime, field_id);
        int32_t field_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, field_class_id);
        const char* field_class_name = SPVM_API_RUNTIME_get_name(runtime, field_class_name_id);
        int32_t field_name_id = SPVM_API_RUNTIME_get_field_name_id(runtime, field_id);
        const char* field_name = SPVM_API_RUNTIME_get_name(runtime, field_name_id);

        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  access_field_id = env->get_field_id(env, object, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "  if (access_field_id < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The field \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    int32_t access_field_offset = env->get_field_offset(env, access_field_id);\n"
                                              "    if (object == NULL) {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_FIELD_ACCESS_INVOCANT_UNDEFINED]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      get_field_object_address = (void**)((intptr_t)object + object_header_byte_size + access_field_offset);\n"
                                              "      int32_t status = env->weaken(env, stack, get_field_object_address);\n"
                                              "      if (status != 0) {\n"
                                              "        exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_WEAKEN_BACK_REFERENCE_ALLOCATION_FAILED]);\n"
                                              "        env->set_exception(env, stack, exception);\n"
                                              "        error = 1;\n"
                                              "      }\n"
                                              "    }\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_UNWEAKEN_FIELD: {
        int32_t field_id = opcode->operand1;
        
        int32_t field_class_id = SPVM_API_RUNTIME_get_field_class_id(runtime, field_id);
        int32_t field_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, field_class_id);
        const char* field_class_name = SPVM_API_RUNTIME_get_name(runtime, field_class_name_id);
        int32_t field_name_id = SPVM_API_RUNTIME_get_field_name_id(runtime, field_id);
        const char* field_name = SPVM_API_RUNTIME_get_name(runtime, field_name_id);

        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  access_field_id = env->get_field_id(env, object, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "    if (access_field_id < 0) {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, \"The field \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    int32_t access_field_offset"
                                              " = env->get_field_offset(env, "
                                              "access_field_id"
                                              ");\n"
                                              "    if (object == NULL) {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_FIELD_ACCESS_INVOCANT_UNDEFINED]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      get_field_object_address = (void**)((intptr_t)object + object_header_byte_size + access_field_offset);\n"
                                              "      env->unweaken(env, stack, get_field_object_address);\n"
                                              "    }\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_ISWEAK_FIELD: {
        int32_t field_id = opcode->operand2;
        
        int32_t field_class_id = SPVM_API_RUNTIME_get_field_class_id(runtime, field_id);
        int32_t field_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, field_class_id);
        const char* field_class_name = SPVM_API_RUNTIME_get_name(runtime, field_class_name_id);
        int32_t field_name_id = SPVM_API_RUNTIME_get_field_name_id(runtime, field_id);
        const char* field_name = SPVM_API_RUNTIME_get_name(runtime, field_name_id);

        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    access_field_id = env->get_field_id(env, object, \"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                              "  if (access_field_id < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The field \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  if (!error) {\n"
                                              "    int32_t access_field_offset = env->get_field_offset(env, access_field_id);\n"
                                              "    if (object == NULL) {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_FIELD_ACCESS_INVOCANT_UNDEFINED]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      get_field_object_address = (void**)((intptr_t)object + object_header_byte_size + "
                                              "access_field_offset"
                                              ");\n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = env->isweak(env, stack, get_field_object_address);\n"
                                              "    }\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_REFCNT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  if (object == NULL) {\n");
        SPVM_STRING_BUFFER_add(string_buffer, "    ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = 0;\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, "  = env->get_ref_count(env, stack, object);\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_BYTE_TO_SHORT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_BYTE_TO_INT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_BYTE_TO_LONG: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_BYTE_TO_FLOAT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_BYTE_TO_DOUBLE: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_SHORT_TO_BYTE: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_SHORT_TO_INT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_SHORT_TO_LONG: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_SHORT_TO_FLOAT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_SHORT_TO_DOUBLE: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_INT_TO_BYTE: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_INT_TO_SHORT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_INT_TO_LONG: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_INT_TO_FLOAT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_INT_TO_DOUBLE: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_LONG_TO_BYTE: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_LONG_TO_SHORT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_LONG_TO_INT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_LONG_TO_FLOAT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_LONG_TO_DOUBLE: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_FLOAT_TO_BYTE: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_FLOAT_TO_SHORT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_FLOAT_TO_INT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_FLOAT_TO_LONG: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_FLOAT_TO_DOUBLE: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_DOUBLE_TO_BYTE: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_DOUBLE_TO_SHORT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_DOUBLE_TO_INT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_DOUBLE_TO_LONG: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_DOUBLE_TO_FLOAT: {
        SPVM_PRECOMPILE_add_convert(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, opcode->operand1);
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_BYTE_TO_STRING:
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_SHORT_TO_STRING:
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_INT_TO_STRING:
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_LONG_TO_STRING:
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_FLOAT_TO_STRING:
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_DOUBLE_TO_STRING:
      {
        switch (opcode_id) {
          case SPVM_OPCODE_C_ID_TYPE_CONVERSION_BYTE_TO_STRING:
            SPVM_STRING_BUFFER_add(string_buffer, "  sprintf(convert_string_buffer, \"%\" PRId8, ");
            SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand1);
            SPVM_STRING_BUFFER_add(string_buffer, ");\n");
            break;
          case SPVM_OPCODE_C_ID_TYPE_CONVERSION_SHORT_TO_STRING:
            SPVM_STRING_BUFFER_add(string_buffer, "  sprintf(convert_string_buffer, \"%\" PRId16, ");
            SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand1);
            SPVM_STRING_BUFFER_add(string_buffer, ");\n");
            break;
          case SPVM_OPCODE_C_ID_TYPE_CONVERSION_INT_TO_STRING:
            SPVM_STRING_BUFFER_add(string_buffer, "  sprintf(convert_string_buffer, \"%\" PRId32, ");
            SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
            SPVM_STRING_BUFFER_add(string_buffer, ");\n");
            break;
          case SPVM_OPCODE_C_ID_TYPE_CONVERSION_LONG_TO_STRING:
            SPVM_STRING_BUFFER_add(string_buffer, "  sprintf(convert_string_buffer, \"%\" PRId64, ");
            SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
            SPVM_STRING_BUFFER_add(string_buffer, ");\n");
            break;
          case SPVM_OPCODE_C_ID_TYPE_CONVERSION_FLOAT_TO_STRING:
            SPVM_STRING_BUFFER_add(string_buffer, "  sprintf(convert_string_buffer, \"%g\", ");
            SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1);
            SPVM_STRING_BUFFER_add(string_buffer, ");\n");
            break;
          case SPVM_OPCODE_C_ID_TYPE_CONVERSION_DOUBLE_TO_STRING:
            SPVM_STRING_BUFFER_add(string_buffer, "  sprintf(convert_string_buffer, \"%g\", ");
            SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1);
            SPVM_STRING_BUFFER_add(string_buffer, ");\n");
            break;
        }
        
        SPVM_STRING_BUFFER_add(string_buffer, "  string_length = strlen(convert_string_buffer);\n"
                                              "  string = env->new_string_raw(env, stack, convert_string_buffer, string_length);\n"
                                              "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", string);\n");

        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_STRING_TO_BYTE_ARRAY: {
        SPVM_STRING_BUFFER_add(string_buffer, "  src_string = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  src_string_length = env->length(env, stack, src_string);"
                                              "  src_string_data = (char*)env->get_chars(env, stack, src_string);"
                                              "  byte_array = env->new_byte_array_raw(env, stack, src_string_length);"
                                              "  byte_array_data = env->get_elems_byte(env, stack, byte_array);"
                                              "  memcpy(byte_array_data, src_string_data, src_string_length);"
                                              "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", byte_array);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_BYTE_ARRAY_TO_STRING: {
        SPVM_STRING_BUFFER_add(string_buffer, "  src_byte_array = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  src_byte_array_length = env->length(env, stack, src_byte_array);"
                                              "  src_byte_array_data = env->get_elems_byte(env, stack, src_byte_array);"
                                              "  string = env->new_string_raw(env, stack, (const char*)src_byte_array_data, src_byte_array_length);"
                                              "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", string);\n");
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_BYTE_TO_BYTE_OBJECT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  value_byte = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE_CLASS;\n"
                                              "  object = env->new_object_raw(env, stack, basic_type_id);\n"
                                              "  fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);\n"
                                              "  *(int8_t*)&fields[0] = value_byte;\n"
                                              "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_SHORT_TO_SHORT_OBJECT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  value_short = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT_CLASS;\n"
                                              "  object = env->new_object_raw(env, stack, basic_type_id);\n"
                                              "  fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);\n"
                                              "  *(int16_t*)&fields[0] = value_short;\n"
                                              "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_INT_TO_INT_OBJECT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  value_int = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_INT_CLASS;\n"
                                              "  object = env->new_object_raw(env, stack, basic_type_id);\n"
                                              "  fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);\n"
                                              "  *(int32_t*)&fields[0] = value_int;\n"
                                              "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_LONG_TO_LONG_OBJECT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  value_long = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_LONG_CLASS;\n"
                                              "  object = env->new_object_raw(env, stack, basic_type_id);\n"
                                              "  fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);\n"
                                              "  *(int64_t*)&fields[0] = value_long;\n"
                                              "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_FLOAT_TO_FLOAT_OBJECT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  value_float = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT_CLASS;\n"
                                              "  object = env->new_object_raw(env, stack, basic_type_id);\n"
                                              "  fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);\n"
                                              "  *(float*)&fields[0] = value_float;\n"
                                              "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_DOUBLE_TO_DOUBLE_OBJECT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  value_double = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  basic_type_id = SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE_CLASS;\n"
                                              "  object = env->new_object_raw(env, stack, basic_type_id);\n"
                                              "  fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);\n"
                                              "  *(double*)&fields[0] = value_double;\n"
                                              "  SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ", object);\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_BYTE_OBJECT_TO_BYTE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (object == NULL) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_UNBOXING_CONVERSION_FROM_UNDEF]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);\n"
                                              "    object_type_dimension_id = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);\n"
                                              "    if (object_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE_CLASS && object_type_dimension_id == 0) {\n"
                                              "      fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);\n"
                                              "      \n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = *(int8_t*)&fields[0];\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_UNBOXING_CONVERSION_NON_CORRESPONDING_NUMERIC_OBJECT_TYPE]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_SHORT_OBJECT_TO_SHORT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (object == NULL) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_UNBOXING_CONVERSION_FROM_UNDEF]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);\n"
                                              "    object_type_dimension_id = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);\n"
                                              "    if (object_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT_CLASS && object_type_dimension_id == 0) {\n"
                                              "      fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);\n"
                                              "      \n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = *(int16_t*)&fields[0];\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_UNBOXING_CONVERSION_NON_CORRESPONDING_NUMERIC_OBJECT_TYPE]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_INT_OBJECT_TO_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (object == NULL) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_UNBOXING_CONVERSION_FROM_UNDEF]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);\n"
                                              "    object_type_dimension_id = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);\n"
                                              "    if (object_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_INT_CLASS && object_type_dimension_id == 0) {\n"
                                              "      fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);\n"
                                              "      \n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = *(int32_t*)&fields[0];\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_UNBOXING_CONVERSION_NON_CORRESPONDING_NUMERIC_OBJECT_TYPE]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_LONG_OBJECT_TO_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (object == NULL) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_UNBOXING_CONVERSION_FROM_UNDEF]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);\n"
                                              "    object_type_dimension_id = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);\n"
                                              "    if (object_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_LONG_CLASS && object_type_dimension_id == 0) {\n"
                                              "      fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);\n"
                                              "      \n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = *(int64_t*)&fields[0];\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_UNBOXING_CONVERSION_NON_CORRESPONDING_NUMERIC_OBJECT_TYPE]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_FLOAT_OBJECT_TO_FLOAT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (object == NULL) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_UNBOXING_CONVERSION_FROM_UNDEF]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);\n"
                                              "    object_type_dimension_id = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);\n"
                                              "    if (object_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT_CLASS && object_type_dimension_id == 0) {\n"
                                              "      fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);\n"
                                              "      \n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = *(float*)&fields[0];\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_UNBOXING_CONVERSION_NON_CORRESPONDING_NUMERIC_OBJECT_TYPE]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_TYPE_CONVERSION_DOUBLE_OBJECT_TO_DOUBLE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (object == NULL) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_UNBOXING_CONVERSION_FROM_UNDEF]);\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    object_basic_type_id = *(int32_t*)((intptr_t)object + (intptr_t)env->object_basic_type_id_offset);\n"
                                              "    object_type_dimension_id = *(uint8_t*)((intptr_t)object + (intptr_t)env->object_type_dimension_offset);\n"
                                              "    if (object_basic_type_id == SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE_CLASS && object_type_dimension_id == 0) {\n"
                                              "      fields = (SPVM_VALUE*)((intptr_t)object + object_header_byte_size);\n"
                                              "      \n");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, " = *(double*)&fields[0];\n"
                                              "    }\n"
                                              "    else {\n"
                                              "      exception = env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_UNBOXING_CONVERSION_NON_CORRESPONDING_NUMERIC_OBJECT_TYPE]);\n"
                                              "      env->set_exception(env, stack, exception);\n"
                                              "      error = 1;\n"
                                              "    }\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_BYTE:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, stack_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_SHORT:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, stack_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_INT:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, stack_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_LONG:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, stack_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_FLOAT:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, stack_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_DOUBLE:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, stack_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_OBJECT:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, stack_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_REF:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, stack_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        stack_index++;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_UNDEF:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, stack_index);
        SPVM_STRING_BUFFER_add(string_buffer, " = NULL;\n");
        stack_index++;
        
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_BYTE: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, stack_index + field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, opcode->operand0, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        stack_index += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_SHORT: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, stack_index + field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, opcode->operand0, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        stack_index += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_INT: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, stack_index + field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, opcode->operand0, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        stack_index += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_LONG: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, stack_index + field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, opcode->operand0, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        stack_index += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_FLOAT: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, stack_index + field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, opcode->operand0, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        stack_index += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_PUSH_ARG_MULNUM_DOUBLE: {
        int32_t fields_length = opcode->operand1;
        for (int32_t field_index = 0; field_index < fields_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, stack_index + field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, opcode->operand0, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        stack_index += fields_length;
        break;
      }
      case SPVM_OPCODE_C_ID_CALL_CLASS_METHOD_BY_ID:
      case SPVM_OPCODE_C_ID_CALL_INSTANCE_METHOD_BY_ID:
      case SPVM_OPCODE_C_ID_CALL_INSTANCE_METHOD_BY_NAME:
      {
        int32_t var_id = opcode->operand0;
        int32_t decl_method_id = opcode->operand1;
        int32_t call_method_args_stack_length = opcode->operand2 >> 16;
        
        int32_t decl_method_name_id = SPVM_API_RUNTIME_get_method_name_id(runtime, decl_method_id);
        const char* decl_method_name = SPVM_API_RUNTIME_get_name(runtime, decl_method_name_id);
        int32_t decl_method_class_id = SPVM_API_RUNTIME_get_method_class_id(runtime, decl_method_id);
        int32_t decl_method_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, decl_method_class_id);
        const char* decl_method_class_name = SPVM_API_RUNTIME_get_name(runtime, decl_method_class_name_id);
        int32_t decl_method_has_precompile_flag = SPVM_API_RUNTIME_get_method_is_precompile(runtime, decl_method_id);
        int32_t decl_method_return_type_id = SPVM_API_RUNTIME_get_method_return_type_id(runtime, decl_method_id);
        int32_t decl_method_return_type_dimension = SPVM_API_RUNTIME_get_type_dimension(runtime, decl_method_return_type_id);
        int32_t decl_method_return_basic_type_id = SPVM_API_RUNTIME_get_type_basic_type_id(runtime, decl_method_return_type_id);
        int32_t decl_method_return_basic_type_category = SPVM_API_RUNTIME_get_basic_type_category(runtime, decl_method_return_basic_type_id);
        
        SPVM_STRING_BUFFER_add(string_buffer, "  // ");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)decl_method_class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "->");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)decl_method_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\n"
                                              "  call_method_args_stack_length = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, call_method_args_stack_length);
        SPVM_STRING_BUFFER_add(string_buffer,
                                              ";\n");
        
        // Call method
        switch (opcode_id) {
          case SPVM_OPCODE_C_ID_CALL_CLASS_METHOD_BY_ID:
          {
            SPVM_STRING_BUFFER_add(string_buffer, "  call_method_id = env->get_class_method_id(env, \"");
            SPVM_STRING_BUFFER_add(string_buffer, (char*)decl_method_class_name);
            SPVM_STRING_BUFFER_add(string_buffer, "\", \"");
            SPVM_STRING_BUFFER_add(string_buffer, (char*)decl_method_name);
            SPVM_STRING_BUFFER_add(string_buffer, "\");\n");
            
            break;
          }
          case SPVM_OPCODE_C_ID_CALL_INSTANCE_METHOD_BY_ID:
          {
            SPVM_STRING_BUFFER_add(string_buffer, "  call_method_id = env->get_instance_method_id_static(env, \"");
            SPVM_STRING_BUFFER_add(string_buffer, (char*)decl_method_class_name);
            SPVM_STRING_BUFFER_add(string_buffer, "\", \"");
            SPVM_STRING_BUFFER_add(string_buffer, (char*)decl_method_name);
            SPVM_STRING_BUFFER_add(string_buffer, "\");\n");
            
            break;
          }
          case SPVM_OPCODE_C_ID_CALL_INSTANCE_METHOD_BY_NAME: {
            SPVM_STRING_BUFFER_add(string_buffer, "  object = stack[0].oval;\n");
            SPVM_STRING_BUFFER_add(string_buffer, "  call_method_id = env->get_instance_method_id(env, object, \"");
            SPVM_STRING_BUFFER_add(string_buffer, (char*)decl_method_name);
            SPVM_STRING_BUFFER_add(string_buffer, "\");\n");
            
            break;
          }
          default: {
            assert(0);
          }
        }
        
        SPVM_STRING_BUFFER_add(string_buffer, "  if (call_method_id < 0) {\n"
                                              "    exception = env->new_string_nolen_raw(env, stack, \"The \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)decl_method_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" method in the \\\"");
        SPVM_STRING_BUFFER_add(string_buffer, (char*)decl_method_class_name);
        SPVM_STRING_BUFFER_add(string_buffer, "\\\" class is not found\");\n"
                                              "    env->set_exception(env, stack, exception);\n"
                                              "    error = 1;\n"
                                              "  }\n");

        SPVM_STRING_BUFFER_add(string_buffer,
                                              "  if (__builtin_expect(error == 0, 1)) {\n"
                                              "    error = env->call_spvm_method(env, stack, call_method_id, call_method_args_stack_length);\n"
                                              "  }\n");
        
        // Call method
        SPVM_STRING_BUFFER_add(string_buffer, "  if (__builtin_expect(error == 0, 1)) {\n");
        if (decl_method_return_type_dimension == 0) {
          switch (decl_method_return_basic_type_category) {
            case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_VOID: {
              break;
            }
            case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_NUMERIC: {
              switch (decl_method_return_basic_type_id) {
                case SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE: {
                  SPVM_STRING_BUFFER_add(string_buffer, "    ");
                  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, var_id);
                  SPVM_STRING_BUFFER_add(string_buffer, " = ");
                  SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, 0);
                  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT: {
                  SPVM_STRING_BUFFER_add(string_buffer, "    ");
                  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, var_id);
                  SPVM_STRING_BUFFER_add(string_buffer, " = ");
                  SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, 0);
                  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_ID_INT: {
                  SPVM_STRING_BUFFER_add(string_buffer, "    ");
                  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, var_id);
                  SPVM_STRING_BUFFER_add(string_buffer, " = ");
                  SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
                  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_ID_LONG: {
                  SPVM_STRING_BUFFER_add(string_buffer, "    ");
                  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, var_id);
                  SPVM_STRING_BUFFER_add(string_buffer, " = ");
                  SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, 0);
                  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT: {
                  SPVM_STRING_BUFFER_add(string_buffer, "    ");
                  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, var_id);
                  SPVM_STRING_BUFFER_add(string_buffer, " = ");
                  SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, 0);
                  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE: {
                  SPVM_STRING_BUFFER_add(string_buffer, "    ");
                  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, var_id);
                  SPVM_STRING_BUFFER_add(string_buffer, " = ");
                  SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, 0);
                  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
                  break;
                }
                default: {
                  assert(0);
                }
              }
              break;
            }
            case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_MULNUM:
            {
              int32_t decl_method_return_class_id = SPVM_API_RUNTIME_get_basic_type_class_id(runtime, decl_method_return_basic_type_id);
              int32_t decl_method_return_class_fields_length = SPVM_API_RUNTIME_get_class_fields_length(runtime, decl_method_return_class_id);
              int32_t decl_method_return_class_fields_base_id = SPVM_API_RUNTIME_get_class_fields_base_id(runtime, decl_method_return_class_id);
              int32_t decl_method_return_class_field_type_id = SPVM_API_RUNTIME_get_field_type_id(runtime, decl_method_return_class_fields_base_id);
              int32_t decl_method_return_class_field_type_basic_type_id = SPVM_API_RUNTIME_get_type_basic_type_id(runtime, decl_method_return_class_field_type_id);
              assert(decl_method_return_class_field_type_basic_type_id >= 0);
              
              switch (decl_method_return_class_field_type_basic_type_id) {
                case SPVM_NATIVE_C_BASIC_TYPE_ID_BYTE: {
                  int32_t decl_method_return_type_stack_length = opcode->operand3;
                  for (int32_t field_index = 0; field_index < decl_method_return_type_stack_length; field_index++) {
                    SPVM_STRING_BUFFER_add(string_buffer, "    ");
                    SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, var_id + field_index);
                    SPVM_STRING_BUFFER_add(string_buffer, " = ");
                    SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, field_index);
                    SPVM_STRING_BUFFER_add(string_buffer, ";\n");
                  }
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_ID_SHORT: {
                  int32_t decl_method_return_type_stack_length = opcode->operand3;
                  for (int32_t field_index = 0; field_index < decl_method_return_type_stack_length; field_index++) {
                    SPVM_STRING_BUFFER_add(string_buffer, "    ");
                    SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, var_id + field_index);
                    SPVM_STRING_BUFFER_add(string_buffer, " = ");
                    SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, field_index);
                    SPVM_STRING_BUFFER_add(string_buffer, ";\n");
                  }
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_ID_INT: {
                  int32_t decl_method_return_type_stack_length = opcode->operand3;
                  for (int32_t field_index = 0; field_index < decl_method_return_type_stack_length; field_index++) {
                    SPVM_STRING_BUFFER_add(string_buffer, "    ");
                    SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, var_id + field_index);
                    SPVM_STRING_BUFFER_add(string_buffer, " = ");
                    SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, field_index);
                    SPVM_STRING_BUFFER_add(string_buffer, ";\n");
                  }
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_ID_LONG: {
                  int32_t decl_method_return_type_stack_length = opcode->operand3;
                  for (int32_t field_index = 0; field_index < decl_method_return_type_stack_length; field_index++) {
                    SPVM_STRING_BUFFER_add(string_buffer, "    ");
                    SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, var_id + field_index);
                    SPVM_STRING_BUFFER_add(string_buffer, " = ");
                    SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, field_index);
                    SPVM_STRING_BUFFER_add(string_buffer, ";\n");
                  }
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_ID_FLOAT: {
                  int32_t decl_method_return_type_stack_length = opcode->operand3;
                  for (int32_t field_index = 0; field_index < decl_method_return_type_stack_length; field_index++) {
                    SPVM_STRING_BUFFER_add(string_buffer, "    ");
                    SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, var_id + field_index);
                    SPVM_STRING_BUFFER_add(string_buffer, " = ");
                    SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, field_index);
                    SPVM_STRING_BUFFER_add(string_buffer, ";\n");
                  }
                  break;
                }
                case SPVM_NATIVE_C_BASIC_TYPE_ID_DOUBLE: {
                  int32_t decl_method_return_type_stack_length = opcode->operand3;
                  for (int32_t field_index = 0; field_index < decl_method_return_type_stack_length; field_index++) {
                    SPVM_STRING_BUFFER_add(string_buffer, "    ");
                    SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, var_id + field_index);
                    SPVM_STRING_BUFFER_add(string_buffer, " = ");
                    SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, field_index);
                    SPVM_STRING_BUFFER_add(string_buffer, ";\n");
                  }
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
            case SPVM_NATIVE_C_BASIC_TYPE_CATEGORY_ANY_OBJECT:
            {
              SPVM_STRING_BUFFER_add(string_buffer, "    SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
              SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, var_id);
              SPVM_STRING_BUFFER_add(string_buffer, ", stack[0].oval);\n");
              break;
            }
            default: {
              assert(0);
            }
          }
        }
        else if (decl_method_return_type_dimension > 0) {
          SPVM_STRING_BUFFER_add(string_buffer, "    SPVM_INLINE_API_OBJECT_ASSIGN(env, stack, &");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, var_id);
          SPVM_STRING_BUFFER_add(string_buffer, ", stack[0].oval);\n");
        }
        else {
          assert(0);
        }

        SPVM_STRING_BUFFER_add(string_buffer, "  }\n");

        stack_index = 0;
        
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_BYTE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  byte_vars[arg_mem_id] = *(int8_t*)&stack[stack_index];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_SHORT: {
        SPVM_STRING_BUFFER_add(string_buffer, "    arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    short_vars[arg_mem_id] = *(int16_t*)&stack[stack_index];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  int_vars[arg_mem_id] = *(int32_t*)&stack[stack_index];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  long_vars[arg_mem_id] = *(int64_t*)&stack[stack_index];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_FLOAT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  float_vars[arg_mem_id] = *(float*)&stack[stack_index];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_DOUBLE: {
        SPVM_STRING_BUFFER_add(string_buffer, "    arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    double_vars[arg_mem_id] = *(double*)&stack[stack_index];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_OBJECT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  object_vars[arg_mem_id] = *(void**)&stack[stack_index];\n"
                                              "  object = *(void**)&object_vars[arg_mem_id];\n"
                                              "  if (object != NULL) {\n"
                                              "    SPVM_INLINE_API_INC_REF_COUNT_ONLY(env, stack, object);\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_REF: {
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  ref_vars[arg_mem_id] = *(void**)&stack[stack_index];\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_MULNUM_BYTE: {
        SPVM_STRING_BUFFER_add(string_buffer, "    arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    type_stack_length = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 >> 8);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    for (int32_t field_index = 0; field_index < type_stack_length; field_index++) {\n"
                                              "      byte_vars[arg_mem_id + field_index] = *(int8_t*)&stack[stack_index + field_index];\n"
                                              "    }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_MULNUM_SHORT: {
        SPVM_STRING_BUFFER_add(string_buffer, "    arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    type_stack_length = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 >> 8);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    for (int32_t field_index = 0; field_index < type_stack_length; field_index++) {\n"
                                              "      short_vars[arg_mem_id + field_index] = *(int16_t*)&stack[stack_index + field_index];\n"
                                              "    }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_MULNUM_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "    arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    type_stack_length = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 >> 8);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    for (int32_t field_index = 0; field_index < type_stack_length; field_index++) {\n"
                                              "      int_vars[arg_mem_id + field_index] = *(int32_t*)&stack[stack_index + field_index];\n"
                                              "    }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_MULNUM_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "    arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    type_stack_length = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 >> 8);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    for (int32_t field_index = 0; field_index < type_stack_length; field_index++) {\n"
                                              "      long_vars[arg_mem_id + field_index] = *(int64_t*)&stack[stack_index + field_index];\n"
                                              "    }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_MULNUM_FLOAT: {
        SPVM_STRING_BUFFER_add(string_buffer, "    arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    type_stack_length = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 >> 8);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    for (int32_t field_index = 0; field_index < type_stack_length; field_index++) {\n"
                                              "      float_vars[arg_mem_id + field_index] = *(float*)&stack[stack_index + field_index];\n"
                                              "    }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_MULNUM_DOUBLE: {
        SPVM_STRING_BUFFER_add(string_buffer, "    arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    type_stack_length = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 >> 8);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    for (int32_t field_index = 0; field_index < type_stack_length; field_index++) {\n"
                                              "      double_vars[arg_mem_id + field_index] = *(double*)&stack[stack_index + field_index];\n"
                                              "    }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_OPTIONAL_BYTE: {
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  args_stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (args_stack_index >= args_stack_length) {\n"
                                              "    byte_vars[arg_mem_id] = ");
        SPVM_STRING_BUFFER_add_byte(string_buffer, (int8_t)(uint8_t)opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    byte_vars[arg_mem_id] = *(int8_t*)&stack[args_stack_index];\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_OPTIONAL_SHORT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  args_stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (args_stack_index >= args_stack_length) {\n"
                                              "    short_vars[arg_mem_id] = ");
        SPVM_STRING_BUFFER_add_short(string_buffer, (int16_t)(uint16_t)opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    short_vars[arg_mem_id] = *(int16_t*)&stack[args_stack_index];\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_OPTIONAL_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  args_stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (args_stack_index >= args_stack_length) {\n"
                                              "    int_vars[arg_mem_id] = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, (int32_t)opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    int_vars[arg_mem_id] = *(int32_t*)&stack[args_stack_index];\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_OPTIONAL_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  args_stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (args_stack_index >= args_stack_length) {\n"
                                              "    long_vars[arg_mem_id] = ");
        SPVM_STRING_BUFFER_add_long(string_buffer, *(int64_t*)&opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    long_vars[arg_mem_id] = *(int64_t*)&stack[args_stack_index];\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_OPTIONAL_FLOAT: {
        SPVM_VALUE value;
        value.ival = (int32_t)opcode->operand1;
        
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "    args_stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (args_stack_index >= args_stack_length) {\n"
                                              "    SPVM_VALUE tmp_constant;\n"
                                              "    float_vars[arg_mem_id] ");
        SPVM_STRING_BUFFER_add(string_buffer, " = (tmp_constant.ival = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, value.ival);
        SPVM_STRING_BUFFER_add(string_buffer, ", tmp_constant.fval);\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    float_vars[arg_mem_id] = *(float*)&stack[args_stack_index];\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_OPTIONAL_DOUBLE: {
        double double_value = *(double*)&opcode->operand1;
        SPVM_VALUE value;
        value.dval = double_value;
        
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  args_stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (args_stack_index >= args_stack_length) {\n"
                                              "    SPVM_VALUE tmp_constant;\n"
                                              "    double_vars[arg_mem_id] = ");
        SPVM_STRING_BUFFER_add(string_buffer, " (tmp_constant.lval = ");
        SPVM_STRING_BUFFER_add_long(string_buffer, value.lval);
        SPVM_STRING_BUFFER_add(string_buffer, ", tmp_constant.dval);\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    double_vars[arg_mem_id] = *(double*)&stack[args_stack_index];\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_GET_ARG_OPTIONAL_OBJECT: {
        SPVM_STRING_BUFFER_add(string_buffer, "  arg_mem_id = ");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  args_stack_index = ");
        SPVM_STRING_BUFFER_add_int( string_buffer, opcode->operand3 & 0xFF);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (args_stack_index >= args_stack_length) {\n"
                                              "    object_vars[arg_mem_id] = NULL;\n"
                                              "  }\n"
                                              "  else {\n"
                                              "    object_vars[arg_mem_id] = *(void**)&stack[args_stack_index];\n"
                                              "    object = *(void**)&object_vars[arg_mem_id];\n"
                                              "    if (object != NULL) {\n"
                                              "      SPVM_INLINE_API_INC_REF_COUNT_ONLY(env, stack, object);\n"
                                              "    }\n"
                                              "  }\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_VOID:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_BYTE: {
        int32_t var_id = opcode->operand0;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, var_id);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_SHORT: {
        int32_t var_id = opcode->operand0;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, var_id);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_INT: {
        int32_t var_id = opcode->operand0;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, var_id);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_LONG: {
        int32_t var_id = opcode->operand0;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, var_id);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_FLOAT: {
        int32_t var_id = opcode->operand0;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, var_id);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_DOUBLE: {
        int32_t var_id = opcode->operand0;
        SPVM_STRING_BUFFER_add(string_buffer, "  ");
        SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, 0);
        SPVM_STRING_BUFFER_add(string_buffer, " = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, var_id);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_OBJECT:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  *(void**)&stack[0] = ");
        SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, opcode->operand0);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                              "  if (*(void**)&stack[0] != NULL) {\n"
                                              "    SPVM_INLINE_API_INC_REF_COUNT_ONLY(env, stack, *(void**)&stack[0]);\n"
                                              "  }\n"
                                              "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_UNDEF:
      {
        SPVM_STRING_BUFFER_add(string_buffer, "  *(void**)&stack[0] = NULL;\n"
                                              "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_MULNUM_BYTE:
      {
        int32_t var_id = opcode->operand0;
        int32_t method_return_type_stack_length = opcode->operand2;
        
        // Multi numeric type
        for (int32_t field_index = 0; field_index < method_return_type_stack_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_BYTE, var_id + field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_MULNUM_SHORT:
      {
        int32_t var_id = opcode->operand0;
        int32_t method_return_type_stack_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < method_return_type_stack_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_SHORT, var_id + field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_MULNUM_INT:
      {
        int32_t var_id = opcode->operand0;
        int32_t method_return_type_stack_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < method_return_type_stack_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, var_id + field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_MULNUM_LONG:
      {
        int32_t var_id = opcode->operand0;
        int32_t method_return_type_stack_length = opcode->operand2;
        
        // Multi numeric type
        for (int32_t field_index = 0; field_index < method_return_type_stack_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_LONG, var_id + field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_MULNUM_FLOAT:
      {
        int32_t var_id = opcode->operand0;
        int32_t method_return_type_stack_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < method_return_type_stack_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT, var_id + field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      case SPVM_OPCODE_C_ID_RETURN_MULNUM_DOUBLE:
      {
        int32_t var_id = opcode->operand0;
        int32_t method_return_type_stack_length = opcode->operand2;
        for (int32_t field_index = 0; field_index < method_return_type_stack_length; field_index++) {
          SPVM_STRING_BUFFER_add(string_buffer, "  ");
          SPVM_PRECOMPILE_add_stack(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, field_index);
          SPVM_STRING_BUFFER_add(string_buffer, " = ");
          SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE, var_id + field_index);
          SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        }
        SPVM_STRING_BUFFER_add(string_buffer, "  goto L");
        SPVM_STRING_BUFFER_add_int(string_buffer, opcode->operand1);
        SPVM_STRING_BUFFER_add(string_buffer, ";\n");
        break;
      }
      default: {
        spvm_warn("[Unexpected Error]The \"%s\" opcode is not defined", SPVM_OPCODE_get_opcode_name(opcode_id));
        assert(0);
      }
    }
    opcode_index++;
  }
  
  // No exception
  SPVM_STRING_BUFFER_add(string_buffer, "  return_value = 0;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  if (error) {\n");
  SPVM_STRING_BUFFER_add(string_buffer, "    return_value = error;\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  }\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  else {\n");
  int32_t method_return_type_check_runtime_assignability_to_any_object = SPVM_API_RUNTIME_get_type_is_object(runtime, method_return_type_id);
  if (method_return_type_check_runtime_assignability_to_any_object) {
    SPVM_STRING_BUFFER_add(string_buffer, "  if (stack[0].oval != NULL) { SPVM_INLINE_API_DEC_REF_COUNT_ONLY(env, stack, stack[0].oval); }\n");
  }
  SPVM_STRING_BUFFER_add(string_buffer, "  }\n"
  "  return return_value;\n"
                                        "}\n"
                                        "\n");
}

const char* SPVM_PRECOMPILE_get_ctype_name(SPVM_PRECOMPILE* precompile, int32_t ctype_id) {
  SPVM_RUNTIME* runtime = precompile->runtime;

  
  switch (ctype_id) {
    case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE:
      return "int8_t";
      break;
    case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT:
      return "int16_t";
      break;
    case SPVM_PRECOMPILE_C_CTYPE_ID_INT:
      return "int32_t";
      break;
    case SPVM_PRECOMPILE_C_CTYPE_ID_LONG:
      return "int64_t";
      break;
    case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT:
      return "float";
      break;
    case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE:
      return "double";
      break;
    case SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT:
      return "void*";
      break;
    case SPVM_PRECOMPILE_C_CTYPE_ID_REF:
      return "void*";
      break;
  }
  
  return NULL;
}

void SPVM_PRECOMPILE_add_var(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  switch (ctype_id) {
    case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE: {
      SPVM_STRING_BUFFER_add(string_buffer, "byte_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT: {
      SPVM_STRING_BUFFER_add(string_buffer, "short_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_INT: {
      SPVM_STRING_BUFFER_add(string_buffer, "int_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_LONG: {
      SPVM_STRING_BUFFER_add(string_buffer, "long_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT: {
      SPVM_STRING_BUFFER_add(string_buffer, "float_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE: {
      SPVM_STRING_BUFFER_add(string_buffer, "double_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_BOOL_CONVERSION_OBJECT:
    case SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT:
    {
      SPVM_STRING_BUFFER_add(string_buffer, "object_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_REF: {
      SPVM_STRING_BUFFER_add(string_buffer, "ref_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    default:
      assert(0);
  }
}

void SPVM_PRECOMPILE_add_var_value(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t index, int32_t field_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;

  
  switch (ctype_id) {
    case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE: {
      SPVM_STRING_BUFFER_add(string_buffer, "byte_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index + field_index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT: {
      SPVM_STRING_BUFFER_add(string_buffer, "short_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index + field_index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_INT: {
      SPVM_STRING_BUFFER_add(string_buffer, "int_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index + field_index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_LONG: {
      SPVM_STRING_BUFFER_add(string_buffer, "long_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index + field_index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT: {
      SPVM_STRING_BUFFER_add(string_buffer, "float_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index + field_index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE: {
      SPVM_STRING_BUFFER_add(string_buffer, "double_vars[");
      SPVM_STRING_BUFFER_add_int(string_buffer, index + field_index);
      SPVM_STRING_BUFFER_add(string_buffer, "]");
      break;
    }
    default:
      assert(0);
  }
}

void SPVM_PRECOMPILE_add_operand(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t var_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;

  
  SPVM_PRECOMPILE_add_var(precompile, string_buffer, ctype_id, var_index);
}

void SPVM_PRECOMPILE_add_operand_deref(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t var_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "*(");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, ctype_id));
  SPVM_STRING_BUFFER_add(string_buffer, "*)*(void**)&");
  SPVM_PRECOMPILE_add_var(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_REF, var_index);
}

void SPVM_PRECOMPILE_add_stack(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t var_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "*(");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, ctype_id));
  SPVM_STRING_BUFFER_add(string_buffer, "*)&");
  SPVM_STRING_BUFFER_add(string_buffer, "stack[");
  SPVM_STRING_BUFFER_add_int(string_buffer, var_index);
  SPVM_STRING_BUFFER_add(string_buffer, "]");
}

void SPVM_PRECOMPILE_add_operand_value(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t var_index, int32_t field_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_PRECOMPILE_add_var_value(precompile, string_buffer, ctype_id, var_index, field_index);
}

void SPVM_PRECOMPILE_add_bool(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
  if (ctype_id == SPVM_PRECOMPILE_C_CTYPE_ID_INT) {
    SPVM_STRING_BUFFER_add(string_buffer, " = ");
    SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in_index);
    SPVM_STRING_BUFFER_add(string_buffer, ";\n");
  }
  else if (ctype_id == SPVM_PRECOMPILE_C_CTYPE_ID_BOOL_CONVERSION_OBJECT) {
    SPVM_STRING_BUFFER_add(string_buffer, " = !!env->get_bool_object_value(env, stack, ");
    SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in_index);
    SPVM_STRING_BUFFER_add(string_buffer, ");\n");
  }
  else {
    SPVM_STRING_BUFFER_add(string_buffer, " = !!");
    SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in_index);
    SPVM_STRING_BUFFER_add(string_buffer, ";\n");
  }
}

void SPVM_PRECOMPILE_add_numeric_eq(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
  SPVM_STRING_BUFFER_add(string_buffer, " = (");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in1_index);
  SPVM_STRING_BUFFER_add(string_buffer, " == ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in2_index);
  SPVM_STRING_BUFFER_add(string_buffer, ");\n");
}

void SPVM_PRECOMPILE_add_numeric_ne(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
  SPVM_STRING_BUFFER_add(string_buffer, " = (");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in1_index);
  SPVM_STRING_BUFFER_add(string_buffer, " != ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in2_index);
  SPVM_STRING_BUFFER_add(string_buffer, ");\n");
}

void SPVM_PRECOMPILE_add_numeric_gt(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
  SPVM_STRING_BUFFER_add(string_buffer, " = (");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in1_index);
  SPVM_STRING_BUFFER_add(string_buffer, " > ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in2_index);
  SPVM_STRING_BUFFER_add(string_buffer, ");\n");
}

void SPVM_PRECOMPILE_add_numeric_ge(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
  SPVM_STRING_BUFFER_add(string_buffer, " = (");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in1_index);
  SPVM_STRING_BUFFER_add(string_buffer, " >= ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in2_index);
  SPVM_STRING_BUFFER_add(string_buffer, ");\n");
}

void SPVM_PRECOMPILE_add_numeric_lt(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
  SPVM_STRING_BUFFER_add(string_buffer, " = (");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in1_index);
  SPVM_STRING_BUFFER_add(string_buffer, " < ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in2_index);
  SPVM_STRING_BUFFER_add(string_buffer, ");\n");
}

void SPVM_PRECOMPILE_add_numeric_le(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
  SPVM_STRING_BUFFER_add(string_buffer, " = (");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in1_index);
  SPVM_STRING_BUFFER_add(string_buffer, " <= ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in2_index);
  SPVM_STRING_BUFFER_add(string_buffer, ");\n");
}

void SPVM_PRECOMPILE_add_numeric_cmp(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t in1_index, int32_t in2_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  

  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, 0);
  SPVM_STRING_BUFFER_add(string_buffer, " = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in1_index);
  SPVM_STRING_BUFFER_add(string_buffer, " > ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in2_index);
  SPVM_STRING_BUFFER_add(string_buffer, " ? 1\n");
  SPVM_STRING_BUFFER_add(string_buffer, "            : ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in1_index);
  SPVM_STRING_BUFFER_add(string_buffer, " < ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in2_index);
  SPVM_STRING_BUFFER_add(string_buffer, " ? -1\n");
  SPVM_STRING_BUFFER_add(string_buffer, "            : 0;\n");
}

void SPVM_PRECOMPILE_add_bit_and(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in1_index);
  SPVM_STRING_BUFFER_add(string_buffer, " & ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in2_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
}

void SPVM_PRECOMPILE_add_bit_or(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in1_index);
  SPVM_STRING_BUFFER_add(string_buffer, " | ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in2_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
}

void SPVM_PRECOMPILE_add_bit_xor(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in1_index, int32_t in2_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in1_index);
  SPVM_STRING_BUFFER_add(string_buffer, " ^ ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in2_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
}

void SPVM_PRECOMPILE_add_negate(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = -");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
}

void SPVM_PRECOMPILE_add_plus(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
}

void SPVM_PRECOMPILE_add_complement(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = ~");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
}

void SPVM_PRECOMPILE_add_inc(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t number) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " += ");
  SPVM_STRING_BUFFER_add_int(string_buffer, number);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
}

void SPVM_PRECOMPILE_add_convert(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t out_ctype_id, int32_t in_ctype_id, int32_t out_index, int32_t in_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, out_ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = (");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, out_ctype_id));
  SPVM_STRING_BUFFER_add(string_buffer, ")");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, in_ctype_id, in_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
}

void SPVM_PRECOMPILE_add_array_get(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t out_index, int32_t array_index, int32_t index_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  array = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, array_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "  index = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, index_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, element_ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = ");
  
  switch (element_ctype_id) {
    case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE: {
      SPVM_STRING_BUFFER_add(string_buffer, "SPVM_INLINE_API_GET_ARRAY_ELEMENT_BYTE(env, stack, array, index, (int32_t*)&error, object_header_byte_size);\n");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT: {
      SPVM_STRING_BUFFER_add(string_buffer, "SPVM_INLINE_API_GET_ARRAY_ELEMENT_SHORT(env, stack, array, index, (int32_t*)&error, object_header_byte_size);\n");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_INT: {
      SPVM_STRING_BUFFER_add(string_buffer, "SPVM_INLINE_API_GET_ARRAY_ELEMENT_INT(env, stack, array, index, (int32_t*)&error, object_header_byte_size);\n");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_LONG: {
      SPVM_STRING_BUFFER_add(string_buffer, "SPVM_INLINE_API_GET_ARRAY_ELEMENT_LONG(env, stack, array, index, (int32_t*)&error, object_header_byte_size);\n");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT: {
      SPVM_STRING_BUFFER_add(string_buffer, "SPVM_INLINE_API_GET_ARRAY_ELEMENT_FLOAT(env, stack, array, index, (int32_t*)&error, object_header_byte_size);\n");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE: {
      SPVM_STRING_BUFFER_add(string_buffer, "SPVM_INLINE_API_GET_ARRAY_ELEMENT_DOUBLE(env, stack, array, index, (int32_t*)&error, object_header_byte_size);\n");
      break;
    }
    default: {
      assert(0);
    }
  }
}

void SPVM_PRECOMPILE_add_array_set(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t array_index, int32_t index_index, int32_t in_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  array = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, array_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "  index = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, index_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "  if (__builtin_expect(array == NULL, 0)) { \n"
                                        "    env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_UNDEFINED]));\n"
                                        "    error = 1;\n"
                                        "  }\n"
                                        "  else { \n"
                                        "    if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) { \n"
                                        "      env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_ACCESS_INDEX_OUT_OF_RANGE]));\n"
                                        "      error = 1;\n"
                                        "    }\n"
                                        "    else { \n"
                                        "((");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, element_ctype_id));
  SPVM_STRING_BUFFER_add(string_buffer, "*)((intptr_t)array + object_header_byte_size))[index] = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, element_ctype_id, in_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "    }\n"
                                        "  }\n");
}

void SPVM_PRECOMPILE_add_mulnum_array_get(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t out_index, int32_t array_index, int32_t index_index, int32_t fields_length) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  array = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, array_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "  index = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, index_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "  if (__builtin_expect(array == NULL, 0)) { \n"
                                        "    env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_UNDEFINED]));\n"
                                        "    error = 1;\n"
                                        "  }\n"
                                        "  else { \n"
                                        "    if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) { \n"
                                        "      env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_ACCESS_INDEX_OUT_OF_RANGE]));\n"
                                        "      error = 1;\n"
                                        "    }\n"
                                        "    else { \n");
  {
    int32_t field_index;
    for (field_index = 0; field_index < fields_length; field_index++) {
      SPVM_STRING_BUFFER_add(string_buffer, "  ");
      SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, element_ctype_id, out_index, field_index);
      SPVM_STRING_BUFFER_add(string_buffer, " = ((");
      SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, element_ctype_id));
      SPVM_STRING_BUFFER_add(string_buffer, "*)((intptr_t)array + object_header_byte_size))[");
      SPVM_STRING_BUFFER_add_int(string_buffer, fields_length);
      SPVM_STRING_BUFFER_add(string_buffer, " * index + ");
      SPVM_STRING_BUFFER_add_int(string_buffer, field_index);
      SPVM_STRING_BUFFER_add(string_buffer, "];\n");
    }
  }
  SPVM_STRING_BUFFER_add(string_buffer, "    }\n"
                                        "  }\n");
}

void SPVM_PRECOMPILE_add_mulnum_array_field_get(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t out_index, int32_t array_index, int32_t index_index, int32_t fields_length, int32_t field_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  array = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, array_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  index = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, index_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "  if (__builtin_expect(array == NULL, 0)) { \n"
                                        "    env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_UNDEFINED]));\n"
                                        "    error = 1;\n"
                                        "  }\n"
                                        "  else { \n"
                                        "    if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) { \n"
                                        "      env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_ACCESS_INDEX_OUT_OF_RANGE]));\n"
                                        "      error = 1;\n"
                                        "    }\n"
                                        "    else { \n"
                                        "      ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, element_ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = ((");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, element_ctype_id));
  SPVM_STRING_BUFFER_add(string_buffer, "*)((intptr_t)array + object_header_byte_size))[");
  SPVM_STRING_BUFFER_add_int(string_buffer, fields_length);
  SPVM_STRING_BUFFER_add(string_buffer, " * index + ");
  SPVM_STRING_BUFFER_add_int(string_buffer, field_index);
  SPVM_STRING_BUFFER_add(string_buffer, "];\n"
                                        "    }\n"
                                        "  }\n");
}

void SPVM_PRECOMPILE_add_mulnum_array_set(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t array_index, int32_t index_index, int32_t in_index, int32_t fields_length) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  array = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, array_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "  index = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, index_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "  if (__builtin_expect(array == NULL, 0)) { \n"
                                        "    env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_UNDEFINED]));\n"
                                        "    error = 1;\n"
                                        "  }\n"
                                        "  else {\n"
                                        "    if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) { \n"
                                        "      env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_ACCESS_INDEX_OUT_OF_RANGE]));\n"
                                        "      error = 1;\n"
                                        "    }\n"
                                        "    else { \n");
  
  {
    int32_t field_index;
    for (field_index = 0; field_index < fields_length; field_index++) {
      SPVM_STRING_BUFFER_add(string_buffer, "  ((");
      SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, element_ctype_id));
      SPVM_STRING_BUFFER_add(string_buffer, "*)((intptr_t)array + object_header_byte_size))[");
      SPVM_STRING_BUFFER_add_int(string_buffer, fields_length);
      SPVM_STRING_BUFFER_add(string_buffer, " * index + ");
      SPVM_STRING_BUFFER_add_int(string_buffer, field_index);
      SPVM_STRING_BUFFER_add(string_buffer, "] = ");
      SPVM_PRECOMPILE_add_operand_value(precompile, string_buffer, element_ctype_id, in_index, field_index);
      SPVM_STRING_BUFFER_add(string_buffer, ";\n");
    }
  }

  SPVM_STRING_BUFFER_add(string_buffer, "    }\n"
                                        "  }\n");
}

void SPVM_PRECOMPILE_add_mulnum_array_field_set(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t array_index, int32_t index_index, int32_t in_index, int32_t fields_length, int32_t field_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  array = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, array_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "  index = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_INT, index_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "  if (__builtin_expect(array == NULL, 0)) { \n"
                                        "    env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_UNDEFINED]));\n"
                                        "    error = 1;\n"
                                        "  }\n"
                                        "  else { \n"
                                        "    if (__builtin_expect(index < 0 || index >= *(int32_t*)((intptr_t)array + (intptr_t)env->object_length_offset), 0)) { \n"
                                        "      env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_ARRAY_ACCESS_INDEX_OUT_OF_RANGE]));\n"
                                        "      error = 1;\n"
                                        "    }\n"
                                        "    else { \n"
                                        "      ((");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, element_ctype_id));
  SPVM_STRING_BUFFER_add(string_buffer, "*)((intptr_t)array + object_header_byte_size))[");
  SPVM_STRING_BUFFER_add_int(string_buffer, fields_length);
  SPVM_STRING_BUFFER_add(string_buffer, " * index + ");
  SPVM_STRING_BUFFER_add_int(string_buffer, field_index);
  SPVM_STRING_BUFFER_add(string_buffer, "] = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, element_ctype_id, in_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "    }\n"
                                        "  }\n");
}

void SPVM_PRECOMPILE_add_mulnum_deref(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t out_var_id, int32_t ref_var_id, int32_t fields_length) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  switch (element_ctype_id) {
    case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_byte = *(int8_t**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_short = *(int16_t**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_INT: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_int = *(int32_t**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_LONG: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_long = *(int64_t**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_float = *(float**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_double = *(double**)&ref_vars[");
      break;
    }
  }
  SPVM_STRING_BUFFER_add_int(string_buffer, ref_var_id);
  SPVM_STRING_BUFFER_add(string_buffer, "];\n");
  for (int32_t field_index = 0; field_index < fields_length; field_index++) {
    SPVM_STRING_BUFFER_add(string_buffer, "    *(");
    SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, element_ctype_id));
    SPVM_STRING_BUFFER_add(string_buffer, "*)&");
    switch (element_ctype_id) {
      case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE: {
        SPVM_STRING_BUFFER_add(string_buffer, "byte_vars");
        break;
      }
      case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT: {
        SPVM_STRING_BUFFER_add(string_buffer, "short_vars");
        break;
      }
      case SPVM_PRECOMPILE_C_CTYPE_ID_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "int_vars");
        break;
      }
      case SPVM_PRECOMPILE_C_CTYPE_ID_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "long_vars");
        break;
      }
      case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT: {
        SPVM_STRING_BUFFER_add(string_buffer, "float_vars");
        break;
      }
      case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE: {
        SPVM_STRING_BUFFER_add(string_buffer, "double_vars");
        break;
      }
    }
    SPVM_STRING_BUFFER_add(string_buffer, "[");
    SPVM_STRING_BUFFER_add_int(string_buffer, out_var_id);
    SPVM_STRING_BUFFER_add(string_buffer, " + ");
    SPVM_STRING_BUFFER_add_int(string_buffer, field_index);
    SPVM_STRING_BUFFER_add(string_buffer, "]  = *(");
    SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, element_ctype_id));
    SPVM_STRING_BUFFER_add(string_buffer, "*)&mulnum_ref_");
    switch (element_ctype_id) {
      case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE: {
        SPVM_STRING_BUFFER_add(string_buffer, "byte");
        break;
      }
      case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT: {
        SPVM_STRING_BUFFER_add(string_buffer, "short");
        break;
      }
      case SPVM_PRECOMPILE_C_CTYPE_ID_INT: {
        SPVM_STRING_BUFFER_add(string_buffer, "int");
        break;
      }
      case SPVM_PRECOMPILE_C_CTYPE_ID_LONG: {
        SPVM_STRING_BUFFER_add(string_buffer, "long");
        break;
      }
      case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT: {
        SPVM_STRING_BUFFER_add(string_buffer, "float");
        break;
      }
      case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE: {
        SPVM_STRING_BUFFER_add(string_buffer, "double");
        break;
      }
    }
    SPVM_STRING_BUFFER_add(string_buffer, "[");
    SPVM_STRING_BUFFER_add_int(string_buffer, field_index);
    SPVM_STRING_BUFFER_add(string_buffer, "];\n");
  }
}

void SPVM_PRECOMPILE_add_mulnum_deref_get_field(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t out_var_id, int32_t ref_var_id, int32_t field_index) {  
  SPVM_RUNTIME* runtime = precompile->runtime;
  switch (element_ctype_id) {
    case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_byte = *(int8_t**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_short = *(int16_t**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_INT: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_int = *(int32_t**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_LONG: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_long = *(int64_t**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_float = *(float**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_double = *(double**)&ref_vars[");
      break;
    }
  }
  SPVM_STRING_BUFFER_add_int(string_buffer, ref_var_id);
  SPVM_STRING_BUFFER_add(string_buffer, "];\n"
                                        "    *(");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, element_ctype_id));
  SPVM_STRING_BUFFER_add(string_buffer, "*)&");
  switch (element_ctype_id) {
    case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE: {
      SPVM_STRING_BUFFER_add(string_buffer, "byte_vars");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT: {
      SPVM_STRING_BUFFER_add(string_buffer, "short_vars");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_INT: {
      SPVM_STRING_BUFFER_add(string_buffer, "int_vars");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_LONG: {
      SPVM_STRING_BUFFER_add(string_buffer, "long_vars");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT: {
      SPVM_STRING_BUFFER_add(string_buffer, "float_vars");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE: {
      SPVM_STRING_BUFFER_add(string_buffer, "double_vars");
      break;
    }
  }
  SPVM_STRING_BUFFER_add(string_buffer, "[");
  SPVM_STRING_BUFFER_add_int(string_buffer, out_var_id);
  SPVM_STRING_BUFFER_add(string_buffer, "] = *(");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, element_ctype_id));
  SPVM_STRING_BUFFER_add(string_buffer, "*)&mulnum_ref_");
  switch (element_ctype_id) {
    case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE: {
      SPVM_STRING_BUFFER_add(string_buffer, "byte");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT: {
      SPVM_STRING_BUFFER_add(string_buffer, "short");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_INT: {
      SPVM_STRING_BUFFER_add(string_buffer, "int");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_LONG: {
      SPVM_STRING_BUFFER_add(string_buffer, "long");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT: {
      SPVM_STRING_BUFFER_add(string_buffer, "float");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE: {
      SPVM_STRING_BUFFER_add(string_buffer, "double");
      break;
    }
  }
  SPVM_STRING_BUFFER_add(string_buffer, "[");
  SPVM_STRING_BUFFER_add_int(string_buffer, field_index);
  SPVM_STRING_BUFFER_add(string_buffer, "];\n");
}

void SPVM_PRECOMPILE_add_mulnum_deref_set_field(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t element_ctype_id, int32_t ref_var_id, int32_t in_var_id, int32_t field_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  switch (element_ctype_id) {
    case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_byte = *(int8_t**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_short = *(int16_t**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_INT: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_int = *(int32_t**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_LONG: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_long = *(int64_t**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_float = *(float**)&ref_vars[");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE: {
      SPVM_STRING_BUFFER_add(string_buffer, "  mulnum_ref_double = *(double**)&ref_vars[");
      break;
    }
  }
  SPVM_STRING_BUFFER_add_int(string_buffer, ref_var_id);
  SPVM_STRING_BUFFER_add(string_buffer, "];\n"
                                        "    *(");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, element_ctype_id));
  SPVM_STRING_BUFFER_add(string_buffer, "*)&mulnum_ref_");
  switch (element_ctype_id) {
    case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE: {
      SPVM_STRING_BUFFER_add(string_buffer, "byte");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT: {
      SPVM_STRING_BUFFER_add(string_buffer, "short");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_INT: {
      SPVM_STRING_BUFFER_add(string_buffer, "int");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_LONG: {
      SPVM_STRING_BUFFER_add(string_buffer, "long");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT: {
      SPVM_STRING_BUFFER_add(string_buffer, "float");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE: {
      SPVM_STRING_BUFFER_add(string_buffer, "double");
      break;
    }
  }
  SPVM_STRING_BUFFER_add(string_buffer, "[");
  SPVM_STRING_BUFFER_add_int(string_buffer, field_index);
  SPVM_STRING_BUFFER_add(string_buffer, "] = *(");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, element_ctype_id));
  SPVM_STRING_BUFFER_add(string_buffer, "*)&");
  switch (element_ctype_id) {
    case SPVM_PRECOMPILE_C_CTYPE_ID_BYTE: {
      SPVM_STRING_BUFFER_add(string_buffer, "byte_vars");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_SHORT: {
      SPVM_STRING_BUFFER_add(string_buffer, "short_vars");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_INT: {
      SPVM_STRING_BUFFER_add(string_buffer, "int_vars");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_LONG: {
      SPVM_STRING_BUFFER_add(string_buffer, "long_vars");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_FLOAT: {
      SPVM_STRING_BUFFER_add(string_buffer, "float_vars");
      break;
    }
    case SPVM_PRECOMPILE_C_CTYPE_ID_DOUBLE: {
      SPVM_STRING_BUFFER_add(string_buffer, "double_vars");
      break;
    }
  }
  SPVM_STRING_BUFFER_add(string_buffer, "[");
  SPVM_STRING_BUFFER_add_int(string_buffer, in_var_id);
  SPVM_STRING_BUFFER_add(string_buffer, "];\n");
}

void SPVM_PRECOMPILE_add_move(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
}

void SPVM_PRECOMPILE_add_get_deref(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = ");
  SPVM_PRECOMPILE_add_operand_deref(precompile, string_buffer, ctype_id, in_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
}

void SPVM_PRECOMPILE_add_set_deref(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t ctype_id, int32_t out_index, int32_t in_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  SPVM_STRING_BUFFER_add(string_buffer, "  ");
  SPVM_PRECOMPILE_add_operand_deref(precompile, string_buffer, ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, ctype_id, in_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
}

void SPVM_PRECOMPILE_add_get_field(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t field_ctype_id, int32_t out_index, int32_t object_index, int32_t field_id) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  int32_t field_class_id = SPVM_API_RUNTIME_get_field_class_id(runtime, field_id);
  int32_t field_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, field_class_id);
  const char* field_class_name = SPVM_API_RUNTIME_get_name(runtime, field_class_name_id);
  int32_t field_name_id = SPVM_API_RUNTIME_get_field_name_id(runtime, field_id);
  const char* field_name = SPVM_API_RUNTIME_get_name(runtime, field_name_id);

  SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, object_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
  SPVM_STRING_BUFFER_add(string_buffer, "  access_field_id = env->get_field_id(env, object, \"");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
  SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                        "    if ("
                                        "access_field_id");
  SPVM_STRING_BUFFER_add(string_buffer, " < 0) {\n");
  SPVM_STRING_BUFFER_add(string_buffer, "    exception = env->new_string_nolen_raw(env, stack, \"The field \\\"");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
  SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                        "    env->set_exception(env, stack, exception);\n"
                                        "    error = 1;\n"
                                        "  }\n"
                                        "  if (!error) {\n"
                                        "    int32_t access_field_offset"
                                        " = env->get_field_offset(env, "
                                        "access_field_id"
                                        ");\n"
                                        "    if (__builtin_expect(object == NULL, 0)) {\n"
                                        "      env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_FIELD_ACCESS_INVOCANT_UNDEFINED]));\n"
                                        "      error = 1;\n"
                                        "    }\n"
                                        "    else {\n"
                                        "      ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, field_ctype_id, out_index);
  SPVM_STRING_BUFFER_add(string_buffer, " = *(");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, field_ctype_id));
  SPVM_STRING_BUFFER_add(string_buffer, "*)((intptr_t)object + object_header_byte_size + "
                                        "access_field_offset"
                                        ");\n"
                                        "    }\n"
                                        "  }\n");
}

void SPVM_PRECOMPILE_add_set_field(SPVM_PRECOMPILE* precompile, SPVM_STRING_BUFFER* string_buffer, int32_t field_ctype_id, int32_t object_index, int32_t field_id, int32_t in_index) {
  SPVM_RUNTIME* runtime = precompile->runtime;
  
  int32_t field_class_id = SPVM_API_RUNTIME_get_field_class_id(runtime, field_id);
  int32_t field_class_name_id = SPVM_API_RUNTIME_get_class_name_id(runtime, field_class_id);
  const char* field_class_name = SPVM_API_RUNTIME_get_name(runtime, field_class_name_id);
  int32_t field_name_id = SPVM_API_RUNTIME_get_field_name_id(runtime, field_id);
  const char* field_name = SPVM_API_RUNTIME_get_name(runtime, field_name_id);

  SPVM_STRING_BUFFER_add(string_buffer, "  object = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, SPVM_PRECOMPILE_C_CTYPE_ID_OBJECT, object_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n");
  SPVM_STRING_BUFFER_add(string_buffer, "    access_field_id = env->get_field_id(env, object, \"");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
  SPVM_STRING_BUFFER_add(string_buffer, "\");\n"
                                        "  if ("
                                        "access_field_id"
                                        " < 0) {\n");
  SPVM_STRING_BUFFER_add(string_buffer, "    exception = env->new_string_nolen_raw(env, stack, \"The field \\\"");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)field_name);
  SPVM_STRING_BUFFER_add(string_buffer, "\\\" is not found\");\n"
                                        "    env->set_exception(env, stack, exception);\n"
                                        "    error = 1;\n"
                                        "  }\n"
                                        "  if (!error) {\n"
                                        "    int32_t access_field_offset"
                                        " = env->get_field_offset(env, "
                                        "access_field_id"
                                        ");\n"
                                        "    if (__builtin_expect(object == NULL, 0)) {\n"
                                        "      env->set_exception(env, stack, env->new_string_nolen_raw(env, stack, SPVM_INLINE_API_STRING_LITERALS[SPVM_INLINE_API_C_STRING_FIELD_ACCESS_INVOCANT_UNDEFINED]));\n"
                                        "      error = 1;\n"
                                        "    }\n"
                                        "    else {\n"
                                        "      *(");
  SPVM_STRING_BUFFER_add(string_buffer, (char*)SPVM_PRECOMPILE_get_ctype_name(precompile, field_ctype_id));
  SPVM_STRING_BUFFER_add(string_buffer, "*)((intptr_t)object + object_header_byte_size + access_field_offset) = ");
  SPVM_PRECOMPILE_add_operand(precompile, string_buffer, field_ctype_id, in_index);
  SPVM_STRING_BUFFER_add(string_buffer, ";\n"
                                        "    }\n"
                                        "  }\n");
}

