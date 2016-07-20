/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: IvPProblem.h                                         */
/*    DATE: Too long ago to remember (1996-1999)                 */
/*                                                               */
/* The algorithms embodied in this software are protected under  */
/* U.S. Pat. App. Ser. Nos. 10/631,527 and 10/911,765 and are    */
/* the property of the United States Navy.                       */
/*                                                               */
/* Permission to use, copy, modify and distribute this software  */
/* and its documentation for any non-commercial purpose, without */
/* fee, and without a written agreement is hereby granted        */
/* provided that the above notice and this paragraph and the     */
/* following three paragraphs appear in all copies.              */
/*                                                               */
/* Commercial licences for this software may be obtained by      */
/* contacting Patent Counsel, Naval Undersea Warfare Center      */
/* Division Newport at 401-832-4736 or 1176 Howell Street,       */
/* Newport, RI 02841.                                            */
/*                                                               */
/* In no event shall the US Navy be liable to any party for      */
/* direct, indirect, special, incidental, or consequential       */
/* damages, including lost profits, arising out of the use       */
/* of this software and its documentation, even if the US Navy   */
/* has been advised of the possibility of such damage.           */
/*                                                               */
/* The US Navy specifically disclaims any warranties, including, */
/* but not limited to, the implied warranties of merchantability */
/* and fitness for a particular purpose. The software provided   */
/* hereunder is on an 'as-is' basis, and the US Navy has no      */
/* obligations to provide maintenance, support, updates,         */
/* enhancements or modifications.                                */
/*****************************************************************/
 
#ifndef IVPPROBLEM_HEADER
#define IVPPROBLEM_HEADER

#include "Problem.h"
#include "Compactor.h"

class IvPProblem: public Problem {
public:
  IvPProblem(Compactor *c=0);
  ~IvPProblem();

  void   preCompact();
  bool   solve(const IvPBox *isolbox=0);

protected:
  void   solvePrior(const IvPBox *b=0);
  void   solveRecurse(int);
  void   solvePost();
  double upperTightBound(int, IvPBox*);
  double upperCheapBound(int, IvPBox*);

protected:  
  IvPBox**   nodeBox;
  Compactor* compactor;
  bool       ownCompactor;
};  

#endif















