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
# 3. replace virtual void getParameterName with static void getParameterName
# 4. Same for getEffectName

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

    s/virtual void getParameterName/static void getParameterName/;
    s/virtual bool getEffectName/static bool getEffectName/;
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


# Proc.cpp file
#
#    1. add a `namespace airwin2rack::(name)` after the endif


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

my @headers = <src/autogen_airwin/*.h>;

open (OFH, "> src/ModuleAdd.h");

foreach my $h (@headers)
{
    $h =~ s:src/autogen_airwin/(.*).h$:$1:;
    print OFH "#include \"autogen_airwin/${h}.h\"\n";
    print OFH "int ${h}_unused = AW2RModule::registerAirwindow(\"${h}\", []() { return new airwin2rack::${h}::${h}(0); });";
    print OFH "\n";
}