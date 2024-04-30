#!/usr/bin/perl

use strict;
use warnings;

my $f = $ARGV[0];
print "Importing " . $f . "\n";

# copy libs/airwindows/plugins/MacVST/(name)/source/*.cpp src/autogen_airwin

# Header File
#
# 1. replace "audioeffectx.h" with "../airwin_consolidated_base.h"
# 2. add a `namespace airwinconsolidated::(name)` at the first enum
# 3. Make sure the define guards are unique

my $inh = "libs/airwindows/plugins/MacVST/$f/source/$f.h";
my $outh = "src/autogen_airwin/$f.h";


open(OFH, "> $outh") || die "Can't open $outh for writing";
open(IFH, "< $inh") || die "Can't open $inh for reading";

my $namespaced = 0;
while (<IFH>)
{
    if (m/enum/ && ! $namespaced)
    {
        $namespaced = 1;
        print OFH "namespace airwinconsolidated::$f {\n";
    }

    s/^\#ifndef\s+__(.*)_H/#ifndef __$1_${f}_H/;
    s/^\#define\s+__(.*)_H/#define __$1_${f}_H/;
    s/audioeffectx.h/..\/airwin_consolidated_base.h/;

    next if m/getChunk/;
    next if m/setChunk/;

    print OFH;

    if (m/getParameterDisplay/)
    {
        print OFH "    // Added by the perl as inverses\n";
        print OFH "    virtual bool parameterTextToValue(VstInt32 index, const char *text, float &value);\n";
        print OFH "    virtual bool canConvertParameterTextToValue(VstInt32 index);\n";
    }


}

print OFH "} // end namespace\n";
close(OFH);
close(IFH);


# .cpp file
#
#    1. add a `namespace airwinconsolidated::(name)` after the endif


$inh = "libs/airwindows/plugins/MacVST/$f/source/$f.cpp";
$outh = "src/autogen_airwin/$f.cpp";


open(OFH, "> $outh") || die "Can't open $outh for writing";
open(IFH, "< $inh") || die "Can't open $inh for reading";

$namespaced = 0;
my $inpn = 0;
my $inChunks = 0;

my $paramDisplay = "";
while (<IFH>)
{
    # CHris has a habit of `default: throw` where he means `default: break`
    s/throw/break/g;
    if (m/getChunk/ || m/setChunk/)
    {
        $inChunks = 1;
    }
    elsif (m/::/)
    {
        $inChunks = 0;
    }

    if (!$inChunks){
        print OFH;
    }

    if (m/#endif/ && !$namespaced)
    {
        print OFH "#include <cmath>\n";
        print OFH "#include <algorithm>\n";
        print OFH "namespace airwinconsolidated::$f {\n";
    }


    if (m/^void/)
    {
        $inpn = 0;
    }

    if (m/getParameterDisplay/)
    {
        $inpn = 1;
    }
    if ($inpn)
    {
        $paramDisplay .= $_;
    }
}

my $pdt = $paramDisplay;
$pdt =~ s:\/\*.*?\*\/::gs;

my $printed = 0;
my $pttv = "bool ${f}::parameterTextToValue(VstInt32 index, const char *text, float &value) {\n    switch(index) {\n";
my $ccpt = "bool ${f}::canConvertParameterTextToValue(VstInt32 index) {\n    switch(index) {\n";
while ($pdt =~ m/^.*?case kParam(\S+):(.*?)break;/s)
{
    my $param = $1;
    my $formatter = $2;
    my $ok = 0;
    my $isSwitch = 0;

    # Chris is insanely regular with his transformations here
    # Easy case - just a float2string
    if ($formatter =~ m/float2string\s*\(\s*(${param}|output)\s*,[^;]+;\s*$/)
    {
        $ok = 1;
        $pttv .= "    case kParam${param}: { auto b = string2float(text, value); return b; break; }\n"
    }
    # Scaled float to string
    elsif ($formatter =~ m/float2string\s*\(\s*\(?\s*${param}\s*\*\s*([\d\.]+)\s*\)?\s*,[^;]+;\s*$/)
    {
        my $scaling = $1;
        $ok = 1;
        $pttv .= "    case kParam${param}: { auto b = string2float(text, value); if (b) { value = value / (${scaling}); } return b; break; }\n"
    }
    # Scaled offsetfloat to string p * scale - off
    elsif ($formatter =~ m/float2string\s*\(\s*\(?\s*\(\s*${param}\s*\*\s*([\d\.]+)\s*\)\s*([\+-])\s*([\d\.]+)\s*\)?\s*,[^;]+;\s*$/)
    {
        my $scaling = $1;
        my $pm = $2;
        my $offset = $3;
        if ($pm =~ m/\+/) { $pm = "-";} else { $pm = "+" };
        $pttv .= "    case kParam${param}: { auto b = string2float(text, value); if (b) { value = (value $pm ${offset}) / (${scaling}); } return b; break; }\n";
        $ok = 1;
    }
    # Lazy - offset positive
    elsif ($formatter =~ m/float2string\s*\(\s*\(\s*${param}\s*\*\s*([\d\.]+)\s*\)\s*\+\s*([\d\.]+)\s*,[^;]+;\s*$/)
    {
        my $scaling = $1;
        my $offset = $2;
        $pttv .= "    case kParam${param}: { auto b = string2float(text, value); if (b) { value = (value - ${offset}) / (${scaling}); } return b; break; }\n";
        $ok = 1;
    }
    # (p-off)*scale
    elsif ($formatter =~ m/float2string\s*\(\s*\(\s*${param}\s*-s*([\d\.]+)\s*\)\s*\*\s*([\d\.]+)\s*,[^;]+;\s*$/)
    {
        my $offset = $1;
        my $scale = $2;
        $pttv .= "    case kParam${param}: { auto b = string2float(text, value); if (b) { value = (value / ${scale}) + ${offset}; } return b; break; }\n";
        $ok = 1;
    }
    # ((P*P)*m)+o - inverse is sqrt(std::min((n-o)/m, 0.001)
    elsif ($formatter =~ /float2string\s*\(\s*\(\s*\(*${param}\*${param}\)*\*([\d\.]+)\)\+([\d\.]+)/)
    {
        my $sqmul = $1;
        my $offst = $2;

        $pttv .= "    case kParam${param}: { auto b = string2float(text, value); if (b) { value = sqrt(std::max((value - ${offst}) / ($sqmul), 0.)); } return b; break; }\n";
        $ok = 1;
    }
    # ((P*P*P)*m)+o - inverse is cbrt(std::min((n-o)/m, 0.001)
    elsif ($formatter =~ /float2string\s*\(\s*\(\s*\(*${param}\*${param}\*${param}\)*\*([\d\.]+)\)\+([\d\.]+)/)
    {
        my $sqmul = $1;
        my $offst = $2;

        $pttv .= "    case kParam${param}: { auto b = string2float(text, value); if (b) { value = std::cbrt((value - ${offst}) / ($sqmul)); } return b; break; }\n";
        $ok = 1;
    }
    # pow(param,N)*M
    elsif ($formatter =~ /float2string\s*\(\s*pow\(${param},([\d\.]+)\)\*([\d\.]+)/)
    {
        my $exp = $1;
        my $mul = $2;

        $pttv .= "    case kParam${param}: { auto b = string2float(text, value); if (b) { value = pow(std::max((value/$mul), 0.), (1.0/$exp)); } return b; break; }\n";
        $ok = 1;
    }
    elsif ($formatter =~ m/float2string\s*\(([^,]+),[^;]+;\s*$/)
    {
        my $arg = $1;
        print " CUSTOM ARG :" . $f . "::" . $param . " >> " . $arg . "\n";
        print "      INPUT : " . $formatter . "\n";

        if ($arg =~ m/^\s*\(([${param}\*]+)([0-9.]+)\)\+([0-9.]+)/)
        {
            print "     - GOT $1 $2 $3\n";
        }
        else
        {
            print "      - UNKNOWN\n"
        }
        $ok = 0;
    }
    # db2string
    elsif ($formatter =~ m/dB2string\s*\(\s*${param}\s*,[^;]+;\s*$/)
    {
        $ok = 1;
        $pttv .= "    case kParam${param}: { auto b = string2dBNorm(text, value); return b; break; }\n"
    }
    elsif ($formatter =~ m/switch/)
    {
        # We know we can't do these
        $isSwitch = 1;
        $ok = 0;
    }
    elsif ($formatter =~ m/int2string\s*\((\(VstInt32\)|\(int\))?\s*(floor)?\((.*)\),/)
    {
        my $body = $3;
        $body =~ s/drive/$param/;

        if ($body =~ m/^\s*${param}\s*\*\s*([\d\.]+)\s*$/)
        {
            my $scl = $1;
            $pttv .= "    case kParam${param}: { auto b = string2float(text, value); if (b) { value = (value + 0.1) / ${scl}; } return b; break; }\n";
            $ok = 1;
        }
        else
        {
            print "  INT BODY: " . $f . "::" . $param . " / " . $body . "\n";
            $ok = 0;
        }
    }
    else
    {
        $printed = 1;
        print " Skipping convert :" . $f . "::" . $param . " / " . $formatter . "\n";
    }

    if ($ok)
    {
        $ccpt .= "        case kParam${param}: return true;\n"
    }
    elsif ($isSwitch)
    {
        # supress this warning
    }
    elsif ($formatter =~ m/int2string/)
    {
        print "  INT UNDONE: " . $f . "::" . $param . " / " . $formatter . "\n";
    }
    else
    {
        print "  UNINVERTED: " . $f . "::" . $param . " / " . $formatter . "\n";
    }

    $pdt =~ s/^.*?case kParam(\S+):(.*?)break;//s;
}
$ccpt .= "\n    }\n    return false;\n}\n";
$pttv .= "\n    }\n    return false;\n}\n";
print OFH $pttv;
print OFH $ccpt;

print OFH "} // end namespace\n";
close(OFH);
close(IFH);


$inh = "libs/airwindows/plugins/MacVST/$f/source/${f}Proc.cpp";
$outh = "src/autogen_airwin/${f}Proc.cpp";


open(OFH, "> $outh") || die "Can't open $outh for writing";
open(IFH, "< $inh") || die "Can't open $inh for reading";

$namespaced = 0;
while (<IFH>)
{
    print OFH;
    if (m/#endif/ && !$namespaced)
    {
        print OFH "namespace airwinconsolidated::$f {\n";
    }
}

print OFH "} // end namespace\n";
close(OFH);
close(IFH);
