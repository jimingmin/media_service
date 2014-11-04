/*
 * server_typedef.h
 *
 *  Created on: 2014年5月6日
 *      Author: jimm
 */

#ifndef SERVER_TYPEDEF_H_
#define SERVER_TYPEDEF_H_

#include "../common/common_typedef.h"
#include "../common/common_object.h"
#include "../common/common_memmgt.h"
#include <stddef.h>
#include "../include/typedef.h"

#define SERVER_NAME			"media"
#define CONFIG_SERVER		"./server_config.xml"


typedef uint8_t			MediaType;
enum
{
	enmMediaDataType_Audio			= 0,
	enmMediaDataType_Video			= 1,
	enmMediaDataType_FlvHead		= 2,
	enmMediaDataType_MetaData		= 3,
	enmMediaDataType_AudioConfig	= 4,
	enmMediaDataType_VideoConfig	= 5,
	enmMediaDataType_KeyFrame		= 6,
};

typedef uint8_t			ServerRole;
enum
{
	enmServerRole_Edge		= 0,	//边缘
	enmServerRole_Source	= 1,	//源
};

struct MediaPacket : public CBaseObject
{
	MediaPacket()
	{
		Init();
	}

	virtual int32_t Init()
	{
		nMediaType = enmMediaDataType_Audio;
		nPacketSize = 0;
		pPacket = NULL;
		return 0;
	}

	virtual int32_t Uninit()
	{
		if(pPacket != NULL)
		{
			FREE(pPacket);
		}
		return 0;
	}

	virtual int32_t GetSize()
	{
		return sizeof(*this);
	}

	MediaType	nMediaType;
	int32_t		nPacketSize;
	uint8_t		*pPacket;
};

struct ServerInfo
{
	ServerInfo()
	{
		nEntityType = enmEntityType_None;
		nServerID = enmInvalidServerID;
	}

	EntityType		nEntityType;
	ServerID		nServerID;
};

struct RoomParam
{
	RoomParam()
	{
		nRoomID	= enmInvalidRoomID;
		nShowerID = enmInvalidUserID;
		nSubscriberID = enmInvalidUserID;
	}

	RoomParam(RoomID nRoomID, UserID nShowerID, UserID nSubscriberID)
	{
		this->nRoomID = nRoomID;
		this->nShowerID = nShowerID;
		this->nSubscriberID = nSubscriberID;
	}

	bool operator == (RoomParam &stRight)
	{
		if((nRoomID == stRight.nRoomID) &&
			(nShowerID == stRight.nShowerID) &&
			(nSubscriberID == stRight.nSubscriberID))
		{
			return true;
		}

		return false;
	}
	RoomID			nRoomID;
	UserID			nShowerID;
	UserID			nSubscriberID;
};

typedef uint8_t		SubscribeSource;
enum
{
	enmSubscribeSource_Client		= 0x00,
	enmSubscribeSource_Server		= 0x01,
};

#endif /* SERVER_TYPEDEF_H_ */

