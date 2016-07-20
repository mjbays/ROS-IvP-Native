README-OS-X.txt
Last updated: 21 July 2014
Maintainer: Mike Benjamin (issues@moos-ivp.org)

==============================================================================
OVERVIEW
==============================================================================
This file gives OS X-specific steps for building and running 
MOOS-IvP software.

==============================================================================
IMPORTANT INFORMATION
==============================================================================
As of June 19th 2012: 
  1. We have ceased to test our build system on pre-Lion systems.
  2. We have ceased to include FLTK as part of the MOOS-IvP tree. It is now
     a package dependency (port install fltk-devel).

==============================================================================
NEEDED APPLICATIONS
==============================================================================

1) XCODE
   You need to install the XCode development package for OS-X available 
   at http://developer.apple.com/technology/xcode.html. It is a free download.

2) X11
   Apple no longer includes X11, but explains why and offers help
      http://support.apple.com/en-us/HT201341
   As of Nov 2014, this is where you get/install X11:
      http://xquartz.macosforge.org/landing/

3) MacPorts
   Website: http://www.macports.org

   MacPorts is an open-source project that provides an easy-to-use system for
   compiling, installing, and upgrading common packages. Instructions for 
   downloading and installing MacPorts can be found at the following address:

      http://www.macports.org/install.php

   It is recommended that you use the Package Installer. By default, this will
   install MacPorts into the /opt/local directory and add /opt/local/bin to 
   the default user's PATH environment variable. More details on environment
   variables can be found below.

==============================================================================
MACPORT PACKAGES
==============================================================================

sudo port install cmake
sudo port install fltk-devel
sudo port install tiff
sudo port install subversion


NOTE: We are no longer including FLTK as part of the  MOOS-IvP tree. If you 
      have trouble finding FLTK 1.3 in your system's package manager, we 
      recommend that you install from the source code from the FLTK website.


BUILDING MOOS-IvP
=================
  ./build-moos.sh
  ./build-ivp.sh


ENVIRONMENT VARIABLES
=====================
When you build the MOOS-IvP software, the executable programs are placed
in the "moos-ivp/bin" subdirectory of the source code tree.

We recommend that you put the absolute path to this directory into
your PATH environment variable.  This is especially important because the
"pAntler" program, which can launch other MOOS/IvP programs, relies on the
PATH variable to find those programs.

We normally just add lines to our ~/.bashrc or ~/.cshrc files to always append
these two directories to the PATH environment variable.

*** NOTE: On OS X, ~/.bashrc isn't automatically executed, as it is on Linux.
*** However, ~/.bash_profile *is* automatically executed.


