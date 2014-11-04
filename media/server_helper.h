/*
 * server_helper.h
 *
 *  Created on: 2014年5月14日
 *      Author: jimm
 */

#ifndef SERVER_HELPER_H_
#define SERVER_HELPER_H_

#include "../common/common_object.h"
#include "../frame/frame_typedef.h"
#include "../netevent/net_impl.h"
#include "../logger/logger.h"
#include "server_typedef.h"
#include "../include/message_head.h"

#include <map>
using namespace std;

using namespace LOGGER;
using namespace FRAME;
using namespace NETEVENT;

typedef map<ServerID, IIOSession *> IDMap;
typedef map<EntityType, IDMap> EntityMap;

class CServerHelper : public CBaseObject
{
public:
	virtual int32_t Init()
	{
		return 0;
	}

	virtual int32_t Uninit()
	{
		return 0;
	}

	virtual int32_t GetSize()
	{
		return sizeof(*this);
	}

	static int32_t SendCSReq(int32_t nMsgID, RoomID nRoomID, UserID nUserID, IMsgBody *pMsgBody,
			EntityType nEntityType, ServerID nServerID);

	static int32_t SendCSResp(int32_t nMsgID, IMsgHead *pMsgHead, IMsgBody *pMsgBody, IIOSession *pIoSession);

	static int32_t SendCSNoti(int32_t nMsgID, IMsgBody *pMsgBody, EntityType nEntityType, ServerID nServerID);

	static int32_t SendSSReq(int32_t nMsgID, RoomID nRoomID, UserID nUserID, IMsgBody *pMsgBody,
			EntityType nEntityType, ServerID nServerID);

	static int32_t SendSSResp(int32_t nMsgID, IMsgHead *pMsgHead, IMsgBody *pMsgBody, IIOSession *pIoSession);

	static int32_t SendSSNoti(int32_t nMsgID, IMsgBody *pMsgBody, RoomID nRoomID, UserID nUserID, TransType nTransType,
			EntityType nEntityType, ServerID nServerID = enmInvalidServerID);

	static void RegistSessionInfo(EntityType nEntityType, int32_t nID, IIOSession *pIoSession);

	static void UnregistSessionInfo(IIOSession *pIoSession);

	static IIOSession *GetIoSession(EntityType nEntityType, int32_t nID);

	static int32_t MakeTransMediaHeadBuf(RoomID nRoomID, UserID nUserID, uint8_t *pHeadBuf, int32_t nBodySize);

protected:
	static EntityMap		m_stEntityMap;
};


#endif /* SERVER_HELPER_H_ */
