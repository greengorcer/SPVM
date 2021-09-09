package SPVM::Short;

1;

=head1 NAME

SPVM::Short - Short in SPVM | Short object

=head1 SYNOPSYS
  
  use Short;
  
  my $short_object = Short->new(5);
  my $short_value = $short_object->value;

=head1 DESCRIPTION

L<Short|SPVM::Short> object stores a C<short> value.

This object is immutable and its value cannot be changed.

L<Short|SPVM::Short> is automatically loaded just after the program starts.

=head1 STATIC METHODS

=head2 new

  sub new : Short ($value : short)

Create a new L<Short|SPVM::Short> object with a C<short> value.

=head1 INSTANCE METHODS

=head2 value

  sub value : short ($self : self)

Get a C<short> value.
