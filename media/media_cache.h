/*
 * media_cache.h
 *
 *  Created on: 2014年6月4日
 *      Author: jimm
 */

#ifndef MEDIA_CACHE_H_
#define MEDIA_CACHE_H_

#include "../common/common_object.h"
#include "../common/common_memmgt.h"
#include "server_typedef.h"
#include <list>
using namespace std;

class CMediaCache : public CBaseObject
{
public:
	CMediaCache()
	{
		Init();
	}

	virtual int32_t Init()
	{
		m_nMaxCacheCount = 512;
		m_nMediaPacketCount = 0;
		m_nKeyFrameCount = 0;
		m_nMaxFrameCount = 1;
		m_nLastPrevFrameCount = 0;
		m_nMaxPrevFrameCount = 200;

		return 0;
	}

	virtual int32_t Uninit()
	{
		for(int32_t i = 0; i < m_nMediaPacketCount; ++i)
		{
			MediaPacket *pMediaPacket = m_stMediaPacketList.front();
			m_stMediaPacketList.pop_front();

			DELETE(pMediaPacket);
		}
		return 0;
	}

	virtual int32_t GetSize()
	{
		return sizeof(*this);
	}

	void CacheMediaPacket(MediaType nMediaType, uint8_t *pBuf, int32_t nBufSize);

	list<MediaPacket *> GetCacheList();

protected:
	uint16_t				m_nMaxCacheCount;			//最大媒体包数量
	int32_t					m_nMediaPacketCount;		//当前媒体包数量
	int32_t					m_nKeyFrameCount;			//当前关键帧数量
	int32_t					m_nMaxFrameCount;			//保存最大关键帧数量
	int32_t					m_nLastPrevFrameCount;		//保存P帧的数量
	int32_t					m_nMaxPrevFrameCount;		//保存最大的P帧数量
	list<MediaPacket *>		m_stMediaPacketList;
};


#endif /* MEDIA_CACHE_H_ */
