use lib "t/lib";
use TestAuto;

use strict;
use warnings;

use Test::More 'no_plan';

use SPVM 'TestCase::Assign';

# Start objects count
my $start_memory_blocks_count = SPVM::get_memory_blocks_count();

# Number literal
{
  ok(TestCase::Assign->assign);

  ok(TestCase::Assign->assign_numeric_to_string);

  ok(TestCase::Assign->assign_byte_to_byte_object);
  ok(TestCase::Assign->assign_short_to_short_object);
  ok(TestCase::Assign->assign_int_to_int_object);
  ok(TestCase::Assign->assign_long_to_long_object);
  ok(TestCase::Assign->assign_float_to_float_object);
  ok(TestCase::Assign->assign_double_to_double_object);

  ok(TestCase::Assign->assign_byte_object_to_byte);
  ok(TestCase::Assign->assign_short_object_to_short);
  ok(TestCase::Assign->assign_int_object_to_int);
  ok(TestCase::Assign->assign_long_object_to_long);
  ok(TestCase::Assign->assign_float_object_to_float);
  ok(TestCase::Assign->assign_double_object_to_double);
}

# All object is freed
my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);
