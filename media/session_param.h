/*
 * session_param.h
 *
 *  Created on: 2014年5月26日
 *      Author: jimm
 */

#ifndef SESSION_PARAM_H_
#define SESSION_PARAM_H_

#include "../common/common_object.h"
#include "server_typedef.h"
#include <list>
using namespace std;

class CSessionParam : public CBaseObject
{
public:
	CSessionParam()
	{
		Init();
	}

	virtual int32_t Init();

	virtual int32_t Uninit();

	virtual int32_t GetSize();

	void SetServerInfo(EntityType nEntityType, ServerID nServerID);

	void GetServerInfo(EntityType &nEntityType, ServerID &nServerID);

	void AddRoomParam(RoomParam &stRoomParam);

	void DelRoomParam(RoomID nRoomID, UserID nShowerID, UserID nSubscriberID);

	RoomParam FirstRoomParam();

	RoomParam FrontRoomParam();

	int32_t RoomParamCount();

private:
	EntityType			m_nEntityType;
	int32_t				m_nServerID;
	list<RoomParam>		m_stRoomParamList;
};



#endif /* SESSION_PARAM_H_ */
