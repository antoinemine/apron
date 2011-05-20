#!/usr/bin/perl -W

open(my $tmp, "+>", undef);

my $prevext = 0;
while ($l = <>) {
    if ($l =~ /external[^"]*\n/){
	my $r = $l;
	$r =~ s/external(.*)\n/external$1/g;
	print $tmp "$r";
	$prevext = 1;
    }
    elsif ($prevext){
	if ($l =~ /^[\h]*$/){
	    $prevext = 0;
	}
	else {
	    print $tmp "$l";
	}
    }
    else {
	print $tmp "$l";
	$prevext = 0;
    }
}

seek($tmp,0,0);
while ($l = <$tmp>){
    print "$l";
}
