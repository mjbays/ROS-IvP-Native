/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: PokeDB.cpp                                           */
/*    DATE: May 9th 2008                                         */
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

#include "PokeDB.h"
#include "MBUtils.h"

using namespace std;

extern bool MOOSAPP_OnConnect(void*);
extern bool MOOSAPP_OnDisconnect(void*);

//------------------------------------------------------------
// Constructor

PokeDB::PokeDB()
{
  m_db_start_time = 0; 
  m_iteration     = 0; 
  m_sServerHost   = "localhost"; 
  m_lServerPort   = 9000;

  m_configure_comms_locally = false;
}

//------------------------------------------------------------
// Constructor

PokeDB::PokeDB(string g_server_host, long int g_server_port)
{
  m_db_start_time = 0; 
  m_iteration     = 0; 
  m_sServerHost   = g_server_host; 
  m_lServerPort   = g_server_port;

  m_configure_comms_locally = false;
}

//------------------------------------------------------------
// Procedure: ConfigureComms
//      Note: Overload the MOOSApp::ConfigureComms implementation
//            which would have grabbed the port/host info from the
//            .moos file instead.

bool PokeDB::ConfigureComms()
{
  //cout << "PokeDB::ConfigureComms:" << endl;
  //cout << "  m_sServerHost: " << M_Sserverhost << endl;
  //cout << "  m_lServErport: " << m_lServerPort << endl;

  if(!m_configure_comms_locally) 
    return(CMOOSApp::ConfigureComms());

  //register a callback for On Connect
  m_Comms.SetOnConnectCallBack(MOOSAPP_OnConnect, this);
  
  //and one for the disconnect callback
  m_Comms.SetOnDisconnectCallBack(MOOSAPP_OnDisconnect, this);
  
  //start the comms client....
  if(m_sMOOSName.empty())
    m_sMOOSName = m_sAppName;
  
  m_nCommsFreq = 10;

  m_Comms.Run(m_sServerHost.c_str(), 
	      m_lServerPort,
	      m_sMOOSName.c_str(), 
	      m_nCommsFreq);
  
  return(true);
}

//------------------------------------------------------------
// Procedure: Iterate()

bool PokeDB::Iterate()
{
  m_iteration++;

  // Make sure we have a chance to receive and read incoming mail
  // on the poke variables, prior to having their new values poked.
  if(m_iteration == 1)
    return(true);
  
  // After the first iteration, poke the all the scheduled values.
  if(m_iteration == 2) {
    unsigned int i, vsize = m_varname.size();
    for(i=0; i<vsize; i++) {
      string varval = m_varvalue[i];
      if(strContains(varval, "@MOOSTIME")) {
	double curr_time = MOOSTime();
	string stime = doubleToStringX(curr_time, 2);
	varval = findReplace(varval, "@MOOSTIME", stime);
      }
      if(m_valtype[i] == "double")
	Notify(m_varname[i], atof(varval.c_str()) );
      else 
	Notify(m_varname[i], varval);
    }  
  }

  if(m_iteration > 3)
    exit(0);

  if(m_iteration == 2)
    printf("\n\nPRIOR to Poking the MOOSDB \n");
  else if(m_iteration == 3)
    printf("\n\nAFTER Poking the MOOSDB \n");

  printReport();
  return(true);
}

//------------------------------------------------------------
// Procedure: OnNewMail()

bool PokeDB::OnNewMail(MOOSMSG_LIST &NewMail)
{    
  MOOSMSG_LIST::iterator p;

  // First scan the mail for the DB_UPTIME message to get an 
  // up-to-date value of DB uptime *before* handling other vars
  if(m_db_start_time == 0) {
    for(p=NewMail.begin(); p!=NewMail.end(); p++) {
      CMOOSMsg &msg = *p;
      if(msg.GetKey() == "DB_UPTIME") 
	m_db_start_time = MOOSTime() - msg.GetDouble();
      else if(msg.GetKey() == "DB_TIME") 
	m_db_time = msg.GetDouble();
    }
  }
  
  // Update the values of all variables we have registered for.  
  // All variables "values" are stored as strings. We let MOOS
  // tell us the type of the variable, and we keep track of the
  // type locally, just so we can put quotes around string values.
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    updateVariable(msg);
  }
  return(true);
}

//------------------------------------------------------------
// Procedure: OnStartUp()

bool PokeDB::OnStartUp()
{
  CMOOSApp::OnStartUp();
  
  registerVariables();
  return(true);
}

//------------------------------------------------------------
// Procedure: OnConnectToServer()

bool PokeDB::OnConnectToServer()
{
  registerVariables();
  return(true);
}

//------------------------------------------------------------
// Procedure: setPokeDouble

void PokeDB::setPokeDouble(const string& varname, const string& value)
{
  m_varname.push_back(varname);
  m_valtype.push_back("double");
  m_varvalue.push_back(value);

  m_dvalue_read.push_back("");
  m_svalue_read.push_back("");
  m_source_read.push_back("");
  m_valtype_read.push_back("");
  m_wrtime_read.push_back("");
  m_varname_recd.push_back(false);
}

//------------------------------------------------------------
// Procedure: setPokeString

void PokeDB::setPokeString(const string& varname, const string& value)
{
  m_varname.push_back(varname);
  m_valtype.push_back("string");
  m_varvalue.push_back(value);

  m_dvalue_read.push_back("");
  m_svalue_read.push_back("");
  m_source_read.push_back("");
  m_valtype_read.push_back("");
  m_wrtime_read.push_back("");
  m_varname_recd.push_back(false);
}

//------------------------------------------------------------
// Procedure: registerVariables

void PokeDB::registerVariables()
{
  unsigned int i, vsize = m_varname.size();
  for(i=0; i<vsize; i++) 
    m_Comms.Register(m_varname[i], 0);

  m_Comms.Register("DB_UPTIME", 0);
  m_Comms.Register("DB_TIME", 0);
}

//------------------------------------------------------------
// Procedure: updateVariable
//      Note: Will read a MOOS Mail message and grab the fields
//            and update the variable only if its in the vector 
//            of variables vector<string> vars.

void PokeDB::updateVariable(CMOOSMsg &msg)
{
  string varname = msg.GetKey();  
  
  int ix = -1;
  unsigned int index, vsize = m_varname.size();
  for(index=0; index<vsize; index++)
    if(m_varname[index] == varname)
      ix = index;
  if(ix == -1)
    return;

  double vtime     = msg.GetTime() - m_db_start_time;
  string vtime_str = doubleToString(vtime, 2);
  vtime_str = dstringCompact(vtime_str);
  
  m_source_read[ix] = msg.GetSource();
  m_wrtime_read[ix] = vtime_str;

  if(msg.IsString()) {
    m_svalue_read[ix]  = msg.GetString();
    m_valtype_read[ix] = "string";
  }      
  else if(msg.IsDouble()) {
    m_dvalue_read[ix]  = doubleToString(msg.GetDouble());
    m_valtype_read[ix] = "double";
  }
}

//------------------------------------------------------------
// Procedure: printReport()

void PokeDB::printReport()
{
  printf("  %-22s", "VarName");
  
  printf("%-12s", "(S)ource");
  printf("%-12s", "(T)ime");
  printf("VarValue\n");
  
  printf("  %-22s", "----------------");
  printf("%-12s", "----------");
  printf("%-12s", "----------");
  printf(" -------------\n");
  
  unsigned int i, vsize = m_varname.size();
  for(i=0; i<vsize; i++) {
    
    printf("  %-22s ", m_varname[i].c_str());
    printf("%-12s",    m_source_read[i].c_str());
    printf("%-12s",    m_wrtime_read[i].c_str());
    if(m_valtype_read[i] == "string") {
      if(m_svalue_read[i] != "") {
	printf("\"%s\"", m_svalue_read[i].c_str());
      }
      else
	printf("n/a");
    }
    else if(m_valtype_read[i] == "double")
      printf("%s", m_dvalue_read[i].c_str());
    printf("\n");		
  }
}







