/*
 * regist_message.h
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#ifndef REGIST_MESSAGE_H_
#define REGIST_MESSAGE_H_

#include "../frame/frame.h"
#include "client_message.h"
#include "server_message.h"
#include "media_handler.h"
#include "command_handler.h"
#include "../include/message_head.h"
#include "../include/message_define.h"

MSGMAP_BEGIN(msgmap)
ON_PROC_PMH_PMB(MSGID_USERREGIST_REQ, MsgHeadCS, UserRegistReq, CCommandHandler, CCommandHandler::UserRegistEvent);
ON_PROC_PMH_PMB(MSGID_USERUNREGIST_REQ, MsgHeadCS, UserUnregistReq, CCommandHandler, CCommandHandler::UserUnregistEvent);
ON_PROC_PMH_PMB(MSGID_USERSUBSCRIBE_REQ, MsgHeadCS, UserSubscribeReq, CCommandHandler, CCommandHandler::UserSubscribeEvent);
ON_PROC_PMH_PMB(MSGID_USERUNSUBSCRIBE_REQ, MsgHeadCS, UserUnsubscribeReq, CCommandHandler, CCommandHandler::UserUnsubscribeEvent);
ON_PROC_PMH_PMB(MSGID_STARTPUBLISH_REQ, MsgHeadCS, StartPublishReq, CCommandHandler, CCommandHandler::StartPublishEvent);
ON_PROC_PMH_PMB(MSGID_STOPPUBLISH_REQ, MsgHeadCS, StopPublishReq, CCommandHandler, CCommandHandler::StopPublishEvent);
ON_PROC_PMH_PU8_I32(MSGID_TRANSMITDATA_NOTI, MediaHead, CMediaHandler, CMediaHandler::TransmitMedia);
ON_PROC_PMH_PU8_I32(MSGID_TRANSMEDIA_NOTI, TransMediaHead, CMediaHandler, CMediaHandler::TransmitServerMedia);
ON_PROC_PMH_PMB(MSGID_SERVERREGIST_REQ, MsgHeadSS, ServerRegistReq, CCommandHandler, CCommandHandler::ServerRegistEvent);
ON_PROC_PMH_PMB(MSGID_SERVERUNREGIST_REQ, MsgHeadSS, ServerUnregistReq, CCommandHandler, CCommandHandler::ServerUnregistEvent);
ON_PROC_PMH_PMB(MSGID_SHOWERLOGIN_NOTI, MsgHeadSS, ShowerLoginNoti, CCommandHandler, CCommandHandler::ShowerLoginEvent);
ON_PROC_PMH_PMB(MSGID_SHOWERLOGOUT_NOTI, MsgHeadSS, ShowerLogoutNoti, CCommandHandler, CCommandHandler::ShowerLogoutEvent);
//ON_PROC_PMH_PMB(MSGID_SERVERSUBSCRIBE_REQ, MsgHeadSS, ServerSubscribeReq, CCommandHandler, CCommandHandler::ServerSubscribeEvent);
//ON_PROC_PMH_PMB(MSGID_SERVERUNSUBSCRIBE_REQ, MsgHeadSS, ServerUnsubscribeReq, CCommandHandler, CCommandHandler::ServerUnsubscribeEvent);
ON_PROC_PMH_PMB(MSGID_SHOWERLIST_NOTI, MsgHeadSS, ShowerListNoti, CCommandHandler, CCommandHandler::ShowerListEvent);
ON_PROC_PMH_PMB(MSGID_HEARTBEAT_REQ, MsgHeadCS, HeartBeatReq, CCommandHandler, CCommandHandler::HeartBeatEvent);
MSGMAP_END(msgmap)


#endif /* REGIST_MESSAGE_H_ */

