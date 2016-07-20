/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: AOF.cpp                                              */
/*    DATE:                                                      */
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

#include "AOF.h"

using namespace std;

//----------------------------------------------------------------
// Procedure: extract()

double AOF::extract(const string& var, const IvPBox* pbox) const 
{
  int index = m_domain.getIndex(var);
  if(index == -1)
    return(0);
  return(m_domain.getVal(index, pbox->pt(index)));
}

//----------------------------------------------------------------
// Procedure: extract()

double AOF::extract(const string& varname, const vector<double>& point) const 
{
  int index = m_domain.getIndex(varname);
  if((index == -1) || ((unsigned int)(index) >= point.size()))
    return(0);
  return(point[index]);
}


