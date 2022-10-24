use lib "t/testlib";
use TestAuto;

use strict;
use warnings;

use Test::More;

use SPVM 'TestCase::Begin';



# Start objects count
my $start_memory_blocks_count = SPVM::get_memory_blocks_count();

# Import sub
{
  ok(SPVM::TestCase::Begin->init_class_var);
}

# All object is freed
my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);

done_testing;
