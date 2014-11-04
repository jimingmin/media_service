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
#include "server_typedef.h"
#include "../include/typedef.h"

#include <string.h>

using namespace FRAME;

class CServerConfig : public IConfig
{
public:
	CServerConfig(const char *pConfigName)
	{
		strcpy(m_szConfigFile, CONFIG_SERVER);
		m_nGroupID = 0;
		m_nListenPort = 0;
		m_nServerType = 0;
		m_nServerID = 0;
		m_nServerRole = enmServerRole_Edge;
		m_nNetNodeCount = 0;
		m_nMaxConnCount = 0;
	}

	virtual ~CServerConfig(){};

	//初始化配置
	virtual int32_t Init();
	//卸载配置
	virtual int32_t Uninit();

	int32_t GetGroupID()
	{
		return m_nGroupID;
	}

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

	ServerRole GetServerRole()
	{
		return m_nServerRole;
	}

	int32_t GetAllNetNode(NetNode arrNetNode[], int32_t nSize)
	{
		int32_t nCount = 0;
		for(int32_t i = 0; i < m_nNetNodeCount; ++i)
		{
			if(nCount >= nSize)
			{
				break;
			}

			arrNetNode[i] = m_arrNetNode[i];
			++nCount;
		}

		return nCount;
	}

	NetNode *FindNetNode(char *szAddress, uint16_t nPort)
	{
		for(int32_t i = 0; i < m_nNetNodeCount; ++i)
		{
			if((strcmp(m_arrNetNode[i].m_szPeerAddress, szAddress) == 0) &&
					(m_arrNetNode[i].m_nPeerPort == nPort))
			{
				return &m_arrNetNode[i];
			}
		}

		return NULL;
	}

	//随机获取一个服务类型的结点
	NetNode *GetOneNetNode(int32_t nEntityType)
	{
		int32_t nCount = 0;
		int32_t arrNetNodeIndex[enmMaxNetNodeCount];
		for(int32_t i = 0; i < m_nNetNodeCount; ++i)
		{
			if(m_arrNetNode[i].m_nPeerType == nEntityType)
			{
				arrNetNodeIndex[nCount++] = i;
			}
		}

		if(nCount <= 0)
		{
			return NULL;
		}
		int32_t nIndex = Random(nCount);
		return &m_arrNetNode[arrNetNodeIndex[nIndex]];
	}

	int32_t GetMaxConnCount()
	{
		return m_nMaxConnCount;
	}
protected:
	char		m_szConfigFile[enmMaxConfigFileNameSize];
	int32_t		m_nGroupID;
	int32_t		m_nListenPort;
	int32_t		m_nServerType;
	int32_t		m_nServerID;
	ServerRole	m_nServerRole;
	int32_t		m_nNetNodeCount;
	NetNode		m_arrNetNode[enmMaxNetNodeCount];
	int32_t		m_nMaxConnCount;
};

#endif /* SERVER_CONFIG_H_ */
