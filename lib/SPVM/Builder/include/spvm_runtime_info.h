#ifndef SPVM_RUNTIME_INFO_H
#define SPVM_RUNTIME_INFO_H

#include "spvm_typedecl.h"

struct spvm_runtime_info {
  // Allocator
  SPVM_ALLOCATOR* allocator;

  // Operation codes
  SPVM_OPCODE* opcodes;

  // String buffer
  const char* string_buffer;

  // String buffer length
  int32_t string_buffer_length;

  // Strings
  SPVM_RUNTIME_STRING* strings;

  // The length of strings
  int32_t strings_length;
  
  // Basic types
  SPVM_RUNTIME_BASIC_TYPE* basic_types;
  
  // The length of basic types
  int32_t basic_types_length;
  
  // Types
  SPVM_RUNTIME_TYPE* types;

  // The length of types
  int32_t types_length;
  
  // Classes
  SPVM_RUNTIME_CLASS* classes;
  
  // The length of classes
  int32_t classes_length;
  
  // Methods
  SPVM_RUNTIME_METHOD* methods;

  // The length of methods
  int32_t methods_length;

  // Method argument type ids
  int32_t* method_arg_type_ids;

  // The length of methods
  int32_t method_arg_type_ids_length;

  // Native method addresses
  void** method_native_addresses;

  // The length of native method addresses
  int32_t method_native_addresses_length;

  // Precompile method addresses
  void** method_precompile_addresses;

  // The length of precompile method addresses
  int32_t method_precompile_addresses_length;

  // Class variables
  SPVM_RUNTIME_CLASS_VAR* class_vars;

  // The length of class variables
  int32_t class_vars_length;

  // Fields
  SPVM_RUNTIME_FIELD* fields;
  
  // The length of the fields
  int32_t fields_length;
  
  // String symtable
  SPVM_HASH* string_symtable;

  // Runtime Basic type symbol table
  SPVM_HASH* basic_type_symtable;

  // Runtime class symbol table
  SPVM_HASH* class_symtable;

};

#endif
