//******************************************************************/
//******************************************************************/
//! All Rights Reserved. Copyright(c)   2015 (��)�ѱ��Ҹ���ġ                /
//******************************************************************/
//! File Name     : RealAddDlg.cpp
//! Function      : ������ ���̾�α�
//! System Name   : Ű�� ����API �׽�Ʈ
//! Create        : , 2015/04/01
//! Update        : 
//! Comment       : 
//******************************************************************/
// RealAddDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "KhOpenApiTest.h"
#include "RealAddDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FIDLIST	_T("9001;302;10;11;25;12;13")

// {��ȸ Ű,		���� Ű,	��, ��, Ÿ��,			�� ����, ����, �� ����, �� ����}
const stGRID lstOPTSBFID[] = 
{
	{_T("�����ڵ�"),		_T("9001"),	-1,	0,	DT_NONE			, FALSE	, DT_CENTER	,	_T(""),	_T("")}, 
	{_T("�����"),		_T("302"),	-1,	1,	DT_NONE			, FALSE	, DT_LEFT	,	_T(""),	_T("")}, 
	{_T("���簡"),		_T("10"),	-1,	2,	DT_ZERO_NUMBER	, TRUE	, DT_RIGHT	,	_T(""),	_T("")}, 
	{_T("���ϴ���ȣ"),	_T("25"),	-1,	3,	DT_SIGN			, TRUE	, DT_CENTER	,	_T(""),	_T("")}, 
	{_T("���ϴ��"),		_T("11"),	-1,	4,	DT_ZERO_NUMBER	, TRUE	, DT_RIGHT	,	_T(""),	_T("")}, 
	{_T("�����"),		_T("12"),	-1,	5,	DT_ZERO_NUMBER	, TRUE	, DT_RIGHT	,	_T(""),	_T("")}, 
	{_T("�ŷ���"),		_T("13"),	-1,	6,	DT_ZERO_NUMBER	, FALSE	, DT_RIGHT	,	_T(""),	_T("")}, 
};

enum eExchangeTypeMode
{
	eExchangeType_Remove = 0	// �ŷ��� ���а� ������ �����ڵ�
	, eExchangeType_Add			// �ŷ��� ���а� �߰��� �����ڵ�
	, eExchangeType_Cnt
};

CRealAddDlg::CRealAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRealAddDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_strScrNo = _T("9999");
	m_cellSelect.row = 0;
	m_cellSelect.col = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strPreConditionName = _T("");
	m_strConditionName = _T("");
	m_nPreConditionIndex = -1;	
	m_nConditionIndex = -1;
}

void CRealAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealAddDlg)
	DDX_Control(pDX, IDC_GRD_REALADD,		m_grdRealAdd);
	DDX_Control(pDX, IDC_CMB_CON,			m_cmbCon);
	DDX_Control(pDX, IDC_LIST_INSERT,		m_listCtl_Insert);
	DDX_Control(pDX, IDC_LIST_DELETE,		m_listCtl_Delete);
	DDX_Control(pDX, IDC_CMB_EXCH,			m_cbExchan);
	//}}AFX_DATA_MAP
}

//*******************************************************************/
// BEGIN_MESSAGE_MAP
//*******************************************************************/
BEGIN_MESSAGE_MAP(CRealAddDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_REAL_REG,		&CRealAddDlg::OnBnRealReg)
	ON_BN_CLICKED(IDC_BTN_REAL_REMOVE,	&CRealAddDlg::OnBnRealRemove)
	ON_BN_CLICKED(IDC_BTN_REAL_REMOVE_ALL,	&CRealAddDlg::OnBnRealRemoveAll)
	ON_BN_CLICKED(IDC_BTN_CONDITION_LOAD, &CRealAddDlg::OnBnConditionLoad)
	ON_BN_CLICKED(IDC_BTN_COND_SEARCH, &CRealAddDlg::OnBnConditionSearch)
	ON_BN_CLICKED(IDC_BTN_COND_SEARCH_REAL, &CRealAddDlg::OnBnSearchReal)
	ON_BN_CLICKED(IDC_BTN_COND_SEARCH_REAL_STOP, &CRealAddDlg::OnBnSearchRealStop)
	ON_CBN_SELCHANGE(IDC_CMB_EXCH, &CRealAddDlg::OnSelChExchange)
END_MESSAGE_MAP()

BOOL CRealAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	InitRealAddGrid();				// �׸��� �ʱ� ó��

	m_cbExchan.ResetContent();
	m_cbExchan.AddString(_T("����"));
	m_cbExchan.AddString(_T("KRX"));
	m_cbExchan.AddString(_T("NXT"));	
	m_cbExchan.SetCurSel(0);

	m_strExchageCode = _T("_AL");// �ŷ��� ���а� ����:_AL, KRX:����, NXT:_NX, ex) 039490_AL, 039490, 039490_NX
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CRealAddDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CRealAddDlg::OnClose()
{
	// ȭ�� ���� �� �θ������쿡�� �뺸(ȭ�� ���� ����)
	if (m_pParent)
	{
		//�ü� �ǽð� ����
		theApp.m_khOpenApi.SetRealRemove(m_strScrNo, _T("ALL"));

		if (m_strConditionName != _T("") && m_nConditionIndex > -1)
		{
			theApp.m_khOpenApi.SendConditionStop(m_strScrNo, GetConditionName(m_strConditionName), m_nConditionIndex);
		}
		int nLen = m_strScrNo.GetLength();
		char *cScrNo = new char[nLen + 1];
		memset(cScrNo, 0x00, nLen + 1);
		memcpy(cScrNo, m_strScrNo, nLen);
		m_pParent->PostMessage(UM_SCRENN_CLOSE, 0U, (LPARAM)cScrNo);
	}

	CDialogEx::OnClose();
}

//******************************************************************/
HCURSOR CRealAddDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRealAddDlg::OnBnRealReg()
{//�ǽð� �ü� ��� �Լ� ȣ��
	//���� ���� ���簡, ���, �����, �ŷ���� ���� �ǽð��� �ޱ� ���� �ݵ�� ȣ���ؾ���.	
	CString strCodeList;
	// �ü�ó��(��ȸ, �ǽð� �ü�)�� �ŷ��� ���а� ��ȯ�ʿ�, ����:_AL, KRX:����, NXT:_AL
	int nCount = m_grdRealAdd.GetRowCount();
	for (int i = 1; i < nCount; i++)
	{
		strCodeList += m_grdRealAdd.GetItemText(i, 0) + m_strExchageCode + _T(";");
	}
	
	if(strCodeList.IsEmpty())
		return;

	long lRet = theApp.m_khOpenApi.SetRealReg(m_strScrNo, strCodeList, FIDLIST, _T("1"));
}

void CRealAddDlg::OnBnRealRemove()
{// �ǽð� �ü� ����.
	theApp.m_khOpenApi.SetRealRemove(m_strScrNo, _T("ALL"));
}

// �׸��� �ʱ� ó��
void CRealAddDlg::InitRealAddGrid()
{
	m_grdRealAdd.SetEditable(false);				//cell�� ����Ʈ ���ϰ� ��.
	m_grdRealAdd.EnableScrollBars(SB_BOTH, FALSE);

	COLORREF clr = RGB(215, 227, 241);

	// ���� ��/�� ����
	m_grdRealAdd.SetFixedRowCount(1);

	// ��/�� ���� ����
	m_grdRealAdd.SetRowCount(1);
	m_grdRealAdd.SetColumnCount(8);

	// ���� ���� ����
	int i, nWidth[] = {60, 130, 80, 20, 80, 70, 100, 15};
	CString strHeader[] = {_T("�����ڵ�"), _T("�����"), _T("���簡"), _T(""), _T("���ϴ��"), _T("�����"), _T("�ŷ���"), _T("")};
	int nCnt = sizeof(nWidth) / sizeof(*nWidth);		// ��üũ�� / ����ũ�� = ���Ұ���
	for (i = 0; i < nCnt; i++)
	{
		m_grdRealAdd.SetColumnWidth(i, nWidth[i]);
		m_grdRealAdd.SetItemFormat(0, i, DT_CENTER);
		m_grdRealAdd.SetItemText(0, i, strHeader[i]);
		m_grdRealAdd.SetItemBkColour(0, i, clr);	// ������ ���� ���� ����
	}

	// ���� ���� ����
	m_grdRealAdd.SetRowHeight(0, 24);

	m_grdRealAdd.Invalidate();
}

void CRealAddDlg::OnReceiveTrDataKhopenapictrl(LPCTSTR sScrNo, LPCTSTR sRQName, LPCTSTR sTrcode, LPCTSTR sRecordName, LPCTSTR sPrevNext, long nDataLength, LPCTSTR sErrorCode, LPCTSTR sMessage, LPCTSTR sSplmMsg)
{// �ü���ȸ ���� �̺�Ʈ
	return;
}

void CRealAddDlg::OnReceiveMsgKhopenapictrl(LPCTSTR sScrNo, LPCTSTR sRQName, LPCTSTR sTrCode, LPCTSTR sMsg)
{// �޽��� ���� �̺�Ʈ
}

void CRealAddDlg::OnReceiveRealDataKhopenapictrl(LPCTSTR sJongmokCode, LPCTSTR sRealType, LPCTSTR sRealData)
{// �ǽð� �ü� ������ ����ó��
	CString strIndex, strJongCode(sJongmokCode);
	// ���ǰ˻��� �����ڵ�� ������� 6�ڸ�, �ü�ó��(��ȸ, �ǽð� �ü�)�� �ŷ��� ���а� ��ȯ�ʿ�, ����:_AL, KRX:����, NXT:_AL
	CString strCovertCode = GetConvertSymbolCode(strJongCode, eExchangeType_Remove);
	if (!m_mapJongCode.Lookup(strCovertCode, strIndex))
	{
		return;
	}

	CString strData;
	CStringArray arrData;
	CString strExchangeCode(sJongmokCode);// �ü�ó��(��ȸ, �ǽð� �ü�)�� �ŷ��� ���а� ��ȯ�ʿ�, ����:_AL, KRX:����, NXT:_AL
	arrData.Add(strExchangeCode);

	int i, nFieldCnt = sizeof(lstOPTSBFID) / sizeof(*lstOPTSBFID);		// ��üũ�� / ����ũ�� = ���Ұ���
	for (i = 1; i < nFieldCnt; i++)
	{
		if (_ttoi(lstOPTSBFID[i].strFID) < 0)
		{
			arrData.Add("");
			continue;
		}
		strData = theApp.m_khOpenApi.GetCommRealData(strExchangeCode, _ttoi(lstOPTSBFID[i].strFID));	strData.Trim();
		arrData.Add(strData);
	}
	SetDataRealAddGrid(arrData, sRealType);
}

void CRealAddDlg::OnReceiveRealCondition(LPCTSTR sCode, LPCTSTR sType, LPCTSTR sConditionName, LPCTSTR sConditionIndex)
{// �ǽð� ���ǰ˻� ��� ���� (���� ����/��Ż)
	CString		strConditionName(sConditionName);
	if(m_strConditionName != strConditionName)	//���� ���õ� ���Ǹ�� �ǽð����� ���� ���Ǹ��� ������ ��.
	{
		TRACE(_T("Err ConditionName %s %s=%s:%s"), m_strConditionName, strConditionName, sCode, sType);
		return;
	}

	CString strName, strMsg, strIndex, strType(sType), strCode(sCode);
	m_cmbCon.GetLBText(m_cmbCon.GetCurSel(), strName);	//���� ���õ� ���Ǹ�
	
	CString strCodeName;
	strCodeName = theApp.m_khOpenApi.GetMasterCodeName(strCode);

	const int nShowRow = 7;
	if (strType == _T("I"))	// ��������
	{
		int nRet = m_grdRealAdd.InsertRow(strCode, 1);
		if(nRet < 1)
		{// ������ ��� ��Ż���� ���� �����߰��Ҷ�
			auto nRowCnt = m_grdRealAdd.GetRowCount();
			m_grdRealAdd.SetRowCount(nRowCnt + 1);
			m_grdRealAdd.SetItemText(1, 0, strCode);
		}
		m_grdRealAdd.SetItemText(1, 1, strCodeName);

		//�ϴ� �޼��� ���̱�
		if (m_listCtl_Insert.GetCount() > nShowRow)
		{
			m_listCtl_Insert.ResetContent();
		}

		strMsg.Format(_T("%s : %s"), strCode, strCodeName);
		m_listCtl_Insert.InsertString(0, strMsg);

		//������ȸ�� ������ �ü��� ������ �ִ� ��Ȳ���� �߰��� ���Ե� ������ �ü��� �����ٶ� 
		// ������ �Ķ���Ϳ� "1"�� �־��־�� ���� ����� �Բ� ���Ե� ������ �ü��� ���´�.
		// ���� "1" �̿��� "0"�� �־��ָ� ������ ����� ����鿡 ���� �ü��� ��������.
		//�ǽð� �ü� ��� �Լ� ȣ��
		strCode = GetConvertSymbolCode(strCode, eExchangeType_Add);
		long lRet = theApp.m_khOpenApi.SetRealReg(m_strScrNo, strCode, FIDLIST, _T("1"));
	} 
	else if (strType == _T("D"))	// ������Ż
	{
		//������ �ִ� �������� Ȯ��.
		if(m_mapJongCode.Lookup(strCode, strIndex) == TRUE)
		{
			//��Ż ���� ����.
			int nCurRow = m_grdRealAdd.GetRowCount();
			if(nCurRow > 1)
				m_grdRealAdd.DeleteRow(_ttoi(strIndex));

			//��Ż�� ������ �ü��� ���� �������� ����Ѵ�.
			theApp.m_khOpenApi.SetRealRemove(m_strScrNo, strCode);

			//�ϴ� �޼��� ���̱�
			if (m_listCtl_Delete.GetCount() > nShowRow)
			{
				m_listCtl_Delete.ResetContent();
			}

			strMsg.Format(_T("%s : %s"), strCode, strCodeName);
			m_listCtl_Delete.InsertString(0, strMsg);
		}
		else
		{
			TRACE(_T("������Ż ���ܻ�Ȳ %s\n"), strCode);
			int nIdx = FindCodeRowIndex(strCode);
			if(nIdx != -1)
			{
				m_grdRealAdd.DeleteRow(nIdx);
				theApp.m_khOpenApi.SetRealRemove(m_strScrNo, strCode);

				//�ϴ� �޼��� ���̱�
				if (m_listCtl_Delete.GetCount() > nShowRow)
					m_listCtl_Delete.ResetContent();

				strMsg.Format(_T("*** %s : %s"), strCode, strCodeName);
				m_listCtl_Delete.InsertString(0, strMsg);
			}
		}
	}

	m_grdRealAdd.Invalidate();
 	m_mapJongCode.RemoveAll();

	int nCount = m_grdRealAdd.GetRowCount();
	for (int i = 1; i < nCount; i++)
	{
		strCode = m_grdRealAdd.GetItemText(i, 0);
		strIndex.Format(_T("%d"), i);
		m_mapJongCode.SetAt(strCode, strIndex);
	}
}

//*******************************************************************/
//! Function Name	: OnReceiveTrCondition
//! Function		: ���ǰ˻� ���񸮽�Ʈ ��ȸ����
//! Param			: LPCTSTR sScrNo					- ȭ���ȣ
//!					: PCTSTR strCodeList				- ���񸮽�Ʈ
//!					: LPCTSTR strConditionName			- ���Ǹ�
//!					: int nIndex						- ���ǽ� ������ȣ
//!					: int nNext							- ������ȸ(2:������ȸ, 0:������ȸ����)
//! Return        : void
//! Create        : 2015/04/01
//! Comment       : 
//******************************************************************/
void CRealAddDlg::OnReceiveTrCondition(LPCTSTR sScrNo,PCTSTR strCodeList, LPCTSTR sConditionName, int nIndex, int nNext)
{// ���ǰ˻� ��û ��� ����
	CString strCndName, strRecvCondName(sConditionName);
	m_cmbCon.GetLBText(m_cmbCon.GetCurSel(), strCndName);	//���õ� ���Ǹ�

	/// ���ǽ� �̸����� �ŷ��� �̸��� ���Ѵ�.
	CString strRecvExchan = theApp.m_khOpenApi.KOA_Functions(_T("GetExchangeName"), strRecvCondName);
	if(strRecvExchan.IsEmpty() == FALSE)
	{/// �ŷ��Ұ� �´��� ��
		CString strExchage, strCondName;
		m_cbExchan.GetLBText(m_cbExchan.GetCurSel(), strExchage);
		m_cmbCon.GetLBText(m_cmbCon.GetCurSel(), strCondName);
		TRACE("%s:%s = %s:%s:%s\n", strExchage, strRecvExchan, strCondName, m_strConditionName, strRecvCondName);
	}

	if(m_strConditionName == strRecvCondName)	// ������ ���Ǹ�� ������ ���Ǹ��� �������� ó��.
	{
		if (strCodeList != _T(""))
		{
			int nIndex(0);
			CString strConditionCode;
			m_mapJongCode.RemoveAll();		// �����ڵ� �ʱ�ȭ
			m_grdRealAdd.SetRowCount(1);

			while ( AfxExtractSubString( strConditionCode, strCodeList, nIndex++, _T(';')) )
			{// �˻��� ������� ���ؼ� �� ����ŭ Row�߰�
			}
		
			m_grdRealAdd.SetRowCount(nIndex-1);

			nIndex = 0;
			CString strIndex, strCodeName;
			int nFieldCnt = sizeof(lstOPTSBFID) / sizeof(*lstOPTSBFID);		// ��üũ�� / ����ũ�� = ���Ұ���
			
			while ( AfxExtractSubString( strConditionCode, strCodeList, nIndex++, _T(';')) )
			{
				if (strConditionCode.IsEmpty())
					continue;
				
				for(auto i = 0; i < nFieldCnt; i++)
				{
					m_grdRealAdd.SetItemFormat(nIndex, lstOPTSBFID[i].nCol, lstOPTSBFID[i].nAlign);
				}

				// �����ڵ�
				m_grdRealAdd.SetItemText(nIndex, 0, strConditionCode);

				// �����
				strCodeName = theApp.m_khOpenApi.GetMasterCodeName(strConditionCode).Trim();
				m_grdRealAdd.SetItemText(nIndex, 1, strCodeName);

				strIndex.Format("%d", nIndex);
				m_mapJongCode.SetAt(strConditionCode, strIndex);			
			}
		}
	}
}

void CRealAddDlg::OnReceiveConditionVer(long lRet, LPCTSTR sMsg)
{// ����� ���ǽ� �����ϰ� ������ȣ�� ���ǽ� �̸����� �Ľ��� ����	
	if (lRet != 1)
	{
		CString		strMsg(sMsg);
		CString		strErr;
		strErr.Format(_T("����� �����б� ���� \r\n\r\n[%s]"), strMsg);
		AfxMessageBox(strErr);
		return;
	}

	//���ǰ˻��� ����Ʈ�� �ҷ��´�.
	CString strCondition, strConditionName, strIndex;
	CString strConditionNameList = theApp.m_khOpenApi.GetConditionNameList();

	//�޾ƿ� ���ǰ˻����� �޺��� �ִ´�.
	m_cmbCon.ResetContent();

	int i(0);
	while( AfxExtractSubString( strCondition, strConditionNameList, i++, _T(';')) )
	{
		if(strCondition.IsEmpty())
			continue;

		AfxExtractSubString(strIndex			, strCondition, 0, _T('^'));// ������ȣ
		AfxExtractSubString(strConditionName	, strCondition, 1, _T('^'));// ���ǽ� �̸�
			
		m_mapNameList.SetAt(strCondition, strIndex);
		m_cmbCon.AddString(strCondition);
	}

	m_cmbCon.SetCurSel(0);
}

void CRealAddDlg::SetDataRealAddGrid(CStringArray &arrData, CString strRealType/* = ""*/)
{// �׸��� ����Ÿ ����

	// �ü������� �����ڵ带 ���ǰ˻� �����ڵ�� ��ȯ, �����ڵ忡 �´� �� ã��
	CString strData = arrData.GetAt(0), strTemp;
	CString strCode = GetConvertSymbolCode(strData, eExchangeType_Remove);
	if (m_mapJongCode.Lookup(strCode, strTemp) == FALSE)
	{
		return;
	}

	int i, nRow = _ttol(strTemp), nCnt = arrData.GetSize();

	// ����Ÿ ����
	CString strTempData;
	for (i = 1; i < nCnt; i++)
	{
		strData = arrData.GetAt(i);
		/////////////////////////////////////////////
		// ���� �� ������ ���� ó������������
		if (strRealType != _T("") && (i == 2 || i == 4 || i == 5))
		{
			strTempData = strData;
			strTempData.Replace(_T("+"), _T(""));
			strTempData.Replace(_T("-"), _T(""));	
			strTempData.Replace(_T("."), _T(""));

			strTemp = m_grdRealAdd.GetItemText(nRow, lstOPTSBFID[i].nCol);
			strTemp.Replace(_T("%"), _T(""));
			strTemp.Replace(_T("+"), _T(""));
			strTemp.Replace(_T("-"), _T(""));
			strTemp.Replace(_T(","), _T(""));	
			strTemp.Replace(_T("."), _T(""));
			if (strTempData != strTemp)
			{
				m_cellSelect.col = lstOPTSBFID[i].nCol;
				m_grdRealAdd.SetFocusCell(m_cellSelect);
				m_grdRealAdd.SetSelectedRange(nRow, m_cellSelect.col, nRow, m_cellSelect.col);
				m_grdRealAdd.Invalidate();
			}
		}
		// ���� �� ������ ���� ó������������
		/////////////////////////////////////////////
		if (lstOPTSBFID[i].bTextColor)
		{
			strTemp = arrData.GetAt(3);
			if (strTemp == _T("1") || strTemp == _T("2"))	// ��ȣ�� ���� ���󺯰�
			{
				strTemp = _T("1");
			}
			else if (strTemp == _T("4") || strTemp == _T("5"))	// ��ȣ�� ���� ���󺯰�
			{
				strTemp = _T("-1");
			}
			else
			{
				strTemp = _T("0");
			}
			theApp.SetDataFgColour(&m_grdRealAdd, nRow, lstOPTSBFID[i].nCol, strTemp);
		}
		if (lstOPTSBFID[i].nDataType == DT_SIGN)
		{
			if (strData.IsEmpty())
			{
				continue;
			}
			theApp.SetSignData(&m_grdRealAdd, nRow, lstOPTSBFID[i].nCol, strData);
		}
		else
		{
			if (strData.IsEmpty())
			{
				continue;
			}
			int nCol = lstOPTSBFID[i].nCol;
			m_grdRealAdd.SetItemText(nRow, lstOPTSBFID[i].nCol, theApp.ConvDataFormat(lstOPTSBFID[i].nDataType, strData, lstOPTSBFID[i].strBeforeData, lstOPTSBFID[i].strAfterData));
		}
	}
	m_grdRealAdd.Invalidate();
}

void CRealAddDlg::OnBnRealRemoveAll()
{// ��ȭ�� �ǽð� �ü� ��� ����
	theApp.m_khOpenApi.SetRealRemove(_T("ALL"), _T("ALL"));
}

void CRealAddDlg::OnBnConditionLoad()
{// ������ ����� ���ǽ� ��û
	long lRet = theApp.m_khOpenApi.GetConditionLoad();
	if(lRet > 0)
	{
		m_cmbCon.ResetContent();
		m_mapNameList.RemoveAll();
	}
}

void CRealAddDlg::OnBnConditionSearch()
{// ���ǰ˻� ��û
	m_cmbCon.GetLBText(m_cmbCon.GetCurSel(), m_strPreConditionName);
	CString strIndex;
	if(m_mapNameList.Lookup(m_strPreConditionName, strIndex) == FALSE)
	{/// ������ ���ǽ� ��ã��.
		return;
	}

	m_nConditionIndex		= _ttoi(strIndex);
	m_strConditionName		= GetConditionName(m_strPreConditionName);
	m_nPreConditionIndex	= m_nConditionIndex;

	// ���񸮽�Ʈ�� OnReceiveTrCondition()�� ���Ͽ� ���´�.
	long lRet = theApp.m_khOpenApi.SendCondition(m_strScrNo, m_strConditionName, m_nConditionIndex, 0);
	if (lRet == 0)
	{//����
		return;
	}
}

void CRealAddDlg::OnBnSearchReal()
{// �ǽð� ���� �˻� ��û
	CString strConditionCodeList = _T(""), strIndex;

	//������ ���Ǹ��� �˻��� ���񸮽�Ʈ�� �ҷ��´�.
	m_cmbCon.GetLBText(m_cmbCon.GetCurSel(), m_strPreConditionName);
	if(m_mapNameList.Lookup(m_strPreConditionName, strIndex) == FALSE)
	{/// ������ ���ǽ� ��ã��.
		return;
	}
	
	m_nConditionIndex		= _ttoi(strIndex);
	m_strConditionName		= GetConditionName(m_strPreConditionName);
	m_nPreConditionIndex	= m_nConditionIndex;

	//���˰˻� �ǽð� ��ȸ
	//���񸮽�Ʈ�� OnReceiveTrCondition()�� ���Ͽ� ���´�.
	//�ǽð� ���� �˻��� �ϰ� ������ ������ �Ķ���Ϳ� 1�� �ش�.
	long lRet = theApp.m_khOpenApi.SendCondition(m_strScrNo, m_strConditionName, m_nConditionIndex, 1);
	if (lRet == 0)
	{//����
		return;
	}

	/// ��������, ��Ż �α� ����
	m_listCtl_Insert.ResetContent();
	m_listCtl_Delete.ResetContent();
}

void CRealAddDlg::OnBnSearchRealStop()
{// �ǽð� ���ǰ˻� ����
	m_strPreConditionName = _T("");
 	CString strConditionName, strIndex;
 	m_cmbCon.GetLBText(m_cmbCon.GetCurSel(), strConditionName);
	if(m_mapNameList.Lookup(strConditionName, strIndex) == TRUE)
	{
		int nConditionIndex = _ttoi(strIndex);
		CString strNewCondtionName = GetConditionName(strConditionName);
		theApp.m_khOpenApi.SendConditionStop(m_strScrNo, strNewCondtionName, nConditionIndex);
	}
}

CString GetExchCoditionName(CString strOrgCoditionName, CString strExchage)
{// ���ǰ˻��� �ʿ��� �ŷ��Һ� ���а��� ���Ѵ�. ����:AL^, KRX:����, NXT:NX^
	CString strNewName;
	if(strExchage == _T("KRX"))
		strNewName = strOrgCoditionName;
	else if(strExchage == _T("NXT"))
		strNewName = theApp.m_khOpenApi.KOA_Functions(_T("GetConditionName_NXT"), strOrgCoditionName);
	else if(strExchage == _T("����"))
		strNewName = theApp.m_khOpenApi.KOA_Functions(_T("GetConditionName_ALL"), strOrgCoditionName);
	else
		strNewName = strOrgCoditionName;
	
	/*
	if(strExchage == _T("KRX"))
		strNewName = strOrgCoditionName;
	else if(strExchage == _T("NXT"))
		strNewName = _T("NX^") + strOrgCoditionName;
	else if(strExchage == _T("����"))
		strNewName = _T("AL^") + strOrgCoditionName;
	else
		strNewName = strOrgCoditionName;
	*/
	return strNewName;

}

//"0^���ǽ�" ��Ʈ������ ���� ���ǰ˻��̸��� ���ϱ� �����Լ�.
CString CRealAddDlg::GetConditionName(CString strTotCondition)
{
	CString strConditionName;

	if (strTotCondition.Find(_T('^')) >= 0)
	{
		AfxExtractSubString(strConditionName	, strTotCondition, 1, _T('^'));
		CString strExchage;
		m_cbExchan.GetLBText(m_cbExchan.GetCurSel(), strExchage);

		CString strNewCodName = GetExchCoditionName(strConditionName, strExchage);
		strConditionName = strNewCodName;
	}
	else
		strConditionName = strTotCondition;
	return strConditionName;
}

void CRealAddDlg::OnSelChExchange()
{// �ŷ��Һ� �����ڵ� �����. ����:_AL, KRX:����, NXT:_NX, 039490_AL, 039490, 039490_NX
	m_strExchageCode = _T("");
	auto nIdx = m_cbExchan.GetCurSel();
	if(nIdx == 0)
		m_strExchageCode = _T("_AL");// ����:_AL
	else if(nIdx == 1)
		m_strExchageCode = _T("");// KRX:����
	else if(nIdx == 2)
		m_strExchageCode = _T("_NX");//NXT:_NX
	else
		m_strExchageCode = _T("");// KRX:����
}

CString CRealAddDlg::GetConvertSymbolCode(CString strOrg, int nOption /* = 0 */)
{// �ü�ó��(��ȸ, �ǽð� �ü�)�� �ŷ��Һ� �ڵ� ��ȯ�ʿ�, ����:_AL, KRX:����, NXT:_AL
	if(strOrg.IsEmpty())
		return strOrg;
	if(nOption == eExchangeType_Remove)
	{// �ŷ��� ���а� ����, ���ǰ˻� ����ó����.
		if(strOrg.GetLength() > 6)
			return strOrg.Left(6);
		else
			return strOrg;
	}
	else if(nOption == eExchangeType_Add)// �ŷ��� ���а� �߰�, �ü�ó��(�ü���ȸ, �ǽð� �ü�)
		return strOrg + m_strExchageCode;
	else
		return strOrg;
}

int CRealAddDlg::FindCodeRowIndex(CString strCode)
{
	CString	strSaved;
	int nFoundRow = -1;
	int nCnt = m_grdRealAdd.GetRowCount();
	for(int nIdx = 1; nIdx < nCnt; nIdx++)
	{
		strSaved = m_grdRealAdd.GetItemText(nIdx, 0);
		if(strSaved == strCode)
		{
			nFoundRow = nIdx;
			break;
		}
	}
	return nFoundRow;
}