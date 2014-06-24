/**********************************************************************************
 *  JFSysData.h
 *  Implementation of the Class JFSysData
 *  Created on:      26-����-2010 15:26:54
 *  Original author: Jiang Feng
 *********************************************************************************/

#include "JFPublicFunction.h"
#include "JFSysData.h"


JFSysData* JFSysData::m_pSysData = NULL;

JFSysData::JFSysData()
: m_curGameMode(HUMAN_VS_COMPUTER)  // Ĭ���˻���ս
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
* ��ʼ���������ݣ��ú�����ϵͳ��ʼ���Ϳ�ʼ�µ�һ��ʱ����
*/
void JFSysData::Initialize()
{
    // ��ʼ��ԭʼ��������
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

    // ��ʼ��������
    m_vBlackLineData.swap(vector<jf_ushort>());
    m_vWhiteLineData.swap(vector<jf_ushort>());
    for (int i=0; i<88; ++i)
    {
        m_vBlackLineData.push_back(0);
        m_vWhiteLineData.push_back(0);
    }

    // �ڰ����ӳ�ʼ��
    m_vBlackCM.swap(vector<JFChessman>());
    m_vWhiteCM.swap(vector<JFChessman>());

    // ��ǰ���µ���������
    m_curCMTypeToPlace = BLACK_CM;
}


/**
* ���ӣ�������������ӵ�ǰ���ӵ���Ϣ
*/
jf_bool JFSysData::PlaceChessman(const JFChessman& chessman)
{
    _ASSERT(NO_CM != chessman.type);
    _ASSERT(chessman.uiWeight == 0);

    jf_long32 col = chessman.position.lX;
    jf_long32 row = chessman.position.lY;

    _ASSERT(col >= 0 && col <= 14);
    _ASSERT(row >= 0 && row <= 14);

    // ��ǰҪ���ӵĵ�ȷʵ�ǿյ�
    if (NO_CM == m_vChessBoard[row][col].type)
    {
        // �����ӷ��õ����������к���������
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
    {// ��ǰ���Ѿ������ӣ��������ڴ˴�����
        return false;
    }
}

/**
* �������ܣ���ʼ��Ϸ�������������
* ��������
* ����ֵ����	
*/
void JFSysData::StartGame(void)
{
    Initialize();

    m_gameState = GAME_ON;
}

/**
* �������ܣ�������Ϸ
* ��������
* ����ֵ����	
*/
void JFSysData::EndGame(void)
{
   m_gameState = GAME_OVER; 
}

/*******************************************************************
* Get methods
*******************************************************************/

/**
* ��ȡָ������ָ��
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
* ��õ�ǰҪ�µ����ӵ�����
*/
const JFCMType& JFSysData::GetCurCMTypeToPlace() const 
{
    _ASSERT(NO_CM != m_curCMTypeToPlace);

    return  m_curCMTypeToPlace;
}



/**
* ��ȡԭʼ��������
*/
const JFChessman* JFSysData::GetCBData()
{
    _ASSERT(NULL != m_vChessBoard);

    return  &(m_vChessBoard[0][0]);
}

/**
* �������ܣ���ȡtype���͵�����������
* ������
*          type : ��������
* ����ֵ��
*          �����ݵĳ�����
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
* ��ȡ��ǰ��Ϸ�Ѷȼ���
*/
const JFGameLevel& JFSysData::GetGameLevel() const 
{
    _ASSERT(EASY_LEVEL == m_curGameLevel || HARD_LEVEL == m_curGameLevel);

    return m_curGameLevel;
}

/**
* �������ܣ���ȡ��ǰ��Ϸģʽ
* ��������
* ����ֵ����ǰ��Ϸģʽ	
*/
const JFGameMode& JFSysData::GetGameMode(void) const
{
    _ASSERT(HUMAN_VS_COMPUTER == m_curGameMode || HUMAN_VS_HUMAN == m_curGameMode);

    return m_curGameMode;
}

/**
* �������ܣ���ȡ��ǰ��Ϸ״̬
* ��������
* ����ֵ����ǰ��Ϸ״̬	
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
 * ������һ��Ҫ�µ����ӵ�����
 */
void JFSysData::SetCurCMTypeToPlace(const JFCMType type)
{
    _ASSERT(NO_CM != type);

    m_curCMTypeToPlace = type;
}


/**
* ���õ�ǰ��Ϸ�Ѷȼ���
*/
void JFSysData::SetGameLevel(const JFGameLevel& level)
{
    _ASSERT(EASY_LEVEL == level || HARD_LEVEL == level);

    m_curGameLevel = level;
}

/**
* �������ܣ����õ�ǰ��Ϸģʽ
* ������
*           mode : ��Ϸģʽ
* ����ֵ��
*           true : ���óɹ�
*           false: ���ò��ɹ�����ǰ��Ϸ�������ڽ���
*/
jf_bool JFSysData::SetGameMode(const JFGameMode& mode)
{
    _ASSERT(HUMAN_VS_HUMAN == mode || HUMAN_VS_COMPUTER == mode);

    if (GAME_ON == m_gameState)
    {// �����ǰ��Ϸ�Ѿ���ʼ������������Ϸģʽ
        return false;
    }
    else
    {
        m_curGameMode = mode;
        return true;
    }
}


/**
* �������ܣ��ж����̽ڵ�pos�ܹ�����
* ������
*          pos : ���̽ڵ�
* ����ֵ��	
*         true : pos��������
*         false: pos����������
*/
jf_bool JFSysData::IsCurPosAvailable(IN const JFPosInCB& pos)
{
    _ASSERT(pos.lX >= 0 && pos.lX <= CB_GRID_NUM
         && pos.lY >= 0 && pos.lY <= CB_GRID_NUM);

    // x���У�y����
    return (NO_CM == m_vChessBoard[pos.lY][pos.lX].type);
}