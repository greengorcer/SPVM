package SPVM::BlessedObject::Array;

use base 'SPVM::BlessedObject';

use overload '@{}' => sub {
  my ($array) = @_;
  
  my $elements = $array->to_elems;
  
  return $elements;
};

use SPVM::ExchangeAPI;

sub length {
  my $self = shift;
  
  my $env = $self->{env};
  
  SPVM::ExchangeAPI::array_length($SPVM::BUILDER, $env, $self);
}

sub to_elems {
  my $self = shift;
  
  my $env = $self->{env};
  
  SPVM::ExchangeAPI::array_to_elems($SPVM::BUILDER, $env, $self);
}

sub to_bin {
  my $self = shift;

  my $env = $self->{env};
  
  SPVM::ExchangeAPI::array_to_bin($SPVM::BUILDER, $env, $self);
}

sub set {
  my $self = shift;
  
  my $env = $self->{env};
  
  SPVM::ExchangeAPI::array_set($SPVM::BUILDER, $env, $self, @_);
}

sub get {
  my $self = shift;
  
  my $env = $self->{env};
  
  SPVM::ExchangeAPI::array_get($SPVM::BUILDER, $env, $self, @_);
}

1;

=head1 NAME

SPVM::BlessedObject::Array - Array based blessed object

=head2 DESCRIPTION

SPVM::BlessedObject::Array is array based blessed object.

This object contains SPVM array object.

=head1 SYNOPSYS

  # Get the value of a array element
  my $value = $spvm_nums->get(2);

  # Set the value of a array element
  $spvm_nums->set(2 => 5);
  
  # Convert SPVM array to Perl array reference
  my $nums = $spvm_nums->to_elems;

  # Convert SPVM array to Perl binary data
  my $binary = $spvm_nums->to_bin;
  
=head1 METHODS

=head2 get

  my $value = $spvm_nums->get(2);

Get the value of a array element.

=head2 set

  $spvm_nums->set(2 => 5);

Set the value of a array element

=head2 to_elems

  my $nums = $spvm_nums->to_elems;

Convert SPVM array to Perl array reference.

=head2 to_bin

  my $binary = $spvm_nums->to_bin;

Convert SPVM array to binary data.

Binary data is unpacked by C<unpack> function.

An exmaple when array is int array:

  my @nums = unpack 'l*', $binary;

=head1 OPERATORS

L<SPVM::BlessedObject::Array> overloads the following operators.

=head2 array dereference
  
Array dereference get the elements of the array.

  # Get elements
  my @elements = @$array;

This is the same as the following.

  my @elements = @{$array->to_elems};
