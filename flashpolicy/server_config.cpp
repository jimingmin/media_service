/*
 * server_config.cpp
 *
 *  Created on: 2014年1月24日
 *      Author: jimm
 */

#include "server_config.h"

#include "../logger/logger.h"
#include "../tinyxml/tinyxml.h"
#include "server_typedef.h"

using namespace LOGGER;

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

	const char* pszValue = NULL;

	TiXmlElement *pListenPorts = pRoot->FirstChildElement("listen_ports");
	if (NULL == pListenPorts)
	{
		WRITE_WARN_LOG(SERVER_NAME, "%s is not found param listen_ports!\n", m_szConfigFile);
		return 1;
	}

	TiXmlElement *pPort = pListenPorts->FirstChildElement("port");
	if(NULL == pPort)
	{
		WRITE_WARN_LOG(SERVER_NAME, "%s is not found node port!\n", m_szConfigFile);
		return 1;
	}

	while(pPort != NULL)
	{
		pszValue = pPort->Attribute("value", (int32_t *)&m_arrListenPort[m_nListenPortCount]);
		if(NULL == pszValue)
		{
			WRITE_WARN_LOG(SERVER_NAME, "%s is not found value node!\n", m_szConfigFile);
			return 1;
		}

		++m_nListenPortCount;
		pPort = pPort->NextSiblingElement();
	}

	return 0;
}

//卸载配置
int32_t CServerConfig::Uninit()
{
	return 0;
}

int32_t CServerConfig::GetAllListenPort(int32_t arrListenPort[], int32_t nSize)
{
	int32_t nCount = m_nListenPortCount;
	if(m_nListenPortCount > nSize)
	{
		nCount = nSize;
	}

	for(int32_t i = 0; i < nCount; i++)
	{
		arrListenPort[i] = m_arrListenPort[i];
	}
	return nCount;
}
