#ifndef SPVM_RUNTIME_INFO_H
#define SPVM_RUNTIME_INFO_H

#include "spvm_typedecl.h"

struct spvm_runtime_info {
  // Operation codes
  SPVM_OPCODE* opcodes;

  // String buffer
  const char* string_buffer;

  // Strings - string literals and symbol names
  SPVM_RUNTIME_STRING* strings;
  
  // String symtable
  SPVM_HASH* string_symtable;

  // Runtime Basic types
  SPVM_LIST* basic_types;
  
  // Runtime Basic type symbol table
  SPVM_HASH* basic_type_symtable;

  // Runtime Basic types
  SPVM_LIST* types;
  
  // Runtime Basic type symbol table
  SPVM_HASH* type_symtable;

  // Runtime class types
  SPVM_LIST* classes;
  
  // Runtime class symbol table
  SPVM_HASH* class_symtable;

  // Runtime methods
  SPVM_LIST* methods;

  // Runtime class variables
  SPVM_LIST* class_vars;

  // Runtime fields
  SPVM_LIST* fields;
  
  // Runtime methods of a class
  SPVM_RUNTIME_METHODS_OF_CLASS* methods_of_class;

  // Runtime fields of a class
  SPVM_RUNTIME_FIELDS_OF_CLASS* fields_of_class;

  // Runtime class variables of a class
  SPVM_RUNTIME_CLASS_VARS_OF_CLASS* class_vars_of_class;
  
  SPVM_ALLOCATOR* allocator;
};

#endif
