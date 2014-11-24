/*
 * client_message.h
 *
 *  Created on: 2014年5月7日
 *      Author: jimm
 */

#ifndef CLIENT_MESSAGE_H_
#define CLIENT_MESSAGE_H_

#include "../common/common_codeengine.h"
#include "../common/common_errordef.h"
#include "../frame/frame_typedef.h"
#include "server_typedef.h"

#include <string>
using namespace std;

using namespace FRAME;

class MediaHead : public IMsgHead
{
public:
	MediaHead()
	{
		m_nHeadType = enmHeadType_UserMedia;
		m_nTotalSize = 0;
		m_nMsgID = 0;
		m_nMediaType = enmMediaDataType_Audio;
		m_nSeq = 0;
		m_nFragSeq = 0;
		m_nFragFin = 0;
	}

	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nHeadType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nTotalSize);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nMsgID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nMediaType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nSeq);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nFragSeq);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nFragFin);
		if(nRet < 0)
		{
			return nRet;
		}

		return S_OK;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nHeadType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nTotalSize);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nMsgID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nMediaType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nSeq);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nFragSeq);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nFragFin);
		if(nRet < 0)
		{
			return nRet;
		}

		return S_OK;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "{mediahead:{m_nHeadType=%d, m_nTotalSize=%d, m_nMsgID=0x%08x, "
			"m_nMediaType=%d, m_nSeq=%u, m_nFragSeq=%d, m_nFragFin=%d}, ",
			m_nHeadType, m_nTotalSize, m_nMsgID, m_nMediaType, m_nSeq, m_nFragSeq, m_nFragFin);
		offset += len;
	}

public:
	HeadType		m_nHeadType;	//1表示media头
	uint16_t		m_nTotalSize;	//数据包总大小
	uint8_t			m_nMsgID;		//消息ID
	MediaType		m_nMediaType;	//此数据类型
	uint32_t		m_nSeq;			//顺序号
	uint16_t		m_nFragSeq;		//分片包的顺序号
	uint8_t			m_nFragFin;		//分片是否结束
};

class MsgHeadCS : public IMsgHead
{
public:
	MsgHeadCS()
	{
		m_nHeadType = enmHeadType_MsgCS;
		m_nTotalSize = 0;
		m_nMsgID = 0;
		m_nRoomID = 0;
		m_nUserID = 0;
		m_nSeq = 0;
	}

	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nHeadType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nTotalSize);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nMsgID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nRoomID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nUserID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nSeq);
		if(nRet < 0)
		{
			return nRet;
		}

		return S_OK;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nHeadType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nTotalSize);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nMsgID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nRoomID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nUserID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nSeq);
		if(nRet < 0)
		{
			return nRet;
		}

		return S_OK;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "{msgheadcs:{m_nHeadType=%d, m_nTotalSize=%d, m_nMsgID=0x%08x, "
				"m_nRoomID=%d, m_nUserID=%d, m_nSeq=%d}, ",
				m_nHeadType, m_nTotalSize, m_nMsgID, m_nRoomID, m_nUserID, m_nSeq);
		offset += len;
	}

public:
	HeadType		m_nHeadType;	//2表示msghead头
	uint16_t		m_nTotalSize;	//数据包总大小
	uint32_t		m_nMsgID;		//消息ID
	RoomID			m_nRoomID;		//房间id
	UserID			m_nUserID;		//用户id
	uint32_t		m_nSeq;			//顺序号
};

#define MSGID_USERREGIST_REQ		0x00002001
class UserRegistReq : public IMsgBody
{
public:
	UserRegistReq()
	{
		m_nUserID = enmInvalidUserID;
		m_strToken = "";
	}

	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nUserID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_strToken);
		if(nRet < 0)
		{
			return nRet;
		}

		return S_OK;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nUserID);
		if(nRet < 0)
		{
			return nRet;
		}
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_strToken);
		if(nRet < 0)
		{
			return nRet;
		}

		return S_OK;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "UserRegistReq msgbody:{m_nUserID=%d, m_strToken=%s}}",
				 m_nUserID, m_strToken.c_str());
		offset += len;
	}

public:
	UserID			m_nUserID;
	string			m_strToken;
};

#define MSGID_USERREGIST_RESP		0x00002101
class UserRegistResp : public IMsgBody
{
public:
	UserRegistResp()
	{
		m_nResult = 0;
	}

	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nResult);
		if(nRet < 0)
		{
			return nRet;
		}
		return nRet;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nResult);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "UserRegistResp msgbody:{m_nResult=%d}}", m_nResult);
		offset += len;
	}

public:
	int32_t			m_nResult;
};

#define MSGID_USERUNREGIST_REQ		0x00002002
class UserUnregistReq : public IMsgBody
{
public:
	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nShowerID);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nShowerID);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "UserUnregistReq msgbody:{m_nShowerID=%d}}", m_nShowerID);
		offset += len;
	}

public:
	UserID				m_nShowerID;
};

#define MSGID_TRANSMITDATA_NOTI		0x01
class TransmitDataNoti : public IMsgBody
{
public:
	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;

		return nRet;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "TransmitDataNoti msgbody:{}}");
		offset += len;
	}
};

#define MSGID_USERSUBSCRIBE_REQ			0x00002003
class UserSubscribeReq : public IMsgBody
{
public:
	UserSubscribeReq()
	{
		m_nUserID = enmInvalidUserID;
	}

	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nUserID);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nUserID);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "UserSubscribeReq msgbody:{m_nUserID=%d}}", m_nUserID);
		offset += len;
	}

public:
	UserID				m_nUserID;		//订阅哪个user的数据
};

#define MSGID_USERSUBSCRIBE_RESP		0x00002103
class UserSubscribeResp : public IMsgBody
{
public:
	UserSubscribeResp()
	{
		m_nResult = 0;
	}

	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nResult);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nResult);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "UserSubscribeResp msgbody:{m_nResult=%d}}", m_nResult);
		offset += len;
	}

public:
	int32_t				m_nResult;
};

#define MSGID_USERUNSUBSCRIBE_REQ		0x00002004
class UserUnsubscribeReq : public IMsgBody
{
public:
	UserUnsubscribeReq()
	{
		m_nUserID = enmInvalidUserID;
	}

	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nUserID);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nUserID);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "UserUnsubscribeReq msgbody:{m_nUserID=%d}}", m_nUserID);
		offset += len;
	}

public:
	UserID				m_nUserID;		//退订哪个user的数据
};

#define MSGID_STARTPUBLISH_REQ		0x00002005
class StartPublishReq : public IMsgBody
{
public:
	StartPublishReq()
	{
		m_nShowerID = enmInvalidUserID;
	}

	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nShowerID);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nShowerID);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "StartPublishReq msgbody:{m_nShowerID=%d}}", m_nShowerID);
		offset += len;
	}

public:
	UserID				m_nShowerID;		//哪个user准备好发布了
};

#define MSGID_STARTPUBLISH_RESP		0x00002105
class StartPublishResp : public IMsgBody
{
public:
	StartPublishResp()
	{
		m_nResult = 0;
	}

	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nResult);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nResult);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "StartPublishResp msgbody:{m_nResult=%d}}", m_nResult);
		offset += len;
	}

public:
	int32_t			m_nResult;
};


#define MSGID_STOPPUBLISH_REQ		0x00002006
class StopPublishReq : public IMsgBody
{
public:
	StopPublishReq()
	{
		m_nShowerID = enmInvalidUserID;
	}

	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nShowerID);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nShowerID);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "StopPublishReq msgbody:{m_nShowerID=%d}}", m_nShowerID);
		offset += len;
	}

public:
	UserID				m_nShowerID;		//哪个user停止发布了
};

#define MSGID_HEARTBEAT_REQ		0x00002007
class HeartBeatReq : public IMsgBody
{
public:
	HeartBeatReq()
	{
	}

	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;

		return nRet;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "HeartBeatReq msgbody:{}}");
		offset += len;
	}
};

#endif /* CLIENT_MESSAGE_H_ */

