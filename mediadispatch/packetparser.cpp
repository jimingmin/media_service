/*
 * packetparser.cpp
 *
 *  Created on: 2014年5月13日
 *      Author: jimm
 */

#include "packetparser.h"
#include "../common/common_codeengine.h"

int32_t CPacketParser::Init()
{
	return 0;
}

int32_t CPacketParser::Uninit()
{
	return 0;
}

int32_t CPacketParser::GetSize()
{
	return sizeof(*this);
}

int32_t CPacketParser::Parser(const uint8_t arrBuf[], const uint32_t nBufSize, uint8_t arrPacket[], int32_t &nPacketSize)
{
	uint16_t nTotalSize = 0;
	uint32_t nOffset = 1;
	int32_t nRet = CCodeEngine::Decode((unsigned char *)arrBuf, nBufSize, nOffset, nTotalSize);
	if(nRet < 0)
	{
		return 0;
	}

	if(nTotalSize > nBufSize)
	{
		return 0;
	}

	nPacketSize = nTotalSize;
	memcpy(arrPacket, arrBuf, nTotalSize);

	return nTotalSize;
}



