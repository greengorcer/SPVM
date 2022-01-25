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

  compiler->const_string_symtable = SPVM_ALLOCATOR_new_hash_compile_eternal(compiler, 0);

  // Parser information
  compiler->op_use_stack = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->op_types = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->basic_types = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->basic_type_symtable = SPVM_ALLOCATOR_new_hash_compile_eternal(compiler, 0);
  compiler->methods = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->method_symtable = SPVM_ALLOCATOR_new_hash_compile_eternal(compiler, 0);
  compiler->fields = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->classes = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->used_class_symtable = SPVM_ALLOCATOR_new_hash_compile_eternal(compiler, 0);
  compiler->class_symtable = SPVM_ALLOCATOR_new_hash_compile_eternal(compiler, 0);
  compiler->class_vars = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->op_constants = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->module_dirs = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  compiler->opcode_array = SPVM_OPCODE_ARRAY_new(compiler);
  compiler->module_file_symtable = SPVM_ALLOCATOR_new_hash_compile_eternal(compiler, 0);
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

  // use Bool module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "Bool", "Bool", 0);
    SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
    SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
    SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
    SPVM_LIST_push(compiler->op_use_stack, op_use);
  }

  // use Byte module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "Byte", "Byte", 0);
    SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
    SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
    SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
    SPVM_LIST_push(compiler->op_use_stack, op_use);
  }

  // use Short module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "Short", "Short", 0);
    SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
    SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
    SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
    SPVM_LIST_push(compiler->op_use_stack, op_use);
  }

  // use Int module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "Int", "Int", 0);
    SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
    SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
    SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
    SPVM_LIST_push(compiler->op_use_stack, op_use);
  }

  // use Long module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "Long", "Long", 0);
    SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
    SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
    SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
    SPVM_LIST_push(compiler->op_use_stack, op_use);
  }

  // use Float module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "Float", "Float", 0);
    SPVM_OP* op_type = SPVM_OP_build_basic_type(compiler, op_name);
    SPVM_OP* op_use = SPVM_OP_new_op(compiler, SPVM_OP_C_ID_USE, op_name->file, op_name->line);
    SPVM_OP_build_use(compiler, op_use, op_type, NULL, 0);
    SPVM_LIST_push(compiler->op_use_stack, op_use);
  }

  // use Double module
  {
    SPVM_OP* op_name = SPVM_OP_new_op_name(compiler, "Double", "Double", 0);
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
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }
  
  // Add undef basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_UNDEF;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add void basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_VOID;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }
  
  // Add byte basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_BYTE;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add short basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_SHORT;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add int basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_INT;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add long basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_LONG;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add float basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_FLOAT;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add double basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_DOUBLE;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add string basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_STRING;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add any object basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_ANY_OBJECT;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add oarray basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_OARRAY;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }
  
  // Add Byte basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_BYTE_OBJECT;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add Short basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_SHORT_OBJECT;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add Int basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_INT_OBJECT;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add Long basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_LONG_OBJECT;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add Float basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_FLOAT_OBJECT;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add Double basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_DOUBLE_OBJECT;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }

  // Add Bool basic_type
  {
     SPVM_BASIC_TYPE* basic_type = SPVM_BASIC_TYPE_new(compiler);
     basic_type->id = SPVM_BASIC_TYPE_C_ID_BOOL_OBJECT;
     basic_type->name = (SPVM_BASIC_TYPE_C_ID_NAMES())[basic_type->id];
     SPVM_LIST_push(compiler->basic_types, basic_type);
     SPVM_HASH_insert(compiler->basic_type_symtable, basic_type->name, strlen(basic_type->name), basic_type);
  }
}

int32_t SPVM_COMPILER_get_error_count(SPVM_COMPILER* compiler) {
  
  return compiler->error_messages->length;
}

int32_t SPVM_COMPILER_compile(SPVM_COMPILER* compiler) {
  
  //yacc/bison debug mode. The default is off.
  SPVM_yydebug = 0;

#ifdef SPVM_DEBUG_YACC
  // Turn on yacc/bison debug mode
  SPVM_yydebug = 1;
#endif
  
  // Initialize added class names
  compiler->added_class_names = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);

  // Initialize error messages
  compiler->error_messages = SPVM_ALLOCATOR_new_list_compile_eternal(compiler, 0);
  
  int32_t error = 0;
  
  /* Tokenize and Parse */
  int32_t parse_start_memory_blocks_count_compile_tmp = compiler->allocator->memory_blocks_count_compile_tmp;
  int32_t parse_error_flag = SPVM_yyparse(compiler);
  assert(compiler->allocator->memory_blocks_count_compile_tmp == parse_start_memory_blocks_count_compile_tmp);
  if (parse_error_flag) {
    error = 1;
  }
  else {
    if (SPVM_COMPILER_get_error_count(compiler) > 0) {
      error = 1;
    }
    else {
      // Check syntax
      int32_t check_start_memory_blocks_count_compile_tmp = compiler->allocator->memory_blocks_count_compile_tmp;
      SPVM_OP_CHECKER_check(compiler);
      assert(compiler->allocator->memory_blocks_count_compile_tmp == check_start_memory_blocks_count_compile_tmp);
      if (SPVM_COMPILER_get_error_count(compiler) > 0) {
        error = 1;
      }
      else {
        // Build operation code
        int32_t build_opcode_array_start_memory_blocks_count_compile_tmp = compiler->allocator->memory_blocks_count_compile_tmp;
        SPVM_OPCODE_BUILDER_build_opcode_array(compiler);
        assert(compiler->allocator->memory_blocks_count_compile_tmp == build_opcode_array_start_memory_blocks_count_compile_tmp);
        if (SPVM_COMPILER_get_error_count(compiler) > 0) {
          error = 1;
        }
      }
    }
  }
  return error;
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
  
  fprintf(stderr, "[CompileError]%s", message);
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
      if (!method->is_class_method && arg_index == 0) {
        // Nothing
        continue;
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
        // Nothing
        continue;
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
