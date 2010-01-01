#!/usr/bin/perl

#########################################################
###
### Sample Verisign Transaction Scripts
###
##########################################################
### initialize these variables to the correct values to localize to your system
###

print "Content-type: text/HTML", "\n\n";

# parse the passed variables and place in contents

if ($ENV{'REQUEST_METHOD'} eq 'POST'){
    # How many bytes are we supposed to receive?
    read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'});
    @pairs = split(/&/, $buffer);
    foreach $pair (@pairs)    {
        ($name, $value) = split(/=/, $pair);
        $contents{$name} = $value;
   }
}


$host = "test-payflow.verisign.com";
$port = 443;

&run_transaction;
&print_results;

###########################################################
sub run_transaction{

# Set environment variables
#  these variables must reflect the locations of your
# 1) pfpro binary
# 2) libpfpro.so
# 3) certificate

# my $PFPRO_EXE_PATH = "../../../bin";
$ENV{LD_LIBRARY_PATH} .="../../../lib:.:/usr/lib";
$ENV{PFPRO_CERT_PATH} .= "../../../certs";


$parms = "TRXTYPE=S&TENDER=C&PWD=$contents{userPW}&USER=$contents{userId}&VENDOR=$contents{vendorId}&PARTNER=$contents{partnerId}&ACCT=$contents{cardNum}&EXPDATE=$contents{cardExp}&AMT=$contents{amount}&ZIP=12345&comment=cgi test";
$cmd = "pfpro $host $port  \"$parms\" 30";	# transaction command
open (RESULTS, "$cmd 2>&1 |");              	# execute the command, (STDERR mapped to STDOUT)

    $status = "";
    while ($_ = <RESULTS>) {
        $status.=$_;
    }
    chomp($status);

    if ( ($status eq "") && ($^O ne "MSWin32") ) {
        $status.="System error: Please check that libpfpro.so exists in /usr/lib directory.";
    }

    close RESULTS;
} ###  <-- end of subroutine

###########################################################
sub print_results{

print <<"EOM";
<HTML>
<HEAD>
        <TITLE>Transaction Result</TITLE>
</HEAD>

<BODY BGCOLOR="#ffffff" text="#000000">
<br><br>
response = $status 
<br><br>

</BODY>
</HTML>
EOM
} ###  <-- end of subroutine

