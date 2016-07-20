/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: Viewer.h                                             */
/*    DATE: Apr 15th 2005                                        */
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

#ifndef VIEWER_HEADER
#define VIEWER_HEADER

#include "IvPDomain.h"
#include "AOF_Cache.h"
#include "IvPFunction.h"
#include "FColorMap.h"
#include "Common_IPFViewer.h"
#include "OF_Rater.h"

class Viewer : public Common_IPFViewer
{
 public:
  Viewer(int x,int y,int w,int h,const char *l=0);
  
  // Pure virtuals that need to be defined
  void   draw();

public:
  int    handle(int);
  void   setAOF(AOF *aof);

public:
  bool   setParam(std::string, std::string);
  bool   setParam(std::string, double);
  void   printParams();

  void   toggleAOF()              {m_draw_aof = !m_draw_aof; redraw();}
  void   toggleIPF()              {m_draw_ipf = !m_draw_ipf; redraw();}
  void   toggleSmartAug();
  void   makeUniformIPF();
  void   makeUniformIPFxN(int amt);
  void   modColorMap(const std::string&);
  void   modPatchAOF(int amt);
  void   modUniformAug(int amt);

  void   runScript();
  void   capture(int);

  void   takeSamples(int amt)     {m_rater.takeSamples(amt);}
  double getParam(const std::string&, bool&);

  std::string getParam(const std::string&);
  std::string getPeakDelta();

protected:
  void   drawIvPFunction(IvPFunction*);
  void   drawIvPBox(const IvPBox&, double, double, int, int);
  void   drawAOF();
  void   drawFocusBox();

private:
  float      m_base_aof;
  float      m_base_ipf;
  float      m_scale;
  int        m_patch;
  bool       m_draw_aof;
  bool       m_draw_ipf;
  AOF_Cache  m_aof_cache;
  IvPDomain  m_domain;
  FColorMap  m_cmap;
  bool       m_strict_range;
  double     m_create_time;
  int        m_piece_count;
 
  IvPFunction *m_unif_ipf;
  OF_Rater     m_rater;

  bool        m_smart_refine;
  bool        m_directed_refine;
  bool        m_autopeak_refine;

  std::string m_uniform_piece_str;
  int         m_uniform_piece_size;

  std::string m_refine_region_str;
  std::string m_refine_piece_str;
  std::string m_smart_percent_str;
  std::string m_smart_amount_str;

  std::string m_reflector_warnings;
  std::string m_aof_peak;

  int        m_focus_box_x;
  int        m_focus_box_y;
  int        m_focus_box_len;
  int        m_focus_unif_len;
};

#endif 





