/** 
* 文 件 名: JFHMI.cpp
* 摘    要: 本文件包含了界面管理的类的声明
*
* 作    者: 姜峰
* 创建时间: 2010.05.22
*/

#include "JFHMI.h"

JFHMI::JFHMI()
: m_btnDown(BTN_INVALID)
, m_btnHighlight(BTN_INVALID)
, m_pGobangDlg(NULL)
, m_btnState(BTN_STATE_INVALID)
{
    InitializeRes();
}


JFHMI::~JFHMI()
{

}

/**
* 函数功能：开始新一局游戏
* 参数：无
* 返回值：无
*/
void JFHMI::StartGame(void)
{
    m_btnDown = BTN_INVALID;
    m_btnHighlight = BTN_INVALID;
    m_btnState = BTN_STATE_INVALID;
}


void JFHMI::OnLButtonDown(const CPoint& point)
{
    JFBtnID btnID = GetBtnByPoint(point);

    if (BTN_INVALID != btnID)
    {
        m_btnDown = btnID;        // 记录当前处于非常态的按钮的ID

        m_btnState = BTN_STATE_DOWN;
        m_callbackBtnStateChange(m_pGobangDlg, &m_btnDown, &m_btnState);
        m_btnState = BTN_STATE_INVALID;
    }
}

void JFHMI::OnLButtonUp(const CPoint& point)
{
    JFBtnID btnID = GetBtnByPoint(point);

    if (BTN_INVALID != btnID)
    {
        m_btnDown = btnID;        // 记录当前处于非常态的按钮的ID

        m_btnState = BTN_STATE_HIGHLIGHT;
        m_callbackBtnStateChange(m_pGobangDlg, &m_btnDown, &m_btnState);
        m_btnState = BTN_STATE_INVALID;
        m_btnHighlight = btnID;
    }

    // 如果Up的按钮是被Down的按钮，那么即触发该按钮对应的事件
    if (btnID == m_btnDown)
    {
        m_callbackBtnClickEvent(m_pGobangDlg, &m_btnDown);
        m_btnDown = BTN_INVALID;
    }
}

void JFHMI::OnMouseMove(const CPoint& point)
{
    JFBtnID btnID = GetBtnByPoint(point);

    // 1、当前有按钮处于高亮
    if (BTN_INVALID != m_btnHighlight)
    {
        // 1.1 当前按钮就是处于高亮的按钮
        if (btnID == m_btnHighlight)
        {
            // 不做处理
        }
        // 1.2 当前按钮不是当前正处于高亮态的按钮
        else
        {
            // 1.2.1 恢复高亮的那个按钮的状态
            m_btnState = BTN_STATE_NORMAL;
            m_callbackBtnStateChange(m_pGobangDlg, &m_btnHighlight, &m_btnState);
            m_btnState = BTN_STATE_INVALID;
            m_btnHighlight = BTN_INVALID;

            // 1.2.2 将当前按钮设置为高亮态
            if (BTN_INVALID != btnID)
            {
                m_btnHighlight = btnID;
                m_btnState = BTN_STATE_HIGHLIGHT;
                m_callbackBtnStateChange(m_pGobangDlg, &m_btnHighlight, &m_btnState);  
                m_btnState = BTN_STATE_INVALID;
            }
        } // end else
    }// end if
    // 2、当前没有按钮处于高亮态
    else
    {
        if (BTN_INVALID == btnID)
        {
            // 当前并不在任何按钮上
        }
        else
        {
            m_btnHighlight = btnID;
            m_btnState = BTN_STATE_HIGHLIGHT;
            m_callbackBtnStateChange(m_pGobangDlg, &m_btnHighlight, &m_btnState); 
            m_btnState = BTN_STATE_INVALID;
        }
    }
}

/*******************************************
* Callback begin
*******************************************/
/**
* 函数功能：设置响应按钮状态改变的回调函数指针和主窗口对象的指针
* 参数：
*              fnCallback : 回调函数指针
*              pGobangDlg : 主窗口对象指针
* 返回值：	无
*/
void JFHMI::SetCallbackBtnStateChange(void (*fnCallback)(void* pGobangDlg, void* pBtnID, void* pBtnState), 
                                      void* pGobangDlg)
{
    _ASSERT(NULL != fnCallback && NULL != pGobangDlg);

    m_callbackBtnStateChange = fnCallback;
    m_pGobangDlg = pGobangDlg;
}

/**
* 函数功能：设置触发按钮点击事件的回调函数指针和主窗口对象指针
* 参数：
*              fnCallback : 回调函数指针
*              pGobangDlg : 主窗口对象指针
* 返回值：	无
*/
void JFHMI::SetCallbackBtnClickEvent(void (*fnCallback)(void* pGobangDlg, void* pBtnID), void* pGobangDlg)
{
    _ASSERT(NULL != fnCallback && NULL != pGobangDlg);

    m_callbackBtnClickEvent = fnCallback;
    m_pGobangDlg = pGobangDlg;
}

/*******************************************
* Callback end
*******************************************/
/**
* 函数功能：获取按钮的位置信息
* 参数：
*          无
* 返回值：	按钮位置信息的引用
*/
const vector<RECT>& JFHMI::GetVBtnRect(void)
{
    return m_vBtnRect;
}

/**
* 函数功能：获取按钮的text信息
* 参数：
*          无
* 返回值：	按钮text信息的引用
*/
const vector<string>& JFHMI::GetVBtnText(void)
{
    return m_vBtnText;
}

/**
* 函数功能：初始化资源
* 参数：无
* 返回值：无	
*/
void JFHMI::InitializeRes(void)
{
    // 初始化按钮位置
    // 5个功能按钮
    RECT rect;
    rect.left = BTN_TOPLEFT_X;
    rect.top  = BTN_TOPLEFT_Y;
    rect.right= BTN_TOPLEFT_X + BTN_WIDTH;
    rect.bottom = BTN_TOPLEFT_Y + BTN_HEIGHT;

    m_vBtnRect.push_back(rect);
    // 还有四个
    for (int i=0; i<4; ++i)
    {
        rect.left = rect.right + BTN_BETWEEN_WIDTH;
        rect.right = rect.left + BTN_WIDTH;

        m_vBtnRect.push_back(rect);
    }

    m_vBtnText.push_back("保存");
    m_vBtnText.push_back("加载");
    m_vBtnText.push_back("开始");
    m_vBtnText.push_back("求和");
    m_vBtnText.push_back("认输");

    // 退出按钮
    rect.left = 840;
    rect.top = 580;
    rect.right = 890;
    rect.bottom = 630;

    m_vBtnRect.push_back(rect);
    m_vBtnText.push_back("退出");
}


/**
* 函数功能：获取点Point所在的按钮
* 参数：
*          point : 界面上的一个点
* 返回值：	
*          该所在的按钮的ID，如果不在按钮上
*          则ID为BTN_INVALID
*/
const JFBtnID JFHMI::GetBtnByPoint(const CPoint& point)
{
    JFBtnID index = BTN_INVALID; // 按钮ID

    size_t uiRectSize = m_vBtnRect.size();
    for (size_t ui=0; ui<uiRectSize; ++ui)
    {
        const RECT& rect = m_vBtnRect[ui]; 

        if (point.x >= rect.left
            && point.x <= rect.right
            && point.y >= rect.top
            && point.y <= rect.bottom)
        {
            index = JFBtnID(ui);
        }
    }

    return index;
}
