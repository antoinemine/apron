#!/usr/bin/perl -W

my @MainNum = ("D","MPQ","MPFR");
my @AllNum = ("Il","Ill","MPZ","Rl","Rll","MPQ","D","Dl","MPFR");

$file = $ARGV[0];
$nbline = 0;

open(my $tmp, "+>", undef);
while ($l = <>) {
    $nbline++;
    if ($l =~ /^\h*MACRO_/){
	my $r = parse_macro(0);
	print $tmp $r;
    }
    elsif ($l =~ /^PERLEVAL\h+([^\n]*)$/){
	eval $1;
    }
    else {
	print $tmp "$l";
    }
}
seek($tmp,0,0);
while ($l = <$tmp>){ print "$l"; }

sub parse_macroblock {
    my $depth = $_[0];
    my $r = "";
    while ($l = <>){
	$nbline++;
	last if ($l =~ /^\h*ENDMACRO/);
	if ($l =~ /^\h*MACRO_/){
	    my $rr = parse_macro($depth+1);
	    $r = $r . $rr;
	}
	else {
	    $r = $r . $l;
	}
    }
    return $r
}

sub parse_macro {
    my $depth = $_[0];
    my $result = "";
    my $count = 0;
    $l =~ /^(\h*)MACRO_(\w+)/;
    my $tab = $1;
    my $cmd = $2;
    if (! $cmd){
	die "macros.pl: in file $file, line $nbline: MACRO not followed by a macro name\n";
    }
    if ($cmd eq "FOREACH"){
	if ($l =~ /^\h*MACRO_FOREACH\h+(\w+)\h+(\("[^"]*"(?:,"[^"]*")*\)|@\w+)/){
	    my $var = $1;
	    my @list = eval $2;
	    my $nb = $nbline+1;
	    my $r = parse_macroblock($depth);
	    foreach my $n (@list) {
		my $rr = $r;
		$rr =~ s/$var/$n/g;
		if ($count == 0){
		    $result = $result . "\n";
		}
		else {
		    $result = $result . "$tab#line $nb \"$file\"\n";
		}
		$result = $result . $rr;
		$count = $count+1;
	    }
	    $result = $result . "\n";
	}
	else {
	    die "macros.pl: in file $file, line $nbline: wrong syntax for MACRO_FOREACH\n";
	}
    }
    elsif ($cmd eq "SWITCH"){
	if ($l =~ /^\h*MACRO_SWITCH\(([^\)]*)\)\h+(\w+)\h*(.*)$/){
	    my $discr = $1;
	    my $var = $2;
	    my $last = $3;
	    my $aftercase = "\n$tab";
	    my $nb = $nbline+1;
	    my $r = parse_macroblock($depth);
	    $result = $result . "$tab" . "switch($discr){\n";
	    foreach my $n (("D","MPQ","MPFR")) {
		my $rr = $r;
		$rr =~ s/$var/$n/g;
		$result = $result . "$tab" . "case AP_SCALAR_$n:$last\n";
		$result = $result . "$tab  #line $nb \"$file\"\n";
		$result = $result . "$rr$tab" . "break;\n";
	    }
	    $result = $result . "$tab" . "default:\n$tab  abort();\n";
	    $result = $result . "$tab}\n";
	    $nb = $nbline+1;
	    $result = $result . "$tab#line $nb \"$file\"\n";
	}
	else {
	    die "macros.pl: in file $file, line $nbline: wrong syntax for MACRO_SWITCH\n";
	}
    }
    else {
	die "macros.pl: in file $file, line $nbline: MACRO not followed by a known macro name\n";
    }
    return $result;
}
