/*
 * media_handler.h
 *
 *  Created on: 2014年5月12日
 *      Author: jimm
 */

#ifndef MEDIA_HANDLER_H_
#define MEDIA_HANDLER_H_

#include "server_helper.h"
#include "../frame/frame_typedef.h"
using namespace FRAME;

class CMediaHandler : public CServerHelper
{
public:
	int32_t TransmitMedia(CBaseObject *pObject, IMsgHead *pMsgHead, uint8_t *pBuf, int32_t nBufSize);

	int32_t TransmitServerMedia(CBaseObject *pObject, IMsgHead *pMsgHead, uint8_t *pBuf, int32_t nBufSize);
};


#endif /* MEDIA_HANDLER_H_ */
