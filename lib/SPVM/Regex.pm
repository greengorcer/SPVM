package SPVM::Regex;

1;

=head1 NAME

SPVM::Regex - Regex in SPVM | Regular expression

=head1 SYNOPSYS
  
  use Regex;
  
  # Pattern match
  {
    my $re = Regex->new("ab*c");
    my $target = "zabcz";
    my $match = $re->match($target, 0);
  }

  # Pattern match - UTF-8
  {
    my $re = Regex->new("あ+");
    my $target = "いあああい";
    my $match = $re->match($target, 0);
  }

  # Pattern match - Character class and the nagation
  {
    my $re = Regex->new("[A-Z]+[^A-Z]+");
    my $target = "ABCzab";
    my $match = $re->match($target, 0);
  }

  # Pattern match with captures
  {
    my $re = Regex->new("^(\w+) (\w+) (\w+)$");
    my $target = "abc1 abc2 abc3";
    my $match = $re->match($target, 0);
    
    if ($match) {
      my $cap1 = $re->captures->[0];
      my $cap2 = $re->captures->[1];
      my $cpa3 = $re->captures->[2];
    }
  }
  
  # Replace
  {
    my $re = Regex->new("abc");
    my $target = "ppzabcz";
    
    # "ppzABCz"
    my $result = $re->replace($target, 0, "ABC");
    
    my $replace_count = $re->replace_count;
  }

  # Replace with a callback and capture
  {
    my $re = Regex->new("a(bc)");
    my $target = "ppzabcz";
    
    # "ppzABbcCz"
    my $result = $re->replace_cb($target, 0, method : string ($re : Regex) {
      return "AB" . $re->captures->[0] . "C";
    });
  }

  # Replace all
  {
    my $re = Regex->new("abc");
    my $target = "ppzabczabcz";
    
    # "ppzABCzABCz"
    my $result = $re->replace_all($target, 0, "ABC");
  }

  # Replace all with a callback and capture
  {
    my $re = Regex->new("a(bc)");
    my $target = "ppzabczabcz";
    
    # "ppzABCbcPQRSzABCbcPQRSz"
    my $result = $re->replace_all_cb($target, 0, method : string ($re : Regex) {
      return "ABC" . $re->captures->[0] . "PQRS";
    });
  }
  
=head1 DESCRIPTION

L<Regex|SPVM::Regex> provides regular expression functions.

=head1 REGULAR EXPRESSION SYNTAX

L<Regex|SPVM::Regex> provides the methodset of Perl regular expression. The target string and regex string is interpretted as UTF-8 string.
  
  # Quantifier
  +     more than or equals to 1 repeats
  *     more than or equals to 0 repeats
  ?     0 or 1 repeats
  {m,n} repeats between m and n
  
  # Regular expression character
  ^    first of string
  $    last of string
  .    all character except "\n"
  
  #    Default mode     ASCII mode
  \d   Not supported    [0-9]
  \D   Not supported    not \d
  \s   Not supported    " ", "\t", "\f", "\r", "\n"
  \S   Not supported    not \s
  \w   Not supported    [a-zA-Z0-9_]
  \W   Not supported    not \w
  
  # Character class and the negatiton
  [a-z0-9]
  [^a-z0-9]
  
  # Capture
  (foo)

B<Regex Options:>

  s    single line mode
  a    ascii mode

Regex options is used by C<new_with_options> method.

  my $re = Regex->new("^ab+c", "sa");

B<Limitations:>

L<Regex|SPVM::Regex> do not support the same set of characters after a quantifier.
      
  # A exception occurs
  Regex->new("a*a");
  Regex->new("a?a");
  Regex->new("a+a");
  Regex->new("a{1,3}a")
      
If 0 width quantifir is between two same set of characters after a quantifier, it is invalid.
      
  # A exception occurs
  Regex->new("\d+\D*\d+");
  Regex->new("\d+\D?\d+");

=head1 CLASS METHODS

=head2 new

  my $re = Regex->new("^ab+c");

Create a new L<Regex|SPVM::Regex> object and compile the regex.

=head2 new_with_options

  my $re = Regex->new("^ab+c", "s");

Create a new L<Regex|SPVM::Regex> object and compile the regex with the options.

=head1 INSTANCE METHODS

=head2 captures

  static method captures : string[] ()

Get the strings captured by "match" method.

=head2  match_start

  static method match_start : int ()

Get the start byte offset of the string matched by "match" method method.

=head2 match_length

  static method match_length : int ()

Get the byte length of the string matched by "match" method method.

=head2 replace_count

  static method replace_count : int ();

Get the replace count of the strings replaced by "replace" or "replace_all" method.

=head2 match

  method match : int ($target : string, $target_offset : int)

Execute pattern matching to the specific string and the start byte offset of the string.

If the pattern match succeeds, 1 is returned, otherwise 0 is returned.

You can get captured strings using "captures" method,
and get the byte offset of the matched whole string using "match_start" method,
and get the length of the matched whole string using "match_length" method.

=head2 replace

  method replace  : string ($target : string, $target_offset : int, $replace : string)

Replace the target string specified with the start byte offset with replace string.

=head2 replace_cb

  method replace_cb  : string ($target : string, $target_offset : int, $replace_cb : Regex::Replacer)

Replace the target string specified with the start byte offset with replace callback. The callback must have "replace_to" method defined in L<Regex::Replacer|SPVM::Regex::Replacer>.

=head2 replace_all

  method replace_all  : string ($target : string, $target_offset : int, $replace : string)

Replace all of the target strings specified with the start byte offset with replace string.

=head2 replace_all_cb

  method replace_all_cb  : string ($target : string, $target_offset : int, $replace_cb : Regex::Replacer)

Replace all of the target strings specified with the start byte offset with replace callback. The callback must have "replace_to" method defined in L<Regex::Replacer|SPVM::Regex::Replacer>.
