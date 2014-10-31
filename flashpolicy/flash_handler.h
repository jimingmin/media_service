/*
 * flash_handler.h
 *
 *  Created on: 2014年2月20日
 *      Author: Administrator
 */

#ifndef FLASH_HANDLER_H_
#define FLASH_HANDLER_H_

#include "../netevent/net_impl.h"
#include "../netevent/net_ioadapter.h"

using namespace NETEVENT;

class CFlashHandler : public CIOHandlerAdapter
{
public:
	virtual int32_t OnOpened(IIOSession *pIoSession);

	virtual int32_t OnRecved(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize);

	virtual int32_t OnSent(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize);

	virtual int32_t OnClosed(IIOSession *pIoSession);

	virtual int32_t OnError(IIOSession *pIoSession);

	virtual int32_t OnTimeout(IIOSession *pIoSession);
};

#endif /* FLASH_HANDLER_H_ */
