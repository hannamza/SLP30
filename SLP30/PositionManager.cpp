#include "stdafx.h"
#include "PositionManager.h"


CPositionManager::CPositionManager()
{
}

CPositionManager::~CPositionManager()
{
}

void CPositionManager::SavePosition(CWnd* pWnd, bool bMax/*=false*/)
{
	CRect rect;
	pWnd->GetClientRect(&rect);
	pWnd->ClientToScreen(&rect);

	CReadWriteState state;
	state.SetFileName(L"pos.ini", 1);
	state.WriteState(L"POSITION", L"LEFT", rect.left);
	state.WriteState(L"POSITION", L"TOP", rect.top);
	state.WriteState(L"POSITION", L"RIGHT", rect.right);
	state.WriteState(L"POSITION", L"BOTTOM", rect.bottom);
	state.WriteState(L"POSITION", L"MAX", bMax);
}

bool CPositionManager::LoadPosition(CWnd* pWnd, CRect & rect, bool & bMax)
{
	CReadWriteState state;
	state.SetFileName(L"pos.ini", 1);
	int nValue;
	bool bResult = true;
	(state.ReadState(L"POSITION", L"LEFT", nValue) == true) ? rect.left = nValue : bResult = false;
	(state.ReadState(L"POSITION", L"TOP", nValue) == true) ? rect.top = nValue : bResult = false;
	(state.ReadState(L"POSITION", L"RIGHT", nValue) == true) ? rect.right = nValue : bResult = false;
	(state.ReadState(L"POSITION", L"BOTTOM", nValue) == true) ? rect.bottom = nValue : bResult = false;
	(state.ReadState(L"POSITION", L"MAX", nValue) == true) ? bMax = nValue : bResult = false;
	return bResult;
}
