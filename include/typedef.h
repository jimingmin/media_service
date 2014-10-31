/*
 * typedef.h
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_

#include "../common/common_typedef.h"
#include "../common/common_object.h"
#include <string.h>

typedef int32_t			UserID;
enum
{
	enmInvalidUserID		= 0,
};

typedef int32_t			RoomID;
enum
{
	enmInvalidRoomID		= 0,
};

typedef int32_t			ServerID;
enum
{
	enmInvalidServerID		= 0,
};

enum
{
	enmMaxConfigFileNameSize	= 1024,		//配置文件名长度
	enmMaxPacketTokenSize		= 64,		//最大包分割字符串长度
	enmIPV4NetAddressSize		= 16,		//IPV4网络地址长度
	enmMaxNetNodeCount			= 32,		//最大连接的服务器数量
};

typedef uint8_t			HeadType;
enum
{
	enmInvalidHeadType			= 0x00,
	enmHeadType_UserMedia		= 0x01,
	enmHeadType_MsgCS			= 0x02,
	enmHeadType_MsgSS			= 0x03,
	enmHeadType_ServerMedia		= 0x04,
};

typedef uint8_t			TransType;
enum
{
	enmTransType_P2P		= 0,
	enmTransType_Room		= 1,
	enmTransType_Server		= 2,
	enmTransType_Random		= 3,
};

typedef uint8_t			EntityType;
enum
{
	enmEntityType_None			= 0,
	enmEntityType_Gate			= 1,	//网关
	enmEntityType_Room			= 2,	//房间
	enmEntityType_Hall			= 3,	//大厅
	enmEntityType_DBProxy		= 4,	//db代理
	enmEntityType_Zone			= 5,	//区域服务器
	enmEntityType_Media			= 6,	//媒体服务器
	enmEntityType_MediaDispatch = 7,	//媒体负载服务器
	enmEntityType_CacheProxy	= 8,	//缓存代理服务器
	enmEntityType_Client	= 255,	//客户端
};

#define MaxUserCountPerServer		3000
#define MaxShowerCountPerServer		1000

struct LoadInfo
{
	LoadInfo()
	{
		nShowerCount = 0;
		nSubscriberCount = 0;
	}

	int32_t		nShowerCount;
	int32_t		nSubscriberCount;
};

struct NetNode
{
public:
	NetNode()
	{
		memset(m_szPeerAddress, 0, sizeof(m_szPeerAddress));
		m_nPeerPort = 0;
		m_nPeerType = enmEntityType_None;
		m_nPeerID = 0;
	}

	char			m_szPeerAddress[enmIPV4NetAddressSize];
	uint16_t		m_nPeerPort;
	int32_t			m_nPeerType;
	int32_t			m_nPeerID;
};

#endif /* TYPEDEF_H_ */
