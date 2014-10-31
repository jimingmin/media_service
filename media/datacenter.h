/*
 * datacenter.h
 *
 *  Created on: 2014年5月12日
 *      Author: jimm
 */

#ifndef DATACENTER_H_
#define DATACENTER_H_

#include "../common/common_singleton.h"
#include "server_typedef.h"
#include "../netevent/net_impl.h"
#include "timer_handler.h"
#include "../include/typedef.h"
using namespace NETEVENT;

#include <map>
#include <string>
using namespace std;

class CMediaChannel;

typedef map<string, CMediaChannel *>	SessionChannelMap;

class CDataCenter
{
public:
	CDataCenter();

	void AddChannel(CMediaChannel *pMediaChannel);

	void DelChannel(CMediaChannel *pMediaChannel);

	CMediaChannel *FindChannel(IIOSession *pIoSession);

	CMediaChannel *FindChannel(RoomID nRoomID, UserID nShowerID, UserID nSubscriberID);

	CMediaChannel *FindChannel(string strKey);

	int32_t GetAllLocalShower(RoomID arrRoomID[], UserID arrShowerID[], int32_t nSize);

	void SetTimerHandler(CTimerHandler *pTimerHandler);

	CTimerHandler *GetTimerHandler();

	int32_t IncShowerCount();

	int32_t DecShowerCount();

	int32_t IncSubscriberCount();

	int32_t DecSubscriberCount();

	LoadInfo GetServerLoadInfo();

	int32_t IncConnCount();

	int32_t DecConnCount();

private:
	LoadInfo				m_stLoadInfo;
	int32_t					m_nConnCount;
	SessionChannelMap		m_stSessionChannelMap;
	CTimerHandler			*m_pTimerHandler;
};

#define g_DataCenter		CSingleton<CDataCenter>::GetInstance()

#endif /* DATACENTER_H_ */
