/*
 * media_handler.cpp
 *
 *  Created on: 2014年5月12日
 *      Author: jimm
 */

#include "media_handler.h"
#include "server_typedef.h"
#include "../logger/logger.h"
#include "media_channel.h"
#include "datacenter.h"
#include "server_message.h"

using namespace LOGGER;

int32_t CMediaHandler::TransmitMedia(CBaseObject *pObject, IMsgHead *pMsgHead, uint8_t *pBuf, int32_t nBufSize)
{
	//WRITE_DEBUG_LOG(SERVER_NAME, "transmit media\n");
	IIOSession *pIoSession = dynamic_cast<IIOSession *>(pObject);
	if(pIoSession == NULL)
	{
		WRITE_DEBUG_LOG(SERVER_NAME, "translate object to iosession error!\n");
		return 0;
	}

	MediaHead *pMediaHead = dynamic_cast<MediaHead *>(pMsgHead);
	if(pMediaHead == NULL)
	{
		WRITE_DEBUG_LOG(SERVER_NAME, "translate msghead to mediahead error!\n");
		return 0;
	}

	CMediaChannel *pMediaChannel = g_DataCenter.FindChannel(pIoSession);
	if((pMediaChannel == NULL) || (pMediaChannel->GetChannelType() != enmChannelType_User))
	{
		WRITE_DEBUG_LOG(SERVER_NAME, "it's not found mediachannel by iosession!{peeraddress=%s, "
				"peerport=%d}\n", pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
		return 0;
	}

	pMediaChannel->CacheMediaData(pMediaHead->m_nMediaType, pBuf, nBufSize);

	pMediaChannel->SendMediaData(pMediaChannel->GetRoomID(), pMediaChannel->GetShowerID(), pBuf, nBufSize);

	return 0;
}

int32_t CMediaHandler::TransmitServerMedia(CBaseObject *pObject, IMsgHead *pMsgHead, uint8_t *pBuf, int32_t nBufSize)
{
	IIOSession *pIoSession = dynamic_cast<IIOSession *>(pObject);
	if(pIoSession == NULL)
	{
		WRITE_DEBUG_LOG(SERVER_NAME, "translate object to iosession error!\n");
		return 0;
	}

	TransMediaHead *pTransMediaHead = dynamic_cast<TransMediaHead *>(pMsgHead);
	if(pTransMediaHead == NULL)
	{
		WRITE_DEBUG_LOG(SERVER_NAME, "translate msghead to transmediahead error!\n");
		return 0;
	}

	CMediaChannel *pMediaChannel = g_DataCenter.FindChannel(pTransMediaHead->m_nRoomID, pTransMediaHead->m_nUserID, pTransMediaHead->m_nUserID);
	if(pMediaChannel == NULL)
	{
		WRITE_DEBUG_LOG(SERVER_NAME, "it's not found mediachannel by iosession!{peeraddress=%s, "
				"peerport=%d}\n", pIoSession->GetPeerAddressStr(), pIoSession->GetPeerPort());
		return 0;
	}

	uint8_t *pMediaPacket = pBuf + pTransMediaHead->GetSize();
	int32_t nMediaSize = nBufSize - pTransMediaHead->GetSize();

	uint32_t nOffset = 0;
	MediaHead stMediaHead;
	int32_t nRet = stMediaHead.Decode(pMediaPacket, nMediaSize, nOffset);
	if(nRet != 0)
	{
		return 0;
	}

	pMediaChannel->CacheMediaData(stMediaHead.m_nMediaType, pMediaPacket, nMediaSize);

	pMediaChannel->SendMediaData(pMediaChannel->GetRoomID(), pMediaChannel->GetShowerID(), pMediaPacket, nMediaSize);

	return 0;
}
