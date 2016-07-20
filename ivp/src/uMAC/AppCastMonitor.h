/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: AppCastMonitor.h                                     */
/*    DATE: June 4th 2012                                        */
/*                                                               */
/* This file is part of MOOS-IvP                                 */
/*                                                               */
/* MOOS-IvP is free software: you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation, either version  */
/* 3 of the License, or (at your option) any later version.      */
/*                                                               */
/* MOOS-IvP is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty   */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See  */
/* the GNU General Public License for more details.              */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with MOOS-IvP.  If not, see                     */
/* <http://www.gnu.org/licenses/>.                               */
/*****************************************************************/

#ifndef APPCAST_MONITOR_HEADER
#define APPCAST_MONITOR_HEADER

#include <string>
#include <map>
#include "MOOS/libMOOS/MOOSLib.h"
#include "AppCastRepo.h"

class AppCastMonitor : public CMOOSApp
{
 public:
  AppCastMonitor();
  ~AppCastMonitor() {}
  
  void handleCommand(char);
  void setTerseMode(bool v) {m_terse_mode=v;}
  
 protected:
  bool OnNewMail(MOOSMSG_LIST &NewMail);
  bool Iterate();
  bool OnConnectToServer();
  bool OnStartUp();
  void RegisterVariables();

 protected:
  bool handleMailAppCast(const std::string&);
  void handleSelectNode(const std::string&);
  void handleSelectChannel(const std::string&);
  void postAppCastRequest(const std::string& node, 
			  const std::string& app,
			  const std::string& key,  
			  const std::string& thresh,
			  double duration);

  void setCurrentNode(const std::string& s)   {m_repo.setCurrentNode(s);}
  void setCurrentProc(const std::string& s)   {m_repo.setCurrentProc(s);}
  bool switchContentMode(const std::string& s="revert");

  void printHelp();
  void printReportNodes();
  void printReportProcs();
  void printReportAppCast();
  void printHeader();

  std::string currentNode() const;
  std::string currentProc() const;

 private: // Configuration variables

  std::string  m_refresh_mode;      // paused,events,streaming
  std::string  m_content_mode;      // help,nodes,procs,appcast 
  std::string  m_content_mode_prev;

  bool         m_terse_mode;
  
 private: // State variables
  unsigned int m_term_reports;
  unsigned int m_iteration;
  double       m_timewarp;  
  double       m_curr_time;
  double       m_last_report_time;
  double       m_term_report_interval;
  bool         m_update_pending;
  
  AppCastRepo  m_repo;
};

#endif 





