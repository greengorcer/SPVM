use lib "t/testlib";
use TestAuto;

use strict;
use warnings;

use Test::More;

use SPVM 'TestCase::Module::Format';

use TestFile;

# Start objects count
my $start_memory_blocks_count = SPVM::get_memory_blocks_count();

# sprintf
{
  ok(SPVM::TestCase::Module::Format->sprintf_d);
  ok(SPVM::TestCase::Module::Format->sprintf_u);
  ok(SPVM::TestCase::Module::Format->sprintf_ld);
  ok(SPVM::TestCase::Module::Format->sprintf_lu);
  ok(SPVM::TestCase::Module::Format->sprintf_f);
  ok(SPVM::TestCase::Module::Format->sprintf_g);
  ok(SPVM::TestCase::Module::Format->sprintf_c);
  ok(SPVM::TestCase::Module::Format->sprintf_s);
  ok(SPVM::TestCase::Module::Format->sprintf_percent);
  ok(SPVM::TestCase::Module::Format->sprintf_x);
  ok(SPVM::TestCase::Module::Format->sprintf_X);
  ok(SPVM::TestCase::Module::Format->sprintf_lx);
  ok(SPVM::TestCase::Module::Format->sprintf_lX);
  ok(SPVM::TestCase::Module::Format->sprintf_all);
}

# All object is freed
my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);

done_testing;
