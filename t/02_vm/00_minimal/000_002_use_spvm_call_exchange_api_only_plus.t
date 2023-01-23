use strict;
use warnings;

use Test::More;

use SPVM;

SPVM::get_memory_blocks_count();

my $obj_int = SPVM::ExchangeAPI::call_method(SPVM::api(), SPVM::GET_ENV(), SPVM::GET_STACK(), "Int", "new", 1);

my $value = SPVM::ExchangeAPI::call_method(SPVM::api(), SPVM::GET_ENV(), SPVM::GET_STACK(), $obj_int, "value");
is($value, 1);

is($obj_int->value, 1);

done_testing;
