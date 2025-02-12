use lib "t/testlib";
use TestAuto;

use strict;
use warnings;

use Test::More;

use SPVM 'TestCase::Literal::Character';



# Start objects count
my $start_memory_blocks_count = SPVM::api->get_memory_blocks_count();

# Character literal
{
  ok(SPVM::TestCase::Literal::Character->character_literal);
  ok(SPVM::TestCase::Literal::Character->character_literal_octal);
  ok(SPVM::TestCase::Literal::Character->character_literal_hex);
}

# All object is freed
my $end_memory_blocks_count = SPVM::api->get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);

done_testing;
