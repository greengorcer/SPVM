use lib "t/testlib";
use TestAuto;

use strict;
use warnings;

use Test::More;

use utf8;
use Data::Dumper;
use File::Basename 'basename';
use Config;
use File::Path 'mkpath';
use File::Spec;

use SPVM::Builder::Exe;

my $file = 't/' . basename $0;

my $test_dir = $ENV{SPVM_TEST_DIR};
my $build_dir = $ENV{SPVM_BUILD_DIR};

{
  my $exe_dir = "$build_dir/work/exe";
  mkpath $exe_dir;
  
  my $class_name = 'MyExe';
  my $module_dirs = [map { "$_/SPVM" } "$test_dir/lib", @INC];
  my $output_file = "$build_dir/work/myexe";
  my $config_file = "$test_dir/myexe.config";

  my $builder_exe = SPVM::Builder::Exe->new(
    class_name => $class_name,
    output_file => $output_file,
    build_dir => $build_dir,
    module_dirs => $module_dirs,
    config_file => $config_file,
  );
  
  $builder_exe->build_exe_file;

  {
    no warnings 'once';
    my $link_info = $main::MYEXE_LINK_INFO;
    
    my $config = SPVM::Builder::Config->new_gnu99(file_optional => 1);
    
    ok($link_info->class_name, 'TestCase::NativeAPI2');
    ok($link_info->ld, $config->ld);
    ok($link_info->ldflags, $config->ldflags);
    like($link_info->output_file, qr|$build_dir/work/myexe$Config{exe_ext}|);
    my $is_object_file_infos = 1;
    for my $object_file_info (@{$link_info->object_file_infos}) {
      unless ($object_file_info->isa('SPVM::Builder::ObjectFileInfo')) {
        $is_object_file_infos = 0;
      }
    }
    ok($is_object_file_infos);
  }

  {
    no warnings 'once';
    my $compile_info = $main::MYEXE_COMPILE_INFO_BEFORE_COMPILE;
    ok(ref $compile_info eq 'SPVM::Builder::CompileInfo');
  }

  {
    no warnings 'once';
    my $compile_info = $main::MYEXE_COMPILE_INFO_GLOBAL_BEFORE_COMPILE;
    ok(ref $compile_info eq 'SPVM::Builder::CompileInfo');
  }
}

# Exe name contain sub directory
{
  my $exe_dir = "$test_dir/.spvm_build/work/exe";
  mkpath $exe_dir;
  
  my $class_name = 'MyExe::Foo::Bar';
  my $module_dirs = [map { "$_/SPVM" } "$test_dir/lib", @INC];
  my $output_file = "$build_dir/work/myexe";

  my $builder_exe = SPVM::Builder::Exe->new(
    class_name => $class_name,
    output_file => $output_file,
    build_dir => $build_dir,
    module_dirs => $module_dirs,
  );
  
  $builder_exe->build_exe_file;
}

ok(1);

done_testing;
