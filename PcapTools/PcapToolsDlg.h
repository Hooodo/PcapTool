
// PcapToolsDlg.h : ͷ�ļ�
//

#pragma once


// CPcapToolsDlg �Ի���
class CPcapToolsDlg : public CDialogEx
{
// ����
public:
	CPcapToolsDlg(CWnd* pParent = NULL);	// ��׼���캯��

	void DataToPcap();
	void PcapToData();
	CString OutputFileName(int type);

	CString m_strFilePathName;
	CString m_strFileName;

// �Ի�������
	enum { IDD = IDD_PCAPTOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_btn_brows;
	CButton m_btn_process;
	CButton m_check;
	CComboBox m_cmb_type;
	CEdit m_edt_file;
	CEdit m_edt_limit;
	CStatic m_stc_count;
	afx_msg void OnClickedButtonBrows();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnClickedCheck();
	afx_msg void OnClickedButtonProcess();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
