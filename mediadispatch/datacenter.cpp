/*
 * datacenter.cpp
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#include "datacenter.h"

void CDataCenter::AddGroupInfo(int32_t nGroupID, CGroupInfo *pGroupInfo)
{
	m_stGroupInfoMap[nGroupID] = pGroupInfo;
}

void CDataCenter::DelGroupInfo(int32_t nGroupID)
{
	GroupInfoMap::iterator it = m_stGroupInfoMap.find(nGroupID);
	if(it != m_stGroupInfoMap.end())
	{
		m_stGroupInfoMap.erase(it);
	}
}

CGroupInfo *CDataCenter::FindGroupInfo(int32_t nGroupID)
{
	GroupInfoMap::iterator it = m_stGroupInfoMap.find(nGroupID);
	if(it != m_stGroupInfoMap.end())
	{
		return it->second;
	}

	return NULL;
}

int32_t CDataCenter::GetAllGroupID(int32_t arrGroupID[], int32_t nSize)
{
	int32_t nCount = 0;
	GroupInfoMap::iterator it = m_stGroupInfoMap.begin();
	for(; it != m_stGroupInfoMap.end(); it++)
	{
		arrGroupID[nCount++] = it->first;
		if(nCount >= nSize)
		{
			break;
		}
	}

	return nCount;
}


