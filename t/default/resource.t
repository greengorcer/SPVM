use lib "t/testlib";
use TestAuto;

use strict;
use warnings;

use Test::More;

use SPVM 'TestCase::UseResource::Foo1';
use SPVM 'TestCase::UseResource::Basic';

# Start objects count
my $start_memory_blocks_count = SPVM::get_memory_blocks_count();

ok(SPVM::TestCase::UseResource::Foo1->test);

ok(SPVM::TestCase::UseResource::Basic->test);
my $resource = $main::TEST_USE_RESOURCE_RET;
is($resource->mode, 'mode1');
is_deeply($resource->args, ['args1', 'args2']);
is_deeply($resource->config->{_test_base_argv}, ['args1', 'args2']);
is_deeply($resource->config->{_test_mode1_argv}, ['args1', 'args2']);
like($resource->config->file, qr|t/.*?\Q/lib/SPVM/TestCase/Resource/Foo1/V1_0_0.mode1.config|);


# All object is freed
my $end_memory_blocks_count = SPVM::get_memory_blocks_count();
is($end_memory_blocks_count, $start_memory_blocks_count);

done_testing;
