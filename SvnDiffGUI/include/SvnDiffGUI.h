
// SvnDiffGUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSvnDiffGUIApp: 
// �йش����ʵ�֣������ SvnDiffGUI.cpp
//

class CSvnDiffGUIApp : public CWinApp
{
public:
	CSvnDiffGUIApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSvnDiffGUIApp theApp;