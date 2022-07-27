#include "spvm_native.h"

#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <errno.h>
#include <stddef.h>
#include <assert.h>

static const char* FILE_NAME = "Fn.c";

int32_t SPVM__Fn__DBL_MAX(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  stack[0].dval = DBL_MAX;
  
  return 0;
}

int32_t SPVM__Fn__DBL_MIN(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  stack[0].dval = DBL_MIN;
  
  return 0;
}

int32_t SPVM__Fn__DOUBLE_MAX(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  stack[0].dval = DBL_MAX;
  
  return 0;
}

int32_t SPVM__Fn__DOUBLE_MIN(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  stack[0].dval = DBL_MIN;
  
  return 0;
}

int32_t SPVM__Fn__FLOAT_MAX(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  stack[0].fval = FLT_MAX;
  
  return 0;
}

int32_t SPVM__Fn__FLOAT_MIN(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  stack[0].fval = FLT_MIN;
  
  return 0;
}

int32_t SPVM__Fn__FLT_MAX(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  stack[0].fval = FLT_MAX;
  
  return 0;
}

int32_t SPVM__Fn__FLT_MIN(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  stack[0].fval = FLT_MIN;
  
  return 0;
}

int32_t SPVM__Fn__RAND_MAX(SPVM_ENV* env, SPVM_VALUE* stack) {
  
  stack[0].ival = 2147483647;

  return 0;
}


static ptrdiff_t SPVM__Fn__static__convert_code_point_to_utf8_char(int32_t uc, uint8_t *dst) {
  if (uc < 0x00) {
    return 0;
  } else if (uc < 0x80) {
    dst[0] = (uint8_t) uc;
    return 1;
  } else if (uc < 0x800) {
    dst[0] = (uint8_t)(0xC0 + (uc >> 6));
    dst[1] = (uint8_t)(0x80 + (uc & 0x3F));
    return 2;
  // Note: we allow encoding 0xd800-0xdfff here, so as not to change
  // the API, however, these are actually invalid in UTF-8
  } else if (uc < 0x10000) {
    dst[0] = (uint8_t)(0xE0 + (uc >> 12));
    dst[1] = (uint8_t)(0x80 + ((uc >> 6) & 0x3F));
    dst[2] = (uint8_t)(0x80 + (uc & 0x3F));
    return 3;
  } else if (uc < 0x110000) {
    dst[0] = (uint8_t)(0xF0 + (uc >> 18));
    dst[1] = (uint8_t)(0x80 + ((uc >> 12) & 0x3F));
    dst[2] = (uint8_t)(0x80 + ((uc >> 6) & 0x3F));
    dst[3] = (uint8_t)(0x80 + (uc & 0x3F));
    return 4;
  } else return 0;
}

int32_t SPVM__Fn___chr_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  
  int32_t code_point = stack[0].ival;
  
  assert(code_point >= 0);
  assert(code_point < 0x110000);
  
  uint8_t utf8_bytes[4];
  int32_t utf8_bytes_length = (int32_t)SPVM__Fn__static__convert_code_point_to_utf8_char(code_point, utf8_bytes);
  
  assert(utf8_bytes_length > 0);
  
  void* utf8_string = env->new_string(env, stack, (char*)utf8_bytes, utf8_bytes_length);
  
  stack[0].oval = utf8_string;
  
  return 0;
}

#define utf_cont(ch)  (((ch) & 0xc0) == 0x80)
#define SPVM_UTF8PROC_ERROR_INVALIDUTF8 -3
static ptrdiff_t spvm_utf8proc_iterate(const uint8_t *str, ptrdiff_t strlen, int32_t *dst) {
  uint32_t uc;
  const uint8_t *end;

  *dst = -1;
  if (!strlen) return 0;
  end = str + ((strlen < 0) ? 4 : strlen);
  uc = *str++;
  if (uc < 0x80) {
    *dst = uc;
    return 1;
  }
  // Must be between 0xc2 and 0xf4 inclusive to be valid
  if ((uc - 0xc2) > (0xf4-0xc2)) return SPVM_UTF8PROC_ERROR_INVALIDUTF8;
  if (uc < 0xe0) {         // 2-byte sequence
     // Must have valid continuation character
     if (str >= end || !utf_cont(*str)) return SPVM_UTF8PROC_ERROR_INVALIDUTF8;
     *dst = ((uc & 0x1f)<<6) | (*str & 0x3f);
     return 2;
  }
  if (uc < 0xf0) {        // 3-byte sequence
     if ((str + 1 >= end) || !utf_cont(*str) || !utf_cont(str[1]))
        return SPVM_UTF8PROC_ERROR_INVALIDUTF8;
     // Check for surrogate chars
     if (uc == 0xed && *str > 0x9f)
         return SPVM_UTF8PROC_ERROR_INVALIDUTF8;
     uc = ((uc & 0xf)<<12) | ((*str & 0x3f)<<6) | (str[1] & 0x3f);
     if (uc < 0x800)
         return SPVM_UTF8PROC_ERROR_INVALIDUTF8;
     *dst = uc;
     return 3;
  }
  // 4-byte sequence
  // Must have 3 valid continuation characters
  if ((str + 2 >= end) || !utf_cont(*str) || !utf_cont(str[1]) || !utf_cont(str[2]))
     return SPVM_UTF8PROC_ERROR_INVALIDUTF8;
  // Make sure in correct range (0x10000 - 0x10ffff)
  if (uc == 0xf0) {
    if (*str < 0x90) return SPVM_UTF8PROC_ERROR_INVALIDUTF8;
  } else if (uc == 0xf4) {
    if (*str > 0x8f) return SPVM_UTF8PROC_ERROR_INVALIDUTF8;
  }
  *dst = ((uc & 7)<<18) | ((*str & 0x3f)<<12) | ((str[1] & 0x3f)<<6) | (str[2] & 0x3f);
  return 4;
}

// https://code.woboq.org/userspace/glibc/stdlib/rand_r.c.html
static int SPVM__Fn__static__rand_r (uint32_t *seed)
{
  uint32_t next = *seed;
  int result;
  next *= 1103515245;
  next += 12345;
  result = (uint32_t) (next / 65536) % 2048;
  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (uint32_t) (next / 65536) % 1024;
  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (uint32_t) (next / 65536) % 1024;
  *seed = next;
  return result;
}

int32_t SPVM__Fn__crand(SPVM_ENV* env, SPVM_VALUE* stack) {
  
  int32_t* seed_ref = stack[0].iref;
  
  int32_t random_value = SPVM__Fn__static__rand_r(seed_ref);

  stack[0].ival = random_value;

  return 0;
}

int32_t SPVM__Fn__get_next_code_point(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  
  void* obj_str = stack[0].oval;
  
  const char* str = env->get_chars(env, stack, obj_str);
  int32_t str_len = env->length(env, stack, obj_str);
  
  int32_t* offset_ref = stack[1].iref;
  
  if (*offset_ref < 0 || *offset_ref > str_len - 1) {
    stack[0].ival = -1;
    return 0;
  }
  
  int32_t dst;
  int32_t uchar_len = (int32_t)spvm_utf8proc_iterate((const uint8_t*)(str + *offset_ref), str_len, &dst);

  int32_t uchar;
  if (uchar_len > 0) {
    uchar = dst;
    *offset_ref += uchar_len;
  }
  else if (uchar_len == 0) {
    uchar = -1;
  }
  else if (uchar_len == SPVM_UTF8PROC_ERROR_INVALIDUTF8) {
    uchar = -1;
  }
  
  stack[0].ival = uchar;
  
  return 0;
}
int32_t SPVM__Fn__is_array(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;

  void* object = stack[0].oval;
  
  int32_t is_array;
  if (env->is_array(env, stack, object)) {
    is_array = 1;
  }
  else {
    is_array = 0;
  }

  stack[0].ival = is_array;

  return 0;
}

int32_t SPVM__Fn__is_mulnum_array(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;

  void* object = stack[0].oval;
  
  int32_t is_mulnum_array;
  if (env->is_mulnum_array(env, stack, object)) {
    is_mulnum_array = 1;
  }
  else {
    is_mulnum_array = 0;
  }

  stack[0].ival = is_mulnum_array;

  return 0;
}

int32_t SPVM__Fn__is_numeric_array(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;

  void* object = stack[0].oval;
  
  int32_t is_numeric_array;
  if (env->is_numeric_array(env, stack, object)) {
    is_numeric_array = 1;
  }
  else {
    is_numeric_array = 0;
  }

  stack[0].ival = is_numeric_array;

  return 0;
}

int32_t SPVM__Fn__is_object_array(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;

  void* object = stack[0].oval;
  
  int32_t is_object_array;
  if (env->is_object_array(env, stack, object)) {
    is_object_array = 1;
  }
  else {
    is_object_array = 0;
  }

  stack[0].ival = is_object_array;

  return 0;
}

int32_t SPVM__Fn__memcpy(SPVM_ENV* env, SPVM_VALUE* stack) {
  
  void* dest = stack[0].oval;
  int32_t dest_offset = stack[1].ival;
  void* source = stack[2].oval;
  int32_t source_offset = stack[3].ival;
  int32_t length = stack[4].ival;

  if (!dest) {
    return env->die(env, stack, "The destination must be defined", FILE_NAME, __LINE__);
  }
  
  if (!(env->is_string(env, stack, dest) || env->is_numeric_array(env, stack, dest) || env->is_mulnum_array(env, stack, dest))) {
    return env->die(env, stack, "The destination must be a string type, a numeric arrya type, or a multi numeric array", FILE_NAME, __LINE__);
  }
  
  if (!source) {
    return env->die(env, stack, "The source must be defined", FILE_NAME, __LINE__);
  }

  if (!(env->is_string(env, stack, source) || env->is_numeric_array(env, stack, source) || env->is_mulnum_array(env, stack, source))) {
    return env->die(env, stack, "The source must be a string type, a numeric arrya type, or a multi numeric array", FILE_NAME, __LINE__);
  }

  if (env->is_read_only(env, stack, dest)) {
    return env->die(env, stack, "The destination must not be a read-only string", FILE_NAME, __LINE__);
  }
  
  if (length == 0) {
    return 0;
  }
  else if (length < 0) {
    return env->die(env, stack, "The length must be more than or equals to 0", FILE_NAME, __LINE__);
  }
  
  char* dest_bytes = (char*)env->get_chars(env, stack, dest);
  int32_t dest_len = env->length(env, stack, dest);
  int32_t dest_elem_byte_size = env->get_elem_byte_size(env, stack, dest);
  int32_t dest_length = dest_elem_byte_size * dest_len;
  
  const char* source_bytes = env->get_chars(env, stack, source);
  int32_t source_len = env->length(env, stack, source);
  int32_t source_elem_byte_size = env->get_elem_byte_size(env, stack, source);
  int32_t source_length = source_elem_byte_size * source_len;
  
  if (dest_offset + length > dest_length) {
    return env->die(env, stack, "The destination offset + length must be within the range of the destination array", FILE_NAME, __LINE__);
  }

  if (source_offset + length > source_length) {
    return env->die(env, stack, "Source offset + length must be within the range of the source array", FILE_NAME, __LINE__);
  }
  
  memcpy((char*)(dest_bytes + dest_offset), (char*)(source_bytes + source_offset), length);
  
  return 0;
}

int32_t SPVM__Fn__memmove(SPVM_ENV* env, SPVM_VALUE* stack) {
  
  void* dest = stack[0].oval;
  int32_t dest_offset = stack[1].ival;
  void* source = stack[2].oval;
  int32_t source_offset = stack[3].ival;
  int32_t length = stack[4].ival;

  if (!dest) {
    return env->die(env, stack, "The destination must be defined", FILE_NAME, __LINE__);
  }
  
  if (!(env->is_string(env, stack, dest) || env->is_numeric_array(env, stack, dest) || env->is_mulnum_array(env, stack, dest))) {
    return env->die(env, stack, "The destination must be a string type, a numeric arrya type, or a multi numeric array", FILE_NAME, __LINE__);
  }
  
  if (!source) {
    return env->die(env, stack, "The source must be defined", FILE_NAME, __LINE__);
  }

  if (!(env->is_string(env, stack, source) || env->is_numeric_array(env, stack, source) || env->is_mulnum_array(env, stack, source))) {
    return env->die(env, stack, "The source must be a string type, a numeric arrya type, or a multi numeric array", FILE_NAME, __LINE__);
  }

  if (env->is_read_only(env, stack, dest)) {
    return env->die(env, stack, "The destination must not be a read-only string", FILE_NAME, __LINE__);
  }
  
  if (length == 0) {
    return 0;
  }
  else if (length < 0) {
    return env->die(env, stack, "The length must be more than or equals to 0", FILE_NAME, __LINE__);
  }
  
  char* dest_bytes = (char*)env->get_chars(env, stack, dest);
  int32_t dest_len = env->length(env, stack, dest);
  int32_t dest_elem_byte_size = env->get_elem_byte_size(env, stack, dest);
  int32_t dest_length = dest_elem_byte_size * dest_len;
  
  const char* source_bytes = env->get_chars(env, stack, source);
  int32_t source_len = env->length(env, stack, source);
  int32_t source_elem_byte_size = env->get_elem_byte_size(env, stack, source);
  int32_t source_length = source_elem_byte_size * source_len;
  
  if (dest_offset + length > dest_length) {
    return env->die(env, stack, "The destination offset + length must be within the range of the destination array", FILE_NAME, __LINE__);
  }

  if (source_offset + length > source_length) {
    return env->die(env, stack, "Source offset + length must be within the range of the source array", FILE_NAME, __LINE__);
  }
  
  memmove((char*)(dest_bytes + dest_offset), (char*)(source_bytes + source_offset), length);
  
  return 0;
}

int32_t SPVM__Fn__shorten(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;

  void* string = stack[0].oval;
  int32_t new_length = stack[1].ival;
  
  env->shorten(env, stack, string, new_length);

  return 0;
}

int32_t SPVM__Fn__to_double(SPVM_ENV* env, SPVM_VALUE* stack) {
  
  void* obj_string = stack[0].oval;
  if (!obj_string) {
    return env->die(env, stack, "String must be defined", FILE_NAME, __LINE__);
  }
  const char* string = env->get_chars(env, stack, obj_string);
  
  char *end;
  errno = 0;
  double num = strtod(string, &end);
  if (*end != '\0') {
    return env->die(env, stack, "Invalid number format", FILE_NAME, __LINE__);
  }
  else if (errno == ERANGE) {
    return env->die(env, stack, "[ERANGE]Out of range", FILE_NAME, __LINE__);
  }
  
  stack[0].dval = num;

  return 0;
}

int32_t SPVM__Fn__to_float(SPVM_ENV* env, SPVM_VALUE* stack) {
  
  void* obj_string = stack[0].oval;
  if (!obj_string) {
    return env->die(env, stack, "String must be defined", FILE_NAME, __LINE__);
  }
  const char* string = env->get_chars(env, stack, obj_string);
  
  char *end;
  errno = 0;
  float num = strtof(string, &end);
  if (*end != '\0') {
    return env->die(env, stack, "Invalid number format", FILE_NAME, __LINE__);
  }
  else if (errno == ERANGE) {
    return env->die(env, stack, "[ERANGE]Out of range", FILE_NAME, __LINE__);
  }
  
  stack[0].fval = num;

  return 0;
}

int32_t SPVM__Fn__to_int_with_base(SPVM_ENV* env, SPVM_VALUE* stack) {
  
  void* obj_string = stack[0].oval;
  if (!obj_string) {
    return env->die(env, stack, "String must be defined", FILE_NAME, __LINE__);
  }
  const char* string = env->get_chars(env, stack, obj_string);
  
  int32_t digit = stack[1].ival;
  
  if (!(digit == 2 || digit == 8 || digit == 10 || digit == 16)) {
    return env->die(env, stack, "Digit must be 2, 8, 10, 16", FILE_NAME, __LINE__);
  }
  
  char *end;
  errno = 0;
  long int num = strtol(string, &end, digit);
  if (*end != '\0') {
    return env->die(env, stack, "Invalid number format", FILE_NAME, __LINE__);
  }
  else if (errno == ERANGE || num < INT32_MIN || num > INT32_MAX) {
    return env->die(env, stack, "Out of range", FILE_NAME, __LINE__);
  }
  
  stack[0].ival = (int32_t)num;

  return 0;
}

int32_t SPVM__Fn__to_long_with_base(SPVM_ENV* env, SPVM_VALUE* stack) {
  
  void* obj_string = stack[0].oval;
  if (!obj_string) {
    return env->die(env, stack, "String must be defined", FILE_NAME, __LINE__);
  }
  const char* string = env->get_chars(env, stack, obj_string);
  
  int32_t digit = stack[1].ival;

  if (!(digit == 2 || digit == 8 || digit == 10 || digit == 16)) {
    return env->die(env, stack, "Digit must be 2, 8, 10, 16", FILE_NAME, __LINE__);
  }
  
  char *end;
  errno = 0;
  long long int num = strtoll(string, &end, digit);
  if (*end != '\0') {
    return env->die(env, stack, "Invalid number format", FILE_NAME, __LINE__);
  }
  else if (errno == ERANGE || num < INT64_MIN || num > INT64_MAX) {
    return env->die(env, stack, "Out of range", FILE_NAME, __LINE__);
  }
  
  stack[0].lval = (int64_t)num;

  return 0;
}
