// http://wiredlayer.tistory.com/20�� ���� �ҽ��� �����ϰ� Ŭ������ �����س��⸸ �� �����Դϴ�^^
#pragma once

#include <Windows.h>
#include <string>

LONG WINAPI my_top_level_filter(__in PEXCEPTION_POINTERS pExceptionPointer);
class MinidumpHelp
{
private:
	CString m_strModulePath;	
public:
	CString get_dump_filename();
	MinidumpHelp(void);
	~MinidumpHelp(void);
	
	void install_self_mini_dump();

};
