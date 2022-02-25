#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "spvm_compiler.h"
#include "spvm_type.h"
#include "spvm_class.h"
#include "spvm_type.h"
#include "spvm_op.h"
#include "spvm_hash.h"
#include "spvm_list.h"
#include "spvm_allocator.h"
#include "spvm_yacc_util.h"
#include "spvm_list.h"
#include "spvm_opcode_array.h"
#include "spvm_method.h"
#include "spvm_method.h"
#include "spvm_field.h"
#include "spvm_class_var.h"
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
  SPVM_COMPILER* compiler = SPVM_ALLOCATOR_new_block_unmanaged(sizeof(SPVM_COMPILER));
  
  // Allocator
  SPVM_ALLOCATOR* allocator = SPVM_ALLOCATOR_new();
  compiler->allocator = allocator;
  SPVM_ALLOCATOR_init(compiler);
  
  compiler->bufptr = "";

  compiler->name_symtable = SPVM_ALLOCATOR_new_hash_compile_eternal(compiler, 0);

  // Eternal information
  compiler->module_dirs = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->basic_types = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->basic_type_symtable = SPVM_ALLOCATOR_new_hash_compile_eternal(compiler, 0);
  compiler->methods = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->fields = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->classes = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->class_symtable = SPVM_ALLOCATOR_new_hash_compile_eternal(compiler, 0);
  compiler->class_vars = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->opcode_array = SPVM_OPCODE_ARRAY_new(compiler);
  compiler->module_source_symtable = SPVM_ALLOCATOR_new_hash_compile_eternal(compiler, 0);

  // Add basic types
  SPVM_COMPILER_add_basic_types(compiler);

  // Add Bool source
  const char* spvm_bool_module_source = "class Bool {\n  INIT {\n    $TRUE = new Bool;\n    $TRUE->{value} = 1;\n    $FALSE = new Bool;\n    $FALSE->{value} = 0;\n  }\n  \n  our $TRUE : ro Bool;\n  our $FALSE : ro Bool;\n  has value : ro int;\n}";
  SPVM_HASH_insert(compiler->module_source_symtable, "Bool", strlen("Bool"), (void*)spvm_bool_module_source);

  // Add Byte source
  const char* spvm_byte_module_source = "class Byte {\n  has value : ro byte;\n  static method new : Byte ($value : byte) {\n    my $self = new Byte;\n    $self->{value} = $value;\n    return $self;\n  }\n}";
  SPVM_HASH_insert(compiler->module_source_symtable, "Byte", strlen("Byte"), (void*)spvm_byte_module_source);

  // Add Short source
  const char* spvm_short_module_source = "class Short {\n  has value : ro short;\n  static method new : Short ($value : short) {\n    my $self = new Short;\n    $self->{value} = $value;\n    return $self;\n  }\n}";
  SPVM_HASH_insert(compiler->module_source_symtable, "Short", strlen("Short"), (void*)spvm_short_module_source);

  // Add Int source
  const char* spvm_int_module_source = "class Int {\n  has value : ro int;\n  static method new : Int ($value : int) {\n    my $self = new Int;\n    $self->{value} = $value;\n    return $self;\n  }\n}";
  SPVM_HASH_insert(compiler->module_source_symtable, "Int", strlen("Int"), (void*)spvm_int_module_source);

  // Add Long source
  const char* spvm_long_module_source = "class Long {\n  has value : ro long;\n  static method new : Long ($value : long) {\n    my $self = new Long;\n    $self->{value} = $value;\n    return $self;\n  }\n}";
  SPVM_HASH_insert(compiler->module_source_symtable, "Long", strlen("Long"), (void*)spvm_long_module_source);

  // Add Float source
  const char* spvm_float_module_source = "class Float {\n  has value : ro float;\n  static method new : Float ($value : float) {\n    my $self = new Float;\n    $self->{value} = $value;\n    return $self;\n  }\n}";
  SPVM_HASH_insert(compiler->module_source_symtable, "Float", strlen("Float"), (void*)spvm_float_module_source);

  // Add Double source
  const char* spvm_double_module_source = "class Double {\n  has value : ro double;\n  static method new : Double ($value : double) {\n    my $self = new Double;\n    $self->{value} = $value;\n    return $self;\n  }\n}";
  SPVM_HASH_insert(compiler->module_source_symtable, "Double", strlen("Double"), (void*)spvm_double_module_source);

  return compiler;
}

void SPVM_COMPILER_add_basic_type(SPVM_COMPILER* compiler, int32_t basic_type_id) {
   SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
   basic_type->id = basic_type_id;
   basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
   SPVM_LIST_push(compiler->basic_types, basic_type);
   SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
}

void SPVM_COMPILER_add_basic_types(SPVM_COMPILER* compiler) {
  // Add basic_types
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_UNKNOWN);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_UNDEF);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_VOID);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_BYTE);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_SHORT);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_INT);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_LONG);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_FLOAT);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_DOUBLE);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_STRING);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_ANY_OBJECT);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_OARRAY);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_BYTE_OBJECT);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_SHORT_OBJECT);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_INT_OBJECT);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_LONG_OBJECT);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_FLOAT_OBJECT);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_DOUBLE_OBJECT);
  SPVM_COMPILER_add_basic_type(compiler, SPVM_BASIC_TYPE_C_ID_BOOL_OBJECT);
}

int32_t SPVM_COMPILER_get_error_messages_length(SPVM_COMPILER* compiler) {
  
  return compiler->error_messages->length;
}

const char* SPVM_COMPILER_get_error_message(SPVM_COMPILER* compiler, int32_t index) {
  
  const char* error_message = (const char*)SPVM_LIST_fetch(compiler->error_messages, index);
  
  return error_message;
}

void SPVM_COMPILER_print_error_messages(SPVM_COMPILER* compiler, FILE* fh) {
  
  for (int32_t i = 0; i < compiler->error_messages->length; i++) {
    const char* error_message = (const char*)SPVM_LIST_fetch(compiler->error_messages, i);
    fprintf(fh, "[CompileError]%s\n", error_message);
  }
}

void SPVM_COMPILER_use(SPVM_COMPILER* compiler, const char* class_name, const char* file, int32_t line) {
  SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, class_name, file, line);
  SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
  SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
  SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
  SPVM_LIST_push(compiler->op_use_stack, op_use);
}

int32_t SPVM_COMPILER_compile_spvm(SPVM_COMPILER* compiler, const char* class_name) {

  compiler->cur_class_base = compiler->classes->length;

  const char* start_file = compiler->start_file;
  int32_t start_line = compiler->start_line;
  
  //yacc/bison debug mode. The default is off.
  SPVM_yydebug = 0;

#ifdef SPVM_DEBUG_YACC
  // Turn on yacc/bison debug mode
  SPVM_yydebug = 1;
#endif

  compiler->parse_start = 1;

  // Initialize added class names
  compiler->added_class_names = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);

  // Initialize error messages
  compiler->error_messages = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  
  int32_t error_code = 0;

  int32_t compile_start_memory_blocks_count_compile_tmp = compiler->allocator->memory_blocks_count_compile_tmp;

  compiler->op_use_stack = SPVM_LIST_new(compiler, 0, 0, NULL);
  compiler->op_types = SPVM_LIST_new(compiler, 0, 0, NULL);
  compiler->used_class_symtable = SPVM_HASH_new(compiler, 0, 0, NULL);
  for (int32_t i = 0; i < compiler->classes->length; i++) {
    SPVM_CLASS* class = SPVM_LIST_fetch(compiler->classes, i);
    const char* class_name = class->name;
    SPVM_HASH_insert(compiler->used_class_symtable, class_name, strlen(class_name), (void*)class_name);
  }
  
  // Use automatically loaded modules
  SPVM_COMPILER_use(compiler, "Bool", "Bool", 0);
  SPVM_COMPILER_use(compiler, "Byte", "Byte", 0);
  SPVM_COMPILER_use(compiler, "Short", "Short", 0);
  SPVM_COMPILER_use(compiler, "Int", "Int", 0);
  SPVM_COMPILER_use(compiler, "Long", "Long", 0);
  SPVM_COMPILER_use(compiler, "Float", "Float", 0);
  SPVM_COMPILER_use(compiler, "Double", "Double", 0);
  
  // Use the module that is specified at the argument
  SPVM_COMPILER_use(compiler, class_name, start_file, start_line);

  /* Tokenize and Parse */
  int32_t parse_start_memory_blocks_count_compile_tmp = compiler->allocator->memory_blocks_count_compile_tmp;
  int32_t parse_error_flag = SPVM_yyparse(compiler);
  assert(compiler->allocator->memory_blocks_count_compile_tmp == parse_start_memory_blocks_count_compile_tmp);
  if (parse_error_flag) {
    error_code = 1;
  }
  else {
    if (SPVM_COMPILER_get_error_messages_length(compiler) > 0) {
      error_code = 2;
    }
    else {
      // Check syntax
      int32_t check_start_memory_blocks_count_compile_tmp = compiler->allocator->memory_blocks_count_compile_tmp;
      SPVM_OP_CHECKER_check(compiler);
      assert(compiler->allocator->memory_blocks_count_compile_tmp == check_start_memory_blocks_count_compile_tmp);
      if (SPVM_COMPILER_get_error_messages_length(compiler) > 0) {
        error_code = 3;
      }
      else {
        // Build operation code
        int32_t build_opcode_array_start_memory_blocks_count_compile_tmp = compiler->allocator->memory_blocks_count_compile_tmp;
        SPVM_OPCODE_BUILDER_build_opcode_array(compiler);
        assert(compiler->allocator->memory_blocks_count_compile_tmp == build_opcode_array_start_memory_blocks_count_compile_tmp);
        if (SPVM_COMPILER_get_error_messages_length(compiler) > 0) {
          error_code = 4;
        }
      }
    }
  }
  
  // Free
  SPVM_LIST_free(compiler->op_use_stack);
  compiler->op_use_stack = NULL;
  
  SPVM_LIST_free(compiler->op_types);
  compiler->op_types = NULL;

  SPVM_HASH_free(compiler->used_class_symtable);
  compiler->used_class_symtable = NULL;

  assert(compiler->allocator->memory_blocks_count_compile_tmp == compile_start_memory_blocks_count_compile_tmp);

  return error_code;
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
  
  char* message = SPVM_ALLOCATOR_new_block_compile_eternal(compiler, message_length + 1);
  
  va_start(args, message_template);
  vsprintf(message, message_template, args);
  va_end(args);

  SPVM_LIST_push(compiler->error_messages, message);
}

const char* SPVM_COMPILER_create_method_signature(SPVM_COMPILER* compiler, SPVM_METHOD* method) {
  
  int32_t length = 0;
  
  // Calcurate signature length
  {
    // Return type basic type
    length += strlen(method->return_type->basic_type->name);
    
    // Return type dimension
    length += method->return_type->dimension * 2;
    
    // (
    length += 1;
    
    int32_t arg_index;
    for (arg_index = 0; arg_index < method->args->length; arg_index++) {
      // self
      if (!method->is_class_method && arg_index == 0) {
        length += 4;
      }
      else {
        SPVM_MY* arg_my_method = SPVM_LIST_fetch(method->args, arg_index);
        SPVM_TYPE* type_arg_method = SPVM_OP_get_type(compiler, arg_my_method->op_my);
        
        // Ref
        if (SPVM_TYPE_is_ref_type(compiler, type_arg_method->basic_type->id, type_arg_method->dimension, type_arg_method->flag)) {
          length += 1;
        }
        
        // TYPE
        length += strlen(type_arg_method->basic_type->name);
        
        // Dimension
        length += type_arg_method->dimension * 2;
      }
      // ,
      if (arg_index != method->args->length - 1) {
        length += 1;
      }
    }
    
    // )
    length += 1;
  }
  
  char* method_signature = SPVM_ALLOCATOR_new_block_compile_eternal(compiler, length + 1);
  
  // Calcurate method signature length
  char* bufptr = method_signature;
  {
    // Return type
    memcpy(bufptr, method->return_type->basic_type->name, strlen(method->return_type->basic_type->name));
    bufptr += strlen(method->return_type->basic_type->name);
    
    int32_t dim_index;
    for (dim_index = 0; dim_index < method->return_type->dimension; dim_index++) {
      memcpy(bufptr, "[]", 2);
      bufptr += 2;
    }
    
    // (
    *bufptr = '(';
    bufptr += 1;
    
    int32_t arg_index;
    for (arg_index = 0; arg_index < method->args->length; arg_index++) {
      // self
      if (!method->is_class_method && arg_index == 0) {
        memcpy(bufptr, "self", 4);
        bufptr += 4;
      }
      else {
        SPVM_MY* arg_my_method = SPVM_LIST_fetch(method->args, arg_index);
        SPVM_TYPE* type_arg_method = SPVM_OP_get_type(compiler, arg_my_method->op_my);
        
        // Ref
        if (SPVM_TYPE_is_ref_type(compiler, type_arg_method->basic_type->id, type_arg_method->dimension, type_arg_method->flag)) {
          *bufptr = '&';
          bufptr += 1;
        }
        
        // TYPE
        memcpy(bufptr, type_arg_method->basic_type->name, strlen(type_arg_method->basic_type->name));
        bufptr += strlen(type_arg_method->basic_type->name);

        int32_t dim_index;
        for (dim_index = 0; dim_index < type_arg_method->dimension; dim_index++) {
          memcpy(bufptr, "[]", 2);
          bufptr += 2;
        }
      }

      // ,
      if (arg_index != method->args->length - 1) {
        memcpy(bufptr, ",", 1);
        bufptr += 1;
      }
    }
    
    // )
    memcpy(bufptr, ")", 1);
    bufptr += 1;
  }

  return method_signature;
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
  
  char* field_signature = SPVM_ALLOCATOR_new_block_compile_eternal(compiler, length + 1);
  
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

const char* SPVM_COMPILER_create_class_var_signature(SPVM_COMPILER* compiler, SPVM_CLASS_VAR* class_var) {
  
  int32_t length = 0;
  
  // Calcurate signature length
  {
    // Basic type
    length += strlen(class_var->type->basic_type->name);
    
    // Type dimension
    length += class_var->type->dimension * 2;
  }
  
  char* class_var_signature = SPVM_ALLOCATOR_new_block_compile_eternal(compiler, length + 1);
  
  // Calcurate class_var signature length
  char* bufptr = class_var_signature;
  {
    // Basic type
    memcpy(bufptr, class_var->type->basic_type->name, strlen(class_var->type->basic_type->name));
    bufptr += strlen(class_var->type->basic_type->name);
    
    // Type dimension
    int32_t dim_index;
    for (dim_index = 0; dim_index < class_var->type->dimension; dim_index++) {
      memcpy(bufptr, "[]", 2);
      bufptr += 2;
    }
  }

  return class_var_signature;
}

void SPVM_COMPILER_free(SPVM_COMPILER* compiler) {

  // Free opcode array
  SPVM_OPCODE_ARRAY_free(compiler, compiler->opcode_array);

  // Free allocator
  SPVM_ALLOCATOR_free(compiler);
  
  SPVM_ALLOCATOR_free_block_unmanaged(compiler);
}
