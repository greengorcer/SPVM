package SPVM::StringUtil;

1;

=head1 NAME

SPVM::StringUtil - Variouse utilities

=head1 SYNOPSYS
  
  use SPVM::StringUtil (chomp_lf, chomp_crlf);

  # Cut a newline LF
  {
    my $ret = chomp_lf("abc\n");
  }
  
  # Cut a newline CR LF
  {
    my $ret = chomp_crlf("abc\r\n");
  }
  
  # Copy a string
  {
    my $str = "abc";
    my $str_copy = copy_str($str);
  }

  # Search substr
  {
    my $found_offset = index("pppabcde", "bcd", 2);
  }

  # split a string by the specific separator
  my $str = "foo,bar,baz";
  my $splited_strs = split(",", $str);

=head1 DESCRIPTION

String Utilities

=head1 STATIC METHODS

=head2 split

  sub split : string[] ($sep : string, $string : string)

Split a string by the specific separator.

=head2 chomp_lf

  sub chomp_lf : string ($string : string)

Copy the string and remove the newline "\n" from the end of string and return it.

=head2 chomp_crlf

  sub chomp_crlf : string ($string : string)

Copy the string and remove the newline "\r\n" from the end of string and return it.

=head2 copy_str

  sub copy_str : string ($string : string)

Copy a string.

If the array is undefined, a exception occurs.

=head2 index

  sub index : int ($str : string, $substr : string, $posision : int)

index function searches for one string within another.
It returns the position of the first occurrence of $substr in $str at or after $position. If $position is omitted, starts
searching from the beginning of the string. $position before the
beginning of the string or after its end is treated as if it were
the beginning or the end, respectively. $position and the return
value are based at zero. If the substring is not found, "index"
returns -1.
            
=head2 isalnum

  sub isalnum : int ($char : int)

If character is alphanumeric('A'-'Z', 'a'-'z', '0'-'9'), return 1. If not, return 0.

=head2 isalpha

  sub isalpha : int ($char : int)

If character is alphabetic('A'-'Z', 'a'-'z'), return 1. If not, return 0.

=head2 isblank

  sub isblank : int ($char : int)

If character is blank(' ', '\t'), return 1. If not, return 0.

=head2 iscntrl

  sub iscntrl : int ($char : int)

If character is a control character(0x00-0x1F, 0x7F), return 1. If not, return 0.

=head2 isdigit

  sub isdigit : int ($char : int)

If character is decimal digit ('0'～'9'), return 1. If not, return 0.

=head2 isgraph

  sub isgraph : int ($char : int)

If character has graphical representation(0x21-0x7E), return 1. If not, return 0.

=head2 islower

  sub islower : int ($char : int)

If character is lowercase letter('a'-'z'), return 1. If not, return 0.

=head2 isprint

  sub isprint : int ($char : int)

If character is printable(0x20-0x7E), return 1. If not, return 0.

=head2 ispunct

  sub ispunct : int ($char : int)

If character is a punctuation character(0x21-0x2f, 0x3a-0x40, 0x5b-0x60, 0x7b-0x7e), return 1. If not, return 0.

=head2 isspace

  sub isspace : int ($char : int)

If character is a white-space(' ',  '\t', '\n', '\v', '\f', '\r'), return 1. If not, return 0.

=head2 isupper

  sub isupper : int ($char : int)

If character is uppercase letter('A'-'Z'), return 1. If not, return 0.

=head2 isxdigit

  sub isxdigit : int ($char : int)

If character is hexadecimal digit('0'-'9', 'A'-'F', 'a'-'f'), return 1. If not, return 0.

=head2 tolower

  sub tolower : int ($char : int)

Convert uppercase letter('A'-'Z') to lowercase. If the character is not uppercase letter, return the character.

=head2 toupper

  sub toupper : int ($char : int)

Convert lowercase letter('a'-'z') to lowercase. If the character is not uppercase letter, return the character.

=head2 is_perl_space

  sub is_perl_space : int ($char : int)

If character is Perl space character(' ', '\r', '\n', '\t', '\f'), return 1. If not, return 0.

=head2 is_perl_word

  sub is_perl_word : int ($char : int)

If character is Perl word character('a'-'z', 'A'-'Z', '_', '0'-'9'), return 1. If not, return 0.

=head2 join

  sub join : string ($sep : string, $strings : string[])
  
Join a string array with separater and return it.

If separater is undef, a exception occurs.

If string array is undef, a exception occurs.

=head2 lc

  sub lc : string($str : string)

Convert uppercase string to lowercase string.

=head2 lcfirst

  sub lcfirst : string($str : string)

Convert first chracter of string from uppercase to lowercase.

=head2 replace

  sub replace : string ($str : string, $substr : string, $replace : string, $start_offset : int, $found_offset_ref : int&)

Replace the sub string in the string with a replace string and return the result string.

You can specify a byte offset of the string.

You can get the found byte offset by int reference.

  my $str = "abcde";
  my $substr = "bcd";
  my $replace = "AB";
  my $found_offset = 0;
  my $result_str = replace($str, $substr, $replace, 0, \$found_offset);

=head2 replace_all

  sub replace_all : string ($str : string, $substr : string, $replace : string)

Replace all the sub string in the string with a replace string and return the result string.

  my $str = "foo bar foo bar foo";
  my $substr = "bar";
  my $replace = "AB";
  my $result_str = replace_all($str, $substr, $replace);

=head2 strtoi

  sub strtoi : int ($string : string, $digit : int);

Convert the string to a int value with a digit(2, 8, 10, 16).

Format is [' ' or '\t' or '\n' or '\v' or '\f' or '\r'][+ or -][0][x][one more than 0-9]. Internal of [] is optional.

If convertion fails, a exception occuer.

  my $string = "-2147483648";
  my $num = strtoi($string, 10);

=head2 strtol

  sub strtol : long ($string : string, $digit : int);

Convert the string to long value with digit(2, 8, 10, 16).

Format is [' ' or '\t' or '\n' or '\v' or '\f' or '\r'][+ or -][0][x][zero more than 0-9]. Internal of [] is optional.

If the convertion fails, a exception occuer.

  my $string = "-9223372036854775808";
  my $num = strtol($string, 10);

=head2 strtof

  sub strtof : float ($string : string);

Convert the string to float value.

Format is [' ' or '\t' or '\n' or '\v' or '\f' or '\r'][+ or -][zero more than 0-9][.][zero more than 0-9][e or E[+ or -]zero more than 0-9]. Internal of [] is optional.

If the convertion fails, a exception occuer.

  my $string = "1.25";
  my $num = strtof($string);

=head2 strtod

  sub strtod : double ($string : string);

Convert the string to float value.

Format is [' ' or '\t' or '\n' or '\v' or '\f' or '\r'][+ or -][zero more than 0-9][.][zero more than 0-9][e or E[+ or -]zero more than 0-9]. Internal of [] is optional.

If the convertion fails, a exception occuer.

  my $string = "1.25";
  my $num = strtod($string);

=head2 uc

  sub uc : string($str : string)

Convert a lowercase string to a uppercase string.

If the string is undef, a exception occur.

=head2 ucfirst

  sub ucfirst : string($str : string)

Convert the first character of a string to a uppercase character.

If the string is undef, a exception occur.

=head2 rindex

  sub rindex : int ($str : string, $substr : string, $offset : int)

Same as "index" function except that the search is the last of the string.

=head2 contains

  sub contains : int ($str : string, $substr : string)

If the string contains the sub string, return 1. If not, return 0.
