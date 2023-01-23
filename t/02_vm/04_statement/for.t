use lib "t/testlib";
use TestAuto;

use strict;
use warnings;

use Test::More;

use SPVM 'TestCase::For';



# Start objects count
my $start_memory_blocks_count = SPVM::api->get_memory_blocks_count();

# for
{
  ok(SPVM::TestCase::For->basic);
  ok(SPVM::TestCase::For->next_statement);
  ok(SPVM::TestCase::For->nested_last);
  ok(SPVM::TestCase::For->nested_next);
  ok(SPVM::TestCase::For->nested_last_with_switch);
  ok(SPVM::TestCase::For->omit_init_inc);
  ok(SPVM::TestCase::For->condition_my);
  ok(SPVM::TestCase::For->foreach);
}

# All object is freed
my $end_memory_blocks_count = SPVM::api->get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);

done_testing;
