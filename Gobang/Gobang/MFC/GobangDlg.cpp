// GobangDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Gobang.h"
#include "GobangDlg.h"

#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")        // ��������

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGobangDlg �Ի���


CGobangDlg::CGobangDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGobangDlg::IDD, pParent)
    , m_curRpElement(REPAINT_ALL)
    , m_uiCurTimer(0)
    , m_uiCurBlackTimeCost(0)
    , m_uiCurWhiteTimeCost(0)
    , m_curTypeToPlace(NO_CM)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGobangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGobangDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CGobangDlg ��Ϣ�������

BOOL CGobangDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // ������Դ
    LoadResource();

    // ���ûص�����
    m_hmi.SetCallbackBtnClickEvent(OnBtnClick, this);
    m_hmi.SetCallbackBtnStateChange(OnChangeBtnState, this);
    m_core.SetCallBackPlaceChessman(OnPlayerPlaceChessman, this);
    m_core.SetCallBackChangeCMTypeToPlace(OnSetCurCMTypeToPlace, this);
    m_core.SetCallBackWin(OnPlayerWin, this);
    m_core.SetCallBackChangePlayerInfo(OnChangePlayerInfo, this);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


BOOL CGobangDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    
    OnEraseBk();
    return true;
    //return CDialog::OnEraseBkgnd(pDC);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGobangDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
        
		CDialog::OnPaint();
        OnRePaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CGobangDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*******************************************************************
* ����Ϊ��Ӧ���ĺ���
*******************************************************************/


void CGobangDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_hmi.OnLButtonDown(point);

    CDialog::OnLButtonDown(nFlags, point);
}

void CGobangDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    const JFPosInCB posInCB = GetPosInCBByPoint(point);

    if (posInCB.lX >= 0 && posInCB.lY >= 0)
    {
        // ����Ҫ����
        m_core.PlaceChessman(posInCB);
    }

    m_hmi.OnLButtonUp(point);

    CDialog::OnLButtonUp(nFlags, point);
}

void CGobangDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    m_hmi.OnMouseMove(point);

    CDialog::OnMouseMove(nFlags, point);
}

void CGobangDlg::OnTimer(UINT_PTR nIDEvent)
{
    m_uiCurTimer++;

    if (m_uiCurTimer % 15 == 0)
    {
        PlaySound(_T("res\\sound\\����.wav"), NULL, SND_FILENAME|SND_ASYNC );
    }

    char buf[16] = {0};
    sprintf_s(buf, "%d", m_uiCurTimer);
    m_vCurRpPlayerInfo.push_back(pair<JFPlayerInfoIndex, string>(PLAYER_INFO_TIMER, string(buf)));
    m_curRpElement = REPAINT_BASIC_INFO;
    UpdateUI();

    CDialog::OnTimer(nIDEvent);
}

/*******************************************************************
* ������������������ݼ�����
*******************************************************************/

/*******************************************
* Callback begin
*******************************************/
/**
* �������ܣ���Ӧ�ı䰴ť��״̬
* ������
*          pGobangDlg   : ָ��ǰ���ʵ����ָ��
*          pBtnID       : ָ��ťID��ָ��
*          pBtnState    : ��ť����״̬
* ����ֵ��	��
*/
void CGobangDlg::OnChangeBtnState(IN void* pGobangDlg, IN void* pBtnID, IN void* pBtnState)
{
    if (NULL != pGobangDlg && NULL != pBtnID && NULL != pBtnState)
    {
        CGobangDlg* pThis = (CGobangDlg*)pGobangDlg;

        pThis->OnChangeBtnState(*(JFBtnID*)pBtnID, *(JFBtnState*)pBtnState);
    }
}

void CGobangDlg::OnChangeBtnState(IN const JFBtnID& btnID, IN const JFBtnState& btnState)
{
    _ASSERT(btnID != BTN_INVALID && btnState != BTN_STATE_INVALID);

    DisplayBtnBmp(btnID, btnState);
    DisplayBtnText(btnID);

    if (BTN_STATE_HIGHLIGHT == btnState)
    {
        PlaySound(_T("res\\sound\\menu.wav"), NULL, SND_FILENAME|SND_ASYNC|SND_NODEFAULT|SND_NOWAIT);
    }
}


/**
* �������ܣ�������Ӧ��������Ϣ�ĸ���
* ������
*          pGobangDlg  : ָ��ǰ���ʵ����ָ��
*          pPlayerInfoIndex : ָ��������Ϣ����Ϣ������
*          pStrInfo    : ָ���µ���Ϣ��ָ��
* ����ֵ�� ��
*/
void CGobangDlg::OnChangePlayerInfo(IN void* pGobangDlg, IN void* pPlayerInfoIndex, IN void* pStrInfo)
{
    if (NULL != pGobangDlg && NULL != pPlayerInfoIndex && NULL != pStrInfo)
    {
        CGobangDlg* pThis = (CGobangDlg*)(pGobangDlg);

        pThis->OnChangePlayerInfo(*(JFPlayerInfoIndex*)pPlayerInfoIndex, *(string*)pStrInfo);
    }
}

void CGobangDlg::OnChangePlayerInfo(IN const JFPlayerInfoIndex& playerInfoIndex, IN const string& strInfo)
{
    m_vCurRpPlayerInfo.push_back(pair<JFPlayerInfoIndex, string>(playerInfoIndex, strInfo));

    m_vPlayerInfo[playerInfoIndex] = strInfo;
    m_curRpElement = REPAINT_BASIC_INFO;
}

/**
* �������ܣ�������Ӧ���õ�ǰҪ���ӵ����ӵ�����
* ������
*          pGobangDlg  : ָ��ǰ���ʵ����ָ��
*          pCMType     : ָ��ǰҪ���ӵ����ӵ�����
* ����ֵ�� ��
*/
void CGobangDlg::OnSetCurCMTypeToPlace(IN void* pGobangDlg, IN void* pCMType)
{
    if (NULL != pGobangDlg && NULL != pCMType)
    {
        CGobangDlg* pThis = (CGobangDlg*)(pGobangDlg);

        pThis->OnSetCurCMTypeToPlace(*(JFCMType*)pCMType);
    }
}

void CGobangDlg::OnSetCurCMTypeToPlace(IN const JFCMType& CMType)
{
    m_curTypeToPlace = CMType;
    DisplayBitmap(CPoint(44, 400), m_vCurBmpCMToPlace[CMType], CM_RADIUS*2, CM_RADIUS*2);
}

/**
* �������ܣ���Ӧ��������
* ������
*          pGobangDlg    : ָ��ǰ���ڵ�ָ�뼴this
*          pCMType  : ��ǰʤ�������ӵ����ͣ���ʤ�����Ƿ���������
*          pPosInCB : ���ӷ��õ����̽ڵ�λ��
* ����ֵ��	��
*/
void CGobangDlg::OnPlayerPlaceChessman(IN void* pGobangDlg, IN void* pCMType, IN void* pPosInCB)
{
    if (NULL != pGobangDlg && NULL != pCMType && NULL != pPosInCB)
    {
        CGobangDlg* pThis = (CGobangDlg*)(pGobangDlg);

        pThis->OnPlayerPlaceChessman(*(JFCMType*)pCMType, *(JFPosInCB*)pPosInCB);
    }
}

void CGobangDlg::OnPlayerPlaceChessman(const JFCMType& CMType, IN JFPosInCB& posInCB)
{
    _ASSERT(NO_CM != CMType);

    CPoint pointInCB = GetPointByPosInCB(posInCB);
    // ��ȡ���ӷ��õĿ�ʼλ��
    CPoint pointCM;
    pointCM.x = pointInCB.x - CM_RADIUS;
    pointCM.y = pointInCB.y - CM_RADIUS;

    JFCMForDisp cmForDisp;
    cmForDisp.lx = pointCM.x;
    cmForDisp.ly = pointCM.y;
    cmForDisp.type = CMType;

    // ����ǰ���ӵ�λ�ô洢����
    m_vChessmanPlaced.push_back(cmForDisp);


    // ���޸���һ���µ����ӵ�ͼ��
    ReDisplayLastCM();
    DisplayCurCM();

    char buf[16] = {0};

    // 1����ǰ�Ǻ�������
    if (BLACK_CM == CMType)
    {
        KillTimer(BLACK_TIMER);
        m_uiCurBlackTimeCost += m_uiCurTimer;
        sprintf_s(buf, "%d", m_uiCurBlackTimeCost);
        OnChangePlayerInfo(PLAYER_INFO_TIMER, string("0"));
        OnChangePlayerInfo(BLACK_TIME_COST, string(buf));

        m_uiCurTimer = 0;
        SetTimer(WHITE_TIMER, 1000, NULL);
    }
    else if (WHITE_CM == CMType)
    {
        KillTimer(WHITE_TIMER);
        m_uiCurWhiteTimeCost += m_uiCurTimer;
        sprintf_s(buf, "%d", m_uiCurWhiteTimeCost);
        OnChangePlayerInfo(PLAYER_INFO_TIMER, string("0"));
        OnChangePlayerInfo(WHITE_TIME_COST, string(buf)); 

        m_uiCurTimer = 0;
        SetTimer(BLACK_TIMER, 1000, NULL);
    }

    UpdateUI();
}

/**
* �������ܣ�����ʤ��ʱ���øûص�����
* ������
*          pGobangDlg    : ָ��ǰ���ڵ�ָ�뼴this
*          pCMType  : ��ǰʤ�������ӵ����ͣ���ʤ�����Ƿ���������
* ����ֵ��	��
*/
void CGobangDlg::OnPlayerWin(IN void* pGobangDlg, IN void* pCMType)
{
    if (NULL != pGobangDlg && NULL != pCMType)
    {
        CGobangDlg* pThis = (CGobangDlg*)(pGobangDlg);

        pThis->OnPlayerWin(*(JFCMType*)pCMType);
    }
}

void CGobangDlg::OnPlayerWin(const JFCMType& CMType)
{
    if (BLACK_CM == CMType)
    {
        KillTimer(WHITE_TIMER);
        DisplayText("����Ӯ!", 80, RGB(255, 0, 0), CPoint(350, 200));
    }
    else
    {
        KillTimer(BLACK_TIMER);
        DisplayText("����Ӯ!", 80, RGB(255, 0, 0), CPoint(350, 200));
    }
}

/**
* �������ܣ���Ӧ���ֵ������Ȱ�ť
* ������
*          pGobangDlg  : ָ��ǰ���ڵ�ָ��
*          pBtnID : ָ��ťID��ָ��
* ����ֵ��	��
*/
void CGobangDlg::OnBtnClick(IN void* pGobangDlg, IN void* pBtnID)
{
    if (NULL != pGobangDlg && NULL != pBtnID)
    {
        CGobangDlg* pThis = (CGobangDlg*)(pGobangDlg);

        pThis->OnBtnClick(*(JFBtnID*)pBtnID);
    }
}

void CGobangDlg::OnBtnClick(IN const JFBtnID& btnID)
{
    if (BTN_INVALID == btnID)
    {
        return;
    }

    switch (btnID)
    {
    case BTN_SAVE :
        {
            break;
        }
    case BTN_LOAD :
        {
            break;
        }
    case BTN_START :
        {
            this->StartGame();
            UpdateUI();
            m_core.StartGame();
            m_hmi.StartGame();
            break;
        }
    case BTN_PEACE :
        {
            break;
        }
    case BTN_GIVEIN :
        {
            break;
        }
    case BTN_CANCEL :
        {
            PostMessage(WM_CLOSE, 0, NULL);
            break;
        }
    default:
        {
            _ASSERT(false);
            break;
        }
    }
}

/*******************************************
* Callback end
*******************************************/

/*******************************************
* paint methods begin
*******************************************/
/**
* �������ܣ���������
* ��������
* ����ֵ����	
*/
void CGobangDlg::OnEraseBk(void)
{
    if (REPAINT_ALL == m_curRpElement)
    {
        DisplayBK();
    }
    else if (REPAINT_BTN == m_curRpElement)
    {
    }
    else if (REPAINT_BASIC_INFO == m_curRpElement)
    {
        size_t uiSize = m_vCurRpPlayerInfo.size();

        for (size_t ui=0; ui<uiSize; ++ui)
        {
            const RECT& rect = m_vPlayerInfoRect[m_vCurRpPlayerInfo[ui].first];

            DisplayBitmap(CPoint(rect.left, rect.top), m_vBmpPlayerInfoBK[m_vCurRpPlayerInfo[ui].first], 80, 30);
        }
    }
    else
    {
        MessageBox(_T("�����ػ���Ҫ�ػ���������"), _T("Waining!"), MB_OK);
    }
}

/**
* �������ܣ��ػ�
* ��������
* ����ֵ����	
*/
void CGobangDlg::OnRePaint(void)
{
    if (REPAINT_ALL == m_curRpElement)
    {
        DisplayAllCM();
        DisplayCurCM();
        DisplayAllBtnText();
        DisplayAllPlayerInfo();

        if (m_curTypeToPlace != NO_CM)
        {
            DisplayBitmap(CPoint(44, 400), m_vCurBmpCMToPlace[m_curTypeToPlace], CM_RADIUS*2, CM_RADIUS*2);
        }
    }
    else if (REPAINT_BTN == m_curRpElement)
    {
    }
    else if (REPAINT_BASIC_INFO == m_curRpElement)
    {
        size_t uiSize = m_vCurRpPlayerInfo.size();

        for (size_t ui=0; ui<uiSize; ++ui)
        {
            const RECT& rect = m_vPlayerInfoRect[m_vCurRpPlayerInfo[ui].first];

            DisplayText(m_vCurRpPlayerInfo[ui].second, 18, RGB(0, 0, 0), CPoint(rect.left, rect.top));
        }

        if (m_curTypeToPlace != NO_CM)
        {
            DisplayBitmap(CPoint(44, 400), m_vCurBmpCMToPlace[m_curTypeToPlace], CM_RADIUS*2, CM_RADIUS*2);
        }

        m_vCurRpPlayerInfo.clear();
        m_curRpElement = REPAINT_ALL;
    }
    else
    {
        MessageBox(_T("�����ػ���Ҫ�ػ���������"), _T("Waining!"), MB_OK);
    }

}

/**
* �������ܣ�������Դ
* ��������
* ����ֵ����	
*/
void CGobangDlg::LoadResource(void)
{
    m_bmpBK.LoadBitmap(IDB_BITMAP_BK);

    // ��ʼ������
    m_vBmpCM[BLACK_CM].LoadBitmap(IDB_BITMAP_BLACK_CHESSMAN);
    m_vBmpCM[WHITE_CM].LoadBitmap(IDB_BITMAP_WHITE_CHESSMAN);

    m_vCurBmpCM[BLACK_CM].LoadBitmap(IDB_BITMAP_CUR_BLACK_CM);
    m_vCurBmpCM[WHITE_CM].LoadBitmap(IDB_BITMAP_CUR_WHITE_CM);

    m_vCurBmpCMToPlace[BLACK_CM].LoadBitmap(IDB_BITMAP_CUR_BLACK_CM_TO_PLACE);
    m_vCurBmpCMToPlace[WHITE_CM].LoadBitmap(IDB_BITMAP_CUR_WHITE_CM_TO_PLACE);
 
    // ��ʼ����ť
    m_vRegularBtnBmp[BTN_STATE_NORMAL].LoadBitmap(IDB_BITMAP_BTN_NORMAL);
    m_vRegularBtnBmp[BTN_STATE_HIGHLIGHT].LoadBitmap(IDB_BITMAP_BTN_HIGHLIGHT);
    m_vRegularBtnBmp[BTN_STATE_DOWN].LoadBitmap(IDB_BITMAP_BTN_DOWN);
    
    m_vCancelBtnBmp[BTN_STATE_NORMAL].LoadBitmap(IDB_BITMAP_CANCEL_NORMAL);              // �˳���ť��̬
    m_vCancelBtnBmp[BTN_STATE_HIGHLIGHT].LoadBitmap(IDB_BITMAP_CANCEL_HIGHLIGHT);        // �˳���ť����
    m_vCancelBtnBmp[BTN_STATE_DOWN].LoadBitmap(IDB_BITMAP_CANCEL_DOWN);                  // �˳���ť����
    
    // ��ʼ����������Ϣ����
    m_vBmpPlayerInfoBK[BLACK_NAME].LoadBitmap(IDB_BITMAP_PLAYER_INFO_BLACK_NAME);
    m_vBmpPlayerInfoBK[BLACK_SCORE].LoadBitmap(IDB_BITMAP_PLAYER_INFO_BLACK_SCORE);
    m_vBmpPlayerInfoBK[BLACK_TIME_COST].LoadBitmap(IDB_BITMAP_PLAYER_INFO_BLACK_TIME_COST);
    m_vBmpPlayerInfoBK[WHITE_NAME].LoadBitmap(IDB_BITMAP_PLAYER_INFO_WHITE_NAME);
    m_vBmpPlayerInfoBK[WHITE_SCORE].LoadBitmap(IDB_BITMAP_PLAYER_INFO_WHITE_SCORE);
    m_vBmpPlayerInfoBK[WHITE_TIME_COST].LoadBitmap(IDB_BITMAP_PLAYER_INFO_WHITE_TIME_COST);
    m_vBmpPlayerInfoBK[PLAYER_INFO_TIMER].LoadBitmap(IDB_BITMAP_PLAYER_INFO_TIMER);

    // ��ʼ��������Ϣ������Ϣ
    m_vPlayerInfo[BLACK_NAME] = string("����");
    m_vPlayerInfo[BLACK_SCORE] = string("0");
    m_vPlayerInfo[WHITE_NAME] = string("����");
    m_vPlayerInfo[WHITE_SCORE] = string("0");

    RECT playerInfoRect;
    playerInfoRect.left = 105;
    playerInfoRect.top = 135 + 3;
    playerInfoRect.right = 185;
    playerInfoRect.bottom = 165;
    m_vPlayerInfoRect[BLACK_NAME] = playerInfoRect;
    playerInfoRect.top += 30;
    playerInfoRect.bottom += 30;
    m_vPlayerInfoRect[BLACK_SCORE] = playerInfoRect;
    playerInfoRect.top += 30;
    playerInfoRect.bottom += 30;
    m_vPlayerInfoRect[BLACK_TIME_COST] = playerInfoRect;


    playerInfoRect.top = 285 + 3;
    playerInfoRect.bottom = 315;
    m_vPlayerInfoRect[WHITE_NAME] = playerInfoRect;
    playerInfoRect.top += 30;
    playerInfoRect.bottom += 30;
    m_vPlayerInfoRect[WHITE_SCORE] = playerInfoRect;
    playerInfoRect.top += 30;
    playerInfoRect.bottom += 30;
    m_vPlayerInfoRect[WHITE_TIME_COST] = playerInfoRect;

    playerInfoRect.top = 400 + 3;
    playerInfoRect.bottom = 430;
    m_vPlayerInfoRect[PLAYER_INFO_TIMER] = playerInfoRect;

    // �������
    m_pointCBTopleft   = CPoint(230, 60);
    m_pointBottomright = CPoint(743, 573);   // +513
}

/**
* �������ܣ��ڵ�point����ʾλͼbitmap
* ������
*          point  : �����ϵ�һ����
*          bitmap : λͼ�ļ�
* ����ֵ��	��
*/
void CGobangDlg::DisplayBitmap(IN const CPoint& point, 
                               IN const CBitmap& bitmap, 
                               IN const int bmpWidth, 
                               IN const int bmpHeight)
{
    CDC* pDC = GetDC();                 // ��ȡ�ڴ�DC
    CDC DCCompatible;                   // �뵱ǰ��ʾ�豸���ݵ�DC
    DCCompatible.CreateCompatibleDC(pDC);
    CBitmap* pBmpOld = DCCompatible.SelectObject((CBitmap*)&bitmap);

    pDC->BitBlt(point.x,            // x����
                point.y,            // y����
                bmpWidth,           // λͼ��
                bmpHeight,          // λͼ��
                &DCCompatible,      // ����DC
                0,                  // Ĭ��Դλͼ��ȫ��
                0,                  // Ĭ��Դλͼ��ȫ��
                SRCCOPY);           // �ڴ濽��

    DCCompatible.SelectObject(pBmpOld);
    DCCompatible.DeleteDC();
    ReleaseDC(pDC);
}

/**
* �������ܣ��ڵ�point����ʾ�ı�,Ĭ�ϼӴ�
* ������
*          strText  : Ҫ��ʾ���ı�����
*          iFontHeight : �����
*          color    : �ı�����ɫ
*          point    : �ı���ʼ��ʾ��λ��
* ����ֵ��	��
*/
void CGobangDlg::DisplayText( IN const string& strText, 
                             IN const int& iFontHeight,
                             IN const COLORREF& color,
                             IN const CPoint& point)
{
    CDC* pDC = GetDC();                 // ��ȡ�ڴ�DC
    CFont font;

    font.CreateFont(iFontHeight,               // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_NORMAL,                    // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        GB2312_CHARSET,            // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        _T("����"));   

    CFont* pOldFont = pDC->SelectObject(&font);

    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(color);

    CString strOut(strText.c_str());

    pDC->TextOut(point.x, point.y, strOut);

    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);
}

/**
* �������ܣ��ڵ�point����ʾһ������
* ������
*          point    : �����ϵ�һ����
*          CMtype   : ��������
* ����ֵ��	��
*/
void CGobangDlg::DisplayCM( IN const CPoint& point, IN const JFCMType CMtype)
{
    DisplayBitmap(point, m_vBmpCM[CMtype], CM_RADIUS*2, CM_RADIUS*2);
}

/**
* �������ܣ���ʾ��ǰ�������
* ��������
* ����ֵ��	��
*/
void CGobangDlg::DisplayCurCM(void)
{
    size_t uiCMNum = m_vChessmanPlaced.size();

    if (uiCMNum >= 1)
    {
        const JFCMForDisp& chessman = m_vChessmanPlaced[m_vChessmanPlaced.size()-1];
        DisplayBitmap(CPoint(chessman.lx, chessman.ly), m_vCurBmpCM[chessman.type], BTN_WIDTH, BTN_HEIGHT);
    }
}

/**
* �������ܣ�������ʾ��һ���������
* ��������
* ����ֵ��	��
*/
void CGobangDlg::ReDisplayLastCM(void)
{
    size_t uiCMNum = m_vChessmanPlaced.size();

    if (uiCMNum >= 2)
    {
        const JFCMForDisp& chessman = m_vChessmanPlaced[m_vChessmanPlaced.size()-2];
        DisplayCM(CPoint(chessman.lx, chessman.ly), chessman.type);
    }
}

/**
* �������ܣ���ʾ����
* ��������
* ����ֵ����	
*/
void CGobangDlg::DisplayBK(void)
{
    DisplayBitmap(CPoint(0,0), m_bmpBK, 890, 630);

#ifdef _DEBUG
    CPoint rowStartPoint(230 + 26, 60 + 10);
    CPoint colStartPoint(230 + 10, 60 + 24);

    for (int i=0; i<15; ++i)
    {
        char buf[16] = {0};
        _itoa(i, buf, 10);
        string str(buf);
        DisplayText(str, 15, RGB(0,0,0), rowStartPoint);
        DisplayText(str, 15, RGB(0,0,0), colStartPoint);

        rowStartPoint.x += 32;
        colStartPoint.y += 32;
    }
#endif
}

/**
* �������ܣ���ʾ���е����е�����
* ��������
* ����ֵ����	
*/
void CGobangDlg::DisplayAllCM(void)
{
    // ��ʾ���еĺڰ�����
    size_t uiCMNum = m_vChessmanPlaced.size();
    CPoint point;
    for (size_t i=0; i<uiCMNum; ++i)
    {
        point.x = m_vChessmanPlaced[i].lx;
        point.y = m_vChessmanPlaced[i].ly;
        DisplayCM(point, m_vChessmanPlaced[i].type);
    }
}

/**
* �������ܣ���ʾ��ť��λͼ
* ������
*           btnID : ��ť��ID
* ����ֵ����	
*/
void CGobangDlg::DisplayBtnBmp(const JFBtnID& btnID, const JFBtnState& btnState)
{
    const vector<RECT>& vRect = m_hmi.GetVBtnRect();

    CBitmap* pBmp = NULL;

    if (BTN_CANCEL != btnID)
    {
        pBmp = &m_vRegularBtnBmp[btnState];
    }
    else
    {
        pBmp = &m_vCancelBtnBmp[btnState];
    }

    CPoint point(vRect[btnID].left, vRect[btnID].top);

    if (BTN_CANCEL == btnID)
    {
        DisplayBitmap(point, *pBmp, BTN_CANCEL_WIDTH, BTN_CANCEL_HEIGHT);
    }
    else
    {
        DisplayBitmap(point, *pBmp, BTN_WIDTH, BTN_HEIGHT);
    }
}

/**
* �������ܣ���ʾ��ť��text
* ������
*           btnID : ��ť��ID
* ����ֵ����	
*/
void CGobangDlg::DisplayBtnText(const JFBtnID& btnID)
{
    const vector<RECT>& vRect = m_hmi.GetVBtnRect();
    const vector<string>& vBtnText = m_hmi.GetVBtnText();

    CPoint point(vRect[btnID].left, vRect[btnID].top);
    string strText = vBtnText[btnID];

    if (BTN_CANCEL == btnID)
    {
        point.x += 8;
        point.y += 20;
    }
    else
    {
        point.x += 20;
        point.y += 3;
    }

    DisplayText(strText, 20, RGB(0, 0, 0), point);
}

/**
* �������ܣ���ʾ���а�ť��text
* ��������
* ����ֵ����	
*/
void CGobangDlg::DisplayAllBtnText(void)
{
    const vector<RECT>& vBtnRect = m_hmi.GetVBtnRect();
    const vector<string>& vBtnText = m_hmi.GetVBtnText();
    CPoint pointToText;

    COLORREF color = RGB(0, 0, 0);        // ��ɫ
    size_t iBtnAcount = vBtnRect.size();

    // �����һ��İ�ť�����������˳���
    for (size_t i=0; i<iBtnAcount-1; ++i)
    {
        pointToText.x = vBtnRect[i].left + 20;
        pointToText.y = vBtnRect[i].top + 3;

        DisplayText(vBtnText[i], HMI_FONT_HEIGHT, color, pointToText);
    }

    pointToText.x = vBtnRect[BTN_CANCEL].left + 8;
    pointToText.y = vBtnRect[BTN_CANCEL].top + 20;
    DisplayText(vBtnText[BTN_CANCEL], HMI_FONT_HEIGHT, color, pointToText);
}

/**
* �������ܣ���ʾ���е�����������Ϣ
* ��������
* ����ֵ����	
*/
void CGobangDlg::DisplayAllPlayerInfo(void)
{
    // ��ʾ���еĺڰ�����
    for (size_t ui=0; ui<PLAYER_INFO_NUM; ++ui)
    {
        DisplayText(m_vPlayerInfo[ui], 
                    18, 
                    RGB(0, 0, 0), 
                    CPoint(m_vPlayerInfoRect[ui].left, m_vPlayerInfoRect[ui].top));
    }
}

/**
* �������ܣ�ˢ�½���
* ��������
* ����ֵ����	
*/
void CGobangDlg::UpdateUI(void)
{
    Invalidate();
}
/*******************************************
* paint methods end
*******************************************/

/*******************************************
* regular methods begin
*******************************************/
/**
* �������ܣ���ʼ�µ�һ��
* ��������
* ����ֵ����	
*/
void CGobangDlg::StartGame(void)
{
    m_curRpElement = REPAINT_ALL;
    m_vChessmanPlaced.clear();
    m_vBtnRepaint.clear();
    m_vCurRpPlayerInfo.clear();

    // Ϊ���Ӻ�ʱ��ʱ
    m_uiCurTimer = 0;
    m_uiCurBlackTimeCost = 0;
    m_uiCurWhiteTimeCost = 0;
    SetTimer(BLACK_TIMER, 1000, NULL);
}


/**
* �������ܣ��жϵ�point�Ƿ���������
* ������
*          point : �û�����������ĵ�
* ����ֵ��	
*          true  : �û������λ����������
*          false : �û������λ�ò��������� 
*/
bool CGobangDlg::IsPointInCB(IN const CPoint& point) const
{
    return ( point.x >= m_pointCBTopleft.x 
        && point.x <= m_pointBottomright.x
        && point.y >= m_pointCBTopleft.y 
        && point.y <= m_pointBottomright.y );
}

/**
* �������ܣ���ȡ��Point���ڵ����̽ڵ�
* ������
*          point : �����ϵ�һ����
* ����ֵ��	
*          �����ڵ����̽ڵ��λ�ã�������ڰ�ť��
*          ��x��y��Ϊ��ֵ
*/
const JFPosInCB CGobangDlg::GetPosInCBByPoint(const CPoint& point)
{
    JFPosInCB posInCB(-1, -1);

    if ( IsPointInCB(point) )
    {
        jf_long32 lXPos = (point.x - m_pointCBTopleft.x) / CB_GRID_WIDTH;
        jf_long32 lXOffset = (point.x - m_pointCBTopleft.x) % CB_GRID_WIDTH;
        if ( lXOffset < CM_RADIUS+1 )
        {
            posInCB.lX = lXPos;
        }
        else if ( lXOffset < CM_RADIUS*2+2 )
        {
            posInCB.lX = -1;
            posInCB.lY = -1;

            return posInCB;        // �ڶ�����̽ڵ���м�����
        }
        else
        {
            posInCB.lX = lXPos + 1;
        }

        jf_long32 lYPos = (point.y - m_pointCBTopleft.y) / CB_GRID_WIDTH;
        jf_long32 lYOffset = (point.y - m_pointCBTopleft.y) % CB_GRID_WIDTH;
        if ( lYOffset < CM_RADIUS+1 )
        {
            posInCB.lY = lYPos;
        }
        else if ( lYOffset < CM_RADIUS*2+2 )
        {
            posInCB.lX = -1;
            posInCB.lY = -1;

            return posInCB;        // �ڶ�����̽ڵ���м�����
        }
        else
        {
            posInCB.lY = lYPos + 1;
        }

        // ���̵�ʵ����Ч�������������СһȦ
        if (0 == posInCB.lX
            || CB_GRID_NUM + 2 == posInCB.lX
            || 0 == posInCB.lY
            || CB_GRID_NUM + 2 == posInCB.lY)
        {
            posInCB.lX = -1;
            posInCB.lY = -1;
        }
        else
        {
            --posInCB.lX;
            --posInCB.lY;
        }
    }

    return posInCB;
}

/**
* �������ܣ��������ӽڵ��λ�û�ȡ���������ϵ�λ��
* ������
*          posInCB : ���̽ڵ��λ��
* ����ֵ��	
*          �����̽ڵ��ڽ����ϵ�����
*/
CPoint CGobangDlg::GetPointByPosInCB(IN const JFPosInCB& posInCB)
{
    jf_long32  lx = posInCB.lX;
    jf_long32  ly = posInCB.lY;

    lx++;   // ��Ϊ���Ƕ��Ǵ����̵ĸ������Ͻ������
    ly++;   // ���Ǵ����̵����Ͻ������

    CPoint point;

    point.x = 32*lx + m_pointCBTopleft.x;
    point.y = 32*ly + m_pointCBTopleft.y;

    return point;
}

/*******************************************
* regular methods end
*******************************************/

