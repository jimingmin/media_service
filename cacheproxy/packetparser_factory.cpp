/*
 * packetparser_factory.cpp
 *
 *  Created on: 2014年5月13日
 *      Author: jimm
 */

#include "packetparser_factory.h"
#include "packetparser.h"
#include "../common/common_memmgt.h"

CPacketParserFactory::CPacketParserFactory()
{

}

IPacketParser *CPacketParserFactory::Create()
{
	return NEW(CPacketParser);
}

void CPacketParserFactory::Destory(IPacketParser *pParser)
{
	DELETE(pParser);
}
