// GobangDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Gobang.h"
#include "GobangDlg.h"

#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")        // 播放音乐

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGobangDlg 对话框


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


// CGobangDlg 消息处理程序

BOOL CGobangDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    // 加载资源
    LoadResource();

    // 设置回调函数
    m_hmi.SetCallbackBtnClickEvent(OnBtnClick, this);
    m_hmi.SetCallbackBtnStateChange(OnChangeBtnState, this);
    m_core.SetCallBackPlaceChessman(OnPlayerPlaceChessman, this);
    m_core.SetCallBackChangeCMTypeToPlace(OnSetCurCMTypeToPlace, this);
    m_core.SetCallBackWin(OnPlayerWin, this);
    m_core.SetCallBackChangePlayerInfo(OnChangePlayerInfo, this);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


BOOL CGobangDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    
    OnEraseBk();
    return true;
    //return CDialog::OnEraseBkgnd(pDC);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGobangDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        
		CDialog::OnPaint();
        OnRePaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CGobangDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*******************************************************************
* 以下为相应鼠标的函数
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
        // 棋手要落子
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
        PlaySound(_T("res\\sound\\闹钟.wav"), NULL, SND_FILENAME|SND_ASYNC );
    }

    char buf[16] = {0};
    sprintf_s(buf, "%d", m_uiCurTimer);
    m_vCurRpPlayerInfo.push_back(pair<JFPlayerInfoIndex, string>(PLAYER_INFO_TIMER, string(buf)));
    m_curRpElement = REPAINT_BASIC_INFO;
    UpdateUI();

    CDialog::OnTimer(nIDEvent);
}

/*******************************************************************
* 以下是五子棋相关数据及方法
*******************************************************************/

/*******************************************
* Callback begin
*******************************************/
/**
* 函数功能：响应改变按钮的状态
* 参数：
*          pGobangDlg   : 指向当前类的实例的指针
*          pBtnID       : 指向按钮ID的指针
*          pBtnState    : 按钮的新状态
* 返回值：	无
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
* 函数功能：用来响应棋手区信息的更改
* 参数：
*          pGobangDlg  : 指向当前类的实例的指针
*          pPlayerInfoIndex : 指向棋手信息区信息的索引
*          pStrInfo    : 指向新的信息的指针
* 返回值： 无
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
* 函数功能：用来响应设置当前要落子的棋子的类型
* 参数：
*          pGobangDlg  : 指向当前类的实例的指针
*          pCMType     : 指向当前要落子的棋子的类型
* 返回值： 无
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
* 函数功能：响应棋手下子
* 参数：
*          pGobangDlg    : 指向当前窗口的指针即this
*          pCMType  : 当前胜利的棋子的类型，由胜利的那方棋手设置
*          pPosInCB : 棋子放置的棋盘节点位置
* 返回值：	无
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
    // 获取棋子放置的开始位置
    CPoint pointCM;
    pointCM.x = pointInCB.x - CM_RADIUS;
    pointCM.y = pointInCB.y - CM_RADIUS;

    JFCMForDisp cmForDisp;
    cmForDisp.lx = pointCM.x;
    cmForDisp.ly = pointCM.y;
    cmForDisp.type = CMType;

    // 将当前棋子的位置存储起来
    m_vChessmanPlaced.push_back(cmForDisp);


    // 先修改上一个下的棋子的图像
    ReDisplayLastCM();
    DisplayCurCM();

    char buf[16] = {0};

    // 1、当前是黑子落子
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
* 函数功能：棋手胜利时调用该回调函数
* 参数：
*          pGobangDlg    : 指向当前窗口的指针即this
*          pCMType  : 当前胜利的棋子的类型，由胜利的那方棋手设置
* 返回值：	无
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
        DisplayText("黑子赢!", 80, RGB(255, 0, 0), CPoint(350, 200));
    }
    else
    {
        KillTimer(BLACK_TIMER);
        DisplayText("白子赢!", 80, RGB(255, 0, 0), CPoint(350, 200));
    }
}

/**
* 函数功能：响应棋手点击保存等按钮
* 参数：
*          pGobangDlg  : 指向当前窗口的指针
*          pBtnID : 指向按钮ID的指针
* 返回值：	无
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
* 函数功能：擦除背景
* 参数：无
* 返回值：无	
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
        MessageBox(_T("棋子重绘需要重绘整个窗口"), _T("Waining!"), MB_OK);
    }
}

/**
* 函数功能：重绘
* 参数：无
* 返回值：无	
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
        MessageBox(_T("棋子重绘需要重绘整个窗口"), _T("Waining!"), MB_OK);
    }

}

/**
* 函数功能：加载资源
* 参数：无
* 返回值：无	
*/
void CGobangDlg::LoadResource(void)
{
    m_bmpBK.LoadBitmap(IDB_BITMAP_BK);

    // 初始化棋子
    m_vBmpCM[BLACK_CM].LoadBitmap(IDB_BITMAP_BLACK_CHESSMAN);
    m_vBmpCM[WHITE_CM].LoadBitmap(IDB_BITMAP_WHITE_CHESSMAN);

    m_vCurBmpCM[BLACK_CM].LoadBitmap(IDB_BITMAP_CUR_BLACK_CM);
    m_vCurBmpCM[WHITE_CM].LoadBitmap(IDB_BITMAP_CUR_WHITE_CM);

    m_vCurBmpCMToPlace[BLACK_CM].LoadBitmap(IDB_BITMAP_CUR_BLACK_CM_TO_PLACE);
    m_vCurBmpCMToPlace[WHITE_CM].LoadBitmap(IDB_BITMAP_CUR_WHITE_CM_TO_PLACE);
 
    // 初始化按钮
    m_vRegularBtnBmp[BTN_STATE_NORMAL].LoadBitmap(IDB_BITMAP_BTN_NORMAL);
    m_vRegularBtnBmp[BTN_STATE_HIGHLIGHT].LoadBitmap(IDB_BITMAP_BTN_HIGHLIGHT);
    m_vRegularBtnBmp[BTN_STATE_DOWN].LoadBitmap(IDB_BITMAP_BTN_DOWN);
    
    m_vCancelBtnBmp[BTN_STATE_NORMAL].LoadBitmap(IDB_BITMAP_CANCEL_NORMAL);              // 退出按钮常态
    m_vCancelBtnBmp[BTN_STATE_HIGHLIGHT].LoadBitmap(IDB_BITMAP_CANCEL_HIGHLIGHT);        // 退出按钮高亮
    m_vCancelBtnBmp[BTN_STATE_DOWN].LoadBitmap(IDB_BITMAP_CANCEL_DOWN);                  // 退出按钮按下
    
    // 初始化棋手区信息背景
    m_vBmpPlayerInfoBK[BLACK_NAME].LoadBitmap(IDB_BITMAP_PLAYER_INFO_BLACK_NAME);
    m_vBmpPlayerInfoBK[BLACK_SCORE].LoadBitmap(IDB_BITMAP_PLAYER_INFO_BLACK_SCORE);
    m_vBmpPlayerInfoBK[BLACK_TIME_COST].LoadBitmap(IDB_BITMAP_PLAYER_INFO_BLACK_TIME_COST);
    m_vBmpPlayerInfoBK[WHITE_NAME].LoadBitmap(IDB_BITMAP_PLAYER_INFO_WHITE_NAME);
    m_vBmpPlayerInfoBK[WHITE_SCORE].LoadBitmap(IDB_BITMAP_PLAYER_INFO_WHITE_SCORE);
    m_vBmpPlayerInfoBK[WHITE_TIME_COST].LoadBitmap(IDB_BITMAP_PLAYER_INFO_WHITE_TIME_COST);
    m_vBmpPlayerInfoBK[PLAYER_INFO_TIMER].LoadBitmap(IDB_BITMAP_PLAYER_INFO_TIMER);

    // 初始化棋手信息区域信息
    m_vPlayerInfo[BLACK_NAME] = string("无名");
    m_vPlayerInfo[BLACK_SCORE] = string("0");
    m_vPlayerInfo[WHITE_NAME] = string("电脑");
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

    // 棋盘相关
    m_pointCBTopleft   = CPoint(230, 60);
    m_pointBottomright = CPoint(743, 573);   // +513
}

/**
* 函数功能：在点point处显示位图bitmap
* 参数：
*          point  : 界面上的一个点
*          bitmap : 位图文件
* 返回值：	无
*/
void CGobangDlg::DisplayBitmap(IN const CPoint& point, 
                               IN const CBitmap& bitmap, 
                               IN const int bmpWidth, 
                               IN const int bmpHeight)
{
    CDC* pDC = GetDC();                 // 获取内存DC
    CDC DCCompatible;                   // 与当前显示设备兼容的DC
    DCCompatible.CreateCompatibleDC(pDC);
    CBitmap* pBmpOld = DCCompatible.SelectObject((CBitmap*)&bitmap);

    pDC->BitBlt(point.x,            // x坐标
                point.y,            // y坐标
                bmpWidth,           // 位图宽
                bmpHeight,          // 位图高
                &DCCompatible,      // 兼容DC
                0,                  // 默认源位图的全部
                0,                  // 默认源位图的全部
                SRCCOPY);           // 内存拷贝

    DCCompatible.SelectObject(pBmpOld);
    DCCompatible.DeleteDC();
    ReleaseDC(pDC);
}

/**
* 函数功能：在点point处显示文本,默认加粗
* 参数：
*          strText  : 要显示的文本内容
*          iFontHeight : 字体高
*          color    : 文本的颜色
*          point    : 文本开始显示的位置
* 返回值：	无
*/
void CGobangDlg::DisplayText( IN const string& strText, 
                             IN const int& iFontHeight,
                             IN const COLORREF& color,
                             IN const CPoint& point)
{
    CDC* pDC = GetDC();                 // 获取内存DC
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
        _T("隶书"));   

    CFont* pOldFont = pDC->SelectObject(&font);

    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(color);

    CString strOut(strText.c_str());

    pDC->TextOut(point.x, point.y, strOut);

    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);
}

/**
* 函数功能：在点point处显示一个棋子
* 参数：
*          point    : 界面上的一个点
*          CMtype   : 棋子类型
* 返回值：	无
*/
void CGobangDlg::DisplayCM( IN const CPoint& point, IN const JFCMType CMtype)
{
    DisplayBitmap(point, m_vBmpCM[CMtype], CM_RADIUS*2, CM_RADIUS*2);
}

/**
* 函数功能：显示当前落的棋子
* 参数：无
* 返回值：	无
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
* 函数功能：重新显示上一个落的棋子
* 参数：无
* 返回值：	无
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
* 函数功能：显示背景
* 参数：无
* 返回值：无	
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
* 函数功能：显示所有的已有的棋子
* 参数：无
* 返回值：无	
*/
void CGobangDlg::DisplayAllCM(void)
{
    // 显示所有的黑白棋子
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
* 函数功能：显示按钮的位图
* 参数：
*           btnID : 按钮的ID
* 返回值：无	
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
* 函数功能：显示按钮的text
* 参数：
*           btnID : 按钮的ID
* 返回值：无	
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
* 函数功能：显示所有按钮的text
* 参数：无
* 返回值：无	
*/
void CGobangDlg::DisplayAllBtnText(void)
{
    const vector<RECT>& vBtnRect = m_hmi.GetVBtnRect();
    const vector<string>& vBtnText = m_hmi.GetVBtnText();
    CPoint pointToText;

    COLORREF color = RGB(0, 0, 0);        // 黑色
    size_t iBtnAcount = vBtnRect.size();

    // 先输出一般的按钮，最后输出“退出”
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
* 函数功能：显示所有的棋手区域信息
* 参数：无
* 返回值：无	
*/
void CGobangDlg::DisplayAllPlayerInfo(void)
{
    // 显示所有的黑白棋子
    for (size_t ui=0; ui<PLAYER_INFO_NUM; ++ui)
    {
        DisplayText(m_vPlayerInfo[ui], 
                    18, 
                    RGB(0, 0, 0), 
                    CPoint(m_vPlayerInfoRect[ui].left, m_vPlayerInfoRect[ui].top));
    }
}

/**
* 函数功能：刷新界面
* 参数：无
* 返回值：无	
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
* 函数功能：开始新的一局
* 参数：无
* 返回值：无	
*/
void CGobangDlg::StartGame(void)
{
    m_curRpElement = REPAINT_ALL;
    m_vChessmanPlaced.clear();
    m_vBtnRepaint.clear();
    m_vCurRpPlayerInfo.clear();

    // 为黑子耗时计时
    m_uiCurTimer = 0;
    m_uiCurBlackTimeCost = 0;
    m_uiCurWhiteTimeCost = 0;
    SetTimer(BLACK_TIMER, 1000, NULL);
}


/**
* 函数功能：判断点point是否在棋盘上
* 参数：
*          point : 用户点击鼠标产生的点
* 返回值：	
*          true  : 用户点击的位置在棋盘上
*          false : 用户点击的位置不在棋盘上 
*/
bool CGobangDlg::IsPointInCB(IN const CPoint& point) const
{
    return ( point.x >= m_pointCBTopleft.x 
        && point.x <= m_pointBottomright.x
        && point.y >= m_pointCBTopleft.y 
        && point.y <= m_pointBottomright.y );
}

/**
* 函数功能：获取点Point所在的棋盘节点
* 参数：
*          point : 界面上的一个点
* 返回值：	
*          该所在的棋盘节点的位置，如果不在按钮上
*          则x、y均为负值
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

            return posInCB;        // 在多个棋盘节点的中间区域
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

            return posInCB;        // 在多个棋盘节点的中间区域
        }
        else
        {
            posInCB.lY = lYPos + 1;
        }

        // 棋盘的实际有效区域比棋盘区域小一圈
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
* 函数功能：根据棋子节点的位置获取其在棋盘上的位置
* 参数：
*          posInCB : 棋盘节点的位置
* 返回值：	
*          该棋盘节点在界面上的坐标
*/
CPoint CGobangDlg::GetPointByPosInCB(IN const JFPosInCB& posInCB)
{
    jf_long32  lx = posInCB.lX;
    jf_long32  ly = posInCB.lY;

    lx++;   // 因为它们都是从棋盘的格子左上角算起的
    ly++;   // 并非从棋盘的左上角算起的

    CPoint point;

    point.x = 32*lx + m_pointCBTopleft.x;
    point.y = 32*ly + m_pointCBTopleft.y;

    return point;
}

/*******************************************
* regular methods end
*******************************************/

