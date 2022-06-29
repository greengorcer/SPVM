package SPVM::Builder::LibInfo;

use strict;
use warnings;
use Config;
use Carp 'confess';
use File::Basename 'dirname';

use overload bool => sub {1}, '""' => sub { shift->to_string }, fallback => 1;

# Field Methods
sub name {
  my $self = shift;
  if (@_) {
    $self->{name} = $_[0];
    return $self;
  }
  else {
    return $self->{name};
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

sub static {
  my $self = shift;
  if (@_) {
    $self->{static} = $_[0];
    return $self;
  }
  else {
    return $self->{static};
  }
}

sub abs {
  my $self = shift;
  if (@_) {
    $self->{abs} = $_[0];
    return $self;
  }
  else {
    return $self->{abs};
  }
}

# Class Methods
sub new {
  my $class = shift;
  
  my $self = {@_};

  bless $self, $class;
  
  return $self;
}

# Instance Methods
sub to_string {
  my ($self) = @_;
  
  my $string;
  if ($self->abs) {
    $string = $self->name;
  }
  else {
    $string = $self->file;
  }
  
  return $string;
}

1;

=head1 NAME

SPVM::Builder::LibInfo - Library Information

=head1 DESCRIPTION

C<SPVM::Builder::LibInfo> is a library file information.

=head1 Field Methods

The list of field methods.

=head2 name

  my $name = $lib_info->name;
  $lib_info->name($name);

Get and set the library name. C<z>, C<png>, etc.

=head2 file

  my $file = $lib_info->file;
  $lib_info->file($file);

Get and set the library file. C</path/libz.so>, C</path/libpng.a>, etc.

This field has the meaning when L</"abs"> is set to a true value.

=head2 static

  my $static = $lib_info->static;
  $lib_info->static($static);

Get and set the flag if the library is linked statically such as C<libfoo.a>.

The default is a false value.

=head2 abs

  my $abs = $lib_info->abs;
  $lib_info->abs($abs);

Get and set the flag if the library is linked as ablosulte path. The default is a false value.

=head2 config

  my $config = $lib_info->config;
  $lib_info->config($config);

Get and set the L<config|SPVM::Builder::Config> that is used to link this library.

=head1 Class Methods

The list of class methods.

=head2 new

  my $lib_info = SPVM::Builder::LibInfo->new;

=head1 Instance Methods

=head2 to_string

  my $lib = $lib_info->to_string;

If L</"abs"> is false value, get the library flag such as C<-lfoo> from L<"/name">.

If L</"abs"> is true value, get the library file path from L<"/file">.

=head1 Operators

L<SPVM::BlessedObject::String> overloads the following operators.

=head2 bool

  my $bool = !!$lib_info;
  
Always true.

=head2 stringify

  my $lib_name = "$lib_info";
  
Alias for L</"to_string">.
