/*
 * regist_message.h
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#ifndef REGIST_MESSAGE_H_
#define REGIST_MESSAGE_H_

#include "../frame/frame.h"
#include "command_handler.h"
#include "../include/message_head.h"
#include "../include/message_define.h"

MSGMAP_BEGIN(msgmap)
ON_PROC_PMH_PMB(MSGID_SERVERREGIST_REQ, MsgHeadSS, ServerRegistReq, CCommandHandler, CCommandHandler::ServerRegistEvent);
ON_PROC_PMH_PMB(MSGID_SERVERUNREGIST_REQ, MsgHeadSS, ServerUnregistReq, CCommandHandler, CCommandHandler::ServerUnregistEvent);
ON_PROC_PMH_PMB(MSGID_SHOWERLOGIN_NOTI, MsgHeadSS, ShowerLoginNoti, CCommandHandler, CCommandHandler::ShowerLoginEvent);
ON_PROC_PMH_PMB(MSGID_SHOWERLOGOUT_NOTI, MsgHeadSS, ShowerLogoutNoti, CCommandHandler, CCommandHandler::ShowerLogoutEvent);
ON_PROC_PMH_PMB(MSGID_SHOWERLIST_NOTI, MsgHeadSS, ShowerListNoti, CCommandHandler, CCommandHandler::ShowerListEvent);
ON_PROC_PMH_PMB(MSGID_UPDATESERVERLOAD_NOTI, MsgHeadSS, UpdateServerLoadNoti, CCommandHandler, CCommandHandler::UpdateServerLoadEvent);
MSGMAP_END(msgmap)


#endif /* REGIST_MESSAGE_H_ */
