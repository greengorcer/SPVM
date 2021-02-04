use lib "t/lib";
use TestAuto;

use strict;
use warnings;

use Test::More 'no_plan';


use FindBin;
use lib "$FindBin::Bin/lib";

use SPVM 'TestCase::UnaryPlus';



# Start objects count
my $start_memory_blocks_count = SPVM::get_memory_blocks_count();

# Spec tests
{
  # Unary plus - Operation
  {
    ok(TestCase::UnaryPlus->unary_plus_byte);
    ok(TestCase::UnaryPlus->unary_plus_short);
    ok(TestCase::UnaryPlus->unary_plus_int);
    ok(TestCase::UnaryPlus->unary_plus_long);
    ok(TestCase::UnaryPlus->unary_plus_float);
    ok(TestCase::UnaryPlus->unary_plus_double);
  }

  # Unary plus - Compile Error
  {
    {
      my $build = SPVM::Builder->new;
      my $success = $build->compile_spvm('TestCase::CompileError::UnaryPlus::NotNumeric', __LINE__, __FILE__);
      ok($success == 0);
    }
  }
}

# All object is freed
my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);
