/*
 * main.cpp
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#include <sys/stat.h>
#include <sys/types.h>
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
#include "packetparser_factory.h"
#include "server_config.h"
#include "public_define.h"
#include "server_typedef.h"
#include "flash_handler.h"

using namespace LOGGER;
using namespace NETEVENT;
using namespace FRAME;

int32_t InitNetAndTimer(CNetHandler *pNetHandler)
{
	CServerConfig *pServerConfig = (CServerConfig *)g_Frame.GetConfig(CONFIG_SERVER);
	if(pServerConfig == NULL)
	{
		return 1;
	}

	//创建包解析器工厂
	IPacketParserFactory *pFlashParserFactory = new CPacketParserFactory("\0");
	CFlashHandler *pFlashHandler = new CFlashHandler();

	int32_t arrListenPort[enmMaxListenPortCount];
	int32_t nListenPort = pServerConfig->GetAllListenPort(arrListenPort, enmMaxListenPortCount);
	for(int32_t i = 0; i < nListenPort; i++)
	{
		CAcceptor *pFlashServer = new CAcceptor(pNetHandler, pFlashParserFactory, pFlashHandler);
		pFlashServer->Bind(arrListenPort[i]);
	}

	CTimerHandler *pTimerHandler = new CTimerHandler();
	int32_t nTimerIndex = 0;
	g_Frame.CreateTimer(static_cast<TimerProc>(&CTimerHandler::HeartBeat),
		pTimerHandler, NULL, 10 * MS_PER_SECOND, true, nTimerIndex);
	
	pFlashHandler->SetTimerHandler(pTimerHandler);

	return 0;
}

int32_t main()
{
	//启动日志线程
	CLogger::Start();

	//创建网络事件处理器
	CNetHandler *pNetHandler = new CNetHandler();
	pNetHandler->CreateReactor();

	g_Frame.AddRunner(pNetHandler);

	g_Frame.Init(SERVER_NAME);

	InitNetAndTimer(pNetHandler);

	while(true)
	{
		g_Frame.Run();
	}

	g_Frame.Uninit();

	return 0;
}
