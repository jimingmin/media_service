/*
 * packetparser.cpp
 *
 *  Created on: 2014年5月13日
 *      Author: jimm
 */

#include "packetparser.h"
#include "public_define.h"
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
	int32_t nDataSize = strlen((char *)arrBuf);
	if(nDataSize == (sizeof(FLASH_SEC_REQ) - 1))
	{
		nPacketSize = nDataSize;
		memcpy(arrPacket, arrBuf, nDataSize);
		arrPacket[nDataSize++] = '\0';
		return nDataSize;
	}

	return 0;
}



