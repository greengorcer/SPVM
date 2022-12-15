use lib "t/testlib";
use TestAuto;

use strict;
use warnings;

use Test::More;

use SPVM 'TestCase::ConditionalBranch';

# Start objects count
my $start_memory_blocks_count = SPVM::get_memory_blocks_count();

# unless Statement
{
  ok(SPVM::TestCase::ConditionalBranch->unless_true_condition);
  ok(SPVM::TestCase::ConditionalBranch->unless_false_condition);
  ok(SPVM::TestCase::ConditionalBranch->unless_elsif);
  ok(SPVM::TestCase::ConditionalBranch->unless_else);
}

# All object is freed
my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);

done_testing;