#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "spvm_compiler.h"
#include "spvm_type.h"
#include "spvm_package.h"
#include "spvm_type.h"
#include "spvm_op.h"
#include "spvm_hash.h"
#include "spvm_list.h"
#include "spvm_util_allocator.h"
#include "spvm_compiler_allocator.h"
#include "spvm_yacc_util.h"
#include "spvm_list.h"
#include "spvm_opcode_array.h"
#include "spvm_sub.h"
#include "spvm_sub.h"
#include "spvm_field.h"
#include "spvm_package_var.h"
#include "spvm_native.h"
#include "spvm_opcode.h"
#include "spvm_basic_type.h"
#include "spvm_use.h"
#include "spvm_op_checker.h"
#include "spvm_opcode_builder.h"
#include "spvm_object.h"
#include "spvm_my.h"
#include "spvm_string_buffer.h"

SPVM_COMPILER* SPVM_COMPILER_new() {
  SPVM_COMPILER* compiler = SPVM_UTIL_ALLOCATOR_safe_malloc_zero(sizeof(SPVM_COMPILER));
  
  // Allocator
  compiler->allocator = SPVM_COMPILER_ALLOCATOR_new(compiler);

  compiler->bufptr = "";

  compiler->name_symtable = SPVM_COMPILER_ALLOCATOR_alloc_hash(compiler, 0);

  // Parser information
  compiler->op_use_stack = SPVM_COMPILER_ALLOCATOR_alloc_list(compiler, 0);
  compiler->op_types = SPVM_COMPILER_ALLOCATOR_alloc_list(compiler, 0);
  compiler->basic_types = SPVM_COMPILER_ALLOCATOR_alloc_list(compiler, 0);
  compiler->basic_type_symtable = SPVM_COMPILER_ALLOCATOR_alloc_hash(compiler, 0);
  compiler->subs = SPVM_COMPILER_ALLOCATOR_alloc_list(compiler, 0);
  compiler->sub_symtable = SPVM_COMPILER_ALLOCATOR_alloc_hash(compiler, 0);
  compiler->fields = SPVM_COMPILER_ALLOCATOR_alloc_list(compiler, 0);
  compiler->packages = SPVM_COMPILER_ALLOCATOR_alloc_list(compiler, 0);
  compiler->package_symtable = SPVM_COMPILER_ALLOCATOR_alloc_hash(compiler, 0);
  compiler->package_vars = SPVM_COMPILER_ALLOCATOR_alloc_list(compiler, 0);
  compiler->op_constants = SPVM_COMPILER_ALLOCATOR_alloc_list(compiler, 0);
  compiler->module_include_pathes = SPVM_COMPILER_ALLOCATOR_alloc_list(compiler, 0);
  compiler->opcode_array = SPVM_OPCODE_ARRAY_new(compiler);
  compiler->module_file_symtable = SPVM_COMPILER_ALLOCATOR_alloc_hash(compiler, 0);
  compiler->module_rel_file_symtable = SPVM_COMPILER_ALLOCATOR_alloc_hash(compiler, 0);
  compiler->module_source_symtable = SPVM_COMPILER_ALLOCATOR_alloc_hash(compiler, 0);
  compiler->added_packages = SPVM_LIST_new(0);

  // Add basic types
  SPVM_COMPILER_add_basic_types(compiler);

  // use SPVM::Byte module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "SPVM::Byte", "SPVM::Byte", 0);
    SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
    SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
    SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
    SPVM_LIST_push(compiler->op_use_stack, op_use);
  }

  // use SPVM::Short module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "SPVM::Short", "SPVM::Short", 0);
    SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
    SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
    SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
    SPVM_LIST_push(compiler->op_use_stack, op_use);
  }

  // use SPVM::Int module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "SPVM::Int", "SPVM::Int", 0);
    SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
    SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
    SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
    SPVM_LIST_push(compiler->op_use_stack, op_use);
  }

  // use SPVM::Long module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "SPVM::Long", "SPVM::Long", 0);
    SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
    SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
    SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
    SPVM_LIST_push(compiler->op_use_stack, op_use);
  }

  // use SPVM::Float module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "SPVM::Float", "SPVM::Float", 0);
    SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
    SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
    SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
    SPVM_LIST_push(compiler->op_use_stack, op_use);
  }

  // use SPVM::Double module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "SPVM::Double", "SPVM::Double", 0);
    SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
    SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
    SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
    SPVM_LIST_push(compiler->op_use_stack, op_use);
  }
  return compiler;
}

void SPVM_COMPILER_add_basic_types(SPVM_COMPILER* compiler) {
  // Add unknown basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_UNKNOWN;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }
  
  // Add undef basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_UNDEF;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add void basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_VOID;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }
  
  // Add byte basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_BYTE;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add short basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_SHORT;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add int basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_INT;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add long basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_LONG;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add float basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_FLOAT;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add double basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_DOUBLE;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add string basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_STRING;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add any object basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_ANY_OBJECT;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add oarray basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_OARRAY;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }
  
  // Add SPVM::Byte basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_BYTE_OBJECT;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add SPVM::Short basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_SHORT_OBJECT;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add SPVM::Int basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_INT_OBJECT;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add SPVM::Long basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_LONG_OBJECT;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add SPVM::Float basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_FLOAT_OBJECT;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add SPVM::Double basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_DOUBLE_OBJECT;
     basic_type->name = SPVM_BASIC_TYPE_C_ID_NAMES[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }
}

void SPVM_COMPILER_compile(SPVM_COMPILER* compiler) {

  // If this is set to 1, you can see yacc parsing result
#ifdef SPVM_DEBUG_YACC
  SPVM_yydebug = 1;
#else
  SPVM_yydebug = 0;
#endif
  
  // Initialize added package names
  if (compiler->tmp_added_package_names) {
    SPVM_LIST_free(compiler->tmp_added_package_names);
  }
  compiler->tmp_added_package_names = SPVM_LIST_new(0);

  /* Parse */
  int32_t parse_error_flag = SPVM_yyparse(compiler);
  if (parse_error_flag) {
    return;
  }
  if (compiler->error_count > 0) {
    return;
  }
  
  // Check syntax
  SPVM_OP_CHECKER_check(compiler);
  if (compiler->error_count > 0) {
    return;
  }

  // Build operation code
  SPVM_OPCODE_BUILDER_build_opcode_array(compiler);
  if (compiler->error_count > 0) {
    return;
  }
  
  // Add added package names if compile is success
  SPVM_LIST_free(compiler->added_packages);
  compiler->added_packages = SPVM_LIST_new(0);
  for (int32_t i = 0; i < compiler->tmp_added_package_names->length; i++) {
    const char* package_name = (const char*)SPVM_LIST_fetch(compiler->tmp_added_package_names, i);
    SPVM_PACKAGE* pakcage = SPVM_HASH_fetch(compiler->package_symtable, package_name, strlen(package_name));
    SPVM_LIST_push(compiler->added_packages, pakcage);
  }
}

void SPVM_COMPILER_error(SPVM_COMPILER* compiler, const char* message_template, ...) {
  
  int32_t message_length = 0;
  
  // Message template
  int32_t message_template_length = (int32_t)strlen(message_template);
  
  va_list args;
  va_start(args, message_template);

  message_length += message_template_length;
  
  // Argument count
  char* found_ptr = (char*)message_template;
  while (1) {
    found_ptr = strchr(found_ptr, '%');
    if (found_ptr) {
      if (*(found_ptr + 1) == 'c') {
        int arg = va_arg(args, int);
        message_length++;
      }
      else if (*(found_ptr + 1) == 's') {
        char* arg = va_arg(args, char*);
        message_length += strlen(arg);
      }
      else if (*(found_ptr + 1) == 'd') {
        (void) va_arg(args, int);
        message_length += 30;
      }
      else if (*(found_ptr + 1) == '%') {
        // Nothing
        found_ptr++;
      }
      else {
        assert(0);
      }
      found_ptr++;
    }
    else {
      break;
    }
  }
  va_end(args);
  
  char* message = SPVM_COMPILER_ALLOCATOR_safe_malloc_zero(compiler, message_length + 1);
  
  va_start(args, message_template);
  vsprintf(message, message_template, args);
  va_end(args);

  compiler->error_count++;
  
  fprintf(stderr, "%s", message);
}

const char* SPVM_COMPILER_create_sub_signature(SPVM_COMPILER* compiler, SPVM_SUB* sub) {
  
  int32_t length = 0;
  
  // Calcurate signature length
  {
    // Return type basic type
    length += strlen(sub->return_type->basic_type->name);
    
    // Return type dimension
    length += sub->return_type->dimension * 2;
    
    // (
    length += 1;
    
    int32_t arg_index;
    for (arg_index = 0; arg_index < sub->args->length; arg_index++) {
      if (sub->call_type_id == SPVM_SUB_C_CALL_TYPE_ID_METHOD && arg_index == 0) {
        // self
        length += 4;
      }
      else {
        SPVM_MY* arg_my_sub = SPVM_LIST_fetch(sub->args, arg_index);
        SPVM_TYPE* type_arg_sub = SPVM_OP_get_type(compiler, arg_my_sub->op_my);
        
        // Ref
        if (SPVM_TYPE_is_ref_type(compiler, type_arg_sub->basic_type->id, type_arg_sub->dimension, type_arg_sub->flag)) {
          length += 1;
        }
        
        // TYPE
        length += strlen(type_arg_sub->basic_type->name);
        
        // Dimension
        length += type_arg_sub->dimension * 2;
      }
      // ,
      if (arg_index != sub->args->length - 1) {
        length += 1;
      }
    }
    
    // )
    length += 1;
  }
  
  char* sub_signature = SPVM_COMPILER_ALLOCATOR_safe_malloc_zero(compiler, length + 1);
  
  // Calcurate sub signature length
  char* bufptr = sub_signature;
  {
    // Return type
    memcpy(bufptr, sub->return_type->basic_type->name, strlen(sub->return_type->basic_type->name));
    bufptr += strlen(sub->return_type->basic_type->name);
    
    int32_t dim_index;
    for (dim_index = 0; dim_index < sub->return_type->dimension; dim_index++) {
      memcpy(bufptr, "[]", 2);
      bufptr += 2;
    }
    
    // (
    *bufptr = '(';
    bufptr += 1;
    
    int32_t arg_index;
    for (arg_index = 0; arg_index < sub->args->length; arg_index++) {
      // self
      if (sub->call_type_id == SPVM_SUB_C_CALL_TYPE_ID_METHOD && arg_index == 0) {
        memcpy(bufptr, "self", 4);
        bufptr += 4;
      }
      else {
        SPVM_MY* arg_my_sub = SPVM_LIST_fetch(sub->args, arg_index);
        SPVM_TYPE* type_arg_sub = SPVM_OP_get_type(compiler, arg_my_sub->op_my);
        
        // Ref
        if (SPVM_TYPE_is_ref_type(compiler, type_arg_sub->basic_type->id, type_arg_sub->dimension, type_arg_sub->flag)) {
          *bufptr = '&';
          bufptr += 1;
        }
        
        // TYPE
        memcpy(bufptr, type_arg_sub->basic_type->name, strlen(type_arg_sub->basic_type->name));
        bufptr += strlen(type_arg_sub->basic_type->name);

        int32_t dim_index;
        for (dim_index = 0; dim_index < type_arg_sub->dimension; dim_index++) {
          memcpy(bufptr, "[]", 2);
          bufptr += 2;
        }
      }

      // ,
      if (arg_index != sub->args->length - 1) {
        memcpy(bufptr, ",", 1);
        bufptr += 1;
      }
    }
    
    // )
    memcpy(bufptr, ")", 1);
    bufptr += 1;
  }

  return sub_signature;
}

const char* SPVM_COMPILER_create_field_signature(SPVM_COMPILER* compiler, SPVM_FIELD* field) {
  
  int32_t length = 0;
  
  // Calcurate signature length
  {
    // Basic type
    length += strlen(field->type->basic_type->name);
    
    // Type dimension
    length += field->type->dimension * 2;
  }
  
  char* field_signature = SPVM_COMPILER_ALLOCATOR_safe_malloc_zero(compiler, length + 1);
  
  // Calcurate field signature length
  char* bufptr = field_signature;
  {
    // Basic type
    memcpy(bufptr, field->type->basic_type->name, strlen(field->type->basic_type->name));
    bufptr += strlen(field->type->basic_type->name);
    
    // Type dimension
    int32_t dim_index;
    for (dim_index = 0; dim_index < field->type->dimension; dim_index++) {
      memcpy(bufptr, "[]", 2);
      bufptr += 2;
    }
  }

  return field_signature;
}

const char* SPVM_COMPILER_create_package_var_signature(SPVM_COMPILER* compiler, SPVM_PACKAGE_VAR* package_var) {
  
  int32_t length = 0;
  
  // Calcurate signature length
  {
    // Basic type
    length += strlen(package_var->type->basic_type->name);
    
    // Type dimension
    length += package_var->type->dimension * 2;
  }
  
  char* package_var_signature = SPVM_COMPILER_ALLOCATOR_safe_malloc_zero(compiler, length + 1);
  
  // Calcurate package_var signature length
  char* bufptr = package_var_signature;
  {
    // Basic type
    memcpy(bufptr, package_var->type->basic_type->name, strlen(package_var->type->basic_type->name));
    bufptr += strlen(package_var->type->basic_type->name);
    
    // Type dimension
    int32_t dim_index;
    for (dim_index = 0; dim_index < package_var->type->dimension; dim_index++) {
      memcpy(bufptr, "[]", 2);
      bufptr += 2;
    }
  }

  return package_var_signature;
}

void SPVM_COMPILER_free(SPVM_COMPILER* compiler) {
  
  // Free allocator
  SPVM_COMPILER_ALLOCATOR_free(compiler);
  
  // Free opcode array
  SPVM_OPCODE_ARRAY_free(compiler, compiler->opcode_array);
  
  free(compiler);
}
