package SPVM::Builder::Exe;

use strict;
use warnings;
use Carp 'confess';
use Pod::Usage 'pod2usage';
use Config;

use SPVM::Builder;
use SPVM::Builder::CC;
use SPVM::Builder::Util;
use File::Find 'find';

use Getopt::Long 'GetOptions';

use ExtUtils::CBuilder;
use File::Copy 'copy', 'move';
use File::Path 'mkpath';
use DynaLoader;
use Scalar::Util 'weaken';

use File::Basename 'dirname', 'basename';

sub builder { shift->{builder} }

sub new {
  my $class = shift;
  
  my $self = {@_};

  # Package name
  my $package_name = $self->{package_name};
  unless (defined $package_name) {
    confess "Package name not specified";
  }
  
  # Excutable file name
  my $exe_name = $self->{exe_name};
  unless (defined $exe_name) {
    $exe_name = $package_name;
    $exe_name =~ s/::/__/g;
    $self->{exe_name} = $exe_name;
  }
  
  # Build directory
  my $build_dir = $self->{build_dir};
  unless (defined $build_dir) {
    $self->{build_dir} = 'spvm_build/work/exe';
  }
  
  # Quiet output
  unless (exists $self->{quiet}) {
    $self->{quiet} = 0;
  }
  
  # Library paths
  unless (exists $self->{library_path}) {
    $self->{library_path} = [];
  }

  # Library
  unless (exists $self->{library}) {
    $self->{library} = [];
  }
  
  return bless $self, $class;
}

sub build_exe_file {
  my ($self) = @_;

  # Package name
  my $package_name = $self->{package_name};
  
  # Excutable file name
  my $exe_name = $self->{exe_name};
  
  # Build directory
  my $build_dir = $self->{build_dir};
  mkpath $build_dir;
  
  # New SPVM::Builder object
  my $builder = SPVM::Builder->new(build_dir => $build_dir);
  $self->{builder} = $builder;
  
  # Compile SPVM
  my $file = 'internal';
  my $line = 0;
  my $compile_success = $builder->compile_spvm($package_name, $file, $line);
  unless ($compile_success) {
    exit(255);
  }

  # Compiled package names
  my $package_names = $builder->get_package_names;
  
  for my $package_name (@$package_names) {
    # This source is UTF-8 binary
    my $module_source = $builder->get_module_source($package_name);
    
    my $module_source_c_hex = $module_source;
    
    # Escape to Hex C launguage string literal
    $module_source_c_hex =~ s/(.)/$_ = sprintf("\\x%02X", ord($1));$_/ges;
    
    # native package name
    my $native_package_name = $package_name;
    $native_package_name =~ s/::/__/g;
    
    my $module_source_get_func = <<"EOS";
const char* SPMODULESOURCE__${native_package_name}__get() {
  const char* module_source = "$module_source_c_hex";
  return module_source;
}
EOS
  }
}

sub compile_spvm_csources {
  my ($self, $package_name, $sub_names, $opt) = @_;
}

sub create_main_csource {
  my ($self, $package_name) = @_;
}

sub compile_main {
  my ($self) = @_;
}

sub link_executable {
  my ($self, $package_name, $all_libs) = @_;
}

1;
