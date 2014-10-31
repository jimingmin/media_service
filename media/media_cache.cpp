/*
 * media_cache.cpp
 *
 *  Created on: 2014年6月4日
 *      Author: jimm
 */

#include "media_cache.h"
#include <string.h>

void CMediaCache::CacheMediaPacket(MediaType nMediaType, uint8_t *pBuf, int32_t nBufSize)
{
	if(nMediaType == enmMediaDataType_Audio)
	{
		//return;
	}

	if(nMediaType == enmMediaDataType_KeyFrame)
	{
		if(m_nKeyFrameCount >= m_nMaxFrameCount)
		{
			list<MediaPacket *> stMediaPacketList = m_stMediaPacketList;
			for(list<MediaPacket *>::iterator it = stMediaPacketList.begin(); it != stMediaPacketList.end(); ++it)
			{
				MediaPacket *pMediaPacket = m_stMediaPacketList.front();
				if(pMediaPacket->nMediaType == enmMediaDataType_KeyFrame)
				{
					if(m_nKeyFrameCount < m_nMaxFrameCount)
					{
						break;
					}

					--m_nKeyFrameCount;
				}

				DELETE(pMediaPacket);
				m_stMediaPacketList.pop_front();
			}
		}

		m_nKeyFrameCount++;
		m_nLastPrevFrameCount = 0;
	}
	else
	{
		++m_nLastPrevFrameCount;
		if(m_nLastPrevFrameCount > m_nMaxPrevFrameCount)
		{
			return;
		}
	}

	MediaPacket *pMediaPacket = NEW(MediaPacket);
	pMediaPacket->nMediaType = nMediaType;
	pMediaPacket->nPacketSize = nBufSize;
	pMediaPacket->pPacket = MALLOC(nBufSize);
	memcpy(pMediaPacket->pPacket, pBuf, nBufSize);

	m_stMediaPacketList.push_back(pMediaPacket);
	//if(m_nMaxCacheCount > m_nMediaPacketCount)
	//{
	//	++m_nMediaPacketCount;
	//}
	//else
	//{
	//	MediaPacket *pMediaPacket = m_stMediaPacketList.front();
	//	DELETE(pMediaPacket);

	//	m_stMediaPacketList.pop_front();
	//}
}

list<MediaPacket *> CMediaCache::GetCacheList()
{
	return m_stMediaPacketList;
}


