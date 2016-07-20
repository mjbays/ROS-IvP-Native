/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: LMV_Utils.h                                          */
/*    DATE: July 8th, 2008                                       */
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

#include "LMV_Utils.h"
#include "MBUtils.h"
#include "FileBuffer.h"

using namespace std;

//--------------------------------------------------------
// Procedure: readEntriesFromFile

vector<string> readEntriesFromFile(const string& filestr, 
				   const string& param)
{
  vector<string> return_vector;

  // Can take multiple parameter strings separated by colons. 
  // For example "polygon:poly" or "seglist:segl:seg_list"
  vector<string> pvector = parseString(param, ':');
  int psize = pvector.size();

  vector<string> file_vector = fileBuffer(filestr);
  int lineCount = file_vector.size();
  
  for(int i=0; i < lineCount; i++) {
    string line = stripBlankEnds(file_vector[i]);
    
    if((line.length()!=0) && ((line)[0]!='#')) {
      vector<string> svector = chompString(line, '=');
      if(svector.size() == 2) {
	string left = tolower(stripBlankEnds(svector[0]));
	bool param_match = false;
	for(int j=0; j<psize; j++)
	  if(left == pvector[j])
	    param_match = true;
	if(param_match) {
	  string right = stripBlankEnds(svector[1]);
	  return_vector.push_back(right);
	}
      }
    }
  }
  return(return_vector);
}







