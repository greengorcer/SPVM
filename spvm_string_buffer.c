#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "spvm_string_buffer.h"
#include "spvm_util_allocator.h"

SPVM_STRING_BUFFER* SPVM_STRING_BUFFER_new(int32_t capacity) {
  
  if (capacity == 0) {
    capacity = 0xFFFF;
  }
  
  SPVM_STRING_BUFFER* string_buffer = (SPVM_STRING_BUFFER*) SPVM_UTIL_ALLOCATOR_safe_malloc_zero(sizeof(SPVM_STRING_BUFFER));
  
  string_buffer->capacity = capacity;
  string_buffer->buffer = (char*)SPVM_UTIL_ALLOCATOR_safe_malloc_zero(capacity);
  
  return string_buffer;
}

char* SPVM_STRING_BUFFER_get_buffer(SPVM_STRING_BUFFER* string_buffer) {
  return string_buffer->buffer;
}

void SPVM_STRING_BUFFER_maybe_extend(SPVM_STRING_BUFFER* string_buffer, int32_t new_length) {

  // Extend
  if (new_length > string_buffer->capacity) {
    int32_t new_capacity = string_buffer->capacity * 2;
    char* new_buffer = SPVM_UTIL_ALLOCATOR_safe_malloc_zero(new_capacity);
    memcpy(new_buffer, string_buffer->buffer, string_buffer->length);
    string_buffer->buffer = new_buffer;
    string_buffer->capacity = new_capacity;
  }
}

void SPVM_STRING_BUFFER_add(SPVM_STRING_BUFFER* string_buffer, char* string) {
  
  int32_t string_length = strlen(string);
  
  int32_t new_length = string_buffer->length + string_length;
  
  // Extend
  SPVM_STRING_BUFFER_maybe_extend(string_buffer, new_length);
  
  memcpy(string_buffer->buffer + string_buffer->length, string, string_length);
  
  string_buffer->length = new_length;
  
  return;
}

void SPVM_STRING_BUFFER_add_int(SPVM_STRING_BUFFER* string_buffer, int32_t value) {
  
  int32_t max_length = 20;
  
  int32_t new_max_length = string_buffer->length + max_length;
  
  // Extend
  SPVM_STRING_BUFFER_maybe_extend(string_buffer, new_max_length);
  
  int32_t write_length = sprintf(string_buffer->buffer + string_buffer->length, "%" PRId32, value);
  
  string_buffer->length += write_length;
  
  return;
}

void SPVM_STRING_BUFFER_add_address(SPVM_STRING_BUFFER* string_buffer, void* value) {
  
  int32_t max_length = 20;
  
  int32_t new_max_length = string_buffer->length + max_length;
  
  // Extend
  SPVM_STRING_BUFFER_maybe_extend(string_buffer, new_max_length);
  
  int32_t write_length = sprintf(string_buffer->buffer + string_buffer->length, "%p", value);
  
  string_buffer->length += write_length;
  
  return;
}

void SPVM_STRING_BUFFER_free(SPVM_STRING_BUFFER* string_buffer) {
  
  free(string_buffer->buffer);
  
  free(string_buffer);
}
