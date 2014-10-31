/*
 * datacenter.h
 *
 *  Created on: 2014年7月11日
 *      Author: jimm
 */

#ifndef DATACENTER_H_
#define DATACENTER_H_

#include "../common/common_singleton.h"
#include "memcache_client.h"

class CDataCenter
{
public:
	CDataCenter();

	CMemcacheClient *GetMemcacheClient();

protected:
	CMemcacheClient			*m_pMemcacheClient;
};

#define g_DataCenter		CSingleton<CDataCenter>::GetInstance()

#endif /* DATACENTER_H_ */
