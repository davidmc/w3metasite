#!/usr/local/bin/perl
#######################
# signiodemo.cgi 
# (c)1999 Verisign
# All Rights Reserved
#
# Assumptions:
#  Module PFProAPI has been installed & configured properly
#  Module Data::Dumper is available
#  Module CGI is available
#
# This example uses a session ID based on time & IP address
# It is best to use cookies, as this can fail when accessed
# through some proxy servers like AOL. 
# Cookies are not implemented here.
#
# A real application would expire the session database as well,
# so that it doesn't grow unreasonably.  Note that potentially
# private information is stored as well.
#
# Note on Telecheck Support: Telecheck changes it's required 
# verbiage # from time to time as laws and rates change.  You
# may need to modify the HTML displayed to be compliant with
# the latest version of the Telecheck specifications.
#

BEGIN {
    $| = 1;
    print "content-type: text/html\n\n";
#    $DEBUG = 100;
    unshift (@INC, ".");
    if ( $^O ne "MSWin32" ) {
      use CGI::Carp qw(carpout);
      carpout(STDOUT);
    }
}


### CONFIGURATION ##

# Move all files to your web-accessible directory.  You will then need
# to make sure that /usr/bin/perl exists, and that this file has been
# set to be executable by your webserver (chmod 0755 signiodemo.cgi)
# If you must run cgi-scripts from a cgi-bin directory, move this file
# (signiodemo.cgi) there now.  Extra configuration will be required.

# WINDOWS NT USERS: You will either need to tell IIS that .cgi scripts
# are exected using Perl either by associating the file type .cgi with
# Perl or editting the script map.  See your webserver documentation for
# details.  You may also choose to rename this script to signiodemo.pl

# Make sure that you edit / install the appropriate Perl client for your
# system.  See PFProAPI.pm in the apropriate client distribution for more
# information.

# You must register with Verisign at http://www.verisign.com/payment for 
# a trial account.
# Once you have your username and password, enter them here.  It generally
# takes about 1 hour from the time of registration for it to become active
# so now is a good time to register :) This will allow you to view all your
# test transactions.

my $USER =    "";
my $VENDOR =  "";
my $PARTNER = "";
my $PWD  =    "";


# Transaction type.  Either 'A' (authorization only) or 'S' (Sale)
my $TRXTYPE = 'S';

# Logfile directory
# We need to store users session data and logs in a directory that is 
# *not* accessible from the webserver.  Enter that directory here
# and make sure that it exists.
# Example (UNIX): /home/mydir/verisign/logfiles/
# Example (NT): c:\\mydir\\verisign\\logfiles\\
# Note trailing slash (required)
# Note the use of double backslash "\\" for NT
my $logDir = "";

# Point this to your sendmail routine to send/receive notification emails
# Example: "/usr/sbin/sendmail";
# Windows NT users:  You must install a sendmail - compatible mailer for this to work you can 
# get BLAT from: http://pages.infinit.net/che/blat/blat.html
$sendmail = "";

# Email orders to
# Example:  'orders@yourcompany.com';
$email = "";

# Email orders to customers - true (1) or false (0)
$emailcustomerorders = 0;		# 1 to enable

# If you have a webserver that allows you to run Perl CGI scripts from your 
# default web directory (usually with an extension of CGI) you can skip the 
# rest of this section.  Otherwise, we need to know something about your 
# web server configuration

# Set html dir to the full path of your html directory.  
# This is where you have copied all the HTML files and images
# Example (UNIX): $htmlDir = /home/www/htdocs/cgidemo/
# Example (NT): $htmlDir = c:\\Inetpub\\wwwroot\\cgidemo\\
# Note trailing slash
# Note the use of double backslash "\\" for NT
my $htmlDir = "";

# Set the html base URL to be used. This should correspond to the physical
# directory above
# example http://www.mycompany.com/cgidemo/
# Note trailing slash
my $htmlBase = "";

## END CONFIGURATION ##

use PFProAPI qw(pfpro);
use Data::Dumper;
use CGI;

# Global Parameters
my $CGILOG      = "$logDir/cgi.log";
my $TRXLOG      = "$logDir/trx.log";
my $ORDLOG      = "$logDir/ord.log";

my $STARTPAGE    = $htmlDir."billing.html";
my $CREDITPAGE   = $htmlDir."credit.html";
my $CHECKPAGE    = $htmlDir."check.html";
my $CONFIRMPAGE  = $htmlDir."confirmation.html";
my $SESSIONFILE  = $logDir."session.dbm";

my $SUCCESSPAGE = "success.html";
my $ERRORPAGE   = "error.html";
my $ORDERMSG    = "neworder.msg";
my $CONFIRMMSG  = "confirm.msg";

my $SESSION;	# Hasref of all session variables


$tcmsg0 = <<EOA
   <img src="Telelogo.gif" align="left" hspace="10" WIDTH="89" HEIGHT="59"><br>
    <font face="Arial" size="4"><p></font><strong><font face="Arial">Data Entry or
    Communications Error<br clear="all">
    </font></strong><font face="Arial" size="4"></p>
    </font><p><strong><font face="Arial" size="5">W</font></strong><font face="Arial" size="2">e
    are unable to process the information as entered. Please make certain that the information
    you entered is correct and submit it again, or select another payment option at this time.
    </p>
EOA
;		

# These are REQUIRED telecheck messages.  You may re-format them to fit your site, however
# the wording cannot be changed.
		
#Internet Code 1 Verbiage - Displayed upon Approval of Transaction 
$tcmsg1 = <<EOB
    <img src="Telelogo.gif" align="left" hspace="10" WIDTH="89" HEIGHT="59"><br>
    <font face="Arial" size="4"><p></font><strong><font face="Arial">Transaction was Approved<br
    clear="all">
    </font></strong><font face="Arial" size="4"></p>
    <p></font><font face="Arial" size="5"><strong>T</strong></font><font face="Arial"
    size="2">hank you for selecting checks as your preferred payment option. TeleCheck has
    approved this transaction, and your&nbsp; checking account will be debited electronically
    through the regular banking system. If for some reason this transaction cannot be
    electronically processed, a paper draft will be created to debit the amount of this
    transaction from your account. TeleCheck will confirm this transaction by written
    notification either by email or the postal service. For more information on TeleCheck's
    process see <a href="TeleLnk2_FAQ.htm">Internet Check FAQ</a></font> </p>
    <i><font size="1"><p></font><font face="Arial" size="1">Under the Fair Credit Reporting
    Act, consumers have the right to a free copy of the consumer&#146;s information held in
    TeleCheck&#146;s files for a period of 60 days following an adverse action. Consumers also
    may dispute the accuracy or completeness of any information in TeleCheck&#146;s consumer
    report. TeleCheck did not make the decision to take an adverse action (i.e., not accept
    the check) and is unable to provide you with the specific reason(s) why an adverse action
    was taken.</font></i>
EOB
;

#Data Entry Error Verbiage - Displayed upon return of Entry Error Code 
$tcmsg2 = <<EOC
    <img src="Telelogo.gif" align="left" hspace="10" WIDTH="89" HEIGHT="59"><br>
    <font face="Arial" size="4"><p></font><strong><font face="Arial">Data Entry Error<br
    clear="all">
    </font></strong><font face="Arial" size="4"></p>
    </font><p><strong><font face="Arial" size="5">W</font></strong><font face="Arial" size="2">e
    are unable to process the information as entered. Please make certain that the information
    you entered is correct and submit it again, or select another payment option at this time.
    </font>
EOC
;

 # Code 3 Verbiage - Displayed upon return of Code 3 Decline	
 $tcmsg3 = <<EOD
	   <img src="Telelogo.gif" align="left" hspace="10" WIDTH="89" HEIGHT="59"><br>
    <font face="Arial" size="4"><p></font><strong><font face="Arial">Sorry, TeleCheck cannot
    approve this transaction<br clear="all">
    </font></strong><font face="Arial" size="4"></p>
    <p></font><font face="Arial" size="5"><strong>T</strong></font><font face="Arial" size="2">hank
    you for selecting checks as your preferred payment option. Unfortunately, TeleCheck cannot
    approve this check transaction at this time. Although there is no negative information in
    TeleCheck&#146;s files, this check transaction has exceeded TeleCheck&#146;s established
    guidelines. TeleCheck&#146;s check transaction acceptance decisions are made utilizing a
    complex system that evaluates various criteria for each transaction. This proprietary
    system enables TeleCheck&#146;s merchants to minimize the risk associated with accepting
    checks, and is key to preserving checks as a valid payment option. For this reason,
    TeleCheck does not disclose the components of this system. If you require more information
    about TeleCheck, please call 1-800-697-9264, or write TeleCheck Checkwriter Services at
    P.O. Box 4513 Houston, TX 77210-4513, click here to access <a
    href="http://www.telecheck.com/ica/internetevalfaq.html">TeleCheck&#146;s Evaluation
    Process FAQ&#146;s</a>. </font></p>
    <i><font size="1"><p></font><font face="Arial" size="1">Under the Fair Credit Reporting
    Act, consumers have the right to a free copy of the consumer&#146;s information held in
    TeleCheck&#146;s files for a period of 60 days following an adverse action. Consumers also
    may dispute the accuracy or completeness of any information in TeleCheck&#146;s consumer
    report. TeleCheck did not make the decision to take an adverse action (i.e., not accept
    the check) and is unable to provide you with the specific reason(s) why an adverse action
    was taken.</font></i>
EOD
;

#Internet Code 4 Verbiage - Displayed upon return of Code 4 Decline
$tcmsg4 = <<EOE
    <img src="Telelogo.gif" align="left" hspace="10" WIDTH="89" HEIGHT="59"><br>
    <font face="Arial" size="4"><p></font><strong><font face="Arial">Sorry, TeleCheck cannot
    approve this transaction<br clear="all">
    </font></strong><font face="Arial" size="4"></p>
    </font>
				<P>
				   <font face="Arial" size="5"><strong>T</strong></font><font face="Arial" size="2">hank
        you for selecting checks as your preferred payment option. Unfortunately, TeleCheck cannot
        approve this check transaction at this time. TeleCheck has information in its files that
        must be resolved before check writing privileges can be restored at any of its 200,000
        merchant locations. Please call TeleCheck&#146;s Check Writer Services department, 24
        hours a day at 1-800-697-9264 for more information or write TeleCheck Checkwriter Services
        at P.O. Box 4513 Houston, TX 77210-4513.</font> <P>
        <br>
        <font face="Arial" size="2">Please have the following information ready so TeleCheck may
        respond promptly to your request for more information:<br>
        </font><ol>
          <li><font face="Arial" size="2">Your full name and its spelling. </font></li>
          <li><font face="Arial" size="2">The name of your company, if applicable. </font></li>
          <li><font face="Arial" size="2">Your driver&#146;s license number and the state where it was
            issued. </font></li>
          <li><font face="Arial" size="2">The banking numbers printed along the bottom of your check. </font></li>
          <li><font face="Arial" size="2">Your complete mailing address including ZIP code. </font></li>
          <li><font face="Arial" size="2">Your daytime telephone number with area code. </font></li>
          <li><font face="Arial" size="2">The name of the merchant that referred you to TeleCheck. </font></li>
        </ol>
        <P>
    <i><font size="1"><p></font><font face="Arial" size="1">Under the Fair Credit Reporting
    Act, consumers have the right to a free copy of the consumer&#146;s information held in
    TeleCheck&#146;s files for a period of 60 days following an adverse action. Consumers also
    may dispute the accuracy or completeness of any information in TeleCheck&#146;s consumer
    report. TeleCheck did not make the decision to take an adverse action (i.e., not accept
    the check) and is unable to provide you with the specific reason(s) why an adverse action
    was taken.</font></i>
EOE
;


MAIN: {
  $query = new CGI;
  if ($DEBUG > 90) {
    print ("<PRE>\n");
    $query->save(STDOUT) if $DEBUG > 90;
    print ("</PRE>\n");
  }

  my $action     = $query->param('action');
  debug(10, $action);
  my $sessionid  = $query->param('SESSIONID');

  if ($sessionid eq "") {
    # First request, create a new session ID
    $sessionid = $ENV{REMOTE_ADDR} . "-".time;
    debug(10, "New Session");
  }
  debug(10, "Session ID: $sessionid");
  
  # open the session database
  dbmopen(%sessionfile, $SESSIONFILE, 0600);
  my $sessioninfo = $sessionfile{$sessionid};
  dbmclose(%sessionfile);

  debug(90, "Session Info: $sessioninfo");
  $SESSION = eval $sessioninfo ;
  debug(90, Dumper $SESSION);

  $SESSION->{SESSIONID} = $sessionid;	
  $SESSION->{STARTTIME} = time;
  $SESSION->{SCRIPTURL} = $ENV{SCRIPT_NAME} || $ENV{SCRIPT_URL};  
  $SESSION->{BASE} = "<BASE HREF=$htmlBase>" if $htmlBase;

  $SESSION->{'AMT'} = "27.23";

  my $page;
  SWITCH: {
    if    ($action eq 'collectpayment') { $page = collectpayment(); }
    elsif ($action eq 'processcredit')  { $page = processcredit();  }
    elsif ($action eq 'processcheck')   { $page = processcheck();   }
    else  {
      # We don't have an action, print the firstpage
      $page = readFile($STARTPAGE);
    }
  }

  # Store the session info
  $SESSION->{ENDTIME} = time;
  $sessioninfo = Dumper ($SESSION);

  # We should file lock this
  dbmopen(%sessionfile, $SESSIONFILE, 0600);
  $sessionfile{$sessionid} = $sessioninfo;
  dbmclose(%sessionfile);

  # print the page
  $page = substVars($SESSION, $page);
  $page = clearVars($page);
  print $page;
  exit(0)
}


##################
# collectpayment #
##################
sub collectpayment {
  debug(10, "+collectpayment");

  my @names =  $query->param;
  foreach (@names) {
    $SESSION->{$_} = $query->param($_);		# store
  }

  if ($query->param('TENDER') eq 'check') {
    $ret = readFile($CHECKPAGE);
  } else {
    $ret = readFile($CREDITPAGE);
  }
  debug(10, "-collectpayment");
  return $ret;
}

#################
# processcredit #
#################
sub processcredit {  
  debug(10, "+processcredit");
  $param->{AMT}     = $SESSION->{'AMT'};
  $param->{STREET}  = $SESSION->{'STREET'};
  $param->{ZIP}     = $SESSION->{'ZIP'};
  $param->{ACCT}    = $query->param('ACCT');
  $param->{EXPDATE} = $query->param('exp_mm') . $query->param('exp_yyyy');
  $param->{USER}    = $USER;
  $param->{VENDOR}  = $VENDOR;
  $param->{PARTNER} = $PARTNER;
  $param->{PWD}     = $PWD;
  $param->{TENDER}  = "C";
  $param->{TRXTYPE} = $TRXTYPE;		


  debug(20, Dumper $param);
  ($result) = pfpro($param); 
  debug(20, Dumper $result);

  $result->{MSG} = qq(
    <font face="Arial" size="5">W</font></strong><font face="Arial" size="2">e
    are unable to process the information as entered. Please make certain that the information
    you entered is correct and submit it again, or select another payment option at this time.
    </p> $result->{RESPMSG} 
  ) if $result->{RESULT} != 0;


  debug(10, "-processcredit");
  return substVars($result, readFile($CONFIRMPAGE));

}

#################
# process check #
#################
sub processcheck {
  debug(10, "+processcheck");

  $param->{AMT}     = $SESSION->{'AMT'};
  $param->{USER}    = $USER;
  $param->{VENDOR}  = $VENDOR;
  $param->{PARTNER} = $PARTNER;
  $param->{PWD}     = $PWD;
  $param->{TENDER}  = 'K';
  $param->{TRXTYPE} = 'S';		
  $param->{DOB}     = $query->param('DOBM') .  $query->param('DOBD') . $query->param('DOBY');
  $param->{NAME}    = $query->param('name');
  $param->{STREET}  = $query->param('street');
  $param->{CITY}    = $query->param('city');
  $param->{ZIP}     = $query->param('zip');
  $param->{STATE}   = $query->param('state');
  $param->{DL}	    = $query->param('dlnum');
  $param->{EMAIL}   = $query->param('email');
  $param->{PHONENUM}= $query->param('PHONENUM');
  $param->{MICR}    = $query->param('MICR');
  $param->{CHKNUM}  = $query->param('CHKNUM');
  $param->{COMMENT1} = "Verisign Test Transaction";
  $param->{COMMENT2} = "SessionID: $sessionid";

  debug(20, Dumper $param);
  ($result) = pfpro($param); 

  $result->{MSG} = ($result->{RESULT} != 0 ? msg($result->{'ERRCODE'}) : $result->{RESPMSG});

  debug(20, Dumper $result);


  debug(10, "-processcheck");
  return substVars($result, readFile($CONFIRMPAGE));
}


### General Subroutines ####

############
# readFile #
############
# Read a local file (quickly)
# IN:     $filename   Full path of filename to read
# OUT:    $page       The page or "" if error

sub readFile {
  my $filename = shift;
  my $page;
  local ($/, *READFILE);

  debug(999, "+readFile $filename");
  if (open(READFILE, $filename)) {
    undef $/;
    $page = <READFILE>;		# Slurp
    close(READFILE);
  }
  debug(999, "-readFile $filename");
  return $page;
}


###########
# logdata #
###########
#  IN: $logfile	Full path to file
#      $data	Reference to variable to store
# OUT: Nothing

sub logdata {
  my $logfile = shift;
  my $data    = shift;
  my $logdata = $OID . ":" . time . ":" .  Dumper ($data);

  open LOG, ">>$logfile";
  print LOG $logdata;
  close (LOG);
}


#############
# sendemail #
#############
#  IN: param hashref of variables to substitute
# OUT: template file to use

sub sendemail {
  my $param = shift;
  my $file = shift;

  unless ($sendmail && -e $sendmail) {
    debug(1, "sendemail called with no sendmail defined!");
    return; 
  }
  debug(10, "+sendemail $file");

  my $template = readFile($file);
  my $msg = substVars($param, $template);
  debug(10, $msg);
  
  open  MAIL, "| $sendmail -i -t -U";
  print MAIL "$msg\n\n";
  close MAIL;
  debug(10, "-sendemail $file");
}


#########
# debug #
#########
# IN : $level   Debug Level
#      $msg     Message To Display
# OUT: $msg written to standout if the current DEBUG level is greater than
#      level specified
# GLOBALS:  $main::DEBUG;#
# NOTES
#      Eventually we want to add "sublevels" to the code, so that we can set
#      a "range" switch.
sub debug {
  my $level = shift;
  my $msg = shift;
  print("$msg<BR>\n") if $main::DEBUG > $level;
}

#############
# substVars #
#############
# Do macro substitutions
# IN : $hash  ref to hash of variables
#      $page  page on which to perform substitutions
# OUT: $page fully interpolated with hash
# NOTES: Page should contain macros of the form #VAR#
#        Convention is to use all uppercase, though not required / enforced
#        vars must be alpha only
sub substVars {
  my $hash = shift;
  my $page = shift;
  my $item;

  foreach $item (keys %{$hash}) {
    $page =~ s/#$item#/$hash->{$item}/ig;
  }
  return $page;
}

#############
# clearVars #
#############
# Delete all macros on a page
# IN : $page  page on which delete macros
# OUT: $page  with no macros
sub clearVars {
  my $page = shift;
  $page =~ s/\#[\w]*\#//g;             # strip out all the #MACROS# left
  return $page;
}


####### Telecheck Verbiage ############

#######
# msg #
#######
# Return appropriate verbiage based on telecheck result code

sub msg {
        my $code = shift;
debug (10, "+msg $code\n");
	return $tcmsg1 if $code eq '000800';
	return $tcmsg1 if $code eq '000801';
	return $tcmsg1 if $code eq '000802';
 	return $tcmsg1 if $code eq '000803';
 	return $tcmsg2 if $code eq '000807';
 	return $tcmsg3 if $code eq '000805';
 	return $tcmsg4 if $code eq '000804';
 	return $tcmsg4 if $code eq '000806';
 	return $tcmsg0; # unknown
# Will never get here 
debug (10, "-msg $code\n");
}


1;
