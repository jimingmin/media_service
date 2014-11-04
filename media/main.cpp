﻿/*
 * main.cpp
 *
 *  Created on: 2014年5月12日
 *      Author: jimm
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "../netevent/net_namespace.h"
#include "../netevent/net_impl.h"
#include "../netevent/net_typedef.h"
#include "../netevent/net_reactor.h"
#include "../netevent/net_handler.h"
#include "../netevent/net_connection.h"
#include "../netevent/net_acceptor.h"
#include "../netevent/net_connector.h"
#include "../logger/logger.h"
#include "../frame/frame.h"
#include "server_config.h"
#include "regist_message.h"
#include "server_typedef.h"
#include "message_handler.h"
#include "packetparser.h"
#include "packetparser_factory.h"
#include "timer_handler.h"
#include "datacenter.h"

using namespace LOGGER;
using namespace NETEVENT;
using namespace FRAME;

int32_t InitNet(CNetHandler *pNetHandler)
{
	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	if(pServerConfig == NULL)
	{
		return 1;
	}

	//创建包解析器工厂
	IPacketParserFactory *pParserFactory = new CPacketParserFactory();

	CMessageHandler *pMessageHandler = new CMessageHandler();

	//创建服务器
	CAcceptor *pAcceptor = new CAcceptor(pNetHandler, pParserFactory, pMessageHandler);
	pAcceptor->Bind(pServerConfig->GetListenPort());

	NetNode arrNetNode[enmMaxNetNodeCount];
	int32_t nNetNodeCount = pServerConfig->GetAllNetNode(arrNetNode, enmMaxNetNodeCount);
	
	//创建连接器
	CConnector *pConnector = new CConnector(pNetHandler, pParserFactory, pMessageHandler);
	pMessageHandler->SetConnector(pConnector);

	for(int32_t i = 0; i < nNetNodeCount; ++i)
	{
		pConnector->Connect(arrNetNode[i].m_szPeerAddress, arrNetNode[i].m_nPeerPort);
	}

	return 0;
}

int32_t InitTimer()
{
	CTimerHandler *pTimerHandler = new CTimerHandler();
	int32_t nTimerIndex = 0;
	g_Frame.CreateTimer(static_cast<TimerProc>(&CTimerHandler::HeartBeat),
		pTimerHandler, NULL, 60 * MS_PER_SECOND, true, nTimerIndex);

	g_DataCenter.SetTimerHandler(pTimerHandler);

	return 0;
}

int32_t main(int argc, char** argv)
{
	bool bDaemon = false;

	//读取命令行参数
	if (argc > 1)
	{
		if (0 == strcmp(argv[1], "-d"))
		{
			bDaemon = true;
		}
	}
#ifndef WIN32
	int lock_fd = open((const char *)argv[0], O_RDONLY);
	if (lock_fd < 0)
	{
		exit(1);
	}

	if (flock(lock_fd, LOCK_EX | LOCK_NB) < 0)
	{
		printf("%s was launched!\n", SERVER_NAME);
		exit(1);
	}

#endif

	//启动日志线程
	CLogger::Start();

	//创建网络事件处理器
	CNetHandler *pNetHandler = new CNetHandler();
	pNetHandler->CreateReactor();

	g_Frame.AddRunner(pNetHandler);

	if(g_Frame.Init(SERVER_NAME, bDaemon) != 0)
	{
		return 0;
	}

	InitNet(pNetHandler);

	InitTimer();

	g_Frame.Run();

	g_Frame.Uninit();

	return 0;
}

