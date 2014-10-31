/*
 * command_handler.h
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

#include "../common/common_object.h"
#include "../frame/frame_typedef.h"
using namespace FRAME;

class CCommandHandler : public CBaseObject
{
public:
	virtual int32_t Init()
	{
		return 0;
	}

	virtual int32_t Uninit()
	{
		return 0;
	}

	virtual int32_t GetSize()
	{
		return sizeof(*this);
	}
	//更新缓存
	int32_t UpdateCacheEvent(CBaseObject *pObject, IMsgHead *pMsgHead, IMsgBody *pMsgBody);
};

#endif /* COMMAND_HANDLER_H_ */
