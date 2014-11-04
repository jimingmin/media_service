/*
 * media_Receiver.h
 *
 *  Created on: 2014年5月12日
 *      Author: jimm
 */

#ifndef MEDIA_Receiver_H_
#define MEDIA_Receiver_H_

#include "client_message.h"
#include "../netevent/net_impl.h"
#include "../common/common_object.h"
#include "media_cache.h"
#include <list>
using namespace std;
using namespace NETEVENT;

class CMediaChannel;
typedef list<CMediaChannel *>		ReceiverList;	//观众列表，数据分发给哪些结点

////flv格式数据流需要4个配置，第一为flv头，第二为meta信息，
////第三，四为音频或者视频配置信息，前2个包一开始就发，音频视频配置信息只在第一次出现音视频数据之前发
//#define MEDIA_CONFIG_PACKET_COUNT	4

class CMediaReceiver : public CBaseObject
{
public:
	CMediaReceiver();

	virtual ~CMediaReceiver();

	virtual int32_t Init();

	virtual int32_t Uninit();

	virtual int32_t GetSize();

	void AddReceiver(CMediaChannel *pMediaChannel);

	void DelReceiver(CMediaChannel *pMediaChannel);

	int32_t UserCount();

	int32_t ServerCount();

	bool Empty();

	bool Find(CMediaChannel *pMediaChannel);

	void AllLeave(CMediaChannel *pMediaChannel);

//	void SaveMediaConfig(uint8_t *pBuf, int32_t nBufSize);
//
//	void CacheMediaPacket(MediaType nMediaType, uint8_t *pBuf, int32_t nBufSize);
//
//	int32_t SendMediaConfig(CMediaChannel *pMediaChannel);
//
//	int32_t SendMediaCache(CMediaChannel *pMediaChannel);

	int32_t SendMediaData(RoomID nRoomID, UserID nUserID, uint8_t *pBuf, int32_t nBufSize);

protected:
//	int32_t MakeTransMediaHeadBuf(RoomID nRoomID, UserID nUserID, uint8_t *pHeadBuf, int32_t nBodySize);

	ReceiverList		m_stUserList;
	ReceiverList		m_stServerList;

//	int32_t				m_nCount;
//	MediaPacket			*m_arrMediaConfig[MEDIA_CONFIG_PACKET_COUNT];

//	CMediaCache			*m_pMediaCache;
};



#endif /* MEDIA_Receiver_H_ */
