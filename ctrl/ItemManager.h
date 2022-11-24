#pragma once

#include <afxtempl.h>
typedef struct LIST_ITEM_INFO
{
	LIST_ITEM_INFO() {
		nSequenceNo = -1;
	}
	int nSequenceNo;
	CString sText;
	LPVOID lpData;
}listItem, *pListItem;

class CItemManager
{
public:
	CItemManager();
	~CItemManager();

public:
	void AddItem(int nSeq, CString sText, LPVOID lpData = NULL);
	bool InsertItem(int nIndex, int nSeq, CString sText, LPVOID lpData = NULL);
	bool RemoveItem(int nIndex);
	int  GetCount();
	pListItem GetItemInfo(int nIndex);


private:
	void ReleaseItem();

protected:
	CList<pListItem, pListItem> m_listItem;
	CCriticalSectionEx m_sync;
};

