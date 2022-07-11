package SPVM::Builder::LinkInfo;

use strict;
use warnings;
use Config;
use Carp 'confess';
use File::Basename 'dirname';

# Fields
sub output_file {
  my $self = shift;
  if (@_) {
    $self->{output_file} = $_[0];
    return $self;
  }
  else {
    return $self->{output_file};
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

sub object_file_infos {
  my $self = shift;
  if (@_) {
    $self->{object_file_infos} = $_[0];
    return $self;
  }
  else {
    return $self->{object_file_infos};
  }
}

sub lib_infos {
  my $self = shift;
  if (@_) {
    $self->{lib_infos} = $_[0];
    return $self;
  }
  else {
    return $self->{lib_infos};
  }
}

sub class_name {
  my $self = shift;
  if (@_) {
    $self->{class_name} = $_[0];
    return $self;
  }
  else {
    return $self->{class_name};
  }
}

sub config {
  my $self = shift;
  if (@_) {
    $self->{config} = $_[0];
    return $self;
  }
  else {
    return $self->{config};
  }
}

# Methods
sub to_string {
  my ($self) = @_;

  my $ld = $self->ld;
  my $ldflags = $self->ldflags;
  my $class_name = $self->class_name;
  my $output_file = $self->output_file;
  my $object_file_infos = $self->object_file_infos;
  my $lib_infos = $self->lib_infos;
  
  my $all_ldflags_str = '';
  
  my $ldflags_str = join(' ', @$ldflags);
  $all_ldflags_str .= $ldflags_str;
  
  my $lib_ldflags_str = join(' ', map { my $tmp = $_->to_string; $tmp } @$lib_infos);
  
  my $object_files = [map { my $tmp = $_->to_string; $tmp } @$object_file_infos];

  my $cbuilder_extra_linker_flags = "$ldflags_str $lib_ldflags_str";
  
  my @link_command = ($ld, '-o', $output_file, @$object_files, $cbuilder_extra_linker_flags);
  
  my $link_command = join(' ', @link_command);
  
  return $link_command;
}

# Methods
sub new {
  my $class = shift;
  
  my $self = {@_};

  bless $self, $class;
  
  unless (defined $self->object_file_infos) {
    $self->object_file_infos([]);
  }

  unless (defined $self->lib_infos) {
    $self->lib_infos([]);
  }

  unless (defined $self->ldflags) {
    $self->ldflags([]);
  }
  
  return $self;
}

1;

=head1 Name

SPVM::Builder::LinkInfo - Link Information

=head1 Description

C<SPVM::Builder::LinkInfo> is a link information. This infromation is used by the linker.

=head1 Field Methods

=head2 output_file

  my $output_file = $link_info->output_file;
  $link_info->output_file($output_file);

Get and set the object file that is compiled.

=head2 ld

  my $ld = $link_info->ld;
  $link_info->ld($ld);

Get and set the linker name.

=head2 ldflags

  my $ldflags = $object_file_info->ldflags;
  $object_file_info->ldflags($ldflags);

Get and set the linker flags.  The default value is C<[]>.

=head2 object_file_infos

  my $object_file_infos = $link_info->object_file_infos;
  $link_info->object_file_infos($object_file_infos);

Get and set the object file informations to be linked. Each object file is a L<SPVM::Builder::ObjectFileInfo> object.

=head2 lib_infos

  my $lib_infos = $link_info->lib_infos;
  $link_info->lib_infos($lib_infos);

Get and set the library informations to be linked. Each object file is a L<SPVM::Builder::LibInfo> object.

=head2 class_name

  my $class_name = $link_info->class_name;
  $link_info->class_name($class_name);

Get and set the class name.

=head2 config

  my $config = $link_info->config;
  $link_info->config($config);

Get and set the L<config|SPVM::Builder::Config> that is used to link the objects.

=head1 Class Methods

=head2 new

  my $link_info = SPVM::Builder::LinkInfo->new;

=head1 Instance Methods

=head2 new

  my $link_info = SPVM::Builder::LinkInfo->new;

Create a new C<SPVM::Builder::LinkInfo> object.

=head2 to_string

  my $string = $link_info->to_string;

Get the string information of the link information.

B<Examples:>

  cc -o dylib.so foo.o bar.o -shared -O2 -Llibdir -lz
