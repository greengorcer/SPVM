package SPVM::DoubleList;

1;

=head1 Name

SPVM::DoubleList - Dynamic double Array

=head1 Usage
  
  use DoubleList;
  
  # Create a double list
  my $list = DoubleList->new;
  my $list = DoubleList->new([(double)1, 2, 3]);
  
  # Create a double list with array length
  my $list = DoubleList->new_len(10);
  
  # Get list length
  my $length = $list->length;
  
  # Push value
  $list->push((double)3);
  
  # Pop value.
  my $element = $list->pop;
  
  # Unshift value.
  $list->unshift((double)3);
  
  # Shift value.
  my $element = $list->shift;
  
  # Set value.
  $list->set(2, (double)3);
  
  # Get value.
  my $element = $list->get(2);
  
  # Insert value
  $list->insert(1, 3);
  
  # Remove value
  my $element = $list->remove(1);
  
  # Convert list to array.
  my $array = $list->to_array;
  
=head1 Description

C<DoubleList> is a dynamic C<double> array.

=head1 Enumerations

  enum {
    DEFAULT_CAPACITY = 4,
  }

=head2 DEFAULT_CAPACITY

The default capacity. The value is C<4>.

=head1 Fields

=head2 capacity

  has capacity : ro int;

The capacity. This is the length of the internally reserved elements to extend the length of the list.

=head2 length

  has length : ro int;

The length of the list.

=head2 values

  has values : ro double[];

The elements of the list. This is the internally used array, but it can be manipulated directly.

  my $elements = $list->values;
  $valeus->[0] = 5;

=head1 Class Methods

=head2 new

  static method new : DoubleList ($array = undef : double[], $capacity = -1 : int);

Create a new C<DoubleList> object using L</"new_len">.

The passed length to L</"new_len"> is the length of the array. If the array is C<undef>, the length is C<0>.

The elements of the array are copied to the values of the the created array.

B<Examples:>

  my $list = DoubleList->new;
  my $list = DoubleList->new([(double)1, 2, 3]);

=head2 new_len

  static method new_len : DoubleList ($length : int, $capacity = -1 : int);

Creates a new C<DoubleList> object with the C<$length> and the C<$capacity>.

If the C<$capacity> is less than C<0>, the capacity is set to the value of L</"DEFAULT_CAPACITY">.

If the C<$length> is greater than the C<$capacity>, the C<$capacity> is set to the C<$length>.

Exceptions:

The C<$length> must be greater than or equal to C<0>.

=head1 Instance Methods

=head2 get

  method get : double ($index : int);

Gets the element of the position of the C<$index>.

Exceptions:

The C<$index> must be greater than or equal to 0.

The C<$index> must be less than the length of the C<$list>.

=head2 insert

  method insert : void ($index : int, $element : double);

Inserts an C<$element> to the position of the C<$index>.

Exceptions:

The C<$index> must be greater than or equal to C<0>.

The C<$index> must be less than or equal to the length of the C<$list>.

=head2 pop

  method pop : double ();

Removes the last element and return it.

Exceptions:

The length of the C<$list> must be greater than C<0>.

=head2 push
  
  method push : void ($element : double);

Adds an C<$element> after the end of the list.

=head2 remove

  method remove : double ($index : int);

Removes the element at the position of the C<$index> and return it.

Exceptions:

The C<$index> must be greater than or equal to C<0>.

The C<$index> must be less than the length of the C<$list>.

=head2 replace

  method replace : void ($offset : int, $remove_length : int, $replace : double[]);

Replaces the elements of the range specified by the C<$offset> and the C<$lenght> with the C<$replace> array.

Exceptions:

The C<$offset> must be greater than or equal to C<0>.

The C<$remove_length> must be greater than or equal to C<0>.

The C<$offset> + the C<$removing> lenght must be less than or equal to the length of the C<$list>.

=head2 reserve

  method reserve : void ($new_capacity : int);

Reserves the elements with the C<$new_capacity>.

If the C<$new_capacity> is greater than the capacity of the list, the capacity of the list is extended to the C<$new_capacity>.

Note that L</"values"> is replaced with the new values and the values of the original list are copied to the new values in the above case.

Exceptions:

The C<$new_capacity> must be greater than or equal to C<0>.

=head2 resize

  method resize : void ($new_length : int);

Resize the list with the C<$new_length>.

Exceptions:

The C<$new_length> must be greater than or equal to C<0>.

=head2 set

  method set : void ($index : int, $element : double);

Sets the C<$element> at the position of the C<$index>.

Exceptions:

The C<$index> must be greater than or equal to C<0>.

The C<$index> must be less than the length of the C<$list>.

=head2 set_array

  method set_array : void ($array : double[]);

Sets an C<$array>. Each element of the C<$array> is copied to the element of the list.

Exceptions:

The C<$array> must be defined.

The length of the C<$array> must be the C<$same> as the length of the C<$list>.

=head2 shift

  method shift : double ();

Removes the first element and return it.

Exceptions:

The length of the C<$list> must be greater than C<0>.

=head2 to_array

  method to_array : double[] ();

Converts the list to an array.

=head2 unshift

  method unshift : void ($element : double);

Inserts an C<$element> at the beginning of the list.
