/*
 * memcache_client.h
 *
 *  Created on: 2014年7月11日
 *      Author: jimm
 */

#ifndef MEMCACHE_CLIENT_H_
#define MEMCACHE_CLIENT_H_

#include <libmemcached/memcached.h>
#include <stdlib.h>
#include <string>

using namespace std;

class CMemcacheClient
{
public:
	CMemcacheClient();

	~CMemcacheClient();

    int32_t Set(const char* szKey, const char* szValue, time_t expiration = 3);

    string Get(const char* szKey);

    int32_t Del(const char *szKey);

private:
    memcached_st*		m_pMemc;
    memcached_server_st *m_pServer;
};



#endif /* MEMCACHE_CLIENT_H_ */
