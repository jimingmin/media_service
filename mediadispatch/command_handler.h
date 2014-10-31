/*
 * command_handler.h
 *
 *  Created on: 2014年7月7日
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
	//表演者登陆服务器
	int32_t ShowerLoginEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//表演者退出服务器
	int32_t ShowerLogoutEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//更新服务器负载值
	int32_t UpdateServerLoadEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
	//发布者列表
	int32_t ShowerListEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
};

#endif /* COMMAND_HANDLER_H_ */
