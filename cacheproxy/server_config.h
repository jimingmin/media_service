/*
 * server_config.h
 *
 *  Created on: 2014年5月12日
 *      Author: jimm
 */

#ifndef SERVER_CONFIG_H_
#define SERVER_CONFIG_H_

#include "../frame/frame_impl.h"
#include "../frame/frame.h"
#include "../common/common_api.h"
#include "../include/typedef.h"
#include "server_typedef.h"

#include <string.h>

using namespace FRAME;

class CServerConfig : public IConfig
{
public:
	CServerConfig(const char *pConfigName)
	{
		strcpy(m_szConfigFile, CONFIG_SERVER);
		m_nListenPort = 0;
		m_nServerType = 0;
		m_nServerID = 0;
		memset(m_szCacheAddress, 0, sizeof(m_szCacheAddress));
		m_nCachePort = 0;
	}

	virtual ~CServerConfig(){};

	//初始化配置
	virtual int32_t Init();
	//卸载配置
	virtual int32_t Uninit();

	int32_t GetListenPort()
	{
		return m_nListenPort;
	}

	int32_t GetServerType()
	{
		return m_nServerType;
	}

	int32_t GetServerID()
	{
		return m_nServerID;
	}

	char *GetCacheAddress()
	{
		return m_szCacheAddress;
	}

	int32_t GetCachePort()
	{
		return m_nCachePort;
	}
protected:
	char		m_szConfigFile[enmMaxConfigFileNameSize];
	int32_t		m_nListenPort;
	int32_t		m_nServerType;
	int32_t		m_nServerID;
	char		m_szCacheAddress[enmIPV4NetAddressSize];
	int32_t		m_nCachePort;
};

#endif /* SERVER_CONFIG_H_ */
