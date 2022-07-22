use lib "t/testlib";
use TestAuto;

use strict;
use warnings;

use Test::More;

use SPVM 'TestCase::Lib::IntList';



# Start objects count
my $start_memory_blocks_count = SPVM::get_memory_blocks_count();

# SPVM::IntList
{
  ok(SPVM::TestCase::Lib::IntList->push);
  ok(SPVM::TestCase::Lib::IntList->pop);
  ok(SPVM::TestCase::Lib::IntList->new);
  ok(SPVM::TestCase::Lib::IntList->new_len);
  ok(SPVM::TestCase::Lib::IntList->unshift);
  ok(SPVM::TestCase::Lib::IntList->shift);
  ok(SPVM::TestCase::Lib::IntList->length);
  ok(SPVM::TestCase::Lib::IntList->get);
  ok(SPVM::TestCase::Lib::IntList->set);
  ok(SPVM::TestCase::Lib::IntList->set_array);
  ok(SPVM::TestCase::Lib::IntList->to_array);
  ok(SPVM::TestCase::Lib::IntList->insert);
  ok(SPVM::TestCase::Lib::IntList->remove);
  ok(SPVM::TestCase::Lib::IntList->replace);
}

# All object is freed
my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);

done_testing;
