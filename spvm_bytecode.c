#include "spvm_bytecode.h"

































































































const char* const SPVM_BYTECODE_C_CODE_NAMES[] = {
  "NOP",
  "UNDEF",
  "LOAD_CONSTANT",
  "LOAD_CONSTANT2",
  "ADD_BYTE",
  "ADD_SHORT",
  "ADD_INT",
  "ADD_LONG",
  "ADD_FLOAT",
  "ADD_DOUBLE",
  "SUBTRACT_BYTE",
  "SUBTRACT_SHORT",
  "SUBTRACT_INT",
  "SUBTRACT_LONG",
  "SUBTRACT_FLOAT",
  "SUBTRACT_DOUBLE",
  "MULTIPLY_BYTE",
  "MULTIPLY_SHORT",
  "MULTIPLY_INT",
  "MULTIPLY_LONG",
  "MULTIPLY_FLOAT",
  "MULTIPLY_DOUBLE",
  "DIVIDE_BYTE",
  "DIVIDE_SHORT",
  "DIVIDE_INT",
  "DIVIDE_LONG",
  "DIVIDE_FLOAT",
  "DIVIDE_DOUBLE",
  "REMAINDER_BYTE",
  "REMAINDER_SHORT",
  "REMAINDER_INT",
  "REMAINDER_LONG",
  "REMAINDER_FLOAT",
  "REMAINDER_DOUBLE",
  "NEGATE_BYTE",
  "NEGATE_SHORT",
  "NEGATE_INT",
  "NEGATE_LONG",
  "NEGATE_FLOAT",
  "NEGATE_DOUBLE",
  "LEFT_SHIFT_BYTE",
  "LEFT_SHIFT_SHORT",
  "LEFT_SHIFT_INT",
  "LEFT_SHIFT_LONG",
  "RIGHT_SHIFT_BYTE",
  "RIGHT_SHIFT_SHORT",
  "RIGHT_SHIFT_INT",
  "RIGHT_SHIFT_LONG",
  "RIGHT_SHIFT_UNSIGNED_BYTE",
  "RIGHT_SHIFT_UNSIGNED_SHORT",
  "RIGHT_SHIFT_UNSIGNED_INT",
  "RIGHT_SHIFT_UNSIGNED_LONG",
  "BIT_AND_BYTE",
  "BIT_AND_SHORT",
  "BIT_AND_INT",
  "BIT_AND_LONG",
  "BIT_OR_BYTE ",
  "BIT_OR_SHORT ",
  "BIT_OR_INT",
  "BIT_OR_LONG",
  "BIT_XOR_BYTE",
  "BIT_XOR_SHORT",
  "BIT_XOR_INT",
  "BIT_XOR_LONG",
  "CONVERT_BYTE_TO_SHORT",
  "CONVERT_BYTE_TO_INT",
  "CONVERT_BYTE_TO_LONG",
  "CONVERT_BYTE_TO_FLOAT",
  "CONVERT_BYTE_TO_DOUBLE",
  "CONVERT_SHORT_TO_BYTE",
  "CONVERT_SHORT_TO_INT",
  "CONVERT_SHORT_TO_LONG",
  "CONVERT_SHORT_TO_FLOAT",
  "CONVERT_SHORT_TO_DOUBLE",
  "CONVERT_INT_TO_BYTE",
  "CONVERT_INT_TO_SHORT",
  "CONVERT_INT_TO_LONG",
  "CONVERT_INT_TO_FLOAT",
  "CONVERT_INT_TO_DOUBLE",
  "CONVERT_LONG_TO_BYTE",
  "CONVERT_LONG_TO_SHORT",
  "CONVERT_LONG_TO_INT",
  "CONVERT_LONG_TO_FLOAT",
  "CONVERT_LONG_TO_DOUBLE",
  "CONVERT_FLOAT_TO_BYTE",
  "CONVERT_FLOAT_TO_SHORT",
  "CONVERT_FLOAT_TO_INT",
  "CONVERT_FLOAT_TO_LONG",
  "CONVERT_FLOAT_TO_DOUBLE",
  "CONVERT_DOUBLE_TO_BYTE",
  "CONVERT_DOUBLE_TO_SHORT",
  "CONVERT_DOUBLE_TO_INT",
  "CONVERT_DOUBLE_TO_LONG",
  "CONVERT_DOUBLE_TO_FLOAT",
  "INC_BYTE",
  "INC_SHORT",
  "INC_INT",
  "INC_LONG",
  "LOAD",
  "STORE",
  "STORE_OBJECT",
  "POP",
  "NEW_OBJECT",
  "NEW_STRING",
  "NEW_OBJECT_ARRAY",
  "ARRAY_LOAD_BYTE",
  "ARRAY_LOAD_SHORT",
  "ARRAY_LOAD_INT",
  "ARRAY_LOAD_LONG",
  "ARRAY_LOAD_FLOAT",
  "ARRAY_LOAD_DOUBLE",
  "ARRAY_LOAD_OBJECT",
  "ARRAY_STORE_BYTE",
  "ARRAY_STORE_SHORT",
  "ARRAY_STORE_INT",
  "ARRAY_STORE_LONG",
  "ARRAY_STORE_FLOAT",
  "ARRAY_STORE_DOUBLE",
  "ARRAY_STORE_OBJECT",
  "ARRAY_LENGTH",
  "GET_FIELD_BYTE",
  "GET_FIELD_SHORT",
  "GET_FIELD_INT",
  "GET_FIELD_LONG",
  "GET_FIELD_FLOAT",
  "GET_FIELD_DOUBLE",
  "GET_FIELD_OBJECT",
  "SET_FIELD_BYTE",
  "SET_FIELD_SHORT",
  "SET_FIELD_INT",
  "SET_FIELD_LONG",
  "SET_FIELD_FLOAT",
  "SET_FIELD_DOUBLE",
  "SET_FIELD_OBJECT",
  "IF_EQ_ZERO",
  "IF_NE_ZERO",
  "TABLE_SWITCH",
  "LOOKUP_SWITCH",
  "GOTO",
  "CALL_SUB",
  "RETURN_VOID",
  "RETURN_BYTE",
  "RETURN_SHORT",
  "RETURN_INT",
  "RETURN_LONG",
  "RETURN_FLOAT",
  "RETURN_DOUBLE",
  "RETURN_OBJECT",
  "CROAK",
  "LOAD_EXCEPTION",
  "STORE_EXCEPTION",
  "CURRENT_LINE",
  "WEAKEN_FIELD",
  "NEW_BYTE_ARRAY",
  "NEW_SHORT_ARRAY",
  "NEW_INT_ARRAY",
  "NEW_LONG_ARRAY",
  "NEW_FLOAT_ARRAY",
  "NEW_DOUBLE_ARRAY",
  "CONCAT_STRING_STRING",
  "CONCAT_STRING_BYTE",
  "CONCAT_STRING_SHORT",
  "CONCAT_STRING_INT",
  "CONCAT_STRING_LONG",
  "CONCAT_STRING_FLOAT",
  "CONCAT_STRING_DOUBLE",
  "PUSH_CATCH_EXCEPTION",
  "POP_CATCH_EXCEPTION",
  "LOAD_PACKAGE_VAR",
  "STORE_PACKAGE_VAR",
  "STORE_PACKAGE_VAR_OBJECT",
  "EQ_BYTE",
  "EQ_SHORT",
  "EQ_INT",
  "EQ_LONG",
  "EQ_FLOAT",
  "EQ_DOUBLE",
  "EQ_OBJECT",
  "NE_BYTE",
  "NE_SHORT",
  "NE_INT",
  "NE_LONG",
  "NE_FLOAT",
  "NE_DOUBLE",
  "NE_OBJECT",
  "GT_BYTE",
  "GT_SHORT",
  "GT_INT",
  "GT_LONG",
  "GT_FLOAT",
  "GT_DOUBLE",
  "GE_BYTE",
  "GE_SHORT",
  "GE_INT",
  "GE_LONG",
  "GE_FLOAT",
  "GE_DOUBLE",
  "LT_BYTE",
  "LT_SHORT",
  "LT_INT",
  "LT_LONG",
  "LT_FLOAT",
  "LT_DOUBLE",
  "LE_BYTE",
  "LE_SHORT",
  "LE_INT",
  "LE_LONG",
  "LE_FLOAT",
  "LE_DOUBLE",
  "BOOL_BYTE",
  "BOOL_SHORT",
  "BOOL_INT",
  "BOOL_LONG",
  "BOOL_FLOAT",
  "BOOL_DOUBLE",
  "BOOL_OBJECT",
  "REG_ADD_BYTE",
  "REG_ADD_SHORT",
  "REG_ADD_INT",
  "REG_ADD_LONG",
  "REG_ADD_FLOAT",
  "REG_ADD_DOUBLE",
  "REG_SUBTRACT_BYTE",
  "REG_SUBTRACT_SHORT",
  "REG_SUBTRACT_INT",
  "REG_SUBTRACT_LONG",
  "REG_SUBTRACT_FLOAT",
  "REG_SUBTRACT_DOUBLE",
  "REG_MULTIPLY_BYTE",
  "REG_MULTIPLY_SHORT",
  "REG_MULTIPLY_INT",
  "REG_MULTIPLY_LONG",
  "REG_MULTIPLY_FLOAT",
  "REG_MULTIPLY_DOUBLE",
  "REG_DIVIDE_BYTE",
  "REG_DIVIDE_SHORT",
  "REG_DIVIDE_INT",
  "REG_DIVIDE_LONG",
  "REG_DIVIDE_FLOAT",
  "REG_DIVIDE_DOUBLE",
};
