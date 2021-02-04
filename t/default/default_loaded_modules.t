use lib "t/lib";
use TestAuto;

use strict;
use warnings;

use Test::More 'no_plan';

use SPVM 'SPVM::Byte';
use SPVM 'SPVM::Short';
use SPVM 'SPVM::Int';
use SPVM 'SPVM::Long';
use SPVM 'SPVM::Float';
use SPVM 'SPVM::Double';


use FindBin;
use lib "$FindBin::Bin/lib";

use SPVM;



# Start objects count
my $start_memory_blocks_count = SPVM::get_memory_blocks_count();

# Default loaded modules
SPVM::Byte->new(1);
SPVM::Short->new(1);
SPVM::Int->new(1);
SPVM::Long->new(1);
SPVM::Float->new(1);
SPVM::Double->new(1);

ok(1);

# All object is freed
my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);
