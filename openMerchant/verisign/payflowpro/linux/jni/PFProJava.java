// Verisign, Inc.
// http://www.verisign.com
// 1600 Bridge Parkway
// Suite 201
// Redwood City, CA 94065
// USA
//
// (888) 883-9770
// vps-support@verisign.com

// Verisign Payflow Pro Java Client
import com.Verisign.payment.PFProAPI;

class PFProJava {

	// Set defaults
	static String HostAddress   = "test-payflow.verisign.com";
	static Integer HostPort = Integer.decode("443");
	static String ParmList      = "";
	static Integer Timeout = Integer.decode("80");
	static String ProxyAddress  = "";
	static Integer ProxyPort = Integer.decode("0");
	static String ProxyLogon    = "";
	static String ProxyPassword = "";

	// Help system
	static void help()
	{
		System.out.println( "Usage Error: \n" );

		System.out.println("pfpro <hostAddress> <hostPort> <parmList> <timeout> <proxyAddress> <proxyPort> <proxyLogon> <proxyPassword>");
		System.out.println("<hostAddress>   host name                   'test-payflow.verisign.com'");
		System.out.println("<hostPort>      host port number            '443'");
		System.out.println("<parmList>      parameter list              'ccNum=5105105105105105100&ccExpDate=1299&amount=1.23'");
		System.out.println("<timeOut>       timeout(sec) - optional     '30'");
		System.out.println("<proxyAddress>  proxy name - optional       'proxy'");
		System.out.println("<proxyPort>     proxy port - optional       '8080'");
		System.out.println("<proxyLogon>    proxy logon name - optional 'admin'");
		System.out.println("<proxyPassword> proxy password - optional   'password'");
	}

	public static void main(String[] args)
	{
		PFProAPI pn = new PFProAPI();

		// Check args, at least the first 3 must be there
		if (args.length < 3) {
			System.out.println( "\nPFPRO " + pn.Version() );

			try {
				if ( args[0].equalsIgnoreCase(new String("-version")) ) {
					System.out.println();
					return;
				}
			} catch (Exception e) { }

			help();
			return;
		}

		// Place the arguments in the correct variables
		// Once we get an OutOfBounds exception, parsing will stop and the rest will
		// retain their default values.
		try {
			HostAddress   = args[0];
			HostPort	  = Integer.decode(args[1]);
			ParmList      = args[2];
			Timeout		  = Integer.decode(args[3]);
			ProxyAddress  = args[4];
			ProxyPort	  = Integer.decode(args[5]);
			ProxyLogon    = args[6];
			ProxyPassword = args[7];
		} catch (Exception e) { }

		// Call the client.
		pn.CreateContext(HostAddress,
                         HostPort.intValue(),
                         Timeout.intValue(),
                         ProxyAddress,
                         ProxyPort.intValue(),
                         ProxyLogon,
                         ProxyPassword);

		String rc = pn.SubmitTransaction(ParmList);

		System.out.println(rc);

		pn.DestroyContext();
	}
}

