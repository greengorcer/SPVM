use lib "t/lib";
use TestAuto;

use strict;
use warnings;

use Test::More 'no_plan';

use utf8;
use Data::Dumper;
use File::Basename 'basename';
use FindBin;
use Config;
use File::Path 'mkpath';
use File::Spec;

use SPVM::Builder::Exe;

my $file = 't/' . basename $0;

use FindBin;

{
  my $exe_dir = 't/spvm_build/work/exe';
  mkpath $exe_dir;
  
  my $target_package_name = 'MyExe';
  my $build_dir = 't/exe/.spvm_build';
  my $module_include_dir = ['t/exe/lib', @INC];
  my $output_file = "$build_dir/work/myexe";

  my $builder_exe = SPVM::Builder::Exe->new(
    target_package_name => $target_package_name,
    output_file => $output_file,
    build_dir => $build_dir,
    include_dirs => $module_include_dir,
  );
  
  $builder_exe->build_exe_file;
}

ok(1);
