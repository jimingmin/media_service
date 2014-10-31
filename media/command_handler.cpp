/*
 * command_handler.cpp
 *
 *  Created on: 2014年5月12日
 *      Author: jimm
 */

#ifndef COMMAND_HANDLER_CPP_
#define COMMAND_HANDLER_CPP_

#include "command_handler.h"
#include "server_typedef.h"
#include "../include/message_head.h"
#include "../include/message_define.h"
#include "../netevent/net_impl.h"
#include "client_message.h"
#include "server_message.h"
#include "datacenter.h"
#include "../frame/frame.h"
#include "server_config.h"
#include "media_channel.h"
#include "session_param.h"

using namespace FRAME;

//服务器注册
int32_t CCommandHandler::ServerRegistEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
//	WRITE_DEBUG_LOG(SERVER_NAME, "recv server regist event!\n");
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

	CTimerHandler *pTimerHandler = g_DataCenter.GetTimerHandler();
	pTimerHandler->DelSessionHB(pIoSession);

	RegistSessionInfo(pServerRegistReq->m_nEntityType, pServerRegistReq->m_nServerID, pIoSession);

	ShowerListNoti stShowerListInfo;
	stShowerListInfo.m_nShowerCount = g_DataCenter.GetAllLocalShower(stShowerListInfo.m_arrRoomID, stShowerListInfo.m_arrShowerID, MaxShowerCountPerServer);
	SendSSNoti(MSGID_SHOWERLIST_NOTI, &stShowerListInfo, enmInvalidRoomID, enmInvalidUserID,
			enmTransType_P2P, pMsgHeadSS->m_nSrcType, pMsgHeadSS->m_nSrcID);

	return 0;
}

//服务器卸载
int32_t CCommandHandler::ServerUnregistEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
//	WRITE_DEBUG_LOG(SERVER_NAME, "recv server unregist event!\n");
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

//	UnregistSessionInfo(pIoSession);

	return 0;
}

//用户注册
int32_t CCommandHandler::UserRegistEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
//	WRITE_DEBUG_LOG(SERVER_NAME, "recv user regist event!\n");

	IIOSession *pIoSession = dynamic_cast<IIOSession *>(pObject);
	if(pIoSession == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate object to iosession error!\n");
		return 0;
	}

	MsgHeadCS *pMsgHeadCS = dynamic_cast<MsgHeadCS *>(pMsgHead);
	if(pMsgHeadCS == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msghead to msgheadcs error!\n");
		return 0;
	}

	UserRegistReq *pUserRegistReq = dynamic_cast<UserRegistReq *>(pMsgBody);
	if(pUserRegistReq == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to server regist req error!\n");
		return 0;
	}

	UserRegistResp stResp;

	string strToken = pUserRegistReq->m_strToken;
	int32_t nTokenSize = strToken.length();
	char szToken[32];
	if(nTokenSize > sizeof(szToken))
	{
		WRITE_WARN_LOG(SERVER_NAME, "token size is too big!{nTokenSize=%d}\n", nTokenSize);
		stResp.m_nResult = 1;
		SendCSResp(MSGID_USERREGIST_RESP, pMsgHead, &stResp, pIoSession);
		return 0;
	}
	static const char *szKey = "69xiukkirnkxi39297";
	static const int32_t nKeySize = strlen(szKey);

	strcpy(szToken, strToken.c_str());
	szToken[nTokenSize] = '\0';

	//异或加密，格式：随机数(3个字节) + UID(长度不固定，为int32_t类型) + 时间戳(int32_t类型，10位)
	for(int32_t i = 0; i < nTokenSize; ++i)
	{
		for(int32_t j = 0; j < nKeySize; ++j)
		{
			szToken[i] ^= szKey[j];
		}
	}

	int32_t nSize = nTokenSize - 13;
	if(nSize <= 0)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found uid!{nTokenSize=%d}\n", nTokenSize);
		stResp.m_nResult = 2;
		SendCSResp(MSGID_USERREGIST_RESP, pMsgHead, &stResp, pIoSession);
		return 0;
	}

	szToken[nTokenSize - 10] = '\0';
	UserID nUserID = atoi(&szToken[3]);
	if(nUserID != pUserRegistReq->m_nUserID)
	{
		WRITE_WARN_LOG(SERVER_NAME, "token verify is not pass!{Decode UserID=%d, Real UserID=%d}\n", nUserID, pUserRegistReq->m_nUserID);
		stResp.m_nResult = 3;
		SendCSResp(MSGID_USERREGIST_RESP, pMsgHead, &stResp, pIoSession);
		return 0;
	}

	stResp.m_nResult = 0;
	SendCSResp(MSGID_USERREGIST_RESP, pMsgHead, &stResp, pIoSession);

	RegistSessionInfo(enmEntityType_Client, pIoSession->GetSessionID(), pIoSession);

	return 0;
}

//用户卸载
int32_t CCommandHandler::UserUnregistEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
//	WRITE_DEBUG_LOG(SERVER_NAME, "recv user unregist event!\n");

	IIOSession *pIoSession = dynamic_cast<IIOSession *>(pObject);
	if(pIoSession == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate object to iosession error!\n");
		return 0;
	}

	MsgHeadCS *pMsgHeadCS = dynamic_cast<MsgHeadCS *>(pMsgHead);
	if(pMsgHeadCS == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msghead to msgheadcs error!\n");
		return 0;
	}

	UserUnregistReq *pUserUnregistReq = dynamic_cast<UserUnregistReq *>(pMsgBody);
	if(pUserUnregistReq == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to user unregist req error!\n");
		return 0;
	}

	//是否表演
	if(pMsgHeadCS->m_nUserID == pUserUnregistReq->m_nShowerID)
	{
		StopPublishReq stStopPublishReq;
		stStopPublishReq.m_nUserID = pMsgHeadCS->m_nUserID;

		StopPublishEvent(pIoSession, pMsgHead, &stStopPublishReq);
	}
	//是否观看
	else
	{
		CMediaChannel *pMediaChannel = g_DataCenter.FindChannel(pMsgHeadCS->m_nRoomID, pUserUnregistReq->m_nShowerID, pMsgHeadCS->m_nUserID);
		if(pMediaChannel != NULL)
		{
			UserUnsubscribeReq stUnsubscribeReq;
			stUnsubscribeReq.m_nUserID = pMediaChannel->GetShowerID();

			UserUnsubscribeEvent(pIoSession, pMsgHead, &stUnsubscribeReq);
		}
	}

//	UnregistSessionInfo(pIoSession);

	return 0;
}

//用户开始发布
int32_t CCommandHandler::StartPublishEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
//	WRITE_DEBUG_LOG(SERVER_NAME, "recv start publish event!\n");

	StartPublishResp stResp;
	stResp.m_nResult = 1;

	IIOSession *pIoSession = dynamic_cast<IIOSession *>(pObject);
	if(pIoSession == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate object to iosession error!\n");
		return 0;
	}

	MsgHeadCS *pMsgHeadCS = dynamic_cast<MsgHeadCS *>(pMsgHead);
	if(pMsgHeadCS == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msghead to msgheadcs error!\n");
		return 0;
	}

	StartPublishReq *pStartPublishReq = dynamic_cast<StartPublishReq *>(pMsgBody);
	if(pStartPublishReq == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to publish req error!\n");
		SendCSResp(MSGID_STARTPUBLISH_RESP, pMsgHead, &stResp, pIoSession);
		return 0;
	}

	CMediaChannel *pShower = g_DataCenter.FindChannel(pMsgHeadCS->m_nRoomID, pStartPublishReq->m_nUserID, pStartPublishReq->m_nUserID);
	if(pShower != NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's found shower exist!{ShowerID=%d}\n", pStartPublishReq->m_nUserID);
		SendCSResp(MSGID_STARTPUBLISH_RESP, pMsgHead, &stResp, pIoSession);
		return 0;
	}

	pShower = NEW(CMediaChannel);
	if(pShower == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to start publish req error!\n");
		SendCSResp(MSGID_STARTPUBLISH_RESP, pMsgHead, &stResp, pIoSession);
		return 0;
	}

	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	pShower->SetServerID(pServerConfig->GetServerID());
	pShower->SetIoSession(pIoSession);
	pShower->SetChannelKey(pMsgHeadCS->m_nRoomID, pMsgHeadCS->m_nUserID, pStartPublishReq->m_nUserID);
	pShower->SetChannelType(enmChannelType_User);
	pShower->StartShow();

	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	RoomParam stRoomParam(pMsgHeadCS->m_nRoomID, pStartPublishReq->m_nUserID, pStartPublishReq->m_nUserID);
	pSessionParam->AddRoomParam(stRoomParam);

	stResp.m_nResult = 0;
	SendCSResp(MSGID_STARTPUBLISH_RESP, pMsgHead, &stResp, pIoSession);

	ShowerLoginNoti stShowerLoginNoti;
	stShowerLoginNoti.m_nUserID = pStartPublishReq->m_nUserID;

	SendSSNoti(MSGID_SHOWERLOGIN_NOTI, &stShowerLoginNoti, pMsgHeadCS->m_nRoomID, pMsgHeadCS->m_nUserID, enmTransType_Server, enmEntityType_Media);
	SendSSNoti(MSGID_SHOWERLOGIN_NOTI, &stShowerLoginNoti, pMsgHeadCS->m_nRoomID, pMsgHeadCS->m_nUserID, enmTransType_Server, enmEntityType_MediaDispatch);
	return 0;
}

//用户停止发布
int32_t CCommandHandler::StopPublishEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "recv stop publish event!\n");

	StartPublishResp stResp;
	stResp.m_nResult = 1;

	IIOSession *pIoSession = dynamic_cast<IIOSession *>(pObject);
	if(pIoSession == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate object to iosession error!\n");
		return 0;
	}

	MsgHeadCS *pMsgHeadCS = dynamic_cast<MsgHeadCS *>(pMsgHead);
	if(pMsgHeadCS == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msghead to msgheadcs error!\n");
		return 0;
	}

	StopPublishReq *pStopPublishReq = dynamic_cast<StopPublishReq *>(pMsgBody);
	if(pStopPublishReq == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to stop publish req error!\n");
		return 0;
	}

	CMediaChannel *pShower = g_DataCenter.FindChannel(pMsgHeadCS->m_nRoomID, pStopPublishReq->m_nUserID, pStopPublishReq->m_nUserID);
	if(pShower == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found shower!{UserID=%d}\n", pMsgHeadCS->m_nUserID);
		return 0;
	}

	pShower->StopShow();
	DELETE(pShower);

	ShowerLogoutNoti stShowerLogoutNoti;
	stShowerLogoutNoti.m_nUserID = pStopPublishReq->m_nUserID;

	SendSSNoti(MSGID_SHOWERLOGOUT_NOTI, &stShowerLogoutNoti, pMsgHeadCS->m_nRoomID, pMsgHeadCS->m_nUserID, enmTransType_Server, enmEntityType_Media);
	SendSSNoti(MSGID_SHOWERLOGOUT_NOTI, &stShowerLogoutNoti, pMsgHeadCS->m_nRoomID, pMsgHeadCS->m_nUserID, enmTransType_Server, enmEntityType_MediaDispatch);
	return 0;
}

//其它服务器的表演者开始发布
int32_t CCommandHandler::ShowerLoginEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
//	WRITE_DEBUG_LOG(SERVER_NAME, "recv shower login event!\n");

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

	CMediaChannel *pShower = NEW(CMediaChannel);
	if(pShower == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "we cann't alloc memory for shower!\n");
		return 0;
	}

	pShower->SetServerID(pMsgHeadSS->m_nSrcID);
	pShower->SetIoSession(pIoSession);
	pShower->SetChannelKey(pMsgHeadSS->m_nRoomID, pShowerLoginNoti->m_nUserID, pShowerLoginNoti->m_nUserID);
	pShower->SetChannelType(enmChannelType_Server);
	pShower->StartShow();

	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	RoomParam stRoomParam(pMsgHeadSS->m_nRoomID, pShowerLoginNoti->m_nUserID, pShowerLoginNoti->m_nUserID);
	pSessionParam->AddRoomParam(stRoomParam);

	return 0;
}

//其它服务器的表演者停止发布
int32_t CCommandHandler::ShowerLogoutEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
//	WRITE_DEBUG_LOG(SERVER_NAME, "recv shower logout event!\n");

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

//	if(g_DataCenter.IsPublisher(pShowerLogoutNoti->m_nUserID))
//	{
//		g_DataCenter.DelPublisher(pShowerLogoutNoti->m_nUserID);
//	}

	CMediaChannel *pShower = g_DataCenter.FindChannel(pMsgHeadSS->m_nRoomID, pShowerLogoutNoti->m_nUserID, pShowerLogoutNoti->m_nUserID);
	if(pShower != NULL)
	{
		pShower->StopShow();
		DELETE(pShower);
		CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
		pSessionParam->DelRoomParam(pMsgHeadSS->m_nRoomID, pShowerLogoutNoti->m_nUserID, pShowerLogoutNoti->m_nUserID);
//		g_DataCenter.DelChannel(pShower);
//		DELETE(pShower);
	}

	return 0;
}

//用户订阅
int32_t CCommandHandler::UserSubscribeEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
//	WRITE_DEBUG_LOG(SERVER_NAME, "recv user subscribe event!\n");

	UserSubscribeResp stResp;
	stResp.m_nResult = 1;

	IIOSession *pIoSession = dynamic_cast<IIOSession *>(pObject);
	if(pIoSession == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate object to iosession error!\n");
		return 0;
	}

	MsgHeadCS *pMsgHeadCS = dynamic_cast<MsgHeadCS *>(pMsgHead);
	if(pMsgHeadCS == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msghead to mediahead error!\n");
		SendCSResp(MSGID_USERSUBSCRIBE_RESP, pMsgHead, &stResp, pIoSession);
		return 0;
	}

	UserSubscribeReq *pSubscribeReq = dynamic_cast<UserSubscribeReq *>(pMsgBody);
	if(pSubscribeReq == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to subscribe req error!\n");
		SendCSResp(MSGID_USERSUBSCRIBE_RESP, pMsgHead, &stResp, pIoSession);
		return 0;
	}

	//查找表演者
	CMediaChannel *pShower = g_DataCenter.FindChannel(pMsgHeadCS->m_nRoomID, pSubscribeReq->m_nUserID, pSubscribeReq->m_nUserID);
	if(pShower == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "she's not a shower!{SubscriberID=%d, ShowerID=%d}\n", pMsgHeadCS->m_nUserID, pSubscribeReq->m_nUserID);
		SendCSResp(MSGID_USERSUBSCRIBE_RESP, pMsgHead, &stResp, pIoSession);
		return 0;
	}

	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	if((!pShower->IsLocalServer()) && (pShower->GetReciverCount() <= 0))
	{
		ServerSubscribeReq stReq;
		stReq.m_nShowerID = pSubscribeReq->m_nUserID;
		SendSSReq(MSGID_SERVERSUBSCRIBE_REQ, pMsgHeadCS->m_nRoomID, pServerConfig->GetServerID(), &stReq,
				enmEntityType_Media, pShower->GetServerID());
	}

	//查找订阅者
	CMediaChannel *pSubscriber = g_DataCenter.FindChannel(pMsgHeadCS->m_nRoomID, pSubscribeReq->m_nUserID, pMsgHeadCS->m_nUserID);
	if(pSubscriber == NULL)
	{
		pSubscriber = NEW(CMediaChannel);
		if(pSubscriber == NULL)
		{
			WRITE_WARN_LOG(SERVER_NAME, "we cann't alloc memory to subscriber!{SubscriberID=%d, ShowerID=%d}\n", pMsgHeadCS->m_nUserID, pSubscribeReq->m_nUserID);
			SendCSResp(MSGID_USERSUBSCRIBE_RESP, pMsgHead, &stResp, pIoSession);
			return 0;
		}

		pSubscriber->SetServerID(pServerConfig->GetServerID());
		pSubscriber->SetIoSession(pIoSession);
		pSubscriber->SetChannelKey(pMsgHeadCS->m_nRoomID, pSubscribeReq->m_nUserID, pMsgHeadCS->m_nUserID);
		pSubscriber->SetChannelType(enmChannelType_User);
	}
	else
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's found subscriber exist!{RoomID=%d, ShowerID=%d, SubscriberID=%d}\n", pMsgHeadCS->m_nRoomID, pMsgHeadCS->m_nUserID, pSubscribeReq->m_nUserID);
		SendCSResp(MSGID_USERSUBSCRIBE_RESP, pMsgHead, &stResp, pIoSession);
		return 0;
	}

	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();

	RoomParam stRoomParam(pMsgHeadCS->m_nRoomID, pSubscribeReq->m_nUserID, pMsgHeadCS->m_nUserID);
	pSessionParam->AddRoomParam(stRoomParam);

	stResp.m_nResult = 0;
	SendCSResp(MSGID_USERSUBSCRIBE_RESP, pMsgHead, &stResp, pIoSession);

	pShower->Join(pSubscriber);

	return 0;
}

//用户取消订阅
int32_t CCommandHandler::UserUnsubscribeEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
	WRITE_DEBUG_LOG(SERVER_NAME, "recv user unsubscribe event!\n");

	IIOSession *pIoSession = dynamic_cast<IIOSession *>(pObject);
	if(pIoSession == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate object to iosession error!\n");
		return 0;
	}

	MsgHeadCS *pMsgHeadCS = dynamic_cast<MsgHeadCS *>(pMsgHead);
	if(pMsgHeadCS == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msghead to msgheadcs error!\n");
		return 0;
	}

	UserUnsubscribeReq *pUnsubscribeReq = dynamic_cast<UserUnsubscribeReq *>(pMsgBody);
	if(pUnsubscribeReq == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to subscribe req error!\n");
		return 0;
	}

	CMediaChannel *pShower = g_DataCenter.FindChannel(pMsgHeadCS->m_nRoomID, pUnsubscribeReq->m_nUserID, pUnsubscribeReq->m_nUserID);
	if(pShower == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found shower channel!{UserID=%d}\n", pUnsubscribeReq->m_nUserID);
		return 0;
	}

	CMediaChannel *pSubscriber = g_DataCenter.FindChannel(pMsgHeadCS->m_nRoomID, pUnsubscribeReq->m_nUserID, pMsgHeadCS->m_nUserID);
	if(pSubscriber == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found subscriber channel!{UserID=%d}\n", pMsgHeadCS->m_nUserID);
		return 0;
	}

	pShower->Leave(pSubscriber);
	DELETE(pSubscriber);

	return 0;
}

//其它服务器订阅
int32_t CCommandHandler::ServerSubscribeEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
//	WRITE_DEBUG_LOG(SERVER_NAME, "recv server subscribe event!\n");

	ServerSubscribeResp stResp;
	stResp.m_nResult = 1;

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

	ServerSubscribeReq *pServerSubscribeReq = dynamic_cast<ServerSubscribeReq *>(pMsgBody);
	if(pServerSubscribeReq == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to server subscribe req error!\n");
		return 0;
	}

	//查找表演者
	CMediaChannel *pShower = g_DataCenter.FindChannel(pMsgHeadSS->m_nRoomID, pServerSubscribeReq->m_nShowerID, pServerSubscribeReq->m_nShowerID);
	if(pShower == NULL)
	{
		WRITE_ERROR_LOG(SERVER_NAME, "she's not a shower!{SubscriberID=%d, ShowerID=%d}\n", pMsgHeadSS->m_nUserID, pServerSubscribeReq->m_nShowerID);
		return 0;
	}

	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	if(pShower->GetServerID() != pServerConfig->GetServerID())
	{
		WRITE_ERROR_LOG(SERVER_NAME, "shower is not in local server!{BelongServerID=%d, LocalServerID=%d, ShowerID=%d}\n",
				pShower->GetServerID(), pServerConfig->GetServerID(), pServerSubscribeReq->m_nShowerID);
		return 0;
	}

	//查找订阅者
	CMediaChannel *pSubscriber = g_DataCenter.FindChannel(pMsgHeadSS->m_nRoomID, pServerSubscribeReq->m_nShowerID, pMsgHeadSS->m_nSrcID);
	if(pSubscriber == NULL)
	{
		pSubscriber = NEW(CMediaChannel);
		if(pSubscriber == NULL)
		{
			WRITE_WARN_LOG(SERVER_NAME, "we cann't alloc memory to shower!{SubscriberID=%d, ShowerID=%d}\n", pMsgHeadSS->m_nUserID, pServerSubscribeReq->m_nShowerID);
			return 0;
		}

		pSubscriber->SetServerID(pMsgHeadSS->m_nSrcID);
		pSubscriber->SetIoSession(pIoSession);
		pSubscriber->SetChannelKey(pMsgHeadSS->m_nRoomID, pServerSubscribeReq->m_nShowerID, pMsgHeadSS->m_nSrcID);
		pSubscriber->SetChannelType(enmChannelType_Server);
	}
	else
	{
		WRITE_ERROR_LOG(SERVER_NAME, "subscriber is exist!{RoomID=%d, ShowerID=%d, SubscriberID=%d}\n", pMsgHeadSS->m_nRoomID, pServerSubscribeReq->m_nShowerID, pMsgHeadSS->m_nSrcID);
		return 0;
	}

	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	RoomParam stRoomParam(pMsgHeadSS->m_nRoomID, pServerSubscribeReq->m_nShowerID, pMsgHeadSS->m_nSrcID);
	pSessionParam->AddRoomParam(stRoomParam);

	pShower->Join(pSubscriber);
	return 0;
}

//其它服务器取消订阅
int32_t CCommandHandler::ServerUnsubscribeEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
//	WRITE_DEBUG_LOG(SERVER_NAME, "recv server unsubscribe event!\n");

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

	ServerUnsubscribeReq *pServerUnsubscribeReq = dynamic_cast<ServerUnsubscribeReq *>(pMsgBody);
	if(pServerUnsubscribeReq == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to server unsubscribe req error!\n");
		return 0;
	}

	CMediaChannel *pShower = g_DataCenter.FindChannel(pMsgHeadSS->m_nRoomID, pServerUnsubscribeReq->m_nShowerID, pServerUnsubscribeReq->m_nShowerID);
	if(pShower == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found shower channel!{UserID=%d}\n", pServerUnsubscribeReq->m_nShowerID);
		return 0;
	}

	CMediaChannel *pSubscriber = g_DataCenter.FindChannel(pMsgHeadSS->m_nRoomID, pServerUnsubscribeReq->m_nShowerID, pMsgHeadSS->m_nSrcID);
	if(pSubscriber == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "it's not found subscriber channel!{UserID=%d}\n", pMsgHeadSS->m_nSrcID);
		return 0;
	}

	CSessionParam *pSessionParam = (CSessionParam *)pIoSession->GetParamPtr();
	pSessionParam->FirstRoomParam();

	pShower->Leave(pSubscriber);
	DELETE(pSubscriber);

	return 0;
}

//通知方告知自己所拥有的发布者
int32_t CCommandHandler::ShowerListEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
//	WRITE_DEBUG_LOG(SERVER_NAME, "recv shower list event!\n");

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

	for(int32_t i = 0; i < pShowerListNoti->m_nShowerCount; ++i)
	{
		CMediaChannel *pShower = NEW(CMediaChannel);
		if(pShower == NULL)
		{
			WRITE_WARN_LOG(SERVER_NAME, "we cann't alloc memory for shower!\n");
			return 0;
		}

		pShower->SetServerID(pMsgHeadSS->m_nSrcID);
		pShower->SetIoSession(pIoSession);
		pShower->SetChannelKey(pShowerListNoti->m_arrRoomID[i], pShowerListNoti->m_arrShowerID[i], pShowerListNoti->m_arrShowerID[i]);
		pShower->SetChannelType(enmChannelType_Server);
		pShower->StartShow();
	}

	return 0;
}

//心跳请求
int32_t CCommandHandler::HeartBeatEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody)
{
//	WRITE_DEBUG_LOG(SERVER_NAME, "recv heart beat event!\n");

	IIOSession *pIoSession = dynamic_cast<IIOSession *>(pObject);
	if(pIoSession == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate object to iosession error!\n");
		return 0;
	}

	MsgHeadCS *pMsgHeadCS = dynamic_cast<MsgHeadCS *>(pMsgHead);
	if(pMsgHeadCS == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msghead to msgheadcs error!\n");
		return 0;
	}

	HeartBeatReq *pHeartBeatReq = dynamic_cast<HeartBeatReq *>(pMsgBody);
	if(pHeartBeatReq == NULL)
	{
		WRITE_WARN_LOG(SERVER_NAME, "translate msgbody to heart beat req error!\n");
		return 0;
	}

	CTimerHandler *pTimerHandler = g_DataCenter.GetTimerHandler();
	pTimerHandler->MoveToIdle(pIoSession);

	return 0;
}

#endif /* COMMAND_HANDLER_CPP_ */
