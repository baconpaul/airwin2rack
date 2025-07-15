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
my %catOrder;
my $currFXCount = 0;
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
                $catOrder{$p} = $currFXCount;
                $currFXCount ++;
            }
        }
        elsif (m/,/)
        {
            my @plist = split(/[\,\.]/);

            foreach my $p (@plist) {
                $p =~ s/\s*//g;
                $cats{$p} = $goingcat;
                $catOrder{$p} = $currFXCount;
                $currFXCount ++;
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

# Step 3: Deduce isMono from MacAU plugin version
my @forceIsMonoForPlugins = ('ToTape8'); # These are plugins that, even though they are stereo, work great in mono also...
my %isMonos;
foreach my $fx (@plugins) {
    $isMonos{$fx} = 1;
    if (!grep(/$fx/, @forceIsMonoForPlugins)) {
        my $fn = $fx;
        $fn =~ s/Point$/Poynt/; # Fix for "weird" naming in MacAU/Point plugin
        open (AU, "< libs/airwindows/plugins/MacAU/$fx/$fn.h") || die "Can't open what: $!";
        while (<AU>) {
            $isMonos{$fx} = 0 if (m/SupportedNumChannels/);
        }
    }
}

open(OFH, "> src/ModuleAdd.h");

foreach my $fx (@plugins) {
    my $cat = "Unclassified";
    my $what = "";
    my $catO = -1;
    my $isMono = "false";
    $cat = $cats{$fx} if (exists $cats{$fx});
    $catO = $catOrder{$fx} if (exists $catOrder{$fx});
    $what = $whats{$fx} if (exists $whats{$fx});
    $isMono = "true" if (exists $isMonos{$fx} && $isMonos{$fx});

    my $coll = "{ }";
    if ($what =~ s/\[(.*)\]\s*$//)
    {
        my @kvlist = split(/;/, $1);
        for my $kv (@kvlist)
        {
            if ($kv =~ m/([^=]+)=(.*)$/)
            {
                my $key = $1;
                my $val = $2;
                if ($key =~ m/coll/ && $val =~ m/\S/)
                {
                    $coll = "{ ";
                    my $pre = "";
                    for my $cl (split(/,/, $val))
                    {
                        $coll .= $pre;
                        # remove leading and trailing spaces from categories
                        $cl =~ s/^\s*//;
                        $cl =~ s/\s*$//;
                        $coll .= "\"$cl\"";
                        $pre = ", ";
                    }
                    $coll .= " }";
                }
            }
        }

    }

    if (!-d "libs/airwindows/plugins/MacVST/$fx") {
        die "Can't find fx directory '$fx'";
    }

    print "UNCLASSIFIED : $fx\n" if ($cat =~  m/Unclassified/);

    system("perl scripts/import.pl $fx");

    my $GHL = `bash scripts/commitDate $fx`;
    chomp $GHL;
    print OFH "#include \"autogen_airwin/${fx}.h\"\n";
    print OFH "int ${fx}_unused = AirwinRegistry::registerAirwindow({\"${fx}\", \"${cat}\", $catO, $isMono, \"${what}\", airwinconsolidated::${fx}::kNumParameters, \"${GHL}\", []() { return std::make_unique<airwinconsolidated::${fx}::${fx}>(0); }, -1, $coll});";
    print OFH "\n";
}

print OFH "int unusedComplete = AirwinRegistry::completeRegistry();\n";
close OFH;

open(OFC, "> src/autogen_airwin/CMakeLists.txt") || die "Cant make cmake";
print OFC "set(AIRWIN_SOURCES \n";
foreach my $fx (@plugins) {
    print OFC "   src/autogen_airwin/${fx}.cpp\n";
    print OFC "   src/autogen_airwin/${fx}Proc.cpp\n"
}
print OFC "\n)\n";
close OFC;


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
