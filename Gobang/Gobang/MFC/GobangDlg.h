// GobangDlg.h : 头文件
//

#pragma once

#include "JFHMI.h"

#define PLAYER_INFO_NUM  7      // 棋手信息区信息个数
#define BLACK_TIMER  0          // 黑子的计时器标识
#define WHITE_TIMER  1          // 白子的计时器标识

// CGobangDlg 对话框
class CGobangDlg : public CDialog
{
// 构造
public:
	CGobangDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GOBANG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
public:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
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
    static void OnChangeBtnState(IN void* pGobangDlg, IN void* pBtnID, IN void* pBtnState);
    void OnChangeBtnState(IN const JFBtnID& btnID, IN const JFBtnState& btnState);

    /**
    * 函数功能：用来响应棋手区信息的更改
    * 参数：
    *          pGobangDlg  : 指向当前类的实例的指针
    *          playerInfoIndex : 指向棋手信息区信息的索引
    *          strInfo    : 指向新的信息的指针
    * 返回值： 无
    */
    static void OnChangePlayerInfo(IN void* pGobangDlg, IN void* pPlayerInfoIndex, IN void* pStrInfo);
    void OnChangePlayerInfo(IN const JFPlayerInfoIndex& playerInfoIndex, IN const string& strInfo);

    /**
    * 函数功能：用来响应设置当前要落子的棋子的类型
    * 参数：
    *          pGobangDlg  : 指向当前类的实例的指针
    *          pCMType     : 指向当前要落子的棋子的类型
    * 返回值： 无
    */
    static void OnSetCurCMTypeToPlace(IN void* pGobangDlg, IN void* pCMType);
    void OnSetCurCMTypeToPlace(IN const JFCMType& CMType);

    /**
    * 函数功能：响应棋手下子
    * 参数：
    *          pGobangDlg    : 指向当前窗口的指针即this
    *          pCMType  : 当前胜利的棋子的类型，由胜利的那方棋手设置
    *          pPosInCB : 棋子放置的棋盘节点位置
    * 返回值：	无
    */
    static void OnPlayerPlaceChessman(IN void* pGobangDlg, IN void* pCMType, IN void* pPosInCB);
    void OnPlayerPlaceChessman(const JFCMType& CMType, IN JFPosInCB& posInCB);

    /**
    * 函数功能：棋手胜利时调用该回调函数
    * 参数：
    *          pGobangDlg    : 指向当前窗口的指针即this
    *          pCMType  : 当前胜利的棋子的类型，由胜利的那方棋手设置
    * 返回值：	无
    */
    static void OnPlayerWin(IN void* pGobangDlg, IN void* pCMType);
    void OnPlayerWin(const JFCMType& CMType);

    /**
    * 函数功能：响应棋手点击保存等按钮
    * 参数：
    *          pGobangDlg  : 指向当前窗口的指针
    *          pBtnID : 指向按钮ID的指针
    * 返回值：	无
    */
    static void OnBtnClick(IN void* pGobangDlg, IN void* pBtnID);
    void OnBtnClick(IN const JFBtnID& btnID);

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
    void OnEraseBk(void);

    /**
     * 函数功能：重绘
     * 参数：无
     * 返回值：无	
     */
    void OnRePaint(void);

    /**
     * 函数功能：加载资源
     * 参数：无
     * 返回值：无	
     */
    void LoadResource(void);

    /**
    * 函数功能：在点point处显示位图bitmap
    * 参数：
    *          point  : 界面上的一个点
    *          bitmap : 位图文件
    * 返回值：	无
    */
    void DisplayBitmap( IN const CPoint& point, 
                        IN const CBitmap& bitmap, 
                        IN const int bmpWidth, 
                        IN const int bmpHeight);

    /**
    * 函数功能：在点point处显示文本,默认加粗
    * 参数：
    *          strText  : 要显示的文本内容
    *          iFontHeight : 字体高
    *          color    : 文本的颜色
    *          point    : 文本开始显示的位置
    * 返回值：	无
    */
    void DisplayText(IN const string& strText, 
                    IN const int& iFontHeight,
                    IN const COLORREF& color,
                    IN const CPoint& point);

    /**
    * 函数功能：在点point处显示一个棋子
    * 参数：
    *          point    : 界面上的一个点
    *          CMtype   : 棋子类型
    * 返回值：	无
    */
    void DisplayCM( IN const CPoint& point, IN const JFCMType CMtype);

    /**
    * 函数功能：显示当前落的棋子
    * 参数：无
    * 返回值：	无
    */
    void DisplayCurCM(void);

    /**
    * 函数功能：重新显示上一个落的棋子
    * 参数：无
    * 返回值：	无
    */
    void ReDisplayLastCM(void);

    /**
    * 函数功能：显示背景
    * 参数：无
    * 返回值：无	
    */
    void DisplayBK(void);


    /**
    * 函数功能：显示所有的已有的棋子
    * 参数：无
    * 返回值：无	
    */
    void DisplayAllCM(void);

    /**
    * 函数功能：显示按钮的位图
    * 参数：
    *           btnID : 按钮的ID
    * 返回值：无	
    */
    void DisplayBtnBmp(const JFBtnID& btnID, const JFBtnState& btnState);

    /**
    * 函数功能：显示按钮的text
    * 参数：
    *           btnID : 按钮的ID
    * 返回值：无	
    */
    void DisplayBtnText(const JFBtnID& btnID);

    /**
    * 函数功能：显示所有按钮的text
    * 参数：无
    * 返回值：无	
    */
    void DisplayAllBtnText(void);

    /**
    * 函数功能：显示所有的棋手区域信息
    * 参数：无
    * 返回值：无	
    */
    void DisplayAllPlayerInfo(void);

    /**
     * 函数功能：刷新界面
     * 参数：无
     * 返回值：无	
     */
    void UpdateUI(void);

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
    void StartGame(void);

    /**
    * 函数功能：判断点point是否在棋盘上
    * 参数：
    *          point : 用户点击鼠标产生的点
    * 返回值：	
    *          true  : 用户点击的位置在棋盘上
    *          false : 用户点击的位置不在棋盘上 
    */
    bool IsPointInCB(IN const CPoint& point) const;

    /**
    * 函数功能：获取点Point所在的棋盘节点
    * 参数：
    *          point : 界面上的一个点
    * 返回值：	
    *          该所在的棋盘节点的位置，如果不在按钮上
    *          则x、y均为负值
    */
    const JFPosInCB GetPosInCBByPoint(const CPoint& point);

    /**
    * 函数功能：根据棋子节点的位置获取其在棋盘上的位置
    * 参数：
    *          posInCB : 棋盘节点的位置
    * 返回值：	
    *          该棋盘节点在界面上的坐标
    */
    CPoint GetPointByPosInCB(IN const JFPosInCB& posInCB);

private:

    JFHMI m_hmi;            // Human Machine Interface management
    JFCore m_core;          // 五子棋引擎接口

    // 状态量
    JFRepaintElement    m_curRpElement;            // 当前需要重绘的元素类型
    vector<JFCMForDisp> m_vChessmanPlaced;         // 当前已经下的棋子的集合
    vector<JFBtnID>     m_vBtnRepaint;             // 当前需要重绘按钮
    vector<pair<JFPlayerInfoIndex, string>> m_vCurRpPlayerInfo;      // 当前需要重绘棋手区的信息,包括时间信息
    JFCMType            m_curTypeToPlace;          // 当前要落子的棋子类型

    // 资源
    CBitmap         m_bmpBK;                       // 背景位图
    CBitmap         m_vBmpCM[3];                   // 黑白棋子
    CBitmap         m_vCurBmpCM[3];                // 当前下的黑白棋子位图
    CBitmap         m_vCurBmpCMToPlace[3];         // 当前要下的黑白棋子位图
    CBitmap         m_vRegularBtnBmp[3];           // 普通的三态按钮
    CBitmap         m_vCancelBtnBmp[3];            // 退出按钮的三态按钮
    CBitmap         m_vBmpPlayerInfoBK[PLAYER_INFO_NUM];         // 棋手信息的背景
    RECT            m_vPlayerInfoRect[PLAYER_INFO_NUM];          // 棋手区信息的位置
    string          m_vPlayerInfo[PLAYER_INFO_NUM];              // 棋手信息区信息

    UINT            m_uiCurTimer;                   // 当前计时器的值
    UINT            m_uiCurBlackTimeCost;           // 当前黑子总共的耗时
    UINT            m_uiCurWhiteTimeCost;           // 当前白子总共的耗时

    // 棋盘相关
    CPoint m_pointCBTopleft;        // 棋盘左上角
    CPoint m_pointBottomright;      // 棋盘右下角    
};
