#!/usr/bin/perl -W

$file = $ARGV[0];
$nbline = 0;
while ($l = <>) {
    $nbline++;
    if ($l =~ /SWITCHX\s*(\([^)]*\))/) {
	$nb = $nbline;
	print "  switch $1 {\n";
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDSWITCH/;
	    $r = $r . $l;
	}
	foreach my $n (("D","MPQ","MPFR")) {
	    $rr = $r;
	    $rr =~ s/XXX/$n/g;
	    print "#line $nb \"$file\"\n";
	    print "  case AP_SCALAR_$n: {\n$rr    }\n    break;\n";
	}
	print "  default: abort();\n";
	print "  }\n";
	$nb = $nbline+1;
	print "#line $nb \"$file\"\n";
    }
    elsif ($l =~ /SWITCHZ\s*(\([^)]*\))/) {
	$nb = $nbline;
	print "  switch $1 {\n";
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDSWITCH/;
	    $r = $r . $l;
	}
	foreach my $n (("D","MPQ","MPFR")) {
	    $rr = $r;
	    $rr =~ s/ZZZ/$n/g;
	    print "#line $nb \"$file\"\n";
	    print "  case AP_SCALAR_$n: {\n$rr    }\n    break;\n";
	}
	print "  default: abort();\n";
	print "  }\n";
	$nb = $nbline+1;
	print "#line $nb \"$file\"\n";
    }
    elsif ($l =~ /MACRO_MAINZ/) {
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
    elsif ($l =~ /MACRO_ALLX_MAINZ/) {
	$nb = $nbline+1;
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDMACRO/;
	    $r = $r . $l;
	}
	foreach my $m (("MPZ","MPQ","D","MPFR")) {
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
    elsif ($l =~ /MACRO_ALLZ/) {
	$nb = $nbline+1;
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDMACRO/;
	    $r = $r . $l;
	}
	foreach my $n (("MPZ","MPQ","D","MPFR")) {
	    $rr = $r;
	    $rr =~ s/ZZZ/$n/g;
	    print "#line $nb \"$file\"\n";
	    print "$rr"
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
	foreach my $n (("MPZ","MPQ","D","MPFR")) {
	    $rr = $r;
	    $rr =~ s/XXX/$n/g;
	    print "#line $nb \"$file\"\n";
	    print "$rr"
	}
	$nb = $nbline+1;
	print "#line $nb \"$file\"\n";
    }
    elsif ($l =~ /MACRO_ALLT_ALLX/) {
	$nb = $nbline+1;
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDMACRO/;
	    $r = $r . $l;
	}
	foreach my $m (("num","bound","eitv","ap_linexpr","ap_lincons","ap_lingen")){
	    foreach my $n (("MPZ","MPQ","D","MPFR")) {
		$rr = $r;
		$rr =~ s/TTT/$m/g;
		$rr =~ s/XXX/$n/g;
		print "#line $nb \"$file\"\n";
		print "$rr"
	    }
	}
	$nb = $nbline+1;
	print "#line $nb \"$file\"\n";
    }
    elsif ($l =~ /MACRO_ALLTARRAY_ALLX/) {
	$nb = $nbline+1;
	$r = "";
	while ($l = <>) {
	    $nbline++;
	    last if $l =~ /ENDMACRO/;
	    $r = $r . $l;
	}
	foreach my $m (("ap_linexpr","ap_lincons","ap_lingen")){
	    foreach my $n (("MPZ","MPQ","D","MPFR")) {
		$rr = $r;
		$rr =~ s/TTT/$m/g;
		$rr =~ s/XXX/$n/g;
		print "#line $nb \"$file\"\n";
		print "$rr"
	    }
	}
	$nb = $nbline+1;
	print "#line $nb \"$file\"\n";
    }
    else { print "$l"; }
}
