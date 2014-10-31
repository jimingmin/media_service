/*
 * datacenter.cpp
 *
 *  Created on: 2014年5月13日
 *      Author: jimm
 */

#include "datacenter.h"
#include "media_channel.h"
#include "session_param.h"
#include "server_config.h"

CDataCenter::CDataCenter()
{
	m_nConnCount = 0;
	m_pTimerHandler = NULL;
}

void CDataCenter::AddChannel(CMediaChannel *pMediaChannel)
{
	//如果有了就不再添加
	if(m_stSessionChannelMap.find(pMediaChannel->GetChannelKey()) != m_stSessionChannelMap.end())
	{
		return;
	}

	m_stSessionChannelMap[pMediaChannel->GetChannelKey()] = pMediaChannel;

//	if(pMediaChannel->IsShower())
//	{
//		m_pTimerHandler->RecordShowerHB(pMediaChannel);
//	}
}

void CDataCenter::DelChannel(CMediaChannel *pMediaChannel)
{
	//没有发现直接返回
	if(m_stSessionChannelMap.find(pMediaChannel->GetChannelKey()) == m_stSessionChannelMap.end())
	{
		return;
	}

	m_stSessionChannelMap.erase(pMediaChannel->GetChannelKey());

//	if(pMediaChannel->IsShower())
//	{
//		m_pTimerHandler->DestroyShowerHB(pMediaChannel);
//	}
}

CMediaChannel *CDataCenter::FindChannel(IIOSession *pIoSession)
{
	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	if((pSessionParam != NULL) && (pSessionParam->RoomParamCount() == 1))
	{
		RoomParam stRoomParam = pSessionParam->FrontRoomParam();
		if(stRoomParam.nRoomID == enmInvalidRoomID || stRoomParam.nShowerID == enmInvalidUserID || stRoomParam.nSubscriberID == enmInvalidUserID)
		{
			return NULL;
		}

		return FindChannel(stRoomParam.nRoomID, stRoomParam.nShowerID, stRoomParam.nSubscriberID);
	}

	return NULL;
}

CMediaChannel *CDataCenter::FindChannel(RoomID nRoomID, UserID nShowerID, UserID nSubscriberID)
{
	char szKey[256];
	sprintf(szKey, "%d:%d:%d", nRoomID, nShowerID, nSubscriberID);
	return FindChannel(szKey);
}

CMediaChannel *CDataCenter::FindChannel(string strKey)
{
	SessionChannelMap::iterator it = m_stSessionChannelMap.find(strKey);
	if(it == m_stSessionChannelMap.end())
	{
		return NULL;
	}

	return it->second;
}

int32_t CDataCenter::GetAllLocalShower(RoomID arrRoomID[], UserID arrShowerID[], int32_t nSize)
{
	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	ServerID nServerID = pServerConfig->GetServerID();

	int32_t nShowerCount = 0;
	for(SessionChannelMap::iterator it = m_stSessionChannelMap.begin(); it != m_stSessionChannelMap.end(); ++it)
	{
		CMediaChannel *pMediaChannel = it->second;
		if(nServerID == pMediaChannel->GetServerID())
		{
			arrRoomID[nShowerCount] = pMediaChannel->GetRoomID();
			arrShowerID[nShowerCount] = pMediaChannel->GetShowerID();
			nShowerCount++;
		}

		if(nShowerCount > nSize)
		{
			break;
		}
	}

	return nShowerCount;
}

void CDataCenter::SetTimerHandler(CTimerHandler *pTimerHandler)
{
	m_pTimerHandler = pTimerHandler;
}

CTimerHandler *CDataCenter::GetTimerHandler()
{
	return m_pTimerHandler;
}

int32_t CDataCenter::IncShowerCount()
{
	return ++m_stLoadInfo.nShowerCount;
}

int32_t CDataCenter::DecShowerCount()
{
	return --m_stLoadInfo.nShowerCount;
}

int32_t CDataCenter::IncSubscriberCount()
{
	return ++m_stLoadInfo.nSubscriberCount;
}

int32_t CDataCenter::DecSubscriberCount()
{
	return --m_stLoadInfo.nSubscriberCount;
}

LoadInfo CDataCenter::GetServerLoadInfo()
{
	return m_stLoadInfo;
}

int32_t CDataCenter::IncConnCount()
{
	return ++m_nConnCount;
}

int32_t CDataCenter::DecConnCount()
{
	return --m_nConnCount;
}


