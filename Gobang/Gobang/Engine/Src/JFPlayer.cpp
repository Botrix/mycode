/**********************************************************************************
 *  JFPlayer.h
 *  Implementation of the Class JFPlayer
 *  Created on:      26-����-2010 15:26:58
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
    m_strName = "";     // string��Ĭ�Ϲ���Ϊ""
}



JFPlayer::~JFPlayer()
{

}


/**
* �������ܣ���ʼ�����
* ������
*          cmType : ��ִ��������
*          playerType : �������
* ����ֵ��	��
*/
void JFPlayer::Initialize(const JFCMType& cmType, const JFPlayerType& playerType)
{
    _ASSERT(NO_CM != cmType);
    _ASSERT(PLAYER_NO != playerType);

    m_CMType = cmType;
    m_playerType = playerType;
}

/**
* �������ܣ���ʼ��Ϸ�������������
* ��������
* ����ֵ����	
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
* �������ܣ��������Ӻ���øú���
* ������
*          pGobangDlg : �����ڵ�ָ��
*          pCMType : ��������
*          pPosInCB: �������ڵ����̽ڵ�λ��
* ����ֵ��	
*/
void JFPlayer::SetCallBackPlaceChessman(void (*callback)(void* pGobangDlg, void* pCMType, void* pPosInCB), void* pGobangDlg)
{
    _ASSERT(NULL != callback && NULL != pGobangDlg);

    m_callbackPlaceChessman = callback;
    m_pGobangDlg = pGobangDlg;
}


/**
* �������ܣ��������Ӻ���øú������ı���һ��Ҫ���ӵ����ӵ�����
* ������
*          pGobangDlg : �����ڵ�ָ��
*          pCMType : ��������
* ����ֵ��	
*/
void JFPlayer::SetCallBackChangeCMTypeToPlace(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg)
{
    _ASSERT(NULL != pGobangDlg && NULL != callback);

    m_pGobangDlg = pGobangDlg;
    m_callbackChangeCMTypeToPlace = callback;
}

/**
* �������ܣ������һ��ʤ���˼����øú���
* ������
*          pGobangDlg   : ���ڵ�ָ��
*          pCMType      : ��������
* ����ֵ��	
*/
void JFPlayer::SetCallBackWin(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg)
{
    _ASSERT(NULL != callback && NULL != pGobangDlg);

    m_callbackWin = callback;
    m_pGobangDlg = pGobangDlg;
}

/**
* �������ܣ����ָı�������Ϣ������Ϣ
* ������
*          pGobangDlg : �����ڵ�ָ��
*          pCMType : ��������
* ����ֵ��	
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
 * ����
 */
jf_bool JFPlayer::GiveIn(){

	return  NULL;
}


/**
 * �������
 * �����ǰ����������򷵻�true�����򷵻�false
 */
void JFPlayer::PlaceChessman(const JFPosInCB& position)
{
    _ASSERT(PLAYER_NO != m_playerType);
    _ASSERT(m_CMType == JFSysData::GetSysData()->GetCurCMTypeToPlace());

    JFChessman chessman;
   
    chessman.type = m_CMType;
    JFCBSituation situation;
    
	if (PLAYER_COMPUTER == m_playerType)
	{// ���ڵ���
        chessman.position = JFEngine::GetEngine()->GetBestPosForPlaceCM(m_CMType);
	}
    else
    {// ������
        chessman.position = position;
    }

    if (JFSysData::GetSysData()->IsCurPosAvailable(chessman.position))
    {
        JFSysData::GetSysData()->PlaceChessman(chessman);

        // 1��������һ��Ҫ�µ����ӵ�����
        JFCMType enemyType;
        GetEnemyCMType(m_CMType, enemyType);
        m_callbackChangeCMTypeToPlace(m_pGobangDlg, &enemyType);
        JFSysData::GetSysData()->SetCurCMTypeToPlace(enemyType);

        // 2�����߽��棬�����Ѿ�����
        m_callbackPlaceChessman(m_pGobangDlg, &m_CMType, &chessman.position);

        // 3��������ǰ��ֵ����ѣ�������֪ͨ�Է���ʤ������
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
* �������ܣ���ȡ����ҵĶ�����ִ�����ӵ�����
* ������
*          selfType  : �Լ���ִ�����ӵ�����
*          enemyType : ������ִ�����ӵ�����
* ����ֵ��	��
*/
void JFPlayer::GetEnemyCMType(IN const JFCMType& selfType, OUT JFCMType& enemyType)
{
    _ASSERT(NO_CM != selfType);

    enemyType = (BLACK_CM == selfType) ? WHITE_CM : BLACK_CM ;
}

/**
 * ����
 */
jf_bool JFPlayer::Retract(){

	return  NULL;
}


/**
 * ���
 */
jf_bool JFPlayer::SueForPeace(){

	return  NULL;
}


/**
* �������ܣ����ø���ҵ�����
* ������
*          strName : Ҫ���õ�����
* ����ֵ��	��
*/
void JFPlayer::SetPlayerName(const string& strName)
{
    _ASSERT(strName.size() != 0);

    m_strName = strName;
}

/**
* �������ܣ���ȡ�������
* ��������
* ����ֵ��
*          ��ǰ��ҵ�����: PLAYER_HUMAN��PLAYER_COMPUTER
*/
const JFPlayerType& JFPlayer::GetPlayerType(void)
{
    _ASSERT(m_playerType != PLAYER_NO);

    return m_playerType;
}

/**
* �������ܣ���ȡ�����ִ��������
* ��������
* ����ֵ��
*          ��ǰ�����ִ��������: BLACK_CM��WHITE_CM
*/
const JFCMType& JFPlayer::GetCMType(void)
{
    _ASSERT(NO_CM != m_CMType);

    return m_CMType;
}

/**
* �������ܣ���ȡ����ҵ�����
* ������
*          strName : ��ȡ������
* ����ֵ��	��
*/
void JFPlayer::GetPlayerName(string& strName)
{
    strName = m_strName;
}