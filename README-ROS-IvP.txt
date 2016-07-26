README-ROS-IvP.txt
Last updated: 17 June 2015


OVERVIEW
======== 
This file provides instructions for compiling and running ROS-IvP on Linux systems.


BUILDING ROS-IvP
=================

Copy the ros_moos_msgs folder into your ros catkin source directory and follow the 
procedures for building a ROS package.

Then follow the instructions on the wiki for installing the dependencies of MOOS-IvP
and then run
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

WARNING
=======
The executable programs of ROS-IvP currently maintain the names of the executables
of MOOS-IvP so if you currently have a separate MOOS-IvP install remove its directories
from your PATH variable so that the correct versions of the programs are run.

RUNNING ROS-IVP
===============
Start the roscore process

edit the .moos file for a mission so that the MOOSDB and pLogger are not started

Start the mission the same way you would start it as a MOOS mission.

CURRENTLY UNSUPPORTED FEATURES
==============================
Wildcard registration

Binary data messages

NOTES
=====
ROS-IvP has currently been tested only on missions from the ivp/missions folder
Missions where multiple MOOSDB are running have only been implemented using pShare
and may not work correctly with pMOOSBridge.
