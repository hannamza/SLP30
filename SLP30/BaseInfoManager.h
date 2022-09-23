#pragma once

#include "TSingleton.h"
#include "CriticalSectionEx.h"
#include <afxtempl.h>
#include <atlcoll.h>

#define		BASIC_COUNT		10

typedef struct BASIC_TYPE
{
	int nIndex;
	CString sInput;
	CString sEquip;
	CString sOutput;
	CString sOutputExp;
}basicType, *pBasicType;

class CBaseInfoManager : public TSingleton<CBaseInfoManager>
{
public:
	CBaseInfoManager();
	~CBaseInfoManager();

private:
	void InitializeBasic();
	void ReleaseManager();
	pBasicType GetBasicType(CString sEquip);
	pBasicType GetBasicType(int nIndex);

protected:
	CList<pBasicType, pBasicType> m_listBasic;
	typedef CAtlMap<int, pBasicType> MAP_BASIC;
	MAP_BASIC m_mapBasic;
};

