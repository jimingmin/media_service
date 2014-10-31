/*
 * group_info.h
 *
 *  Created on: 2014年7月7日
 *      Author: jimm
 */

#ifndef GROUP_INFO_H_
#define GROUP_INFO_H_

#include "../common/common_object.h"
#include "../include/typedef.h"
#include <map>
#include <string>
using namespace std;

typedef map<UserID, string>		ShowerMap;
typedef map<string, LoadInfo>	ServerLoadMap;

class CGroupInfo : public CBaseObject
{
public:
	virtual int32_t Init();

	virtual int32_t Uninit();

	virtual int32_t GetSize();

	void SetGroupID(int32_t nGroupID);

	int32_t GetGroupID();

	void AddShower(UserID nUserID, string strServerAddress);

	void DelShower(UserID nUserID);

	string FindShower(UserID nUserID);

	void AddServer(string strServerAddress);

	void DelServer(string strServerAddress);

	void UpdateServerLoadInfo(string strServerAddress, LoadInfo &stLoadInfo);

	int32_t GetAllServerCount(string arrServerAddress[], int32_t nSize);

	string GetServerListString();

	LoadInfo GetLoadInfo(string strServerAddress);

protected:
	int32_t				m_nGroupID;
	ShowerMap			m_stShowerMap;
	ServerLoadMap		m_stServerLoadMap;
};


#endif /* GROUP_INFO_H_ */
