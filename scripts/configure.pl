use strict;
use warnings;
open(IN, "< fxconfig.in") || die "Can't open fxconfig.in. Running from the root dir?";
open (OFH, "> src/ModuleAdd.h");

while(<IN>)
{
    next if (m/^#.*/);
    next if (m/^\s+$/);

    my $fx = "";
    my $cat = "";
    if (m/^(\S+);(.*)$/)
    {
        $fx = $1;
        $cat = $2;
    }
    elsif (m/^(\S*)\s*$/)
    {
        $fx = $1;
        $cat = "Unclassified";
    }

    if (! -d "libs/airwindows/plugins/MacVST/$fx")
    {
        die "Can't find fx directory '$fx'";
    }

    system("perl scripts/import.pl $fx");

    print OFH "#include \"autogen_airwin/${fx}.h\"\n";
    print OFH "int ${fx}_unused = AW2RModule::registerAirwindow({\"${fx}\", \"${cat}\", airwin2rack::${fx}::kNumParameters, []() { return std::make_unique<airwin2rack::${fx}::${fx}>(0); }});";
    print OFH "\n";
}

print OFH "int unusedComplete = AW2RModule::completeRegistry();\n";
close OFH;
close IN;

#my @headers = <src/autogen_airwin/*.h>;

#foreach my $h (@headers)
#{
#$h =~ s:src/autogen_airwin/(.*).h$:$1:;
#print OFH "#include \"autogen_airwin/${h}.h\"\n";
#print OFH "int ${h}_unused = AW2RModule::registerAirwindow({\"${h}\", airwin2rack::${h}::kNumParameters, []() { return std::make_unique<airwin2rack::${h}::${h}>(0); }});";
#print OFH "\n";
#}