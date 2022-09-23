#pragma once

struct ST_YEONDONG
{
	BYTE bt65536Divid; // Pointer�� ���� Address / 0x10000
	BYTE bt256DMod;
	BYTE bt256Mod;
};

struct ST_MAINROM
{
	ST_MAINROM() {
		memset(this, 0x00, sizeof(*this));
	}
	// Index ��� : (unit * 102 + channel * 256 + ȸ��) * 3
	ST_YEONDONG 	stLinkPointer[63 * 4 * 256]; 	// 256ȸ�� * 3Byte * 4���� * 63����
													// = 193,536Byte(0x02F400) 
	ST_YEONDONG 	stPSPointer[256];				// �з½���ġ
	ST_YEONDONG 	stPumpPointer[256];				// ����
	ST_YEONDONG 	stPatternPointer[512];			// ����
};

struct ST_CIRCUIT
{
	ST_CIRCUIT() {
		memset(this, 0x00, sizeof(*this));
	}
	BYTE count;
	BYTE inputType;
	BYTE outputType;
	ST_YEONDONG msgAddr;
};

#define		ONE_MEGA_SIZE		(1024*1024)

class CDataLinkManager : public TSingleton<CDataLinkManager>
{
public:
	CDataLinkManager();
	~CDataLinkManager();

public:
	bool MakeDataLinkTotal();
	void SetDirPath(CString sPath);

	bool MakeRedboxOPData(CWnd* pWnd);
	bool MakeCRTData();
	bool MakeFloor();

	int MakeExcelData(CString sPath);

	void ReleaseExcelWaitPopup();

protected:
	CString GetCircuitNo(int nNum);
	int  fileWrite(FILE* f, int dwSize, int nElement, BYTE* pBuf);
	bool MakeEmerData(FILE* f);
	bool LcdContentsWrite(SYSTEM_INFO_* pInfo, FILE* f, long & lSize);
	bool LcdContentsWriteCommon(FILE* f, CString sContent, long & lSize);

	bool WriteCircuitInfo(FILE* OPf, FILE* lcdf, long & lMainPt, long & lLcdPt);
	bool WritePsInfo(FILE* OPf, FILE* lcdf, long & lMainPt, long & lLcdPt);
	bool WritePumpInfo(FILE* OPf, FILE* lcdf, long & lMainPt, long & lLcdPt);
	bool WritePatternInfo(FILE* OPf, FILE* lcdf, long & lMainPt, long & lLcdPt);

private:
	void MakeDataLink(SYSTEM_INFO_* pInfo);

private:
	CString m_sPath;
	BYTE* m_bLcdW;
	int m_nLcdLen;
};
