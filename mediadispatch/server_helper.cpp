/*
 * server_helper.cpp
 *
 *  Created on: 2014年5月14日
 *      Author: jimm
 */

#include "server_helper.h"
#include "session_param.h"
#include "../include/typedef.h"
#include "../frame/frame.h"
#include "server_config.h"
#include "server_typedef.h"
#include "../include/message_head.h"
#include "../common/common_memmgt.h"

using namespace FRAME;

EntityMap CServerHelper::m_stEntityMap;

int32_t CServerHelper::SendSSReq(int32_t nMsgID, RoomID nRoomID, UserID nUserID, IMsgBody *pMsgBody,
		EntityType nEntityType, ServerID nServerID)
{
	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);

	uint32_t nOffset = 0;
	uint8_t arrBuf[enmMaxMessageSize];
	MsgHeadSS stMsgHeadSS;
	stMsgHeadSS.m_nHeadSize = stMsgHeadSS.GetSize();
	stMsgHeadSS.m_nMsgID = nMsgID;
	stMsgHeadSS.m_nRoomID = nRoomID;
	stMsgHeadSS.m_nUserID = nUserID;
	stMsgHeadSS.m_nTransType = enmTransType_P2P;
	stMsgHeadSS.m_nSrcType = pServerConfig->GetServerType();
	stMsgHeadSS.m_nSrcID = pServerConfig->GetServerID();
	stMsgHeadSS.m_nDstType = nEntityType;
	stMsgHeadSS.m_nDstID = nServerID;

	IIOSession *pIoSession = GetIoSession(nEntityType, nServerID);
	if(pIoSession == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found iosession!{EntityType=%d, ServerID=%d}\n", nEntityType, nServerID);
		return 0;
	}

	int32_t nRet = stMsgHeadSS.Encode(arrBuf, sizeof(arrBuf), nOffset);
	if(nRet < 0)
	{
		WRITE_WARN_LOG(SERVER_NAME, "msg head cs encode failed!\n");
		return 0;
	}

	nRet = pMsgBody->Encode(arrBuf, sizeof(arrBuf) - nOffset, nOffset);
	if(nRet < 0)
	{
		WRITE_WARN_LOG(SERVER_NAME, "msg body encode failed!\n");
		return 0;
	}

	stMsgHeadSS.m_nTotalSize = (uint16_t)nOffset;
	nOffset = 1;
	nRet = CCodeEngine::Encode(arrBuf, sizeof(arrBuf), nOffset, stMsgHeadSS.m_nTotalSize);
	if(nRet < 0)
	{
		WRITE_WARN_LOG(SERVER_NAME, "encode total size failed!\n");
		return 0;
	}

	g_Frame.Dump(&stMsgHeadSS, pMsgBody, "send ss req ");

	return pIoSession->Write(arrBuf, stMsgHeadSS.m_nTotalSize);
}

int32_t CServerHelper::SendSSResp(int32_t nMsgID, IMsgHead *pMsgHead, IMsgBody *pMsgBody, IIOSession *pIoSession)
{
	return 0;
}

int32_t CServerHelper::SendSSNoti(int32_t nMsgID, IMsgBody *pMsgBody, RoomID nRoomID, UserID nUserID, TransType nTransType,
		EntityType nEntityType, ServerID nServerID)
{
	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	MsgHeadSS stMsgHeadSS;
	stMsgHeadSS.m_nTotalSize = 0;
	stMsgHeadSS.m_nMsgID = nMsgID;
	stMsgHeadSS.m_nRoomID = nRoomID;
	stMsgHeadSS.m_nUserID = nUserID;
	stMsgHeadSS.m_nTransType = nTransType;
	stMsgHeadSS.m_nSrcType = pServerConfig->GetServerType();
	stMsgHeadSS.m_nSrcID = pServerConfig->GetServerID();
	stMsgHeadSS.m_nDstType = nEntityType;
	stMsgHeadSS.m_nDstID = nServerID;

	uint32_t nOffset = 0;
	uint8_t arrBuf[enmMaxMessageSize];

	int32_t nRet = stMsgHeadSS.Encode(arrBuf, sizeof(arrBuf), nOffset);
	if(nRet < 0)
	{
		return 0;
	}

	nRet = pMsgBody->Encode(arrBuf, sizeof(arrBuf) - nOffset, nOffset);
	if(nRet < 0)
	{
		return 0;
	}

	stMsgHeadSS.m_nTotalSize = nOffset;
	nOffset = 1;
	nRet = CCodeEngine::Encode(arrBuf, sizeof(arrBuf), nOffset, stMsgHeadSS.m_nTotalSize);
	if(nRet < 0)
	{
		return 0;
	}

	switch(nTransType)
	{
	case enmTransType_P2P:
	{
		IIOSession *pIoSession = GetIoSession(nEntityType, nServerID);
		if(pIoSession == NULL)
		{
			return 0;
		}

		pIoSession->Write(arrBuf, stMsgHeadSS.m_nTotalSize);
	}
		break;
	case enmTransType_Room:
		break;
	case enmTransType_Server:
		for(IDMap::iterator it = m_stEntityMap[nEntityType].begin(); it != m_stEntityMap[nEntityType].end(); ++it)
		{
			if((it->first == nServerID) || (nServerID == enmInvalidServerID))
			{
				it->second->Write(arrBuf, stMsgHeadSS.m_nTotalSize);
			}
		}
		break;
	case enmTransType_Random:
	{
		IIOSession *pIoSession = GetIoSession(nEntityType, nServerID);
		if(pIoSession == NULL)
		{
			return 0;
		}

		pIoSession->Write(arrBuf, stMsgHeadSS.m_nTotalSize);
	}
		break;
	default:
		break;
	}

	g_Frame.Dump(&stMsgHeadSS, pMsgBody, "send ss noti ");

	return 0;
}

void CServerHelper::RegistSessionInfo(EntityType nEntityType, int32_t nID, IIOSession *pIoSession)
{
	m_stEntityMap[nEntityType][nID] = pIoSession;

	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	if(pSessionParam == NULL)
	{
		pSessionParam = NEW(CSessionParam);
		pIoSession->SetParamPtr(pSessionParam);
	}

	pSessionParam->SetEntityType(nEntityType);
	pSessionParam->SetServerID(nID);
}

void CServerHelper::UnregistSessionInfo(IIOSession *pIoSession)
{
	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	if(pSessionParam == NULL)
	{
		return;
	}

	EntityType nEntityType = pSessionParam->GetEntityType();
	ServerID nServerID = pSessionParam->GetServerID();

	EntityMap::iterator it = m_stEntityMap.find(nEntityType);
	if(it != m_stEntityMap.end())
	{
		IDMap::iterator et = it->second.find(nServerID);
		if(et != it->second.end())
		{
			it->second.erase(et);
		}
	}

	DELETE(pSessionParam);
}

IIOSession *CServerHelper::GetIoSession(EntityType nEntityType, int32_t nID)
{
	IIOSession *pIoSession = NULL;

	EntityMap::iterator it = m_stEntityMap.find(nEntityType);
	if(it != m_stEntityMap.end())
	{
		IDMap::iterator et = it->second.find(nID);
		if(et != it->second.end())
		{
			pIoSession = et->second;
		}
	}

	return pIoSession;
}

IIOSession *CServerHelper::GetOneIoSession(EntityType nEntityType)
{
	IIOSession *pIoSession = NULL;

	EntityMap::iterator it = m_stEntityMap.find(nEntityType);
	if(it != m_stEntityMap.end())
	{
		int32_t nServerCount = m_stEntityMap.count(nEntityType);
		if(nServerCount <= 0)
		{
			return pIoSession;
		}

		//随机出一个索引值
		int32_t nServerIndex = Random(nServerCount);
		int32_t nCount = 0;
		for(IDMap::iterator et = it->second.begin(); et != it->second.end(); ++et)
		{
			if(nCount >= nServerIndex)
			{
				pIoSession = et->second;
				break;
			}
			++nCount;
		}
	}

	return pIoSession;
}

