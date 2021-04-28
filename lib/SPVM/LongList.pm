package SPVM::LongList;

1;

=head1 NAME

SPVM::LongList - Continuous dynamic long array

=head1 SYNOPSYS
  
  use SPVM::LongList;
  
  # Create a long list
  my $long_list = SPVM::LongList->new_len(10);

  # Create a long list with array
  my $long_list = SPVM::LongList->new([(long)1, 2, 3]);
  
  # Get list length
  my $length = $long_list->length;
  
  # Push long value
  $long_list->push(3L);

  # Pop long value.
  my $long_value = $long_list->pop;

  # Unshift long value.
  $long_list->unshift(3L);
  
  # Shift long value.
  my $long_value = $long_list->shift;
  
  # Set long value.
  $long_list->set(2, 3L);

  # Get long value.
  my $long_value = $long_list->get(2);

  # Insert long value
  $long_list->insert(1, 3);

  # Remove long value
  my $long_value = $long_list->remove(1);

  # Convert SPVM::LongList to long array.
  my $long_array = $long_list->to_array;

=head1 DESCRIPTION

L<SPVM::LongList> is continuous dynamic long array.

=head1 STATIC METHODS

=head2 new

    sub new : SPVM::LongList ($array : long[])

Create a new L<SPVM::LongList> object with specific C<long> array.

=head2 new_len

    sub new_len : SPVM::LongList ($length : int)

Create a new L<SPVM::LongList> object with array length.

=head1 INSTANCE METHODS

=head2 length
  
  sub length : int ()

Get list length.

=head2 push
  
  sub push : void ($self : self, $value : long)

Appending the value to the end of list.

=head2 pop

  sub pop : long ($self : self)

Pops and returns the last value of the list, shortening the array by one element
If there are no elements in the list, exception occur.

=head2 unshift

  sub unshift : void ($self : self, $value : long)

Appending the value to the top of list.

=head2 shift

  sub shift : long ($self : self)

Shifts the first value of the list off and returns it, shortening
the array by 1 and moving everything down.
If there are no elements in the list, exception occur.

=head2 set

  sub set : void ($self : self, $index : int, $value : long)

Set the value with index.

=head2 get

  sub get : long ($self : self, $index : int)

Get the value with index.

=head2 insert

  sub insert : void ($self : self, $index : int, $value : long)

Insert a element to the specific index.

=head2 remove

  sub remove : long ($self : self, $index : int)

Remove and return the element which is specified by the index.
  
=head2 to_array

  sub to_array : long[] ($self : self)

Convert L<SPVM::LongList> to long array.

=head2 resize

  sub resize : void ($self : self, $new_length : int)

Resize list.

New length must be more than or equals to 0, otherwise a exception occur.
