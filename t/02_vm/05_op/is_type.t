use lib "t/testlib";
use TestAuto;

use strict;
use warnings;

use Test::More;

use SPVM 'TestCase::IsType';



# Start objects count
my $start_memory_blocks_count = SPVM::api->get_memory_blocks_count();

# is_type
{
  ok(SPVM::TestCase::IsType->is_type);
}

# is_compile_type
{
  ok(SPVM::TestCase::IsType->is_compile_type);
}

# All object is freed
my $end_memory_blocks_count = SPVM::api->get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);

done_testing;
