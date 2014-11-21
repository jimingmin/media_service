/*
 * message_handler.cpp
 *
 *  Created on: 2014年7月6日
 *      Author: jimm
 */

#include "message_handler.h"
#include "server_typedef.h"
#include "../logger/logger.h"
#include "../frame/frame.h"
#include "../frame/frame_typedef.h"
#include "../common/common_codeengine.h"
#include "server_config.h"
#include "server_helper.h"

using namespace LOGGER;
using namespace FRAME;

int32_t CMessageHandler::OnOpened(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "new session!{peeraddress=%s, peerport=%d, socketfd=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort(), pIoSession->GetSocketFD());

	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	NetNode *pNetNode = pServerConfig->FindNetNode(pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
	if(pNetNode == NULL)
	{
		return 0;
	}

	//注册服务器连接
	CServerHelper::RegistSessionInfo((EntityType)pNetNode->m_nPeerType, pNetNode->m_nPeerID, pIoSession);

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
	switch(nHeadType)
	{
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




