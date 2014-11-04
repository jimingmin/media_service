/*
 * timer_handler.h
 *
 *  Created on: 2014年6月10日
 *      Author: jimm
 */

#ifndef TIMER_HANDLER_H_
#define TIMER_HANDLER_H_

#include "../common/common_object.h"
#include "../frame/frame_timer.h"
#include "../netevent/net_impl.h"
#include <list>
#include <map>
#include <string>

using namespace std;
using namespace NETEVENT;
using namespace FRAME;

class CMediaChannel;

class CTimerHandler : public CBaseObject
{
public:
	CTimerHandler()
	{
		Init();
	}

	virtual int32_t Init()
	{
		m_nCurIndex = 0;
		m_nIdleIndex = 1;
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

	void RecordShowerHB(CMediaChannel *pMediaChannel);

	void DestroyShowerHB(CMediaChannel *pMediaChannel);

	void AddSessionHB(IIOSession *pIoSession);

	void DelSessionHB(IIOSession *pIoSession);

	void MoveToIdle(IIOSession *pIoSession);

	int32_t HeartBeat(CTimer *pTimer);

private:
	list<CMediaChannel *>			m_stShowerHB;			//表演者心跳
	int32_t							m_nCurIndex;			//当前正在使用的索引
	int32_t							m_nIdleIndex;			//切换过的索引
	map<IIOSession *, int64_t>		m_arrSubscriberHB[2];	//观看者心跳
};

#endif /* TIMER_HANDLER_H_ */
