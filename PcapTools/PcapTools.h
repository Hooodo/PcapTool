
// PcapTools.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPcapToolsApp:
// �йش����ʵ�֣������ PcapTools.cpp
//

class CPcapToolsApp : public CWinApp
{
public:
	CPcapToolsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPcapToolsApp theApp;