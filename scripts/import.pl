#!/usr/bin/perl

use strict;
use warnings;

my $f = $ARGV[0];
print "Importing " . $f . "\n";

# copy libs/airwindows/plugins/MacVST/(name)/source/*.cpp src/autogen_airwin

# Header File
#
# 1. replace "audioeffectx.h" with "../airwin2rackbase.h"
# 2. add a `namespace airwin2rack::(name)` at the first enum
# 3. Make sure the define guards are unique

my $inh = "libs/airwindows/plugins/MacVST/$f/source/$f.h";
my $outh = "src/autogen_airwin/$f.h";

print "Converting $inh to $outh\n";

open(OFH, "> $outh") || die "Can't open $outh for writing";
open(IFH, "< $inh") || die "Can't open $inh for reading";

my $namespaced = 0;
while (<IFH>)
{
    if (m/enum/ && ! $namespaced)
    {
        $namespaced = 1;
        print OFH "namespace airwin2rack::$f {\n";
    }

    s/^\#ifndef\s+__(.*)_H/#ifndef __$1_${f}_H/;
    s/^\#define\s+__(.*)_H/#define __$1_${f}_H/;
    s/audioeffectx.h/..\/airwin2rackbase.h/;

    print OFH;
}

print OFH "} // end namespace\n";
close(OFH);
close(IFH);


# .cpp file
#
#    1. add a `namespace airwin2rack::(name)` after the endif


$inh = "libs/airwindows/plugins/MacVST/$f/source/$f.cpp";
$outh = "src/autogen_airwin/$f.cpp";

print "Converting $inh to $outh\n";

open(OFH, "> $outh") || die "Can't open $outh for writing";
open(IFH, "< $inh") || die "Can't open $inh for reading";

$namespaced = 0;
while (<IFH>)
{
    print OFH;
    if (m/#endif/ && !$namespaced)
    {
        print OFH "namespace airwin2rack::$f {\n";
    }
}

print OFH "} // end namespace\n";
close(OFH);
close(IFH);


$inh = "libs/airwindows/plugins/MacVST/$f/source/${f}Proc.cpp";
$outh = "src/autogen_airwin/${f}Proc.cpp";

print "Converting $inh to $outh\n";

open(OFH, "> $outh") || die "Can't open $outh for writing";
open(IFH, "< $inh") || die "Can't open $inh for reading";

$namespaced = 0;
while (<IFH>)
{
    print OFH;
    if (m/#endif/ && !$namespaced)
    {
        print OFH "namespace airwin2rack::$f {\n";
    }
}

print OFH "} // end namespace\n";
close(OFH);
close(IFH);
