use ExtUtils::MakeMaker;
use ExtUtils::Manifest;
use File::Copy;
use Config;

my $LIBS = '-L./lib -L. -lpfpro -lMSVCRT ';
my $fullperl = $Config{perlpath};

# Win32: Create a hints file in a new hints subdirectory
# so we can get rid of the use of the -nodefaultlib flag
if ( $^O eq "MSWin32" ) {

	mkdir("hints",0744);
	open( HINT, ">hints/${^O}.pl" );
	print HINT <<END_HINT;
my \$flag = \$Config{lddlflags};
\$flag =~ s/-nodefaultlib//g;
\$self->{LDDLFLAGS} = \$flag;
END_HINT
	close HINT;

	copy("MANIFEST_NT","MANIFEST") if -e "MANIFEST_NT";

	# For Win32 platforms, if this is not the correct location of the
	# .lib files containing the required symbols, please change the value
	# assigned to $EXTRALIBS to the correct location where the required
	# .lib files are located, so make will successfully find all symbols:

	$EXTRALIBS = '-Lc:\\\"Program Files\"\\\"Microsoft Visual Studio\"\\VC98\\Lib -Lc:\\dev\Client\Perl';
	$INCLUDE = '-Ic:\\\"Program Files"\\\"Microsoft Visual Studio"\\\VC98\\\Include';
}
else {
	copy("MANIFEST_UNIX","MANIFEST") if -e "MANIFEST_UNIX";
	$fullperl = 'LD_LIBRARY_PATH=$(LD_RUN_PATH) '.$fullperl;
}

$LIBS .= $EXTRALIBS;

# Find required lib files to copy from Payflow Pro Client package
my $man = ExtUtils::Manifest::maniread("MANIFEST");
foreach $key (keys %$man) {
	if ( $key =~ m|^lib/| ) {
		$key =~ s|^lib/||;
		push(@reqfiles, $key);
	}
}

# Copy required files from the Payflow Pro Client package in ../lib
# to populate a newly created lib subdirectory so all files required
# by the MANIFEST file are found when the WriteMakefile() is called.
mkdir("lib",0744);
foreach $file (@reqfiles) {
    copy("../lib/$file","lib/$file");
    ($d,$i,$m) = stat("../lib/$file");
    chmod($m,"lib/$file");
    if ( !(-e "lib/$file") ) {
        print "\n\t==> files missing: <==\n";
        print   "\t\t@reqfiles\n\n";
        last;
    }
}

# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    'NAME'         => 'PFProAPI',
    'VERSION_FROM' => 'PFProAPI.pm', # finds $VERSION
    'LIBS'         => $LIBS, # e.g., '-lm' 
    'DEFINE'       => '',    # e.g., '-DHAVE_SOMETHING' 
    'INC'          => $INCLUDE,    # e.g., '-I/usr/include/other'
    'PM'           => {'PFProAPI.pm' => '$(INST_LIBDIR)/PFProAPI.pm'},
    'FULLPERL'     => $fullperl,
);
