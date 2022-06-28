package SPVM::Builder::Config;

use strict;
use warnings;
use Config;
use Carp 'confess';
use File::Basename 'dirname';
use SPVM::Builder::Util;

# Fields
sub file_optional {
  my $self = shift;
  if (@_) {
    $self->{file_optional} = $_[0];
    return $self;
  }
  else {
    return $self->{file_optional};
  }
}

sub file {
  my $self = shift;
  if (@_) {
    $self->{file} = $_[0];
    return $self;
  }
  else {
    return $self->{file};
  }
}

sub ext {
  my $self = shift;
  if (@_) {
    $self->{ext} = $_[0];
    return $self;
  }
  else {
    return $self->{ext};
  }
}

sub quiet {
  my $self = shift;
  if (@_) {
    $self->{quiet} = $_[0];
    return $self;
  }
  else {
    return $self->{quiet};
  }
}

sub cc {
  my $self = shift;
  if (@_) {
    $self->{cc} = $_[0];
    return $self;
  }
  else {
    return $self->{cc};
  }
}

sub cc_each {
  my $self = shift;
  if (@_) {
    $self->{cc_each} = $_[0];
    return $self;
  }
  else {
    return $self->{cc_each};
  }
}

sub ccflags {
  my $self = shift;
  if (@_) {
    $self->{ccflags} = $_[0];
    return $self;
  }
  else {
    return $self->{ccflags};
  }
}

sub ccflags_each {
  my $self = shift;
  if (@_) {
    $self->{ccflags_each} = $_[0];
    return $self;
  }
  else {
    return $self->{ccflags_each};
  }
}

sub optimize {
  my $self = shift;
  if (@_) {
    $self->{optimize} = $_[0];
    return $self;
  }
  else {
    return $self->{optimize};
  }
}

sub optimize_each {
  my $self = shift;
  if (@_) {
    $self->{optimize_each} = $_[0];
    return $self;
  }
  else {
    return $self->{optimize_each};
  }
}

sub ld_optimize {
  my $self = shift;
  if (@_) {
    $self->{ld_optimize} = $_[0];
    return $self;
  }
  else {
    return $self->{ld_optimize};
  }
}

sub include_dirs {
  my $self = shift;
  if (@_) {
    $self->{include_dirs} = $_[0];
    return $self;
  }
  else {
    return $self->{include_dirs};
  }
}

sub builder_include_dir {
  my $self = shift;
  if (@_) {
    $self->{builder_include_dir} = $_[0];
    return $self;
  }
  else {
    return $self->{builder_include_dir};
  }
}

sub builder_src_dir {
  my $self = shift;
  if (@_) {
    $self->{builder_src_dir} = $_[0];
    return $self;
  }
  else {
    return $self->{builder_src_dir};
  }
}

sub own_include_dir {
  my $self = shift;
  if (@_) {
    $self->{own_include_dir} = $_[0];
    return $self;
  }
  else {
    return $self->{own_include_dir};
  }
}

sub own_src_dir {
  my $self = shift;
  if (@_) {
    $self->{own_src_dir} = $_[0];
    return $self;
  }
  else {
    return $self->{own_src_dir};
  }
}

sub ld {
  my $self = shift;
  if (@_) {
    $self->{ld} = $_[0];
    return $self;
  }
  else {
    return $self->{ld};
  }
}

sub ldflags {
  my $self = shift;
  if (@_) {
    $self->{ldflags} = $_[0];
    return $self;
  }
  else {
    return $self->{ldflags};
  }
}

sub dynamic_lib_ldflags {
  my $self = shift;
  if (@_) {
    $self->{dynamic_lib_ldflags} = $_[0];
    return $self;
  }
  else {
    return $self->{dynamic_lib_ldflags};
  }
}

sub lib_dirs {
  my $self = shift;
  if (@_) {
    $self->{lib_dirs} = $_[0];
    return $self;
  }
  else {
    return $self->{lib_dirs};
  }
}

sub libs {
  my $self = shift;
  if (@_) {
    $self->{libs} = $_[0];
    return $self;
  }
  else {
    return $self->{libs};
  }
}

sub source_files {
  my $self = shift;
  if (@_) {
    $self->{source_files} = $_[0];
    return $self;
  }
  else {
    return $self->{source_files};
  }
}

sub force {
  my $self = shift;
  if (@_) {
    $self->{force} = $_[0];
    return $self;
  }
  else {
    return $self->{force};
  }
}

sub before_link {
  my $self = shift;
  if (@_) {
    $self->{before_link} = $_[0];
    return $self;
  }
  else {
    return $self->{before_link};
  }
}

sub dependent_files {
  my $self = shift;
  if (@_) {
    $self->{dependent_files} = $_[0];
    return $self;
  }
  else {
    return $self->{dependent_files};
  }
}

sub output_type {
  my $self = shift;
  if (@_) {
    $self->{output_type} = $_[0];
    return $self;
  }
  else {
    return $self->{output_type};
  }
}

# Methods
sub new {
  my $class = shift;
  
  my $self = {@_};

  bless $self, ref $class || $class;
  
  my $file_optional = $self->file_optional;
  
  my $file = $self->file;
  if (!$file_optional && !defined $file) {
    confess "\"file\" option must be specified";
  }
  
  # quiet
  unless (defined $self->{quiet}) {
    $self->quiet(1);
  }

  # force
  unless (defined $self->{force}) {
    $self->force(0);
  }
  
  # ext
  unless (defined $self->{ext}) {
    $self->ext(undef);
  }

  # cc
  unless (defined $self->{cc}) {
    $self->cc($Config{cc});
  }

  my $builder_dir = SPVM::Builder::Util::get_builder_dir_from_config_module();
  
  # builder_include_dir
  unless (defined $self->{builder_include_dir}) {
    my $builder_include_dir = "$builder_dir/include";
    $self->builder_include_dir($builder_include_dir);
  }

  # builder_src_dir
  unless (defined $self->{builder_src_dir}) {
    my $builder_src_dir = "$builder_dir/src";
    $self->builder_src_dir($builder_src_dir);
  }
  
  # include_dirs
  unless (defined $self->{include_dirs}) {
    $self->include_dirs([]);
  }

  # Resource directory
  if (defined $file) {
    my $resource_dir = $self->remove_ext_from_config_file($file);
    
    $resource_dir .= '.native';
    
    # own_include_dir
    unless (defined $self->{own_include_dir}) {
      my $own_include_dir = "$resource_dir/include";
      $self->own_include_dir($own_include_dir);
    }

    # own_src_dir
    unless (defined $self->{own_src_dir}) {
      my $own_src_dir = "$resource_dir/src";
      $self->own_src_dir($own_src_dir);
    }
  }
  
  # ccflags
  unless (defined $self->{ccflags}) {
    $self->ccflags([]);
    
    my @default_ccflags;
    
    # If dynamic link libraries must link position independent codes, add -fPIC option.
    if ($Config{cccdlflags} =~ /-fPIC\b/) {
      push @default_ccflags, '-fPIC';
    }
    
    $self->add_ccflags(@default_ccflags);
  }

  # optimize
  unless (defined $self->{optimize}) {
    $self->optimize('-O3');
  }
  
  # ld
  unless (defined $self->{ld}) {
    $self->ld($Config{ld});
  }

  # lib_dirs
  unless (defined $self->{lib_dirs}) {
    $self->lib_dirs([]);
  }

  # resources
  unless (defined $self->{resources}) {
    $self->{resources} = {};
  }

  # source_files
  unless (defined $self->{source_files}) {
    $self->source_files([]);
  }
  
  # libs
  unless (defined $self->{libs}) {
    $self->libs([]);
  }
  
  # ld_optimize
  unless (defined $self->{ld_optimize}) {
    $self->ld_optimize('-O2');
  }

  unless (defined $self->{dependent_files}) {
    $self->{dependent_files} = [];
  }
  
  unless (defined $self->output_type) {
    $self->output_type('dynamic_lib');
  }

  # dynamic_lib_ldflags
  unless (defined $self->{dynamic_lib_ldflags}) {
    $self->dynamic_lib_ldflags([]);
    
    if ($self->output_type eq 'dynamic_lib') {
      my @dynamic_lib_ldflags;
      
      # Dynamic link options
      if ($^O eq 'MSWin32') {
        push @dynamic_lib_ldflags, '-mdll', '-s';
      }
      else {
        push @dynamic_lib_ldflags, '-shared';
      }
      $self->dynamic_lib_ldflags(\@dynamic_lib_ldflags);
    }
  }

  # ldflags
  unless (defined $self->{ldflags}) {
    $self->ldflags([]);
  }
  
  return $self;
}

sub new_c {
  my $class = shift;
  
  my $self = $class->new(@_);
  
  # NativeAPI
  $self->ext('c');
  
  return $self;
}

sub new_c99 {
  my $class = shift;
  
  my $self = $class->new_c(@_);
  
  # C99
  $self->set_std('c99');
  
  return $self;
}

sub new_gnu99 {
  my $class = shift;
  
  my $self = $class->new_c(@_);
  
  # C99
  $self->set_std('gnu99');
  
  return $self;
}

sub new_cpp {
  my $class = shift;
  
  my $self = $class->new(@_);
  
  # The compiler
  # [Memo]Free BSD don't have g++ in the environment clang++ exists.
  # [Memo]"Clang" or "clang" is assumed.
  my $config_gcc_version = $Config{gccversion};
  if ($config_gcc_version =~ /\bclang\b/i) {
    $self->cc('clang++');
    $self->ld('clang++');
  }
  else {
    $self->cc('g++');
    $self->ld('g++');
  }
  
  # NativeAPI
  $self->ext('cpp');
  
  return $self;
}

sub new_cpp11 {
  my $class = shift;
  
  my $self = $class->new_cpp(@_);
  
  # C++11
  $self->set_std('c++11');
  
  return $self;
}

sub add_ccflags {
  my ($self, @ccflags) = @_;
  
  push @{$self->{ccflags}}, @ccflags;
}

sub add_include_dirs {
  my ($self, @include_dirs) = @_;
  
  push @{$self->{include_dirs}}, @include_dirs;
}

sub set_std {
  my ($self, $standard) = @_;
  
  my $ccflags = $self->ccflags;
  
  push @$ccflags, "-std=$standard";
  
  # Add -std=foo section
  $self->ccflags($ccflags);
  
  return $self;
}

sub add_ldflags {
  my ($self, @ldflags) = @_;
  
  push @{$self->{ldflags}}, @ldflags;
}

sub add_lib_dirs {
  my ($self, @lib_dirs) = @_;
  
  push @{$self->{lib_dirs}}, @lib_dirs;
}

sub add_libs {
  my ($self, @libs) = @_;
  
  push @{$self->{libs}}, @libs;
}

sub add_source_files {
  my ($self, @source_files) = @_;
  
  push @{$self->{source_files}}, @source_files;
}

sub load_config {
  my ($self, $config_file, @args) = @_;

  unless (-f $config_file) {
    confess "Can't find config file \"$config_file\"";
  }
  local @ARGV = @args;
  my $config = do File::Spec->rel2abs($config_file);
  if ($@) {
    confess "Can't parse config file \"$config_file\": $@";
  }
  
  unless (defined $config && $config->isa('SPVM::Builder::Config')) {
    confess "The config file must be a SPVM::Builder::Config object";
  }
  
  push @{$config->dependent_files}, $config_file;
  
  return $config;
}


sub remove_ext_from_config_file {
  my ($self, $config_file) = @_;
  
  my $config_file_without_ext = $config_file;
  
  $config_file_without_ext =~ s/(\.[a-zA-Z0-9_]+)?\.config$//;
  
  return $config_file_without_ext;
}

sub load_mode_config {
  my ($self, $config_file, $mode, @argv) = @_;
  
  my $mode_config_file = $self->remove_ext_from_config_file($config_file);
  if (defined $mode) {
    $mode_config_file .= ".$mode";
  }
  $mode_config_file .= ".config";
  
  unless (-f $mode_config_file) {
    confess "Can't find the config file \"$mode_config_file\"";
  }
  
  my $config = $self->load_config($mode_config_file, @argv);
  
  return $config;
}

sub load_base_config {
  my ($self, $config_file, @args) = @_;
  
  my $config = $self->load_mode_config($config_file, undef, @args);

  return $config;
}

sub add_static_libs {
  my ($self, @static_libs) = @_;
  
  my @static_lib_infos = map { {type => 'static', name => $_ } } @static_libs;
  
  $self->add_libs(@static_lib_infos);
}

sub add_dynamic_libs {
  my ($self, @dynamic_libs) = @_;
  
  my @dynamic_lib_infos = map { {type => 'dynamic', name => $_ } } @dynamic_libs;
  
  $self->add_libs(@dynamic_lib_infos);
}

sub use_resource {
  my ($self, @args) = @_;
  
  my $first_arg;
  unless (@args % 2 == 0) {
    $first_arg = shift @args;
  }
  
  my $resource;
  if (ref $first_arg) {
    $resource = $first_arg;
  }
  else {
    my $class_name = $first_arg;
    my %args = @args;
    if (exists $args{class_name}) {
      $class_name = delete $args{class_name};
    }
    $resource = SPVM::Builder::Resource->new(class_name => $class_name, %args);
  }
  
  my $resource_class_name = $resource->class_name;
  my $resource_mode = $resource->mode;
  my $resource_args = $resource->args;
  
  my $ext = defined $resource_mode ? "$resource_mode.config" : 'config';
  my $config_file_base = SPVM::Builder::Util::convert_class_name_to_rel_file($resource_class_name, $ext);
  
  my $config_file = SPVM::Builder::Util::get_config_file_from_class_name($resource_class_name, $resource_mode);
  
  my $config = $self->load_config($config_file, @$resource_args);
  $config->file($config_file);
  
  $resource->config($config);
  
  my $index = keys %{$self->{resources}};
  
  $self->{resources}->{$resource_class_name} = {resource => $resource, index => $index};
  
  return $resource;
}

sub get_resource {
  my ($self, $resource_class_name) = @_;
  
  unless (defined $self->{resources}{$resource_class_name}) {
    return;
  }
  
  my $resource = $self->{resources}{$resource_class_name}{resource};
  
  return $resource;
}

sub get_resource_names {
  my ($self) = @_;
  
  my @resource_names = sort { $self->{resources}{$a}{index} <=> $self->{resources}{$b}{index} } keys %{$self->{resources}};
  
  return \@resource_names;
}

1;

=head1 Name

SPVM::Builder::Config - Configurations of Compile and Link of Native Sources

=head1 Synopsys

  use SPVM::Builder::Config;
  
  # Create a config
  my $config = SPVM::Builder::Config->new(file => __FILE__);
  
  # Create a config with "GNU99" standard of "C" language
  my $config = SPVM::Builder::Config->new_gnu99(file => __FILE__);

  # Create a config with "C99" standard of "C" language
  my $config = SPVM::Builder::Config->new_c99(file => __FILE__);

  # Create a config as "C++"
  my $config = SPVM::Builder::Config->new_cpp(file => __FILE__);

  # Create a config with "C++11" standard of "C++"
  my $config = SPVM::Builder::Config->new_cpp11(file => __FILE__);
  
  # Optimize
  $config->optimize('-O2');
  
  # Optimize with debug mode
  $config->optimize('-O0 -g');
  
  # Add source files
  $config->add_source_files('foo.c', 'bar.c', 'baz/baz.c');
  
  # Use resource
  $config->use_resource('TestCase::Resource::Zlib::V1_0_0');
  $config->use_resource('TestCase::Resource::Foo1::V1_0_0', mode => 'mode1', args => ['args1', 'args2']);
  
  # Get resouce information
  my $resource = $config->get_resource('TestCase::Resource::Zlib::V1_0_0');

=head1 Description

L<SPVM::Builder::Config> is configuration of c/c++ compile and link.

=head1 Field Methods

Fields.

=head2 ext

  my $ext = $config->ext;
  $config->ext($ext);

Get and set the extension of the SPVM native source.

The default is C<undef>.

B<Examples:>
  
  # Foo/Bar.c
  $config->ext('c');
  
  # Foo/Bar.cpp
  $config->ext('cpp');
  
=head2 cc

  my $cc = $config->cc;
  $config->cc($cc);

Get and set a compiler name. The default is the value of C<cc> of L<Config> module.

B<Examples:>
  
  # gcc
  $config->cc('gcc');
  
  # g++ for C++
  $config->cc('g++');
  
  # nvcc for CUDA/GUP
  $config->cc('nvcc');
  
  # cc that compiled this Perl
  use Config;
  $config->cc($Config{cc});

=head2 cc_each

  my $cc_each = $config->cc_each;
  $config->cc_each($cc_each);

Get and set a callback that returns the compiler name for each source file. The call back receives L<SPVM::Bulder::Config> object and optional arguments as a hash reference.

B<Optional Arguments:>

=over 2

=item * source_file

Each source file.

=item * class_name

The class name

=back

If C<cc_each> is defined, the compiler use the return value of C<cc_each> as the compiler name instead of C<cc>.

B<Examples:>
  
  $config->cc_each(sub {
    my ($config, $args) = @_;
    
    # Source file
    my $source_file = $args->{source_file};
    
    # Class name
    my $class_name = $args->{class_name}
    
    my $cc;
    # C source file
    if ($source_file =~ /\.c$/) {
      $cc = 'gcc';
    }
    # C++ source file
    elsif ($source_file =~ /\.cpp$/) {
      $cc = 'g++';
    }
    
    return $cc;
  });

=head2 include_dirs

  my $include_dirs = $config->include_dirs;
  $config->include_dirs($include_dirs);

Get and set header including directories of the compiler. This is same as C<-I> option of C<gcc>. 

=head2 builder_include_dir

  my $builder_include_dir = $config->builder_include_dir;
  $config->builder_include_dir($builder_include_dir);

Get and set the header including directory of L<SPVM::Builder>.

The default value is C<SPVM/Builder/include> of one up of directory that C<SPVM::Buidler::Config> is loaded.

=head2 builder_src_dir

  my $builder_src_dir = $config->builder_src_dir;
  $config->builder_src_dir($builder_src_dir);

Get and set the source directory of L<SPVM::Builder>.

The default value is C<SPVM/Builder/src> of one up of the directory that C<SPVM::Buidler::Config> is loaded.

=head2 own_include_dir

  my $own_include_dir = $config->own_include_dir;
  $config->own_include_dir($own_include_dir);

Get and set the header including directory of this module.

The default value is the name that removing C<[.mode].config> from the L<file|/"file"> and add C<.native/include>.

=head2 own_src_dir

  my $own_src_dir = $config->own_src_dir;
  $config->own_src_dir($own_src_dir);

Get and set the source directory of this module.

The default value is the name that removing C<[.mode].config> from the L<file|/"file"> and add C<.native/src>.

=head2 ccflags

  my $ccflags = $config->ccflags;
  $config->ccflags($ccflags);

Get and set compiler flags.

B<Default:>

  # $Config{cccdlflags} has -fPIC.
  ['-fPIC']
  
  # Other
  []

=head2 ccflags_each

  my $ccflags_each = $config->ccflags_each;
  $config->ccflags_each($ccflags_each);

Get and set a callback that returns the compiler flags for each source file. The call back receives L<SPVM::Bulder::Config> object and optional arguments as a hash reference.

B<Optional Arguments:>

=over 2

=item * source_file

Each source file.

=item * class_name

The class name

=item * cc

The compiler name that is the value after the process of the process of L<cc> or L<cc_each>.

=back

If C<ccflags_each> is defined, the compiler use the return value of C<ccflags_each> as the compiler flags instead of C<ccflags>.

B<Examples:>
  
  $config->ccflags_each(sub {
    my ($config, $source_file) = @_;

    # Source file
    my $source_file = $args->{source_file};
    
    # Class name
    my $class_name = $args->{class_name}

    # The compiler name
    my $cc = $args->{cc};
    
    # ccflags
    my $config_ccflags = $config->ccflags;
    
    my $ccflags = [];
    # C source file
    if ($source_file =~ /\.c$/) {
      $ccflags = ['-DFoo', @$config_ccflags];
    }
    # C++ source file
    elsif ($source_file =~ /\.cpp$/) {
      $ccflags = ['-DBar', @$config_ccflags];
    }
    
    return $ccflags;
  });

=head2 optimize

  my $optimize = $config->optimize;
  $config->optimize($optimize);

Get and set the option for optimization of the compiler.

The default is C<-O3>.

B<Examples:>

  $config->optimize('-O3');
  $config->optimize('-O2');
  $config->optimize('-g3 -O0');

=head2 optimize_each

  my $optimize_each = $config->optimize_each;
  $config->optimize_each($optimize_each);

Get and set a callback that returns the value of optimize for each source file. The callback receives L<SPVM::Bulder::Config> object and optional arguments as a hash reference.

B<Optional Arguments:>

=over 2

=item * source_file

Each source file.

=item * class_name

The class name

=item * cc

The compiler name that is the value after the process of the process of L<cc> or L<cc_each>.

=back

If C<optimize_each> is defined, the compiler use the return value of C<optimize_each> as the compiler flags instead of C<optimize>.

B<Examples:>
  
  $config->optimize_each(sub {
    my ($config, $source_file) = @_;

    # Source file
    my $source_file = $args->{source_file};
    
    # Class name
    my $class_name = $args->{class_name}

    # The compiler name
    my $cc = $args->{cc};
    
    # optimize
    my $config_optimize = $config->optimize;
    
    my $optimize = [];
    # C source file
    if ($source_file =~ /\.c$/) {
      $optimize = '-O3';
    }
    # C++ source file
    elsif ($source_file =~ /\.cpp$/) {
      $optimize = '-O3';
    }
    
    return $optimize;
  });

=head2 source_files

  my $source_files = $config->source_files;
  $config->source_files($source_files);

Get and get source files. The file name is the relative pass from L</"own_src_dir">.

B<Examples:>

  $config->source_files(['foo.c', 'bar.c']);

=head2 ld

  my $ld = $config->ld;
  $config->ld($ld);

Get and set a linker. Default is C<ld> of L<Config> module.

=head2 lib_dirs

  my $lib_dirs = $config->lib_dirs;
  $config->lib_dirs($lib_dirs);

Get and set the directories that libraries are searched for by the linker. This is same as C<-L> option of C<gcc>.

B<Default:>

Windows
  
  The directory that perlxxx.dll exists
  
Not Windows

  empty list

=head2 libs

  my $libs = $config->libs;
  $config->libs($libs);

Get and set libraries. These libraries are linked by the linker.

If a dynamic link library is found from L<"lib_dirs">, this is linked. Otherwise if a static link library is found from L<"lib_dirs">, this is linked.

B<Examples:>

  $config->libs(['gsl', 'png']);

If you want to link only dynamic link library, you can use the following hash reference as the value of the element instead of the library name.

  {type => 'dynamic', name => 'gsl'}

If you want to link only static link library, you can use the following hash reference as the value of the element instead of the library name.

  {type => 'static', name => 'gsl'}

=head2 ldflags

  my ldflags = $config->ldflags;
  $config->ldflags(ldflags);

Get and set linker flags. The default value is an emtpy array reference.

=head2 dynamic_lib_ldflags

  my dynamic_lib_ldflags = $config->dynamic_lib_ldflags;
  $config->dynamic_lib_ldflags(dynamic_lib_ldflags);

Get and set linker flags for dynamic link.

B<Default:>

Windows

  ['-mdll', '-s']
  
Non-Windows

  ['-shared']

=head2 ld_optimize

  my $ld_optimize = $config->ld_optimize;
  $config->ld_optimize($ld_optimize);

Get and set the option for optimization of the linker such as C<-O3>, C<-O2>, C<-g3 -O0>.

The default is C<-O2>.

=head2 force

  my $force = $config->force;
  $config->force($force);

Get and set the flag to force compiles and links without caching.

=head2 before_link

  my $before_link = $config->before_link;
  $config->before_link($before_link);

Get and set the callback that is executed before the link. The callback receives L<SPVM::Builder::Config> object and the L<SPVM::Builder::LinkInfo> object used by the linker.

B<Examples:>

  $config->before_link(sub {
    my ($config, $link_info) = @_;
    
    my $object_file_infos = $link_info->object_file_infos;
    
    # Do something
    
  });

=head2 quiet

  my $quiet = $config->quiet;
  $config->quiet($quiet);

Get and set the flag if the compiler and the linker output the results.

The default is C<1>.

=head2 file

  my $file = $config->file;
  $config->file($file);

Get and set the config file path.

The default is C<1>.

=head2 file_optional

  my $file_optional = $config->file_optional;
  $config->file_optional($file_optional);

Get and set the value that indicates L<file|/"file"> field is needed for C<new|/"new"> method.

The default is C<0>.

=head2 output_type

  my $output_type = $config->output_type;
  $config->output_type($type);

=head1 Class Methods

=head2 new

  my $config = SPVM::Builder::Config->new(file => __FILE__);
  
Create L<SPVM::Builder::Config> object.

=head2 new_c
  
  my $config = SPVM::Builder::Config->new_c(file => __FILE__);

Create default build config with C settings. This is L<SPVM::Builder::Config> object.

If you want to use the specific C version, use C<set_std> method.

  $config->set_std('c99');

=head2 new_c99
  
  my $config = SPVM::Builder::Config->new_gnu99(file => __FILE__);

Create default build config with C99 settings. This is L<SPVM::Builder::Config> object.

=head2 new_cpp
  
  my $config = SPVM::Builder::Config->new_cpp(file => __FILE__);

Create default build config with C++ settings. This is L<SPVM::Builder::Config> object.

If you want to use the specific C++ version, use C<set_std> method.

  $config->set_std('c++11');

=head2 new_cpp11
  
  my $config = SPVM::Builder::Config->new_cpp11(file => __FILE__);

Create default build config with C++11 settings. This is L<SPVM::Builder::Config> object.

=head1 Instance Methods

=head2 set_std

  $config->set_std($std);

Add the value that is converted to C<-std=$std> after the last element of C<ccflags> field.

B<Example:>

  $config->set_std('gnu99');

=head2 add_ccflags

  $config->add_ccflags(@ccflags);

Add values after the last element of C<ccflags> field.

=head2 add_ldflags

  $config->add_ldflags(@ldflags);

Add values after the last element of C<ldflags> field.

=head2 add_include_dirs

  $config->add_include_dirs(@include_dirs);

Add values after the last element of C<include_dirs> field.

=head2 add_lib_dirs

  $config->add_lib_dirs(@lib_dirs);

Add values after the last element of  C<lib_dirs> field.

=head2 add_source_files

  $config->add_source_files(@source_files);

Add the values after the last element of C<source_files> field.

=head2 add_libs

  $config->add_libs(@libs);

Add the values after the last element of C<libs> field.

B<Examples:>

  $config->add_libs('gsl');

=head2 add_static_libs

  $config->add_static_libs(@libs);

Add the values that each element is converted to the following hash reference after the last element of C<libs> field.

  {type => 'static', name => $lib}

B<Examples:>

  $config->add_static_libs('gsl');

=head2 add_dynamic_libs

  $config->add_dynamic_libs(@libs);

Add the values that each element is converted to the following hash reference after the last element of C<libs> field.

  {type => 'dynamic', name => $lib}

B<Examples:>

  $config->add_dynamic_libs('gsl');

=head2 use_resource

  $config->use_resource($resource);
  $config->use_resource('Resource::Zlib::V1_0_0');
  $config->use_resource('Resource::Zlib::V1_0_0', mode => 'prod', args => ['foo', 'bar']);

Use a resource. 

The first argument is a L<SPVM::Builder::Resource> object.

If the first argument is a class name of the resource, a L<SPVM::Builder::Resource> object is created by L<SPVM::Builder::Resource|/"new"> method with C<class_name> option.

  my $resource = SPVM::Builder::Resource->new(class_name => 'Resource::Zlib::V1_0_0');
  $config->use_resource($resource);

If the rest arguments are used as the options of L<SPVM::Builder::Resource|/"new"> of L<SPVM::Builder::Resource>.

  my $resource = SPVM::Builder::Resource->new(
    class_name => 'Resource::Zlib::V1_0_0',
    mode => 'prod',
    args => ['foo', 'bar'],
  );
  $config->use_resource($resource);

=head2 get_resource

  my $resource = $config->get_resource('Resource::Zlib::V1_0_0');

Get a resource. The resource is a L<SPVM::Builder::Resource> object.

=head2 get_resource_names

  my $resource_names = $config->get_resource_names;

Get resource names.
