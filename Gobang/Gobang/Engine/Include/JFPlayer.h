/**********************************************************************************
 *  JFPlayer.h
 *  Implementation of the Class JFPlayer
 *  Created on:      26-四月-2010 15:26:58
 *  Original author: Jiang Feng
 *********************************************************************************/

#if !defined(EA_55EE549B_34E6_43c6_BED7_118D3B53BBD0__INCLUDED_)
#define EA_55EE549B_34E6_43c6_BED7_118D3B53BBD0__INCLUDED_

#include "JFStruct.h"
#include "JFEngine.h"

/**
 * 玩家类
 * 电脑和棋手均是该类的对象
 */
class JFPlayer
{

public:
	JFPlayer();
	virtual ~JFPlayer();

    /**
     * 函数功能：初始化玩家
     * 参数：
     *          cmType : 所执棋子类型
     *          playerType : 玩家类型
     * 返回值：	无
     */
    void Initialize(const JFCMType& cmType, const JFPlayerType& playerType);

    /**
    * 函数功能：开始游戏，重置相关数据
    * 参数：无
    * 返回值：无	
    */
    void StartGame(void);

    /*******************************************************************
    * Callback begin
    *******************************************************************/

    /**
    * 函数功能：棋手落子后调用该函数
    * 参数：
    *          pGobangDlg : 父窗口的指针
    *          pCMType    : 棋子类型
    *          pPosInCB   : 棋子所在的棋盘节点位置
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
    * 函数功能：如果有一方胜利了即调用该函数
    * 参数：
    *          pGobangDlg   : 窗口的指针
    *          pCMType      : 棋子类型
    * 返回值：	
    */
    void SetCallBackWin(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg);

    /**
    * 函数功能：棋手改变棋手信息区的信息
    * 参数：
    *          pGobangDlg : 父窗口的指针
    *          pCMType : 棋子类型
    * 返回值：	
    */
    void SetCallBackChangePlayerInfo(void (*callback)(void* pGobangDlg, void* pPlayerInfoIndex, void* pStrInfo),
                                     void* pGobangDlg);

    /*******************************************************************
    * Callback end
    *******************************************************************/

    /*******************************************************************
     * set methods
     *******************************************************************/
    /**
     * 函数功能：设置该玩家的名字
     * 参数：
     *          strName : 要设置的名字
     * 返回值：	无
     */
    void SetPlayerName(const string& strName);

    /*******************************************************************
    * get methods
    *******************************************************************/
    /**
     * 函数功能：获取玩家类型
     * 参数：无
     * 返回值：
     *          当前玩家的类型: PLAYER_HUMAN、PLAYER_COMPUTER
     */
    const JFPlayerType& GetPlayerType(void);

    /**
    * 函数功能：获取玩家所执棋子类型
    * 参数：无
    * 返回值：
    *          当前玩家所执棋子类型: BLACK_CM、WHITE_CM
    */
    const JFCMType& GetCMType(void);

    /**
    * 函数功能：获取该玩家的名字
    * 参数：
    *          strName : 获取的名字
    * 返回值：	无
    */
    void GetPlayerName(string& strName);

    /*******************************************************************
     * 玩家的操作
     *******************************************************************/
	/**
	 * 认输
	 */
	virtual jf_bool GiveIn();
	/**
	 * 玩家落子
	 */
	virtual void PlaceChessman(const JFPosInCB& position);
	/**
	 * 悔棋
	 */
	virtual jf_bool Retract();
	/**
	 * 求和
	 */
	virtual jf_bool SueForPeace();
private:
    /**
     * 函数功能：获取该玩家的对手所执的棋子的类型
     * 参数：
     *          selfType  : 自己所执的棋子的类型
     *          enemyType : 对手所执的棋子的类型
     * 返回值：	无
     */
    inline void GetEnemyCMType(IN const JFCMType& selfType, OUT JFCMType& enemyType);

private:
	/**
	 * 该玩家所执的棋子的颜色
	 * 执黑者先落子，切必须落在棋盘中央
	 */
	JFCMType m_CMType;

    /** 
     * 当前玩家的类型
     */
    JFPlayerType m_playerType;

	/**
	 * 玩家的名字
	 */
	jf_string m_strName;

    void (*m_callbackPlaceChessman)(void* pParent, void* pCMType, void* pPosInCB);  // 回调函数，下子后调用
    void (*m_callbackChangeCMTypeToPlace)(void* pGobangDlg, void* pCMType);         // 用以改变当前要落子的棋子类型的函数指针
    void (*m_callbackWin)(void* pGobangDlg, void* pCMType);                         // 回调函数，胜利时调用
    void (*m_callbackChangePlayerInfo)(void* pGobangDlg, void* pPlayerInfoIndex, void* pStrInfo);   // 改变棋手信息

    void* m_pGobangDlg;                                                             // 指向窗口的指针

	/**
	 * 该玩家到目前为止胜利场数
	 */
	jf_uint m_uiWinGame;

//     /**
//     * 该玩家到目前为止失败场数
//     */
//     jf_uint m_uiLostGame;
// 
//     /**
//     * 该玩家到目前为止平局数
//     */
//     jf_uint m_uiPeaceGame;

};
#endif // !defined(EA_55EE549B_34E6_43c6_BED7_118D3B53BBD0__INCLUDED_)
