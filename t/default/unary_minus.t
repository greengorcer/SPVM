use lib "t/lib";
use TestAuto;

use strict;
use warnings;

use Test::More 'no_plan';


use FindBin;
use lib "$FindBin::Bin/lib";

use SPVM 'TestCase::UnaryMinus';



# Start objects count
my $start_memory_blocks_count = SPVM::get_memory_blocks_count();

# Spec tests
{
  # Unary minus - Operation
  {
    ok(TestCase::UnaryMinus->unary_minus_byte);
    ok(TestCase::UnaryMinus->unary_minus_short);
    ok(TestCase::UnaryMinus->unary_minus_int);
    ok(TestCase::UnaryMinus->unary_minus_long);
    ok(TestCase::UnaryMinus->unary_minus_float);
    ok(TestCase::UnaryMinus->unary_minus_double);
  }

  # Unary minus - Compile Error
  {
    {
      my $build = SPVM::Builder->new;
      my $success = $build->compile_spvm('TestCase::CompileError::UnaryMinus::NotNumeric', __LINE__, __FILE__);
      ok($success == 0);
    }
  }
}
# All object is freed
my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);
