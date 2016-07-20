/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: ZAIC_HLEQ.cpp                                        */
/*    DATE: June 15th 2006                                       */
/*                                                               */
/* This file is part of IvP Helm Core Libs                       */
/*                                                               */
/* IvP Helm Core Libs is free software: you can redistribute it  */
/* and/or modify it under the terms of the Lesser GNU General    */
/* Public License as published by the Free Software Foundation,  */
/* either version 3 of the License, or (at your option) any      */
/* later version.                                                */
/*                                                               */
/* IvP Helm Core Libs is distributed in the hope that it will    */
/* be useful but WITHOUT ANY WARRANTY; without even the implied  */
/* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR       */
/* PURPOSE. See the Lesser GNU General Public License for more   */
/* details.                                                      */
/*                                                               */
/* You should have received a copy of the Lesser GNU General     */
/* Public License along with MOOS-IvP.  If not, see              */
/* <http://www.gnu.org/licenses/>.                               */
/*****************************************************************/

#include <iostream>
#include <cmath>
#include "ZAIC_HLEQ.h"
#include "MBUtils.h"
#include "BuildUtils.h"

using namespace std;

//-------------------------------------------------------------
// Procedure: Constructor

ZAIC_HLEQ::ZAIC_HLEQ(IvPDomain g_domain, const string& varname) 
{
  m_state_ok     = true;

  m_ivp_domain   = subDomain(g_domain, varname);
  if(m_ivp_domain.size() != 1)
    m_state_ok = false;

  if(m_ivp_domain.getVarDelta(0) <= 0)
    m_state_ok = false;

  m_domain_ix    = m_ivp_domain.getIndex(varname);
  m_domain_high  = m_ivp_domain.getVarHigh(m_domain_ix);
  m_domain_low   = m_ivp_domain.getVarLow(m_domain_ix);
  m_domain_pts   = m_ivp_domain.getVarPoints(m_domain_ix);
  m_domain_delta = m_ivp_domain.getVarDelta(m_domain_ix);
  
  m_summit       = 0;
  m_summit_delta = 0;
  m_basewidth    = 0;
  m_minutil      = 0;
  m_maxutil      = 100.0;

  m_ipt_low      = 0;
  m_ipt_one      = 0;
  m_ipt_two      = 0;
  m_ipt_high     = 0;
}

//-------------------------------------------------------------
// Procedure: setSummit
//      Note: Setting the summit outside of the domain range is *not* 
//            treated as a hard error resulting in setting state_ok to
//            false, but will merely generate a warning.

bool ZAIC_HLEQ::setSummit(double val)
{
  m_summit = val;

  if((m_summit < m_domain_low) || (m_summit > m_domain_high))
    m_warning += "given summit value out of domain range (suspicious):";

  return(true);
}

//-------------------------------------------------------------
// Procedure: setSummitDelta

bool ZAIC_HLEQ::setSummitDelta(double val)
{
  if(val < 0)
    val = 0;

  m_summit_delta = val;
  return(true);
}

//-------------------------------------------------------------
// Procedure: setBaseWidth

bool ZAIC_HLEQ::setBaseWidth(double val)
{
  bool ok = (val >= 0);
  if(!ok) {
    m_state_ok = false;
    m_warning += "setBaseWidth:val<0 : ";
  }
  else
    m_basewidth = val;

  return(ok);
}

//------------------------------------------------
bool ZAIC_HLEQ::setMinMaxUtil(double minval, double maxval)
{
  if(minval >= maxval) {
    m_state_ok = false;
    m_warning += "setMinMaxUtil:min>=max : ";
    return(false);
  }

  m_minutil = minval;
  m_maxutil = maxval;
  
  return(true);
}

//-------------------------------------------------------------
// Procedure: getParam
//   Purpose: 

double ZAIC_HLEQ::getParam(string param)
{
  param = tolower(param);
  if(param == "summit")
    return(m_summit);
  else if(param == "summit_delta")
    return(m_summit_delta);
  else if(param == "basewidth")
    return(m_basewidth);
  else if(param == "minutil")
    return(m_minutil);
  else if(param == "maxutil")
    return(m_maxutil);
  else
    return(0);
}




