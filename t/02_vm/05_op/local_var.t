use lib "t/testlib";
use TestAuto;

use strict;
use warnings;

use Test::More;

use SPVM 'TestCase::LocalVar';

# Start objects count
my $start_memory_blocks_count = SPVM::api->get_memory_blocks_count();

# Local variable
{
  ok(SPVM::TestCase::LocalVar->local_var_name);
  ok(SPVM::TestCase::LocalVar->var_brace);
  ok(SPVM::TestCase::LocalVar->local_var_upper_case);
  ok(SPVM::TestCase::LocalVar->local_var_high_precidence_than_class_var);
  ok(SPVM::TestCase::LocalVar->local_var_high_precidence_than_capture_var);
}

# All object is freed
my $end_memory_blocks_count = SPVM::api->get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);

done_testing;
