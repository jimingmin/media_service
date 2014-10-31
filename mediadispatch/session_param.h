/*
 * session_param.h
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#ifndef SESSION_PARAM_H_
#define SESSION_PARAM_H_

#include "../common/common_object.h"
#include "../include/typedef.h"

class CSessionParam : public CBaseObject
{
public:
	CSessionParam()
	{
		Init();
	}

	virtual int32_t Init();

	virtual int32_t Uninit();

	virtual int32_t GetSize();

	void SetGroupID(int32_t nGroupID);

	int32_t GetGroupID();

	void SetEntityType(EntityType nEntityType);

	EntityType GetEntityType();

	void SetServerID(ServerID nServerID);

	ServerID GetServerID();

protected:
	int32_t			m_nGroupID;
	EntityType		m_nEntityType;
	ServerID		m_nServerID;
};

#endif /* SESSION_PARAM_H_ */
