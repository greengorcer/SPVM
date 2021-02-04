use lib "t/lib";
use TestAuto;

use strict;
use warnings;

use Test::More 'no_plan';

use SPVM 'TestCase::Literal::Character';



# Start objects count
my $start_memory_blocks_count = SPVM::get_memory_blocks_count();

# Character literal
{
  ok(TestCase::Literal::Character->character());
  ok(TestCase::Literal::Character->escape());
  ok(TestCase::Literal::Character->escape_ascii());
}

# All object is freed
my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);
