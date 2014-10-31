/*
 * server_config.cpp
 *
 *  Created on: 2014年5月13日
 *      Author: jimm
 */

#include "server_config.h"

#include "../logger/logger.h"
#include "../tinyxml/tinyxml.h"
#include "server_typedef.h"

using namespace LOGGER;

//注册到配置管理器
REGIST_CONFIG(CONFIG_SERVER, CServerConfig);

//初始化配置
int32_t CServerConfig::Init()
{
	TiXmlDocument doc(m_szConfigFile);
	if (!doc.LoadFile(TIXML_ENCODING_UTF8))
	{
		WRITE_WARN_LOG(SERVER_NAME, "%s is not format utf8!\n", m_szConfigFile);
		return 1;
	}

	//获取根节点
	TiXmlElement *pRoot = doc.RootElement();
	if (NULL == pRoot)
	{
		WRITE_WARN_LOG(SERVER_NAME, "%s is not found server node!\n", m_szConfigFile);
		return 1;
	}

	TiXmlElement *pSelf = pRoot->FirstChildElement("self");
	if(NULL == pSelf)
	{
		WRITE_WARN_LOG(SERVER_NAME, "%s is not found self node!\n", m_szConfigFile);
		return 1;
	}

	const char* pszValue = NULL;

	pszValue = pSelf->Attribute("listen_port", &m_nListenPort);
	if(NULL == pszValue)
	{
		WRITE_WARN_LOG(SERVER_NAME, "%s is not found listen_port node!\n", m_szConfigFile);
		return 1;
	}

	pszValue = pSelf->Attribute("server_type", &m_nServerType);
	if(NULL == pszValue)
	{
		WRITE_WARN_LOG(SERVER_NAME, "%s is not found server_type node!\n", m_szConfigFile);
		return 1;
	}

	pszValue = pSelf->Attribute("server_id", &m_nServerID);
	if(NULL == pszValue)
	{
		WRITE_WARN_LOG(SERVER_NAME, "%s is not found server_id node!\n", m_szConfigFile);
		return 1;
	}

	TiXmlElement *pCache = pRoot->FirstChildElement("cache");
	if(NULL == pCache)
	{
		WRITE_WARN_LOG(SERVER_NAME, "%s is not found cache node!\n", m_szConfigFile);
		return 1;
	}

	pszValue = pCache->Attribute("server_address");
	if(NULL == pszValue)
	{
		WRITE_WARN_LOG(SERVER_NAME, "%s is not found server_address node!\n", m_szConfigFile);
		return 1;
	}

	strncpy(m_szCacheAddress, pszValue, strlen(pszValue));

	pszValue = pCache->Attribute("server_port", &m_nCachePort);
	if(NULL == pszValue)
	{
		WRITE_WARN_LOG(SERVER_NAME, "%s is not found server_port node!\n", m_szConfigFile);
		return 1;
	}

	return 0;
}

//卸载配置
int32_t CServerConfig::Uninit()
{
	return 0;
}


