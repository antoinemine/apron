#!/usr/bin/perl -W

$file = $ARGV[0];
$nbline = 0;
while ($l = <>) {
    $nbline++;
    if ($l =~ /SWITCHZ\s*(\([^)]*\))/) {
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
    else { print "$l"; }
}
