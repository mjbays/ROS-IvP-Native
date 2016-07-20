/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: LogViewLauncher.h                                    */
/*    DATE: May 31st, 2005                                       */
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

#ifndef LOGVIEW_LAUNCHER
#define LOGVIEW_LAUNCHER

#include <vector>
#include "REPLAY_GUI.h"
#include "ALogDataBroker.h"

class LogViewLauncher
{
 public:
  LogViewLauncher();
  virtual ~LogViewLauncher() {}
  
  REPLAY_GUI *launch(int argc, char **argv);

protected:
  bool parseCommandArgs(int argc, char **argv);
  bool sanityCheck();

  bool configDataBroker();
  bool configGraphical();

  bool handleMinTime(std::string);
  bool handleMaxTime(std::string);
  bool handleBackground(std::string);
  bool handleGeometry(std::string);
  bool handleInitialLogPlotL(std::string);
  bool handleInitialLogPlotR(std::string);
  bool handlePanX(std::string);
  bool handlePanY(std::string);
  bool handleZoom(std::string);
  bool handleNowTime(std::string);


private:
  std::vector<std::string> m_alog_files;
  
  std::string  m_tiff_file;
  std::string  m_alt_nav_prefix;
  std::string  m_start_veh_lft;
  std::string  m_start_var_lft;
  std::string  m_start_veh_rgt;
  std::string  m_start_var_rgt;
  double       m_start_panx;
  double       m_start_pany;
  double       m_start_zoom;
  double       m_start_time;
  bool         m_quick_start;
  bool         m_min_time_set;
  bool         m_max_time_set;
  double       m_min_time;
  double       m_max_time;
  double       m_gui_height;
  double       m_gui_width;

  ALogDataBroker m_dbroker;

  REPLAY_GUI *m_gui;
};

#endif 
