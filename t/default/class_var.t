use lib "t/lib";
use TestAuto;

use strict;
use warnings;

use Test::More 'no_plan';

use SPVM 'TestCase';



# Start objects count
my $start_memory_blocks_count = SPVM::get_memory_blocks_count();

# Class variable relative name
{
  my $start_memory_blocks_count = SPVM::get_memory_blocks_count();
  ok(SPVM::TestCase->class_var_rel_name());
  my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
  is($start_memory_blocks_count, $end_memory_blocks_count);
}

# Class variable
{
  my $start_memory_blocks_count = SPVM::get_memory_blocks_count();
  ok(SPVM::TestCase->class_var());
  ok(SPVM::TestCase->class_var_other_class());
  my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
  is($start_memory_blocks_count, $end_memory_blocks_count);
}

# All object is freed
my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);
