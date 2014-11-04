/*
 * timer_handler.cpp
 *
 *  Created on: 2014年6月10日
 *      Author: jimm
 */

#include "timer_handler.h"
#include "../common/common_datetime.h"
#include "../netevent/net_impl.h"
#include "media_channel.h"
#include "session_param.h"

using namespace NETEVENT;

void CTimerHandler::RecordShowerHB(CMediaChannel *pMediaChannel)
{
	DelSessionHB(pMediaChannel->GetIoSession());
	m_stShowerHB.push_back(pMediaChannel);
}

void CTimerHandler::DestroyShowerHB(CMediaChannel *pMediaChannel)
{
	m_stShowerHB.remove(pMediaChannel);
}

void CTimerHandler::AddSessionHB(IIOSession *pIoSession)
{
	m_arrSubscriberHB[m_nIdleIndex][pIoSession] = 0;
}

void CTimerHandler::DelSessionHB(IIOSession *pIoSession)
{
	m_arrSubscriberHB[m_nIdleIndex].erase(pIoSession);
	m_arrSubscriberHB[m_nCurIndex].erase(pIoSession);
}

void CTimerHandler::MoveToIdle(IIOSession *pIoSession)
{
	m_arrSubscriberHB[m_nCurIndex].erase(pIoSession);
	m_arrSubscriberHB[m_nIdleIndex][pIoSession] = 0;
}

int32_t CTimerHandler::HeartBeat(CTimer *pTimer)
{
	int64_t nCurTime = CTimeValue::CurrentTime().Seconds();

	int32_t nCount = m_stShowerHB.size();
	list<CMediaChannel *> stShowerHB = m_stShowerHB;
	for(int32_t i = 0; i < nCount; ++i)
	{
		CMediaChannel *pMediaChannel = stShowerHB.front();
		stShowerHB.pop_front();

		IIOSession *pIoSession = pMediaChannel->GetIoSession();

		CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
		if(pSessionParam == NULL)
		{
			pIoSession->Close();
			continue;
		}

		EntityType nEntityType = enmEntityType_None;
		ServerID nServerID = enmInvalidServerID;
		pSessionParam->GetServerInfo(nEntityType, nServerID);
		if((nEntityType == enmEntityType_None) || (nServerID == enmInvalidServerID))
		{
			if(nCurTime - pIoSession->GetLastRecvTime() > 30)
			{
				pIoSession->Close();
			}
		}
	}

	map<IIOSession *, int64_t> stSubscriberHB = m_arrSubscriberHB[m_nCurIndex];
	map<IIOSession *, int64_t>::iterator it = stSubscriberHB.begin();
	for(; it != stSubscriberHB.end(); ++it)
	{
		it->first->Close();
	}

	int32_t nIndex = m_nCurIndex;
	m_nCurIndex = m_nIdleIndex;
	m_nIdleIndex = nIndex;

	return 0;
}


