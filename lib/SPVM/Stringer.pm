package SPVM::Stringer;

1;

=head1 NAME

SPVM::Stringer - Stringer in SPVM | a callback type to stringify a object

=head1 SYNOPSYS
  
  use Stringer;
  
  my $stringer : Stringer = method : string ($object : object) {
    my $point = (Point)$object;
    my $x = $point->x;
    my $y = $point->y;
    
    my $string = "($x, $y)";
    
    return $string;
  };
  
  my $point = Point->new(1, 2);
  my $string = $stringer->($point);

=head1 DESCRIPTION

L<Stringer|SPVM::Stringer> is a callback type to stringify a object.

=head1 CALLBACK METHOD

  method : string ($object : object)

This method receives a object and return the string expression.

This method will be implemented as a anon method.
