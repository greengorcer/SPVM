package SPVM::Build::Precompile;

use strict;
use warnings;
use Carp 'croak', 'confess';
use File::Spec;

use ExtUtils::CBuilder;
use Config;
use File::Copy 'move';
use File::Path 'mkpath', 'rmtree';

use File::Basename 'dirname', 'basename';

use SPVM::Build::SPVMInfo;
use SPVM::Build::Util;

sub new {
  my $class = shift;
  
  my $self = {};

  $self->{category} = 'precompile';
  
  return bless $self, $class;
}

sub compile_packages {
  my $self = shift;
  
  my $packages = SPVM::Build::SPVMInfo::get_packages();
  for my $package (@$packages) {
    if ($package->{is_precompile} && !$package->{is_interface}) {
      $self->compile_precompile_package($package);
    }
  }
}

sub compile_precompile_package {
  my ($self, $package) = @_;
  
  my $package_id = $package->{id};
  my $package_name = $package->{name};
  
  my $subs = SPVM::Build::SPVMInfo::get_subs_from_package_id($package->{id});
  $subs = [grep { !$_->{have_native_desc} && !$_->{is_enum} } @$subs];

  my $csource_source = '';
  for my $sub (@$subs) {
    my $sub_name = $sub->{name};
    my $sub_id = $sub->{id};
    
    my $sub_csource_source = $self->build_csource($sub_id);
    $csource_source .= "$sub_csource_source\n";

    # Precompile Subroutine names
    my $native_sub_name = $sub_name;
    $native_sub_name =~ s/:/_/g;
    $native_sub_name = "SPVM_BUILD_COMPILE_$native_sub_name";
    
    $sub->{native_sub_name} = $native_sub_name;
  }

  # Build Precompile code
  my $build_dir = $SPVM::BUILD_DIR;
  unless (defined $build_dir && -d $build_dir) {
    confess "SPVM build directory must be specified for precompile";
  }
  
  my $precompile_package_file_name = $package_name;
  $precompile_package_file_name =~ s/::/__/g;
  
  my $precompile_source_file = "$build_dir/$precompile_package_file_name.c";
  my $precompile_shared_lib_file = "$build_dir/$precompile_package_file_name.$Config{dlext}";

  # Get old csource source
  my $old_csource_source;
  if (-f $precompile_source_file) {
    open my $fh, '<', $precompile_source_file
      or die "Can't open $precompile_source_file";
    $old_csource_source = do { local $/; <$fh> };
  }
  else {
    $old_csource_source = '';
  }
  
  # Only compile when source is different
  if ($csource_source ne $old_csource_source) {
  
    # Compile Precompile code
    open my $fh, '>', $precompile_source_file
      or die "Can't create $precompile_source_file";
    print $fh $csource_source;
    close $fh;
    
    {
      my $source_file = $precompile_source_file;
      
      # Source directory
      my $source_dir = dirname $source_file;
      
      # Object created directory
      my $object_dir = $source_dir;
      
      # Include directory
      my $include_dirs = [];
      
      # Default include path
      my $env_header_include_dir = $INC{"SPVM/Build/Native.pm"};
      $env_header_include_dir =~ s/\/Build\/Native\.pm$//;
      push @$include_dirs, $env_header_include_dir;
      
      my $cbuilder_config = {};
      
      # OPTIMIZE
      $cbuilder_config->{optimize} ||= $SPVM::BUILD->native->optimize;
      
      # Compile source files
      my $quiet = 1;
      my $cbuilder = ExtUtils::CBuilder->new(quiet => $quiet, config => $cbuilder_config);
      my $object_files = [];
      
      # Object file
      my $object_file = $source_file;
      $object_file =~ s/\.c$//;
      $object_file .= '.o';
      
      # Compile source file
      $cbuilder->compile(
        source => $source_file,
        object_file => $object_file,
        include_dirs => $include_dirs,
        extra_compiler_flags => $SPVM::BUILD->native->extra_compiler_flags
      );
      push @$object_files, $object_file;
      
      my $native_sub_names = [map { $_->{native_sub_name} } @$subs];
      my $lib_file = $cbuilder->link(
        objects => $object_files,
        module_name => $precompile_package_file_name,
        dl_func_list => $native_sub_names,
      );
    }
  }
  
  # Bind precompile subroutine
  for my $sub (@$subs) {
    my $sub_name = $sub->{name};
    my $native_sub_name = $sub->{native_sub_name};
    my $sub_precompile_address = SPVM::Build::Util::get_shared_lib_func_address($precompile_shared_lib_file, $native_sub_name);
    
    $self->bind_csource_sub($sub_name, $sub_precompile_address);
  }
}

sub create_precompile_sub_file_name {
  my ($self, $sub_name) = @_;
  
  my $precompile_sub_file_name = $sub_name;
  
  $precompile_sub_file_name =~ s/:/_/g;

  # In windows, upper case file name and lower case file name.
  # so If FOO and foo subroutine is exists, one overwrite another.
  # I want to prevent this. so '-' is appended to upper case character
  $precompile_sub_file_name =~ s/([A-Z])/-$1/g;
  
  $precompile_sub_file_name = "SPVM_BUILD_COMPILE_$precompile_sub_file_name";

  return $precompile_sub_file_name;
}

sub compile_precompile_sub {
  my ($self, $sub_id, $sub_csource_source) = @_;
  
  my $sub_abs_name = SPVM::Build::SPVMInfo::get_sub_name($sub_id);
  my $precompile_sub_name = $self->create_precompile_sub_name($sub_abs_name);
  my $precompile_sub_file_name = $self->create_precompile_sub_file_name($sub_abs_name);
  
  # Build Precompile code
  my $build_dir = $SPVM::BUILD_DIR;
  unless (defined $build_dir && -d $build_dir) {
    confess "SPVM build directory must be specified for precompile";
  }
  
  my $precompile_source_file = "$build_dir/$precompile_sub_file_name.c";
  my $precompile_shared_lib_file = "$build_dir/$precompile_sub_file_name.$Config{dlext}";

  # Compile Precompile code
  open my $fh, '>', $precompile_source_file
    or die "Can't create $precompile_source_file";
  print $fh $sub_csource_source;
  close $fh;
  
  {
    my $source_file = $precompile_source_file;
    
    # Source directory
    my $source_dir = dirname $source_file;
    
    # Object created directory
    my $object_dir = $source_dir;
    
    # Include directory
    my $include_dirs = [];
    
    # Default include path
    my $env_header_include_dir = $INC{"SPVM/Build/Native.pm"};
    $env_header_include_dir =~ s/\/Build\/Native\.pm$//;
    push @$include_dirs, $env_header_include_dir;
    
    my $cbuilder_config = {};
    
    # OPTIMIZE
    $cbuilder_config->{optimize} ||= $SPVM::BUILD->native->optimize;
    
    # Compile source files
    my $quiet = 1;
    my $cbuilder = ExtUtils::CBuilder->new(quiet => $quiet, config => $cbuilder_config);
    my $object_files = [];
    
    # Object file
    my $object_file = $source_file;
    $object_file =~ s/\.c$//;
    $object_file .= '.o';
    
    # Compile source file
    $cbuilder->compile(
      source => $source_file,
      object_file => $object_file,
      include_dirs => $include_dirs,
      extra_compiler_flags => $SPVM::BUILD->native->extra_compiler_flags
    );
    push @$object_files, $object_file;

    # Precompile Subroutine names
    my $native_sub_name = $sub_abs_name;
    $native_sub_name =~ s/:/_/g;
    $native_sub_name = "SPVM_BUILD_COMPILE_$native_sub_name";
    
    my $lib_file = $cbuilder->link(
      objects => $object_files,
      module_name => $precompile_sub_file_name,
      dl_func_list => [$native_sub_name],
    );
  }
  
  my $sub_precompile_address = SPVM::Build::Util::get_shared_lib_func_address($precompile_shared_lib_file, $precompile_sub_name);
  
  $self->bind_csource_sub($sub_abs_name, $sub_precompile_address);
  
  my $success = 1;
  
  return $success;
}

1;
