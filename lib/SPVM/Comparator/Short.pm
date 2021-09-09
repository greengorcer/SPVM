package SPVM::Comparator::Short;

1;

=head1 NAME

SPVM::Comparator::Short - a callback interface for short comparation

=head1 SYNOPSYS
  
  use SPVM::Comparator::Short;
  
  my $comparator : SPVM::Comparator::Short = sub : int ($self : self, $a : short, $b : short); {
    return $a <=> $b;
  };
  
  my $result = $comparator->(3, 5);

=head1 DESCRIPTION

L<Comparator::Short|SPVM::Comparator::Short> is a callback interface to compare two short values.

=head1 CALLBACK METHOD INTERFACE

  sub : int ($self : self, $a : short, $b : short);

This method must receive two numbers and return 1 if $a is more than $b, -1 if $x is less than $b, 0 if $a equals $b in the implementation.

This method is planned to be implemented in other classes.
