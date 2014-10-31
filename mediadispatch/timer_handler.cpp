/*
 * timer_handler.cpp
 *
 *  Created on: 2014年6月10日
 *      Author: jimm
 */

#include "timer_handler.h"
#include "../common/common_datetime.h"
#include "datacenter.h"
#include "../logger/logger.h"
#include "server_typedef.h"

using namespace LOGGER;

int32_t CTimerHandler::PrintMediaLoadInfo(CTimer *pTimer)
{
	int32_t arrGroupID[1024];
	int32_t nGroupCount = g_DataCenter.GetAllGroupID(arrGroupID, 1024);

	for(int32_t nGroupIndex = 0; nGroupIndex < nGroupCount; ++nGroupIndex)
	{
		CGroupInfo *pGroupInfo = g_DataCenter.FindGroupInfo(arrGroupID[nGroupIndex]);
		if(pGroupInfo == NULL)
		{
			continue;
		}

		string arrServerAddress[32];
		int32_t nShowerCount = pGroupInfo->GetAllServerCount(arrServerAddress, 64);
		for(int32_t nShowerIndex  = 0; nShowerIndex < nShowerCount; ++nShowerIndex)
		{
			LoadInfo stLoadInfo = pGroupInfo->GetLoadInfo(arrServerAddress[nShowerIndex]);
			WRITE_INFO_LOG(SERVER_NAME, "group id : %d, server address : %s, shower count : %d, subscriber count : %d\n",
					arrGroupID[nGroupIndex], arrServerAddress[nShowerIndex].c_str(), stLoadInfo.nShowerCount, stLoadInfo.nSubscriberCount);
		}
	}

	return 0;
}


