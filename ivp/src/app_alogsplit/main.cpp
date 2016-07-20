/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: main.cpp                                             */
/*    DATE: Feb 2nd, 2015                                       */
/*****************************************************************/

#include <string>
#include <cstdlib>
#include <iostream>
#include "MBUtils.h"
#include "ReleaseInfo.h"
#include "SplitHandler.h"

using namespace std;

//--------------------------------------------------------
// Procedure: main

int main(int argc, char *argv[])
{
  // Look for a request for version information
  if(scanArgs(argc, argv, "-v", "--version", "-version")) {
    showReleaseInfo("alogsplit", "gpl");
    return(0);
  }
  
  // Look for a request for usage information
  if(scanArgs(argc, argv, "-h", "--help", "-help")) {
    cout << "Usage: " << endl;
    cout << "  alogsplit in.alog [OPTIONS]                              " << endl;
    cout << "                                                           " << endl;
    cout << "Synopsis:                                                  " << endl;
    cout << "  Split the given alog file into a directory, within which " << endl;
    cout << "  each MOOS variable is split into its own (klog) file     " << endl;
    cout << "  containing only that variable. The split will also create" << endl;
    cout << "  a summary.klog file with summary information.            " << endl;
    cout << "                                                           " << endl;
    cout << "  Given file.alog, file_alvtmp/ directory will be created. " << endl;
    cout << "  Will not overwrite directory if previously created.      " << endl;
    cout << "  This is essentially the operation done at the outset of  " << endl;
    cout << "  launching the alogview applicaton.                       " << endl;
    cout << "                                                           " << endl;
    cout << "Standard Arguments:                                        " << endl;
    cout << "  in.alog  - The input logfile.                            " << endl;
    cout << "                                                           " << endl;
    cout << "Options:                                                   " << endl;
    cout << "  -h,--help      Displays this help message                " << endl;
    cout << "  -v,--version   Displays the current release version      " << endl;
    cout << "  --verbose      Show output for successful operation      " << endl;
    cout << "  --dir=DIR      Override the default dir with given dir.  " << endl;
    cout << "                                                           " << endl;
    cout << "Further Notes:                                             " << endl;
    cout << "  (1) The order of arguments is irrelevent.                " << endl;
    cout << "  (2) See also: alogscan, alogrm, aloggrep, alogclip, alogview " << endl;
    cout << endl;
    return(0);
  }

  string alogfile_in;
  string given_dir;

  bool verbose = false;
  for(int i=1; i<argc; i++) {
    string sarg = argv[i];
    if(strEnds(sarg, ".alog")) {
      if(alogfile_in == "")
	alogfile_in = sarg;
      else {
	cout << "Only one alog file allowed." << endl;
	exit(1);
      }
    }
    else if(sarg == "--verbose")
      verbose = true;
    else if(strBegins(sarg, "--dir=")) 
      given_dir = sarg.substr(6);
  }
  
  if(alogfile_in == "") {
    cout << "No alog file given - exiting" << endl;
    exit(1);
  }
  
  SplitHandler handler(alogfile_in);
  handler.setVerbose(verbose);
  handler.setDirectory(given_dir);
  
  bool handled = handler.handle();
  if(!handled)
    return(1);

  return(0);
}






