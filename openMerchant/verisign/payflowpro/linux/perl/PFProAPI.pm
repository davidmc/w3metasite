# Payflow Pro API    
# (c)1998-2000 Verisign
# All Rights Reserved.

package PFProAPI;

use strict;
use vars qw (@ISA @EXPORT_OK $VERSION $DEBUG $PFPRO
             $HOST $PORT $PROXYADDRESS $PROXYLOGIN $PROXYPASS $PROXYPORT
             $USER $VENDOR $PARTNER $PWD $TRXTYPE $TENDER $TIMEOUT $INTERNAL);

require Exporter;
require DynaLoader;
require AutoLoader;

@ISA = qw(Exporter DynaLoader);
@EXPORT_OK = qw(pfpro timeIt call_pfpro makeparams parseparams);
$VERSION = '0.03';


## START CONFIGURATION ##
# For any platform on which you cannot build XSUBS, or for WINDOWS NT if
# you are not able to run the binary Payflow Pro Perl dll, you may set the 
# variable $PFPRO to the path to your pfpro binary.  Also works on Unix if
# you cannot build PFProAPI.
# Uncomment the following line and correct the path to your pfpro 
# executable if you can't build the xsub or can't run the dll on NT:
#$PFPRO      = "c:\\verisign\\payflowpro\\bin\\pfpro.exe";		

$USER         = "";	# You must register with Verisign to get 
$VENDOR       = "";	# your USER/VENDOR/PARTNER/PWD
$PARTNER      = "";
$PWD          = "";

# Default setup when calling pfpro().  Override in call.
$HOST         = "test-payflow.verisign.com";
$PORT         = "443";
$TIMEOUT      = 30;
$PROXYADDRESS = "";
$PROXYPORT    = 0;
$PROXYLOGIN   = "";
$PROXYPASS    = "";

# Default data stream when calling pfpro(). Override in hash data.
$TRXTYPE      = "S";                    # Default to sale (immediate capture)
$TENDER       = "C";                    # Default to credit cards

## END CONFIGURATION ##
$INTERNAL = 1 if $PFPRO eq "";
bootstrap PFProAPI $VERSION if $INTERNAL;	

###########
# pfpro   #
###########
# Main payment routine
# IN: $data
#     ref to a hash containing name-value pairs as defined in
#     "Verisign Payment Processing Client Integration Documentation"
#     $host (optional) : host to connect to (defaults to value in $HOST)
#     $port (optional) : port to connect to (defaults to value in $PORT)
#     $timeout      (optional) : timeout value to (defaults to value in $TIMEOUT)
#     $proxyaddress (optional) : address of proxy server (defaults to value in $PROXYADDRESS)
#     $proxyport    (optional) : port of proxy server (defaults to value in $PROXYPORT)
#     $proxylogin   (optional) : login (defaults to value in $PROXYLOGIN)
#     $proxypass    (optional) : passsword (defaults to value in $PROXYPASS)

# NOTE:
#     USER and PWD are optional in the $data hash, as they will be taken
#     from the globals $USER and $PWD if not present
sub pfpro {
  my $data = shift;
  my $host = shift || $HOST;
  my $port = shift || $PORT;
  my $timeout      = shift || $TIMEOUT;
  my $proxyaddress = shift || $PROXYADDRESS;
  my $proxyport    = shift || $PROXYPORT;
  my $proxylogin   = shift || $PROXYLOGIN;
  my $proxypass    = shift || $PROXYPASS;

  my $parmlist;
  my ($name, $value);

  if (! $data)   {
    # If no data was passed, we will build a test transaction
    $data->{ACCT}    = "5499740000000016";
    $data->{EXPDATE} = "1209";
    $data->{AMT}     = "1.00";
  }

  # Set defaults if they haven't been provided in hash
  $data->{USER}    = $USER    unless $data->{USER};
  $data->{VENDOR}  = $VENDOR  unless $data->{VENDOR};
  $data->{PARTNER} = $PARTNER unless $data->{PARTNER};
  $data->{PWD}     = $PWD     unless $data->{PWD};
  $data->{TRXTYPE} = $TRXTYPE unless $data->{TRXTYPE};
  $data->{TENDER}  = $TENDER  unless $data->{TENDER};

  $parmlist = makeparams($data);

  my $resultstr = call_pfpro($host,
                      $port,
                      $parmlist,
                      $timeout,
                      $proxyaddress,
                      $proxyport,
                      $proxylogin,
                      $proxypass );

  # Split the result code up into a hash

  my $result = parseparams ($resultstr);

  # return the result
  return ($result, $resultstr);
}

##########
# timeIt #
##########
# Timing Utility
# IN:  $transactions Number of transactions to run
# OUT: ($transactions, $secs, $tps);
# Side Effects:  Prints message if $DEBUG is > 100;
sub timeIt {
  my $transactions = shift || 100;
  my $data;
  my $result;
  my $resultstr;

  $data->{ACCT}    = "5499740000000016";
  $data->{EXPDATE} = "1002";
  $data->{AMT}     = "11.11";
  $data->{USER}    = "user";
  $data->{VENDOR}  = "vendor";
  $data->{PARTNER} = "partner";
  $data->{PWD}     = "pwd";

  my $start = time;
  my $x;
  my $i;
  for ($i = 1; $i <= $transactions; $i++) {
    ($result, $resultstr) = pfpro($data);
    debug(100, "$i : $resultstr" );
  }
  my $end = time;
  my $secs = $end - $start; 
  $secs = $secs || 1;	# Round up if zero, otherwise next line goes boom
  my $tps  = $transactions / $secs;
  debug(100, "($tps TPS) $transactions transactions in $secs seconds.");
  return ($transactions, $secs, $tps);
}

pfproInit();

################
# call_pfpro   #
################
# Will be a native method on supported platforms
# Simply maps parameters into the appropriat call (pfpro | xsub)
sub call_pfpro {
  my $host         = shift;
  my $port         = shift;
  my $data         = shift;
  my $timeout      = shift;
  my $proxyaddress = shift;
  my $proxyport    = shift;
  my $proxylogin   = shift;
  my $proxypass    = shift;
  my $result       = "";

  # Create the return string

  if ($INTERNAL) {
	my $context;

	pfproCreateContext($context, $host, $port, $timeout, $proxyaddress, $proxyport, $proxylogin, $proxypass);
	pfproSubmitTransaction($context, $data, length($data), $result);
	pfproDestroyContext($context);
  } else {
    # add quotes around data
    my $parmlist = qq("$data");
    $result = `$PFPRO $host $port $parmlist $timeout $proxyaddress $proxyport $proxylogin $proxypass`;  
  }
  
  return $result;
}


END {
	pfproCleanup();
}

##############
# makeparams #
##############
# In:  Perl HASH of values
# Out: Properly escaped string for use with Verisign
sub makeparams {
   my $hash = shift;
   my $data;
   my $key;
   my $value;
  
   my $length;
   foreach $key (keys %$hash) {
     $value = $hash->{$key};    
     $length = length($value);
     $key .= "[$length]";
     $data .= "$key=$value&";
   }
   chop $data;
   return $data;
}

###############
# parseparams #
###############
# In: Name/value pairs returned from Verisign
# Out: Perl HASH of values
sub parseparams {
  my $data = shift;
  my $hash;
  my ($name, $value);

  my @vars = split "&", $data;
  foreach (@vars) {
    ($name, $value) = split "=";
    $hash->{$name} = $value;
  }
  return $hash;
}

#########
# debug #
#########
sub debug {
  my $level = shift;
  my $msg   = shift;

  print "$msg\n" if $main::DEBUG > $level;
}


1;
__END__

=head1 NAME

PFProAPI - Perl extension for using the Verisign Payflow Pro client.

=head1 SYNOPSIS

SIMPLE (non-shared environment):

  use PFProAPI qw (pfpro);
  $data->{ACCT}    = "5499740000000016";  # Test Card
  $data->{EXPDATE} = "1202";
  $data->{AMT}     = "1.00";
  $data->{USER}    = "user";
  $data->{VENDOR}  = "vendor";
  $data->{PARTNER} = "partner";
  $data->{PWD}     = "pwd";
  ($resulthashref, $resultstr) = pfpro($data);

SHARED (ISP environment) [ignores defaults]

  use PFProAPI qw call_pfpro;
  # Build up the transaction string

  $data->{ACCT}    = "5499740000000016";  # Test Card
  $data->{EXPDATE} = "1202";
  $data->{AMT}     = "1.00";
  $data->{USER}    = $USER;
  $data->{USER}    = $VENDOR;
  $data->{USER}    = $PARTNER;
  $data->{PWD}     = $PWD;
  $data->{TRXTYPE} = $TRXTYPE;
  $data->{TENDER}  = $TENDER;

  # better to use makeparams() to escape required variables
  # $parmlist = makeparams($data);
  
  foreach $name (keys %$data) {
    $parmlist .= $name."=".$data->{$name}."&";
  }
  chop $parmlist;       # Remove extra "&";

  my $resultstr = call_pfpro($host,
                      $port,
                      $parmlist,
                      $TIMEOUT,      # number of seconds, usually 30
                      $PROXYADDRESS, # Only if you have a proxy / firewall
                      $PROXYPORT,
                      $PROXYLOGIN,
                      $PROXYPASS );

  # Split the result code up into a hash
  # OR: $resultstr = parseparams($result);
  my $result;
  my (@values) = split "&", $resultstr;
  foreach (@values) {
    ($name, $value) = split "=";
    $result->{$name} = $value;
  }



=head1 DESCRIPTION

You must be a knowledgeable Windows or NT Perl programmer to effectively
use this module.  

This is ALPHA code and subject to change.  The "alpha" state does 
not reflect code quality.  Rather, the API may change in future 
versions to better support additional features and OS's (i.e., Windows NT).  

For any platform on which you cannot build XSUBS, or if you are not able
to run the binary Payflow Pro Perl dll for WINDOWS NT, you may set the 
variable $PFPRO to the path to your pfpro binary.  Note that permissions
are critical in this case, as many Webservers need special configuration
to spawn additional processes.

Before using this module, you should modify the default configuration 
in the head of this file (PFProAPI.pm) to reflect your own setup.  At the 
very least you will need to obtain a test account with Verisign 
(http://www.verisign.com/payment).  You may then wish to run the example 
code above.

You *must* read this module and understand various options to use this
module successfully.  If you are using a shared isntallation (i.e, an ISP),
you will want to leave the defaults blank and force users to supply the
appropriate numbers in their own scripts by calling call_pfpro directly.

          use PFProAPI qw (pfpro
                           makeparams
                           parseparams);
      
      Please see the Verisign Integration documentation for a complete
      discussion of parameters.  pfpro maps a Perl call to the internal
      Payflow Pro Client library.  The parameters are the same as the
      binary and library client.  The return value is a list of name=value
      pairs, exactly as you would receive from the client.

      ($result, resultstr) = pfpro ($data	  # Data to pass to server
                           $host,         # Host address
                           $port,         # Port to use
                           $timeout,      # default: 30 (30 seconds)
                           $proxyaddress, # Your proxy information:
                           $proxyport,    #  (Leave your proxy
                           $proxylogin,   #   information blank
                           $proxypass     #   if not required.)
                           );

      The $result is a reference to a an associative array of variables 
      to pass to the server.  The $resultstr is the raw return codes,
      for debugging or logging purposes.

      Note that the Verisign system, while it does use name=value pairs,
      similar to HTTP encodings, is not the same as that defined in the
      HTTP specifications.  The makeparams and getparams functions take
      the data and encode it properly.  

      The makeparams() convenience function accepts a *reference* to a
      Perl hash (associative array) and returns a scalar which is encoded
      appropriately for passing in the $params parameter of pfpro.

            $data = makeparams($datahashref);

      The getparams() convenenience function does the inverse: converts a
      Verisign encoded string to an associative array for easier processing.

            $datarehash = getparams($data);


=head1 AUTHOR

Verisign, Inc.  vps-support@verisign.com

=head1 SEE ALSO

perl(1).

=cut