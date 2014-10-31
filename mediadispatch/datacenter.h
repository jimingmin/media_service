/*
 * datacenter.h
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#ifndef DATACENTER_H_
#define DATACENTER_H_

#include "../common/common_singleton.h"
#include "group_info.h"
#include <map>
using namespace std;

typedef map<int32_t, CGroupInfo *>	GroupInfoMap;

class CDataCenter
{
public:
	void AddGroupInfo(int32_t nGroupID, CGroupInfo *pGroupInfo);

	void DelGroupInfo(int32_t nGroupID);

	CGroupInfo *FindGroupInfo(int32_t nGroupID);

	int32_t GetAllGroupID(int32_t arrGroupID[], int32_t nSize);
protected:
	GroupInfoMap		m_stGroupInfoMap;
};

#define g_DataCenter		CSingleton<CDataCenter>::GetInstance()

#endif /* DATACENTER_H_ */
