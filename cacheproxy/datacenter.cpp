/*
 * datacenter.cpp
 *
 *  Created on: 2014年7月11日
 *      Author: jimm
 */

#include "datacenter.h"

CDataCenter::CDataCenter()
{
	m_pMemcacheClient = NULL;
}

CMemcacheClient *CDataCenter::GetMemcacheClient()
{
	if(m_pMemcacheClient == NULL)
	{
		m_pMemcacheClient = new CMemcacheClient();
	}

	return m_pMemcacheClient;
}

