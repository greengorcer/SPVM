#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <assert.h>

#include <spvm_native.h>

static const char* MFILE = "TestCase/NativeAPI.c";

int32_t SPVM__TestCase__NativeAPI__check_native_api_indexes(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  void** env_array = (void**)env;
  
  if ((void*)&env->class_vars_heap != &env_array[0]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->object_header_byte_size != &env_array[1]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->object_weaken_backref_head_offset != &env_array[2]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->object_ref_count_offset != &env_array[3]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->object_basic_type_id_offset != &env_array[4]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->object_type_dimension_offset != &env_array[5]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->object_type_category_offset != &env_array[6]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->object_flag_offset != &env_array[7]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->object_length_offset != &env_array[8]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->byte_object_basic_type_id != &env_array[9]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->short_object_basic_type_id != &env_array[10]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->int_object_basic_type_id != &env_array[11]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->long_object_basic_type_id != &env_array[12]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->float_object_basic_type_id != &env_array[13]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->double_object_basic_type_id != &env_array[14]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->compiler != &env_array[15]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->exception_object != &env_array[16]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->native_mortal_stack != &env_array[17]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->native_mortal_stack_top != &env_array[18]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->native_mortal_stack_capacity != &env_array[19]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_basic_type_id != &env_array[20]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_id != &env_array[21]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_offset != &env_array[22]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_id != &env_array[23]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_method_id != &env_array[24]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_instance_method_id != &env_array[25]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_object_raw != &env_array[26]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_object != &env_array[27]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_byte_array_raw != &env_array[28]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_byte_array != &env_array[29]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_short_array_raw != &env_array[30]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_short_array != &env_array[31]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_int_array_raw != &env_array[32]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_int_array != &env_array[33]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_long_array_raw != &env_array[34]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_long_array != &env_array[35]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_float_array_raw != &env_array[36]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_float_array != &env_array[37]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_double_array_raw != &env_array[38]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_double_array != &env_array[39]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_object_array_raw != &env_array[40]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_object_array != &env_array[41]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_muldim_array_raw != &env_array[42]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_muldim_array != &env_array[43]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_mulnum_array_raw != &env_array[44]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_mulnum_array != &env_array[45]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_string_nolen_raw != &env_array[46]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_string_nolen != &env_array[47]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_string_raw != &env_array[48]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_string != &env_array[49]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_pointer_raw != &env_array[50]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_pointer != &env_array[51]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->concat_raw != &env_array[52]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->concat != &env_array[53]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_stack_trace_raw != &env_array[54]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_stack_trace != &env_array[55]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->length != &env_array[56]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_elems_byte != &env_array[57]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_elems_short != &env_array[58]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_elems_int != &env_array[59]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_elems_long != &env_array[60]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_elems_float != &env_array[61]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_elems_double != &env_array[62]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_elem_object != &env_array[63]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_elem_object != &env_array[64]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_byte != &env_array[65]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_short != &env_array[66]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_int != &env_array[67]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_long != &env_array[68]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_float != &env_array[69]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_double != &env_array[70]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_object != &env_array[71]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_byte != &env_array[72]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_short != &env_array[73]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_int != &env_array[74]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_long != &env_array[75]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_float != &env_array[76]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_double != &env_array[77]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_object != &env_array[78]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_byte != &env_array[79]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_short != &env_array[80]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_int != &env_array[81]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_long != &env_array[82]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_float != &env_array[83]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_double != &env_array[84]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_object != &env_array[85]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_byte != &env_array[86]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_short != &env_array[87]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_int != &env_array[88]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_long != &env_array[89]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_float != &env_array[90]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_double != &env_array[91]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_object != &env_array[92]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_pointer != &env_array[93]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_pointer != &env_array[94]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->call_spvm_method != &env_array[95]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_exception != &env_array[96]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_exception != &env_array[97]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_ref_count != &env_array[98]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->inc_ref_count != &env_array[99]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->dec_ref_count != &env_array[100]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->enter_scope != &env_array[101]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->push_mortal != &env_array[102]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->leave_scope != &env_array[103]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->remove_mortal != &env_array[104]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->is_type != &env_array[105]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->has_callback != &env_array[106]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_object_basic_type_id != &env_array[107]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_object_type_dimension != &env_array[108]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->weaken != &env_array[109]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->isweak != &env_array[110]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->unweaken != &env_array[111]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->alloc_memory_block_zero != &env_array[112]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->free_memory_block != &env_array[113]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_memory_blocks_count != &env_array[114]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_type_name_raw != &env_array[115]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_type_name != &env_array[116]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_env != &env_array[117]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->free_env != &env_array[118]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->memory_blocks_count != &env_array[119]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_chars != &env_array[120]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->die != &env_array[121]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_object_by_name != &env_array[122]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->new_pointer_by_name != &env_array[123]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_byte_by_name != &env_array[124]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_short_by_name != &env_array[125]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_int_by_name != &env_array[126]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_long_by_name != &env_array[127]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_float_by_name != &env_array[128]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_double_by_name != &env_array[129]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_field_object_by_name != &env_array[130]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_byte_by_name != &env_array[131]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_short_by_name != &env_array[132]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_int_by_name != &env_array[133]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_long_by_name != &env_array[134]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_float_by_name != &env_array[135]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_double_by_name != &env_array[136]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_object_by_name != &env_array[137]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_byte_by_name != &env_array[138]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_short_by_name != &env_array[139]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_int_by_name != &env_array[140]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_long_by_name != &env_array[141]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_float_by_name != &env_array[142]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_double_by_name != &env_array[143]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->set_class_var_object_by_name != &env_array[144]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_byte_by_name != &env_array[145]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_short_by_name != &env_array[146]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_int_by_name != &env_array[147]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_long_by_name != &env_array[148]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_float_by_name != &env_array[149]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_double_by_name != &env_array[150]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_class_var_object_by_name != &env_array[151]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->call_class_method_by_name != &env_array[152]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->call_instance_method_by_name != &env_array[153]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_field_string_chars_by_name != &env_array[154]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->any_object_basic_type_id != &env_array[155]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->dump_raw != &env_array[156]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->dump != &env_array[157]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->call_class_method != &env_array[158]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->call_instance_method != &env_array[159]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_instance_method_id_static != &env_array[160]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->get_bool_object_value != &env_array[161]) { stack[0].ival = 0; return 0; }
  if ((void*)&env->string_basic_type_id != &env_array[162]) { stack[0].ival = 0; return 0; }

  stack[0].ival = 1;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_byte_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  int8_t value = env->get_class_var_byte_by_name(env, "TestCase::NativeAPI", "$BYTE_VALUE", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].bval = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_byte_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  int8_t value = env->get_class_var_byte_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].bval = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_short_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  int16_t value = env->get_class_var_short_by_name(env, "TestCase::NativeAPI", "$SHORT_VALUE", &e, MFILE, __LINE__);
  if (e) { return e; }

  stack[0].sval = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_short_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  int16_t value = env->get_class_var_short_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].sval = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_int_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  int32_t value = env->get_class_var_int_by_name(env, "TestCase::NativeAPI", "$INT_VALUE", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].ival = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_int_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  int32_t value = env->get_class_var_int_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].ival = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_long_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  int64_t value = env->get_class_var_long_by_name(env, "TestCase::NativeAPI", "$LONG_VALUE", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].lval = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_long_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  int64_t value = env->get_class_var_long_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].lval = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_float_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  float value = env->get_class_var_float_by_name(env, "TestCase::NativeAPI", "$FLOAT_VALUE", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].fval = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_float_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  float value = env->get_class_var_float_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].fval = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_double_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  double value = env->get_class_var_double_by_name(env, "TestCase::NativeAPI", "$DOUBLE_VALUE", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].dval = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_double_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  double value = env->get_class_var_double_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].dval = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_object_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;

  void* value = env->get_class_var_object_by_name(env, "TestCase::NativeAPI", "$MINIMAL_VALUE", "TestCase::Minimal", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].oval = value;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_class_var_object_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  void* value = env->get_class_var_object_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", "TestCase::Minimal", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].oval = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_byte_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  env->set_class_var_byte_by_name(env, "TestCase::NativeAPI", "$BYTE_VALUE", INT8_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_byte_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  env->set_class_var_byte_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", INT8_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_short_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  env->set_class_var_short_by_name(env, "TestCase::NativeAPI", "$SHORT_VALUE", INT16_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_short_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  env->set_class_var_short_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", INT16_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_int_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  env->set_class_var_int_by_name(env, "TestCase::NativeAPI", "$INT_VALUE", INT32_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_int_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  env->set_class_var_int_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", INT32_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_long_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;
  
  env->set_class_var_long_by_name(env, "TestCase::NativeAPI", "$LONG_VALUE", INT64_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_long_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;
  
  env->set_class_var_long_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", INT64_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_float_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  env->set_class_var_float_by_name(env, "TestCase::NativeAPI", "$FLOAT_VALUE", FLT_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_float_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  env->set_class_var_float_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", FLT_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_double_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  env->set_class_var_double_by_name(env, "TestCase::NativeAPI", "$DOUBLE_VALUE", DBL_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_double_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  env->set_class_var_double_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", DBL_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_object_by_name_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;
  
  void* minimal = env->new_object_by_name(env, "TestCase::Minimal", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  env->set_class_var_object_by_name(env, "TestCase::NativeAPI", "$MINIMAL_VALUE", "TestCase::Minimal", minimal, &e, MFILE, __LINE__);
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__set_class_var_object_by_name_test_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  void* minimal = env->new_object_by_name(env, "TestCase::Minimal", &e, MFILE, __LINE__);
  
  env->set_class_var_object_by_name(env, "TestCase::NativeAPI", "$NOT_FOUND", "TestCase::Minimal", minimal, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_new_object_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  void* minimal = env->new_object_by_name(env, "TestCase::Minimal", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].oval = minimal;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_new_object_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  void* minimal = env->new_object_by_name(env, "NotFound", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].oval = minimal;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_new_pointer_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* pointer;
  int32_t e;
  void* minimal = env->new_pointer_by_name(env, "TestCase::NotFound", pointer, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  stack[0].oval = minimal;
  
  return 0;
}


int32_t SPVM__TestCase__NativeAPI__has_callback_test(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* object = stack[0].oval;
  
  int32_t basic_type_id = env->get_basic_type_id(env, "TestCase::Callback::Callback");
  if (basic_type_id < 0) {
    assert(0);
  }
  
  int32_t match = env->has_callback(env, object, basic_type_id);
  
  stack[0].ival = match;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__is_type_test_minimals(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* object = stack[0].oval;
  
  int32_t basic_type_id = env->get_basic_type_id(env, "TestCase::Minimal");
  if (basic_type_id < 0) {
    assert(0);
  }
  
  int32_t match = env->is_type(env, object, basic_type_id, 1);
  
  stack[0].ival = match;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__ref_byte_sum(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int8_t x_in1 = stack[0].bval;
  int8_t x_in2 = stack[1].bval;
  
  *(stack[2].bref) = x_in1 + x_in2;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__ref_short_sum(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int16_t x_in1 = stack[0].sval;
  int16_t x_in2 = stack[1].sval;
  
  *(stack[2].sref) = x_in1 + x_in2;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__ref_int_sum(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t x_in1 = stack[0].ival;
  int32_t x_in2 = stack[1].ival;
  
  *(stack[2].iref) = x_in1 + x_in2;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__ref_long_sum(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int64_t x_in1 = stack[0].lval;
  int64_t x_in2 = stack[1].lval;
  
  *(stack[2].lref) = x_in1 + x_in2;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__ref_float_sum(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  float x_in1 = stack[0].fval;
  float x_in2 = stack[1].fval;
  
  *(stack[2].fref) = x_in1 + x_in2;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__ref_double_sum(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  double x_in1 = stack[0].dval;
  double x_in2 = stack[1].dval;
  
  *(stack[2].dref) = x_in1 + x_in2;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_byte_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  void* object = stack[0].oval;
  
  env->set_field_byte_by_name(env, object, "TestCase::Simple", "byte_value", INT8_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_byte_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  void* object = stack[0].oval;
  
  env->set_field_byte_by_name(env, object, "TestCase::Simple", "not_found", INT8_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_short_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;
  void* object = stack[0].oval;
  
  env->set_field_short_by_name(env, object, "TestCase::Simple", "short_value", INT16_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_short_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;
  void* object = stack[0].oval;
  
  env->set_field_short_by_name(env, object, "TestCase::Simple", "not_found", INT16_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_int_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;
  void* object = stack[0].oval;
  
  env->set_field_int_by_name(env, object, "TestCase::Simple", "int_value", INT32_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_int_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;
  void* object = stack[0].oval;
  
  env->set_field_int_by_name(env, object, "TestCase::Simple", "not_found", INT32_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_long_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;
  void* object = stack[0].oval;
  
  env->set_field_long_by_name(env, object, "TestCase::Simple", "long_value", INT64_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_long_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;
  void* object = stack[0].oval;
  
  env->set_field_long_by_name(env, object, "TestCase::Simple", "not_found", INT64_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_float_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;
  void* object = stack[0].oval;
  
  env->set_field_float_by_name(env, object, "TestCase::Simple", "float_value", FLT_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_float_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;
  void* object = stack[0].oval;
  
  env->set_field_float_by_name(env, object, "TestCase::Simple", "not_found", FLT_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_double_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;
  void* object = stack[0].oval;
  
  env->set_field_double_by_name(env, object, "TestCase::Simple", "double_value", DBL_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_double_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;
  void* object = stack[0].oval;
  
  env->set_field_double_by_name(env, object, "TestCase::Simple", "not_found", DBL_MIN, &e, MFILE, __LINE__);
  if (e) { return e; }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_object_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  void* object_simple = stack[0].oval;
  
  int32_t basic_type_id = env->get_basic_type_id(env, "TestCase::Minimal");
  if (basic_type_id < 0) {
    return 1;
  }
  void* object_minimal = env->new_object(env, basic_type_id);
  
  int32_t field_id_minimal_x = env->get_field_id(env, "TestCase::Minimal", "x", "int");
  if (field_id_minimal_x < 0) {
    return 1;
  }
  env->set_field_int(env, object_minimal, field_id_minimal_x, 3);
  
  env->set_field_object_by_name(env, object_simple, "TestCase::Simple", "object_value", "TestCase::Minimal", object_minimal, &e, MFILE, __LINE__);
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_set_field_object_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  void* object_simple = stack[0].oval;
  
  int32_t basic_type_id = env->get_basic_type_id(env, "TestCase::Minimal");
  if (basic_type_id < 0) {
    return 1;
  }
  void* object_minimal = env->new_object(env, basic_type_id);
  
  int32_t field_id_minimal_x = env->get_field_id(env, "TestCase::Minimal", "x", "int");
  if (field_id_minimal_x < 0) {
    return 1;
  }
  env->set_field_int(env, object_minimal, field_id_minimal_x, 3);
  
  env->set_field_object_by_name(env, object_simple, "TestCase::Simple", "not_found", "TestCase::Minimal", object_minimal, &e, MFILE, __LINE__);
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_byte_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;
  void* object = stack[0].oval;
  
  int8_t byte_value = env->get_field_byte_by_name(env, object, "TestCase::Simple", "byte_value", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  if (byte_value == INT8_MIN) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_byte_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  int32_t e;
  void* object = stack[0].oval;
  
  int8_t byte_value = env->get_field_byte_by_name(env, object, "TestCase::Simple", "not_found", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  if (byte_value == INT8_MIN) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_short_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  void* object = stack[0].oval;
  
  int16_t short_value = env->get_field_short_by_name(env, object, "TestCase::Simple", "short_value", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  if (short_value == INT16_MIN) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_short_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  void* object = stack[0].oval;
  
  int16_t short_value = env->get_field_short_by_name(env, object, "TestCase::Simple", "not_found", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  if (short_value == INT16_MIN) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_int_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  void* object = stack[0].oval;
  
  int32_t int_value = env->get_field_int_by_name(env, object, "TestCase::Simple", "int_value", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  if (int_value == INT32_MIN) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_int_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  void* object = stack[0].oval;
  
  int32_t int_value = env->get_field_int_by_name(env, object, "TestCase::Simple", "not_found", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  if (int_value == INT32_MIN) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_long_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  void* object = stack[0].oval;
  
  int64_t long_value = env->get_field_long_by_name(env, object, "TestCase::Simple", "long_value", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  if (long_value == INT64_MIN) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_long_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  void* object = stack[0].oval;
  
  int64_t long_value = env->get_field_long_by_name(env, object, "TestCase::Simple", "not_found", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  if (long_value == INT64_MIN) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_float_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  void* object = stack[0].oval;
  
  float float_value = env->get_field_float_by_name(env, object, "TestCase::Simple", "float_value", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  if (float_value == FLT_MIN) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_float_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  void* object = stack[0].oval;
  
  float float_value = env->get_field_float_by_name(env, object, "TestCase::Simple", "not_found", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  if (float_value == FLT_MIN) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_double_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  void* object = stack[0].oval;
  
  double double_value = env->get_field_double_by_name(env, object, "TestCase::Simple", "double_value", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  if (double_value == DBL_MIN) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_double_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;

  void* object = stack[0].oval;
  
  double double_value = env->get_field_double_by_name(env, object, "TestCase::Simple", "not_found", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  if (double_value == DBL_MIN) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_object_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;
  
  void* object_simple = stack[0].oval;
  
  void* object_minimal = env->get_field_object_by_name(env, object_simple, "TestCase::Simple", "object_value", "TestCase::Minimal", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  int32_t field_id_minimal_x = env->get_field_id(env, "TestCase::Minimal", "x", "int");
  if (field_id_minimal_x < 0) {
    return 1;
  }
  
  int32_t x = env->get_field_int(env, object_minimal, field_id_minimal_x);
  
  if (x == 5) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_string_chars_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;
  
  void* string_chars_simple = stack[0].oval;
  
  const char* string_chars = env->get_field_string_chars_by_name(env, string_chars_simple, "TestCase::Simple", "string_value", &e, MFILE, __LINE__);
  if (e) { return e; };
  
  if (strcmp(string_chars, "abc") == 0) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_string_chars_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;
  
  void* string_chars_simple = stack[0].oval;
  
  const char* string_chars = env->get_field_string_chars_by_name(env, string_chars_simple, "TestCase::Simple", "not_found", &e, MFILE, __LINE__);
  if (e) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_get_field_object_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  int32_t e;
  
  void* object_simple = stack[0].oval;
  
  void* object_minimal = env->get_field_object_by_name(env, object_simple, "TestCase::Simple", "not_found", "TestCase::Minimal", &e, MFILE, __LINE__);
  if (e) { return e; }
  
  int32_t field_id_minimal_x = env->get_field_id(env, "TestCase::Minimal", "x", "int");
  if (field_id_minimal_x < 0) {
    return 1;
  }
  
  int32_t x = env->get_field_int(env, object_minimal, field_id_minimal_x);
  
  if (x == 5) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_use_strlen(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* string = stack[0].oval;
  
  int8_t* bytes = env->get_elems_byte(env, string);
  
  int32_t length = (int32_t)strlen((char*)bytes);
  
  stack[0].ival = length;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_env_get_field_byte(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* test_case = stack[0].oval;
  
  int32_t field_id = env->get_field_id(env, test_case, "x_byte", "byte");
  if (field_id < 0) {
    return 1;
  }
  
  int8_t value = env->get_field_byte(env, test_case, field_id);
  
  stack[0].bval = value;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_env_get_field_short(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* test_case = stack[0].oval;
  
  int32_t field_id = env->get_field_id(env, test_case, "x_short", "short");
  if (field_id < 0) {
    return 1;
  }
  
  int16_t value = env->get_field_short(env, test_case, field_id);
  
  stack[0].sval = value;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_env_get_field_int(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* test_case = stack[0].oval;
  
  int32_t field_id = env->get_field_id(env, test_case, "x_int", "int");
  if (field_id < 0) {
    return 1;
  }
  
  int32_t value = env->get_field_int(env, test_case, field_id);
  
  stack[0].ival = value;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_env_get_field_long(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* test_case = stack[0].oval;
  
  int32_t field_id = env->get_field_id(env, test_case, "x_long", "long");
  if (field_id < 0) {
    return 1;
  }
  
  int64_t value = env->get_field_long(env, test_case, field_id);
  
  stack[0].ival = value;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_env_get_field_float(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* test_case = stack[0].oval;
  
  int32_t field_id = env->get_field_id(env, test_case, "x_float", "float");
  if (field_id < 0) {
    return 1;
  }
  
  float value = env->get_field_float(env, test_case, field_id);
  
  stack[0].fval = value;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_env_get_field_double(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* test_case = stack[0].oval;
  
  int32_t field_id = env->get_field_id(env, test_case, "x_double", "double");
  if (field_id < 0) {
    return 1;
  }
  
  double value = env->get_field_double(env, test_case, field_id);
  
  stack[0].ival = value;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_env_get_field_object(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* test_case = stack[0].oval;
  
  int32_t field_id = env->get_field_id(env, test_case, "minimal", "TestCase::Minimal");
  if (field_id < 0) {
    return 1;
  }
  
  void* value = env->get_field_object(env, test_case, field_id);
  
  stack[0].oval = value;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__sum(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t total = stack[0].ival + stack[1].ival;
  
  stack[0].ival = total;
  
  return 0;
}


int32_t SPVM__TestCase__NativeAPI__add_int_array(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* obj_nums1 = stack[0].oval;
  void* obj_nums2 = stack[1].oval;
  
  int32_t length = env->length(env, obj_nums1);
  
  int32_t* nums1 = env->get_elems_int(env, obj_nums1);
  int32_t* nums2 = env->get_elems_int(env, obj_nums2);
  
  void* onums3 = env->new_int_array(env, length);
  int32_t* nums3 = env->get_elems_int(env, onums3);
  
  for (int32_t i = 0; i < length; i++) {
    nums3[i] = nums1[i] + nums2[i];
  }
  
  stack[0].oval = onums3;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__call_void_method_exception_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  return env->die(env, "Exception %s", "ppp", "TestCase/NativeAPI.native/NativeAPI.c", __LINE__);
}

int32_t SPVM__TestCase__NativeAPI__call_byte_method_exception_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  return env->die(env, "Exception", "TestCase/NativeAPI.native/NativeAPI.c", __LINE__);
}

int32_t SPVM__TestCase__NativeAPI__call_short_method_exception_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;

  return env->die(env, "Exception", "TestCase/NativeAPI.native/NativeAPI.c", __LINE__);
}

int32_t SPVM__TestCase__NativeAPI__call_int_method_exception_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  return env->die(env, "Exception", "TestCase/NativeAPI.native/NativeAPI.c", __LINE__);
}

int32_t SPVM__TestCase__NativeAPI__call_long_method_exception_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  return env->die(env, "Exception", "TestCase/NativeAPI.native/NativeAPI.c", __LINE__);
}

float SPVM__TestCase__NativeAPI__call_float_method_exception_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  return env->die(env, "Exception", "TestCase/NativeAPI.native/NativeAPI.c", __LINE__);
}

int32_t SPVM__TestCase__NativeAPI__call_double_method_exception_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  return env->die(env, "Exception", "TestCase/NativeAPI.native/NativeAPI.c", __LINE__);
}

int32_t SPVM__TestCase__NativeAPI__call_object_method_exception_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  return env->die(env, "Exception", "TestCase/NativeAPI.native/NativeAPI.c", __LINE__);
}

int32_t SPVM__TestCase__NativeAPI__mortal_api(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  // Check if object count is zero in extension.t
  int32_t ref_count = 0;
  
  int32_t length = 10;
  // 1
  {
    void* sp_values = env->new_byte_array(env, length);
    ref_count += env->get_ref_count(env, sp_values);
  }
  // 2
  {
    void* sp_values = env->new_short_array(env, length);
    ref_count += env->get_ref_count(env, sp_values);
  }
  // 3
  {
    void* sp_values = env->new_int_array(env, length);
    ref_count += env->get_ref_count(env, sp_values);
  }
  // 4
  {
    void* sp_values = env->new_long_array(env, length);
    ref_count += env->get_ref_count(env, sp_values);
  }
  // 5
  {
    void* sp_values = env->new_float_array(env, length);
    ref_count += env->get_ref_count(env, sp_values);
  }
  // 6
  {
    void* sp_values = env->new_long_array(env, length);
    ref_count += env->get_ref_count(env, sp_values);
  }
  // 7
  {
    void* sp_values = env->new_string(env, "foo", (int32_t)strlen("foo"));
    ref_count += env->get_ref_count(env, sp_values);
  }
  // 8
  {
    int32_t basic_type_id = env->get_basic_type_id(env, "TestCase::Minimal");
    if (basic_type_id < 0) {
      return 1;
    }
    void* sp_object = env->new_object(env, basic_type_id);
    ref_count += env->get_ref_count(env, sp_object);
  }
  // 9
  {
    int32_t basic_type_id = env->get_basic_type_id(env, "TestCase::Minimal");
    if (basic_type_id < 0) {
      return 1;
    }
    void* sp_objects = env->new_object_array(env, basic_type_id, 3);
    ref_count += env->get_ref_count(env, sp_objects);
  }
  // 10
  {
    int32_t basic_type_id = env->get_basic_type_id(env, "TestCase::Minimal");
    if (basic_type_id < 0) {
      return 1;
    }
    void* sp_objects = env->new_object_array(env, basic_type_id, 3);
    ref_count += env->get_ref_count(env, sp_objects);
  }
  // 11
  {
    int32_t basic_type_id = env->get_basic_type_id(env, "TestCase::Pointer");
    if (basic_type_id < 0) {
      return 1;
    }
    void* sp_objects = env->new_pointer(env, basic_type_id, NULL);
    ref_count += env->get_ref_count(env, sp_objects);
  }
  
  if (ref_count == 11) {
    stack[0].ival = 1;
  }
  else {
    stack[0].ival = 0;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__enter_scope_leave_scope(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t length = 10;
  int32_t start_memory_blocks_count = env->get_memory_blocks_count(env);
  env->new_int_array(env, length);
  env->new_int_array(env, length);
  int32_t before_enter_memory_blocks_count = env->get_memory_blocks_count(env);
  int32_t before_leave_memory_blocks_count;
  {
    int32_t scope_id = env->enter_scope(env);

    env->new_int_array(env, length);
    env->new_int_array(env, length);
    env->new_int_array(env, length);
    
    before_leave_memory_blocks_count = env->get_memory_blocks_count(env);
    env->leave_scope(env, scope_id);
  }
  
  int32_t after_leave_memory_blocks_counts = env->get_memory_blocks_count(env);
  
  stack[0].ival = 0;
  if ((before_enter_memory_blocks_count - start_memory_blocks_count) == 2) {
    if (before_enter_memory_blocks_count == after_leave_memory_blocks_counts) {
      if ((before_leave_memory_blocks_count - before_enter_memory_blocks_count) == 3) {
        stack[0].ival = 1;
      }
    }
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_call_spvm_method(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t method_id = env->get_class_method_id(env, "TestCase::NativeAPI", "my_value", "int(int)");
  if (method_id < 0) {
    return 1;
  }
  int32_t output;
  {
    stack[0].ival = 5;
    int32_t exception_flag = env->call_spvm_method(env, method_id, stack);
    if (exception_flag) {
      return 1;
    }
    output = stack[0].ival;
  }
  
  stack[0].ival = 0;
  
  if (output == 5) {
    stack[0].ival = 1;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_call_class_method(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t method_id = env->get_class_method_id(env, "TestCase::NativeAPI", "my_value", "int(int)");
  if (method_id < 0) {
    return 1;
  }
  int32_t output;
  {
    stack[0].ival = 5;
    int32_t exception_flag = env->call_class_method(env, method_id, stack);
    if (exception_flag) {
      return 1;
    }
    output = stack[0].ival;
  }
  
  stack[0].ival = 0;
  
  if (output == 5) {
    stack[0].ival = 1;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_call_instance_method(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t method_id = env->get_class_method_id(env, "TestCase::NativeAPI", "my_value", "int(int)");
  if (method_id < 0) {
    return 1;
  }
  int32_t output;
  {
    stack[0].ival = 5;
    int32_t exception_flag = env->call_instance_method(env, method_id, stack);
    if (exception_flag) {
      return 1;
    }
    output = stack[0].ival;
  }
  
  stack[0].ival = 0;
  
  if (output == 5) {
    stack[0].ival = 1;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_call_class_method_by_name(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t output;
  {
    stack[0].ival = 5;
    int32_t exception_flag = env->call_class_method_by_name(env, "TestCase::NativeAPI", "my_value", "int(int)", stack, MFILE, __LINE__);
    if (exception_flag) {
      return exception_flag;
    }
    output = stack[0].ival;
  }
  
  stack[0].ival = 0;
  
  if (output == 5) {
    stack[0].ival = 1;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__native_call_class_method_by_name_exception(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t output;
  {
    stack[0].ival = 5;
    int32_t exception_flag = env->call_class_method_by_name(env, "TestCase::NativeAPI", "not_found", "int(int)", stack, MFILE, __LINE__);
    if (exception_flag) {
      return exception_flag;
    }
    output = stack[0].ival;
  }
  
  stack[0].ival = 0;
  
  if (output == 5) {
    stack[0].ival = 1;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__call_instance_method_by_name_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  void* minimal = stack[0].oval;
  
  int32_t output;
  {
    stack[0].oval = minimal;
    int32_t exception_flag = env->call_instance_method_by_name(env, minimal, "x", "int(self)", stack, MFILE, __LINE__);
    if (e) { return e; }
    output = stack[0].ival;
  }
  
  stack[0].ival = output;
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__call_instance_method_by_name_exception_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;

  void* minimal = stack[0].oval;
  
  int32_t output;
  {
    stack[0].oval = minimal;
    e = env->call_instance_method_by_name(env, minimal, "not_found", "int(self)", stack, MFILE, __LINE__);
    if (e) { return e; };
    output = stack[0].ival;
  }
  
  stack[0].ival = 0;
  
  if (output == 5) {
    stack[0].ival = 1;
  }
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__push_mortal_multi(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  void* iarray = env->new_int_array_raw(env, 10);
  
  env->push_mortal(env, iarray);
  env->push_mortal(env, iarray);
  
  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_instance_method_id_static_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  void* minimal = stack[0].oval;
  
  int32_t method_id = env->get_instance_method_id_static(env, "TestCase::Minimal", "x", "int(self)");
  if (method_id < 0) { return 0; }
  
  int32_t ret;
  {
    env->call_spvm_method(env, method_id, stack);
    if (e) { return e; }
    ret = stack[0].ival;
  }
  
  stack[0].ival = ret;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__get_bool_object_value_native(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  void* bool_object = stack[0].oval;
  
  int32_t value = env->get_bool_object_value(env, bool_object);
  
  stack[0].ival = value;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__new_string_raw(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  // Basic
  {
    void* string = env->new_string_raw(env, "abc", 3);

    int32_t string_basic_type_id = env->get_object_basic_type_id(env, string);
    int32_t string_type_dimension = env->get_object_type_dimension(env, string);
    if (!(string_basic_type_id == (intptr_t)env->string_basic_type_id && string_type_dimension == 0)) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }

    if (env->length(env, string) != 3) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strcmp(string_chars, "abc") != 0) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    env->inc_ref_count(env, string);
    env->dec_ref_count(env, string);
  }

  // Length is shorten than the string
  {
    void* string = env->new_string_raw(env, "abc", 1);

    if (env->length(env, string) != 1) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strcmp(string_chars, "a") != 0) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    env->inc_ref_count(env, string);
    env->dec_ref_count(env, string);
  }

  // Length is shorten than the string 0
  {
    void* string = env->new_string_raw(env, "abc", 0);

    if (env->length(env, string) != 0) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strcmp(string_chars, "") != 0) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    env->inc_ref_count(env, string);
    env->dec_ref_count(env, string);
  }

  // Length is longer than the string
  {
    void* string = env->new_string_raw(env, "abc", 4);

    if (env->length(env, string) != 4) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strncmp(string_chars, "abc\0\0", 5) != 0) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    env->inc_ref_count(env, string);
    env->dec_ref_count(env, string);
  }

  // NULL
  {
    void* string = env->new_string_raw(env, NULL, 4);

    if (env->length(env, string) != 4) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strncmp(string_chars, "\0\0\0\0\0", 5) != 0) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    env->inc_ref_count(env, string);
    env->dec_ref_count(env, string);
  }

  // NULL 0
  {
    void* string = env->new_string_raw(env, NULL, 0);

    if (env->length(env, string) != 0) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strncmp(string_chars, "\0", 1) != 0) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    env->inc_ref_count(env, string);
    env->dec_ref_count(env, string);
  }


  stack[0].ival = 1;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__new_string(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  // Basic
  {
    void* string = env->new_string(env, "abc", 3);

    int32_t string_basic_type_id = env->get_object_basic_type_id(env, string);
    int32_t string_type_dimension = env->get_object_type_dimension(env, string);
    if (!(string_basic_type_id == (intptr_t)env->string_basic_type_id && string_type_dimension == 0)) {
      stack[0].ival = 0;
      return 0;
    }

    if (env->length(env, string) != 3) {
      stack[0].ival = 0;
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strcmp(string_chars, "abc") != 0) {
      stack[0].ival = 0;
      return 0;
    }
  }

  // Length is shorten than the string
  {
    void* string = env->new_string(env, "abc", 1);

    if (env->length(env, string) != 1) {
      stack[0].ival = 0;
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strcmp(string_chars, "a") != 0) {
      stack[0].ival = 0;
      return 0;
    }
  }

  // Length is shorten than the string 0
  {
    void* string = env->new_string(env, "abc", 0);

    if (env->length(env, string) != 0) {
      stack[0].ival = 0;
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strcmp(string_chars, "") != 0) {
      stack[0].ival = 0;
      return 0;
    }
  }

  // Length is longer than the string
  {
    void* string = env->new_string(env, "abc", 4);

    if (env->length(env, string) != 4) {
      stack[0].ival = 0;
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strncmp(string_chars, "abc\0\0", 5) != 0) {
      stack[0].ival = 0;
      return 0;
    }
  }

  // NULL
  {
    void* string = env->new_string(env, NULL, 4);

    if (env->length(env, string) != 4) {
      stack[0].ival = 0;
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strncmp(string_chars, "\0\0\0\0\0", 5) != 0) {
      stack[0].ival = 0;
      return 0;
    }
  }

  // NULL 0
  {
    void* string = env->new_string(env, NULL, 0);

    if (env->length(env, string) != 0) {
      stack[0].ival = 0;
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strncmp(string_chars, "\0", 1) != 0) {
      stack[0].ival = 0;
      return 0;
    }
  }

  stack[0].ival = 1;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__new_string_nolen_raw(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  // Basic
  {
    void* string = env->new_string_nolen_raw(env, "abc");

    int32_t string_basic_type_id = env->get_object_basic_type_id(env, string);
    int32_t string_type_dimension = env->get_object_type_dimension(env, string);
    if (!(string_basic_type_id == (intptr_t)env->string_basic_type_id && string_type_dimension == 0)) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }

    if (env->length(env, string) != 3) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strcmp(string_chars, "abc") != 0) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    env->inc_ref_count(env, string);
    env->dec_ref_count(env, string);
  }

  // ""
  {
    void* string = env->new_string_nolen_raw(env, "");

    if (env->length(env, string) != 0) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strcmp(string_chars, "") != 0) {
      stack[0].ival = 0;
      env->inc_ref_count(env, string);
      env->dec_ref_count(env, string);
      return 0;
    }
    
    env->inc_ref_count(env, string);
    env->dec_ref_count(env, string);
  }

  stack[0].ival = 1;

  return 0;
}

int32_t SPVM__TestCase__NativeAPI__new_string_nolen(SPVM_ENV* env, SPVM_VALUE* stack) {
  (void)env;
  (void)stack;
  
  int32_t e;
  
  // Basic
  {
    void* string = env->new_string_nolen(env, "abc");

    int32_t string_basic_type_id = env->get_object_basic_type_id(env, string);
    int32_t string_type_dimension = env->get_object_type_dimension(env, string);
    if (!(string_basic_type_id == (intptr_t)env->string_basic_type_id && string_type_dimension == 0)) {
      stack[0].ival = 0;
      return 0;
    }

    if (env->length(env, string) != 3) {
      stack[0].ival = 0;
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strcmp(string_chars, "abc") != 0) {
      stack[0].ival = 0;
      return 0;
    }
  }

  // ""
  {
    void* string = env->new_string_nolen(env, "");

    if (env->length(env, string) != 0) {
      stack[0].ival = 0;
      return 0;
    }
    
    const char* string_chars = env->get_chars(env, string);
    if (strcmp(string_chars, "") != 0) {
      stack[0].ival = 0;
      return 0;
    }
  }

  stack[0].ival = 1;

  return 0;
}
