/**********************************************************************************
 *  JFSysData.h
 *  Implementation of the Class JFSysData
 *  Created on:      26-四月-2010 15:26:54
 *  Original author: Jiang Feng
 *********************************************************************************/

#include "JFPublicFunction.h"
#include "JFSysData.h"


JFSysData* JFSysData::m_pSysData = NULL;

JFSysData::JFSysData()
: m_curGameMode(HUMAN_VS_COMPUTER)  // 默认人机对战
, m_gameState(GAME_OVER)
{
    Initialize();
}



JFSysData::~JFSysData()
{
    if (NULL != m_pSysData)
    {
        delete m_pSysData;
        m_pSysData = NULL;
    }
}

/**
* 初始化棋盘数据，该函数在系统初始化和开始新的一局时调用
*/
void JFSysData::Initialize()
{
    // 初始化原始棋盘数据
    for (jf_long32 row=0; row < 15; ++row)
    {
        for (jf_long32 col=0; col < 15; ++col)
        {
            m_vChessBoard[row][col].position.lX = col;
            m_vChessBoard[row][col].position.lY = row;
            m_vChessBoard[row][col].uiWeight = 0;
            m_vChessBoard[row][col].type = NO_CM;
        }
    }

    // 初始化线数据
    m_vBlackLineData.swap(vector<jf_ushort>());
    m_vWhiteLineData.swap(vector<jf_ushort>());
    for (int i=0; i<88; ++i)
    {
        m_vBlackLineData.push_back(0);
        m_vWhiteLineData.push_back(0);
    }

    // 黑白棋子初始化
    m_vBlackCM.swap(vector<JFChessman>());
    m_vWhiteCM.swap(vector<JFChessman>());

    // 当前该下的棋子类型
    m_curCMTypeToPlace = BLACK_CM;
}


/**
* 落子，即向数据中添加当前棋子的信息
*/
jf_bool JFSysData::PlaceChessman(const JFChessman& chessman)
{
    _ASSERT(NO_CM != chessman.type);
    _ASSERT(chessman.uiWeight == 0);

    jf_long32 col = chessman.position.lX;
    jf_long32 row = chessman.position.lY;

    _ASSERT(col >= 0 && col <= 14);
    _ASSERT(row >= 0 && row <= 14);

    // 当前要落子的点确实是空的
    if (NO_CM == m_vChessBoard[row][col].type)
    {
        // 将棋子放置到棋盘数据中和线数据中
        m_vChessBoard[row][col] = chessman;

        if (BLACK_CM == chessman.type)
        {
            m_vBlackCM.push_back(chessman);
            AddCMToLineData(m_vBlackLineData, chessman);
        }
        else
        {
            m_vWhiteCM.push_back(chessman);
            AddCMToLineData(m_vWhiteLineData, chessman);
        }

        return true;
    }
    else
    {// 当前点已经有棋子，不能再在此处落子
        return false;
    }
}

/**
* 函数功能：开始游戏，重置相关数据
* 参数：无
* 返回值：无	
*/
void JFSysData::StartGame(void)
{
    Initialize();

    m_gameState = GAME_ON;
}

/**
* 函数功能：结束游戏
* 参数：无
* 返回值：无	
*/
void JFSysData::EndGame(void)
{
   m_gameState = GAME_OVER; 
}

/*******************************************************************
* Get methods
*******************************************************************/

/**
* 获取指向单例的指针
*/
JFSysData* JFSysData::GetSysData()
{
    if (NULL == m_pSysData)
    {
        m_pSysData = new JFSysData;
    }

    return  m_pSysData;
}

/**
* 获得当前要下的棋子的类型
*/
const JFCMType& JFSysData::GetCurCMTypeToPlace() const 
{
    _ASSERT(NO_CM != m_curCMTypeToPlace);

    return  m_curCMTypeToPlace;
}



/**
* 获取原始棋盘数据
*/
const JFChessman* JFSysData::GetCBData()
{
    _ASSERT(NULL != m_vChessBoard);

    return  &(m_vChessBoard[0][0]);
}

/**
* 函数功能：获取type类型的棋子线数据
* 参数：
*          type : 棋子类型
* 返回值：
*          对数据的常引用
*/
const vector<jf_ushort>& JFSysData::GetLineData(IN const JFCMType& type)
{
    _ASSERT(NO_CM != type);

    if (BLACK_CM == type)
    {
        return m_vBlackLineData;
    }
    else
    {
        return m_vWhiteLineData;
    }
}


/**
* 获取当前游戏难度级别
*/
const JFGameLevel& JFSysData::GetGameLevel() const 
{
    _ASSERT(EASY_LEVEL == m_curGameLevel || HARD_LEVEL == m_curGameLevel);

    return m_curGameLevel;
}

/**
* 函数功能：获取当前游戏模式
* 参数：无
* 返回值：当前游戏模式	
*/
const JFGameMode& JFSysData::GetGameMode(void) const
{
    _ASSERT(HUMAN_VS_COMPUTER == m_curGameMode || HUMAN_VS_HUMAN == m_curGameMode);

    return m_curGameMode;
}

/**
* 函数功能：获取当前游戏状态
* 参数：无
* 返回值：当前游戏状态	
*/
const JFGameState& JFSysData::GetGameState(void) const
{
    _ASSERT(GAME_ON == m_gameState || GAME_OVER == m_gameState);

    return m_gameState;
}

/*******************************************************************
* Set methods
*******************************************************************/

/**
 * 设置下一个要下的棋子的类型
 */
void JFSysData::SetCurCMTypeToPlace(const JFCMType type)
{
    _ASSERT(NO_CM != type);

    m_curCMTypeToPlace = type;
}


/**
* 设置当前游戏难度级别
*/
void JFSysData::SetGameLevel(const JFGameLevel& level)
{
    _ASSERT(EASY_LEVEL == level || HARD_LEVEL == level);

    m_curGameLevel = level;
}

/**
* 函数功能：设置当前游戏模式
* 参数：
*           mode : 游戏模式
* 返回值：
*           true : 设置成功
*           false: 设置不成功，当前游戏可能正在进行
*/
jf_bool JFSysData::SetGameMode(const JFGameMode& mode)
{
    _ASSERT(HUMAN_VS_HUMAN == mode || HUMAN_VS_COMPUTER == mode);

    if (GAME_ON == m_gameState)
    {// 如果当前游戏已经开始则不能再设置游戏模式
        return false;
    }
    else
    {
        m_curGameMode = mode;
        return true;
    }
}


/**
* 函数功能：判断棋盘节点pos能够落子
* 参数：
*          pos : 棋盘节点
* 返回值：	
*         true : pos处可落子
*         false: pos处不可落子
*/
jf_bool JFSysData::IsCurPosAvailable(IN const JFPosInCB& pos)
{
    _ASSERT(pos.lX >= 0 && pos.lX <= CB_GRID_NUM
         && pos.lY >= 0 && pos.lY <= CB_GRID_NUM);

    // x是列，y是行
    return (NO_CM == m_vChessBoard[pos.lY][pos.lX].type);
}