/**********************************************************************************
*  JFCore.h
*  Implementation of the Class JFCore
*  Created on:      26-四月-2010 15:26:55
*  Original author: Jiang Feng
*********************************************************************************/

#if !defined(EA_D6278271_629D_45e9_AAB2_983D99E43C97__INCLUDED_)
#define EA_D6278271_629D_45e9_AAB2_983D99E43C97__INCLUDED_

#include "stdafx.h"
#include "JFStruct.h"
#include "JFPlayer.h"

/**
* JFCore的作用是MFC到JFEngine、JFSysData的接口，任何操作信息都要经过这里。相当于隐藏下层结构的屏障。
*/
class JFCore
{

public:
    JFCore();
    virtual ~JFCore();

    /**
     * 函数功能：初始化数据成员，用于开局（包括开始新一局）
     * 参数：无
     * 返回值：无	
     */
    void Initialize(void);

    /**
     * 函数功能：电脑落子
     * 参数：无
     * 返回值：无	
     */
    void ComputerPlaceCM(void);

    /**
     * 函数功能：线程过程函数
     * 参数：
     *          传递给线程执行函数的参数
     * 返回值：	无
     */
    static UINT BeginComputerThread(LPVOID pParam);

    /*******************************************************************
    * Callback begin
    *******************************************************************/

    /**
    * 函数功能：棋手落子后调用该函数
    * 参数：
    *          pGobangDlg : 父窗口的指针
    *          pCMType : 棋子类型
    *          pPosInCB: 棋子所在的棋盘节点位置
    * 返回值：	
    */
    void SetCallBackPlaceChessman(void (*callback)(void* pGobangDlg, void* pCMType, void* pPosInCB), void* pGobangDlg);

    /**
    * 函数功能：棋手落子后调用该函数来改变下一个要落子的棋子的类型
    * 参数：
    *          pGobangDlg : 父窗口的指针
    *          pCMType : 棋子类型
    * 返回值：	
    */
    void SetCallBackChangeCMTypeToPlace(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg);

    /**
    * 函数功能：棋手改变棋手信息区的信息
    * 参数：
    *          pGobangDlg : 父窗口的指针
    *          pCMType : 棋子类型
    * 返回值：	
    */
    void SetCallBackChangePlayerInfo(void (*callback)(void* pGobangDlg, void* pPlayerInfoIndex, void* pStrInfo), void* pGobangDlg);

    /**
    * 函数功能：如果有一方胜利了即调用该函数
    * 参数：
    *          pGobangDlg : 父窗口的指针
    *          pCMType : 棋子类型
    * 返回值：	
    */
    void SetCallBackWin(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg);

    /*******************************************************************
    * Callback end
    *******************************************************************/

    /*******************************************************************
    * JFPlayer begin
    *******************************************************************/
    /**
    * 函数功能：落子
    * 参数：
    *          position : 放置棋子的棋盘节点位置
    * 返回值：	
    *          true  : position处落子成功
    *          false : position处已经有棋子，不允许再放置棋子
    */
    void PlaceChessman(const JFPosInCB& position);

    /**
    * 获取电脑所执的棋子的类型
    */
    const JFCMType GetCMTypeOfComputer() const;

    /**
     * 函数功能：设置玩家的名字
     * 参数：
     *          type : 要设置的玩家所执的棋子的类型
     *          strName : 要设置的名字
     * 返回值：	
     */
    void SetPlayerName(IN const JFCMType& type, IN const string& strNmae);

    /**
    * 函数功能：获取玩家的名字
    * 参数：
    *          type : 要获取的玩家所执的棋子的类型
    *          strName : 获取的名字
    * 返回值：	
    */
    void GetPlayerName(IN const JFCMType& type, IN string& strName);

    /*******************************************************************
    * JFPlayer end
    *******************************************************************/

    /*******************************************************************
    * JFSysData start
    *******************************************************************/

    /**
    * 函数功能：设置当前的游戏模式
    * 参数：
    *           gameMode : 游戏模式
    * 返回值：
    *           true  : 设置成功
    *           false : 设置失败
    */
    jf_bool SetCurGameMode(const JFGameMode& gameMode);

    /**
    * 函数功能：获取当前的游戏模式
    * 参数：无
    * 返回值：
    *          当前对战模式
    */
    const JFGameMode& GetCurGameMode(void);

    /**
    * 获取当前要下的棋子的类型
    */
    const JFCMType& GetCurCMTypeToPlace() const;

    /**
    * 设置游戏难度
    */
    void SetGameLevel(const JFGameLevel& level);

    /*******************************************************************
    * JFSysData end
    *******************************************************************/

    /*******************************************************************
    * JFEngine start
    *******************************************************************/

    /**
    * 获取当前棋局
    */
    const JFCBSituation& GetCurSituation(); 

    /*******************************************************************
    * JFEngine end
    *******************************************************************/

    /**
    * 加载保存过的棋局文件
    */
    jf_bool LoadFile();
    /**
    * 加载保存过的replay文件
    */
    jf_bool LoadReplay();

    /**
    * 保存棋局文件
    */
    jf_bool SaveFile();
    /**
    * 保存replay文件
    */
    jf_bool SaveReplay();

    /**
    * 函数功能：开始游戏，重置相关数据
    * 参数：无
    * 返回值：无	
    */
    void StartGame(void);

    /**
    * 函数功能：结束游戏
    * 参数：无
    * 返回值：无	
    */
    void EndGame();

public:
        //CRITICAL_SECTION m_crtSection;          // 临界区
    CWinThread* m_pCompterThread;           // 电脑下子线程
    jf_bool m_bComDoing;                       // 电脑正在落子
private:

    // 存储指向棋手对象的指针
    // [0]:NO_CM 
    // [1]: BLACK_CM 执黑者
    // [2]: WHITE_CM 执白者
    JFPlayer* m_vpPlayer[3]; 
    JFPlayer* m_pComputer;   // 指向电脑

    void*     m_pGobangDlg;  // 指向主窗口对象的指针
    void (*m_callbackChangeCMTypeToPlace)(void* pGobangDlg, void* pCMType);      // 用以改变当前要落子的棋子类型的函数指针

    
     
    JFCBSituation m_curSituation;           // 当前棋局
};
#endif // !defined(EA_D6278271_629D_45e9_AAB2_983D99E43C97__INCLUDED_)
