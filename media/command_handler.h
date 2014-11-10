/*
 * command_handler.h
 *
 *  Created on: 2014年5月11日
 *      Author: jimm
 */

#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

#include "server_helper.h"
#include "../frame/frame_typedef.h"
using namespace FRAME;

class CCommandHandler : public CServerHelper
{
public:
	//服务器注册
	int32_t ServerRegistEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//服务器卸载
	int32_t ServerUnregistEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//用户注册
	int32_t UserRegistEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//用户卸载
	int32_t UserUnregistEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//用户开始发布
	int32_t StartPublishEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//用户停止发布
	int32_t StopPublishEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//表演者登陆服务器
	int32_t ShowerLoginEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//表演者退出服务器
	int32_t ShowerLogoutEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//用户订阅
	int32_t UserSubscribeEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//用户取消订阅
	int32_t UserUnsubscribeEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	////其它服务器订阅
	//int32_t ServerSubscribeEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	////其它服务器取消订阅
	//int32_t ServerUnsubscribeEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//通知方告知自己所拥有的发布者
	int32_t ShowerListEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//心跳
	int32_t HeartBeatEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
};


#endif /* COMMAND_HANDLER_H_ */
