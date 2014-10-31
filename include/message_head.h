/*
 * message_head.h
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#ifndef MESSAGE_HEAD_H_
#define MESSAGE_HEAD_H_

#include "../common/common_codeengine.h"
#include "../common/common_errordef.h"
#include "../frame/frame_typedef.h"
#include "typedef.h"

using namespace FRAME;

class MsgHeadSS : public IMsgHead
{
public:
	MsgHeadSS()
	{
		m_nHeadType = enmHeadType_MsgSS;
		m_nTotalSize = 0;
		m_nHeadSize = 0;
		m_nMsgID = 0;
		m_nRoomID = enmInvalidRoomID;
		m_nUserID = enmInvalidUserID;
		m_nTransType = enmTransType_P2P;
		m_nSrcType = enmEntityType_None;
		m_nSrcID = enmInvalidServerID;
		m_nDstType = enmEntityType_None;
		m_nDstID = enmInvalidServerID;
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

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nHeadSize);
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

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nTransType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nSrcType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nSrcID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nDstType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nDstID);
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

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nHeadSize);
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

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nTransType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nSrcType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nSrcID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nDstType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nDstID);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "{msgheadss:{m_nHeadType=%d, m_nTotalSize=%d, "
				"m_nHeadSize=%d, m_nMsgID=0x%08x, m_nRoomID=%d, m_nUserID=%d, m_nTransType=%d, "
				"m_nSrcType=%d, m_nSrcID=%d, m_nDstType=%d, m_nDstID=%d}, ",
				m_nHeadType, m_nTotalSize, m_nHeadSize, m_nMsgID, m_nRoomID, m_nUserID, m_nTransType,
				m_nSrcType, m_nSrcID, m_nDstType, m_nDstID);
		offset += len;
	}

	int32_t GetSize()
	{
		return sizeof(m_nHeadType) + sizeof(m_nTotalSize) + sizeof(m_nHeadSize) + sizeof(m_nMsgID)
			+ sizeof(m_nRoomID) + sizeof(m_nUserID) + sizeof(m_nTransType)
			+ sizeof(m_nSrcType) + sizeof(m_nSrcID) + sizeof(m_nDstType)
			+ sizeof(m_nDstID);
	}

public:
	HeadType			m_nHeadType;
	uint16_t			m_nTotalSize;
	uint16_t			m_nHeadSize;
	uint32_t			m_nMsgID;
	RoomID				m_nRoomID;
	UserID				m_nUserID;
	uint8_t				m_nTransType;
	EntityType			m_nSrcType;
	ServerID			m_nSrcID;
	EntityType			m_nDstType;
	ServerID			m_nDstID;
};

#endif /* MESSAGE_HEAD_H_ */
