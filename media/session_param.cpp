/*
 * session_param.cpp
 *
 *  Created on: 2014年5月26日
 *      Author: jimm
 */

#include "session_param.h"

int32_t CSessionParam::Init()
{
	m_nEntityType = enmEntityType_None;
	m_nServerID = enmInvalidServerID;
	m_nUserID = enmInvalidUserID;
	return 0;
}

int32_t CSessionParam::Uninit()
{
	return 0;
}

int32_t CSessionParam::GetSize()
{
	return sizeof(*this);
}

void CSessionParam::SetUserID(UserID nUserID)
{
	m_nUserID = nUserID;
}

UserID CSessionParam::GetUserID()
{
	return m_nUserID;
}

void CSessionParam::SetServerInfo(EntityType nEntityType, ServerID nServerID)
{
	m_nEntityType = nEntityType;
	m_nServerID = nServerID;
}

void CSessionParam::GetServerInfo(EntityType &nEntityType, ServerID &nServerID)
{
	nEntityType = m_nEntityType;
	nServerID = m_nServerID;
}

void CSessionParam::AddRoomParam(RoomParam &stRoomParam)
{
	m_stRoomParamList.push_back(stRoomParam);
}

void CSessionParam::DelRoomParam(RoomID nRoomID, UserID nShowerID, UserID nSubscriberID)
{
	RoomParam stRoomParam(nRoomID, nShowerID, nSubscriberID);
	for(list<RoomParam>::iterator it = m_stRoomParamList.begin(); it != m_stRoomParamList.end(); ++it)
	{
		if((*it) == stRoomParam)
		{
			m_stRoomParamList.erase(it);
			break;
		}
	}
}

RoomParam CSessionParam::FirstRoomParam()
{
	if(m_stRoomParamList.empty())
	{
		return RoomParam(enmInvalidRoomID, enmInvalidUserID, enmInvalidUserID);
	}

	RoomParam stRoomParam = m_stRoomParamList.front();
	m_stRoomParamList.pop_front();
	return stRoomParam;
}

RoomParam CSessionParam::FrontRoomParam()
{
	if(m_stRoomParamList.empty())
	{
		return RoomParam(enmInvalidRoomID, enmInvalidUserID, enmInvalidUserID);
	}

	return m_stRoomParamList.front();
}

int32_t CSessionParam::RoomParamCount()
{
	return m_stRoomParamList.size();
}

