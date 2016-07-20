/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: XYPolygon.h                                          */
/*    DATE: Apr 29, 2005                                         */
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
 
#ifndef XY_POLYGON_HEADER
#define XY_POLYGON_HEADER

#include <vector>
#include <string>
#include "XYSegList.h"

class XYPolygon : public XYSegList {
public:
  XYPolygon();
  ~XYPolygon() {}

  // Polygon create and edit functions
  bool   add_vertex(double, double, bool=true);
  bool   add_vertex(double, double, double, bool=true);
  bool   add_vertex(double, double, double, std::string, bool=true);
  bool   alter_vertex(double, double, double=0);
  bool   delete_vertex(double, double);
  void   grow_by_pct(double pct);
  void   grow_by_amt(double amt);
  bool   insert_vertex(double, double, double=0);
  bool   is_clockwise() const;

  void   clear();
  bool   apply_snap(double snapval);
  void   reverse();
  void   rotate(double, double, double);
  void   rotate(double);

public:
  bool   contains(double, double) const;
  bool   contains(const XYPolygon&) const;
  bool   intersects(const XYPolygon&) const;
  double dist_to_poly(double px, double py) const;
  double dist_to_poly(double x1, double y1, double x2, double y2) const;
  double dist_to_poly(double px, double py, double angle) const;
  bool   seg_intercepts(double, double, double, double) const;
  bool   vertex_is_viewable(unsigned int, double, double) const;
  bool   is_convex() const  {return(m_convex_state);}
  void   determine_convexity();

  double max_radius() const;
  bool   closest_point_on_poly(double sx, double sy, double& rx, double& ry) const;


  XYSegList exportSegList(double x=0, double y=0);

protected:
  int    side(double x1, double y1, double x2, 
	      double y2, double x3, double y3) const;
  void   set_side(int);

private:
  std::vector<int> m_side_xy;

  bool     m_convex_state;
};

#endif





