/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: MBUtils.h                                            */
/*    DATE: (1996-2005)                                          */
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
 
#ifndef MBUTILS_HEADER_EX
#define MBUTILS_HEADER_EX

#include <vector>
#include <string>
#include <set>
#include <time.h>

std::vector<std::string> parseString(const std::string&, char);
std::vector<std::string> parseString(const std::string&, 
				     const std::string&);

std::vector<std::string> parseStringQ(const std::string&, char);
std::vector<std::string> parseStringQ(const std::string&, char, unsigned int);


std::vector<std::string> parseStringZ(const std::string&, char, 
				      const std::string&);

std::vector<std::string> parseStringToWords(const std::string&, char c=0);

std::vector<std::string> parseQuotedString(const std::string&, char separator);

std::vector<std::string> chompString(const std::string&, char);
std::vector<std::string> sortStrings(std::vector<std::string>);
std::vector<std::string> mergeVectors(std::vector<std::string>,
				      std::vector<std::string>);
std::vector<std::string> removeDuplicates(const std::vector<std::string>&);

bool vectorContains(const std::vector<std::string>&, 
		    const std::string&);

std::string augmentSpec(const std::string&, const std::string&, char=',');
std::string removeWhite(const std::string&);

std::string biteString(std::string&, char);
std::string biteStringX(std::string&, char);
std::string biteString(std::string&, char, char);

std::string rbiteString(std::string&, char);


std::string stripBlankEnds(const std::string&);
std::string tolower(const std::string&);
std::string toupper(const std::string&);
std::string truncString(const std::string&, unsigned int newlen, 
			std::string="");
std::string boolToString(bool);
std::string uintToString(unsigned int);
std::string ulintToString(unsigned long int);
std::string intToString(int);
std::string intToCommaString(int);
std::string uintToCommaString(unsigned int);
std::string floatToString(float, int=5);
std::string setToString(const std::set<std::string>&);
std::string doubleToString(double, int=5);
std::string doubleToStringX(double, int=5);
std::string dstringCompact(const std::string&);
std::string compactConsecutive(const std::string&, char);
std::string findReplace(const std::string&, char, char);
std::string findReplace(const std::string&, const std::string&, 
			 const std::string&);
std::string padString(const std::string&, std::string::size_type, bool=true);
std::string stripComment(const std::string&, const std::string&);
std::string stripQuotes(const std::string&);
std::string stripBraces(const std::string&);
std::string doubleToHex(double);

std::string svectorToString(const std::vector<std::string>&, char=',');

bool isValidIPAddress(const std::string&);
bool strContains(const std::string&, const std::string&);
bool strContains(const std::string&, const char);
bool strContainsWhite(const std::string&);
bool strBegins(const std::string&, const std::string&, bool=true);
bool strEnds(const std::string&, const std::string&, bool=true);

bool  tokParse(const std::string&, const std::string&, 
		char, char, std::string&);
bool  tokParse(const std::string&, const std::string&, 
		char, char, double&);
bool  tokParse(const std::string&, const std::string&, 
		char, char, bool&);

std::string tokStringParse(const std::string&, const std::string&, 
			   char, char);
double tokDoubleParse(const std::string&, const std::string&, 
		      char, char);

double minElement(const std::vector<double>&);
double maxElement(const std::vector<double>&);
double vclip(const double& var, const double& low, const double& high);
double vclip_min(const double& var, const double& low);
double vclip_max(const double& var, const double& high);
double randomDouble(double min=0, double max=1);

bool  isBoolean(const std::string&);
bool  isNumber(const std::string&, bool=true);
bool  isAlphaNum(const std::string&, const std::string& s="");
bool  isQuoted(const std::string&);
bool  isBraced(const std::string&);

int   getArg(int, char**, int, const char*, const char *s=0);
bool  scanArgs(int, char**, const char*, const char *a=0, const char *b=0);
int   validateArgs(int, char **, std::string);

double snapToStep(double, double v=1.0);
double snapDownToStep(double, double v=1.0);

bool  setBooleanOnString(bool& boolval, std::string str, bool=true);
bool  setPosDoubleOnString(double& dval, std::string str);
bool  setNonNegDoubleOnString(double& dval, std::string str);
bool  setNonWhiteVarOnString(std::string& svar, std::string str);

bool  okFileToRead(std::string);
bool  okFileToWrite(std::string);

void  millipause(int milliseconds);

std::string modeShorten(std::string, bool really_short=true);

std::vector<std::string> tokenizePath(const std::string&);

std::string parseAppName(const std::string&);

bool isKnownVehicleType(const std::string&);

unsigned int charCount(const std::string&, char);

std::vector<std::string> justifyLen(const std::vector<std::string>&,
				    unsigned int maxlen);
std::vector<std::string> justifyLen(const std::string&, unsigned int maxlen);

#endif



