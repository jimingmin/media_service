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
#include <list>

using namespace std;
using namespace FRAME;

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

	int32_t PrintMediaLoadInfo(CTimer *pTimer);

};

#endif /* TIMER_HANDLER_H_ */
