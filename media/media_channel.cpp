/*
 * media_channel.cpp
 *
 *  Created on: 2014年5月11日
 *      Author: jimm
 */

#include "media_channel.h"
#include "server_helper.h"
#include "session_param.h"
#include "../common/common_memmgt.h"
#include "server_message.h"
#include "server_config.h"
#include "server_helper.h"
#include "../include/message_define.h"

CMediaChannel::CMediaChannel()
{
	Init();
}

CMediaChannel::~CMediaChannel()
{
}

int32_t CMediaChannel::Init()
{
	m_nChannelType = enmChannelType_User;
	m_nRoomID = enmInvalidRoomID;
	m_nShowerID = enmInvalidUserID;
	m_nSubscriberID = enmInvalidUserID;
	m_bIsShower	= false;
	m_bIsLocalServer = false;
	m_nBelongServerID = enmInvalidServerID;
	m_pIoSession = NULL;
	m_pReceiver = NULL;
	m_nCount = 0;
	for(int32_t i = 0; i < MEDIA_CONFIG_PACKET_COUNT; ++i)
	{
		m_arrMediaConfig[i] = NULL;
	}
	m_pMediaCache = NULL;
	return 0;
}

int32_t CMediaChannel::Uninit()
{
	if(m_pReceiver != NULL)
	{
		//减少表演者数量
		g_DataCenter.DecShowerCount();

		m_pReceiver->AllLeave(this);
		DELETE(m_pReceiver);

		UpdateServerLoadNoti stUpdateServerLoadNoti;
		stUpdateServerLoadNoti.m_stLoadInfo = g_DataCenter.GetServerLoadInfo();
		CServerHelper::SendSSNoti(MSGID_UPDATESERVERLOAD_NOTI, &stUpdateServerLoadNoti, m_nRoomID,
				m_nSubscriberID, enmTransType_Server, enmEntityType_MediaDispatch, enmInvalidServerID);
	}

	for(int32_t i = 0; i < m_nCount; ++i)
	{
		DELETE(m_arrMediaConfig[i]);
	}
	m_nCount = 0;

	if(m_pMediaCache != NULL)
	{
		DELETE(m_pMediaCache);
		m_pMediaCache = NULL;
	}

	return 0;
}

int32_t CMediaChannel::GetSize()
{
	return sizeof(*this);
}

//订阅
int32_t CMediaChannel::Join(CMediaChannel *pMediaChannel)
{
	if(pMediaChannel == NULL)
	{
		return 0;
	}

	g_DataCenter.AddChannel(pMediaChannel);

	bool bShowerLogin = false;
	if(m_pReceiver == NULL)
	{
		m_pReceiver = NEW(CMediaReceiver);
		g_DataCenter.IncShowerCount();
		bShowerLogin = true;
	}

	m_pReceiver->AddReceiver(pMediaChannel);
	SendMediaConfig(pMediaChannel);
	SendMediaCache(pMediaChannel);

	//增加订阅者数量
	int32_t nSubscriberCount = g_DataCenter.IncSubscriberCount();
	if(bShowerLogin || (nSubscriberCount % 10 == 0))
	{
		UpdateServerLoadNoti stUpdateServerLoadNoti;
		stUpdateServerLoadNoti.m_stLoadInfo = g_DataCenter.GetServerLoadInfo();
		CServerHelper::SendSSNoti(MSGID_UPDATESERVERLOAD_NOTI, &stUpdateServerLoadNoti, m_nRoomID,
				m_nSubscriberID, enmTransType_Server, enmEntityType_MediaDispatch, enmInvalidServerID);
	}

	return 0;
}

//取消订阅
int32_t CMediaChannel::Leave(CMediaChannel *pMediaChannel)
{
	if(pMediaChannel == NULL)
	{
		return 0;
	}

	g_DataCenter.DelChannel(pMediaChannel);

	if(m_pReceiver == NULL)
	{
		return 0;
	}

	m_pReceiver->DelReceiver(pMediaChannel);

//	if((m_pReceiver->UserCount() <= 0) && (m_nChannelType == enmChannelType_Server))
//	{
//		CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
//		ServerUnsubscribeReq stReq;
//		stReq.m_nShowerID = m_nShowerID;
//		CServerHelper::SendSSReq(MSGID_SERVERUNSUBSCRIBE_REQ, m_nRoomID, pServerConfig->GetServerID(), &stReq,
//				enmEntityType_Media, m_nBelongServerID);
//	}

	//减少订阅者数量
	int32_t nSubscriberCount = g_DataCenter.DecSubscriberCount();
	if(nSubscriberCount % 10 == 0)
	{
		UpdateServerLoadNoti stUpdateServerLoadNoti;
		stUpdateServerLoadNoti.m_stLoadInfo = g_DataCenter.GetServerLoadInfo();
		CServerHelper::SendSSNoti(MSGID_UPDATESERVERLOAD_NOTI, &stUpdateServerLoadNoti, m_nRoomID,
				m_nSubscriberID, enmTransType_Server, enmEntityType_MediaDispatch, enmInvalidServerID);
	}

	return m_pReceiver->UserCount();
}

//是否在接受者列表里面
bool CMediaChannel::IsIn(CMediaChannel *pMediaChannel)
{
	if(m_pReceiver == NULL)
	{
		return false;
	}
	return m_pReceiver->Find(pMediaChannel);
}

//开始直播
int32_t CMediaChannel::StartShow()
{
	if(IsLocalServer())
	{
		CTimerHandler *pTimerHandler = g_DataCenter.GetTimerHandler();
		pTimerHandler->RecordShowerHB(this);

		//加入所有本组内其它media
		IIOSession *arrIoSession[enmMaxNetNodeCount];
		int32_t nCount = CServerHelper::GetAllIoSession(enmEntityType_Media, arrIoSession, enmMaxNetNodeCount);
		for(int32_t i = 0; i < nCount; ++i)
		{
			CSessionParam *pSessionParam = (CSessionParam *)arrIoSession[i]->GetParamPtr();
			if(pSessionParam == NULL)
			{
				continue;
			}

			EntityType nEntityType = enmEntityType_None;
			ServerID nServerID = enmInvalidServerID;
			pSessionParam->GetServerInfo(nEntityType, nServerID);

			CMediaChannel *pSubscriber = NEW(CMediaChannel);
			pSubscriber->SetServerID(nServerID);
			pSubscriber->SetIoSession(arrIoSession[i]);
			pSubscriber->SetChannelKey(m_nRoomID, m_nShowerID, nServerID);
			pSubscriber->SetChannelType(enmChannelType_Server);

			Join(pSubscriber);
		}
	}

	g_DataCenter.AddChannel(this);

	return 0;
}

//结束直播
int32_t CMediaChannel::StopShow()
{
	if(IsLocalServer())
	{
		CTimerHandler *pTimerHandler = g_DataCenter.GetTimerHandler();
		pTimerHandler->DestroyShowerHB(this);
	}

	if(m_pReceiver != NULL)
	{
		m_pReceiver->AllLeave(this);
	}

	g_DataCenter.DelChannel(this);

	return 0;
}

//获取用户id
UserID CMediaChannel::GetSubscriberID()
{
	return m_nSubscriberID;
}

//获取表演者id
UserID CMediaChannel::GetShowerID()
{
	return m_nShowerID;
}

//获取房间id
RoomID CMediaChannel::GetRoomID()
{
	return m_nRoomID;
}

void CMediaChannel::SetIoSession(IIOSession *pIoSession)
{
	m_pIoSession = pIoSession;
}

//获取iosession
IIOSession *CMediaChannel::GetIoSession()
{
	return m_pIoSession;
}

//设置serverid
void CMediaChannel::SetServerID(ServerID nServerID)
{
	m_nBelongServerID = nServerID;

	//判断是否在本地服务器
	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	if(pServerConfig->GetServerID() == nServerID)
	{
		m_bIsLocalServer = true;
	}
}

//获取serverid
ServerID CMediaChannel::GetServerID()
{
	return m_nBelongServerID;
}

//是否在本地服务器上
bool CMediaChannel::IsLocalServer()
{
	return m_bIsLocalServer;
}

//设置channel key
void CMediaChannel::SetChannelKey(RoomID nRoomID, UserID nShowerID, UserID nSubscriberID)
{
	//m_strChannelKey = string(nRoomID) + string(nShowerID);
	char szKey[256];
	sprintf(szKey, "%d:%d:%d", nRoomID, nShowerID, nSubscriberID);
	m_strChannelKey = szKey;

	m_nRoomID = nRoomID;
	m_nShowerID = nShowerID;
	m_nSubscriberID = nSubscriberID;

	if(nShowerID == nSubscriberID)
	{
		m_bIsShower = true;
	}
}

//获取channel key
string CMediaChannel::GetChannelKey()
{
	return m_strChannelKey;
}

//设置channel type
void CMediaChannel::SetChannelType(ChannelType nChannelType)
{
	m_nChannelType = nChannelType;
}

//获取channel type
ChannelType CMediaChannel::GetChannelType()
{
	return m_nChannelType;
}

//获取接受者数量
int32_t CMediaChannel::GetReciverCount()
{
	if(m_pReceiver != NULL)
	{
		return m_pReceiver->UserCount();
	}

	return 0;
}

//是否是表演者
bool CMediaChannel::IsShower()
{
	return m_bIsShower;
}

////缓存最近的N个媒体包
//void CMediaChannel::CacheMediaData(MediaType nMediaType, uint8_t *pBuf, int32_t nBufSize)
//{
//	if(m_pReceiver == NULL)
//	{
//		m_pReceiver = NEW(CMediaReceiver);
//	}
//	m_pReceiver->CacheMediaPacket(nMediaType, pBuf, nBufSize);
//}

void CMediaChannel::SaveMediaConfig(uint8_t *pBuf, int32_t nBufSize)
{
//	ASSERT(m_nCount < MEDIA_CONFIG_PACKET_COUNT);

	if(m_nCount >= MEDIA_CONFIG_PACKET_COUNT)
	{
		return;
	}
	MediaPacket *pMediaPacket = NEW(MediaPacket);
	pMediaPacket->nPacketSize = nBufSize;
	pMediaPacket->pPacket = MALLOC(nBufSize);
	memcpy(pMediaPacket->pPacket, pBuf, nBufSize);
	m_arrMediaConfig[m_nCount++] = pMediaPacket;
}

//缓存最近的N个媒体包
void CMediaChannel::CacheMediaData(MediaType nMediaType, uint8_t *pBuf, int32_t nBufSize)
{
	if((nMediaType == enmMediaDataType_FlvHead) ||
		(nMediaType == enmMediaDataType_MetaData) ||
		(nMediaType == enmMediaDataType_AudioConfig) ||
		(nMediaType == enmMediaDataType_VideoConfig))
	{
		SaveMediaConfig(pBuf, nBufSize);
	}

	if(m_pMediaCache == NULL)
	{
		m_pMediaCache = NEW(CMediaCache);
	}
	m_pMediaCache->CacheMediaPacket(nMediaType, pBuf, nBufSize);
}

int32_t CMediaChannel::SendMediaConfig(CMediaChannel *pMediaChannel)
{
	for(int32_t i = 0; i < m_nCount; ++i)
	{
		int32_t nBufSize = m_arrMediaConfig[i]->nPacketSize;
		uint8_t *pBuf = m_arrMediaConfig[i]->pPacket;
		if(pMediaChannel->GetChannelType() == enmChannelType_Server)
		{
			int32_t nSendSize = 0;
			uint8_t arrSend[enmMaxMessageSize];
			int32_t nTransHeadSize = CServerHelper::MakeTransMediaHeadBuf(pMediaChannel->GetRoomID(),
					pMediaChannel->GetShowerID(), arrSend, nBufSize);

			memcpy(&arrSend[nTransHeadSize], pBuf, nBufSize);
			nSendSize = nTransHeadSize + nBufSize;

			pMediaChannel->GetIoSession()->Write(arrSend, nSendSize);
		}
		else
		{
			pMediaChannel->GetIoSession()->Write(pBuf, nBufSize);
		}
	}

	return 0;
}

int32_t CMediaChannel::SendMediaCache(CMediaChannel *pMediaChannel)
{
	if(m_pMediaCache == NULL)
	{
		return 0;
	}

	ChannelType nChannelType = pMediaChannel->GetChannelType();
	RoomID nRoomID = pMediaChannel->GetRoomID();
	UserID nShowerID = pMediaChannel->GetShowerID();

	list<MediaPacket *> stCacheList = m_pMediaCache->GetCacheList();
	for(list<MediaPacket *>::iterator it = stCacheList.begin(); it != stCacheList.end(); ++it)
	{
		uint8_t *pBuf = (*it)->pPacket;
		int32_t nBufSize = (*it)->nPacketSize;

		if(nChannelType == enmChannelType_Server)
		{
			int32_t nSendSize = 0;
			uint8_t arrSend[enmMaxMessageSize];

			int32_t nTransHeadSize = CServerHelper::MakeTransMediaHeadBuf(nRoomID, nShowerID, arrSend, nBufSize);

			memcpy(&arrSend[nTransHeadSize], pBuf, nBufSize);
			nSendSize = nTransHeadSize + nBufSize;

			pMediaChannel->GetIoSession()->Write(arrSend, nSendSize);
		}
		else
		{
			pMediaChannel->GetIoSession()->Write(pBuf, nBufSize);
		}
	}

	return 0;
}

//发送数据
void CMediaChannel::SendMediaData(RoomID nRoomID, UserID nUserID, uint8_t *pBuf, int32_t nBufSize)
{
	if(m_pReceiver != NULL)
	{
		m_pReceiver->SendMediaData(nRoomID, nUserID, pBuf, nBufSize);
	}
}

