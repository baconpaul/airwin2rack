use strict;
use warnings;

# Step 1: Get all the plugins
my $plugdir = "libs/airwindows/plugins/MacVST";
opendir my $dir, $plugdir or die "Cannot open directory: $!";
my @pluginPotentials = readdir $dir;
closedir $dir;
my @plugins;
for my $p (@pluginPotentials) {
    next if ($p =~ m/^ClipOnly$/);
    if (-d "$plugdir/$p/source") {
        push @plugins, $p;
    }
}

@plugins = sort @plugins;

# Step 2: Read the categories from airwindopedia
open(AW, "< libs/airwindows/Airwindopedia.txt") || die "Cant open $!";
my %cats;
my $goingcat = $1;
my $going = 0;
while (<AW>) {
    next if (m/^\s*$/);
    $going = 0 if (m/^###/);

    if ($going) {
        if (m/^(.*):\s*(.*)$/) {
            $goingcat = $1;
            # Work around typos in the pedia
            my @plist = split(/[\,\.]/, $2);

            foreach my $p (@plist) {
                $p =~ s/\s*//g;
                $cats{$p} = $goingcat;
            }
        }
        elsif (m/,/)
        {
            my @plist = split(/[\,\.]/);

            foreach my $p (@plist) {
                $p =~ s/\s*//g;
                $cats{$p} = $goingcat;
            }
        }
    }
    $going = 1 if (m/^\# Categories/);
}

open (WH, "< libs/airwindows/what.txt") || die "Can't open what: $!";
my %whats;
while( <WH>)
{
    s/\R//g;
    if (m/^(\S+) /)
    {
        $whats{$1} = $_;
    }
}

open(OFH, "> src/ModuleAdd.h");

foreach my $fx (@plugins) {
    my $cat = "Unclassified";
    my $what = "";
    $cat = $cats{$fx} if (exists $cats{$fx});
    $what = $whats{$fx} if (exists $whats{$fx});

    if (!-d "libs/airwindows/plugins/MacVST/$fx") {
        die "Can't find fx directory '$fx'";
    }

    print "UNCLASSIFIED : $fx\n" if ($cat =~  m/Unclassified/);

    #system("perl scripts/import.pl $fx");

    print OFH "#include \"autogen_airwin/${fx}.h\"\n";
    print OFH "int ${fx}_unused = AirwinRegistry::registerAirwindow({\"${fx}\", \"${cat}\", \"${what}\", airwin2rack::${fx}::kNumParameters, []() { return std::make_unique<airwin2rack::${fx}::${fx}>(0); }});";
    print OFH "\n";
}

print OFH "int unusedComplete = AirwinRegistry::completeRegistry();\n";
close OFH;

open(IN, "< libs/airwindows/Airwindopedia.txt");
my %helpText;
my @currents;
my $inCurrent = 0;
while(<IN>)
{
    if (m/##### (\S+)/)
    {
        my $current = $1;
        s/(#+)/#/;

        if ($inCurrent == 0) {
            $helpText{$current} .= $_;
            @currents =($current);
        }
        else
        {
            $helpText{$current} .= $_;
            push(@currents, $current);
        }
        $inCurrent = 1;
    }
    else
    {
        $inCurrent = 0;
        foreach my $cc (@currents) {
            $helpText{$cc} .= $_;
        }

    }
}
close IN;

foreach my $dfx ( keys %helpText)
{
    my $fn = ${dfx};
    $fn =~ s/[^A-Za-z0-9\-]+//g;
    open (DC, "> res/awpdoc/${fn}.txt");
    print DC $helpText{$dfx} . "\n";
    close DC;

}
