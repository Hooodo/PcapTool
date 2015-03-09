
// PcapToolsDlg.h : 头文件
//

#pragma once


// CPcapToolsDlg 对话框
class CPcapToolsDlg : public CDialogEx
{
// 构造
public:
	CPcapToolsDlg(CWnd* pParent = NULL);	// 标准构造函数

	void DataToPcap();
	void PcapToData();
	CString OutputFileName(int type);

	CString m_strFilePathName;
	CString m_strFileName;

// 对话框数据
	enum { IDD = IDD_PCAPTOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
