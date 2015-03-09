
// PcapToolsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PcapTools.h"
#include "PcapToolsDlg.h"
#include "afxdialogex.h"
#include "PcapCore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPcapToolsDlg �Ի���




CPcapToolsDlg::CPcapToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPcapToolsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPcapToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_BROWS, m_btn_brows);
	DDX_Control(pDX, IDC_BUTTON_PROCESS, m_btn_process);
	DDX_Control(pDX, IDC_CHECK, m_check);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cmb_type);
	DDX_Control(pDX, IDC_EDIT_FILE, m_edt_file);
	DDX_Control(pDX, IDC_EDIT_LIMIT, m_edt_limit);
	DDX_Control(pDX, IDC_STATIC_COUNT, m_stc_count);
}

BEGIN_MESSAGE_MAP(CPcapToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWS, &CPcapToolsDlg::OnClickedButtonBrows)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CHECK, &CPcapToolsDlg::OnClickedCheck)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CPcapToolsDlg::OnClickedButtonProcess)
END_MESSAGE_MAP()


// CPcapToolsDlg ��Ϣ�������

BOOL CPcapToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CenterWindow();
	
	m_cmb_type.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPcapToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPcapToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPcapToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPcapToolsDlg::OnClickedButtonBrows()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Pcap Files (*.pcap)|*.pcap|All Files|*.*||",
		NULL);
	if(dlg.DoModal() == IDOK)
	{
		m_strFilePathName = dlg.GetPathName();
		m_edt_file.SetWindowText(m_strFilePathName);
	}
}


void CPcapToolsDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	char filePath[MAX_PATH];
	int count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (count)
	{
		DragQueryFile(hDropInfo, 0, filePath, MAX_PATH);
		m_edt_file.SetWindowText(filePath);
		m_strFilePathName = CString(filePath);
	}
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}


void CPcapToolsDlg::OnClickedCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_check.GetCheck())
	{
		if (m_cmb_type.GetCurSel() == 1)
		{
			MessageBox("This type don't support set packet limit", 0, MB_ICONINFORMATION);
			m_check.SetCheck(0);
		}
		else
			m_edt_limit.EnableWindow();
	}
	else
	{
		m_edt_limit.EnableWindow(FALSE);
	}
}


void CPcapToolsDlg::OnClickedButtonProcess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int cursel = m_cmb_type.GetCurSel();
	CString str;
	m_edt_file.GetWindowText(str);
	if (str.IsEmpty())
	{
		MessageBox("Please select or drag a file to process!", 0, MB_ICONINFORMATION);
		return;
	}
	packet_count = 0;
	OutputFileName(m_cmb_type.GetCurSel());
	if (cursel == 0)
	{
		DataToPcap();
	}
	else if (cursel == 1)
	{
		PcapToData();
	}
	else
	{}
	str.Format("Process count : %d", packet_count);
	m_stc_count.SetWindowText(str);
}

void CPcapToolsDlg::PcapToData()
{
	pcap_t *pt;
	char errbuf[PCAP_ERRBUF_SIZE];

	if ((pt = pcap_open_offline((LPSTR)(LPCTSTR)m_strFilePathName, errbuf)) == NULL)
	{
		MessageBox(errbuf, 0, MB_ICONWARNING);
		return;
	}
	fp_2 = fopen((LPSTR)(LPCTSTR)m_strFileName, "wb+");
	if (fp_2 == NULL)
	{
		MessageBox("Can't open the file!", 0, MB_ICONWARNING);
		return;
	}

	if (pcap_loop(pt, -1, packet_handler, NULL)==-1)
	{
		MessageBox(pcap_geterr(pt), 0, MB_ICONWARNING);
	}
	fclose(fp_2);
}

void CPcapToolsDlg::DataToPcap()
{
	pcap_write_init();

	FILE *fp2;
	unsigned char flag;
	ip_hdr hdr;
	unsigned short hdrlen;
	unsigned char buf2[MAX_LEN];
	int bytesread;
	int count, maxpacket;
	int offset;
	CString str;
	bool b_check = false;

	if(m_check.GetCheck())
	{
		m_edt_limit.GetWindowText(str);
		if (str.IsEmpty())
		{
			MessageBox("Please input a number or uncheck the limit control!", 0, MB_ICONINFORMATION);
			return;
		}
		maxpacket = _ttoi(str);
		if(maxpacket>0)
			b_check = true;
	}
	else
		b_check = false;

	fp2 = fopen((LPSTR)(LPCTSTR)m_strFilePathName, "rb+");
	if(fp2 == NULL)
	{
		MessageBox("Can't open the file!", 0, MB_ICONWARNING);
		return;
	}
	fread(&flag, 1, 1, fp2);
	if(flag != 0x45)
	{
		MessageBox("This is not a correct ip data!", 0, MB_ICONSTOP);
		return;
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
}

CString CPcapToolsDlg::OutputFileName(int type)
{
	CString strName;
	strName = m_strFilePathName.Right(m_strFilePathName.GetLength() - m_strFilePathName.ReverseFind('\\') - 1);
	if (strName.ReverseFind('.') >= 0)
	{
		m_strFileName = strName.Left(strName.ReverseFind('.'));
	}
	else
		m_strFileName = strName;

	if (type == 0)
	{
		m_strFileName += ".pcap";
	}
	else
	{
		m_strFileName += ".dat";
	}

    return m_strFileName;
}

BOOL CPcapToolsDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN)
	{
		int key = (int)pMsg->wParam;
		if (key == VK_RETURN)
		{
			OnClickedButtonProcess();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
