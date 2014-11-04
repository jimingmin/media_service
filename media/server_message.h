/*
 * server_message.h
 *
 *  Created on: 2014年5月14日
 *      Author: jimm
 */

#ifndef SERVER_MESSAGE_H_
#define SERVER_MESSAGE_H_

#include "../common/common_codeengine.h"
#include "../common/common_errordef.h"
#include "../frame/frame_typedef.h"
#include "server_typedef.h"

#include <string>
using namespace std;

using namespace FRAME;

class TransMediaHead : public IMsgHead
{
public:
	TransMediaHead()
	{
		m_nHeadType = enmHeadType_ServerMedia;
		m_nTotalSize = 0;
		m_nMsgID = 0;
		m_nRoomID = enmInvalidRoomID;
		m_nUserID = enmInvalidUserID;
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

		return nRet;
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

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "{transmediahead:{m_nHeadType=%d, m_nTotalSize=%d, "
				"m_nMsgID=0x%08x, m_nRoomID=%d, m_nUserID=%d}, ",
				m_nHeadType, m_nTotalSize, m_nMsgID, m_nRoomID, m_nUserID);
		offset += len;
	}

	int32_t GetSize()
	{
		return sizeof(m_nTotalSize) + sizeof(m_nHeadType) + sizeof(m_nMsgID)
			+ sizeof(m_nRoomID) + sizeof(m_nUserID);
	}

public:
	HeadType			m_nHeadType;
	uint16_t			m_nTotalSize;
	uint32_t			m_nMsgID;
	RoomID				m_nRoomID;
	UserID				m_nUserID;
};

#define MSGID_TRANSMEDIA_NOTI				0x00061201
class TransmitMediaNoti : public IMsgBody
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
		int32_t len = sprintf(buf + offset, "TransmitMediaNoti msgbody:{}}");
		offset += len;
	}
};

#define MSGID_SERVERSUBSCRIBE_REQ			0x00061002
class ServerSubscribeReq : public IMsgBody
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
		int32_t len = sprintf(buf + offset, "ServerSubscribeReq msgbody:{m_nShowerID=%d}}", m_nShowerID);
		offset += len;
	}

public:
	UserID			m_nShowerID;
};

#define MSGID_SERVERSUBSCRIBE_RESP			0x00061102
class ServerSubscribeResp : public IMsgBody
{
public:
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
		int32_t len = sprintf(buf + offset, "ServerSubscribeResp msgbody:{m_nResult=%d}}", m_nResult);
		offset += len;
	}

public:
	int32_t			m_nResult;
};


#define MSGID_SERVERUNSUBSCRIBE_REQ			0x00061003
class ServerUnsubscribeReq : public IMsgBody
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
		int32_t len = sprintf(buf + offset, "ServerUnsubscribeReq msgbody:{m_nShowerID=%d}}", m_nShowerID);
		offset += len;
	}

public:
	UserID			m_nShowerID;
};

#endif /* SERVER_MESSAGE_H_ */

