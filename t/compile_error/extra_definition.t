use strict;
use warnings;
use utf8;
use FindBin;

use SPVM::Builder;

use lib "$FindBin::Bin/../default/lib";
use lib "$FindBin::Bin/lib";

use MyTest qw(compile_not_ok_file compile_not_ok);

use Test::More;

# Class
{
  # Class descriptor
  {
    {
      my $source = 'class MyClass : pointer_t { has x : int; }';
      compile_not_ok($source, qr|The pointer class can't have fields|);
    }
  }
  
  # use
  {
    compile_not_ok_file('CompileError::Use::ImportMethodNotFound');
    compile_not_ok_file('CompileError::Use::AliasStartsLowerCase');
    compile_not_ok_file('CompileError::Use::AliasDuplication');
  }

  # Interface
  {
    compile_not_ok_file('CompileError::Interface::HaveBlock');
    compile_not_ok_file('CompileError::Interface::NativeMethod');
    compile_not_ok_file('CompileError::Interface::StaticMethod');
    compile_not_ok_file('CompileError::Interface::ArrayElementCantAssign');
    compile_not_ok_file('CompileError::Interface::NotHaveInterfaceMethod');
    compile_not_ok_file('CompileError::Interface::NoMethods');
    compile_not_ok_file('CompileError::Interface::MultiRequiredMethods');
    compile_not_ok_file('CompileError::Interface::HasImplNotFound');
  }
  # Class variable difinition
  {
    # Access control
    compile_not_ok_file('CompileError::ClassVar::Private');
    
    # Inheritance - extends syntax
    {
      {
        my $source = 'class MyClass extends Stringable {}';
        compile_not_ok($source);
      }
      {
        my $source = ['class MyClass extends PointerType {}', 'class PointerType : pointer_t {}'];
        compile_not_ok($source);
      }
      {
        my $source = 'class MyClass extends MyClass {}';
        compile_not_ok($source);
      }
      {
        my $source = ['class MyClass extends MyClass2 {}', 'class MyClass2 extends MyClass {}'];
        compile_not_ok($source);
      }
      {
        my $source = ['class MyClass extends MyParentClass { has x : int; }', 'class MyParentClass { has x : int; }'];
        compile_not_ok($source);
      }
    }
  }

  
  # Field definition
  {
    # mulnum_t
    {
      compile_not_ok_file('CompileError::MultiNumeric::FieldsZero');
      compile_not_ok_file('CompileError::MultiNumeric::Fields17');
      {
        my $source = 'class MyClass_2i : mulnum_t { static method foo : void () {} }';
        compile_not_ok($source);
      }
      {
        my $source = 'class MyClass_2i : mulnum_t { our $foo : int; }';
        compile_not_ok($source);
      }
      {
        my $source = 'class MyClass_2i : mulnum_t { }';
        compile_not_ok($source);
      }
    }
    # Access control
    {
      compile_not_ok_file('CompileError::Field::Private');
    }
    
    # Field name
    {
      compile_not_ok_file('CompileError::Field::HasFieldNameContainsUnderScoreTwice');
      compile_not_ok_file('CompileError::Field::HasFieldNameStartDigit');
      {
        my $source = 'class MyClass { has foo::x : int; }';
        compile_not_ok($source);
      }
    }
  }

  # Method definition
  {
    compile_not_ok_file('CompileError::Method::INIT');
    compile_not_ok_file('CompileError::Method::TooManyArguments');
    compile_not_ok_file('CompileError::Method::TooManyArgumentsMulnum');
    
    # Method name
    {
      compile_not_ok_file('CompileError::Method::MethodNameStartDigit');
      compile_not_ok_file('CompileError::Method::MethodNameContainsUnderScoreTwice');
      {
        my $source = 'class MyClass { static method foo::main : void () { } }';
        compile_not_ok($source);
      }
    }
    {
      my $source = 'class MyClass { static method main : void (); }';
      compile_not_ok($source);
    }
    # Destructor(DESTORY)
    {
      {
        my $source = 'class MyClass { static method DESTROY : void () { } }';
        compile_not_ok($source);
      }
      {
        my $source = 'class MyClass { method DESTROY : int () { } }';
        compile_not_ok($source);
      }
      {
        my $source = 'class MyClass { method DESTROY : void ($num : int) { } }';
        compile_not_ok($source);
      }
    }

    # Optional argument
    {
      {
        my $source = 'class MyClass { static method foo : void ($args0 = Int->new(1) : int) { } }';
        compile_not_ok($source);
      }
      {
        my $source = 'class MyClass { static method foo : void ($args0 = 0.3 : float) { } }';
        compile_not_ok($source);
      }
      {
        my $source = 'class MyClass { static method foo : void ($args0 = "abc" : object) { } }';
        compile_not_ok($source);
      }
      {
        my $source = 'class MyClass { static method foo : void ($args0 = undef : int*) { } }';
        compile_not_ok($source);
      }
      {
        my $source = 'class MyClass { use Complex_2d; static method foo : void ($args0 = 0 : Complex_2d) { } }';
        compile_not_ok($source);
      }
      {
        my $source = 'class MyClass { static method foo : void ($args0 = 0 : int, $args1 : int) { } }';
        compile_not_ok($source);
      }
    }
  }

  # Enumeration definition
  {
    compile_not_ok_file('CompileError::Enum::PrivateAccess');
    {
      my $source = q|class MyClass { interface_t enum { ONE } }|;
      compile_not_ok($source);
    }
    {
      my $source = q|class MyClass { public private enum { ONE } }|;
      compile_not_ok($source);
    }
  }
}

done_testing;
