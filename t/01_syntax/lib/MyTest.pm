use strict;
use warnings;
use Exporter;
use File::Temp();
use Test::More;
use Carp 'confess';
use File::Path 'mkpath';
use File::Basename 'dirname';

our @EXPORT_OK = qw(compile_not_ok_file compile_not_ok);

sub compile_not_ok {
  my ($sources, $error_message_re) = @_;
  
  if (defined $error_message_re) {
    unless (ref $error_message_re) {
      my $quotemeta = quotemeta $error_message_re;
      $error_message_re = qr/$quotemeta/;
    }
  }
  
  unless (ref $sources eq 'ARRAY') {
    $sources = [$sources];
  }
  
  my (undef, $file, $line) = caller;
  
  my $builder = SPVM::Builder->new;
  
  my $tmp_module_dir = File::Temp->newdir;
    
  my $first_class_name;
  for my $source (@$sources) {
    my $class_name;
    if ($source =~ /\bclass\s+([\w+:]+)\s*/) {
      $class_name = $1;
    }
    unless (defined $class_name) {
      die "Can't find class name in the source";
    }
    
    unless (defined $first_class_name) {
      $first_class_name = $class_name;
    }
    
    my $module_file = "$tmp_module_dir/$class_name.spvm";
    $module_file =~ s|::|/|g;
    
    mkpath dirname $module_file;
    open my $module_fh, '>', $module_file
      or confess "Can't open file \"$module_file\":$!";
    
    print $module_fh $source;
    close $module_fh;
  }
  
  compile_not_ok_file($first_class_name, $error_message_re, {module_dir => "$tmp_module_dir", file => $file, line => $line});
}

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
  if (defined $module_dir) {
    unshift @{$builder->module_dirs}, $module_dir;
  }

  my $compiler = SPVM::Builder::Compiler->new(
    module_dirs => $builder->module_dirs
  );
  
  my $success = $compiler->compile($class_name, $file, $line);
  ok(!$success);
  my $error_messages = $compiler->get_error_messages;
  my $first_error_message = $error_messages->[0];
  my $message_ok;
  if ($error_message_re) {
    $message_ok = like($first_error_message, $error_message_re);
  }
  
  if ($success || ($error_message_re && !$message_ok)) {
    warn "  at $file line $line\n";
  }
}

sub compile_ok {
  my ($sources) = @_;
  
  unless (ref $sources eq 'ARRAY') {
    $sources = [$sources];
  }
  
  my (undef, $file, $line) = caller;
  
  my $builder = SPVM::Builder->new;
  
  my $tmp_module_dir = File::Temp->newdir;
    
  my $first_class_name;
  for my $source (@$sources) {
    my $class_name;
    if ($source =~ /\bclass\s+([\w+:]+)\s*/) {
      $class_name = $1;
    }
    unless (defined $class_name) {
      die "Can't find class name in the source";
    }
    
    unless (defined $first_class_name) {
      $first_class_name = $class_name;
    }
    
    my $module_file = "$tmp_module_dir/$class_name.spvm";
    $module_file =~ s|::|/|g;
    
    mkpath dirname $module_file;
    open my $module_fh, '>', $module_file
      or confess "Can't open file \"$module_file\":$!";
    
    print $module_fh $source;
    close $module_fh;
  }
  
  compile_ok_file($first_class_name, {module_dir => "$tmp_module_dir", file => $file, line => $line});
}

sub compile_ok_file {
  my ($class_name, $options) = @_;
  
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
  if (defined $module_dir) {
    unshift @{$builder->module_dirs}, $module_dir;
  }
  
  my $compiler = SPVM::Builder::Compiler->new(
    module_dirs => $builder->module_dirs
  );
  my $success = $compiler->compile($class_name, $file, $line);
  ok($success);
  
  if (!$success) {
    warn "  at $file line $line\n";
    
    my $error_messages = $compiler->get_error_messages;
    my $first_error_message = $error_messages->[0];
    warn "[Compile Error]$first_error_message";
  }
}
