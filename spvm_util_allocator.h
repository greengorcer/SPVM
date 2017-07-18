#ifndef SPVM_UTIL_ALLOCATOR_H
#define SPVM_UTIL_ALLOCATOR_H

void* SPVM_UTIL_ALLOCATOR_safe_malloc_i32(int32_t count, int32_t size);
void* SPVM_UTIL_ALLOCATOR_safe_malloc_i32_zero(int32_t count, int32_t size);
void* SPVM_UTIL_ALLOCATOR_safe_realloc_i32(void* ptr, int32_t count, int32_t size);
void* SPVM_UTIL_ALLOCATOR_safe_realloc_i32_zero(void* ptr, int32_t count, int32_t size);

#endif
