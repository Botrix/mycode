/**********************************************************************************
 *  JFCore.h
 *  Implementation of the Class JFCore
 *  Created on:      26-四月-2010 15:26:56
 *  Original author: Jiang Feng
 *********************************************************************************/

#include "JFCore.h"


JFCore::JFCore()
: m_pComputer(NULL)
, m_pGobangDlg(NULL)
, m_callbackChangeCMTypeToPlace(NULL)
, m_pCompterThread(NULL)
, m_bComDoing(false)
{
    m_vpPlayer[NO_CM] = NULL;
    m_vpPlayer[BLACK_CM] = new JFPlayer;
    m_vpPlayer[BLACK_CM]->Initialize(BLACK_CM, PLAYER_HUMAN);       // 人执黑棋
    JFSysData::GetSysData()->SetCurCMTypeToPlace(BLACK_CM);
    m_vpPlayer[BLACK_CM]->SetPlayerName("姜峰");
    m_vpPlayer[WHITE_CM] = new JFPlayer;

    // 如果是人人对弈
    if (HUMAN_VS_HUMAN == JFSysData::GetSysData()->GetGameMode())
    {
        m_vpPlayer[WHITE_CM]->Initialize(WHITE_CM, PLAYER_HUMAN);    // 另一个人执黑棋
        m_vpPlayer[WHITE_CM]->SetPlayerName("黄佳");
    }
    else
    {
        m_vpPlayer[WHITE_CM]->Initialize(WHITE_CM, PLAYER_COMPUTER);    // 电脑执黑棋
        m_vpPlayer[WHITE_CM]->SetPlayerName("电脑");
        m_pComputer = m_vpPlayer[WHITE_CM];
    } 

    //InitializeCriticalSection(&m_crtSection);       // 初始化临界区
}



JFCore::~JFCore()
{
    if (m_vpPlayer[BLACK_CM] != NULL)
    {
        delete m_vpPlayer[BLACK_CM];
        m_vpPlayer[BLACK_CM] = NULL;
    }

    if (m_vpPlayer[WHITE_CM] != NULL)
    {
        delete m_vpPlayer[WHITE_CM];
        m_vpPlayer[WHITE_CM] = NULL;
    }

    if (!m_pCompterThread)
    {
        delete m_pCompterThread;
        m_pCompterThread = NULL;
    }

    //DeleteCriticalSection(&m_crtSection);       // 删除临界区
}

/**
* 函数功能：初始化数据成员，用于开局（包括开始新一局）
* 参数：无
* 返回值：无	
*/
void JFCore::Initialize(void)
{

}

/**
* 函数功能：电脑落子
* 参数：无
* 返回值：无	
*/
void JFCore::ComputerPlaceCM(void)
{
    m_pComputer->PlaceChessman(JFPosInCB(0,0));
    m_bComDoing = false;
}

/**
* 函数功能：线程过程函数
* 参数：
*          传递给线程执行函数的参数
* 返回值：	无
*/
UINT JFCore::BeginComputerThread(LPVOID pParam)
{
    JFCore* pThis = reinterpret_cast<JFCore*>(pParam);

    while (true)
    {
        pThis->ComputerPlaceCM();
        pThis->m_pCompterThread->SuspendThread();
        pThis->m_bComDoing = false;;
    }

    //LeaveCriticalSection(&pThis->m_crtSection);        // 离开临界区
    return 1;
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
void JFCore::SetCallBackPlaceChessman(void (*callback)(void* pGobangDlg, void* pCMType, void* pPosInCB), void* pGobangDlg)
{
    _ASSERT(NULL != pGobangDlg && NULL != callback);

    m_vpPlayer[BLACK_CM]->SetCallBackPlaceChessman(callback, pGobangDlg);
    m_vpPlayer[WHITE_CM]->SetCallBackPlaceChessman(callback, pGobangDlg);
}

/**
* 函数功能：棋手落子后调用该函数来改变下一个要落子的棋子的类型
* 参数：
*          pGobangDlg : 父窗口的指针
*          pCMType : 棋子类型
* 返回值：	
*/
void JFCore::SetCallBackChangeCMTypeToPlace(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg)
{
    _ASSERT(NULL != pGobangDlg && NULL != callback);

    m_pGobangDlg = pGobangDlg;
    m_callbackChangeCMTypeToPlace = callback;

    m_vpPlayer[BLACK_CM]->SetCallBackChangeCMTypeToPlace(callback, pGobangDlg);
    m_vpPlayer[WHITE_CM]->SetCallBackChangeCMTypeToPlace(callback, pGobangDlg);
}

/**
* 函数功能：如果有一方胜利了即调用该函数
* 参数：
*          pGobangDlg   : 窗口的指针
*          pCMType      : 棋子类型
* 返回值：	
*/
void JFCore::SetCallBackWin(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg)
{
    _ASSERT(NULL != pGobangDlg && NULL != callback);

    m_vpPlayer[BLACK_CM]->SetCallBackWin(callback, pGobangDlg);
    m_vpPlayer[WHITE_CM]->SetCallBackWin(callback, pGobangDlg);
}


/**
* 函数功能：棋手改变棋手信息区的信息
* 参数：
*          pGobangDlg : 父窗口的指针
*          pCMType : 棋子类型
* 返回值：	
*/
void JFCore::SetCallBackChangePlayerInfo(void (*callback)(void* pGobangDlg, void* pPlayerInfoIndex, void* pStrInfo), 
                                         void* pGobangDlg)
{
    _ASSERT(NULL != pGobangDlg && NULL != callback);

    m_vpPlayer[BLACK_CM]->SetCallBackChangePlayerInfo(callback, pGobangDlg);
    m_vpPlayer[WHITE_CM]->SetCallBackChangePlayerInfo(callback, pGobangDlg);
}
/*******************************************************************
* Callback end
*******************************************************************/



/**
 * 加载保存过的棋局文件
 */
jf_bool JFCore::LoadFile(){

	return  NULL;
}


/**
 * 加载保存过的replay文件
 */
jf_bool JFCore::LoadReplay()
{

	return  NULL;
}


/**
* 函数功能：落子
* 参数：
*          position : 放置棋子的棋盘节点位置
* 返回值：	
*          true  : position处落子成功
*          false : position处已经有棋子，不允许再放置棋子
*/
void JFCore::PlaceChessman(const JFPosInCB& position)
{
    //EnterCriticalSection(&m_crtSection);            // 进入临界区

    if (!m_bComDoing)
    {
        if (GAME_ON == JFSysData::GetSysData()->GetGameState())
        {
            const JFCMType& curTypePlace = JFSysData::GetSysData()->GetCurCMTypeToPlace();

            if (!JFSysData::GetSysData()->IsCurPosAvailable(position))
            {
                return;
            }

            m_vpPlayer[curTypePlace]->PlaceChessman(position);

            if (HUMAN_VS_COMPUTER == JFSysData::GetSysData()->GetGameMode()
                && GAME_ON == JFSysData::GetSysData()->GetGameState())
            {// 若是人机对战
                if (m_pCompterThread != NULL)
                {
                    ::ResumeThread(m_pCompterThread->m_hThread);
                    m_bComDoing = true;
                    //m_pCompterThread->ResumeThread();
                }
                else
                {
                    m_pCompterThread = AfxBeginThread(BeginComputerThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
                    ::ResumeThread(m_pCompterThread->m_hThread);
                    m_bComDoing = true;
                }
            }
        }
    }
}

/**
* 函数功能：设置当前的游戏模式
* 参数：
*           gameMode : 游戏模式
* 返回值：
*           true  : 设置成功
*           false : 设置失败
*/
jf_bool JFCore::SetCurGameMode(const JFGameMode& gameMode)
{
    return JFSysData::GetSysData()->SetGameMode(gameMode);
}

/**
* 函数功能：获取当前的游戏模式
* 参数：无
* 返回值：
*          当前对战模式
*/
const JFGameMode& JFCore::GetCurGameMode(void)
{
    return JFSysData::GetSysData()->GetGameMode();
}


/**
 * 保存棋局文件
 */
jf_bool JFCore::SaveFile(){

	return  NULL;
}


/**
 * 保存replay文件
 */
jf_bool JFCore::SaveReplay(){

	return  NULL;
}


/**
* 函数功能：开始新的一局游戏
* 参数：无
* 返回值：无	
*/
void JFCore::StartGame()
{
    m_vpPlayer[BLACK_CM]->StartGame();
    m_vpPlayer[WHITE_CM]->StartGame();
    JFSysData::GetSysData()->StartGame();
    JFEngine::GetEngine()->StartGame();

    m_curSituation.typeWin = NO_CM;
    m_curSituation.vFiveRenju.clear();

    const JFCMType& curTypePlace = JFSysData::GetSysData()->GetCurCMTypeToPlace();

    JFCMType cmType = const_cast<JFCMType>(curTypePlace);
    m_callbackChangeCMTypeToPlace(m_pGobangDlg, &cmType);
}

/**
* 结束当前游戏
*/
void JFCore::EndGame()
{
}

/**
 * 获取电脑所执的棋子的类型
 */
const JFCMType JFCore::GetCMTypeOfComputer() const 
{
    if (m_vpPlayer[BLACK_CM]->GetPlayerType() == PLAYER_COMPUTER)
    {
        return m_vpPlayer[BLACK_CM]->GetCMType();
    }
    else if (m_vpPlayer[WHITE_CM]->GetPlayerType() == PLAYER_COMPUTER)
    {
        return m_vpPlayer[WHITE_CM]->GetCMType();
    }
    else
    {
        return NO_CM;
    }
}


/**
* 函数功能：设置玩家的名字
* 参数：
*          type : 要设置的玩家所执的棋子的类型
*          strName : 要设置的名字
* 返回值：	
*/
void JFCore::SetPlayerName(IN const JFCMType& type, IN const string& strNmae)
{
    _ASSERT(type != NO_CM);

    m_vpPlayer[type]->SetPlayerName(strNmae);
}

/**
* 函数功能：获取玩家的名字
* 参数：
*          type : 要获取的玩家所执的棋子的类型
*          strName : 获取的名字
* 返回值：	
*/
void JFCore::GetPlayerName(IN const JFCMType& type, IN string& strName)
{
    _ASSERT(type != NO_CM);

    m_vpPlayer[type]->GetPlayerName(strName);
}


/**
 * 获取当前要下的棋子的类型
 */
const JFCMType& JFCore::GetCurCMTypeToPlace() const 
{
    return  JFSysData::GetSysData()->GetCurCMTypeToPlace();
}


/**
 * 获取当前棋局
 */
const JFCBSituation& JFCore::GetCurSituation()
{
    const JFCMType& typeToPlace = JFSysData::GetSysData()->GetCurCMTypeToPlace();
    JFCMType type = NO_CM;
    if (BLACK_CM == typeToPlace)
    {
        type = WHITE_CM;
    }
    else
    {
        type = BLACK_CM;
    }

    JFEngine::GetEngine()->GetCurSituation(type, m_curSituation);
    
	return  m_curSituation;
}

/**
* 设置游戏难度
*/
void JFCore::SetGameLevel(const JFGameLevel& level){

}