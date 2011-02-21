#!/usr/bin/perl -W

$file = $ARGV[0];
print "#line 1 \"$file\"\n";
$nbline = 0;

while ($l = <>) {
    $nbline++;
    if ($l =~ /MACRO_MAINZ/) {
	$nb = $nbline+1;
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDMACRO/;
	    $r = $r . $l;
	}
	foreach my $n (("D","MPQ","MPFR")) {
	    $rr = $r;
	    $rr =~ s/ZZZ/$n/g;
	    print "#line $nb \"$file\"\n";
	    print "$rr"
	}
	$nb = $nbline+1;
	print "#line $nb \"$file\"\n";
    }
    elsif ($l =~ /MACRO_ALLZ/) {
	$nb = $nbline+1;
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDMACRO/;
	    $r = $r . $l;
	}
	foreach my $n (("Il","Ill","MPZ","Rl","Rll","MPQ","D","Dl","MPFR")) {
	    $rr = $r;
	    $rr =~ s/ZZZ/$n/g;
	    print "#line $nb \"$file\"\n";
	    print "$rr"
	}
	$nb = $nbline+1;
	print "#line $nb \"$file\"\n";
    }
    elsif ($l =~ /MACRO_ALLX_MAINZ/) {
	$nb = $nbline+1;
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDMACRO/;
	    $r = $r . $l;
	}
	foreach my $m (("Il","Ill","MPZ","Rl","Rll","MPQ","D","Dl","MPFR")) {
	    foreach my $n (("D","MPQ","MPFR")) {
		$rr = $r;
		$rr =~ s/XXX/$m/g;
		$rr =~ s/ZZZ/$n/g;
		print "#line $nb \"$file\"\n";
		print "$rr"
	    }
	}
	$nb = $nbline+1;
	print "#line $nb \"$file\"\n";
    }
    elsif ($l =~ /MACRO_ALLX_ALLZ/) {
	$nb = $nbline+1;
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDMACRO/;
	    $r = $r . $l;
	}
	foreach my $m (("Il","Ill","MPZ","Rl","Rll","MPQ","D","Dl","MPFR")) {
	    foreach my $n (("Il","Ill","MPZ","Rl","Rll","MPQ","D","Dl","MPFR")) {
		$rr = $r;
		$rr =~ s/XXX/$m/g;
		$rr =~ s/ZZZ/$n/g;
		print "#line $nb \"$file\"\n";
		print "$rr"
	    }
	}
	$nb = $nbline+1;
	print "#line $nb \"$file\"\n";
    }
    elsif ($l =~ /MACRO_ALLX/) {
	$nb = $nbline+1;
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDMACRO/;
	    $r = $r . $l;
	}
	foreach my $n (("Il","Ill","MPZ","Rl","Rll","MPQ","D","Dl","MPFR")) {
	    $rr = $r;
	    $rr =~ s/XXX/$n/g;
	    print "#line $nb \"$file\"\n";
	    print "$rr"
	}
	$nb = $nbline+1;
	print "#line $nb \"$file\"\n";
    }
    elsif ($l =~ /MACRO_numIRyyy/) {
	$nb = $nbline+1;
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDMACRO/;
	    $r = $r . $l;
	}
	foreach my $n (("l","ll")) {
	    $rr = $r;
	    $rr =~ s/yyy/$n/g;
	    print "#line $nb \"$file\"\n";
	    print "$rr"
	}
	$nb = $nbline+1;
	print "#line $nb \"$file\"\n";
    }
    elsif ($l =~ /MACRO_numDyyy/) {
	$nb = $nbline+1;
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDMACRO/;
	    $r = $r . $l;
	}
	foreach my $n (("","l")) {
	    $rr = $r;
	    $rr =~ s/yyy/$n/g;
	    print "#line $nb \"$file\"\n";
	    print "$rr"
	}
	$nb = $nbline+1;
	print "#line $nb \"$file\"\n";
    }
    else { print "$l"; }
}
