use Test::More;
use strict;
use warnings;
use utf8;
use Data::Dumper;
use File::Basename 'basename';
use FindBin;
use Config;
use File::Path 'mkpath';
use File::Spec;

use SPVM::Builder;

my $file = 't/' . basename $0;

use FindBin;
use lib "$FindBin::Bin/exe/lib";

my $output_expect = 'AAA t/exe/myexe.pl 3 1 1 7 args1 args2';

{
  my $exe_dir = 't/.spvm_build/work/exe';
  mkpath $exe_dir;
  
  # Basic
  {
    my $spvmcc_cmd = qq($^X -Mblib blib/script/spvmcc -B t/exe/.spvm_build -I t/exe/lib/SPVM -o t/.spvm_build/work/exe/myexe -c t/exe/myexe.config MyExe);
    system($spvmcc_cmd) == 0
      or die "Can't execute spvmcc command $spvmcc_cmd:$!";

    my $execute_cmd = File::Spec->catfile(qw/t .spvm_build work exe myexe/);
    my $execute_cmd_with_args = "$execute_cmd args1 args2";
    system($execute_cmd_with_args) == 0
      or die "Can't execute command:$execute_cmd_with_args:$!";
    
    my $output = `$execute_cmd_with_args`;
    chomp $output;
    my $output_expect = 'AAA t/.spvm_build/work/exe/myexe 3 1 1 7 args1 args2';
    is($output, $output_expect);
  }
  
  # -O, -f,  --ccflags, --lddlflags
  {
    my $spvmcc_cmd = qq($^X -Mblib blib/script/spvmcc -f -O "-O0 -g" -B t/exe/.spvm_build -I t/exe/lib/SPVM -o t/.spvm_build/work/exe/myexe --config t/exe/myexe.config MyExe);
    system($spvmcc_cmd) == 0
      or die "Can't execute spvmcc command $spvmcc_cmd:$!";

    my $execute_cmd = File::Spec->catfile(qw/t .spvm_build work exe myexe/);
    my $execute_cmd_with_args = "$execute_cmd args1 args2";
    system($execute_cmd_with_args) == 0
      or die "Can't execute command: $execute_cmd_with_args:$!";
  }
}

# SPVM script
{
  $ENV{SPVM_BUILD_DIR} = "$FindBin::Bin/.spvm_build";
  my $spvm_script = qq($^X -Mblib -I t/exe/lib t/exe/myexe.pl args1 args2);
  system($spvm_script) == 0
    or die "Can't execute SPVM script: $spvm_script:$!";
}

ok(1);
done_testing;
