/*
 * server_config.h
 *
 *  Created on: 2014年1月24日
 *      Author: jimm
 */

#ifndef SERVER_CONFIG_H_
#define SERVER_CONFIG_H_

#include "../frame/frame_impl.h"
#include "../frame/frame.h"
#include "../common/common_api.h"
#include "public_define.h"
#include "server_typedef.h"

#include <string.h>

using namespace FRAME;

class CServerConfig : public IConfig
{
public:
	CServerConfig(const char *szFileName)
	{
		strcpy(m_szConfigFile, szFileName);
		m_nListenPortCount = 0;
		memset(m_arrListenPort, 0, sizeof(m_arrListenPort));
	}

	virtual ~CServerConfig(){};

	//初始化配置
	virtual int32_t Init();
	//卸载配置
	virtual int32_t Uninit();

	int32_t GetAllListenPort(int32_t arrListenPort[], int32_t nSize);


protected:
	char		m_szConfigFile[enmMaxConfigFileNameSize];
	int32_t		m_nListenPortCount;
	int32_t		m_arrListenPort[1024];
};

//注册到配置管理器
REGIST_CONFIG(CONFIG_SERVER, CServerConfig);

#endif /* SERVER_CONFIG_H_ */
