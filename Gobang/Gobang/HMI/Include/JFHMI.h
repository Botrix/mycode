/** 
 * 文 件 名: JFHMI.h
 * 摘    要: 本文件包含了界面管理的类的声明
 *
 * 作    者: 姜峰
 * 创建时间: 2010.05.22
 */

#ifndef JFHMI_H_
#define JFHMI_H_

#include "stdafx.h"
#include "JFStruct.h"
#include "JFCore.h"

#define BTN_WIDTH 80    // 按钮宽80
#define BTN_HEIGHT 26   // 按钮高26

#define BTN_CANCEL_WIDTH 50
#define BTN_CANCEL_HEIGHT 50

#define BTN_TOPLEFT_X 230   // 保存按钮的左上角的坐标为（230,590）
#define BTN_TOPLEFT_Y 590

#define BTN_BETWEEN_WIDTH 28    // 按钮间距离28

// 按钮顺序（从左到右）
// 1、保存
// 2、加载
// 3、开始
// 4、求和
// 5、认输
// 6、退出

class JFHMI
{
public:
    JFHMI();
    virtual ~JFHMI();
    /**
     * 函数功能：开始新一局游戏
     * 参数：无
     * 返回值：无
     */
    void StartGame(void);

public:
    void OnLButtonDown(const CPoint& point);
    void OnLButtonUp(const CPoint& point);
    void OnMouseMove(const CPoint& point);

public:
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
    void SetCallbackBtnStateChange(void (*fnCallback)(void* pGobangDlg, void* pBtnID, void* pBtnState), void* pGobangDlg);


    /**
    * 函数功能：设置触发按钮点击事件的回调函数指针和主窗口对象指针
    * 参数：
    *              fnCallback : 回调函数指针
    *              pGobangDlg : 主窗口对象指针
    * 返回值：	无
    */
    void SetCallbackBtnClickEvent(void (*fnCallback)(void* pGobangDlg, void* pBtnID), void* pGobangDlg);

    /*******************************************
    * Callback end
    *******************************************/

    /**
     * 函数功能：获取按钮的位置信息
     * 参数：
     *          无
     * 返回值：	按钮位置信息的引用
     */
    const vector<RECT>& GetVBtnRect(void);

    /**
    * 函数功能：获取按钮的text信息
    * 参数：
    *          无
    * 返回值：	按钮text信息的引用
    */
    const vector<string>& GetVBtnText(void);
private:
    /**
     * 函数功能：初始化资源
     * 参数：无
     * 返回值：无	
     */
    void InitializeRes(void);

    /**
     * 函数功能：获取点Point所在的按钮
     * 参数：
     *          point : 界面上的一个点
     * 返回值：	
     *          该所在的按钮的ID，如果不在按钮上
     *          则ID为BTN_INVALID
     */
    const JFBtnID GetBtnByPoint(const CPoint& point);

private:
    
    // 按钮相关
    vector<RECT>        m_vBtnRect;            // 按钮位置
    vector<string>      m_vBtnText;            // 按钮text
    JFBtnID             m_btnDown;             // 当前按下的按钮的ID 
    JFBtnID             m_btnHighlight;        // 当前处于高亮的按钮的ID 
    JFBtnState          m_btnState;            // 当前的激活的按钮的状态
    void*               m_pGobangDlg;          // CGobangDlg pointer

    // callback function pointer
    // 响应按钮状态改变
    void (*m_callbackBtnStateChange)(void* pGobangDlg, void* pBtnID, void* pBtnState);

    // 触发按钮点击事件
    void (*m_callbackBtnClickEvent)(void* pGobangDlg, void* pBtnID);

};

#endif