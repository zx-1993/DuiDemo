#include "stdafx.h"
#include "MainWnd.h"


CMainWnd::CMainWnd()
{
	m_strXMLPath = L"duilib.xml";
}


CMainWnd::~CMainWnd()
{
}
LPCTSTR CMainWnd::GetWindowClassName() const
{
	return _T("MainWNd");
}

CDuiString CMainWnd::GetSkinFile()
{
	return m_strXMLPath;
}

CDuiString CMainWnd::GetSkinFolder()
{
	return _T("UIRes\\");
}

//��ȡ���ڻ��ƹ�����
CPaintManagerUI* CMainWnd::GetMainWndPaintManager()
{
	return &m_PaintManager;
}
//��Ϣ������
LRESULT CMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCLBUTTONDBLCLK:
	{	//���ε�˫�����������ʾ
		return 0;
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_RETURN) {
		
		}
		break;
	}
	case WM_KEYUP:
	{
		if (wParam == VK_TAB) {
			
			return 0;
		}
		break;
	}
	default:
		break;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}
LRESULT CMainWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CMainWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CMainWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CMainWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if (!::IsZoomed(*this))
	{
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if (pt.y < rcClient.top + rcSizeBox.top)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
	}

	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom)
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), _T("Button")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("Option")) != 0 &&
			_tcscmp(pControl->GetClass(), _T("Text")) != 0)
		{
			return HTCAPTION;
		}
	}
	return HTCLIENT;
}

LRESULT CMainWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0))
	{
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CMainWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-rcWork.left, -rcWork.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right;
	lpMMI->ptMaxSize.y = rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT CMainWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE)
	{
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed)
	{
		if (!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(true);
		}
		else
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}

//֪ͨ����
void CMainWnd::Notify(TNotifyUI& msg)
{
	//�������Ϣ
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("btnClose")) {//����رհ�ť
			Close(0);
			return;
		}
		else if(msg.pSender->GetName() == _T("btnGetEdit1"))
		{
			CEditUI* pEdit1 = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Edit1")));
			::AfxMessageBox(pEdit1->GetText());
		}
		else if (msg.pSender->GetName() == _T("btnProcessBar"))
		{
			for (size_t i = 1; i <= 10; i++)
			{
				m_pProgress->SetValue(i*10);
				Sleep(200);
			}
		}
		else if (msg.pSender->GetName() == _T("btnAddlistItem"))
		{
			CListTextElementUI* pListElement = (CListTextElementUI*)lParam;
			CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("listview")));
			if (pList) pList->Add(pListElement);
		}
	}

	else if (msg.sType == _T("selectchanged")) {//tabѡ��ı���Ϣ
		if (msg.pSender->GetName() == _T("VisuPasswd")) {
		}
		
	}
	else if (msg.sType == _T("setfocus")) {//��ý���
										   /*�˺�*/
		if (msg.pSender->GetName() == _T("editUserName")) {
			
		}
		else if (msg.pSender->GetName() == _T("editPassword")) {//����
		
		}
		else if (msg.pSender->GetName() == _T("btnLogin")) {
		
		}
	}
	else if (msg.sType == _T("killfocus")) {//ʧȥ����
											/*�˺�*/
		
	}
	else if (msg.sType == _T("textchanged")) {//���ֱ䶯��Ϣ
		
	}
	else if (msg.sType == _T("timer")) {//��ʱ����Ϣ
		
	}
	else if (msg.sType == _T("dropdown")) {//��Ͽ�������Ϣ

	}

	return __super::Notify(msg);
}

void CMainWnd::InitWindow()
{
	CenterWindow();
	m_pProgress = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("progress1")));
}


bool CMainWnd::OnSChanged(void* param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;
	if (pMsg->sType == _T("valuechanged"))
	{
		short H, S, L;
		CPaintManagerUI::GetHSL(&H, &S, &L);
		CPaintManagerUI::SetHSL(true, H, (static_cast<CSliderUI*>(pMsg->pSender))->GetValue(), L);
	}
	return true;
}

bool CMainWnd::OnHChanged(void* param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;
	if (pMsg->sType == _T("valuechanged"))
	{
		short H, S, L;
		CPaintManagerUI::GetHSL(&H, &S, &L);
		CPaintManagerUI::SetHSL(true, (static_cast<CSliderUI*>(pMsg->pSender))->GetValue(), S, L);
	}
	return true;
}
bool CMainWnd::OnLChanged(void* param)
{
	TNotifyUI* pMsg = (TNotifyUI*)param;
	if (pMsg->sType == _T("valuechanged"))
	{
		short H, S, L;
		CPaintManagerUI::GetHSL(&H, &S, &L);
		CPaintManagerUI::SetHSL(true, H, S, (static_cast<CSliderUI*>(pMsg->pSender))->GetValue());
	}
	return true;
}

void CMainWnd::OnPrepare()
{
	CSliderUI* pSilder;

// 	pSilder = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("h_controlor")));
// 	if (pSilder)
// 	{
// 		pSilder->OnNotify += MakeDelegate(this, &CMainWnd::OnHChanged);  //���ؼ��ͷ�����
// 																			 //�����Ա��������Ϊ�ص����������뵽OnNotify�У�Ȼ�����pMsg->pSender->OnNotify(pMsg)��ʱ��ѭ���������е��ຯ����ʵ��֪ͨ��Ч����
// 	}
// 	pSilder = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("s_controlor")));
// 	if (pSilder)
// 	{
// 		pSilder->OnNotify += MakeDelegate(this, &CMainWnd::OnSChanged);
// 	}
// 	pSilder = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("l_controlor")));
// 	if (pSilder)
// 	{
// 		pSilder->OnNotify += MakeDelegate(this, &CMainWnd::OnLChanged);
// 	}

}