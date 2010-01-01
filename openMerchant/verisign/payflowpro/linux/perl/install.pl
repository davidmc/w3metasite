
use ExtUtils::Install;
use Config;

use vars qw( %Config );

$NAME="PFProAPI";
$FULLEXT="PFProAPI";

# Binary Distribution directories
$INST_LIB="blib/lib";
$INST_ARCHLIB="blib/arch";

$pm_to_blib_files = {
	"PFProAPI.pm"   => "$INST_LIB/PFProAPI.pm",
};

# Copy files in preparation for installation
pm_to_blib( $pm_to_blib_files, "$INST_LIB/auto" );

# Perl Installation directories
$SITEARCHEXP = $Config{sitearchexp};
$INSTALLSITEARCH = $Config{installsitearch};
$INSTALLSITELIB = $Config{installsitelib};
$INSTALLARCHLIB = $Config{installarchlib};


$SEPARATOR = ( ( $^O eq "MSWin32" ) ? "\\" : "/" );

$autopacklist = "/auto/$FULLEXT/.packlist";
$autopacklist =~ s|/|$SEPARATOR|g;


$mod_install_files = {
    read          => "$SITEARCHEXP$autopacklist",
    write         => "$INSTALLSITEARCH$autopacklist",
    $INST_LIB     => $INSTALLSITELIB,
    $INST_ARCHLIB => $INSTALLSITEARCH,
};

# If you wish to uninstall PFProAPI at a later time,
# just uncomment the following line:
#$UNINSTALL = 1;

if ( $UNINSTALL ) {
    print "Uninstalling: $INSTALLSITEARCH$autopacklist\n";
    uninstall( "$INSTALLSITEARCH$autopacklist", 1, 0 );

    exit(0);
}

# Here is where the files get installed:

install( $mod_install_files, 1, 0 );

# Now that the files are installed, we can reference PFProAPI:

require PFProAPI;

$LINKTYPE  = "dynamic";
$VERSION   = $PFProAPI::VERSION;
$EXE_FILES = "";

doc_install( "Module", $NAME, 
             "installed into", $INSTALLSITELIB,
             "LINKTYPE", $LINKTYPE,
             "VERSION", $VERSION,
             "EXE_FILES", $EXE_FILES  );


sub doc_install {

	open ( SAVEOUT, ">&STDOUT" );
	open ( STDOUT, ">> $INSTALLARCHLIB/perllocal.pod" );

    $\="\n\n"; 
    print '=head2 ', scalar(localtime), ': C<', shift, '>', ' L<', shift, '>';
    print '=over 4';
    while (defined($$key = shift) and defined($$val = shift)) {
        print '=item *';
		$$val =~ s|/|$SEPARATOR|g;
		print 'C<', "$$key: $$val", '>';
    }
    print '=back';

	close ( STDOUT );
	open ( STDOUT, ">&SAVEOUT" );

	my $localpodfile = "$INSTALLARCHLIB/perllocal.pod";
	$localpodfile =~ s|/|$SEPARATOR|g;

    print "Appending installation info to $localpodfile";

	return;

}

