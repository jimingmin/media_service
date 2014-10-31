/*
 * message_define.h
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#ifndef MESSAGE_DEFINE_H_
#define MESSAGE_DEFINE_H_

#include "../common/common_codeengine.h"
#include "../common/common_errordef.h"
#include "../frame/frame_typedef.h"
#include "typedef.h"
#include <stdio.h>

using namespace FRAME;

#define MSGID_SERVERREGIST_REQ		0xff000001
class ServerRegistReq : public IMsgBody
{
public:
	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nGroupID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nEntityType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nServerID);
		if(nRet < 0)
		{
			return nRet;
		}

		return S_OK;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nGroupID);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nEntityType);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nServerID);
		if(nRet < 0)
		{
			return nRet;
		}

		return S_OK;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "ServerRegistReq msgbody:{m_nGroupID=%d, "
				"m_nEntityType=%d, m_nServerID=%d}}", m_nGroupID, m_nEntityType, m_nServerID);
		offset += len;
	}

public:
	int32_t				m_nGroupID;
	EntityType			m_nEntityType;
	ServerID			m_nServerID;
};

#define MSGID_SERVERUNREGIST_REQ		0xff000002
class ServerUnregistReq : public IMsgBody
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
		int32_t len = sprintf(buf + offset, "ServerUnregistReq msgbody:{}}");
		offset += len;
	}
};

//=========begin=======media<---->mediadispatch============

#define MSGID_SHOWERLOGIN_NOTI			0x01061201
class ShowerLoginNoti : public IMsgBody
{
public:
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
		int32_t len = sprintf(buf + offset, "ShowerLoginNoti msgbody:{m_nUserID=%d}}", m_nUserID);
		offset += len;
	}

public:
	UserID			m_nUserID;
};

#define MSGID_SHOWERLOGOUT_NOTI			0x01061202
class ShowerLogoutNoti : public IMsgBody
{
public:
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
		int32_t len = sprintf(buf + offset, "ShowerLogoutNoti msgbody:{m_nUserID=%d}}", m_nUserID);
		offset += len;
	}

public:
	UserID			m_nUserID;
};

#define MSGID_SHOWERLIST_NOTI				0x01061203
class ShowerListNoti : public IMsgBody
{
public:
	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;

		if(m_nShowerCount > MaxShowerCountPerServer)
		{
			m_nShowerCount = MaxShowerCountPerServer;
		}

		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_nShowerCount);
		if(nRet < 0)
		{
			return nRet;
		}

		for(int32_t i = 0; i < m_nShowerCount; ++i)
		{
			nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_arrRoomID[i]);
			if(nRet < 0)
			{
				return nRet;
			}

			nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_arrShowerID[i]);
			if(nRet < 0)
			{
				return nRet;
			}
		}

		return S_OK;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_nShowerCount);
		if(nRet < 0)
		{
			return nRet;
		}

		if(m_nShowerCount > MaxShowerCountPerServer)
		{
			return E_UNKNOWN;
		}

		for(int32_t i = 0; i < m_nShowerCount; ++i)
		{
			nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_arrRoomID[i]);
			if(nRet < 0)
			{
				return nRet;
			}

			nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_arrShowerID[i]);
			if(nRet < 0)
			{
				return nRet;
			}
		}

		return S_OK;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "ShowerListNoti msgbody:{m_nShowerCount=%d, m_arrShowerInfo:{",
			m_nShowerCount);
		offset += len;

		if(m_nShowerCount <= 0)
		{
			len = sprintf(buf + offset, "}}}");
			offset += len;
		}
		else
		{
			for(int32_t i = 0; i < m_nShowerCount - 1; ++i)
			{
				len = sprintf(buf + offset, "%d : %d, ", m_arrRoomID[i], m_arrShowerID[i]);
				offset += len;
			}

			len = sprintf(buf + offset, "%d : %d}}}", m_arrRoomID[m_nShowerCount - 1], m_arrShowerID[m_nShowerCount - 1]);
			offset += len;
		}
	}

public:
	int32_t			m_nShowerCount;
	RoomID			m_arrRoomID[MaxShowerCountPerServer];
	UserID			m_arrShowerID[MaxShowerCountPerServer];
};

#define MSGID_UPDATESERVERLOAD_NOTI			0x01061204
class UpdateServerLoadNoti : public IMsgBody
{
public:
	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_stLoadInfo.nShowerCount);
		if(nRet < 0)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_stLoadInfo.nSubscriberCount);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_stLoadInfo.nShowerCount);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_stLoadInfo.nSubscriberCount);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "UpdateServerLoadNoti msgbody:"
				"LoadInfo={nShowerCount=%d, nSubscriberCount=%d}}", m_stLoadInfo.nShowerCount,
				m_stLoadInfo.nSubscriberCount);
		offset += len;
	}

public:
	LoadInfo		m_stLoadInfo;
};

//=========end============media<---->mediadispatch============


//=========begin==========mediadispatch<---->cacheproxy==============
#define MSGID_UPDATECACHE_NOTI			0x01071201
class UpdateCacheNoti : public IMsgBody
{
public:
	int32_t Encode(uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_strKey);
		if(nRet < 0)
		{
			return nRet;
		}
		nRet = CCodeEngine::Encode(pBuf, nBufSize, nOffset, m_strValue);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	int32_t Decode(const uint8_t *pBuf, const int32_t nBufSize, uint32_t &nOffset)
	{
		int32_t nRet = S_OK;
		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_strKey);
		if(nRet < 0)
		{
			return nRet;
		}

		nRet = CCodeEngine::Decode(pBuf, nBufSize, nOffset, m_strValue);
		if(nRet < 0)
		{
			return nRet;
		}

		return nRet;
	}

	void Dump(char* buf, const uint32_t size, uint32_t& offset)
	{
		int32_t len = sprintf(buf + offset, "UpdateServerLoadNoti msgbody:{m_strKey=%s, m_strKey=%s}}",
				m_strKey.c_str(), m_strValue.c_str());
		offset += len;
	}

public:
	string			m_strKey;
	string			m_strValue;
};
//=========end============mediadispatch<---->cacheproxy==============

#endif /* MESSAGE_DEFINE_H_ */
