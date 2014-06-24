/**********************************************************************************
 *  JFPlayer.h
 *  Implementation of the Class JFPlayer
 *  Created on:      26-����-2010 15:26:58
 *  Original author: Jiang Feng
 *********************************************************************************/

#if !defined(EA_55EE549B_34E6_43c6_BED7_118D3B53BBD0__INCLUDED_)
#define EA_55EE549B_34E6_43c6_BED7_118D3B53BBD0__INCLUDED_

#include "JFStruct.h"
#include "JFEngine.h"

/**
 * �����
 * ���Ժ����־��Ǹ���Ķ���
 */
class JFPlayer
{

public:
	JFPlayer();
	virtual ~JFPlayer();

    /**
     * �������ܣ���ʼ�����
     * ������
     *          cmType : ��ִ��������
     *          playerType : �������
     * ����ֵ��	��
     */
    void Initialize(const JFCMType& cmType, const JFPlayerType& playerType);

    /**
    * �������ܣ���ʼ��Ϸ�������������
    * ��������
    * ����ֵ����	
    */
    void StartGame(void);

    /*******************************************************************
    * Callback begin
    *******************************************************************/

    /**
    * �������ܣ��������Ӻ���øú���
    * ������
    *          pGobangDlg : �����ڵ�ָ��
    *          pCMType    : ��������
    *          pPosInCB   : �������ڵ����̽ڵ�λ��
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
    * �������ܣ������һ��ʤ���˼����øú���
    * ������
    *          pGobangDlg   : ���ڵ�ָ��
    *          pCMType      : ��������
    * ����ֵ��	
    */
    void SetCallBackWin(void (*callback)(void* pGobangDlg, void* pCMType), void* pGobangDlg);

    /**
    * �������ܣ����ָı�������Ϣ������Ϣ
    * ������
    *          pGobangDlg : �����ڵ�ָ��
    *          pCMType : ��������
    * ����ֵ��	
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
     * �������ܣ����ø���ҵ�����
     * ������
     *          strName : Ҫ���õ�����
     * ����ֵ��	��
     */
    void SetPlayerName(const string& strName);

    /*******************************************************************
    * get methods
    *******************************************************************/
    /**
     * �������ܣ���ȡ�������
     * ��������
     * ����ֵ��
     *          ��ǰ��ҵ�����: PLAYER_HUMAN��PLAYER_COMPUTER
     */
    const JFPlayerType& GetPlayerType(void);

    /**
    * �������ܣ���ȡ�����ִ��������
    * ��������
    * ����ֵ��
    *          ��ǰ�����ִ��������: BLACK_CM��WHITE_CM
    */
    const JFCMType& GetCMType(void);

    /**
    * �������ܣ���ȡ����ҵ�����
    * ������
    *          strName : ��ȡ������
    * ����ֵ��	��
    */
    void GetPlayerName(string& strName);

    /*******************************************************************
     * ��ҵĲ���
     *******************************************************************/
	/**
	 * ����
	 */
	virtual jf_bool GiveIn();
	/**
	 * �������
	 */
	virtual void PlaceChessman(const JFPosInCB& position);
	/**
	 * ����
	 */
	virtual jf_bool Retract();
	/**
	 * ���
	 */
	virtual jf_bool SueForPeace();
private:
    /**
     * �������ܣ���ȡ����ҵĶ�����ִ�����ӵ�����
     * ������
     *          selfType  : �Լ���ִ�����ӵ�����
     *          enemyType : ������ִ�����ӵ�����
     * ����ֵ��	��
     */
    inline void GetEnemyCMType(IN const JFCMType& selfType, OUT JFCMType& enemyType);

private:
	/**
	 * �������ִ�����ӵ���ɫ
	 * ִ���������ӣ��б���������������
	 */
	JFCMType m_CMType;

    /** 
     * ��ǰ��ҵ�����
     */
    JFPlayerType m_playerType;

	/**
	 * ��ҵ�����
	 */
	jf_string m_strName;

    void (*m_callbackPlaceChessman)(void* pParent, void* pCMType, void* pPosInCB);  // �ص����������Ӻ����
    void (*m_callbackChangeCMTypeToPlace)(void* pGobangDlg, void* pCMType);         // ���Ըı䵱ǰҪ���ӵ��������͵ĺ���ָ��
    void (*m_callbackWin)(void* pGobangDlg, void* pCMType);                         // �ص�������ʤ��ʱ����
    void (*m_callbackChangePlayerInfo)(void* pGobangDlg, void* pPlayerInfoIndex, void* pStrInfo);   // �ı�������Ϣ

    void* m_pGobangDlg;                                                             // ָ�򴰿ڵ�ָ��

	/**
	 * ����ҵ�ĿǰΪֹʤ������
	 */
	jf_uint m_uiWinGame;

//     /**
//     * ����ҵ�ĿǰΪֹʧ�ܳ���
//     */
//     jf_uint m_uiLostGame;
// 
//     /**
//     * ����ҵ�ĿǰΪֹƽ����
//     */
//     jf_uint m_uiPeaceGame;

};
#endif // !defined(EA_55EE549B_34E6_43c6_BED7_118D3B53BBD0__INCLUDED_)
