use strict;
use warnings;
use utf8;
use FindBin;

use SPVM::Builder;

use lib "$FindBin::Bin/../default/lib";
use lib "$FindBin::Bin/lib";

use MyTest qw(compile_not_ok_file compile_not_ok);

use Test::More;

# Literal
{
  # Integer literal
  {
    # Integer literal decimal notation
    {
      compile_not_ok_file('CompileError::Literal::Integer::IntOutOfRange', qr/The numeric literal "8232624535311216194" is out of range of maximum and minimum values of int type/);
      {
        # Greater than int max value
        my $source = 'class Tmp { static method main : void () { 2147483648; } }';
        compile_not_ok($source, qr/The numeric literal "2147483648" is out of range of maximum and minimum values of int type/);
      }
      {
        # Less than int minimal value
        my $source = 'class Tmp { static method main : void () { -2147483649; } }';
        compile_not_ok($source, qr/The numeric literal "-2147483649" is out of range of maximum and minimum values of int type/);
      }
      {
        # Greater than long max value
        my $source = 'class Tmp { static method main : void () { 9223372036854775808L; } }';
        compile_not_ok($source, qr/The numeric literal "9223372036854775808L" is out of range of maximum and minimum values of long type/);
      }
      {
        # Less than long minimal value
        my $source = 'class Tmp { static method main : void () { -9223372036854775809L; } }';
        compile_not_ok($source, qr/The numeric literal "-9223372036854775809L" is out of range of maximum and minimum values of long type/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { _-123; } }';
        compile_not_ok($source, qr/Unexpected token "-123"/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { -_123; } }';
        compile_not_ok($source, qr/Unexpected token ";"/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { 123L_; } }';
        compile_not_ok($source, qr/Unexpected token "_"/);
      }
    }

    # Integer literal hexadecimal notation
    {
      {
        # Greater than int max value
        my $source = 'class Tmp { static method main : void () { 0x100000000; } }';
        compile_not_ok($source, qr/The numeric literal "0x100000000" is out of range of maximum and minimum values of int type at/);
      }
      {
        # Greater than long max value
        my $source = 'class Tmp { static method main : void () { 0x10000000000000000L; } }';
        compile_not_ok($source, qr/The numeric literal "0x10000000000000000L" is out of range of maximum and minimum values of long type at/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { _-0xFF; } }';
        compile_not_ok($source, qr/Unexpected token "-0xFF"/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { -0_xFF; } }';
        compile_not_ok($source, qr/Unexpected token "xFF"/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { 0xFFL_; } }';
        compile_not_ok($source, qr/Unexpected token "_"/);
      }
    }

    # Integer literal octal notation
    {
      {
        # Greater than int max value
        my $source = 'class Tmp { static method main : void () { 040000000000; } }';
        compile_not_ok($source, qr/The numeric literal "040000000000" is out of range of maximum and minimum values of int type at/);
      }
      {
        # Greater than long max value
        my $source = 'class Tmp { static method main : void () { 0x2000000000000000000000L; } }';
        compile_not_ok($source, qr/The numeric literal "0x2000000000000000000000L" is out of range of maximum and minimum values of long type at/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { _-077; } }';
        compile_not_ok($source, qr/Unexpected token "-077"/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { 0x77L_; } }';
        compile_not_ok($source, qr/Unexpected token "_"/);
      }
    }

    # Integer literal binary notation
    {
      {
        # Greater than int max value
        my $source = 'class Tmp { static method main : void () { 0b100000000000000000000000000000000; } }';
        compile_not_ok($source, qr/The numeric literal "0b100000000000000000000000000000000" is out of range of maximum and minimum values of int type at/);
      }
      {
        # Greater than long max value
        my $source = 'class Tmp { static method main : void () { 0b10000000000000000000000000000000000000000000000000000000000000000L; } }';
        compile_not_ok($source, qr/The numeric literal "0b10000000000000000000000000000000000000000000000000000000000000000L" is out of range of maximum and minimum values of long type at/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { _-0b11; } }';
        compile_not_ok($source, qr/Unexpected token "-0b11"/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { -0_b11; } }';
        compile_not_ok($source, qr/Unexpected token "b11"/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { 0b11L_; } }';
        compile_not_ok($source, qr/Unexpected token "_"/);
      }
    }
  }
  
  # Floating point literal
  {
    # Floating point literal decimal notation
    {
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { _-1.23; } }';
        compile_not_ok($source, qr/Unexpected token "-1.23"/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { -_1.23; } }';
        compile_not_ok($source, qr/Unexpected token "."/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { 1.23f_; } }';
        compile_not_ok($source, qr/Unexpected token "_"/);
      }
    }

    # Floating point literal dexadecimal notation
    {
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { _-0xFF.FF; } }';
        compile_not_ok($source, qr/Unexpected token "-0xFF.FF"/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { -0_xFF.FF; } }';
        compile_not_ok($source, qr/Unexpected token "xFF"/);
      }
      {
        # Invalid "_"
        my $source = 'class Tmp { static method main : void () { 0xFF.FFp5F_; } }';
        compile_not_ok($source, qr/Unexpected token "_"/);
      }
    }
  }

  # Caharater literal
  {
    compile_not_ok_file('CompileError::Literal::Character::InvalidCharacterLiteralEmpty', qr/The character literal can't be empty/);
    compile_not_ok_file('CompileError::Literal::Character::InvalidCharacterLiteral', qr/\Q"\A" is the invalid charater literal escape character/);
    compile_not_ok_file('CompileError::Literal::Character::NotEnd', qr/The character literal must ends with "'"/);
    compile_not_ok_file('CompileError::Literal::Character::InvalidHexAscii1', qr/\QOne or tow hexadecimal numbers must be follow by "\x" of the hexadecimal escape character/);
    compile_not_ok_file('CompileError::Literal::Character::InvalidHexAscii2', qr/The character literal must ends with "'"/);
    {
      my $source = q|class Tmp { static method main : void () { '\x{a' } }|;
      compile_not_ok($source, qr/The hexadecimal escape character is not closed by "}"/);
    }
    {
      my $source = q|class Tmp { static method main : void () { '\xaz' } }|;
      compile_not_ok($source, qr/The character literal must ends with "'"/);
    }
  }

  # String literal
  {
    {
      my $source = q|class Tmp { static method main : void () { "Foo \xg Bar" } }|;
      compile_not_ok($source, qr/\QOne or tow hexadecimal numbers must be follow by "\x" of the hexadecimal escape character/);
    }
    {
      my $source = q|class Tmp { static method main : void () { "Foo \x{a Bar" } }|;
      compile_not_ok($source, qr/The hexadecimal escape character is not closed by "}"/);
    }
    {
      my $source = q|class Tmp { static method main : void () { "\u" }|;
      compile_not_ok($source, qr/Invalid string literal escape character "\\u"/);
    }
    
    # Unicode escape character
    {
      {
        my $source = q|class Tmp { static method main : void () { "\N{U+}" }|;
        compile_not_ok($source, qr/One or more than one hexadecimal numbers must be followed by "\\N\{U\+" of the Unicode escape character/);
      }
      {
        my $source = q|class Tmp { static method main : void () { "\N{U+FFFFFFFFA}" }|;
        compile_not_ok($source, qr/Too big Unicode escape character/);
      }
      {
        my $source = q|class Tmp { static method main : void () { "\N{U+DFFF}" }|;
        compile_not_ok($source, qr/The code point of Unicode escape character must be a Unicode scalar value/);
      }
      {
        my $source = q|class Tmp { static method main : void () { "\N{U+DFFF}" }|;
        compile_not_ok($source, qr/The code point of Unicode escape character must be a Unicode scalar value/);
      }
      {
        my $source = q|class Tmp { static method main : void () { "\N{U+D800}" }|;
        compile_not_ok($source, qr/The code point of Unicode escape character must be a Unicode scalar value/);
      }
    }
    {
      my $source = q|class Tmp { static method main : void () { " } }|;
      compile_not_ok($source, qr/The string literal must be end with '"'/);
    }
    {
      my $source = q|class Tmp { static method main : void () { "$foo->{foo-" }|;
      compile_not_ok($source, qr/The getting field in a string literal must be closed with "}"/);
    }
    {
      my $source = q|class Tmp { static method main : void () { "$foo->[3-" }|;
      compile_not_ok($source, qr/The getting array element in a string literal must be closed with "]"/);
    }
    {
      my $source = q|class Tmp { static method main : void () { "$foo->bar" }|;
      compile_not_ok($source, qr/\QThe character after "->" in a string literal must be "[" or "{"/);
    }
  }
  
  # Block
  {
    {
      my $source = q|class Tmp { static method main : void () { {} }|;
      compile_not_ok($source, qr/Unexpected token "}"/);
    }
  }
}

done_testing;
