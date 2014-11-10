/*
 * flash_handler.cpp
 *
 *  Created on: 2014年2月20日
 *      Author: Administrator
 */

#include "../logger/logger.h"
#include "../frame/frame_msghandle.h"
#include "server_typedef.h"
#include "public_define.h"
#include "flash_handler.h"

using namespace LOGGER;

int32_t CFlashHandler::OnOpened(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "new session!{peeraddress=%s, peerport=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());

	//pIoSession->Write((uint8_t *)FLASH_SEC_RESP, (int32_t)sizeof(FLASH_SEC_RESP));
	m_pTimerHandler->AddSession(pIoSession);

	return 0;
}

int32_t CFlashHandler::OnRecved(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "recv message : [size=%d]:%s\n", nBufSize, (char *)pBuf);

	//if(strncmp((char *)pBuf, FLASH_SEC_REQ, strlen(FLASH_SEC_REQ)) == 0)
	{
		pIoSession->Write((uint8_t *)FLASH_SEC_RESP, (int32_t)sizeof(FLASH_SEC_RESP));
	}

	return 0;
}

int32_t CFlashHandler::OnSent(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "sent message : [size=%d]:%s\n", nBufSize, (char *)pBuf);
	pIoSession->Close();

	return 0;
}

int32_t CFlashHandler::OnClosed(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "session closed!{peeraddress=%s, peerport=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());

	m_pTimerHandler->DelSession(pIoSession);
	return 0;
}

int32_t CFlashHandler::OnError(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "session error!{peeraddress=%s, peerport=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());

	return 0;
}

int32_t CFlashHandler::OnTimeout(IIOSession *pIoSession)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "session timeout!{peeraddress=%s, peerport=%d}\n",
			pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());

	return 0;
}

void CFlashHandler::SetTimerHandler(CTimerHandler *pTimerHandler)
{
	m_pTimerHandler = pTimerHandler;
}

