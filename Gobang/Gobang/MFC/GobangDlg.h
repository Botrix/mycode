// GobangDlg.h : ͷ�ļ�
//

#pragma once

#include "JFHMI.h"

#define PLAYER_INFO_NUM  7      // ������Ϣ����Ϣ����
#define BLACK_TIMER  0          // ���ӵļ�ʱ����ʶ
#define WHITE_TIMER  1          // ���ӵļ�ʱ����ʶ

// CGobangDlg �Ի���
class CGobangDlg : public CDialog
{
// ����
public:
	CGobangDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GOBANG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
public:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
    /*******************************************************************
    * ������������������ݼ�����
    *******************************************************************/

    /*******************************************
    * Callback begin
    *******************************************/
    /**
    * �������ܣ���Ӧ�ı䰴ť��״̬
    * ������
    *          pGobangDlg   : ָ��ǰ���ʵ����ָ��
    *          pBtnID       : ָ��ťID��ָ��
    *          pBtnState    : ��ť����״̬
    * ����ֵ��	��
    */
    static void OnChangeBtnState(IN void* pGobangDlg, IN void* pBtnID, IN void* pBtnState);
    void OnChangeBtnState(IN const JFBtnID& btnID, IN const JFBtnState& btnState);

    /**
    * �������ܣ�������Ӧ��������Ϣ�ĸ���
    * ������
    *          pGobangDlg  : ָ��ǰ���ʵ����ָ��
    *          playerInfoIndex : ָ��������Ϣ����Ϣ������
    *          strInfo    : ָ���µ���Ϣ��ָ��
    * ����ֵ�� ��
    */
    static void OnChangePlayerInfo(IN void* pGobangDlg, IN void* pPlayerInfoIndex, IN void* pStrInfo);
    void OnChangePlayerInfo(IN const JFPlayerInfoIndex& playerInfoIndex, IN const string& strInfo);

    /**
    * �������ܣ�������Ӧ���õ�ǰҪ���ӵ����ӵ�����
    * ������
    *          pGobangDlg  : ָ��ǰ���ʵ����ָ��
    *          pCMType     : ָ��ǰҪ���ӵ����ӵ�����
    * ����ֵ�� ��
    */
    static void OnSetCurCMTypeToPlace(IN void* pGobangDlg, IN void* pCMType);
    void OnSetCurCMTypeToPlace(IN const JFCMType& CMType);

    /**
    * �������ܣ���Ӧ��������
    * ������
    *          pGobangDlg    : ָ��ǰ���ڵ�ָ�뼴this
    *          pCMType  : ��ǰʤ�������ӵ����ͣ���ʤ�����Ƿ���������
    *          pPosInCB : ���ӷ��õ����̽ڵ�λ��
    * ����ֵ��	��
    */
    static void OnPlayerPlaceChessman(IN void* pGobangDlg, IN void* pCMType, IN void* pPosInCB);
    void OnPlayerPlaceChessman(const JFCMType& CMType, IN JFPosInCB& posInCB);

    /**
    * �������ܣ�����ʤ��ʱ���øûص�����
    * ������
    *          pGobangDlg    : ָ��ǰ���ڵ�ָ�뼴this
    *          pCMType  : ��ǰʤ�������ӵ����ͣ���ʤ�����Ƿ���������
    * ����ֵ��	��
    */
    static void OnPlayerWin(IN void* pGobangDlg, IN void* pCMType);
    void OnPlayerWin(const JFCMType& CMType);

    /**
    * �������ܣ���Ӧ���ֵ������Ȱ�ť
    * ������
    *          pGobangDlg  : ָ��ǰ���ڵ�ָ��
    *          pBtnID : ָ��ťID��ָ��
    * ����ֵ��	��
    */
    static void OnBtnClick(IN void* pGobangDlg, IN void* pBtnID);
    void OnBtnClick(IN const JFBtnID& btnID);

    /*******************************************
    * Callback end
    *******************************************/

    /*******************************************
    * paint methods begin
    *******************************************/

    /**
    * �������ܣ���������
    * ��������
    * ����ֵ����	
    */
    void OnEraseBk(void);

    /**
     * �������ܣ��ػ�
     * ��������
     * ����ֵ����	
     */
    void OnRePaint(void);

    /**
     * �������ܣ�������Դ
     * ��������
     * ����ֵ����	
     */
    void LoadResource(void);

    /**
    * �������ܣ��ڵ�point����ʾλͼbitmap
    * ������
    *          point  : �����ϵ�һ����
    *          bitmap : λͼ�ļ�
    * ����ֵ��	��
    */
    void DisplayBitmap( IN const CPoint& point, 
                        IN const CBitmap& bitmap, 
                        IN const int bmpWidth, 
                        IN const int bmpHeight);

    /**
    * �������ܣ��ڵ�point����ʾ�ı�,Ĭ�ϼӴ�
    * ������
    *          strText  : Ҫ��ʾ���ı�����
    *          iFontHeight : �����
    *          color    : �ı�����ɫ
    *          point    : �ı���ʼ��ʾ��λ��
    * ����ֵ��	��
    */
    void DisplayText(IN const string& strText, 
                    IN const int& iFontHeight,
                    IN const COLORREF& color,
                    IN const CPoint& point);

    /**
    * �������ܣ��ڵ�point����ʾһ������
    * ������
    *          point    : �����ϵ�һ����
    *          CMtype   : ��������
    * ����ֵ��	��
    */
    void DisplayCM( IN const CPoint& point, IN const JFCMType CMtype);

    /**
    * �������ܣ���ʾ��ǰ�������
    * ��������
    * ����ֵ��	��
    */
    void DisplayCurCM(void);

    /**
    * �������ܣ�������ʾ��һ���������
    * ��������
    * ����ֵ��	��
    */
    void ReDisplayLastCM(void);

    /**
    * �������ܣ���ʾ����
    * ��������
    * ����ֵ����	
    */
    void DisplayBK(void);


    /**
    * �������ܣ���ʾ���е����е�����
    * ��������
    * ����ֵ����	
    */
    void DisplayAllCM(void);

    /**
    * �������ܣ���ʾ��ť��λͼ
    * ������
    *           btnID : ��ť��ID
    * ����ֵ����	
    */
    void DisplayBtnBmp(const JFBtnID& btnID, const JFBtnState& btnState);

    /**
    * �������ܣ���ʾ��ť��text
    * ������
    *           btnID : ��ť��ID
    * ����ֵ����	
    */
    void DisplayBtnText(const JFBtnID& btnID);

    /**
    * �������ܣ���ʾ���а�ť��text
    * ��������
    * ����ֵ����	
    */
    void DisplayAllBtnText(void);

    /**
    * �������ܣ���ʾ���е�����������Ϣ
    * ��������
    * ����ֵ����	
    */
    void DisplayAllPlayerInfo(void);

    /**
     * �������ܣ�ˢ�½���
     * ��������
     * ����ֵ����	
     */
    void UpdateUI(void);

    /*******************************************
    * paint methods end
    *******************************************/


    /*******************************************
    * regular methods begin
    *******************************************/
    /**
     * �������ܣ���ʼ�µ�һ��
     * ��������
     * ����ֵ����	
     */
    void StartGame(void);

    /**
    * �������ܣ��жϵ�point�Ƿ���������
    * ������
    *          point : �û�����������ĵ�
    * ����ֵ��	
    *          true  : �û������λ����������
    *          false : �û������λ�ò��������� 
    */
    bool IsPointInCB(IN const CPoint& point) const;

    /**
    * �������ܣ���ȡ��Point���ڵ����̽ڵ�
    * ������
    *          point : �����ϵ�һ����
    * ����ֵ��	
    *          �����ڵ����̽ڵ��λ�ã�������ڰ�ť��
    *          ��x��y��Ϊ��ֵ
    */
    const JFPosInCB GetPosInCBByPoint(const CPoint& point);

    /**
    * �������ܣ��������ӽڵ��λ�û�ȡ���������ϵ�λ��
    * ������
    *          posInCB : ���̽ڵ��λ��
    * ����ֵ��	
    *          �����̽ڵ��ڽ����ϵ�����
    */
    CPoint GetPointByPosInCB(IN const JFPosInCB& posInCB);

private:

    JFHMI m_hmi;            // Human Machine Interface management
    JFCore m_core;          // ����������ӿ�

    // ״̬��
    JFRepaintElement    m_curRpElement;            // ��ǰ��Ҫ�ػ��Ԫ������
    vector<JFCMForDisp> m_vChessmanPlaced;         // ��ǰ�Ѿ��µ����ӵļ���
    vector<JFBtnID>     m_vBtnRepaint;             // ��ǰ��Ҫ�ػ水ť
    vector<pair<JFPlayerInfoIndex, string>> m_vCurRpPlayerInfo;      // ��ǰ��Ҫ�ػ�����������Ϣ,����ʱ����Ϣ
    JFCMType            m_curTypeToPlace;          // ��ǰҪ���ӵ���������

    // ��Դ
    CBitmap         m_bmpBK;                       // ����λͼ
    CBitmap         m_vBmpCM[3];                   // �ڰ�����
    CBitmap         m_vCurBmpCM[3];                // ��ǰ�µĺڰ�����λͼ
    CBitmap         m_vCurBmpCMToPlace[3];         // ��ǰҪ�µĺڰ�����λͼ
    CBitmap         m_vRegularBtnBmp[3];           // ��ͨ����̬��ť
    CBitmap         m_vCancelBtnBmp[3];            // �˳���ť����̬��ť
    CBitmap         m_vBmpPlayerInfoBK[PLAYER_INFO_NUM];         // ������Ϣ�ı���
    RECT            m_vPlayerInfoRect[PLAYER_INFO_NUM];          // ��������Ϣ��λ��
    string          m_vPlayerInfo[PLAYER_INFO_NUM];              // ������Ϣ����Ϣ

    UINT            m_uiCurTimer;                   // ��ǰ��ʱ����ֵ
    UINT            m_uiCurBlackTimeCost;           // ��ǰ�����ܹ��ĺ�ʱ
    UINT            m_uiCurWhiteTimeCost;           // ��ǰ�����ܹ��ĺ�ʱ

    // �������
    CPoint m_pointCBTopleft;        // �������Ͻ�
    CPoint m_pointBottomright;      // �������½�    
};
