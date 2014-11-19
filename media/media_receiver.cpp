/*
 * media_Receiver.cpp
 *
 *  Created on: 2014年5月13日
 *      Author: jimm
 */

#include "media_receiver.h"
#include "../common/common_memmgt.h"
#include "../common/common_api.h"
#include "media_channel.h"
#include "server_message.h"
#include "server_helper.h"
#include <algorithm>

CMediaReceiver::CMediaReceiver()
{
	Init();
}

CMediaReceiver::~CMediaReceiver()
{
}

int32_t CMediaReceiver::Init()
{
//	m_nCount = 0;
//	for(int32_t i = 0; i < MEDIA_CONFIG_PACKET_COUNT; ++i)
//	{
//		m_arrMediaConfig[i] = NULL;
//	}
//	m_pMediaCache = NULL;

	return 0;
}

int32_t CMediaReceiver::Uninit()
{
//	for(int32_t i = 0; i < m_nCount; ++i)
//	{
//		DELETE(m_arrMediaConfig[i]);
//	}
//	m_nCount = 0;
//
//	if(m_pMediaCache != NULL)
//	{
//		DELETE(m_pMediaCache);
//		m_pMediaCache = NULL;
//	}

	return 0;
}

int32_t CMediaReceiver::GetSize()
{
	return sizeof(*this);
}

void CMediaReceiver::AddReceiver(CMediaChannel *pMediaChannel)
{
	if(pMediaChannel->GetChannelType() == enmChannelType_User)
	{
		m_stUserList.push_back(pMediaChannel);
	}
	else
	{
		m_stServerList.push_back(pMediaChannel);
	}
}

void CMediaReceiver::DelReceiver(CMediaChannel *pMediaChannel)
{
	if(pMediaChannel->GetChannelType() == enmChannelType_User)
	{
		m_stUserList.remove(pMediaChannel);
	}
	else
	{
		m_stServerList.remove(pMediaChannel);
	}
}

int32_t CMediaReceiver::UserCount()
{
	return m_stUserList.size();
}

int32_t CMediaReceiver::ServerCount()
{
	return m_stServerList.size();
}

bool CMediaReceiver::Empty()
{
	return m_stUserList.empty();
}

bool CMediaReceiver::Find(CMediaChannel *pMediaChannel)
{
	ReceiverList::iterator it = find(m_stUserList.begin(), m_stUserList.end(), pMediaChannel);
	if(it != m_stUserList.end())
	{
		return true;
	}

	it = find(m_stServerList.begin(), m_stServerList.end(), pMediaChannel);
	if(it != m_stServerList.end())
	{
		return true;
	}

	return false;
}

void CMediaReceiver::AllLeave(CMediaChannel *pMediaChannel)
{
	if(!m_stUserList.empty())
	{
		ReceiverList stUserList = m_stUserList;
		for(ReceiverList::iterator it = stUserList.begin(); it != stUserList.end(); ++it)
		{
			(*it)->GetIoSession()->Close();
		}
	}

	if(!m_stServerList.empty())
	{
		ReceiverList stServerList = m_stServerList;
		for(ReceiverList::iterator it = stServerList.begin(); it != stServerList.end(); ++it)
		{
			//(*it)->Leave(pMediaChannel);
			pMediaChannel->Leave(*it);
			DELETE((*it));
		}
	}
}

//void CMediaReceiver::SaveMediaConfig(uint8_t *pBuf, int32_t nBufSize)
//{
////	ASSERT(m_nCount < MEDIA_CONFIG_PACKET_COUNT);
//
//	if(m_nCount >= MEDIA_CONFIG_PACKET_COUNT)
//	{
//		return;
//	}
//	MediaPacket *pMediaPacket = NEW(MediaPacket);
//	pMediaPacket->nPacketSize = nBufSize;
//	pMediaPacket->pPacket = MALLOC(nBufSize);
//	memcpy(pMediaPacket->pPacket, pBuf, nBufSize);
//	m_arrMediaConfig[m_nCount++] = pMediaPacket;
//}
//
//void CMediaReceiver::CacheMediaPacket(MediaType nMediaType, uint8_t *pBuf, int32_t nBufSize)
//{
//	if((nMediaType == enmMediaDataType_FlvHead) ||
//		(nMediaType == enmMediaDataType_MetaData) ||
//		(nMediaType == enmMediaDataType_AudioConfig) ||
//		(nMediaType == enmMediaDataType_VideoConfig))
//	{
//		SaveMediaConfig(pBuf, nBufSize);
//	}
//
//	if(m_pMediaCache == NULL)
//	{
//		m_pMediaCache = NEW(CMediaCache);
//	}
//	m_pMediaCache->CacheMediaPacket(nMediaType, pBuf, nBufSize);
//}
//
//int32_t CMediaReceiver::SendMediaConfig(CMediaChannel *pMediaChannel)
//{
//	for(int32_t i = 0; i < m_nCount; ++i)
//	{
//		int32_t nBufSize = m_arrMediaConfig[i]->nPacketSize;
//		uint8_t *pBuf = m_arrMediaConfig[i]->pPacket;
//		if(pMediaChannel->GetChannelType() == enmChannelType_Server)
//		{
//			int32_t nSendSize = 0;
//			uint8_t arrSend[enmMaxMessageSize];
//			int32_t nTransHeadSize = MakeTransMediaHeadBuf(pMediaChannel->GetRoomID(),
//					pMediaChannel->GetShowerID(), arrSend, nBufSize);
//
//			memcpy(&arrSend[nTransHeadSize], pBuf, nBufSize);
//			nSendSize = nTransHeadSize + nBufSize;
//
//			pMediaChannel->GetIoSession()->Write(arrSend, nSendSize);
//		}
//		else
//		{
//			pMediaChannel->GetIoSession()->Write(pBuf, nBufSize);
//		}
//	}
//
//	return 0;
//}
//
//int32_t CMediaReceiver::SendMediaCache(CMediaChannel *pMediaChannel)
//{
//	if(m_pMediaCache == NULL)
//	{
//		return 0;
//	}
//
//	ChannelType nChannelType = pMediaChannel->GetChannelType();
//	RoomID nRoomID = pMediaChannel->GetRoomID();
//	UserID nShowerID = pMediaChannel->GetShowerID();
//
//	list<MediaPacket *> stCacheList = m_pMediaCache->GetCacheList();
//	for(list<MediaPacket *>::iterator it = stCacheList.begin(); it != stCacheList.end(); ++it)
//	{
//		uint8_t *pBuf = (*it)->pPacket;
//		int32_t nBufSize = (*it)->nPacketSize;
//
//		if(nChannelType == enmChannelType_Server)
//		{
//			int32_t nSendSize = 0;
//			uint8_t arrSend[enmMaxMessageSize];
//
//			int32_t nTransHeadSize = MakeTransMediaHeadBuf(nRoomID, nShowerID, arrSend, nBufSize);
//
//			memcpy(&arrSend[nTransHeadSize], pBuf, nBufSize);
//			nSendSize = nTransHeadSize + nBufSize;
//
//			pMediaChannel->GetIoSession()->Write(arrSend, nSendSize);
//		}
//		else
//		{
//			pMediaChannel->GetIoSession()->Write(pBuf, nBufSize);
//		}
//	}
//
//	return 0;
//}

int32_t CMediaReceiver::SendMediaData(RoomID nRoomID, UserID nUserID, uint8_t *pBuf, int32_t nBufSize)
{
	if(!m_stUserList.empty())
	{
		for(ReceiverList::iterator it = m_stUserList.begin(); it != m_stUserList.end(); ++it)
		{
			(*it)->GetIoSession()->Write(pBuf, nBufSize);
		}
	}

	if(!m_stServerList.empty())
	{
		int32_t nSendSize = 0;
		uint8_t arrSend[enmMaxMessageSize];

		int32_t nTransHeadSize = CServerHelper::MakeTransMediaHeadBuf(nRoomID, nUserID, arrSend, nBufSize);

		memcpy(&arrSend[nTransHeadSize], pBuf, nBufSize);
		nSendSize = nTransHeadSize + nBufSize;

		for(ReceiverList::iterator it = m_stServerList.begin(); it != m_stServerList.end(); ++it)
		{
			(*it)->GetIoSession()->Write(arrSend, nSendSize);
		}
	}

	return nBufSize;
}


