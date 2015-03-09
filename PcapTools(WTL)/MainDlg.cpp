// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include "PcapCore.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	m_edt_file.Attach(GetDlgItem(IDC_EDIT_FILE));
	m_edt_limit.Attach(GetDlgItem(IDC_EDIT_LIMIT));
	m_stc_count.Attach(GetDlgItem(IDC_STATIC_COUNT));
	m_check.Attach(GetDlgItem(IDC_CHECK));
	m_cmb_type.Attach(GetDlgItem(IDC_COMBO_TYPE));
	m_btn_process.Attach(GetDlgItem(IDC_BUTTON_PROCESS));

	m_cmb_type.AddString("data to pcap");
	m_cmb_type.AddString("pcap to data");
	m_cmb_type.SetCurSel(0);

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDROP *hDropInfo = (HDROP *)wParam;
	char filePath[MAX_PATH];
	int count = DragQueryFile(*hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (count)
	{
		DragQueryFile(*hDropInfo, 0, filePath, MAX_PATH);
		m_edt_file.SetWindowText(filePath);
		m_strPathname = CString(filePath);
	}
	DragFinish(*hDropInfo);
	return 0;
}

LRESULT CMainDlg::OnCheck(WORD , WORD wID, HWND , BOOL& )
{
	if (m_check.GetCheck())
		m_edt_limit.EnableWindow();
	else
		m_edt_limit.EnableWindow(FALSE);
	
	return 0;
}

LRESULT CMainDlg::OnBrowse(WORD , WORD wID, HWND , BOOL& )
{
	CFileDialog dlg(TRUE,
		"Pcap",
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Pcap Files (*.pcap)|*.pcap|All Files|*.*||",
		m_hWnd);
	if(dlg.DoModal() == IDOK)
	{	
		m_strPathname = CString(dlg.m_szFileName);
		m_edt_file.SetWindowText(m_strPathname);
	}
	return 0;
}

LRESULT CMainDlg::OnProcess(WORD , WORD wID, HWND , BOOL& )
{
	DWORD id;
	int cursel = m_cmb_type.GetCurSel();
	char szFileName[MAX_PATH];
	
	m_edt_file.GetWindowTextA(szFileName, MAX_PATH);
	if (strlen(szFileName) <= 0)
	{
		MessageBox("Please select or drag a file to process!", 0, MB_ICONINFORMATION);
		return 0;
	}
	packet_count = 0;
	OutputFileName(m_cmb_type.GetCurSel());
	m_btn_process.EnableWindow(FALSE);
	if (cursel == 0)
	{
		CreateThread(NULL, 0, DataToPcap, (LPVOID)this, 0, &id);
	}
	else if (cursel == 1)
	{
		CreateThread(NULL, 0, PcapToData, (LPVOID)this, 0, &id);
	}
	else
	{}
	
	return 0;
}

DWORD WINAPI CMainDlg::DataToPcap(LPVOID lparam)
{
	CMainDlg *pDlg = (CMainDlg *)lparam;
	pcap_write_init();

	FILE *fp2;
	unsigned char flag;
	ip_hdr hdr;
	unsigned short hdrlen;
	unsigned char buf2[MAX_LEN];
	char szCount[MAX_PATH];
	int bytesread;
	int count, maxpacket;
	int offset;
	CString str;
	bool b_check = false;
	char szlimit[100];

	if(pDlg->m_check.GetCheck())
	{
		pDlg->m_edt_limit.GetWindowTextA(szlimit, 100);
		if (str.IsEmpty())
		{
			pDlg->MessageBox("Please input a number or uncheck the limit control!", 0, MB_ICONINFORMATION);
			return 0;
		}
		maxpacket = _ttoi(szlimit);
		if(maxpacket>0)
			b_check = true;
	}
	else
		b_check = false;

	fp2 = fopen((LPSTR)(LPCTSTR)pDlg->m_strPathname, "rb+");
	if(fp2 == NULL)
	{
		pDlg->MessageBox("Can't open the file!", 0, MB_ICONWARNING);
		return 0;
	}
	fread(&flag, 1, 1, fp2);
	if((flag>>4) != 0x04)
	{
		pDlg->MessageBox("This is not a correct ip data!", 0, MB_ICONSTOP);
		return 0;
	}
	fseek(fp2, 0, SEEK_SET);
	memset(buf2, 0, MAX_LEN);

	hdrlen = 0;
	offset = 0;
	count = 0;
	bytesread = fread(&hdr, 1, sizeof(hdr), fp2);
	offset = ftell(fp2);

	while((bytesread == sizeof(hdr)))
	{
		if (b_check)
		{
			if (count>=maxpacket)
			{
				break;
			}
		}
		hdrlen = ntohs(hdr.tot_len);
		bytesread = fread(buf2+ETHERNET_LEN+sizeof(hdr), 1, hdrlen-20, fp2);
		if(bytesread < hdrlen-20 || bytesread > MAX_LEN)
		{
			fseek(fp2, offset, SEEK_SET);
			find_ip_head(fp2, offset);
			bytesread = fread(&hdr, 1, sizeof(hdr), fp2);
			continue;
		}
		memcpy(buf2+ETHERNET_LEN, &hdr, sizeof(hdr));
		buf2[12] = 0x08;

		pcap_write_body(buf2, hdrlen);
		bytesread = fread(&hdr, 1, sizeof(hdr), fp2);
		offset = ftell(fp2);
		count++;
	}
	packet_count = count;
	pcap_write_end();
	
	sprintf_s(szCount, "Process count : %d", packet_count);
	pDlg->m_stc_count.SetWindowTextA(szCount);
	pDlg->m_btn_process.EnableWindow();
	fclose(fp2);
	return 0;
}

DWORD WINAPI CMainDlg::PcapToData(LPVOID lparam)
{
	CMainDlg *pDlg = (CMainDlg *)lparam;
	pcap_t *pt;
	char errbuf[PCAP_ERRBUF_SIZE];
	char szCount[MAX_PATH];

	if ((pt = pcap_open_offline((LPSTR)(LPCTSTR)pDlg->m_strPathname, errbuf)) == NULL)
	{
		pDlg->MessageBox(errbuf, 0, MB_ICONWARNING);
		return 0;
	}
	fp_2 = fopen((LPSTR)(LPCTSTR)pDlg->m_strFilename, "wb+");
	if (fp_2 == NULL)
	{
		pDlg->MessageBox("Can't open the file!", 0, MB_ICONWARNING);
		return 0;
	}

	if (pcap_loop(pt, -1, packet_handler, NULL)==-1)
	{
		pDlg->MessageBox(pcap_geterr(pt), 0, MB_ICONWARNING);
	}
	fclose(fp_2);
	sprintf_s(szCount, "Process count : %d", packet_count);
	pDlg->m_stc_count.SetWindowTextA(szCount);
	pDlg->m_btn_process.EnableWindow();
	return 0;
}

CString CMainDlg::OutputFileName(int type)
{
	CString strName;
	strName = m_strPathname.Right(m_strPathname.GetLength() - m_strPathname.ReverseFind('\\') - 1);
	if (strName.ReverseFind('.') >= 0)
	{
		m_strFilename = strName.Left(strName.ReverseFind('.'));
	}
	else
		m_strFilename = strName;

	if (type == 0)
	{
		m_strFilename += ".pcap";
	}
	else
	{
		m_strFilename += ".dat";
	}

	return m_strFilename;
}