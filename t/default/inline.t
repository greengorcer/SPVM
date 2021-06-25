use lib "t/lib";
use TestAuto;

use strict;
use warnings;

use Test::More 'no_plan';

use SPVM 'TestCase::Inline';



# Start objects count
my $start_memory_blocks_count = SPVM::get_memory_blocks_count();

# Inline constant sub
{
  ok(TestCase::Inline->constant_method);
  ok(TestCase::Inline->constant_method_return_double == 0.25);
}

# Inline new
{
  ok(TestCase::Inline->new_inline);
  ok(TestCase::Inline->new);
  is(ref TestCase::Inline->new, "TestCase::Inline");
}

# All object is freed
my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);
