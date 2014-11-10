/*
 * timer_handler.cpp
 *
 *  Created on: 2014年6月10日
 *      Author: jimm
 */

#include "timer_handler.h"
#include "../common/common_datetime.h"
#include "../netevent/net_impl.h"

using namespace NETEVENT;

void CTimerHandler::AddSession(IIOSession *pIoSession)
{
	m_stConnMap[pIoSession] = pIoSession->GetCreateTime();
}

void CTimerHandler::DelSession(IIOSession *pIoSession)
{
	m_stConnMap.erase(pIoSession);
}

int32_t CTimerHandler::HeartBeat(CTimer *pTimer)
{
	int64_t nCurTime = CTimeValue::CurrentTime().Seconds();
	map<IIOSession *, int64_t> stConnMap = m_stConnMap;
	map<IIOSession *, int64_t>::iterator it = stConnMap.begin();
	for(; it != stConnMap.end(); ++it)
	{
		if(nCurTime > it->second + 10)
		{
			it->first->Close();
		}
	}

	return 0;
}


