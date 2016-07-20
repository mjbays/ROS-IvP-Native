/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: GeomUtils.h                                          */
/*    DATE: May 8, 2005 Sunday morning at Brueggers              */
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
 
#ifndef XY_GEOM_UTILS_HEADER
#define XY_GEOM_UTILS_HEADER

#include "XYPoint.h"
#include "XYPolygon.h"

// Determines the distance between two points
double distPointToPointXXX(double x1, double y1, double x2, double y2);
double distPointToPoint(double x1, double y1, double x2, double y2);
double distPointToPoint(const XYPoint& pt1, const XYPoint& pt2);

// Determine the distance from a line segment to a point (px,py)
double distPointToSeg(double x1, double y1, double x2, 
		      double y2, double px, double py);

// Determine the distance from a line segment to a point (px,py)
// at a particular given angle. Returns -1 if doesn't intersect
double distPointToSeg(double x1, double y1, double x2, double y2,
                      double px, double py, double ANGLE);

// Determine the distance between two line segments
double distSegToSeg(double x1, double y1, double x2, double y2, 
		    double x3, double y3, double x4, double y4);

// Determine whether and where two lines intersect
bool   linesCross(double x1, double y1, double x2, double y2,
		  double x3, double y3, double x4, double y4,
		  double &ix, double& iy);

// Determine whether and where a given ray and line intersect
bool   lineRayCross(double x1, double y1, double ray_angle,
		    double x3, double y3, double x4, double y4,
		    double &ix, double& iy);

// Determine if two line segments intersect
bool   segmentsCross(double x1, double y1, double x2, double y2,
		     double x3, double y3, double x4, double y4);

// Determine the angle between the two segments x1,y1<-->x2,y2
// and  x2,y2<-->x3,y3
double segmentAngle(double x1, double y1, double x2, double y2,
		    double x3, double y3);

// Determine a point on a segment that, with another point, makes
// a line that is a right angle to the segment
void    perpSegIntPt(double x1, double y1, double x2, double y2,
		     double qx, double qy, double& rx, double& ry);

// Determine a point on a line that, with another point, makes
// a line that is a right angle to the given line
void   perpLineIntPt(double x1, double y1, double x2, double y2,
		     double qx, double qy, double& rx, double& ry);

// Determine a point that is a certain angle and distance from 
// another point
void    projectPoint(double ANGLE, double DIST, double cx, 
		     double cy,    double& rx,  double &ry);
XYPoint projectPoint(double ANGLE, double DIST, double cx, double cy);


void   addVectors(double deg1, double mag1, double deg2, double mag2,
		  double& rdeg, double& rmag); 

bool   bearingMinMaxToPoly(double x, double y, const XYPolygon& poly,
			   double& bmin, double& bmax);


// DEPRECATED INTERFACES
double distToPoint(double x1, double y1, double x2, double y2);

double distToSegment(double x1, double y1, double x2, 
		     double y2, double px, double py);
  
#endif
