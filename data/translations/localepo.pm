#Modification to support obsolete strings added by granqvist@opera.com
#NOTE! Only parsing the PO supports obsolete strings, outputting obsolete strings is not implemented yet!!!

use strict;
use warnings;

package Locale::PO;

use Carp;

#use vars qw($VERSION @ISA @EXPORT @EXPORT_OK);
#use locale;

#require Exporter;
#require AutoLoader;

#@ISA     = qw(Exporter AutoLoader);
#@EXPORT  = qw();
our $VERSION = '0.17';

# Preloaded methods go here.

sub new {
    my $this    = shift;
    my %options = @_;
    my $class   = ref($this) || $this;
    my $self    = {};
    bless $self, $class;
    $self->msgid( $options{'-msgid'} ) if defined( $options{'-msgid'} );
    $self->msgid_plural( $options{'-msgid_plural'} )
      if defined( $options{'-msgid_plural'} );
    $self->msgstr( $options{'-msgstr'} ) if defined( $options{'-msgstr'} );
    $self->msgstr_n( $options{'-msgstr_n'} )
      if defined( $options{'-msgstr_n'} );
    $self->comment( $options{'-comment'} ) if defined( $options{'-comment'} );
    $self->fuzzy( $options{'-fuzzy'} )     if defined( $options{'-fuzzy'} );
    $self->automatic( $options{'-automatic'} )
      if defined( $options{'-automatic'} );
    $self->reference( $options{'-reference'} )
      if defined( $options{'-reference'} );
    $self->obsolete(1) if defined( $options{'-obsolete'} );
    $self->c_format(1) if defined( $options{'-c-format'} );
    $self->c_format(1) if defined( $options{'-c_format'} );
    $self->c_format(0) if defined( $options{'-no-c-format'} );
    $self->c_format(0) if defined( $options{'-no_c_format'} );
    $self->qt_format(1) if defined( $options{'-qt-format'} );
    $self->qt_format(1) if defined( $options{'-qt_format'} );
    $self->python_format(1) if defined( $options{'-python-format'} );
    $self->python_format(1) if defined( $options{'-python_format'} );
    return $self;
}

sub msgid {
    my $self = shift;
    @_ ? $self->{'msgid'} = $self->quote(shift) : $self->{'msgid'};
}

sub msgctxt {
    my $self = shift;
    @_ ? $self->{'msgctxt'} = $self->quote(shift) : $self->{'msgctxt'};
}

sub msgid_plural {
    my $self = shift;
    @_
      ? $self->{'msgid_plural'} =
        $self->quote(shift)
      : $self->{'msgid_plural'};
}

sub msgstr {
    my $self = shift;
    @_ ? $self->{'msgstr'} = $self->quote(shift) : $self->{'msgstr'};
}

sub msgstr_n {
    my $self = shift;
    if (@_) {
        my $hashref = shift;

        # check that we have a hashref.
        croak
          'Argument to msgstr_n must be a hashref: { n => "string n", ... }.'
          unless ref($hashref) eq 'HASH';

        # Check that the keys are all numbers.
        croak 'Keys to msgstr_n hashref must be numbers'
          if grep { m/\D/ } keys %$hashref;

        # Quote all the values in the hashref.
        $self->{'msgstr_n'}{$_} = $self->quote( $$hashref{$_} )
          for keys %$hashref;

    }

    return $self->{'msgstr_n'};
}

sub comment {
    my $self = shift;
    @_ ? $self->{'comment'} = shift: $self->{'comment'};
}

sub automatic {
    my $self = shift;
    @_ ? $self->{'automatic'} = shift: $self->{'automatic'};
}

sub reference {
    my $self = shift;
    @_ ? $self->{'reference'} = shift: $self->{'reference'};
}

sub fuzzy {
    my $self = shift;
    @_ ? $self->{'fuzzy'} = shift: $self->{'fuzzy'};
}

sub c_format {
    my $self = shift;
    @_ ? $self->{'c_format'} = shift: $self->{'c_format'};
}

sub qt_format {
    my $self = shift;
    @_ ? $self->{'qt_format'} = shift: $self->{'qt_format'};
}

sub python_format {
    my $self = shift;
    @_ ? $self->{'python_format'} = shift: $self->{'python_format'};
}

sub obsolete {
    my $self = shift;
    @_ ? $self->{'obsolete'} = shift: $self->{'obsolete'};
    #print "DEBUG: Obsotele set as " .  $self->{obsolete} . "\n";
}

sub php_format {
    my $self = shift;
    @_ ? $self->{'php_format'} = shift: $self->{'php_format'};
}

sub normalize_str {
    my $self     = shift;
    my $string   = shift;
    my $dequoted = $self->dequote($string);

    # This isn't quite perfect, but it's fast and easy
    if ( $dequoted =~ /(^|[^\\])(\\\\)*\\n./ ) {

        # Multiline
        my $output;
        my @lines;
        $output = '""' . "\n";
        @lines = split( /\\n/, $dequoted, -1 );
        my $lastline = pop @lines;    # special treatment for this one
        foreach (@lines) {
            $output .= $self->quote("$_\\n") . "\n";
        }
        $output .= $self->quote($lastline) . "\n" if $lastline ne "";
        return $output;
    }
    else {

        # Single line
        return "$string\n";
    }
}

sub dump {
    my $self = shift;
    my $dump;
    $dump = $self->dump_multi_comment( $self->comment, "# " )
      if ( $self->comment );
    $dump .= $self->dump_multi_comment( $self->automatic, "#. " )
      if ( $self->automatic );
    $dump .= $self->dump_multi_comment( $self->reference, "#: " )
      if ( $self->reference );
    my $flags = '';
    $flags .= ", fuzzy" if $self->fuzzy;
    $flags .= ", c-format"
      if ( defined( $self->c_format )
        and $self->c_format );
    $flags .= ", no-c-format"
      if ( defined( $self->c_format )
        and !$self->c_format );
    $flags .= ", qt-format"
      if ( defined( $self->qt_format )
        and $self->qt_format );
    $flags .= ", python-format"
      if ( defined( $self->python_format )
        and $self->python_format );
    $flags .= ", php-format"
      if ( defined( $self->php_format )
        and $self->php_format );
    $dump .= "#$flags\n" if length $flags;
    $dump .= "msgid " . $self->normalize_str( $self->msgid );
    $dump .= "msgid_plural " . $self->normalize_str( $self->msgid_plural )
      if $self->msgid_plural;

    $dump .= "msgstr " . $self->normalize_str( $self->msgstr ) if $self->msgstr;

    if ( my $msgstr_n = $self->msgstr_n ) {
        $dump .= "msgstr[$_] " . $self->normalize_str( $$msgstr_n{$_} )
          for sort { $a <=> $b } keys %$msgstr_n;
    }

    $dump .= "\n";
    return $dump;
}

sub dump_multi_comment {
    my $self    = shift;
    my $comment = shift;
    my $leader  = shift;
    my $chopped = $leader;
    chop($chopped);
    my $result = $leader . $comment;
    $result =~ s/\n/\n$leader/g;
    $result =~ s/^$leader$/$chopped/gm;
    $result .= "\n";
    return $result;
}

# Quote a string properly
sub quote {
    my $self   = shift;
    my $string = shift;
    $string =~ s/"/\\"/g;
    return "\"$string\"";
}

sub dequote {
    my $self   = shift;
    my $string = shift;
    $string =~ s/^"(.*)"/$1/;
    $string =~ s/\\"/"/g;
    return $string;
}

sub save_file_fromarray {
    my $self = shift;
    $self->save_file( @_, 0 );
}

sub save_file_fromhash {
    my $self = shift;
    $self->save_file( @_, 1 );
}

sub save_file {
    my $self    = shift;
    my $file    = shift;
    my $entries = shift;
    my $ashash  = shift;
    open( OUT, ">$file" ) or return undef;
    if ($ashash) {
        foreach ( sort keys %$entries ) {
            print OUT $entries->{$_}->dump;
        }
    }
    else {
        foreach (@$entries) {
            print OUT $_->dump;
        }
    }
    close OUT;
}

sub load_file_asarray {
    my $self = shift;
    $self->load_file( $_[0], 0 );
}

sub load_file_ashash {
    my $self = shift;
    $self->load_file( $_[0], 1 );
}

sub load_file {
    my $self   = shift;
    my $file   = shift;
    my $ashash = shift;
    my ( @entries, %entries );
    my $po;
    my %buffer;
    my $last_buffer;
    open( IN, "<$file" ) or return undef;

    while (<IN>) {
        chop;
        if (/^$/) {

            # Empty line. End of an entry.

            if ( defined($po) ) {

                $po->msgid( $buffer{msgid} ) if defined $buffer{msgid};
                $po->msgctxt( $buffer{msgctxt} ) if defined $buffer{msgctxt};
                $po->msgid_plural( $buffer{msgid_plural} )
                  if defined $buffer{msgid_plural};
                $po->msgstr( $buffer{msgstr} )     if defined $buffer{msgstr};
                $po->msgstr_n( $buffer{msgstr_n} ) if defined $buffer{msgstr_n};

                if ($ashash) {
                    my $key = $po->msgid;
                    if ( defined( $entries{$key} ) ) {

                        # Prefer translated ones.
                        $entries{ $po->msgid } = $po
                          if $entries{$key}->msgstr !~ /\w/;
                    }
                    else {

                        # No previous entry
                        $entries{ $po->msgid } = $po;
                    }
                }
                else {
                    push( @entries, $po );
                }

                undef $po;
                undef $last_buffer;
                %buffer = ();
            }
        }
        elsif ( /^#\s+(.*)/ or /^#()$/ ) {

            # Translator comments
            $po = new Locale::PO unless defined($po);
            if ( defined( $po->comment ) ) {
                $po->comment( $po->comment . "\n$1" );
            }
            else {
                $po->comment($1);
            }
        }
        elsif (/^#\.\s+(.*)/) {

            # Automatic comments
            $po = new Locale::PO unless defined($po);
            if ( defined( $po->automatic ) ) {
                $po->automatic( $po->automatic . "\n$1" );
            }
            else {
                $po->automatic($1);
            }
        }
        elsif (/^#:\s+(.*)/) {

            # reference
            $po = new Locale::PO unless defined($po);
            if ( defined( $po->reference ) ) {
                $po->reference( $po->reference . "\n$1" );
            }
            else {
                $po->reference($1);
            }
        }
        elsif (/^#,\s+(.*)/) {

            # flags
            my $flags = $1;
            $po = new Locale::PO unless defined($po);
            $po->fuzzy(1)      if $flags =~ /fuzzy/i;
            $po->c_format(1)   if $flags =~ /c-format/i;
            $po->c_format(0)   if $flags =~ /no-c-format/i;
            $po->qt_format(1)   if $flags =~ /qt-format/i;
            $po->python_format(1)   if $flags =~ /python-format/i;
            $po->php_format(1) if $flags =~ /php-format/i;
        }
        elsif (/^msgctxt\s+(.*)/) {
            $po = new Locale::PO unless defined($po);
            $buffer{msgctxt} = $self->dequote($1);
            $last_buffer = \$buffer{msgctxt};
            $po->obsolete(0);
        }
        elsif (/^#\~ msgctxt\s+(.*)/) {
            $po = new Locale::PO unless defined($po);
            $buffer{msgctxt} = $self->dequote($1);
            $last_buffer = \$buffer{msgctxt};
            $po->obsolete(1);
        }
        elsif (/^#\| msgctxt\s+(.*)/) {
            $po = new Locale::PO unless defined($po);
            $buffer{msgctxt} = $self->dequote($1);
            $last_buffer = \$buffer{msgctxt};
            $po->obsolete(1);
            # We should only mark it fuzzy if the fuzzy mark itself is still there, so commenting out below
            #$po->fuzzy(1)
        }
        
        elsif (/^msgid\s+(.*)/) {
            $po = new Locale::PO unless defined($po);
            $buffer{msgid} = $self->dequote($1);
            $last_buffer = \$buffer{msgid};
            $po->obsolete(0);
        }
        elsif (/^msgid_plural\s+(.*)/) {
            $po = new Locale::PO unless defined($po);
            $buffer{msgid_plural} = $self->dequote($1);
            $last_buffer .= \$buffer{msgid_plural};
            $po->obsolete(0);
        }
        elsif (/^msgstr\s+(.*)/) {

            # translated string
            $buffer{msgstr} = $self->dequote($1);
            $last_buffer = \$buffer{msgstr};
        }
        elsif (/^msgstr\[(\d+)\]\s+(.*)/) {

            # translated string
            $buffer{msgstr_n}{$1} = $self->dequote($2);
            $last_buffer = \$buffer{msgstr_n}{$1};
        }
        elsif (/^"/) {

            # contined string
            $$last_buffer .= $self->dequote($_);
        } elsif (/^#~ msgid\s+(.*)/) {
            $po = new Locale::PO unless defined($po);
            $buffer{msgid} = $self->dequote($1);
            $last_buffer = \$buffer{msgid};
            $po->obsolete(1);
        } elsif (/^#\| msgid\s+(.*)/) {
            $po = new Locale::PO unless defined($po);
            $buffer{msgid} = $self->dequote($1);
            $last_buffer = \$buffer{msgid};
            $po->obsolete(1);
            # We should only mark it fuzzy if the fuzzy mark itself is still there, so commenting out below
            #$po->fuzzy(1)
        }  elsif (/^#~ msgstr\s+(.*)/) {

            # translated string
            $buffer{msgstr} = $self->dequote($1);
            $last_buffer = \$buffer{msgstr};
        }
        else {
            warn "Strange line in $file: $_\n";
        }
    }
    if ( defined($po) ) {
        $po->msgid( $buffer{msgid} ) if defined $buffer{msgid};
        $po->msgid_plural( $buffer{msgid_plural} )
          if defined $buffer{msgid_plural};
        $po->msgstr( $buffer{msgstr} )     if defined $buffer{msgstr};
        $po->msgstr_n( $buffer{msgstr_n} ) if defined $buffer{msgstr_n};

        $entries{ $po->msgid } = $po if $ashash;
        push( @entries, $po ) unless $ashash;
    }
    close IN;
    return ( $ashash ? \%entries : \@entries );
}

# Autoload methods go after =cut, and are processed by the autosplit program.

1;
__END__

# Below is the stub of documentation for your module. You better edit it!

=head1 NAME

Locale::PO - Perl module for manipulating .po entries from GNU gettext

=head1 SYNOPSIS

  use Locale::PO;

  $po = new Locale::PO([-option=>value,...])
  [$string =] $po->msgid([new string]);
  [$string =] $po->msgstr([new string]);
  [$string =] $po->comment([new string]);
  [$string =] $po->automatic([new string]);
  [$string =] $po->reference([new string]);
  [$value =] $po->fuzzy([value]);
  [$value =] $po->c_format([value]);
  print $po->dump;

  $quoted_string = $po->quote($string);
  $string = $po->dequote($quoted_string);

  $aref = Locale::PO->load_file_asarray(<filename>);
  $href = Locale::PO->load_file_ashash(<filename>);
  Locale::PO->save_file_fromarray(<filename>,$aref);
  Locale::PO->save_file_fromhash(<filename>,$href);

=head1 DESCRIPTION

This module simplifies management of GNU gettext .po files and is an
alternative to using emacs po-mode. It provides an object-oriented
interface in which each entry in a .po file is a Locale::PO object.

=head1 METHODS

=over 4

=item new

Create a new Locale::PO object to represent a po entry.
You can optionally set the attributes of the entry by passing 
a list/hash of the form:

 -option=>value, -option=>value, etc.

Where options are msgid, msgstr, comment, automatic, reference,
fuzzy, and c-format. See accessor methods below.

To generate a po file header, add an entry with an empty
msgid, like this:

   $po = new Locale::PO(-msgid=>'', -msgstr=>
	"Project-Id-Version: PACKAGE VERSION\\n" .
	"PO-Revision-Date: YEAR-MO-DA HO:MI +ZONE\\n" .
	"Last-Translator: FULL NAME <EMAIL@ADDRESS>\\n" .
	"Language-Team: LANGUAGE <LL@li.org>\\n" .
	"MIME-Version: 1.0\\n" .
	"Content-Type: text/plain; charset=CHARSET\\n" .
	"Content-Transfer-Encoding: ENCODING\\n");

=item msgid

Set or get the untranslated string from the object.

=item msgid_plural

Set or get the untranslated plural string from the object.

=item msgstr

Set or get the translated string from the object.

=item msgstr_n

Get or set the translations if there are purals involved. Takes and
returns a hashref where the keys are the 'N' case and the values are
the strings. eg:

    $po->msgstr_n(
        {
            0 => 'found %d plural translations',
            1 => 'found %d singular translation',
        }
    );

=item comment

Set or get translator comments from the object.

=item automatic

Set or get automatic comments from the object (inserted by 
emacs po-mode or xgettext).

=item reference

Set or get reference marking comments from the object (inserted
by emacs po-mode or gettext).

=item fuzzy

Set or get the fuzzy flag on the object ("check this translation").
When setting, use 1 to turn on fuzzy, and 0 to turn it off.

=item c_format

Set or get the c-format or no-c-format flag on the object.
This can take 3 values: 1 implies c-format, 0 implies no-c-format,
and blank or undefined implies neither.

=item qt-format

Set or get the qt-format flag on the object.

=item python-format

Set or get the python-format flag on the object.

=item dump

Returns the entry as a string, suitable for output to a po file.

=item quote

Applies po quotation rules to a string, and returns the quoted
string. The quoted string will have all existing double-quote
characters escaped by backslashes, and will be enclosed in double
quotes.

=item dequote

Returns a quoted po string to its natural form.

=item load_file_asarray

Given the filename of a po-file, reads the file and returns a
reference to a list of Locale::PO objects corresponding to the contents of
the file, in the same order.

=item load_file_ashash

Given the filename of a po-file, reads the file and returns a
reference to a hash of Locale::PO objects corresponding to the contents of
the file. The hash keys are the untranslated strings, so this is a cheap
way to remove duplicates. The method will prefer to keep entries that
have been translated.

=item save_file_fromarray

Given a filename and a reference to a list of Locale::PO objects,
saves those objects to the file, creating a po-file.

=item save_file_fromhash

Given a filename and a reference to a hash of Locale::PO objects,
saves those objects to the file, creating a po-file. The entries
are sorted alphabetically by untranslated string.

=back

=head1 AUTHOR

Alan Schwartz, alansz@pennmush.org

=head1 BUGS

If you load_file then save_file, the output file may have slight
cosmetic differences from the input file (an extra blank line here or there).

=head1 SEE ALSO

xgettext(1).

=cut
