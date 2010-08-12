#!/usr/bin/perl -W

$file = $ARGV[0];
print "#line 1 \"$file\"\n";
$nbline = 0;

while ($l = <>) {
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
    }
    elif ($l =~ /MACRO_ALLZ/) {
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
    }
    else { print "$l"; }
}
