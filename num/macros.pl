#!/usr/bin/perl -W

while ($l = <>) {
    if ($l =~ /MACROZ/) {
	$r = "";
	while ($l = <>) {
	    last if $l =~ /ENDMACRO/;
	    $r = $r . $l;
	}
	foreach my $n (("D","MPQ","MPFR")) {
	    $rr = $r;
	    $rr =~ s/ZZZ/$n/g;
	    print "$rr"
	}
    }
    else { print "$l"; }
}
