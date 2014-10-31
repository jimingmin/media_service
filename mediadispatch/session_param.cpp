/*
 * session_param.cpp
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#include "session_param.h"

int32_t CSessionParam::Init()
{
	return 0;
}

int32_t CSessionParam::Uninit()
{
	return 0;
}

int32_t CSessionParam::GetSize()
{
	return sizeof(*this);
}

void CSessionParam::SetGroupID(int32_t nGroupID)
{
	m_nGroupID = nGroupID;
}

int32_t CSessionParam::GetGroupID()
{
	return m_nGroupID;
}

void CSessionParam::SetEntityType(EntityType nEntityType)
{
	m_nEntityType = nEntityType;
}

EntityType CSessionParam::GetEntityType()
{
	return m_nEntityType;
}

void CSessionParam::SetServerID(ServerID nServerID)
{
	m_nServerID = nServerID;
}

ServerID CSessionParam::GetServerID()
{
	return m_nServerID;
}

