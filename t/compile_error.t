use strict;
use warnings;
use utf8;
use Data::Dumper;
use File::Basename 'basename';
use FindBin;
use File::Temp ();

use SPVM::Builder;

use Test::More;

my $file = 't/' . basename $0;

use FindBin;
use lib "$FindBin::Bin/default/lib";

sub compile_not_ok_file {
  my ($class_name, $error_message_re, $options) = @_;
  
  unless ($options) {
    $options = {};
  }
  
  my $module_dir = $options->{module_dir};
  
  my (undef, $caller_file, $caller_line) = caller;
  
  my $file;
  if (defined $options->{file}) {
    $file = $options->{file};
  }
  else {
    $file = $caller_file;
  }

  my $line;
  if (defined $options->{line}) {
    $line = $options->{line};
  }
  else {
    $line = $caller_line;
  }
  
  my $builder = SPVM::Builder->new;
  unshift @{$builder->module_dirs}, $module_dir;
  
  my $success = $builder->compile_spvm($class_name, $file, $line);
  ok($success == 0);
  unless ($success == 0) {
    warn "  at $file line $line\n";
  }
  my $error_messages = $builder->get_error_messages;
  my $first_error_message = $error_messages->[0];
  if ($error_message_re) {
    like($first_error_message, $error_message_re);
  }
  print_error_messages($builder);
}

sub compile_not_ok {
  my ($source, $error_message_re) = @_;
  
  my (undef, $file, $line) = caller;
  
  my $builder = SPVM::Builder->new;
  
  my $class_name;
  if ($source =~ /\bclass\s+([\w+:]+)\s*/) {
    $class_name = $1;
  }
  unless (defined $class_name) {
    die "Can't find class name in the source";
  }
  
  my $tmp_module_dir = File::Temp->newdir;
  
  my $module_file = "$tmp_module_dir/$class_name.spvm";
  
  open my $module_fh, '>', $module_file
    or die "Can't open $module_file: $!";
  
  print $module_fh $source;
  
  close $module_fh;
  
  compile_not_ok_file($class_name, $error_message_re, {module_dir => "$tmp_module_dir", file => $file, line => $line});
}

sub print_error_messages {
  my ($builder) = @_;
  
  my $error_messages = $builder->get_error_messages;
  
  for my $error_message (@$error_messages) {
    print STDERR "[CompileError]$error_message\n";
  }
}

# SPVM compile error
{
  my $command = "$^X -Mblib $FindBin::Bin/compile_error.pl 2>&1";
  my $output = `$command 2>&1`;
  like($output, qr/CompileError/);
}

# SPVM dist compile error
{
  my $command = "$^X -Mblib $FindBin::Bin/compile_error_dist.pl 2>&1";
  my $output = `$command 2>&1`;
  like($output, qr/CompileError/);
}


# Class
{
  compile_not_ok_file('TestCase::CompileError::Class::NotClosed');
  compile_not_ok_file('TestCase::CompileError::Class::ClassNameDifferntFromModuleName');
  compile_not_ok_file('TestCase::CompileError::Class::classPartNameStartWithLowerCase');
  compile_not_ok_file('foo');
  compile_not_ok_file('4foo');
}

# Pakcage name must start SPVM::
{
  compile_not_ok_file('ClassNameCompileError');
}

# use
{
  compile_not_ok_file('TestCase::CompileError::Use::ImportMethodNotFound');
  compile_not_ok_file('TestCase::CompileError::Use::AliasStartsLowerCase');
  compile_not_ok_file('TestCase::CompileError::Use::AliasDuplication');
}

# Logical operator
{
  compile_not_ok_file('TestCase::CompileError::LogicalOperator::AndNotExpression');
  compile_not_ok_file('TestCase::CompileError::LogicalOperator::OrNotExpression');
  compile_not_ok_file('TestCase::CompileError::LogicalOperator::NotNotExpression');
}

# oarray
{
  compile_not_ok_file('TestCase::CompileError::OArray::AssignNumeric');
  compile_not_ok_file('TestCase::CompileError::OArray::AssignNumericArray');
}

# Literal
{
  # Caharater
  {
    compile_not_ok_file('TestCase::CompileError::Literal::Character::InvalidCharacterLiteralEmpty');
    compile_not_ok_file('TestCase::CompileError::Literal::Character::InvalidCharacterLiteral');
    compile_not_ok_file('TestCase::CompileError::Literal::Character::NotEnd');
    compile_not_ok_file('TestCase::CompileError::Literal::Character::InvalidHexAscii1');
    compile_not_ok_file('TestCase::CompileError::Literal::Character::InvalidHexAscii2');
  }
  # Integer
  compile_not_ok_file('TestCase::CompileError::Literal::Integer::IntOutOfRange');
}

# Lexcarl Variable
{
  compile_not_ok_file('TestCase::CompileError::LexVar::LexVarNameStartDigit');
  compile_not_ok_file('TestCase::CompileError::LexVar::LexVarNameInvalidColon');
  compile_not_ok_file('TestCase::CompileError::LexVar::LexVarNameEndColon2');
  compile_not_ok_file('TestCase::CompileError::LexVar::LexVarNameContainsUnderScoreTwice');
  compile_not_ok_file('TestCase::CompileError::LexVar::LexVarNameColon2Twice');
}

# Class Variable
{
  compile_not_ok_file('TestCase::CompileError::ClassVar::Private');
  compile_not_ok_file('TestCase::CompileError::ClassVar::OurClassVarNameStartDigit');
  compile_not_ok_file('TestCase::CompileError::ClassVar::OurClassVarNameInvalidColon');
  compile_not_ok_file('TestCase::CompileError::ClassVar::OurClassVarNameEndColon2');
  compile_not_ok_file('TestCase::CompileError::ClassVar::OurClassVarNameContainsUnderScoreTwice');
  compile_not_ok_file('TestCase::CompileError::ClassVar::OurClassVarNameColon2Twice');
}

# Method
{
  compile_not_ok_file('TestCase::CompileError::Method::Begin');
  compile_not_ok_file('TestCase::CompileError::Method::MethodNameStartDigit');
  compile_not_ok_file('TestCase::CompileError::Method::MethodNameContainsUnderScoreTwice');
  compile_not_ok_file('TestCase::CompileError::Method::TooManyArguments', qr/Too many arguments/i);
  compile_not_ok_file('TestCase::CompileError::Method::TooManyArgumentsMulnum'. qr/Too many arguments/i);
}

# Field
{
  compile_not_ok_file('TestCase::CompileError::Field::Private');
  compile_not_ok_file('TestCase::CompileError::Field::HasFieldNameContainsUnderScoreTwice');
  compile_not_ok_file('TestCase::CompileError::Field::HasFieldNameStartDigit');
}


# Enum
{
  compile_not_ok_file('TestCase::CompileError::Enum::PrivateAccess');
}

# Call sub
{
  {
    
    compile_not_ok_file('TestCase::CompileError::Switch::NoLastBreak');
    
    
  }
}

# Value
{
  compile_not_ok_file('TestCase::CompileError::Value::FieldsZero');
  compile_not_ok_file('TestCase::CompileError::Value::Fields17');
}

# Call method
{
  compile_not_ok_file('TestCase::CompileError::CallMethod::NotFound');
  compile_not_ok_file('TestCase::CompileError::CallMethod::ClassNotFound');
}

# Assign
{
  compile_not_ok_file('TestCase::CompileError::Assign::StringToBytArray');
  compile_not_ok_file('TestCase::CompileError::Assign::DifferentObject');
  compile_not_ok_file('TestCase::CompileError::Assign::ConstToNoConst');
  compile_not_ok_file('TestCase::CompileError::Assign::NotNumericToNumeric');
  compile_not_ok_file('TestCase::CompileError::Assign::NarrwoingConvertionIntToByte');
  compile_not_ok_file('TestCase::CompileError::Assign::NarrwoingConvertionIntToShort');
  compile_not_ok_file('TestCase::CompileError::Assign::NarrwoingConvertionLongToByte');
  compile_not_ok_file('TestCase::CompileError::Assign::NarrwoingConvertionLongToInt');
  compile_not_ok_file('TestCase::CompileError::Assign::NarrwoingConvertionLongToShort');
  compile_not_ok_file('TestCase::CompileError::Assign::UnboxingDifferentType');
  compile_not_ok_file('TestCase::CompileError::Assign::BoxingDifferentType');
}

# Remainder
{
  compile_not_ok_file('TestCase::CompileError::Remainder::RightIsNotIntegral');
  compile_not_ok_file('TestCase::CompileError::Remainder::LeftIsNotIntegral');
}

# String
{
  compile_not_ok_file('TestCase::CompileError::String::CharacterAssign');
  compile_not_ok_file('TestCase::CompileError::String::AssignNonMutableToMutable');
}

# Comparison operator
{
  compile_not_ok_file('TestCase::CompileError::ComparisonOperator::GtNotNumeric');
  compile_not_ok_file('TestCase::CompileError::ComparisonOperator::GeNotNumeric');
  compile_not_ok_file('TestCase::CompileError::ComparisonOperator::LtNotNumeric');
  compile_not_ok_file('TestCase::CompileError::ComparisonOperator::LeNotNumeric');
}

# Bool
{
  compile_not_ok_file('TestCase::CompileError::Bool::NotNumericObject');
}

# isa
{
  compile_not_ok_file('TestCase::CompileError::Isa::LeftIsNotObject');
}

{
  compile_not_ok_file('TestCase::CompileError::InvalidType');
}

{
  compile_not_ok_file('TestCase::CompileError::TypeCantBeDetectedUndef');
}

{
  compile_not_ok_file('TestCase::CompileError::TypeCantBeDetectedUndefDefault');
}

{
  compile_not_ok_file('TestCase::CompileError::New::Private');
}

# Convert
{
  compile_not_ok_file('TestCase::CompileError::Convert::ConvertFromByteArrayToString');
  compile_not_ok_file('TestCase::CompileError::Convert::ConvertFromStringToByteArray');
  compile_not_ok_file('TestCase::CompileError::Convert::ConvertToRef');
  compile_not_ok_file('TestCase::CompileError::Convert::ConvertFromRef');
  compile_not_ok_file('TestCase::CompileError::Convert::ConvertFromValueType');
}

# Concat
{
  compile_not_ok_file('TestCase::CompileError::Concat::RightIsNotString');
  compile_not_ok_file('TestCase::CompileError::Concat::LeftIsNotString');
}

# Type
{
  compile_not_ok_file('TestCase::CompileError::Type::MutableNoStringCaseStringArray');
  compile_not_ok_file('TestCase::CompileError::Type::MutableNoStringCaseInt');
  compile_not_ok_file('TestCase::CompileError::Type::MultiDimensionalAnyObject', qr/Multi dimensional array of any object/i);
}

# Divide - Compile Error
{
  compile_not_ok_file('TestCase::CompileError::Divide::LeftIsNotNumeric');
  compile_not_ok_file('TestCase::CompileError::Divide::RightIsNotNumeric');
  compile_not_ok_file('TestCase::CompileError::Divide::DivuiLeftIsNotInt');
  compile_not_ok_file('TestCase::CompileError::Divide::DivuiRightIsNotInt');
  compile_not_ok_file('TestCase::CompileError::Divide::DivulLeftIsNotLong');
  compile_not_ok_file('TestCase::CompileError::Divide::DivulRightIsNotLong');
}


# Remainder - Compile Error
{
  compile_not_ok_file('TestCase::CompileError::Remainder::LeftIsNotIntegral', qr/left.+%.+integral/);
  compile_not_ok_file('TestCase::CompileError::Remainder::RightIsNotIntegral', qr/right.+%.+integral/);
  compile_not_ok_file('TestCase::CompileError::Remainder::RemuiLeftIsNotInt', qr/left.+remui.+int/);
  compile_not_ok_file('TestCase::CompileError::Remainder::RemuiRightIsNotInt', qr/right.+remui.+int/);
  compile_not_ok_file('TestCase::CompileError::Remainder::RemulLeftIsNotLong', qr/left.+remul.+long/);
  compile_not_ok_file('TestCase::CompileError::Remainder::RemulRightIsNotLong', qr/right.+remul.+long/);
}

# Interface
{
  compile_not_ok_file('TestCase::CompileError::Interface::HaveBlock', qr/interface.+block/i);
  compile_not_ok_file('TestCase::CompileError::Interface::NativeMethod', qr/interface.+native/i);
  compile_not_ok_file('TestCase::CompileError::Interface::StaticMethod', qr/interface.+instance/i);
  compile_not_ok_file('TestCase::CompileError::Interface::ArrayElementCantAssign', qr/List to Stringable/i);
  compile_not_ok_file('TestCase::CompileError::Interface::NotHaveInterfaceMethod', qr/TestCase::CompileError::Interface::NotHaveInterfaceMethod.+to_string.*string\(self\).+interface.+Stringable/i);
  compile_not_ok_file('TestCase::CompileError::Interface::NoMethods', qr/one required method/i);
  compile_not_ok_file('TestCase::CompileError::Interface::MultiRequiredMethods', qr/multiple required method/i);
  compile_not_ok_file('TestCase::CompileError::Interface::HasImplNotFound', qr/interface.+TestCase::Pointable.+the method declaration.+not_found/i);
}

# Syntax
{
  compile_not_ok_file('TestCase::CompileError::Syntax::LineNumber', qr/our.*\b8:3\b/i);
}

# Type comment
{
  compile_not_ok_file('TestCase::CompileError::TypeComment::NotExistType', qr/NotExists::XXXX/);
}

# Weaken
{
  compile_not_ok_file('TestCase::CompileError::Weaken::IsweakFieldNotObjectType', qr/isweak.*object/);
  compile_not_ok_file('TestCase::CompileError::Weaken::UnweakenFieldNotObjectType', qr/unweaken.*object/);
  compile_not_ok_file('TestCase::CompileError::Weaken::WeakenFieldNotObjectType', qr/weaken.*object/);
}

# Symbol name
{
  # A symbol name can't conatain "__"
  {
    my $source = 'class Foo__Bar { static method main : void () { } }';
    compile_not_ok($source, qr/\QThe symbol name "Foo__Bar" can't constain "__"/);
  }

  # A symbol name can't end with "::"
  {
    my $source = 'class Tmp { use Int as Foo::; static method main : void () { } }';
    compile_not_ok($source, qr/\QThe symbol name "Foo::" can't end with "::"/);
  }

  # A symbol name can't contains "::::".
  {
    my $source = 'class Tmp { use Int as Foo::::Bar; static method main : void () { } }';
    compile_not_ok($source, qr/\QThe symbol name "Foo::::Bar" can't contains "::::"/);
  }
}

# Fat comma
{
  # The string literal of the left operand of the fat camma can't contains "::".
  {
    my $source = 'class Tmp { static method main : void () { {Foo::Bar => 1}; } }';
    compile_not_ok($source, qr/\QThe string literal "Foo::Bar" of the left operand of the fat camma can't contains "::"/);
  }
}

done_testing;
