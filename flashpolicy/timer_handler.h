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

	void AddSession(IIOSession *pIoSession);

	void DelSession(IIOSession *pIoSession);

	int32_t HeartBeat(CTimer *pTimer);

private:
	map<IIOSession *, int64_t>		m_stConnMap;
};

#endif /* TIMER_HANDLER_H_ */
