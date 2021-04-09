package SPVM::ShortList;

1;

=head1 NAME

SPVM::ShortList - Continuous dynamic short array

=head1 SYNOPSYS
  
  use SPVM::ShortList;
  
  # Create a short list with array length
  my $short_list = SPVM::ShortList->new_len(10);

  # Create a short list with array
  my $short_list = SPVM::ShortList->new([(short)1, 2, 3]);
  
  # Get list length
  my $length = $short_list->length;
  
  # Push short value
  $short_list->push((short)3);

  # Pop short value.
  my $short_value = $short_list->pop;

  # Unshift short value.
  $short_list->unshift((short)3);
  
  # Shift short value.
  my $short_value = $short_list->shift;
  
  # Set short value.
  $short_list->set(2, (short)3);

  # Get short value.
  my $short_value = $short_list->get(2);

  # Insert byte value
  $byte_list->insert(1, 3);

  # Remove byte value
  my $byte_value = $byte_list->remove(1);

  # Convert SPVM::ShortList to short array.
  my $short_array = $short_list->to_array;

=head1 DESCRIPTION

L<SPVM::ShortList> is continuous dynamic short array.

=head1 STATIC METHODS

=head2 new

    sub new : SPVM::ShortList ($array : short[])

Create a new L<SPVM::ShortList> object with specific C<short> array.

=head2 new_len

    sub new_len : SPVM::ShortList ($length : int)

Create a new L<SPVM::ShortList> object with array length.

=head1 INSTANCE METHODS

=head2 length
  
  sub length : int ()

Get list length.

=head2 push
  
  sub push : void ($self : self, $value : short)

Appending the value to the end of list.

=head2 pop

  sub pop : short ($self : self)

Pops and returns the last value of the list, shortening the array by one element
If there are no elements in the list, exception occur.

=head2 unshift

  sub unshift : void ($self : self, $value : short)

Appending the value to the top of list.

=head2 shift

  sub shift : short ($self : self)

Shifts the first value of the list off and returns it, shortening
the array by 1 and moving everything down.
If there are no elements in the list, exception occur.

=head2 set

  sub set : void ($self : self, $index : int, $value : short)

Set the value with index.

=head2 get

  sub get : short ($self : self, $index : int)

Get the value with index.

=head2 insert

  sub insert : void ($self : self, $index : int, $value : short)

Insert a element to the specific index.

=head2 remove

  sub remove : short ($self : self, $index : int)

Remove and return the element which is specified by the index.

=head2 to_array

  sub to_array : short[] ($self : self)

Convert L<SPVM::ShortList> to short array.

=head2 resize

  sub resize : void ($self : self, $new_length : int)

Resize list.

New length must be more than or equals to 0, otherwise a exception occur.
