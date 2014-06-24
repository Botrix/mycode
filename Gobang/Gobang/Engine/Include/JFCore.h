/**********************************************************************************
*  JFCore.h
*  Implementation of the Class JFCore
*  Created on:      26-����-2010 15:26:55
*  Original author: Jiang Feng
*********************************************************************************/

#if !defined(EA_D6278271_629D_45e9_AAB2_983D99E43C97__INCLUDED_)
#define EA_D6278271_629D_45e9_AAB2_983D99E43C97__INCLUDED_

#include "stdafx.h"
#include "JFStruct.h"
#include "JFPlayer.h"

/**
* JFCore��������MFC��JFEngine��JFSysData�Ľӿڣ��κβ�����Ϣ��Ҫ��������൱�������²�ṹ�����ϡ�
*/
class JFCore
{

public:
    JFCore();
    virtual ~JFCore();

    /**
     * �������ܣ���ʼ�����ݳ�Ա�����ڿ��֣�������ʼ��һ�֣�
     * ��������
     * ����ֵ����	
     */
    void Initialize(void);

    /**
     * �������ܣ���������
     * ��������
     * ����ֵ����	
     */
    void ComputerPlaceCM(void);

    /**
     * �������ܣ��̹߳��̺���
     * ������
     *          ���ݸ��߳�ִ�к����Ĳ���
     * ����ֵ��	��
     */
    static UINT BeginComputerThread(LPVOID pParam);

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
    void SetCallBackPlaceChessman(void (*callback)(void* pGobangDlg, void* pCMType, void* pPosInCB), void* pGobangDlg);

    /**
    * �������ܣ��������Ӻ���øú������ı���һ��Ҫ���ӵ����ӵ�����
    * ������
    *          pGobangDlg : �����ڵ�ָ��
    *          pCMType : ��������
    * ����ֵ��	
    */
    void SetCallBackChangeCMTypeToPlace(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg);

    /**
    * �������ܣ����ָı�������Ϣ������Ϣ
    * ������
    *          pGobangDlg : �����ڵ�ָ��
    *          pCMType : ��������
    * ����ֵ��	
    */
    void SetCallBackChangePlayerInfo(void (*callback)(void* pGobangDlg, void* pPlayerInfoIndex, void* pStrInfo), void* pGobangDlg);

    /**
    * �������ܣ������һ��ʤ���˼����øú���
    * ������
    *          pGobangDlg : �����ڵ�ָ��
    *          pCMType : ��������
    * ����ֵ��	
    */
    void SetCallBackWin(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg);

    /*******************************************************************
    * Callback end
    *******************************************************************/

    /*******************************************************************
    * JFPlayer begin
    *******************************************************************/
    /**
    * �������ܣ�����
    * ������
    *          position : �������ӵ����̽ڵ�λ��
    * ����ֵ��	
    *          true  : position�����ӳɹ�
    *          false : position���Ѿ������ӣ��������ٷ�������
    */
    void PlaceChessman(const JFPosInCB& position);

    /**
    * ��ȡ������ִ�����ӵ�����
    */
    const JFCMType GetCMTypeOfComputer() const;

    /**
     * �������ܣ�������ҵ�����
     * ������
     *          type : Ҫ���õ������ִ�����ӵ�����
     *          strName : Ҫ���õ�����
     * ����ֵ��	
     */
    void SetPlayerName(IN const JFCMType& type, IN const string& strNmae);

    /**
    * �������ܣ���ȡ��ҵ�����
    * ������
    *          type : Ҫ��ȡ�������ִ�����ӵ�����
    *          strName : ��ȡ������
    * ����ֵ��	
    */
    void GetPlayerName(IN const JFCMType& type, IN string& strName);

    /*******************************************************************
    * JFPlayer end
    *******************************************************************/

    /*******************************************************************
    * JFSysData start
    *******************************************************************/

    /**
    * �������ܣ����õ�ǰ����Ϸģʽ
    * ������
    *           gameMode : ��Ϸģʽ
    * ����ֵ��
    *           true  : ���óɹ�
    *           false : ����ʧ��
    */
    jf_bool SetCurGameMode(const JFGameMode& gameMode);

    /**
    * �������ܣ���ȡ��ǰ����Ϸģʽ
    * ��������
    * ����ֵ��
    *          ��ǰ��սģʽ
    */
    const JFGameMode& GetCurGameMode(void);

    /**
    * ��ȡ��ǰҪ�µ����ӵ�����
    */
    const JFCMType& GetCurCMTypeToPlace() const;

    /**
    * ������Ϸ�Ѷ�
    */
    void SetGameLevel(const JFGameLevel& level);

    /*******************************************************************
    * JFSysData end
    *******************************************************************/

    /*******************************************************************
    * JFEngine start
    *******************************************************************/

    /**
    * ��ȡ��ǰ���
    */
    const JFCBSituation& GetCurSituation(); 

    /*******************************************************************
    * JFEngine end
    *******************************************************************/

    /**
    * ���ر����������ļ�
    */
    jf_bool LoadFile();
    /**
    * ���ر������replay�ļ�
    */
    jf_bool LoadReplay();

    /**
    * ��������ļ�
    */
    jf_bool SaveFile();
    /**
    * ����replay�ļ�
    */
    jf_bool SaveReplay();

    /**
    * �������ܣ���ʼ��Ϸ�������������
    * ��������
    * ����ֵ����	
    */
    void StartGame(void);

    /**
    * �������ܣ�������Ϸ
    * ��������
    * ����ֵ����	
    */
    void EndGame();

public:
        //CRITICAL_SECTION m_crtSection;          // �ٽ���
    CWinThread* m_pCompterThread;           // ���������߳�
    jf_bool m_bComDoing;                       // ������������
private:

    // �洢ָ�����ֶ����ָ��
    // [0]:NO_CM 
    // [1]: BLACK_CM ִ����
    // [2]: WHITE_CM ִ����
    JFPlayer* m_vpPlayer[3]; 
    JFPlayer* m_pComputer;   // ָ�����

    void*     m_pGobangDlg;  // ָ�������ڶ����ָ��
    void (*m_callbackChangeCMTypeToPlace)(void* pGobangDlg, void* pCMType);      // ���Ըı䵱ǰҪ���ӵ��������͵ĺ���ָ��

    
     
    JFCBSituation m_curSituation;           // ��ǰ���
};
#endif // !defined(EA_D6278271_629D_45e9_AAB2_983D99E43C97__INCLUDED_)
