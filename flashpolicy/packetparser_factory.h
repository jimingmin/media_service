/*
 * packetparser_factory.h
 *
 *  Created on: 2014年5月12日
 *      Author: jimm
 */

#ifndef PACKETPARSER_FACTORY_H_
#define PACKETPARSER_FACTORY_H_

#include "../netevent/net_namespace.h"
#include "../netevent/net_impl.h"
#include "public_define.h"

using namespace NETEVENT;

class CPacketParserFactory : public IPacketParserFactory
{
public:
	CPacketParserFactory(const char *strPacketToken);

	virtual IPacketParser *Create();

	virtual void Destory(IPacketParser *pParser);

protected:
	char	m_arrPacketToken[enmMaxPacketTokenSize];
};


#endif /* PACKETPARSER_FACTORY_H_ */
