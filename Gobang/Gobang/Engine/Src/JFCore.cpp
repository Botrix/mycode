/**********************************************************************************
 *  JFCore.h
 *  Implementation of the Class JFCore
 *  Created on:      26-����-2010 15:26:56
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
    m_vpPlayer[BLACK_CM]->Initialize(BLACK_CM, PLAYER_HUMAN);       // ��ִ����
    JFSysData::GetSysData()->SetCurCMTypeToPlace(BLACK_CM);
    m_vpPlayer[BLACK_CM]->SetPlayerName("����");
    m_vpPlayer[WHITE_CM] = new JFPlayer;

    // ��������˶���
    if (HUMAN_VS_HUMAN == JFSysData::GetSysData()->GetGameMode())
    {
        m_vpPlayer[WHITE_CM]->Initialize(WHITE_CM, PLAYER_HUMAN);    // ��һ����ִ����
        m_vpPlayer[WHITE_CM]->SetPlayerName("�Ƽ�");
    }
    else
    {
        m_vpPlayer[WHITE_CM]->Initialize(WHITE_CM, PLAYER_COMPUTER);    // ����ִ����
        m_vpPlayer[WHITE_CM]->SetPlayerName("����");
        m_pComputer = m_vpPlayer[WHITE_CM];
    } 

    //InitializeCriticalSection(&m_crtSection);       // ��ʼ���ٽ���
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

    //DeleteCriticalSection(&m_crtSection);       // ɾ���ٽ���
}

/**
* �������ܣ���ʼ�����ݳ�Ա�����ڿ��֣�������ʼ��һ�֣�
* ��������
* ����ֵ����	
*/
void JFCore::Initialize(void)
{

}

/**
* �������ܣ���������
* ��������
* ����ֵ����	
*/
void JFCore::ComputerPlaceCM(void)
{
    m_pComputer->PlaceChessman(JFPosInCB(0,0));
    m_bComDoing = false;
}

/**
* �������ܣ��̹߳��̺���
* ������
*          ���ݸ��߳�ִ�к����Ĳ���
* ����ֵ��	��
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

    //LeaveCriticalSection(&pThis->m_crtSection);        // �뿪�ٽ���
    return 1;
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
void JFCore::SetCallBackPlaceChessman(void (*callback)(void* pGobangDlg, void* pCMType, void* pPosInCB), void* pGobangDlg)
{
    _ASSERT(NULL != pGobangDlg && NULL != callback);

    m_vpPlayer[BLACK_CM]->SetCallBackPlaceChessman(callback, pGobangDlg);
    m_vpPlayer[WHITE_CM]->SetCallBackPlaceChessman(callback, pGobangDlg);
}

/**
* �������ܣ��������Ӻ���øú������ı���һ��Ҫ���ӵ����ӵ�����
* ������
*          pGobangDlg : �����ڵ�ָ��
*          pCMType : ��������
* ����ֵ��	
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
* �������ܣ������һ��ʤ���˼����øú���
* ������
*          pGobangDlg   : ���ڵ�ָ��
*          pCMType      : ��������
* ����ֵ��	
*/
void JFCore::SetCallBackWin(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg)
{
    _ASSERT(NULL != pGobangDlg && NULL != callback);

    m_vpPlayer[BLACK_CM]->SetCallBackWin(callback, pGobangDlg);
    m_vpPlayer[WHITE_CM]->SetCallBackWin(callback, pGobangDlg);
}


/**
* �������ܣ����ָı�������Ϣ������Ϣ
* ������
*          pGobangDlg : �����ڵ�ָ��
*          pCMType : ��������
* ����ֵ��	
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
 * ���ر����������ļ�
 */
jf_bool JFCore::LoadFile(){

	return  NULL;
}


/**
 * ���ر������replay�ļ�
 */
jf_bool JFCore::LoadReplay()
{

	return  NULL;
}


/**
* �������ܣ�����
* ������
*          position : �������ӵ����̽ڵ�λ��
* ����ֵ��	
*          true  : position�����ӳɹ�
*          false : position���Ѿ������ӣ��������ٷ�������
*/
void JFCore::PlaceChessman(const JFPosInCB& position)
{
    //EnterCriticalSection(&m_crtSection);            // �����ٽ���

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
            {// �����˻���ս
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
* �������ܣ����õ�ǰ����Ϸģʽ
* ������
*           gameMode : ��Ϸģʽ
* ����ֵ��
*           true  : ���óɹ�
*           false : ����ʧ��
*/
jf_bool JFCore::SetCurGameMode(const JFGameMode& gameMode)
{
    return JFSysData::GetSysData()->SetGameMode(gameMode);
}

/**
* �������ܣ���ȡ��ǰ����Ϸģʽ
* ��������
* ����ֵ��
*          ��ǰ��սģʽ
*/
const JFGameMode& JFCore::GetCurGameMode(void)
{
    return JFSysData::GetSysData()->GetGameMode();
}


/**
 * ��������ļ�
 */
jf_bool JFCore::SaveFile(){

	return  NULL;
}


/**
 * ����replay�ļ�
 */
jf_bool JFCore::SaveReplay(){

	return  NULL;
}


/**
* �������ܣ���ʼ�µ�һ����Ϸ
* ��������
* ����ֵ����	
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
* ������ǰ��Ϸ
*/
void JFCore::EndGame()
{
}

/**
 * ��ȡ������ִ�����ӵ�����
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
* �������ܣ�������ҵ�����
* ������
*          type : Ҫ���õ������ִ�����ӵ�����
*          strName : Ҫ���õ�����
* ����ֵ��	
*/
void JFCore::SetPlayerName(IN const JFCMType& type, IN const string& strNmae)
{
    _ASSERT(type != NO_CM);

    m_vpPlayer[type]->SetPlayerName(strNmae);
}

/**
* �������ܣ���ȡ��ҵ�����
* ������
*          type : Ҫ��ȡ�������ִ�����ӵ�����
*          strName : ��ȡ������
* ����ֵ��	
*/
void JFCore::GetPlayerName(IN const JFCMType& type, IN string& strName)
{
    _ASSERT(type != NO_CM);

    m_vpPlayer[type]->GetPlayerName(strName);
}


/**
 * ��ȡ��ǰҪ�µ����ӵ�����
 */
const JFCMType& JFCore::GetCurCMTypeToPlace() const 
{
    return  JFSysData::GetSysData()->GetCurCMTypeToPlace();
}


/**
 * ��ȡ��ǰ���
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
* ������Ϸ�Ѷ�
*/
void JFCore::SetGameLevel(const JFGameLevel& level){

}