/*
 * message_handler.cpp
 *
 *  Created on: 2014年5月7日
 *      Author: jimm
 */

#include "message_handler.h"
#include "server_typedef.h"
#include "../include/message_head.h"
#include "../include/message_define.h"
#include "server_message.h"
#include "client_message.h"
#include "../logger/logger.h"
#include "../frame/frame.h"
#include "../frame/frame_typedef.h"
#include "media_channel.h"
#include "session_param.h"
#include "server_config.h"
#include "server_helper.h"

using namespace LOGGER;
using namespace FRAME;

int32_t CMessageHandler::OnOpened(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "new session!{peeraddress=%s, peerport=%d, socketfd=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort(), pIoSession->GetSocketFD());

	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	int32_t nConnCount = g_DataCenter.IncConnCount();
	if(nConnCount > pServerConfig->GetMaxConnCount())
	{
		WRITE_DEBUG_LOG(SERVER_NAME, "current connections number is bigger than max_connections!"
				"{nConnCount=%d, nMaxConnCount=%d}\n", nConnCount, pServerConfig->GetMaxConnCount());
		pIoSession->Close();
		return 0;
	}

	NetNode *pNetNode = pServerConfig->FindNetNode(pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	if(pNetNode == NULL)
	{
		CTimerHandler *pTimerHandler = g_DataCenter.GetTimerHandler();
		pTimerHandler->AddSessionHB(pIoSession);
		return 0;
	}

	//注册服务器连接
	CServerHelper::RegistSessionInfo((EntityType)pNetNode->m_nPeerType, pNetNode->m_nPeerID, pIoSession);

	//向远端服务器发送注册请求
	ServerRegistReq stReq;
	stReq.m_nGroupID = pServerConfig->GetGroupID();
	stReq.m_nEntityType = pServerConfig->GetServerType();
	stReq.m_nServerID = pServerConfig->GetServerID();

	CServerHelper::SendSSReq(MSGID_SERVERREGIST_REQ, enmInvalidRoomID, enmInvalidUserID, &stReq,
			pNetNode->m_nPeerType, pNetNode->m_nPeerID);

	ShowerListNoti stShowerListInfo;
	stShowerListInfo.m_nShowerCount = g_DataCenter.GetAllLocalShower(stShowerListInfo.m_arrRoomID, stShowerListInfo.m_arrShowerID, MaxShowerCountPerServer);
	CServerHelper::SendSSNoti(MSGID_SHOWERLIST_NOTI, &stShowerListInfo, enmInvalidRoomID, enmInvalidUserID,
			enmTransType_P2P, (EntityType)pNetNode->m_nPeerType, pNetNode->m_nPeerID);

	return 0;
}

int32_t CMessageHandler::OnRecved(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize)
{
	//WRITE_DEBUG_LOG(SERVER_NAME, "recv message:[size=%d]:%s\n", nBufSize, (char *)pBuf);

	uint32_t nOffset = 0;
	uint8_t nHeadType = 0;
	int32_t nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, nHeadType);
	if(nRet < 0)
	{
		return 0;
	}

	int32_t nMsgID = 0;
	//mediahead
	switch(nHeadType)
	{
		case enmHeadType_ServerMedia:
		{
			uint32_t nID = 0;
			nOffset += sizeof(uint16_t);
			nRet = CCodeEngine::Decode(pBuf, nBufSize - nOffset, nOffset, nID);
			if(nRet < 0)
			{
				return 0;
			}
			nMsgID = nID;
			break;
		}
		case enmHeadType_UserMedia:
		{
			uint8_t nID = 0;
			nOffset += sizeof(uint16_t);
			nRet = CCodeEngine::Decode(pBuf, nBufSize - nOffset, nOffset, nID);
			if(nRet < 0)
			{
				return 0;
			}
			nMsgID = nID;
			break;
		}
		case enmHeadType_MsgCS:
		{
			uint32_t nID = 0;
			nOffset += sizeof(uint16_t);
			nRet = CCodeEngine::Decode(pBuf, nBufSize - nOffset, nOffset, nID);
			if(nRet < 0)
			{
				return 0;
			}
			nMsgID = nID;
			break;
		}
		case enmHeadType_MsgSS:
		{
			uint32_t nID = 0;
			nOffset += (sizeof(uint16_t) + sizeof(uint16_t));
			nRet = CCodeEngine::Decode(pBuf, nBufSize - nOffset, nOffset, nID);
			if(nRet < 0)
			{
				return 0;
			}
			nMsgID = nID;
			break;
		}
		default:
		{
			WRITE_WARN_LOG(SERVER_NAME, "it's unknow headtype!{headtype=%d}\n", nHeadType);
			return 0;
		}
	}


	nRet = g_Frame.FrameCallBack(nMsgID, pIoSession, pBuf, nBufSize);
	if(nRet == -1)
	{
		WRITE_WARN_LOG(SERVER_NAME, "frame call back return fail!\n");
	}
	else if(nRet == 1)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found msg handler!{msgid=0x%04x}\n", nMsgID);
	}
	else if(nRet == 2)
	{
		WRITE_WARN_LOG(SERVER_NAME, "decode msg failed!{msgid=0x%04x}\n", nMsgID);
	}

	return 0;
}

int32_t CMessageHandler::OnSent(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize)
{
	//WRITE_DEBUG_LOG(SERVER_NAME, "send data!\n");
	return 0;
}

int32_t CMessageHandler::OnClosed(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "session closed!{peeraddress=%s, peerport=%d, socketfd=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort(), pIoSession->GetSocketFD());

	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	NetNode *pNetNode = pServerConfig->FindNetNode(pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	//用户连接是有心跳记录的
	if(pNetNode == NULL)
	{
		CTimerHandler *pTimerHandler = g_DataCenter.GetTimerHandler();
		pTimerHandler->DelSessionHB(pIoSession);
	}

	g_DataCenter.DecConnCount();

	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	if(pSessionParam != NULL)
	{
		MsgHeadCS stMsgHeadCS;
		stMsgHeadCS.m_nMsgID = MSGID_USERUNREGIST_REQ;

		for(RoomParam stRoomParam = pSessionParam->FirstRoomParam();(stRoomParam.nRoomID != enmInvalidRoomID) &&
			(stRoomParam.nSubscriberID != enmInvalidUserID) && (stRoomParam.nShowerID != enmInvalidUserID);)
		{
			stMsgHeadCS.m_nRoomID = stRoomParam.nRoomID;
			stMsgHeadCS.m_nUserID = stRoomParam.nSubscriberID;

			UserUnregistReq stUserUnregistReq;
			stUserUnregistReq.m_nShowerID = stRoomParam.nShowerID;

			uint32_t nOffset = 0;
			uint8_t arrBuf[enmMaxMessageSize];

			int32_t nRet = stMsgHeadCS.Encode(arrBuf, sizeof(arrBuf), nOffset);
			if(nRet < 0)
			{
				continue;
			}

			nRet = stUserUnregistReq.Encode(arrBuf, sizeof(arrBuf) - nOffset, nOffset);
			if(nRet < 0)
			{
				continue;
			}

			uint16_t nTotalSize = nOffset;
			nOffset = 1;

			nRet = CCodeEngine::Encode(arrBuf, sizeof(arrBuf), nOffset, nTotalSize);
			if(nRet < 0)
			{
				continue;
			}

			nRet = g_Frame.FrameCallBack(stMsgHeadCS.m_nMsgID, pIoSession, arrBuf, nTotalSize);
			if(nRet == -1)
			{
				WRITE_WARN_LOG(SERVER_NAME, "frame call back return fail!\n");
			}

			stRoomParam.nRoomID = enmInvalidRoomID;
			stRoomParam.nShowerID = enmInvalidUserID;
			stRoomParam.nSubscriberID = enmInvalidUserID;
		}

		//EntityType nEntityType = enmEntityType_None;
		//ServerID nServerID = enmInvalidServerID;

		//MsgHeadSS stMsgHeadSS;
		//stMsgHeadSS.m_nHeadSize = stMsgHeadSS.GetSize();
		//stMsgHeadSS.m_nMsgID = MSGID_SERVERUNREGIST_REQ;
		//stMsgHeadSS.m_nRoomID = enmInvalidRoomID;
		//stMsgHeadSS.m_nUserID = enmInvalidUserID;
		//stMsgHeadSS.m_nTransType = enmTransType_P2P;
		//stMsgHeadSS.m_nDstType = pServerConfig->GetServerType();
		//stMsgHeadSS.m_nDstID = pServerConfig->GetServerID();

		//pSessionParam->GetServerInfo(nEntityType, nServerID);
		//if((nEntityType == enmEntityType_None) || (nServerID == enmInvalidServerID))
		//{
		//	NetNode *pNetNode = pServerConfig->FindNetNode(pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
		//	if(pNetNode == NULL)
		//	{
		//		return 0;
		//	}
		//	nEntityType = pNetNode->m_nPeerType;
		//	nServerID = pNetNode->m_nPeerID;
		//}

		//stMsgHeadSS.m_nSrcType = nEntityType;
		//stMsgHeadSS.m_nSrcID = nServerID;

		//uint32_t nOffset = 0;
		//uint8_t arrBuf[enmMaxMessageSize];

		//int32_t nRet = stMsgHeadSS.Encode(arrBuf, sizeof(arrBuf) - nOffset, nOffset);
		//if(nRet != 0)
		//{
		//	return nRet;
		//}

		//ServerUnregistReq stServerUnregistReq;
		//nRet = stServerUnregistReq.Encode(arrBuf, sizeof(arrBuf) - nOffset, nOffset);
		//if(nRet != 0)
		//{
		//	return nRet;
		//}

		//uint16_t nTotalSize = nOffset;
		//nOffset = 1;

		//nRet = CCodeEngine::Encode(arrBuf, sizeof(arrBuf), nOffset, nTotalSize);
		//if(nRet != 0)
		//{
		//	return nRet;
		//}

		//nRet = g_Frame.FrameCallBack(stMsgHeadSS.m_nMsgID, pIoSession, arrBuf, nTotalSize);
		//if(nRet == -1)
		//{
		//	WRITE_WARN_LOG(SERVER_NAME, "frame call back return fail!\n");
		//}

		CServerHelper::UnregistSessionInfo(pIoSession);
	}

	if(pNetNode != NULL)
	{
		m_pConnector->Connect(pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	}

	return 0;
}

int32_t CMessageHandler::OnError(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "session error!{peeraddress=%s, peerport=%d, socketfd=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort(), pIoSession->GetSocketFD());

	return 0;
}

int32_t CMessageHandler::OnTimeout(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "session timeout!{peeraddress=%s, peerport=%d, socketfd=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort(), pIoSession->GetSocketFD());

	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	NetNode *pNetNode = pServerConfig->FindNetNode(pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	if(pNetNode != NULL)
	{
		m_pConnector->Connect(pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	}

	return 0;
}

void CMessageHandler::SetConnector(CConnector *pConnector)
{
	m_pConnector = pConnector;
}

