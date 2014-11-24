/*
 * command_handler.cpp
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#include "command_handler.h"
#include "../include/message_define.h"
#include "../include/message_head.h"
#include "datacenter.h"
#include "session_param.h"
#include "../frame/frame.h"
#include "server_config.h"

using namespace FRAME;

//服务器注册
int32_t CCommandHandler::ServerRegistEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
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

	ServerRegistReq *pServerRegistReq = dynamic_cast<ServerRegistReq *>(pMsgBody);
	if(pServerRegistReq == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to server regist req error!\n");
		return 0;
	}

	if(pServerRegistReq->m_nEntityType != enmEntityType_Media)
	{
		WRITE_WARN_LOG(SERVER_NAME, "regist req is not from media!\n");
		pIoSession->Close();
		return 0;
	}

	CSessionParam *pSessionParam = NEW(CSessionParam);
	pSessionParam->SetGroupID(pServerRegistReq->m_nGroupID);
	pSessionParam->SetEntityType(pServerRegistReq->m_nEntityType);
	pSessionParam->SetServerID(pServerRegistReq->m_nServerID);

	pIoSession->SetParamPtr(pSessionParam);

	CGroupInfo *pGroupInfo = g_DataCenter.FindGroupInfo(pServerRegistReq->m_nGroupID);
	if(pGroupInfo == NULL)
	{
		pGroupInfo = NEW(CGroupInfo);
		pGroupInfo->SetGroupID(pServerRegistReq->m_nGroupID);

		g_DataCenter.AddGroupInfo(pServerRegistReq->m_nGroupID, pGroupInfo);
	}

	pGroupInfo->AddServer(pIoSession->GetPeerAddressStr());

	return 0;
}

//服务器卸载
int32_t CCommandHandler::ServerUnregistEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "recv server unregist event!\n");

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

	ServerUnregistReq *pServerUnregistReq = dynamic_cast<ServerUnregistReq *>(pMsgBody);
	if(pServerUnregistReq == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to server unregist req error!\n");
		return 0;
	}

	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	if(pSessionParam != NULL)
	{
		CGroupInfo *pGroupInfo = g_DataCenter.FindGroupInfo(pSessionParam->GetGroupID());
		if(pGroupInfo != NULL)
		{
			g_DataCenter.DelGroupInfo(pSessionParam->GetGroupID());
			DELETE(pGroupInfo);
		}
		//释放内存
		DELETE(pSessionParam);
	}

	return 0;
}

//表演者登陆服务器
int32_t CCommandHandler::ShowerLoginEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "recv shower login event!\n");

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

	ShowerLoginNoti *pShowerLoginNoti = dynamic_cast<ShowerLoginNoti *>(pMsgBody);
	if(pShowerLoginNoti == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to showlogin noti!");
		return 0;
	}

	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	if(pSessionParam == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found session param!\n");
		return 0;
	}

	CGroupInfo *pGroupInfo = g_DataCenter.FindGroupInfo(pSessionParam->GetGroupID());
	if(pGroupInfo == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found group info!{groupid=%d}\n", pSessionParam->GetGroupID());
		return 0;
	}

	pGroupInfo->AddShower(pShowerLoginNoti->m_nShowerID, pIoSession->GetPeerAddressStr());

	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);

	char arrString[256];
	UpdateCacheNoti stUpdateCacheNoti;
	int32_t len = sprintf(arrString, "media-%d", pMsgHeadSS->m_nRoomID);
	arrString[len] = '\0';
	stUpdateCacheNoti.m_strKey = string(arrString);
	stUpdateCacheNoti.m_strValue = pServerConfig->GetDomainByGroup(pSessionParam->GetGroupID());
	SendSSNoti(MSGID_UPDATECACHE_NOTI, &stUpdateCacheNoti, pMsgHeadSS->m_nRoomID, pMsgHeadSS->m_nUserID, enmTransType_P2P, enmEntityType_CacheProxy, 1);

	return 0;
}

//表演者退出服务器
int32_t CCommandHandler::ShowerLogoutEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "recv shower logout event!\n");

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

	ShowerLogoutNoti *pShowerLogoutNoti = dynamic_cast<ShowerLogoutNoti *>(pMsgBody);
	if(pShowerLogoutNoti == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to showerlogout noti!");
		return 0;
	}

	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	if(pSessionParam == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found session param!\n");
		return 0;
	}

	CGroupInfo *pGroupInfo = g_DataCenter.FindGroupInfo(pSessionParam->GetGroupID());
	if(pGroupInfo == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found group info!{groupid=%d}\n", pSessionParam->GetGroupID());
		return 0;
	}

	pGroupInfo->DelShower(pShowerLogoutNoti->m_nShowerID);

	char arrString[256];
	UpdateCacheNoti stUpdateCacheNoti;
	int32_t len = sprintf(arrString, "media-%d", pMsgHeadSS->m_nRoomID);
	arrString[len] = '\0';
	stUpdateCacheNoti.m_strKey = string(arrString);
	stUpdateCacheNoti.m_strValue = string("0");
	SendSSNoti(MSGID_UPDATECACHE_NOTI, &stUpdateCacheNoti, pMsgHeadSS->m_nRoomID, pMsgHeadSS->m_nUserID, enmTransType_P2P, enmEntityType_CacheProxy, 1);

	return 0;
}

//更新服务器负载值
int32_t CCommandHandler::UpdateServerLoadEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "recv update server load event!\n");

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

	UpdateServerLoadNoti *pUpdateServerLoadNoti = dynamic_cast<UpdateServerLoadNoti *>(pMsgBody);
	if(pUpdateServerLoadNoti == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to updateserverload noti!");
		return 0;
	}

	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	if(pSessionParam == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found session param!\n");
		return 0;
	}

	CGroupInfo *pGroupInfo = g_DataCenter.FindGroupInfo(pSessionParam->GetGroupID());
	if(pGroupInfo == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found group info!{groupid=%d}\n", pSessionParam->GetGroupID());
		return 0;
	}

	pGroupInfo->UpdateServerLoadInfo(pIoSession->GetPeerAddressStr(), pUpdateServerLoadNoti->m_stLoadInfo);

	return 0;
}

//发布者列表
int32_t CCommandHandler::ShowerListEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "recv shower list event!\n");

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

	ShowerListNoti *pShowerListNoti = dynamic_cast<ShowerListNoti *>(pMsgBody);
	if(pShowerListNoti == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to shower list noti error!\n");
		return 0;
	}

	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	if(pSessionParam == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found session param!\n");
		return 0;
	}

	CGroupInfo *pGroupInfo = g_DataCenter.FindGroupInfo(pSessionParam->GetGroupID());
	if(pGroupInfo == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found group info!{groupid=%d}\n", pSessionParam->GetGroupID());
		return 0;
	}

	for(int32_t i = 0; i < pShowerListNoti->m_nShowerCount; ++i)
	{
		pGroupInfo->AddShower(pShowerListNoti->m_arrShowerID[i], pIoSession->GetPeerAddressStr());
	}
	pGroupInfo->AddServer(pIoSession->GetPeerAddressStr());

	return 0;
}

