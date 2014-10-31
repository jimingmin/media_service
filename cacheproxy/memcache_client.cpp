/*
 * memcache_client.cpp
 *
 *  Created on: 2014年7月11日
 *      Author: jimm
 */

#include "memcache_client.h"
#include <iostream>
#include <time.h>
#include <string.h>
#include "server_config.h"
#include "datacenter.h"
#include "../frame/frame.h"

using namespace FRAME;
using namespace std;

CMemcacheClient::CMemcacheClient()
{
	memcached_return rc;

	m_pMemc = memcached_create(NULL);

	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	m_pServer = memcached_server_list_append(NULL, pServerConfig->GetCacheAddress(), pServerConfig->GetCachePort(), &rc);

	rc = memcached_server_push(m_pMemc, m_pServer);

	if (MEMCACHED_SUCCESS != rc)
	{
		cout << "memcached_server_push failed! rc: " << rc << endl;
	}

	memcached_server_list_free(m_pServer);
	m_pServer = NULL;

	//设置为非阻塞
	memcached_behavior_set(m_pMemc, MEMCACHED_BEHAVIOR_NO_BLOCK, 1);
}

CMemcacheClient::~CMemcacheClient()
{
	if(m_pMemc != NULL)
	{
		memcached_free(m_pMemc);
		m_pMemc = NULL;
	}
}

int32_t CMemcacheClient::Set(const char* szKey, const char* szValue, time_t expiration)
{
    if (NULL == szKey || NULL == szValue)
    {
        return -1;
    }

    uint32_t flags = 0;

    memcached_return rc;

    rc = memcached_set(m_pMemc, szKey, strlen(szKey),szValue, strlen(szValue), expiration, flags);

    // insert ok
    if (MEMCACHED_SUCCESS == rc)
    {
        return 1;
    }
    else
    {
        return 0;
    }
};


string CMemcacheClient::Get(const char* szKey)
{
	if (NULL == szKey)
	{
		return "";
    }
    uint32_t flags = 0;

    memcached_return rc;

    size_t szValue_length;
    char* szValue = memcached_get(m_pMemc, szKey, strlen(szKey), &szValue_length, &flags, &rc);

    // get ok
    if(rc == MEMCACHED_SUCCESS)
    {
        return szValue;
    }

    return "";
};

int32_t CMemcacheClient::Del(const char *szKey)
{
	if (NULL == szKey)
	{
		return 0;
    }

	memcached_return rc = memcached_delete(m_pMemc, szKey, strlen(szKey), 0);

	return 0;
}

