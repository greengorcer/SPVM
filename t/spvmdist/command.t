use Test::More;
use strict;
use warnings;
use utf8;
use Data::Dumper;
use File::Basename 'basename';
use FindBin;
use Config;
use File::Path 'mkpath', 'rmtree';
use File::Spec;
use SPVM::Builder::Util;
use File::Temp();
use Cwd 'getcwd';

use SPVM::Builder;

my $file = 't/' . basename $0;

use FindBin;
use lib "$FindBin::Bin/exe/lib";

# Minimal
{
  my $spvmdist_path = File::Spec->rel2abs('blib/script/spvmdist');
  my $blib = File::Spec->rel2abs('blib/lib');
  
  my $tmp_dir = File::Temp->newdir;
  my $spvmdist_cmd = qq($^X -I$blib $spvmdist_path Foo);
  my $save_cur_dir = getcwd();
  chdir($tmp_dir) or die;
  system($spvmdist_cmd) == 0
    or die "Can't execute spvmdist command $spvmdist_cmd:$!";
  
  # Makefile.PL
  my $makefile_pl_file = "$tmp_dir/Foo/Makefile.PL";
  ok(-f $makefile_pl_file);
  ok(SPVM::Builder::Util::file_contains($makefile_pl_file, "'Foo'"));
  ok(SPVM::Builder::Util::file_contains($makefile_pl_file, "'lib/SPVM/Foo.pm'"));
  
  ok(-f "$tmp_dir/Foo/README.md");
  ok(-f "$tmp_dir/Foo/Changes");
  ok(-f "$tmp_dir/Foo/.gitignore");
  ok(-f "$tmp_dir/Foo/MANIFEST.SKIP");
  
  my $perl_module_file = "$tmp_dir/Foo/lib/SPVM/Foo.pm";
  ok(-f $perl_module_file);
  ok(SPVM::Builder::Util::file_contains($perl_module_file, "package SPVM::Foo;"));
  
  my $spvm_module_file = "$tmp_dir/Foo/lib/SPVM/Foo.spvm";
  ok(SPVM::Builder::Util::file_contains($spvm_module_file, "class Foo {"));
  ok(-f "$tmp_dir/Foo/lib/SPVM/Foo.spvm");
  ok(-f "$tmp_dir/Foo/t/basic.t");

  chdir($save_cur_dir) or die;
}

done_testing;
