/*
 * server_helper.cpp
 *
 *  Created on: 2014年5月14日
 *      Author: jimm
 */

#include "server_helper.h"
#include "client_message.h"
#include "server_message.h"
#include "server_typedef.h"
#include "../frame/frame.h"
#include "server_config.h"
#include "session_param.h"
#include "../common/common_memmgt.h"

using namespace FRAME;

EntityMap CServerHelper::m_stEntityMap;

int32_t CServerHelper::SendCSReq(int32_t nMsgID, RoomID nRoomID, UserID nUserID,
		IMsgBody *pMsgBody, EntityType nEntityType, ServerID nServerID)
{
	uint32_t nOffset = 0;
	uint8_t arrBuf[enmMaxMessageSize];
	MsgHeadCS stMsgHeadCS;
	stMsgHeadCS.m_nMsgID = nMsgID;
	stMsgHeadCS.m_nRoomID = nRoomID;
	stMsgHeadCS.m_nUserID = nUserID;
	stMsgHeadCS.m_nSeq = 0;
	stMsgHeadCS.m_nTotalSize = 0;

	IIOSession *pIoSession = GetIoSession(nEntityType, nServerID);
	if(pIoSession == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found iosession!{EntityType=%d, ServerID=%d}\n", nEntityType, nServerID);
		return 0;
	}

	int32_t nRet = stMsgHeadCS.Encode(arrBuf, sizeof(arrBuf), nOffset);
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

	uint16_t nTotalSize = (uint16_t)nOffset;
	nOffset = 1;
	nRet = CCodeEngine::Encode(arrBuf, sizeof(arrBuf), nOffset, nTotalSize);
	if(nRet < 0)
	{
		WRITE_WARN_LOG(SERVER_NAME, "encode total size failed!\n");
		return 0;
	}

	g_Frame.Dump(&stMsgHeadCS, pMsgBody, "send cs req ");

	return pIoSession->Write(arrBuf, nTotalSize);
}


int32_t CServerHelper::SendCSResp(int32_t nMsgID, IMsgHead *pMsgHead, IMsgBody *pMsgBody, IIOSession *pIoSession)
{
	MsgHeadCS *pMsgHeadCS = dynamic_cast<MsgHeadCS *>(pMsgHead);
	if(pMsgHeadCS == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msghead to msgheadcs error!\n");
		return 0;
	}

	uint32_t nOffset = 0;
	uint8_t arrBuf[enmMaxMessageSize];
	MsgHeadCS stMsgHeadCS;
	stMsgHeadCS.m_nMsgID = nMsgID;
	stMsgHeadCS.m_nRoomID = pMsgHeadCS->m_nRoomID;
	stMsgHeadCS.m_nUserID = pMsgHeadCS->m_nUserID;
	stMsgHeadCS.m_nSeq = pMsgHeadCS->m_nSeq;
	stMsgHeadCS.m_nTotalSize = 0;

	int32_t nRet = stMsgHeadCS.Encode(arrBuf, sizeof(arrBuf), nOffset);
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

	stMsgHeadCS.m_nTotalSize = (uint16_t)nOffset;
	nOffset = 1;
	nRet = CCodeEngine::Encode(arrBuf, sizeof(arrBuf), nOffset, stMsgHeadCS.m_nTotalSize);
	if(nRet < 0)
	{
		WRITE_WARN_LOG(SERVER_NAME, "encode total size failed!\n");
		return 0;
	}

	g_Frame.Dump(&stMsgHeadCS, pMsgBody, "send cs resp ");

	return pIoSession->Write(arrBuf, stMsgHeadCS.m_nTotalSize);
}


int32_t CServerHelper::SendCSNoti(int32_t nMsgID, IMsgBody *pMsgBody, EntityType nEntityType, ServerID nServerID)
{
	return 0;
}

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

	if(nEntityType != enmEntityType_Client)
	{
		pSessionParam->SetServerInfo(nEntityType, nID);
	}
}

void CServerHelper::UnregistSessionInfo(IIOSession *pIoSession)
{
	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	if(pSessionParam == NULL)
	{
		return;
	}

	EntityType nEntityType = enmEntityType_None;
	ServerID nServerID = enmInvalidServerID;

	pSessionParam->GetServerInfo(nEntityType, nServerID);

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

int32_t CServerHelper::GetAllIoSession(EntityType nEntityType, IIOSession *arrIoSession[], int32_t nSize)
{
	int32_t nCount = 0;
	EntityMap::iterator it = m_stEntityMap.find(nEntityType);
	if(it != m_stEntityMap.end())
	{
		IDMap::iterator et = it->second.begin();
		for(; et != it->second.end(); ++et)
		{
			arrIoSession[nCount++] = et->second;
			if(nCount >= nSize)
			{
				break;
			}
		}
	}

	return nCount;
}

int32_t CServerHelper::MakeTransMediaHeadBuf(RoomID nRoomID, UserID nUserID, uint8_t *pHeadBuf, int32_t nBodySize)
{
	uint32_t nOffset = 0;

	TransMediaHead stTransMediaHead;
	stTransMediaHead.m_nMsgID = MSGID_TRANSMEDIA_NOTI;
	stTransMediaHead.m_nRoomID = nRoomID;
	stTransMediaHead.m_nUserID = nUserID;
	stTransMediaHead.m_nTotalSize = stTransMediaHead.GetSize() + nBodySize;

	int32_t nRet = stTransMediaHead.Encode(pHeadBuf, stTransMediaHead.m_nTotalSize, nOffset);
	if(nRet != 0)
	{
		return 0;
	}

	return stTransMediaHead.GetSize();
}

