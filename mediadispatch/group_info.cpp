/*
 * group_info.cpp
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#include "group_info.h"

int32_t CGroupInfo::Init()
{
	m_nGroupID = 0;
	return 0;
}

int32_t CGroupInfo::Uninit()
{
	m_nGroupID = 0;
	m_stServerLoadMap.clear();
	m_stShowerMap.clear();

	return 0;
}

int32_t CGroupInfo::GetSize()
{
	return sizeof(*this);
}

void CGroupInfo::SetGroupID(int32_t nGroupID)
{
	m_nGroupID = nGroupID;
}

int32_t CGroupInfo::GetGroupID()
{
	return m_nGroupID;
}

void CGroupInfo::AddShower(UserID nUserID, string strServerAddress)
{
	m_stShowerMap[nUserID] = strServerAddress;
}

void CGroupInfo::DelShower(UserID nUserID)
{
	ShowerMap::iterator it = m_stShowerMap.find(nUserID);
	if(it != m_stShowerMap.end())
	{
		m_stShowerMap.erase(it);
	}
}

string CGroupInfo::FindShower(UserID nUserID)
{
	ShowerMap::iterator it = m_stShowerMap.find(nUserID);
	if(it != m_stShowerMap.end())
	{
		return it->second;
	}

	return "";
}

void CGroupInfo::AddServer(string strServerAddress)
{
	LoadInfo stLoadInfo;
	m_stServerLoadMap[strServerAddress] = stLoadInfo;
}

void CGroupInfo::DelServer(string strServerAddress)
{
	ServerLoadMap::iterator it = m_stServerLoadMap.find(strServerAddress);
	if(it != m_stServerLoadMap.end())
	{
		m_stServerLoadMap.erase(it);
	}
}

void CGroupInfo::UpdateServerLoadInfo(string strServerAddress, LoadInfo &stLoadInfo)
{
	m_stServerLoadMap[strServerAddress] = stLoadInfo;
}

int32_t CGroupInfo::GetAllServerCount(string arrServerAddress[], int32_t nSize)
{
	int32_t nCount = 0;
	ServerLoadMap::iterator it = m_stServerLoadMap.begin();
	for(; it != m_stServerLoadMap.end(); it++)
	{
		arrServerAddress[nCount++] = it->first;
		if(nCount >= nSize)
		{
			break;
		}
	}

	return nCount;
}

string CGroupInfo::GetServerListString()
{
	if(m_stServerLoadMap.empty())
	{
		return "0";
	}

	ServerLoadMap::iterator it = m_stServerLoadMap.begin();
	string strServerList = it->first;
	++it;
	for(; it != m_stServerLoadMap.end(); it++)
	{
		strServerList += ("-" + it->first);
	}

	return strServerList;
}

LoadInfo CGroupInfo::GetLoadInfo(string strServerAddress)
{
	return m_stServerLoadMap[strServerAddress];
}

