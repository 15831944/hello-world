// MatrialItemeEdit_MatrialDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MatrialItemeEdit_MatrialDlg.h"
#include "afxdialogex.h"


// MatrialItemeEdit_MatrialDlg �Ի���

IMPLEMENT_DYNAMIC(MatrialItemeEdit_MatrialDlg, CDialogEx)

MatrialItemeEdit_MatrialDlg::MatrialItemeEdit_MatrialDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MatrialItemeEdit_MatrialDlg::IDD, pParent)
	, m_MatrialNum(_T(""))
	, m_MatrialName(_T(""))
{

}

MatrialItemeEdit_MatrialDlg::~MatrialItemeEdit_MatrialDlg()
{
}

void MatrialItemeEdit_MatrialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SELECT_NUM, m_MatrialNum);
	DDX_CBString(pDX, IDC_EDIT_MATRIAL, m_MatrialName);
	DDX_Control(pDX, IDC_EDIT_MATRIAL, m_MatrialNameBox);
}


BEGIN_MESSAGE_MAP(MatrialItemeEdit_MatrialDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MatrialItemeEdit_MatrialDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// MatrialItemeEdit_MatrialDlg ��Ϣ�������

//��ʾ��ǰ�����б�
BOOL MatrialItemeEdit_MatrialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i < m_MatrialNameList.size() ; i++)
	{
		m_MatrialNameBox.InsertString(i,m_MatrialNameList[i].c_str());
	}
	m_MatrialName = m_MatrialNameList[0].c_str();
	m_MatrialNameBox.SetCurSel(0);

	m_MatrialNum.Format("�ֹ����� �� ��%d��������ѡ��", m_MatrialNameList.size());

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//��Ч��ǰ��������
void MatrialItemeEdit_MatrialDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialogEx::OnOK();
}

//׷�ӵ���ѡ���������б�
void MatrialItemeEdit_MatrialDlg::AppendMatrialName( std::string name )
{
	for (int i = 0; i < m_MatrialNameList.size() ; i++)
	{
		if (m_MatrialNameList[i].compare(name) == 0)
		{
			return;
		}
	}
	m_MatrialNameList.push_back(name);
}
