/*
 * command_handler.cpp
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#include "command_handler.h"
#include "../include/message_define.h"
#include "../include/message_head.h"
#include "../netevent/net_impl.h"
#include "../logger/logger.h"
#include "server_typedef.h"
#include "datacenter.h"

using namespace NETEVENT;
using namespace LOGGER;

//更新缓存
int32_t CCommandHandler::UpdateCacheEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "recv server regist event!\n");

	IIOSession *pIoSession = dynamic_cast<IIOSession *>(pObject);
	if(pIoSession == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate object to iosession error!\n");
		return 0;
	}

	MsgHeadSS *pMsgHeadSS = dynamic_cast<MsgHeadSS *>(pMsgHead);
	if(pMsgHeadSS == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msghead to msgheadss error!\n");
		return 0;
	}

	UpdateCacheNoti *pUpdateCacheNoti = dynamic_cast<UpdateCacheNoti *>(pMsgBody);
	if(pUpdateCacheNoti == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to server regist req error!\n");
		return 0;
	}

	CMemcacheClient *pMemcacheClient = g_DataCenter.GetMemcacheClient();
	if(pUpdateCacheNoti->m_strValue == "0")
	{
		pMemcacheClient->Del(pUpdateCacheNoti->m_strKey.c_str());
	}
	else
	{
		pMemcacheClient->Set(pUpdateCacheNoti->m_strKey.c_str(), pUpdateCacheNoti->m_strValue.c_str(), 0);
	}

	return 0;
}

