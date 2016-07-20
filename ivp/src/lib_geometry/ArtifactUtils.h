/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: ArtifactUtils.h                                      */
/*    DATE: Oct 24 2007                                          */
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

#ifndef XY_ARTIFACTUTILS_HEADER
#define XY_ARTIFACTUTILS_HEADER


#include "XYSegList.h"
#include "XYPolygon.h"

const XYSegList generateLawnmower(const XYPolygon&, double, double, double, double, bool);
const XYSegList generateLawnmowerFull(const XYPolygon&, double, double);
const std::vector<std::string> generateArtifacts(double, double, double, double, double, double, const XYPolygon&);


#endif






