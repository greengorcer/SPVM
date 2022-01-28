=head1 NAME

SPVM::Document::NativeAPI - SPVM Native APIs

=head1 DESCRIPTION

SPVM Native APIs is C APIs used in SPVM native method. This document describes the way to define native methods, and shows the all of SPVM Native APIs. If you want to know the list of Native APIs, see L<List of Native APIs|/"List-of-Native-APIs">.

Native method can be written by C language or C++, If the code is compatible with C language or C++(for example, CUDA/nvcc), it can be compiled into native method. If you see examples of SPVM Native APIs, see L<Examples using SPVM Native APIs|https://github.com/yuki-kimoto/SPVM/tree/master/examples/native>. This contains the examples of C language, C++ and CUDA/nvcc.

=head1 Defintion of SPVM Native Method

=head2 Native Method Declaration

Native Method Declaration is written using Method Descriptor "native" in SPVM module file. SPVM Native Method Declaration ends with a semicolon without Sobroutine Block.

  # SPVM/Foo/Bar.spvm
  class Foo::Bar {
    native static method sum : int ($num1 : int, $num2 : int);
  }

=head2 SPVM Native Config File

SPVM Native Config File must be created for SPVM Native Method. The base name without the extension of native config file must be same as SPVM module file and the extension must be ".config".

  # Native configuration file for Foo::Bar module
  SPVM/Foo/Bar.config

If native configuration file does not exist, an exception occurs.

Native Config File is Perl source code. Native Config File must return properly L<Builder::Config|SPVM::Builder::Config> object, otherwise an exception occurs.

=head3 C99 Config File Example

  # C99 Config File
  use strict;
  use warnings;

  use SPVM::Builder::Config;
  my $config = SPVM::Builder::Config->new_gnu99;

  $config;

=head3 C11 Config File Example

  # C11 Config File
  use strict;
  use warnings;

  use SPVM::Builder::Config;
  my $config = SPVM::Builder::Config->new_c;

  $config->set_std('c11');

  $config;

=head3 C++ Config File Example

  # C++ Config File
  use strict;
  use warnings;

  use SPVM::Builder::Config;
  my $config = SPVM::Builder::Config->new_cpp;

  $config;

=head3 C++11 Config File Example

  # C++11 Config File
  use strict;
  use warnings;

  use SPVM::Builder::Config;
  my $config = SPVM::Builder::Config->new_cpp;

  $config->set_std('c++11');

  $config;

=head3 CUDA/nvcc Config File Example

  use strict;
  use warnings;

  my $config = SPVM::Builder::Config->new;

  # Compiler and Linker common
  my @ccldflags = qw(--compiler-options '-fPIC');

  # Compiler
  $config->cc('nvcc');
  $config->add_ccflags(@ccldflags);
  $config->ext('cu');

  # Linker
  $config->ld('nvcc');
  $config->add_ldflags('-shared', @ccldflags);

  $config;

=head3 Show the compile commands

  # C99 Config File
  use strict;
  use warnings;

  use SPVM::Builder::Config;
  my $config = SPVM::Builder::Config->new_gnu99;

  # Show the compile commands
  $config->quiet(0);

  $config;

=head3 Force the compiles

  use strict;
  use warnings;

  use SPVM::Builder::Config;
  my $config = SPVM::Builder::Config->new_gnu99;

  # Show the compile commands
  $config->force(1);

  $config;

=head2 Native Method Definition

Native Method Definition is written in native source file. Native source file is basically C language source file which extension is ".c". This extension can be changed to ".cpp" for C++ source file, or ".cu" for CUDA source file, etc.

  # Native source file for Foo::Bar module
  SPVM/Foo/Bar.c

The following is natvie source file example written by C language.

  #include "spvm_native.h"

  int32_t SPVM__Foo__Bar__sum(SPVM_ENV* env, SPVM_VALUE* stack) {

    int32_t num1 = stack[0].ival;
    int32_t num2 = stakc[1].ival;

    int32_t total = num1 + num2;

    stack[0].ival = total;

    return 0;
  }

=head3 Include spvm_native.h

Include "spvm_native.h" at the beginning of the native source file. This header file defines SPVM Native APIs and data structures for Native APIs.

spvm_native.h include the following types.

  SPVM_ENV
  SPVM_VALUE

See L<List of Native APIs|/"List-of-Native-APIs"> about included SPVM Native APIs.

=head3 Return Value

The return type is "int32_t" type. If the method raises an exception, "1" is returned. If the method is succeed "0" is returned.

=head3 Function Name

Native Method Definition is a simple C language function such as

  SPVM__Foo__Bar__sum

The function name starts with "SPVM__".

Followed by class name "Foo__Bar", which is replaced "::" in Foo::Bar.

Followed by "__".

Followed by method name "sum".

If Native Method Name is invalid, a compile error will occur.

There are two arguments, the first argument is "SPVM_ENV* env" which has the information of the execution environment, and the second argument is "SPVM_VALUE* stack" which is used for the argument and return value.

  int32_t SPVM__Foo__Bar__sum(SPVM_ENV* env, SPVM_VALUE* stack) {
  
  }

In the above sample, it takes two int type arguments of SPVM, calculates the sum, and returns the return value.

  #include "spvm_native.h"

  int32_t SPVM__Foo__Bar__sum(SPVM_ENV* env, SPVM_VALUE* stack) {

    int32_t num1 = stack[0].ival;
    int32_t num2 = stakc[1].ival;

    int32_t total = num1 + num2;

    stack[0].ival = total;

    return 0;
  }

=head2 Compile Native Method

Native methods are compiled into a shared libraries. teay are shared libraries (.so) on Unix/Linux, dynamic link libraries (.dll) on Windows or etc corresponding to your os.

The compilation is done when SPVM is compiled. The build directory must exist, otherwise an exception occures.

The default build directory is the "~/.spvm_build" directory in the directory containing the executed Perl script, and can be changed with the environment variable "SPVM_BUILD_DIR".

If you want to use SPVM Native Method from Perl, create a "~/.spvm_build" directory in the directory where the executed Perl script exists.

  ~/.spvm_build

The generated object files exists under "work/object" under the build directory. The object file name is the name which the extension of the SPVM module name is changed to ".o".

  ~/.spvm_build/work/object/Foo/Bar.o

The generated shared libraries exists under "work/lib" under the build directory. The name of shared library is the name which the extension of the SPVM module name is changed to ".so", or etc corresponding to your os.

  # Unix/Linux
  ~/.spvm_build/work/object/Foo/Bar.so

  # Windows
  ~/.spvm_build/work/object/Foo/Bar.dll

=head2 Stack

The stack is the second argument of the definition of the Native Method. This is called stack. Stack is used getting arguments and return the value.

  int32_t SPVM__Foo__Bar__sum(SPVM_ENV* env, SPVM_VALUE* stack) {

  }

SPVM_VALUE is a union type of C language to store SPVM values. You can save integral value, floating point value, object value, and reference value to it.

For example, to get the value of the first argument(0th) of int type, write as follows.

  int32_t args0 = stack[0].ival;

For example, to get the value of the second argument(1th) of long type, write as follows.

  int64_t args1 = stack[1].lval;

For example, to return a value of double type, write as follows.

  stack[0].dval = 0.5;
  
=head2 Getting Arguments

=head3 Get byte type argument

To get the SPVM byte argument, access the bval field. Assign to the C language int8_t type.

  int8_t args0 = stack[0].bval;

=head3 Get short type argument

To get the short argument of SPVM, access the sval field. Assign it to the C language int16_t type.

  int16_t args0 = stack[0].sval;

=head3 Get int type argument

To get the SPVM int type argument, access the ival field. Assign to the C language int32_t type.

  int32_t args0 = stack[0].ival;

=head3 Get long type argument

To get the long argument of SPVM, access the lval field. Assign to the C language int64_t type.

  int64_t args0 = stack[0].lval;

=head3 Get float type argument

To get the SPVM float type argument, access the fval field. Assign to float type of C language.

  float args0 = stack[0].fval;

=head3 Get double type argument

To get the SPVM double argument, access the dval field. Assign to the C language double type.

  double args0 = stack[0].dval;

=head3 Get object type argument

To get the SPVM object type argument, access the oval field. Assign it to void* type in C language.

  void* args0 = stack[0].oval;

=head3 Get byte Reference Type Argument

If you get SPVM byte Reference Type argument, use "bref" field. it can be assinged to the value of C language int8_t* type.

  int8_t* args0 = stack[0].bref;

=head3 Get short Reference Type Argument

If you get SPVM short Reference Type argument, use "sref" field. it can be assinged to the value of C language int16_t* type.

  int16_t* args0 = stack[0].sref;

=head3 Get int Reference Type Argument

If you get SPVM int Reference Type argument, use "iref" field. it can be assinged to the value of C language int32_t* type.

  int32_t* args0 = stack[0].iref;

=head3 Get long Reference Type Argument

If you get SPVM long Reference Type argument, use "lref" field. it can be assinged to the value of C language int64_t* type.

  int64_t* args0 = stack[0].lref;

=head3 Get float Reference Type Argument

If you get SPVM float Reference Type argument, use "fref" field. it can be assinged to the value of C language float* type.

  float* args0 = stack[0].fref;

=head3 Get double Reference Type Argument

If you get SPVM double Reference Type Argument, use "dref" field. it can be assinged to the value of C language double* type.

  double* args0 = stack[0].dref;

=head3 Get multiple numeric type arguments

In a Native Method, multiple numeric type arguments are assigned to the coresponding multiple arguments.

For example, In the case of the argument values of L<Complex_2d|SPVM::Complex_2d> type, you can get them by the following way.

  double args_re = stack[0].dval;
  double args_im = stack[1].dval;

Note that you cannot access the values by the field name of L<Complex_2d|SPVM::Complex_2d>.


=head2 Return Value
  
=head3 Set return value of byte type

Use C<bval> field of C<SPVM_VALUE> to set a return value which type of SPVM is C<byte>. This is corresponding to C<int8_t> type of C language.

  int8_t retval;
  stack[0].bval = retval;

=head3 Set return value of short type

Use C<sval> field of C<SPVM_VALUE> to set a return value which type of SPVM is C<short>. This is corresponding to C<int16_t> type of C language.

  int16_t retval;
  stack[0].sval = retval;

=head3 Set return value of int type

Use C<ival> field of C<SPVM_VALUE> to set a return value which type of SPVM is C<int>. This is corresponding to C<int32_t> type of C language.

  int32_t retval;
  stack[0].ival = retval;

=head3 Set long type return value

Use C<lval> field of C<SPVM_VALUE> to set a return value which type of SPVM is C<long>. This is corresponding to C<int64_t> type of C language.

  int64_t retval;
  stack[0].lval = retval;

=head3 Set return value of float type

Use C<fval> field of C<SPVM_VALUE> to set a return value which type of SPVM is C<float>. This is corresponding to C<float> type of C language.

  float retval;
  stack[0].fval = retval;

=head3 Set return value of double type

Use C<dval> field of C<SPVM_VALUE> to set a return value which type of SPVM is C<double>. This is corresponding to C<double> type of C language.

  double retval;
  stack[0].dval = retval;

=head3 Set return value of object type

Use C<oval> field of C<SPVM_VALUE> to set a return value which type of SPVM is object. This is corresponding to C<void*> type of C language.

  void* retval;
  stack[0].oval = retval;

=head3 Set multiple numeric return value

If you set multiple numeric return value in native method, set multiple return values.

For example, in the case of L<Complex_2d|SPVM::Complex_2d>, do the following.

  double retval_x;
  double retval_y;
  stack[0].dval = retval_x;
  stack[1].dval = retval_y;

=head2 Call SPVM Method

If you want to call a method, you get a method id using L<get_class_method_id|"get_class_method_id"> or L<get_instance_method_id|"get_instance_method_id">.

L<get_class_method_id|"get_class_method_id"> get a method id of a class method.

L<get_instance_method_id|"get_instance_method_id"> get a method id of a instance method.

  // Get method id of class method
  int32_t method_id = env->get_class_method_id(env, "Foo", "sum", "int(int,int)");

  // Get method id of instance method
  int32_t method_id = env->get_instance_method_id(env, object, "sum", "int(int,int)");

If method_id is less than 0, it means that the method was not found. It is safe to handle exceptions as follows.

  if (method_id < 0) { return env->die(env, "Can't find method id", "Foo/Bar.c", __LINE__); }

Set the SPVM method argument to stack before calling the method.

  stack[0].ival = 1;
  stack[0].ival = 2;

To call a SPVM method, use the <a href="#native-api-native-sub-api-call_spvm_method">call_spvm_method</a> function.

  int32_t exception_flag = env->call_spvm_method(env, method_id, stack);

Nonzero if the method raised an exception, 0 if no exception occurred.

The return value of the method is stored in the first element of the stack.

  int32_t total = stack[0].ival;

=head2 Native Method Scope

Native method are entirely enclosed in scope.

Objects added to the mortal stack will automatically have their reference count decremented by 1 when the Native Method ends. When the reference count reaches 0, it is released.

Use push_mortal to add objects to the mortal stack.

  env->push_mortal(env, object);

Native APIs that normally create an object such as "new_object" will add the automatically created object to the mortal stack so you don't need to use this.

Use "enter_scope" to create a scope. The return value is the ID of that scope.

  int32_t scope_id = env->enter_scope (env);

Use "leave_scope" to leave the scope. For the argument, it is necessary to specify the scope ID obtained in "enter_scope".

  env->leave_scope(env, scope_id);

Use "remove_mortal" to remove the object from the mortal stack. For the argument, specify the scope ID obtained by "enter_scope" and the object you want to remove. The object is removed from the mortal stack and the reference count is automatically decremented by 1. When the reference count reaches 0, it is released.

  env->remove_mortal(env, scope_id, object);

Information about the mortal stack is stored in env.

=head2 Exception in Native Method

In the Native Method, it is the return value that indicates whether an exception has occurred.

  return 0;

  return 1;

If no exception occurs, "0" is returned. This is defined as "0".

If an exception occurs, "1" is returned. It is defined as a value other than "0".

If you want to set the exception message yourself, you can create an exception message with "new_string_nolen" and set it with "set_exception".

  env->set_exception(env, env->new_string_nolen(env, "Exception occur");
  return 1;

If no exception message is set, a default exception message will be set.

Usually, L<die|"die"">  is defined to make it easier to use, so it is better to use this.

  return env->die("Error. Values must be %d and %d", 3, 5, "Foo/Bar.c", __LINE__);

L<die|"die""> can be used in the same way as the C language sprintf function. Be sure to include this file name in the second from the end, and the line number in the last argument. If the message exceeds 255 bytes, the excess is truncated.

The exception is stored in env.

=head2 Pointer Type

There is a type called pointer type in SPVM, but I will explain how to use it.

The pointer type definition specifies the pointer_t descriptor in the SPVM class definition. Pointer types cannot have field definitions. This example describes how to use the C standard "struct tm" as a pointer type.

  # SPVM/MyTimeInfo.spvm
  class MyTimeInfo : pointer_t {

    # Constructor
    native static method new : MyTimeInfo ();

    # Get second
    native method sec : int ();

    # Destructor
    native method DESTROY : ();
  }

It defines a new constructor, a method that takes seconds information called sec, and a destructor called DESTROY. These are Native Method.

Next is the definition on the C language side.

  # SPVM/MyTimeInfo.c

  int32_t SPVM__MyTimeInfo__new(SPVM_ENV* env, SPVM_VALUE* stack) {

    // Alloc strcut tm
    void* tm_ptr = env->alloc_memory_block_zero (sizeof (struct tm));

    // Create strcut tm instance
    void* tm_obj = env->new_pointer(env, "MyTimeInfo", tm_ptr);

    stack[0].oval = tm_obj;

    return 0;
  }

  int32_t SPVM__MyTimeInfo__sec(SPVM_ENV* env, SPVM_VALUE* stack) {
    void* tm_obj = stack[0].oval;

    strcut tm* tm_ptr = (struct tm*) env->get_pointer(env, tm_obj);

    stack[0].ival = tm_ptr-> tm_sec;

    return 0;
  }

  int32_t SPVM__MyTimeInfo__DESTROY(SPVM_ENV* env, SPVM_VALUE* stack) {

    void* tm_obj = stack[0].oval;
    strcut tm* tm_ptr = (struct tm*) env->get_pointer(env, tm_obj);

    env->free_memory_block (tm_ptr);

    return 0;
  }

In the constructor new, the memory of "struct tm" is first allocated by the alloc_memory_block_zero function. This is a function that reserves one memory block in SPVM. Similar to malloc, this function increments the memory block count by one, making it easier to spot memory leaks.

  // Alloc strcut tm
  void* tm_ptr = env->alloc_memory_block_zero (sizeof (struct tm));

Next, use the new_pointer function to create a new pointer type object with MyTimeInfo associated with it in the allocated memory.

  // Create strcut tm instance
  void* tm_obj = env->new_pointer(env, "MyTimeInfo", tm_ptr);

If you return this as a return value, the constructor is complete.

  stack[0].ival = tm_ptr-> tm_sec;
  
  return 0;

Next, let's get the value of tm_sec. sec method. The get_pointer function can be used to get a pointer to the memory allocated as a "struct tm" from a pointer type object.

  void* tm_obj = stack[0].oval;

  strcut tm* tm_ptr = (struct tm*) env->get_pointer(env, tm_obj);

  stack[0].ival = tm_ptr-> tm_sec;

The last is the destructor. Be sure to define a destructor, as the allocated memory will not be released automatically.

  int32_t SPVM__MyTimeInfo__DESTROY(SPVM_ENV* env, SPVM_VALUE* stack) {

    void* tm_obj = stack[0].oval;

    strcut tm* tm_ptr = (struct tm*) env->get_pointer(env, tm_obj);

    env->free_memory_block (tm_ptr);

    return 0;
  }

Execute the free_memory_block function to free the memory. Be sure to free the memory allocated by alloc_memory_block_zero with the free_memory_block function. Releases the memory and decrements the memory block count by one.

=head2 Call Native API

Native API can be called from "SPVM_ENV* env" passed as an argument. Note that you have to pass env as the first argument.

  int32_t basic_type_id = env->get_basic_type_id(env, "Int");

=head1 IDs of Native API

Native APIs of L<SPVM> have the IDs that is corresponding to the names. These IDs are permanently same for the binary compatibility in the future release. When a new Native API is added, it will be added after the end of IDs.

  0 class_vars_heap
  1 object_header_byte_size
  2 object_weaken_backref_head_offset
  3 object_ref_count_offset
  4 object_basic_type_id_offset
  5 object_type_dimension_offset
  6 object_type_category_offset
  7 object_flag_offset
  8 object_length_offset
  9 byte_object_basic_type_id
  10 short_object_basic_type_id
  11 int_object_basic_type_id
  12 long_object_basic_type_id
  13 float_object_basic_type_id
  14 double_object_basic_type_id
  15 compiler
  16 exception_object
  17 native_mortal_stack
  18 native_mortal_stack_top
  19 native_mortal_stack_capacity
  20 get_basic_type_id
  21 get_field_id
  22 get_field_offset
  23 get_class_var_id
  24 get_class_method_id
  25 get_instance_method_id
  26 new_object_raw
  27 new_object
  28 new_byte_array_raw
  29 new_byte_array
  30 new_short_array_raw
  31 new_short_array
  32 new_int_array_raw
  33 new_int_array
  34 new_long_array_raw
  35 new_long_array
  36 new_float_array_raw
  37 new_float_array
  38 new_double_array_raw
  39 new_double_array
  40 new_object_array_raw
  41 new_object_array
  42 new_muldim_array_raw
  43 new_muldim_array
  44 new_mulnum_array_raw
  45 new_mulnum_array
  46 new_string_nolen_raw
  47 new_string_nolen
  48 new_string_raw
  49 new_string
  50 new_pointer_raw
  51 new_pointer
  52 concat_raw
  53 concat
  54 new_stack_trace_raw
  55 new_stack_trace
  56 length
  57 get_elems_byte
  58 get_elems_short
  59 get_elems_int
  60 get_elems_long
  61 get_elems_float
  62 get_elems_double
  63 get_elem_object
  64 set_elem_object
  65 get_field_byte
  66 get_field_short
  67 get_field_int
  68 get_field_long
  69 get_field_float
  70 get_field_double
  71 get_field_object
  72 set_field_byte
  73 set_field_short
  74 set_field_int
  75 set_field_long
  76 set_field_float
  77 set_field_double
  78 set_field_object
  79 get_class_var_byte
  80 get_class_var_short
  81 get_class_var_int
  82 get_class_var_long
  83 get_class_var_float
  84 get_class_var_double
  85 get_class_var_object
  86 set_class_var_byte
  87 set_class_var_short
  88 set_class_var_int
  89 set_class_var_long
  90 set_class_var_float
  91 set_class_var_double
  92 set_class_var_object
  93 get_pointer
  94 set_pointer
  95 call_spvm_method
  96 get_exception
  97 set_exception
  98 get_ref_count
  99 inc_ref_count
  100 dec_ref_count
  101 enter_scope
  102 push_mortal
  103 leave_scope
  104 remove_mortal
  105 is_type
  106 has_callback
  107 get_object_basic_type_id
  108 get_object_type_dimension
  109 weaken
  110 isweak
  111 unweaken
  112 alloc_memory_block_zero
  113 free_memory_block
  114 get_memory_blocks_count
  115 get_type_name_raw
  116 get_type_name
  117 new_env
  118 free_env
  119 memory_blocks_count
  120 get_chars
  121 die
  122 new_object_by_name
  123 new_pointer_by_name
  124 set_field_byte_by_name
  125 set_field_short_by_name
  126 set_field_int_by_name
  127 set_field_long_by_name
  128 set_field_float_by_name
  129 set_field_double_by_name
  130 set_field_object_by_name
  131 get_field_byte_by_name
  132 get_field_short_by_name
  133 get_field_int_by_name
  134 get_field_long_by_name
  135 get_field_float_by_name
  136 get_field_double_by_name
  137 get_field_object_by_name
  138 set_class_var_byte_by_name
  139 set_class_var_short_by_name
  140 set_class_var_int_by_name
  141 set_class_var_long_by_name
  142 set_class_var_float_by_name
  143 set_class_var_double_by_name
  144 set_class_var_object_by_name
  145 get_class_var_byte_by_name
  146 get_class_var_short_by_name
  147 get_class_var_int_by_name
  148 get_class_var_long_by_name
  149 get_class_var_float_by_name
  150 get_class_var_double_by_name
  151 get_class_var_object_by_name
  152 call_class_method_by_name
  153 call_instance_method_by_name
  154 get_field_string_chars_by_name
  155 any_object_basic_type_id
  156 dump_raw
  157 dump
  158 call_class_method
  159 call_instance_method
  160 get_instance_method_id_static
  161 get_bool_object_value
  
=head1 List of Native APIs

List of Native APIs of L<SPVM>.

=head2 class_vars_heap

  void* class_vars_heap;

the pointer to the storage area of the class variables. This is used internally.

=head2 object_header_byte_size

  void* object_header_byte_size;

The byte size of the object's header. This is used internally.

=head2 object_weaken_backref_head_offset

  void* object_weaken_backref_head_offset;

Offset to a pointer to the back reference of the weak reference in the object structure. This is used internally.

=head2 object_ref_count_offset

  void* object_ref_count_offset;

Reference count offset in the object structure. This is used internally.

=head2 object_basic_type_id_offset

  void* object_basic_type_id_offset;

Offset of basic type ID in object structure. This is used internally.

=head2 object_type_dimension_offset

  void* object_type_dimension_offset;

Offset of type dimension in object structure. This is used internally.

=head2 object_type_category_offset

  void* object_type_category_offset;

Offset of runtime type category in object structure. This is used internally.

=head2 object_flag_offset

  void* object_flag_offset;

Offset of flag in object structure. This is used internally.

=head2 object_length_offset

  void* object_length_offset;

The length offset in the object structure. This is used internally.

=head2 byte_object_basic_type_id

  void* byte_object_basic_type_id;

Basic type ID of L<Byte|SPVM::Byte> type. This is used internally.

=head2 short_object_basic_type_id

  void* short_object_basic_type_id;

ID of the base type of L<Short|SPVM::Short> type. This is used internally.

=head2 int_object_basic_type_id

  void* int_object_basic_type_id;

ID of the base type of L<Int|SPVM::Int> type. This is used internally.

=head2 long_object_basic_type_id

  void* long_object_basic_type_id;

ID of the base type of L<Long|SPVM::Long> type. This is used internally.

=head2 float_object_basic_type_id

  void* float_object_basic_type_id;

ID of the base type of L<Float|SPVM::Float> type. This is used internally.

=head2 double_object_basic_type_id

  void* double_object_basic_type_id;

ID of the base type of L<Double|SPVM::Double> type. This is used internally.

=head2 compiler

  void* compiler;

A pointer to the SPVM compiler. This is used internally.

=head2 exception_object

  void* exception_object;

Exception object. This is used internally.

=head2 native_mortal_stack

  void* native_mortal_stack;

Mortal stack used for native calls. This is used internally.

=head2 native_mortal_stack_top

  void* native_mortal_stack_top;

The top position of the mortal stack used for native calls. This is used internally.

=head2 native_mortal_stack_capacity

  void* native_mortal_stack_capacity;

The amount of mortal stack used for native calls. This is used internally.

=head2 get_basic_type_id

  int32_t (*get_basic_type_id)(SPVM_ENV* env, const char* basic_type_name);

Get the ID of the base type given the name of the base type. If it does not exist, a value less than 0 is returned.

B<Examples:>

  int32_t basic_type_id = env->get_basic_type_id(env, "Int");

=head2 get_field_id

  int32_t (*get_field_id)(SPVM_ENV* env, const char* class_name, const char* field_name, const char* signature);

Get the ID of the field given the class name, field name, and signature. If the field does not exist, a value less than 0 is returned.

The signature is the same as the field type name.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "int");

=head2 get_field_offset

  int32_t (*get_field_offset)(SPVM_ENV* env, int32_t field_id);

Gets the offset of the field given the field ID. The field ID must be a valid field ID obtained with the field_id function.

=head2 get_class_var_id

  int32_t (*get_class_var_id)(SPVM_ENV* env, const char* class_name, const char* class_var_name, const char* signature);

Get the class variable ID given the class name, class variable name and signature. If the class variable does not exist, a value less than 0 is returned.

The signature is the same as the class variable type name.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "int");

=head2 get_class_method_id

  int32_t (*get_class_method_id)(SPVM_ENV* env, const char* class_name, const char* method_name, const char* signature);

Get a class method ID by the class name, the method name, and the method signature. If the class method does not exists, a negative value is returned.

This ID is used by L<"call_class_method">.

The method signature has the following format.

  ReturnValueType(ArgumentType1,ArgumentType2,...)

B<Examples:>

  int32_t method_id = env->get_class_method_id(env, "Foo", "get", "int(long,string)");

=head2 get_instance_method_id

  int32_t (*get_instance_method_id)(SPVM_ENV* env, void* object, const char* method_name, const char* signature);

Get a instance method ID by the object, the method name, and the method signatre. If the instance method does not exist, a negative value is returned.

This ID is used by L<"call_instance_method">.

The method signature has the following format,

  ReturnValueType(ArgumentType1,ArgumentType2,...)

B<Examples:>

  int32_t method_id = env->get_instance_method_id(env, object, "get", "int(long,string)");

=head2 new_object_raw

  void* (*new_object_raw)(SPVM_ENV* env, int32_t basic_type_id);

Create a new object with a basic type ID. The basic type ID must be the correct base type ID return by C<get_basic_type_id> function.

=head2 new_object

  void* (*new_object)(SPVM_ENV* env, int32_t basic_type_id);

Do the same as C<new_object_raw>, and add the created object to the mortal stack of the environment. Use this function in normal use instead of C<new_object_raw>.

B<Examples:>

  int32_t basic_type_id = env->get_basic_type_id(env, "Int");
  void* object = env->new_object(env, basic_type_id);

=head2 new_byte_array_raw

  void* (*new_byte_array_raw)(SPVM_ENV* env, int32_t length);

Create a new byte[] type array by specifying the length.

=head2 new_byte_array

  void* (*new_byte_array)(SPVM_ENV* env, int32_t length);

Do the same as C<new_byte_array_raw>, and add the created array to the mortal stack of the environment. Use this function in normal use instead of C<new_byte_array_raw>.

B<Examples:>

  void* byte_array = env->new_byte_array(env, 100);

=head2 new_short_array_raw

  void* (*new_short_array_raw)(SPVM_ENV* env, int32_t length);

Create a new short[] type array by specifying the length.

=head2 new_short_array

  void* (*new_short_array)(SPVM_ENV* env, int32_t length);

Do the same as C<new_short_array_raw>, and add the created array to the mortal stack of the environment. Use this function in normal use instead of C<new_short_array_raw>.

B<Examples:>

  void* short_array = env->new_short_array(env, 100);

=head2 new_int_array_raw

  void* (*new_int_array_raw)(SPVM_ENV* env, int32_t length);

Create a new int[] type array by specifying the length.

=head2 new_int_array

  void* (*new_int_array)(SPVM_ENV* env, int32_t length);

Do the same as C<new_int_array_raw>, and add the created array to the mortal stack of the environment. Use this function in normal use instead of C<new_int_array_raw>.

B<Examples:>

  void* int_array = env->new_int_array(env, 100);

=head2 new_long_array_raw

  void* (*new_long_array_raw)(SPVM_ENV* env, int32_t length);

Create a new long[] type array by specifying the length.

=head2 new_long_array

  void* (*new_long_array)(SPVM_ENV* env, int32_t length);

Do the same as C<new_long_array_raw>, and add the created array to the mortal stack of the environment. Use this function in normal use instead of C<new_long_array_raw>.

B<Examples:>

  void* long_array = env->new_long_array(env, 100);

=head2 new_float_array_raw

  void* (*new_float_array_raw)(SPVM_ENV* env, int32_t length);

Create a new float[] type array by specifying the length.

=head2 new_float_array

  void* (*new_float_array)(SPVM_ENV* env, int32_t length);

Do the same as C<new_float_array_raw>, and add the created array to the mortal stack of the environment. Use this function in normal use instead of C<new_float_array_raw>.

B<Examples:>

  void* float_array = env->new_float_array(env, 100);

=head2 new_double_array_raw

  void* (*new_double_array_raw)(SPVM_ENV* env, int32_t length);

Create a new double[] type array by specifying the length.

=head2 new_double_array

  void* (*new_double_array)(SPVM_ENV* env, int32_t length);

Do the same as C<new_double_array_raw>, and add the created array to the mortal stack of the environment. Use this function in normal use instead of C<new_double_array_raw>.

B<Examples:>

  void* double_array = env->new_double_array(env, 100);

=head2 new_object_array_raw

  void* (*new_object_array_raw)(SPVM_ENV* env, int32_t basic_type_id, int32_t length);

Create a new object type array by specifying the basic type ID and the array length. The basic type ID must be the correct basic type ID got by C<get_basic_type_id> function.

=head2 new_object_array

  void* (*new_object_array)(SPVM_ENV* env, int32_t basic_type_id, int32_t length);

Do the same as C<new_object_array_raw>, and add the created array to the mortal stack of the environment. Use this function in normal use instead of C<new_object_array_raw>.

B<Examples:>

  int32_t basic_type_id = env->get_basic_type_id(env, "Int");
  void* object_array = env->new_object_array(env, basic_type_id, 100);

=head2 new_muldim_array_raw

  void* (*new_muldim_array_raw)(SPVM_ENV* env, int32_t basic_type_id, int32_t element_dimension, int32_t length);

Create a new multi dimension array by specifying the basic type ID, the type dimension of the element, and the array length. The basic type ID must be the correct basic type ID got bu C<get_basic_type_id> function. the type dimension of the element must be less than or equals to 255.

=head2 new_muldim_array

  void* (*new_muldim_array_raw)(SPVM_ENV* env, int32_t basic_type_id, int32_t element_dimension, int32_t length);

Do the same as C<new_muldim_array_raw>, and add the created array to the mortal stack of the environment. Use this function in normal use instead of C<new_muldim_array_raw>.

B<Examples:>

  // new Int[][][100]
  int32_t basic_type_id = env->get_basic_type_id(env, "Int");
  void* multi_array = env->new_muldim_array(env, basic_type_id, 2, 100);

=head2 new_mulnum_array_raw

  void* (*new_mulnum_array_raw)(SPVM_ENV* env, int32_t basic_type_id, int32_t length);

Create a new multi-numeric array by specifying the basic type ID and the array length. The basic type ID must be the correct basic type ID got by C<basic_type_id> function.

=head2 new_mulnum_array

  void* (*new_mulnum_array)(SPVM_ENV* env, int32_t basic_type_id, int32_t length);

Do the same as C<new_mulnum_array_raw>, and add the created array to the mortal stack of the environment. Use this function in normal use instead of C<new_mulnum_array_raw>.

B<Examples:>

  int32_t basic_type_id = env->get_basic_type_id(env, "Complex_2d");
  void* value_array = env->new_mulnum_array(env, basic_type_id, 100);

=head2 new_string_nolen_raw

  void* (*new_string_nolen_raw)(SPVM_ENV* env, const char* bytes);

Create a new string object by specifying C language char* type value. this value must end with "\0".

=head2 new_string_nolen

  void* (*new_string_nolen)(SPVM_ENV* env, const char* bytes);

Do the same as C<new_string_nolen_raw>, and add the created string object to the mortal stack of the environment. Use this function in normal use instead of C<new_string_nolen_raw>.

B<Examples:>

  void* str_obj = env->new_string_nolen(env, "Hello World");

=head2 new_string_raw

  void* (*new_string_raw)(SPVM_ENV* env, const char* bytes, int32_t length);

Create a new string object by specifying C language char* type value and the length.

=head2 new_string

  void* (*new_string)(SPVM_ENV* env, const char* bytes, int32_t length);

Do the same as C<new_string_raw>, and add the created string object to the mortal stack of the environment. Use this function in normal use instead of C<new_string_raw>.

B<Examples:>

  void* str_obj = env->new_string(env, "Hello \0World", 11);

=head2 new_pointer_raw

  void* (*new_pointer_raw)(SPVM_ENV* env, int32_t basic_type_id, void* pointer);

Create a pointer type object by specifying a basic type ID and a C language pointer. The basic type ID must be the correct basic type ID got by C<get_basic_type_id> function.

=head2 new_pointer

  void* (*new_pointer)(SPVM_ENV* env, int32_t basic_type_id, void* pointer);

Do the same as C<new_pointer_raw>, and add the created string object to the mortal stack of the environment. Use this function in normal use instead of C<new_pointer_raw>.

B<Examples:>

  int32_t basic_type_id = env->get_basic_type_id(env, "MyTime");
  void* pointer = malloc(sizeof (struct tm));
  void* pointer_obj = env->new_pointer(env, basic_type_id, pointer);

=head2 concat_raw

  void* (*concat_raw)(SPVM_ENV* env, void* string1, void* string2);

Concat two strings.

=head2 concat

  void* (*concat)(SPVM_ENV* env, void* string1, void* string2);

Do the same as C<concat_raw>, and add the created string object to the mortal stack of the environment. Use this function in normal use instead of C<concat_raw>.

=head2 new_stack_trace_raw

  void* (*new_stack_trace_raw)(SPVM_ENV* env, void* exception, const char* class_name, const char* method_name, const char* file, int32_t line);

If you specify a byte[] type exception message and a class name, method name, file name and line number, the character string of the class name, method name, file name and line number is added to the end of the byte[] type exception message. The added character string will be returned.

This function does not add objects to the mortal stack, use new_stack_trace to avoid memory leaks for normal use.

=head2 new_stack_trace

  void* (*new_stack_trace)(SPVM_ENV* env, void* exception, const char* class_name, const char* method_name, const char* file, int32_t line);

When a byte[] type exception message and a class name, method name, file name and line number are specified, the string of the class name, method name, file name and line number is added to the end of the string type exception message. Returns a new string type object. Add the newly created object to the mortal stack.

=head2 length

  int32_t (*length)(SPVM_ENV*, void* array);

If you specify an array, the length of the array is returned.

B<Examples:>

  int32_t length = env->length(env, array);

=head2 get_elems_byte

  int8_t* (*get_elems_byte)(SPVM_ENV* env, void* array);

If you specify a byte[] type array, the pointer at the beginning of the internally stored C language int8_t[] type array is returned.  

B<Examples:>

  int8_t* values = env->get_elems_byte(env, array);
  values[3] = 5;

=head2 get_elems_short

  int16_t* (*get_elems_short)(SPVM_ENV* env, void* array);

If a short[] type array is specified, the pointer at the beginning of the internally stored C language int16_t[] type array is returned.

B<Examples:>

  int16_t* values = env->get_elems_short(env, array);
  values[3] = 5;

=head2 get_elems_int

  int32_t* (*get_elems_int)(SPVM_ENV* env, void* array);

When an int[] type array is specified, the pointer at the beginning of the internally stored C language int32_t[] type array is returned.

B<Examples:>

  int32_t* values = env->get_elems_int(env, array);
  values[3] = 5;

=head2 get_elems_long

  int64_t* (*get_elems_long)(SPVM_ENV* env, void* array);

When a long[] type array is specified, the pointer at the beginning of the internally stored C language int64_t[] type array is returned.

B<Examples:>

  int64_t* values = env->get_elems_long(env, array);
  values[3] = 5;

=head2 get_elems_float

  float* (*get_elems_float)(SPVM_ENV* env, void* array);

When a float[] type array is specified, the pointer at the beginning of the C language float[] type array internally held is returned.

B<Examples:>

  float* values = env->get_elems_float(env, array);
  values[3] = 1.5f;

=head2 get_elems_double

  double* (*get_elems_double)(SPVM_ENV* env, void* array);

If a double[] type array is specified, the pointer at the beginning of the internally stored C double[] type array is returned.

B<Examples:>

  double* values = env->get_elems_double(env, array);
  values[3] = 1.5;

=head2 get_elem_object

  void* (*get_elem_object)(SPVM_ENV* env, void* array, int32_t index);

Gets an object of an element given an array of object types and a methodscript. If the element is a weak reference, the weak reference is removed.

B<Examples:>

  void* object = env->get_elem_object(env, array, 3);

=head2 set_elem_object

  void (*set_elem_object)(SPVM_ENV* env, void* array, int32_t index, void* value);

If you specify an array of object type and methodscript and element objects, the element object is assigned to the corresponding methodscript position. If the element's object has a weak reference, the weak reference is removed. The reference count of the originally assigned object is decremented by 1.

B<Examples:>

  env->get_elem_object(env, array, 3, object);

=head2 get_field_byte

  int8_t (*get_field_byte)(SPVM_ENV* env, void* object, int32_t field_id);

If an object and field ID are specified, the byte field value will be returned as a C language int8_t type value. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "byte");
  int8_t field_value = env->get_field_byte(env, object, field_id);

=head2 get_field_short

  int16_t (*get_field_short)(SPVM_ENV* env, void* object, int32_t field_id);

If you specify the object and field ID, the value of the short type field will be returned as the int16_t type value of C language. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "short");
  int16_t field_value = env->get_field_short(env, object, field_id);

=head2 get_field_int

  int32_t (*get_field_int)(SPVM_ENV* env, void* object, int32_t field_id);

If an object and a field ID are specified, the value of the int type field will be returned as a C language int32_t type value. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "int");
  int32_t field_value = env->get_field_int(env, object, field_id);

=head2 get_field_long

  int64_t (*get_field_long)(SPVM_ENV* env, void* object, int32_t field_id);

If you specify the object and field ID, the value of the long type field will be returned as the value of int64_t type of C language. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "long");
  int64_t field_value = env->get_field_long(env, object, field_id);

=head2 get_field_float

  float (*get_field_float)(SPVM_ENV* env, void* object, int32_t field_id);

If you specify the object and field ID, the value of the float type field will be returned as a C language float type value. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "float");
  float field_value = env->get_field_float(env, object, field_id);

=head2 get_field_double

  double (*get_field_double)(SPVM_ENV* env, void* object, int32_t field_id);

If you specify the object and field ID, the value of the double type field will be returned as a double type value in C language. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "double");
  double field_value = env->get_field_double(env, object, field_id);

=head2 get_field_object

  void* (*get_field_object)(SPVM_ENV* env, void* object, int32_t field_id);

If you specify the object and field ID, the value of the object type field is returned as a void* type value in C language. The field ID must be a valid field ID obtained with the field_id function. If the field is a weak reference, it will be removed.

  int32_t field_id = env->get_field_id(env, "Foo", "x", "Int");
  void* field_value = env->get_field_object(env, object, field_id);

=head2 set_field_byte

  void (*set_field_byte)(SPVM_ENV* env, void* object, int32_t field_id, int8_t value);

If you specify the object and field ID and the value of the field, the value is set to the byte type field. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "byte");
  int8_t field_value = 5;
  env->set_field_byte(env, object, field_id, field_value);

=head2 set_field_short

  void (*set_field_short)(SPVM_ENV* env, void* object, int32_t field_id, int16_t value);

If you specify the object and field ID and the value of the field, the value is set to the short type field. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "short");
  int16_t field_value = 5;
  env->set_field_short(env, object, field_id, field_value);

=head2 set_field_int

  void (*set_field_int)(SPVM_ENV* env, void* object, int32_t field_id, int32_t value);

If you specify the object and field ID and the value of the field, the value is set to the int type field. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "int");
  int32_t field_value = 5;
  env->set_field_int(env, object, field_id, field_value);

=head2 set_field_long

  void (*set_field_long)(SPVM_ENV* env, void* object, int32_t field_id, int64_t value);

If you specify the object and field ID and the value of the field, the value is set to the long type field. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "long");
  int64_t field_value = 5;
  env->set_field_long(env, object, field_id, field_value);

=head2 set_field_float

  void (*set_field_float)(SPVM_ENV* env, void* object, int32_t field_id, float value);

If you specify the object and field ID and the value of the field, the value is set to the float type field. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "float");
  float field_value = 1.5f;
  env->set_field_float(env, object, field_id, field_value);

=head2 set_field_double

  void (*set_field_double)(SPVM_ENV* env, void* object, int32_t field_id, double value);

If you specify the object and field ID and the value of the field, the value is set to the double type field. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "double");
  double field_value = 1.55;
  env->set_field_double(env, object, field_id, field_value);

=head2 set_field_object

  void (*set_field_object)(SPVM_ENV* env, void* object, int32_t field_id, void* value);

Object and field Specify the ID and the value of the field and set the value to the object type field. The field ID must be a valid field ID obtained with the field_id function. After setting, the reference count is incremented by 1. The original value has the reference count decremented by 1.

B<Examples:>

  int32_t field_id = env->get_field_id(env, "Foo", "x", "Int");
  int32_t basic_type_id = env->get_basic_type_id(env, "Int");
  void* object = env->new_object(env, basic_type_id);
  env->set_field_object(env, object, field_id, object);

=head2 get_class_var_byte

  int8_t (*get_class_var_byte)(SPVM_ENV* env, int32_t pkgvar_id);

If an object and a class variable ID are specified, the value of the byte type class variable is returned as a C language int8_t type value. The class variable ID must be a valid class variable ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "byte");
  int8_t pkgvar_value = env->get_class_var_byte(env, object, pkgvar_id);

=head2 get_class_var_short

  int16_t (*get_class_var_short)(SPVM_ENV* env, int32_t pkgvar_id);

If an object and a class variable ID are specified, the value of the short type class variable will be returned as a C language int16_t type value. The class variable ID must be a valid class variable ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "short");
  int16_t pkgvar_value = env->get_class_var_short(env, object, pkgvar_id);

=head2 get_class_var_int

  int32_t (*get_class_var_int)(SPVM_ENV* env, int32_t pkgvar_id);

If an object and a class variable ID are specified, the value of the int type class variable will be returned as a C language int32_t type value. The class variable ID must be a valid class variable ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "int");
  int32_t pkgvar_value = env->get_class_var_int(env, object, pkgvar_id);

=head2 get_class_var_long

  int64_t (*get_class_var_long)(SPVM_ENV* env, int32_t pkgvar_id);

If an object and a class variable ID are specified, the value of the long type class variable will be returned as a C language int64_t type value. The class variable ID must be a valid class variable ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "long");
  int64_t pkgvar_value = env->get_class_var_long(env, object, pkgvar_id);

=head2 get_class_var_float

  float (*get_class_var_float)(SPVM_ENV* env, int32_t pkgvar_id);

If an object and a class variable ID are specified, the value of the float type class variable will be returned as a C language float type value. The class variable ID must be a valid class variable ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "float");
  float pkgvar_value = env->get_class_var_float(env, object, pkgvar_id);

=head2 get_class_var_double

  double (*get_class_var_double)(SPVM_ENV* env, int32_t pkgvar_id);

If you specify an object and a class variable ID, the value of the double type class variable is returned as a C type double type value. The class variable ID must be a valid class variable ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "double");
  double pkgvar_value = env->get_class_var_double(env, object, pkgvar_id);

=head2 get_class_var_object

  void* (*get_class_var_object)(SPVM_ENV* env, int32_t pkgvar_id);

When an object and a class variable ID are specified, the value of the object type class variable is returned as a C language void* type value. The class variable ID must be a valid class variable ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "Int");
  void* pkgvar_value = env->get_class_var_byte(env, object, pkgvar_id);

=head2 set_class_var_byte

  void (*set_class_var_byte)(SPVM_ENV* env, int32_t pkgvar_id, int8_t value);

If you specify the object and field ID and the value of the field, the value is set to the byte type field. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "byte");
  int8_t pkgvar_value = 5;
  env->set_class_var_byte(env, pkgvar_id, pkgvar_value);

=head2 set_class_var_short

  void (*set_class_var_short)(SPVM_ENV* env, int32_t pkgvar_id, int16_t value);

If you specify the object and field ID and the value of the field, the value is set to the short type field. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "short");
  int16_t pkgvar_value = 5;
  env->set_class_var_short(env, pkgvar_id, pkgvar_value);

=head2 set_class_var_int

  void (*set_class_var_int)(SPVM_ENV* env, int32_t pkgvar_id, int32_t value);

If you specify the object and field ID and the value of the field, the value is set to the int type field. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "int");
  int32_t pkgvar_value = 5;
  env->set_class_var_int(env, pkgvar_id, pkgvar_value);

=head2 set_class_var_long

  void (*set_class_var_long)(SPVM_ENV* env, int32_t pkgvar_id, int64_t value);

If you specify the object and field ID and the value of the field, the value is set to the long type field. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "long");
  int64_t pkgvar_value = 5;
  env->set_class_var_long(env, pkgvar_id, pkgvar_value);

=head2 set_class_var_float

  void (*set_class_var_float)(SPVM_ENV* env, int32_t pkgvar_id, float value);

If you specify the object and field ID and the value of the field, the value is set to the float type field. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "float");
  float pkgvar_value = 5;
  env->set_class_var_float(env, pkgvar_id, pkgvar_value);

=head2 set_class_var_double

  void (*set_class_var_double)(SPVM_ENV* env, int32_t pkgvar_id, double value);

If you specify the object and field ID and the value of the field, the value is set to the double type field. The field ID must be a valid field ID obtained with the field_id function.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "double");
  double pkgvar_value = 5;
  env->set_class_var_double(env, pkgvar_id, pkgvar_value);

=head2 set_class_var_object

  void (*set_class_var_object)(SPVM_ENV* env, int32_t pkgvar_id, void* value);

Object and field Specify the ID and the value of the field and set the value to the object type field. The field ID must be a valid field ID obtained with the field_id function. After setting, the reference count is incremented by 1. The original value has the reference count decremented by 1.

B<Examples:>

  int32_t pkgvar_id = env->get_class_var_id(env, "Foo", "$VAR", "Int");
  int32_t basic_type_id = env->get_basic_type_id(env, "Int");
  void* object = env->new_object(env, basic_type_id);
  env->set_class_var_object(env, pkgvar_id, pkgvar_value);

=head2 get_pointer

  void* (*get_pointer)(SPVM_ENV* env, void* pointer_object);

Specify a pointer type object and return the C language pointer stored inside the object.

B<Examples:>

  strcut tm* tm_ptr = (struct tm*) env->get_pointer(env, tm_obj);

=head2 set_pointer

  void (*set_pointer)(SPVM_ENV* env, void* pointer_object, void* pointer);

If you specify a pointer type object and a C language pointer, the C language pointer is saved in the internal data of the pointer type object.

=head2 call_spvm_method

  int32_t (*call_spvm_method)(SPVM_ENV* env, int32_t method_id, SPVM_VALUE* args);

Call a method by specifying the method ID and argument. If an exception occurs in the method, The return value is 1. If not, return 0.

The return value of the method is set to args[0].

=head2 get_exception

  void* (*get_exception)(SPVM_ENV* env);

Get a exception message which type is byte[].

=head2 set_exception

  void (*set_exception)(SPVM_ENV* env, void* exception);

Set a exception message which type is byte[].

=head2 get_ref_count

  int32_t (*get_ref_count)(SPVM_ENV* env, void* object);

Get the refernce count of the object.

=head2 inc_ref_count

  void (*inc_ref_count)(SPVM_ENV* env, void* object);

Specifying an object increments the reference count of the object.

Use this method only if you have a specific reason to use it. Normally, the reference count is managed automatically.

=head2 dec_ref_count

  void (*dec_ref_count)(SPVM_ENV* env, void* object);

Specifying an object decrements the object's reference count by 1. When the reference count reaches 0, the object is released.

Use this method only if you have a specific reason to use it. Normally, the reference count is managed automatically.

=head2 enter_scope

  int32_t (*enter_scope)(SPVM_ENV* env);

Create a new scope and return the scope ID.

=head2 push_mortal

  int32_t (*push_mortal)(SPVM_ENV* env, void* object);

Add an object to the mortal stack.

If this method succeed, return 0.

If this method don't alloc memory for new mortal information, return 1.

=head2 leave_scope

  void (*leave_scope)(SPVM_ENV* env, int32_t scope_id);

Specify a scope ID to exit that scope and decrement the object's reference count stored in the mortal stack. Objects with a reference count of 0 are released. The scope ID must be the ID obtained by the enter_scope function.

=head2 remove_mortal

  int32_t (*remove_mortal)(SPVM_ENV* env, int32_t scope_id, void* remove_object);

Given a scope ID and an object, delete the specified object from the mortal stack.

=head2 is_type

  int32_t (*is_type)(SPVM_ENV* env, void* object, int32_t basic_type_id, int32_t type_dimension);

Given an object and a base type ID and a type dimension, returns a nonzero value if the object matches both the base type ID and the type dimension, and 0 otherwise.

=head2 has_callback

  int32_t (*has_callback)(SPVM_ENV* env, void* object, int32_t callback_basic_type_id);

Given a base type id for the object and the callback type, returns a non-zero value if the object conforms to the callback type, and zero otherwise.

=head2 get_object_basic_type_id

  int32_t (*get_object_basic_type_id)(SPVM_ENV* env, void* object);

Gets the base type ID of the object.

=head2 get_object_type_dimension

  int32_t (*get_object_type_dimension)(SPVM_ENV* env, void* object);

Gets the dimension of the type of object.

=head2 weaken

  int32_t (*weaken)(SPVM_ENV* env, void** object_address);

Create weak reference to the object which is specified by object address.

The reference count of the object is decrimented by 1 and weaken flag is added to the object address.

If the reference count is 1, "dec_ref_count" is called to the object.

If object_address is NULL, this method do nothing.

If the object is already weaken, this method do nothing.

This method allocate memory internally to add the back reference from the object to the object address.

This method success return 0.

If failing memory allocation of back reference, return 1.

=head2 isweak 

  int32_t (*isweak()SPVM_ENV* env, void** object);

Given the address of an object, returns non-zero if the object is a weak reference, 0 otherwise.

=head2 unweaken

  void (*unweaken)(SPVM_ENV* env, void** object_address);

Specifying the address of the object releases the weak reference to the object.

=head2 alloc_memory_block_zero

  void* (*alloc_memory_block_zero)(SPVM_ENV* env, int64_t byte_size);

If you specify the size in bytes, the memory block is allocated and the pointer of the allocated memory block is returned. If fail to alloc memory, return NULL. If success, all bits in the memory block are initialized with 0 and the memory block count (memory_blocks_count)is incremented by 1.

=head2 free_memory_block

  void (*free_memory_block)(SPVM_ENV* env, void* block);

If block is not NULL, free the memory and memory blocks count(memory_blocks_count) is decremented by 1.

=head2 get_memory_blocks_count

  int32_t (*get_memory_blocks_count)(SPVM_ENV* env);

Returns the current number of memory blocks.

The memory block is increased by 1 when an object is created, when the alloc_memory_block_zero function is called, and when a back reference is added by the weaken function.

=head2 get_type_name_raw

  void* (*get_type_name_raw)(SPVM_ENV* env, void* object);

If you specify an object, a new byte[] type object that stores the type name is returned.

This function does not add objects to the mortal stack, so use type_name for normal use to avoid memory leaks.

=head2 get_type_name

  void* (*get_type_name)(SPVM_ENV* env, void* object);

If you specify an object, a new byte[] type object that stores the type name is returned. Add the newly created object to the mortal stack.

=head2 new_env

  SPVM_ENV* (*new_env)(SPVM_ENV* env);

Create a new execution environment based on the current execution environment.

Create a new SPVM runtime environment.

The exception object and mortal stack information will be initialized.

Share the class variables with the original execution environment.

The number of memory blocks is shared with the original execution environment.

If thie method can't allocate memory for SPVM runtime environment, return NULL.

=head2 free_env

  void (*free_env)(SPVM_ENV* env);

Release the execution environment.

=head2 memory_blocks_count

  void* memory_blocks_count;

Memory blocks count. This is used internally.

=head2 get_chars

  const char* (*get_chars)(SPVM_ENV* env, void* string_object);

Get characters pointer in the string object.

B<Examples:>

  const char* bytes = env->get_chars(env, string_object);

=head2 die

  int32_t (*die)(SPVM_ENV* env, const char* message, ...);

Create a C<sprintf> formatted message with file name and line number and set it to the exception.

Last two arguments are file name and line number.

Return value is always 1;

B<Examples:>

  return env->die(env, "Value must be %d", 3, __FILE__, __LINE__);

=head2 new_object_by_name

  void* (*new_object_by_name)(SPVM_ENV* env, const char* class_name, int32_t* exception_flag, const char* file, int32_t line);

This is same as C<new_object> function, but you can specify class name directry.

If function is succeeded, C<exception_flag> is set to 0. If a exception occurs, C<exception_flag> is set to 1. 

B<Examples:>

  int32_t e;
  void* minimal = env->new_object_by_name(env, "TestCase::Minimal", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 new_pointer_by_name

  void* (*new_pointer_by_name)(SPVM_ENV* env, const char* class_name, void* pointer, int32_t* exception_flag, const char* file, int32_t line);

This is same as C<new_pointer> function, but you can specify class name directry.

If function is succeeded, C<exception_flag> is set to 0. If a exception occurs, C<exception_flag> is set to 1. 

  int32_t e;
  void* minimal = env->new_pointer_by_name(env, "TestCase::Pointer", pointer, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_field_byte_by_name

  void (*set_field_byte_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name, int8_t value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_field_byte> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is set to 0. If a exception occurs, C<exception_flag> is set to 1. 

B<Examples:>

  int32_t e;
  env->set_field_byte_by_name(env, object, "TestCase::Simple", "byte_value", 13, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_field_short_by_name

  void (*set_field_short_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name, int16_t value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_field_short> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is set to 0. If a exception occurs, C<exception_flag> is set to 1. 

B<Examples:>

  int32_t e;
  env->set_field_short_by_name(env, object, "TestCase::Simple", "short_value", 13, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_field_int_by_name

  void (*set_field_int_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name, int32_t value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_field_int> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is set to 0. If a exception occurs, C<exception_flag> is set to 1. 

B<Examples:>

  int32_t e;
  env->set_field_int_by_name(env, object, "TestCase::Simple", "int_value", 13, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_field_long_by_name

  void (*set_field_long_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name, int64_t value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_field_long> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is set to 0. If a exception occurs, C<exception_flag> is set to 1. 

B<Examples:>

  int32_t e;
  env->set_field_long_by_name(env, object, "TestCase::Simple", "long_value", 13, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_field_float_by_name

  void (*set_field_float_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name, float value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_field_float> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is set to 0. If a exception occurs, C<exception_flag> is set to 1. 

B<Examples:>

  int32_t e;
  env->set_field_float_by_name(env, object, "TestCase::Simple", "float_value", 13, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_field_double_by_name

  void (*set_field_double_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name, double value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_field_double> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is set to 0. If a exception occurs, C<exception_flag> is set to 1. 

B<Examples:>

  int32_t e;
  env->set_field_double_by_name(env, object, "TestCase::Simple", "double_value", 13, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_field_object_by_name

  void (*set_field_object_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name, const char* signature, void* value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_field_object> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is set to 0. If a exception occurs, C<exception_flag> is set to 1. 

B<Examples:>

  int32_t e;
  env->set_field_object_by_name(env, object_simple, "TestCase::Simple", "object_value", "TestCase::Minimal", object_minimal, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_field_byte_by_name

  int8_t (*get_field_byte_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_field_byte> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  int8_t byte_value = env->get_field_byte_by_name(env, object, "TestCase::Simple", "byte_value", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_field_short_by_name

  int16_t (*get_field_short_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_field_short> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  int8_t short_value = env->get_field_short_by_name(env, object, "TestCase::Simple", "short_value", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_field_int_by_name

  int32_t (*get_field_int_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_field_int> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  int8_t int_value = env->get_field_int_by_name(env, object, "TestCase::Simple", "int_value", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_field_long_by_name

  int64_t (*get_field_long_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_field_long> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  int8_t long_value = env->get_field_long_by_name(env, object, "TestCase::Simple", "long_value", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_field_float_by_name

  float (*get_field_float_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_field_float> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  int8_t float_value = env->get_field_float_by_name(env, object, "TestCase::Simple", "float_value", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_field_double_by_name

  double (*get_field_double_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_field_double> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  int8_t double_value = env->get_field_double_by_name(env, object, "TestCase::Simple", "double_value", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_field_object_by_name

  void* (*get_field_object_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name, const char* signature,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_field_object> function, but you can specify class name and field name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  void* object_minimal = env->get_field_object_by_name(env, object_simple, "TestCase::Simple", "object_value", "TestCase::Minimal", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_class_var_byte_by_name

  void (*set_class_var_byte_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name, int8_t value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_class_var_byte> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  env->set_class_var_byte_by_name(env, "TestCase::NativeAPI", "$BYTE_VALUE", 15, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_class_var_short_by_name

  void (*set_class_var_short_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name, int16_t value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_class_var_short> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  env->set_class_var_short_by_name(env, "TestCase::NativeAPI", "$SHORT_VALUE", 15, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_class_var_int_by_name

  void (*set_class_var_int_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name, int32_t value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_class_var_int> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  env->set_class_var_int_by_name(env, "TestCase::NativeAPI", "$INT_VALUE", 15, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_class_var_long_by_name

  void (*set_class_var_long_by_name)(SPVM_ENV* env
    const char* class_name, const char* class_var_name, int64_t value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_class_var_long> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  env->set_class_var_long_by_name(env, "TestCase::NativeAPI", "$LONG_VALUE", 15, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_class_var_float_by_name

  void (*set_class_var_float_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name, float value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_class_var_float> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  env->set_class_var_float_by_name(env, "TestCase::NativeAPI", "$FLOAT_VALUE", 15, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_class_var_double_by_name

  void (*set_class_var_double_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name, double value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_class_var_double> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  env->set_class_var_double_by_name(env, "TestCase::NativeAPI", "$DOUBLE_VALUE", 15, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 set_class_var_object_by_name

  void (*set_class_var_object_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name, const char* signature, void* value,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<set_class_var_object> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  env->set_class_var_object_by_name(env, "TestCase::NativeAPI", "$MINIMAL_VALUE", "TestCase::Minimal", minimal, &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_class_var_byte_by_name

  int8_t (*get_class_var_byte_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_class_var_byte> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  int8_t value = env->get_class_var_byte_by_name(env, "TestCase::NativeAPI", "$BYTE_VALUE", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_class_var_short_by_name

  int16_t (*get_class_var_short_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_class_var_short> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  int16_t value = env->get_class_var_short_by_name(env, "TestCase::NativeAPI", "$SHORT_VALUE", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_class_var_int_by_name

  int32_t (*get_class_var_int_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_class_var_int> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  int8_t value = env->get_class_var_byte_by_name(env, "TestCase::NativeAPI", "$BYTE_VALUE", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_class_var_long_by_name

  int64_t (*get_class_var_long_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_class_var_long> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  int64_t value = env->get_class_var_long_by_name(env, "TestCase::NativeAPI", "$LONG_VALUE", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_class_var_float_by_name

  float (*get_class_var_float_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_class_var_float> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  float value = env->get_class_var_float_by_name(env, "TestCase::NativeAPI", "$FLOAT_VALUE", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_class_var_double_by_name

  double (*get_class_var_double_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_class_var_double> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>

  int32_t e;
  double value = env->get_class_var_double_by_name(env, "TestCase::NativeAPI", "$DOUBLE_VALUE", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 get_class_var_object_by_name

  void* (*get_class_var_object_by_name)(SPVM_ENV* env,
    const char* class_name, const char* class_var_name, const char* signature,
    int32_t* exception_flag, const char* file, int32_t line);

This is same as C<get_class_var_object> function, but you can specify the class name directry.

If function is succeeded, C<exception_flag> is get to 0. If a exception occurs, C<exception_flag> is get to 1. 

B<Examples:>
  
  int32_t e;
  void* value = env->get_class_var_object_by_name(env, "TestCase::NativeAPI", "$MINIMAL_VALUE", "TestCase::Minimal", &e, __FILE__, __LINE__);
  if (e) { return e; }

=head2 call_class_method_by_name

  int32_t (*call_class_method_by_name)(SPVM_ENV* env,
    const char* class_name, const char* method_name, const char* signature, SPVM_VALUE* stack,
    const char* file, int32_t line);

This is same as C<call_spvm_method> function, but you can specify the class name and sub name directry.

B<Examples:>

  int32_t output;
  {
    stack[0].ival = 5;
    int32_t exception_flag = env->call_class_method_by_name(env, "TestCase::NativeAPI", "my_value", "int(int)", stack, __FILE__, __LINE__);
    if (exception_flag) {
      return exception_flag;
    }
    output = stack[0].ival;
  }

=head2 call_instance_method_by_name

  int32_t (*call_instance_method_by_name)(SPVM_ENV* env, void* object,
    const char* method_name, const char* signature, SPVM_VALUE* stack,
    const char* file, int32_t line);

B<Examples:>

=head2 get_field_string_chars_by_name

  const char* (*get_field_string_chars_by_name)(SPVM_ENV* env, void* object,
    const char* class_name, const char* field_name,
    int32_t* exception_flag, const char* file, int32_t line);

B<Examples:>

=head2 any_object_basic_type_id

  void* any_object_basic_type_id;

Basic type ID of any object type. This is used internally.

=head2 dump_raw

  void* (*dump_raw)(SPVM_ENV* env, void* object);

Get the string which dump the object. The string is the same as the return value of C<dump> operator.

=head2 dump

  void* (*dump)(SPVM_ENV* env, void* object);

Do the same as C<dump_raw>, and add the created string object to the mortal stack of the environment. Use this function in normal use instead of C<dump_raw>.

=head2 call_class_method

Alias for L<"call_spvm_method">

=head2 call_instance_method

Alias for L<"call_spvm_method">

=head2 get_instance_method_id_static

  int32_t (*get_instance_method_id_static)(SPVM_ENV* env, const char* class_name, const char* method_name, const char* signature);

Get a instance method ID by the class name, the method name, and the method signature. If the instance method does not exists, a negative value is returned.

This ID is used by L<"call_instance_method">.

The method signature has the following format.

  ReturnValueType(ArgumentType1,ArgumentType2,...)

B<Examples:>

  int32_t method_id = env->get_instance_method_id_static(env, "Foo", "get", "int(long,string)");

=head2 get_bool_object_value

  int32_t (*get_bool_object_value)(SPVM_ENV* env, void* bool_object);

Get the value of a L<SPVM::Bool|Bool> object. If the Bool object is true, return 1, otherwise return 0.

B<Examples:>

  int32_t bool_value = env->get_bool_object_value(env, bool_object);

=head1 Examples

=over 2

=item * L<Examples using SPVM Native APIs|https://github.com/yuki-kimoto/SPVM/tree/master/examples/native>

=back

