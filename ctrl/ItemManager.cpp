#include "stdafx.h"
#include "ItemManager.h"


CItemManager::CItemManager()
{
}

CItemManager::~CItemManager()
{
	ReleaseItem();
}

void CItemManager::AddItem(int nSeq, CString sText, LPVOID lpData /*= NULL*/)
{
	pListItem pItem = new listItem;
	pItem->nSequenceNo = nSeq;
	pItem->sText = sText;
	pItem->lpData = lpData;
	m_listItem.AddTail(pItem);
}

bool CItemManager::InsertItem(int nIndex, int nSeq, CString sText, LPVOID lpData /*= NULL*/)
{
	bool bResult = false;
	m_sync.Enter();
	if (m_listItem.GetCount() > nIndex) {
		pListItem pItem = new listItem;
		pItem->nSequenceNo = nSeq;
		pItem->sText = sText;
		pItem->lpData = lpData;

		m_listItem.InsertBefore(m_listItem.FindIndex(nIndex), pItem);
		bResult = true;
	}
	m_sync.Leave();

	return bResult;
}

bool CItemManager::RemoveItem(int nIndex)
{
	bool bResult = false;
	m_sync.Enter();
	if (m_listItem.GetCount() > nIndex) {
		pListItem pItem = m_listItem.GetAt(m_listItem.FindIndex(nIndex));
		m_listItem.RemoveAt(m_listItem.FindIndex(nIndex));
		SAFE_DELETE(pItem);
		bResult = true;
	}
	m_sync.Leave();

	return bResult;
}

int  CItemManager::GetCount()
{
	m_sync.Enter();
	int nCount = m_listItem.GetCount();
	m_sync.Leave();
	return nCount;
}

pListItem CItemManager::GetItemInfo(int nIndex)
{
	pListItem pItem = NULL;
	m_sync.Enter();
	if (m_listItem.GetCount() > nIndex) {
		pItem = m_listItem.GetAt(m_listItem.FindIndex(nIndex));
	}
	m_sync.Leave();

	return pItem;
}

void CItemManager::ReleaseItem()
{
	m_sync.Enter();
	while (!m_listItem.IsEmpty()) {
		pListItem pItem = m_listItem.RemoveHead();
		SAFE_DELETE(pItem);
	}
	m_sync.Leave();
}