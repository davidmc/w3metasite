#! /usr/bin/perl
# ptkpos: A demo point-of-sale terminal using perl/Tk
######################################################
# INSTALLATION: After installing Perl and PFPro, you
# must install the Tk package.  Under Windows, use
# PPM, and type "install Tk".  Under Unix, you may
# wish to use the CPAN utility (perldoc CPAN) to
# install the Tk packages.  You must have X-Windows
# and related librairies installed to use this demo.
#

use Tk;
use Tk::DialogBox;
use Tk::NoteBook;
use Tk::LabEntry;
use PFProAPI qw(call_pfpro);

use Data::Dumper;
use strict;     # Always!
$^W++;          # Turn on warnings

##### Global Configuration Structures

my @cards   = ('Visa', 'MasterCard',
               'American Express', 'Discover');
my $trxtypetranslate = {'Sale' 			=> 'S', 
                        'Credit'		=> 'C', 
                        'Authorization'		=> 'A', 
                        'Delayed Capture'	=> 'D', 
                        'Voice Authorization'	=> 'F', 
                        'Void'			=> 'V'
                       };

my @trxtype = keys %$trxtypetranslate;

####
my $DEBUG = 1;  # turn off (0) if you don't want to see internals
my $mainwindow;
my $f;		# dialog window

# Default configuration. This is loaded from
# "master.pcg" at startup, if it exists.
my $config = { HOST    =>  'test-payflow.verisign.com',
               PORT    =>   443,
               USER    =>  '',
               VENDOR  =>  '',
               PARTNER =>  '',
               PWD     =>  '',
               P_ADDR  =>  '',
               P_PORT  =>  0,
               P_LOGON =>  '',
               P_PASSWORD  =>  '' };

my $trans= { TENDER     => 'C', # only support credit cars
             TRXTYPE    => '',
             ACCT       => '',
             AMT        => '',
             COMMENT1   => '',
             COMMENT2   => '',
             EXPDATE    => '',
             ORIGID     => '',
             STREET     => '',
             ZIP        => '',   	     
             _NAME      => '',
             _CARDTYPE  => '',
             _PARAM     => ''	# Compete transaction, as send
           };
             
  
my $results = {	RESP     => '',	# Full response
                PNREF    => '',
 		RESULT   => '',
		RESPCODE => '',
                RESPMSG  => '',
                AUTHCODE => '',
		ERRCODE  => '',
                ERRMSG   => '',
		AVSZIP	 => '',
		AVSADDR  => ''
              };

# Begin main program
MAIN: {
  debug("+MAIN");

  # Attempt to load the master.pcg configuration file
  readConfig("master.pcg");

  $mainwindow = MainWindow->new();
  my $menubar = $mainwindow->Frame()->grid(-row => 0, -col => 0,
                                           -columnspan => 3,
                                           -sticky => 'nw');
 
  my $filemenu = $menubar->Menubutton(-text => 'File');

  $filemenu->command( -label   =>  'Open Config',
                                    -command =>  \&loadConfig );
  $filemenu->command( -label   =>  'Save Config',
                                   -command => \&saveConfig  );
  $filemenu->separator();
  $filemenu->command(-label   => 'Configuration...',
                                   -command => \&doConfig );
  $filemenu->separator();
  $filemenu->command(-label => 'Exit', -command => sub {exit;} );

  $filemenu->pack(-side => 'left');

  my $left   = $mainwindow->Frame->grid(-row => 1,
                                        -col => 0,
                                        -sticky => 'nw');
  my $right  = $mainwindow->Frame->grid(-row => 1,
                                        -col => 1,
                                        -sticky => 'nw');
  my $bottom = $mainwindow->Frame->grid(-row => 2,
                                        -col => 0,
                                        -columnspan => 3,
                                        -sticky => 'nw');


  ### Left panel, input functionality

  $left->Optionmenu(
               -options => \@trxtype,
               -variable => \$trans->{TRXTYPE},
               )->pack(-side => "top", -anchor => "nw");

  $left->Optionmenu(
               -options => \@cards,
               -variable => \$trans->{_CARDTYPE},
               )->pack(-side => "top", -anchor => "nw");

  $left->LabEntry(-label => "Name",
     -labelPack => [-side => "right", -anchor => "w"],
     -width => 20,
     -textvariable => \$trans->{_NAME})->pack(-side => "top",
                                              -anchor => "nw");

  $left->LabEntry(-label => "Card Number",
     -labelPack => [-side => "right", -anchor => "w"],
     -width => 20,
     -textvariable => \$trans->{ACCT})->pack(-side => "top",
                                             -anchor => "nw");

  $left->LabEntry(-label => "Exp Date (1201)",
     -labelPack => [-side => "right", -anchor => "w"],
     -width => 20,
     -textvariable => \$trans->{EXPDATE})->pack(-side => "top",
                                                -anchor => "nw");

  $left->LabEntry(-label => "Invoice Number",
     -labelPack => [-side => "right", -anchor => "w"],
     -width => 20,
     -textvariable => \$trans->{INVNUM})->pack(-side => "top",
                                               -anchor => "nw");

  $left->LabEntry(-label => "Amount",
     -labelPack => [-side => "right", -anchor => "w"],
     -width => 20,
     -textvariable => \$trans->{AMT})->pack(-side => "top",
                                            -anchor => "nw");

  $left->LabEntry(-label => "Original ID",
     -labelPack => [-side => "right", -anchor => "w"],
     -width => 20,
     -textvariable => \$trans->{ORIGID})->pack(-side => "top",
                                               -anchor => "nw");

  $left->LabEntry(-label => "Street (for AVS)",
     -labelPack => [-side => "right", -anchor => "w"],
     -width => 20,
     -textvariable => \$trans->{STREET})->pack(-side => "top",
                                               -anchor => "nw");

  $left->LabEntry(-label => "Zip (for AVS)",
     -labelPack => [-side => "right", -anchor => "w"],
     -width => 20,
     -textvariable => \$trans->{ZIP})->pack(-side => "top",
                                            -anchor => "nw");

  $left->LabEntry(-label => "Comment 1",
     -labelPack => [-side => "right", -anchor => "w"],
     -width => 20,
     -textvariable => \$trans->{COMMENT1})->pack(-side => "top",
                                                 -anchor => "nw");

  $left->LabEntry(-label => "Comment 2",
     -labelPack => [-side => "right", -anchor => "w"],
     -width => 20,
     -textvariable => \$trans->{COMMENT2})->pack(-side => "top",
                                                 -anchor => "nw");

  # Now we will create the output section
  $right->Label(-text => 'PNRef')->
                grid(-row => 0, -column => 0,-sticky => 'nw');
  $right->Label(-textvariable => \$results->{PNREF})->
                grid(-row => 0, -column => 1,-sticky => 'nw');
 
  $right->Label(-text => 'Result')->
               grid(-row => 1, -column => 0,-sticky => 'nw');
  $right->Label(-textvariable => \$results->{RESULT})->
               grid(-row => 1, -column => 1,-sticky => 'nw');

  $right->Label(-text => 'Respcode')->
               grid(qw/-row 2 -column 0 -sticky nw/);
  $right->Label(-textvariable => \$results->{RESPCODE})->
               grid(qw/-row 2 -column 1 -sticky nw/);

  $right->Label(-text => 'Resmsg')->
               grid(qw/-row 3 -column 0 -sticky nw/);
  $right->Label(-textvariable => \$results->{RESPMSG})->
               grid(qw/-row 2 -column 1 -sticky nw/);

  $right->Label(-text => 'Authcode')->
               grid(qw/-row 4 -column 0 -sticky nw/);
  $right->Label(-textvariable => \$results->{AUTHCODE})->
               grid(qw/-row 3 -column 1 -sticky nw/);

  $right->Label(-text => 'Errcode')->
               grid(qw/-row 5 -column 0 -sticky nw/);
  $right->Label(-textvariable => \$results->{ERRCODE})->
               grid(qw/-row 4 -column 1 -sticky nw/);

  $right->Label(-text => 'Errmsg')->
               grid(qw/-row 6 -column 0 -sticky nw/);
  $right->Label(-textvariable => \$results->{ERRMSG})->
               grid(qw/-row 5 -column 1 -sticky nw/);

  $right->Label(-text => 'AVSZip')->
               grid(qw/-row 7 -column 0 -sticky nw/);
  $right->Label(-textvariable => \$results->{AVSZIP})->
               grid(qw/-row 6 -column 1 -sticky nw/);

  $right->Label(-text => 'AVSAddress')->
               grid(qw/-row 8 -column 0 -sticky nw/);
  $right->Label(-textvariable => \$results->{AVSADDR})->
               grid(qw/-row 7 -column 1 -sticky nw/);

  ### Bottom panel, both input & output

  $bottom->LabEntry(-label => "(Sent)",
     -labelPack => [-side => "right", -anchor => "w"],
     -width => 96,
     -textvariable => \$trans->{_PARAM})->
     grid(qw/-row 0 -column 0 -sticky nw/);

  $bottom->LabEntry(-label => "(Recvd)",
     -labelPack => [-side => "right", -anchor => "w"],
     -width => 96,
     -textvariable => \$results->{RESP})->
     grid(qw/-row 1 -column 0 -sticky nw/);


  $bottom->Button(-text => 'Process Transaction',
                  -command => \&processTransaction )->
                  grid(qw/-row 2 -column 0 -sticky nesw/);

  # Protect from stray signals
  # You could just call MainLoop() w/o the fancy eval
  while (1) {
    eval MainLoop();   # Start the event processing
  }

  # Will never get here 
  debug ("-MAIN");
}

##############
# saveConfig #
##############
# Save Configuration information to a file.
#
# Note: We are saving this information in a plon text
# file.  This shouldn't be done unless the machine is
# known secure.
#  

sub saveConfig {
  debug("+saveConfig");
  # Types are listed in the dialog widget
  my @types = (["Config Files", '.pcg', 'TEXT'],
               ["All Files", "*"] );

  # Uses standard file dialog for OS
  my $file = $mainwindow->getSaveFile(-filetypes => \@types,
				  -initialfile => 'master',
				  -defaultextension => '.pcg');

  # Write out as a Perl variable list
  open  OUT, ">$file";
  print OUT Dumper $config;
  debug(Dumper $config);
  close (OUT);

  debug("-saveConfig"); 
  return;
}


##############
# loadConfig #
##############
# Load configuration information
# 
sub loadConfig {
  debug("+loadConfig");
  my $file;
    
  # Types are listed in the dialog widget
  my @types = (["Config Files", '.pcg', 'TEXT'],
               ["All Files", "*"] );
  
  $file = $mainwindow->getOpenFile(-filetypes => \@types);

  readConfig($file);

  debug(Dumper $config);
  debug("-loadConfig");
}

##############
# readConfig #
##############
sub readConfig {
  my $file = shift;
  my $VAR1;	# used by Data::Dumper;

  local ($/);   
  undef $/;


  return unless $file;
  return unless -e $file;       # skip if it doesn't exist

  open IN, $file;
  my $textfile  = <IN>;         # slurp
  close (IN);

  # should do a sanity check for bad data, as
  $config = eval $textfile;
}


############
# doConfig #
############
# Create a configuration dialogue

sub doConfig {
  debug("+doConfig");   
  my ($name, $email, $os);

  my $localconfig;

  # Copy all the configuration items to a local array
  foreach (keys %$config) {
    $localconfig->{$_} = $config->{$_};
    debug ($_);
  }

	my $f = $mainwindow->DialogBox(-title => "Configuration",
			     -buttons => ["OK", "Cancel"]);
	my $n = $f->add('NoteBook', -ipadx => 6, -ipady => 6);

        my $vendor_p = $n->add("vendor", -label => "Vendor ID",
                                         -underline => 0);
        my $host_p   = $n->add("host",  -label => "Host",
                                        -underline => 0);
        my $proxy_p  = $n->add("proxy", -label => "Proxy",
                                        -underline => 0);
	
	$vendor_p->LabEntry(-label => "User:             ",
	     -labelPack => [-side => "left", -anchor => "w"],
	     -width => 20,
             -textvariable => \$localconfig->{USER})->
             pack(-side => "top", -anchor => "nw");

	$vendor_p->LabEntry(-label => "Vendor:           ",
	     -labelPack => [-side => "left", -anchor => "w"],
	     -width => 20,
             -textvariable => \$localconfig->{VENDOR})->
             pack(-side => "top", -anchor => "nw");

	$vendor_p->LabEntry(-label => "Partner:          ",
	     -labelPack => [-side => "left", -anchor => "w"],
	     -width => 20,
             -textvariable => \$localconfig->{PARTNER})->
             pack(-side => "top", -anchor => "nw");

    $vendor_p->LabEntry(-label => "Pwd:             ",
	     -labelPack => [-side => "left", -anchor => "w"],
	     -width => 20,
             -textvariable => \$localconfig->{PWD})->
             pack(-side => "top", -anchor => "nw");

	$host_p->LabEntry(-label => "Host:             ",
	     -labelPack => [-side => "left", -anchor => "w"],
	     -width => 50,
             -textvariable => \$localconfig->{HOST})->
             pack(-side => "top", -anchor => "nw");

	$host_p->LabEntry(-label => "Port:              ",
	     -labelPack => [-side => "left", -anchor => "w"],
	     -width => 20,
             -textvariable => \$localconfig->{PORT})->
             pack(-side => "top", -anchor => "nw");

	$proxy_p->LabEntry(-label => "Proxy Address:    ",
	     -labelPack => [-side => "left"],
	     -width => 20,
             -textvariable => \$localconfig->{P_ADDR})->
             pack(-side => "top", -anchor => "nw");

	$proxy_p->LabEntry(-label => "Proxy Port:          ",
	     -labelPack => [-side => "left"],
	     -width => 20,
             -textvariable => \$localconfig->{P_PORT})->
             pack(-side => "top", -anchor => "nw");

	$proxy_p->LabEntry(-label => "Proxy Logon:       ",
	     -labelPack => [-side => "left"],
	     -width => 20,
             -textvariable => \$localconfig->{P_LOGON})->
             pack(-side => "top", -anchor => "nw");

	$proxy_p->LabEntry(-label => "Proxy Password:  ",
	     -labelPack => [-side => "left"],
	     -width => 20,
             -textvariable => \$localconfig->{P_PASSWORD})->
             pack(-side => "top", -anchor => "nw");

	$n->pack(-expand => "yes",
		 -fill => "both",
		 -padx => 5, -pady => 5,
		 -side => "top");
	

    my $result = $f->Show;	    # Execute the dialog box

    if ($result =~ /OK/) {
       # Copy all the configuration items back
       foreach (keys %$config) {
         $config->{$_} = $localconfig->{$_};
       }
      debug(Dumper $config);
    }
    debug("-doConfig"); 
}


######################
# ProcessTransaction #
######################
# Code to  process the transaction
# A more robust system would log each and every transaction
# We just display the results
# 
sub processTransaction {
  my ($name, $value, $data, @vars);
  debug("+processTransaction");
 
  # We will create a local copy of the transaction structure
  my $ltrans;
  foreach $name (keys %$trans) {
    next if $name =~ /_/;	# _VAR doesn't get passed
    $ltrans->{$name} = $trans->{$name};
  }

  # Translate the verbose transaction types to codes
  $ltrans->{TRXTYPE} = $trxtypetranslate->{$ltrans->{TRXTYPE}};

  # support for F (force voice transactions)
  # Munge ORIGID into AUTHCODE
  if ($ltrans->{TRXTYPE} eq 'F') {
    $ltrans->{AUTHCODE} = $ltrans->{ORIGID};
    undef $ltrans->{ORIGID};
  }
  
  # create the data stream from the input paramaters
  my $data = "USER=$config->{USER}&VENDOR=$config->{VENDOR}&PARTNER=$config->{PARTNER}&PWD=$config->{PWD}&";

  foreach $name (keys %$ltrans) {
    $data .= "$name=$ltrans->{$name}&";
  }
  chop $data;

  debug($data);

  # clear the output display
  foreach (keys %$results) {
    $results->{$_} = '';
  }

  # Show user the completed transaction request
  $trans->{_PARAM} = $data;   


  my $result = call_pfpro($config->{HOST},
                           $config->{PORT},
                           $data,
                           30,          # timeout
                           $config->{P_ADDR},
                           $config->{P_PORT},
                           $config->{P_LOGON},
                           $config->{P_PASSWORD} );

  # Create the results list (disply them as a side effect)
  $results->{RESP} = $result;
  (@vars) = split "&", $result;
  foreach (@vars) {
    ($name, $value) = split "=";
    $results->{$name} = $value;
  }

  debug("-processTransaction");
}


#########
# debug #
#########
# Print debugging information to stdout
sub debug {
  my @msg = shift;
  print @msg, "\n" if $DEBUG;
}

