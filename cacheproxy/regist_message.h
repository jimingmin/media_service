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
ON_PROC_PMH_PMB(MSGID_UPDATECACHE_NOTI, MsgHeadSS, UpdateCacheNoti, CCommandHandler, CCommandHandler::UpdateCacheEvent);
MSGMAP_END(msgmap)


#endif /* REGIST_MESSAGE_H_ */
