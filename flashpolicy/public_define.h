/*
 * public_define.h
 *
 *  Created on: 2014年1月22日
 *      Author: jimm
 */

#ifndef PUBLIC_DEFINE_H_
#define PUBLIC_DEFINE_H_

enum
{
	enmMaxConfigFileNameSize	= 1024,		//配置文件名长度
	enmMaxPacketTokenSize		= 64,		//最大包分割字符串长度
	enmIPV4NetAddressSize		= 16,		//IPV4网络地址长度
	enmMaxListenPortCount		= 1024,		//最大监听的端口数量
};

#define FLASH_SEC_REQ	"<policy-file-request/>"
#define FLASH_SEC_RESP	"<?xml version=\"1.0\"?><cross-domain-policy><site-control permitted-cross-domain-policies=\"all\"/><allow-access-from domain=\"*\" to-ports=\"*\"/></cross-domain-policy>"


#endif /* PUBLIC_DEFINE_H_ */

