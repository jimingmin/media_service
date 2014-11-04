/*
 * media_channel.h
 *
 *  Created on: 2014年5月11日
 *      Author: jimm
 */

#ifndef MEDIA_CHANNEL_H_
#define MEDIA_CHANNEL_H_

#include "../common/common_object.h"
#include "../netevent/net_impl.h"
#include "media_receiver.h"
#include "datacenter.h"

#include <string>
using namespace std;
using namespace NETEVENT;

typedef uint8_t		ChannelType;
enum
{
	enmChannelType_User		= 0x00,		//用户连接
	enmChannelType_Server	= 0x01,		//服务器连接
};

class CMediaCache;

//flv格式数据流需要4个配置，第一为flv头，第二为meta信息，
//第三，四为音频或者视频配置信息，前2个包一开始就发，音频视频配置信息只在第一次出现音视频数据之前发
#define MEDIA_CONFIG_PACKET_COUNT	4

class CMediaChannel : public CBaseObject
{
public:
	CMediaChannel();
	virtual ~CMediaChannel();

	virtual int32_t Init();

	virtual int32_t Uninit();

	virtual int32_t GetSize();

	//订阅
	int32_t Join(CMediaChannel *pMediaChannel);
	//取消订阅
	int32_t Leave(CMediaChannel *pMediaChannel);
	//是否在接受者列表里面
	bool IsIn(CMediaChannel *pMediaChannel);
	//开始直播
	int32_t StartShow();
	//结束直播
	int32_t StopShow();
	//获取订阅者id
	int32_t GetSubscriberID();
	//获取表演者id
	UserID GetShowerID();
	//获取房间id
	RoomID GetRoomID();
	//设置iosession
	void SetIoSession(IIOSession *pIoSession);
	//获取iosession
	IIOSession *GetIoSession();
	//设置serverid
	void SetServerID(ServerID nServerID);
	//获取serverid
	ServerID GetServerID();
	//是否在本地服务器上
	bool IsLocalServer();
	//是否关联过远程服务器
	bool AttachRemoteServer();
	//设置channel key
	void SetChannelKey(RoomID nRoomID, UserID nShowerID, UserID nSubscriberID);
	//获取channel key
	string GetChannelKey();
	//设置channel type
	void SetChannelType(ChannelType nChannelType);
	//获取channel type
	ChannelType GetChannelType();
	//获取接受者数量
	int32_t GetReciverCount();
	//表演者是否关联远程服务器
	int32_t IsAttachRemoteServer();
	//是否是表演者
	bool IsShower();
	//缓存最近的N个媒体包
	void CacheMediaData(MediaType nMediaType, uint8_t *pBuf, int32_t nBufSize);
	//发送数据
	void SendMediaData(RoomID nRoomID, UserID nUserID, uint8_t *pBuf, int32_t nBufSize);

	void SaveMediaConfig(uint8_t *pBuf, int32_t nBufSize);

	void CacheMediaPacket(MediaType nMediaType, uint8_t *pBuf, int32_t nBufSize);

	int32_t SendMediaConfig(CMediaChannel *pMediaChannel);

	int32_t SendMediaCache(CMediaChannel *pMediaChannel);

protected:
	ChannelType			m_nChannelType;
	string				m_strChannelKey;
	RoomID				m_nRoomID;
	UserID				m_nShowerID;
	int32_t				m_nSubscriberID;
	bool				m_bIsShower;
	bool				m_bIsLocalServer;
	ServerID			m_nBelongServerID;
	IIOSession			*m_pIoSession;
	CMediaReceiver		*m_pReceiver;
	CMediaCache			*m_pMediaCache;

	int32_t				m_nCount;
	MediaPacket			*m_arrMediaConfig[MEDIA_CONFIG_PACKET_COUNT];
};

#endif /* MEDIA_CHANNEL_H_ */
