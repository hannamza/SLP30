#include "stdafx.h"
#include "BaseInfoManager.h"


CBaseInfoManager::CBaseInfoManager()
{
	InitializeBasic();
}

CBaseInfoManager::~CBaseInfoManager()
{
	ReleaseManager();
}

void CBaseInfoManager::InitializeBasic()
{
	pBasicType pType = NULL;
	for (int nIndex = 0; nIndex < BASIC_COUNT; nIndex) {
		pBasicType pType = new basicType;
		pType->nIndex = nIndex + 1;
		switch (nIndex) {
		case 1:  pType->sInput = L"��ȭ��"; pType->sEquip = L"��Ž������"; pType->sOutput = L"��������"; pType->sOutputExp = L"���� ����"; break;
		case 2:  pType->sInput = L"�߽ű�"; pType->sEquip = L"��Ž������"; pType->sOutput = L"��������"; pType->sOutputExp = L"���� ����"; break;
		case 3:  pType->sInput = L"��ܰ�����"; pType->sEquip = L"��Ž������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 4:  pType->sInput = L"ELEV������"; pType->sEquip = L"��Ž������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 5:  pType->sInput = L"������"; pType->sEquip = L"��Ž������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 6:  pType->sInput = L"AN����"; pType->sEquip = L"���ľƳ��α�"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 7:  pType->sInput = L"AN����"; pType->sEquip = L"���ľƳ��α�"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 8:  pType->sInput = L"�ּ�������"; pType->sEquip = L"���� �ּ���"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 9:  pType->sInput = L"�ּ�������"; pType->sEquip = L"���� �ּ���"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 10: pType->sInput = L"�ּ�������"; pType->sEquip = L"���� �ּ���"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 11: pType->sInput = L"SVP������A"; pType->sEquip = L"A����������"; pType->sOutput = L"���̷�"; pType->sOutputExp = L"���̷�"; break;
		case 12: pType->sInput = L"SVP������B"; pType->sEquip = L"B����������"; pType->sOutput = L"�����׼�"; pType->sOutputExp = L"���"; break;
		case 13: pType->sInput = L"SVP P/S"; pType->sEquip = L"ȭ�������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 14: pType->sInput = L"SVP T/S"; pType->sEquip = L"���ú�����"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 15: pType->sInput = L"AV P/S"; pType->sEquip = L"ȭ�������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 16: pType->sInput = L"AV T/S"; pType->sEquip = L"���ú�����"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 17: pType->sInput = L"���Ϳ�������"; pType->sEquip = L"��Ž������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 18: pType->sInput = L"���Ϳ�������"; pType->sEquip = L"��Ž������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 19: pType->sInput = L"����1�����"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"��ȭ����"; pType->sOutputExp = L"1�����"; break;
		case 20: pType->sInput = L"����2�����"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"��ȭ����"; pType->sOutputExp = L"2�����"; break;
		case 21: pType->sInput = L"���Ǳޱ����"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"��������"; pType->sOutputExp = L"��������"; break;
		case 22: pType->sInput = L"���Ǳޱ����"; pType->sEquip = L"ȭ�������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 23: pType->sInput = L"���ǹ�����"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"��������"; pType->sOutputExp = L"��������"; break;
		case 24: pType->sInput = L"���ǹ�����"; pType->sEquip = L"ȭ�������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 25: pType->sInput = L"�ڵ����"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"���Թ�����"; pType->sOutputExp = L"�ڵ����"; break;
		case 26: pType->sInput = L"â�����"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"â������"; pType->sOutputExp = L"â�����"; break;
		case 27: pType->sInput = L"�迬â"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"â������"; pType->sOutputExp = L"�迬â"; break;
		case 28: pType->sInput = L"��ȭ��"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"���Թ�����"; pType->sOutputExp = L"��ȭ��"; break;
		case 29: pType->sInput = L"P/L����"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"��Ÿ"; pType->sOutputExp = L"��ȭ�� �⵿"; break;
		case 30: pType->sInput = L"����������"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"��Ÿ"; pType->sOutputExp = L"������ ����"; break;
		case 31: pType->sInput = L"����������A"; pType->sEquip = L"��Ž������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 32: pType->sInput = L"����������B"; pType->sEquip = L"��Ž������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 33: pType->sInput = L"��������Ȯ��"; pType->sEquip = L"ȭ�������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 34: pType->sInput = L"���������⵿"; pType->sEquip = L"ȭ�������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 35: pType->sInput = L"������F/S"; pType->sEquip = L"���ú�����"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 36: pType->sInput = L"PUMP P/S"; pType->sEquip = L"���ú�����"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 37: pType->sInput = L"PUMP T/S"; pType->sEquip = L"���ú�����"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 38: pType->sInput = L"���������⵿"; pType->sEquip = L"ȭ�������"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 39: pType->sInput = L"SMD Ȯ��"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"������"; pType->sOutputExp = L"�ŽǴ���"; break;
		case 40: pType->sInput = L"MVD Ȯ��"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"��������"; pType->sOutputExp = L"���մ���"; break;
		case 41: pType->sInput = L"�ޱ���"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"������"; pType->sOutputExp = L"�ޱ���"; break;
		case 42: pType->sInput = L"�����"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"������"; pType->sOutputExp = L"�����"; break;
		case 43: pType->sInput = L"������T/S"; pType->sEquip = L"���ú�����"; pType->sOutput = L""; pType->sOutputExp = L""; break;
		case 44: pType->sInput = L"����"; pType->sEquip = L"�Ϲ�Ȯ��"; pType->sOutput = L"���Թ�����"; pType->sOutputExp = L"����"; break;
		default: break;
		}
		m_listBasic.AddTail(pType);
		m_mapBasic.SetAt(nIndex, pType);
	}
}

void CBaseInfoManager::ReleaseManager()
{
	pBasicType pType = NULL;
	while (!m_listBasic.IsEmpty()) {
		pType = m_listBasic.RemoveHead();
		SAFE_DELETE(pType);
	}
	m_mapBasic.RemoveAll();
}

pBasicType CBaseInfoManager::GetBasicType(CString sEquip)
{
	pBasicType pType = NULL;
	POSITION pos = m_listBasic.GetHeadPosition();
	while (pos) {
		pType = m_listBasic.GetNext(pos);
		if (pType && pType->sEquip == sEquip) {
			return pType;
		}
	}
	return NULL;
}

pBasicType CBaseInfoManager::GetBasicType(int nIndex)
{
	pBasicType pType = NULL;
	if (m_mapBasic.Lookup(nIndex, pType)) {
		return pType;
	}
	return NULL;
}
