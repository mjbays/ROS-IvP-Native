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
//   This file is part of a  MOOS Core Component. 
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
// MOOSCommunity.cpp: implementation of the CMOOSCommunity class.
//
//////////////////////////////////////////////////////////////////////
#include "MOOSCommunity.h"
#include "MOOS/libMOOS/MOOSLib.h"

#define DEFAULT_BRIDGE_COMMS_TICK 20
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

using namespace std;
//switch yard for connect ot all communities..
bool gOnCommunityConnect(void * pParam)
{
    CMOOSCommunity * pCommunity  = (CMOOSCommunity *)(pParam);

    return pCommunity->DoRegistration();

}


#define DEFAULT_SHARED_FREQ 10

CMOOSCommunity::CMOOSCommunity()
{
    m_nSharedFreq = DEFAULT_SHARED_FREQ;
    m_nUDPPort = -1;
    m_sUDPHost = "";
    m_bMOOSClientRunning = false;
}

CMOOSCommunity::~CMOOSCommunity()
{

}

bool CMOOSCommunity::WantsToSink(const SP &sIndex)
{
    if(m_Sinks.empty())
        return false;

    return m_Sinks.find(sIndex)!=m_Sinks.end();    
}

bool CMOOSCommunity::AddSource(const string &sStr)
{
    //add it to permanent list (will be registered on call back)
    m_Sources.insert(sStr);


    //and also subscribe now
    if(m_CommClient.IsConnected())
    {
        double dfPeriod = m_nSharedFreq==0?0.0:1.0/m_nSharedFreq;
        return m_CommClient.Register(sStr,dfPeriod);
    }

    return true;
}

bool CMOOSCommunity::AddSink(const SP &sIndex,const string &sAlias)
{
    //m_Sinks.insert(sVar);

    if(sAlias.empty())
    {
        m_Sinks[sIndex] = sIndex.first;
    }
    else
    {
        //Comm@Host:port:Var
        m_Sinks[sIndex] = sAlias;
    }
    
    return true;
}

std::string CMOOSCommunity::GetAlias(const SP & sIndex)
{
    std::map<SP,std::string>::iterator p = m_Sinks.find(sIndex);

    if(p ==m_Sinks.end())
    {
        MOOSTrace("trouble! not entry for %s %s  %s\n",sIndex.first.c_str(),sIndex.second.c_str(),MOOSHERE);
        MOOSTrace("returning query name as alias\n");
        return sIndex.first;
    }

    return p->second;
}


bool CMOOSCommunity::InitialiseMOOSClient( const string &sHostName,
                                          long nPort,
                                          const string & sMOOSName,
                                          int nFreq)
{

    //this is used to determine the subscription period.
    m_nSharedFreq = nFreq;
    
    m_CommClient.SetOnConnectCallBack(gOnCommunityConnect,this);
    
    //we wanna be transparent...
    m_CommClient.FakeSource(true);
    
    //
    
    
    m_bMOOSClientRunning =  m_CommClient.Run(sHostName.c_str(),
                                             nPort,
                                             sMOOSName.c_str(),
                                             DEFAULT_BRIDGE_COMMS_TICK);
    
    return m_bMOOSClientRunning;
}


bool CMOOSCommunity::HasMOOSSRegistration(const std::string & sVariable)
{
    return m_CommClient.IsRegisteredFor(sVariable);
}


bool CMOOSCommunity::IsMOOSClientRunning()
{
    return m_bMOOSClientRunning;
}

std::string  CMOOSCommunity::GetCommunityName()
{
    return m_sCommunityName;
}

bool CMOOSCommunity::Initialise(const string &sCommunityName)
{

    m_sCommunityName = sCommunityName;
    return true;
}

bool CMOOSCommunity::SetUDPInfo(const std::string & sHost, int nPort)
{
    //this is the address of the place UDP packets should be sent to
    m_sUDPHost = sHost;
    m_nUDPPort = nPort;
    return true;
}

bool CMOOSCommunity::HasUDPConfigured() const
{
    return m_nUDPPort!=-1;
}

int CMOOSCommunity::GetUDPPort()
{
    return m_nUDPPort;
}

std::string CMOOSCommunity::GetUDPHost()
{
    return m_sUDPHost;
}

bool CMOOSCommunity::Fetch(MOOSMSG_LIST & Mail)
{
	return m_CommClient.Fetch(Mail);
}

bool CMOOSCommunity::Post(CMOOSMsg & M)
{
    return m_CommClient.Post(M);
}


bool CMOOSCommunity::DoRegistration()
{
    std::set<std::string>::iterator q;

    for(q=m_Sources.begin();q!=m_Sources.end();q++)
    {
        double dfPeriod = m_nSharedFreq==0?0.0:1.0/m_nSharedFreq;

        if(!m_CommClient.Register(*q,dfPeriod))
            return false;
    }
    return true;
}

string CMOOSCommunity::GetFormattedName()
{
    return MOOSFormat("%s@%s",m_sCommunityName.c_str(),m_CommClient.GetDescription().c_str());
}

string CMOOSCommunity::GetCommsName()
{
    return  m_CommClient.GetDescription();
}

