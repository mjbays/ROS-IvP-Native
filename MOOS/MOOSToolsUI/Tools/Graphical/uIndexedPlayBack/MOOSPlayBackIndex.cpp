///////////////////////////////////////////////////////////////////////////
//
//   MOOS - Mission Oriented Operating Suite 
//  
//   A suit of Applications and Libraries for Mobile Robotics Research 
//   Copyright (C) 2001-2005 Massachusetts Institute of Technology and 
//   Oxford University. 
//    
//   This software was written by Paul Newman at MIT 2001-2002 and Oxford 
//   University 2003-2005. email: pnewman@robots.ox.ac.uk. 
//      
//   This file is part of a  MOOS Utility Component. 
//        
//   This program is free software; you can redistribute it and/or 
//   modify it under the terms of the GNU General Public License as 
//   published by the Free Software Foundation; either version 2 of the 
//   License, or (at your option) any later version. 
//          
//   This program is distributed in the hope that it will be useful, 
//   but WITHOUT ANY WARRANTY; without even the implied warranty of 
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
//   General Public License for more details. 
//            
//   You should have received a copy of the GNU General Public License 
//   along with this program; if not, write to the Free Software 
//   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 
//   02111-1307, USA. 
//
//////////////////////////    END_GPL    //////////////////////////////////
// MOOSPlayBack.cpp: implementation of the CMOOSPlayBackIndex class.
//
//////////////////////////////////////////////////////////////////////
#if (_MSC_VER == 1200)
#pragma warning(disable: 4786)
#pragma warning(disable: 4503)
#endif

#include "MOOSPlayBackIndex.h"
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_CHOKE_TIME 2.0

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMOOSPlayBackIndex::CMOOSPlayBackIndex()
{
    m_dfLastMessageTime = 0;
    m_nCurrentLine = 0;
    m_dfTickTime = 0.01;
    m_dfLastClientProcessedTime = -1;
    m_bWaitingForClientCatchup = false;
}

CMOOSPlayBackIndex::~CMOOSPlayBackIndex()
{

}

void CMOOSPlayBackIndex::Initialise(const string &sFileName)
{
    if (m_ALog.IsOpen())
        m_ALog.Close();

    m_nCurrentLine = 0;
    m_dfLastClientProcessedTime = -1;
    m_sFileName = sFileName;

    // This could throw
    m_ALog.Open(sFileName);
}

bool CMOOSPlayBackIndex::IsOpen()
{
    return m_ALog.IsOpen();
}

int CMOOSPlayBackIndex::GetSize()
{
    return m_ALog.GetLineCount();
}

double CMOOSPlayBackIndex::GetTimeNow()
{
    return m_dfLastMessageTime - m_dfLogStart;
}

int CMOOSPlayBackIndex::GetCurrentLine()
{
    return m_nLastLine;
}

double CMOOSPlayBackIndex::GetStartTime()
{
    if (!m_ALog.IsOpen() || m_ALog.GetLineCount() < 0)
        return 0;

    return m_ALog.GetEntryTime(0);

}

double CMOOSPlayBackIndex::GetFinishTime()
{
    if (!m_ALog.IsOpen() || m_ALog.GetLineCount() < 0)
        return 0;

    return m_ALog.GetEntryTime(GetSize() - 1);

}

bool CMOOSPlayBackIndex::IsEOF()
{
    if (!m_ALog.IsOpen())
        return true;

    return m_nCurrentLine >= m_ALog.GetLineCount();
}

bool CMOOSPlayBackIndex::Iterate(MOOSMSG_LIST &Output)
{
    if (IsEOF())
        return false;

    double dfStopTime = m_dfLastMessageTime + m_dfTickTime;

    bool bDone = false;

    while (!bDone && !IsEOF())
    {
        CMOOSMsg NewMsg;

        double dfTNext = m_ALog.GetEntryTime(m_nCurrentLine);

        //are we in a mode in which we are slaved to a client
        //via its publishing of MOOS_CHOKE?
        m_dfClientLagTime = MOOSTime() - m_dfLastClientProcessedTime;
        if (m_dfLastClientProcessedTime != -1 && m_dfClientLagTime
                > MAX_CHOKE_TIME)
        {
            m_bWaitingForClientCatchup = true;
            bDone = true;
            dfStopTime = dfTNext;
            continue;
        } else
        {
            //normal sequential processing under our own steam
            m_bWaitingForClientCatchup = false;

            if (dfTNext <= dfStopTime)
            {

                if (MessageFromLine(m_ALog.GetLine(m_nCurrentLine), NewMsg))
                {
                    if (!IsFiltered(NewMsg.GetSource()))
                    {
                        Output.push_front(NewMsg);
                    }
                }

                CMOOSMsg timeMsg(MOOS_NOTIFY, "PLAYBACK_DB_TIME", dfTNext,
                        dfTNext);
                Output.push_front(timeMsg);

                // arh moved this out of the loop above, because a failed
                // call to MessageFromLine would make uPlayback hang in
                // an infinite loop
                m_nCurrentLine++;
            } else
            {
                bDone = true;
            }

        }
    }

    m_dfLastMessageTime = dfStopTime;

    return true;
}

bool CMOOSPlayBackIndex::Reset()
{
    if (!m_ALog.IsOpen())
        return false;

    m_nCurrentLine = 0;
    m_dfLastMessageTime = m_ALog.GetEntryTime(0) - 1e-6;

    return true;
}

bool CMOOSPlayBackIndex::SetTickInterval(double dfInterval)
{
    m_dfTickTime = dfInterval;
    return true;
}

bool CMOOSPlayBackIndex::IsFiltered(const std::string & sSrc)
{
    return m_SourceFilter.find(sSrc) != m_SourceFilter.end();
}

bool CMOOSPlayBackIndex::Filter(const std::string & sSrc, bool bWanted)
{
    if (bWanted)
    {
        //remove from our filter things that are wanted
        STRING_SET::iterator p = m_SourceFilter.find(sSrc);

        if (p != m_SourceFilter.end())
        {
            m_SourceFilter.erase(p);
        }
    } else
    {
        //we don;t want this type of message (from thsinclient)
        //so add it to our filter
        m_SourceFilter.insert(sSrc);
        MOOSTrace("Filtering messages from %s\n", sSrc.c_str());
    }
    return true;
}

bool CMOOSPlayBackIndex::ClearFilter()
{
    m_SourceFilter.clear();
    return true;
}

bool CMOOSPlayBackIndex::MessageFromLine(const std::string & sLine,
        CMOOSMsg &Msg)
{
    int n = 0;
    std::string sTime, sKey, sSrc;

    m_ALog.GetNextToken(sLine, n, sTime);
    m_ALog.GetNextToken(sLine, n, sKey);
    m_ALog.GetNextToken(sLine, n, sSrc);

    // this needs to be size_t and _not_ unsigned int to avoid segfaults on
    // 64-bit machines: size_t is 64-bit, unsigned int is 32-bit
    size_t nData = sLine.find_first_not_of(" \t", n);
    if (nData == string::npos)
        return false;

    std::string sData = sLine.substr(nData, sLine.size() - nData);

    Msg.m_dfTime = MOOSTime();

    if (MOOSIsNumeric(sData))
    {
        Msg.m_dfVal = atof(sData.c_str());
        Msg.m_cDataType = MOOS_DOUBLE;
        Msg.m_sVal = "";
    }
    else
    {
        Msg.m_dfVal = 0.0;

        if (sData.find("<MOOS_BINARY>") != std::string::npos &&
        		sData.find("</MOOS_BINARY>") != std::string::npos	)
        {
        	MOOSChomp(sData,"<MOOS_BINARY>");
        	sData = MOOSChomp(sData,"</MOOS_BINARY>");

        	//Msg.MarkAsBinary();
            long long nOffset;
            if (!MOOSValFromString(nOffset, sData, "Offset"))
                return MOOSFail(
                        "badly formed MOOS_BINARY indicator - missing \"Offset=xyz\"");

            std::string sFile;
            if (!MOOSValFromString(sFile, sData, "File"))
                return MOOSFail(
                        "badly formed MOOS_BINARY indicator - missing \"File=XYZ\"");

            int nBytes;
            if (!MOOSValFromString(nBytes, sData, "Bytes"))
                return MOOSFail(
                        "badly formed MOOS_BINARY indicator - missing \"Bytes=xyz\"");

            //corner case of binary file changing half way through a log....(Why this might happen
            //I don't know but catch it anyway....
            if (sFile != m_sBinaryFileName && m_BinaryFile.is_open())
            {
                m_BinaryFile.close();
            }
            m_sBinaryFileName = sFile;

            if (!m_BinaryFile.is_open())
            {
                //open the file with the correct name
                //here we need to point the file towards the full path - the blog file should be sitting next
                //to teh alog being played

                /** splits a fully qualified path into parts -path, filestem and extension */
                std::string sPath, sFileName, sExtension;

                if (!MOOSFileParts(m_ALog.GetFileName(), sPath, sFileName,
                        sExtension))
                    return MOOSFail(
                            "failed to parse alog file into file parts %s",
                            MOOSHERE);

                std::string sFullBinaryLogPath = sPath + "/"
                        + m_sBinaryFileName;

                MOOSTrace("opening binary file %s\n",
                        sFullBinaryLogPath.c_str());

                m_BinaryFile.open(sFullBinaryLogPath.c_str(), std::ios::binary);
                if (!m_BinaryFile)
                {
                    return MOOSFail("unable to open binary file called %s",
                            m_sBinaryFileName.c_str());
                }

            }

            //move to the right place in the file
            m_BinaryFile.seekg(nOffset);

            //make space
            char * pBD = new char[nBytes];

            //read the right number of bytes
            m_BinaryFile.read(pBD, nBytes);

            //copy data
            Msg.m_sVal.assign(pBD, nBytes);

            //delete temporary space
            delete[] pBD;

            Msg.MarkAsBinary();

        } else
        {
            Msg.m_dfVal = 0.0;
            Msg.m_cDataType = MOOS_STRING;
            Msg.m_sVal = sData;

        }

    }

    Msg.m_sSrc = sSrc;
    Msg.m_sKey = sKey;
    Msg.m_cMsgType = MOOS_NOTIFY;

    return true;
}

double CMOOSPlayBackIndex::GetLastMessageTime()
{
    return m_dfLastMessageTime;
}

bool CMOOSPlayBackIndex::SetLastTimeProcessed(double dfTime)
{
    m_dfLastClientProcessedTime = dfTime;
    //double diff = MOOSTime() - dfTime;
    return true;
}

bool CMOOSPlayBackIndex::IsWaitingForClient()
{
    return m_bWaitingForClientCatchup;
}

string CMOOSPlayBackIndex::GetStatusString()
{
    return MOOSFormat("%s Client Lag %g",
            m_bWaitingForClientCatchup ? "Waiting for Client CATCHUP"
                    : "Playing just fine", m_dfClientLagTime);
}

bool CMOOSPlayBackIndex::GotoTime(double dfT)
{
    int n = m_ALog.SeekToFindTime(dfT);

    if (n != -1)
    {
        m_nCurrentLine = n;
        m_dfLastClientProcessedTime = -1;
        m_bWaitingForClientCatchup = false;
        m_dfLastMessageTime = m_ALog.GetEntryTime(m_nCurrentLine) - 1e-6;
        return true;
    } else
    {
        return false;
    }
}
