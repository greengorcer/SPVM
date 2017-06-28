#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "spvm_extention.h"
#include "spvm_runtime.h"
#include "spvm_runtime_api.h"

#include "spvm_env.h"

void SPVM_EXTENTION_std__sum_int(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  SPVM_DATA_ARRAY* data_array = SPVM_RUNTIME_API_get_var_address(env, 0);
  
  int32_t length = SPVM_RUNTIME_API_get_array_length(env, data_array);
  
  int32_t* values = SPVM_RUNTIME_API_get_array_values_int(env, data_array);
  
  int64_t total = 0;
  {
    int32_t i;
    for (i = 0; i < length; i++) {
      total += values[i];
    }
  }
  
  SPVM_RUNTIME_API_push_return_value_int(env, total);
}

void SPVM_EXTENTION_std__print(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  SPVM_DATA_ARRAY* data_array = SPVM_RUNTIME_API_get_var_address(env, 0);

  int32_t length = SPVM_RUNTIME_API_get_array_length(env, data_array);
  int8_t* string = SPVM_RUNTIME_API_get_array_values_byte(env, data_array);
  
  {
    int32_t i;
    for (i = 0; i < length; i++) {
      putchar((int)string[i]);
    }
  }
}

void SPVM_EXTENTION_std__println(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  SPVM_DATA_ARRAY* data_array = SPVM_RUNTIME_API_get_var_address(env, 0);

  int32_t length = SPVM_RUNTIME_API_get_array_length(env, data_array);
  
  int8_t* string = SPVM_RUNTIME_API_get_array_values_byte(env, data_array);
  
  {
    int32_t i;
    for (i = 0; i < length; i++) {
      putchar((int)string[i]);
    }
  }
  
  printf("\n");
}

void SPVM_EXTENTION_std__println_byte(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  int8_t value = SPVM_RUNTIME_API_get_var_byte(env, 0);
  
  printf("%" PRId8 "\n", value);
}

void SPVM_EXTENTION_std__println_short(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  int16_t value = SPVM_RUNTIME_API_get_var_short(env, 0);
  
  printf("%" PRId16 "\n", value);
}

void SPVM_EXTENTION_std__println_int(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  int32_t value = SPVM_RUNTIME_API_get_var_int(env, 0);
  
  printf("%" PRId32 "\n", value);
}

void SPVM_EXTENTION_std__println_long(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  int64_t value = SPVM_RUNTIME_API_get_var_long(env, 0);
  
  printf("%" PRId64 "\n", value);
}

void SPVM_EXTENTION_std__println_float(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  float value = SPVM_RUNTIME_API_get_var_float(env, 0);
  
  printf("%f\n", value);
}

void SPVM_EXTENTION_std__println_double(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  double value = SPVM_RUNTIME_API_get_var_double(env, 0);
  
  printf("%f\n", value);
}

void SPVM_EXTENTION_std__print_byte(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  int8_t value = SPVM_RUNTIME_API_get_var_byte(env, 0);
  
  printf("%" PRId8, value);
}

void SPVM_EXTENTION_std__print_short(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  int16_t value = SPVM_RUNTIME_API_get_var_short(env, 0);
  
  printf("%" PRId16, value);
}

void SPVM_EXTENTION_std__print_int(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  int32_t value = SPVM_RUNTIME_API_get_var_int(env, 0);
  
  printf("%" PRId32, value);
}

void SPVM_EXTENTION_std__print_long(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  int64_t value = SPVM_RUNTIME_API_get_var_long(env, 0);
  
  printf("%" PRId64, value);
}

void SPVM_EXTENTION_std__print_float(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  float value = SPVM_RUNTIME_API_get_var_float(env, 0);
  
  printf("%f", value);
}

void SPVM_EXTENTION_std__print_double(SPVM_ENV* env) {
  SPVM_RUNTIME* runtime = (SPVM_RUNTIME*)env->runtime;
  
  double value = SPVM_RUNTIME_API_get_var_double(env, 0);
  
  printf("%f", value);
}
