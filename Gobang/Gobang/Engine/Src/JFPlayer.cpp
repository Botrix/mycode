/**********************************************************************************
 *  JFPlayer.h
 *  Implementation of the Class JFPlayer
 *  Created on:      26-四月-2010 15:26:58
 *  Original author: Jiang Feng
 *********************************************************************************/

#include "JFPlayer.h"


JFPlayer::JFPlayer()
: m_CMType(NO_CM)
, m_playerType(PLAYER_NO)
, m_callbackPlaceChessman(NULL)
, m_callbackWin(NULL)
, m_uiWinGame(0)
// , m_uiLostGame(0)
// , m_uiPeaceGame(0)
{
    m_strName = "";     // string类默认构造为""
}



JFPlayer::~JFPlayer()
{

}


/**
* 函数功能：初始化玩家
* 参数：
*          cmType : 所执棋子类型
*          playerType : 玩家类型
* 返回值：	无
*/
void JFPlayer::Initialize(const JFCMType& cmType, const JFPlayerType& playerType)
{
    _ASSERT(NO_CM != cmType);
    _ASSERT(PLAYER_NO != playerType);

    m_CMType = cmType;
    m_playerType = playerType;
}

/**
* 函数功能：开始游戏，重置相关数据
* 参数：无
* 返回值：无	
*/
void JFPlayer::StartGame(void)
{
//     m_CMType = NO_CM;
//     m_playerType = PLAYER_NO;
//     m_strName = string("");  
}
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
void JFPlayer::SetCallBackPlaceChessman(void (*callback)(void* pGobangDlg, void* pCMType, void* pPosInCB), void* pGobangDlg)
{
    _ASSERT(NULL != callback && NULL != pGobangDlg);

    m_callbackPlaceChessman = callback;
    m_pGobangDlg = pGobangDlg;
}


/**
* 函数功能：棋手落子后调用该函数来改变下一个要落子的棋子的类型
* 参数：
*          pGobangDlg : 父窗口的指针
*          pCMType : 棋子类型
* 返回值：	
*/
void JFPlayer::SetCallBackChangeCMTypeToPlace(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg)
{
    _ASSERT(NULL != pGobangDlg && NULL != callback);

    m_pGobangDlg = pGobangDlg;
    m_callbackChangeCMTypeToPlace = callback;
}

/**
* 函数功能：如果有一方胜利了即调用该函数
* 参数：
*          pGobangDlg   : 窗口的指针
*          pCMType      : 棋子类型
* 返回值：	
*/
void JFPlayer::SetCallBackWin(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg)
{
    _ASSERT(NULL != callback && NULL != pGobangDlg);

    m_callbackWin = callback;
    m_pGobangDlg = pGobangDlg;
}

/**
* 函数功能：棋手改变棋手信息区的信息
* 参数：
*          pGobangDlg : 父窗口的指针
*          pCMType : 棋子类型
* 返回值：	
*/
void JFPlayer::SetCallBackChangePlayerInfo(void (*callback)(void* pGobangDlg, void* pPlayerInfoIndex, void* pStrInfo),
                                           void* pGobangDlg)
{
    _ASSERT(NULL != callback && NULL != pGobangDlg);

    m_callbackChangePlayerInfo = callback;
    m_pGobangDlg = pGobangDlg;
}

/*******************************************************************
* Callback end
*******************************************************************/

/**
 * 认输
 */
jf_bool JFPlayer::GiveIn(){

	return  NULL;
}


/**
 * 玩家落子
 * 如果当前点可以落子则返回true，否则返回false
 */
void JFPlayer::PlaceChessman(const JFPosInCB& position)
{
    _ASSERT(PLAYER_NO != m_playerType);
    _ASSERT(m_CMType == JFSysData::GetSysData()->GetCurCMTypeToPlace());

    JFChessman chessman;
   
    chessman.type = m_CMType;
    JFCBSituation situation;
    
	if (PLAYER_COMPUTER == m_playerType)
	{// 对于电脑
        chessman.position = JFEngine::GetEngine()->GetBestPosForPlaceCM(m_CMType);
	}
    else
    {// 对于人
        chessman.position = position;
    }

    if (JFSysData::GetSysData()->IsCurPosAvailable(chessman.position))
    {
        JFSysData::GetSysData()->PlaceChessman(chessman);

        // 1、设置下一个要下的棋子的类型
        JFCMType enemyType;
        GetEnemyCMType(m_CMType, enemyType);
        m_callbackChangeCMTypeToPlace(m_pGobangDlg, &enemyType);
        JFSysData::GetSysData()->SetCurCMTypeToPlace(enemyType);

        // 2、告诉界面，棋子已经落下
        m_callbackPlaceChessman(m_pGobangDlg, &m_CMType, &chessman.position);

        // 3、发出当前棋局的提醒（可能是通知对方俺胜利啦）
        JFCBSituation situation;
        JFEngine::GetEngine()->GetCurSituation(m_CMType, situation);

        if (situation.typeWin != NO_CM)
        {
            m_callbackWin(m_pGobangDlg, &m_CMType);
            
            m_uiWinGame++;
            char buf[16] = {0};
            sprintf_s(buf, "%d", m_uiWinGame);

            string strInfo(buf);
            JFPlayerInfoIndex index;
            if (BLACK_CM == m_CMType)
            {
                index = BLACK_SCORE;
            } 
            else
            {
                index = WHITE_SCORE;
            }

            m_callbackChangePlayerInfo(m_pGobangDlg, &index, &strInfo);
            JFSysData::GetSysData()->EndGame();
        }
    }
}

/**
* 函数功能：获取该玩家的对手所执的棋子的类型
* 参数：
*          selfType  : 自己所执的棋子的类型
*          enemyType : 对手所执的棋子的类型
* 返回值：	无
*/
void JFPlayer::GetEnemyCMType(IN const JFCMType& selfType, OUT JFCMType& enemyType)
{
    _ASSERT(NO_CM != selfType);

    enemyType = (BLACK_CM == selfType) ? WHITE_CM : BLACK_CM ;
}

/**
 * 悔棋
 */
jf_bool JFPlayer::Retract(){

	return  NULL;
}


/**
 * 求和
 */
jf_bool JFPlayer::SueForPeace(){

	return  NULL;
}


/**
* 函数功能：设置该玩家的名字
* 参数：
*          strName : 要设置的名字
* 返回值：	无
*/
void JFPlayer::SetPlayerName(const string& strName)
{
    _ASSERT(strName.size() != 0);

    m_strName = strName;
}

/**
* 函数功能：获取玩家类型
* 参数：无
* 返回值：
*          当前玩家的类型: PLAYER_HUMAN、PLAYER_COMPUTER
*/
const JFPlayerType& JFPlayer::GetPlayerType(void)
{
    _ASSERT(m_playerType != PLAYER_NO);

    return m_playerType;
}

/**
* 函数功能：获取玩家所执棋子类型
* 参数：无
* 返回值：
*          当前玩家所执棋子类型: BLACK_CM、WHITE_CM
*/
const JFCMType& JFPlayer::GetCMType(void)
{
    _ASSERT(NO_CM != m_CMType);

    return m_CMType;
}

/**
* 函数功能：获取该玩家的名字
* 参数：
*          strName : 获取的名字
* 返回值：	无
*/
void JFPlayer::GetPlayerName(string& strName)
{
    strName = m_strName;
}