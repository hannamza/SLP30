#pragma once

#include "TSingleton.h"

class CPositionManager : public TSingleton<CPositionManager>
{
public:
	CPositionManager();
	~CPositionManager();

public:
	void SavePosition(CWnd* pWnd);
	bool LoadPosition(CWnd* pWnd, CRect & rect);
};

