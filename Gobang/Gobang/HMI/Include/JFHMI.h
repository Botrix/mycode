/** 
 * �� �� ��: JFHMI.h
 * ժ    Ҫ: ���ļ������˽��������������
 *
 * ��    ��: ����
 * ����ʱ��: 2010.05.22
 */

#ifndef JFHMI_H_
#define JFHMI_H_

#include "stdafx.h"
#include "JFStruct.h"
#include "JFCore.h"

#define BTN_WIDTH 80    // ��ť��80
#define BTN_HEIGHT 26   // ��ť��26

#define BTN_CANCEL_WIDTH 50
#define BTN_CANCEL_HEIGHT 50

#define BTN_TOPLEFT_X 230   // ���水ť�����Ͻǵ�����Ϊ��230,590��
#define BTN_TOPLEFT_Y 590

#define BTN_BETWEEN_WIDTH 28    // ��ť�����28

// ��ť˳�򣨴����ң�
// 1������
// 2������
// 3����ʼ
// 4�����
// 5������
// 6���˳�

class JFHMI
{
public:
    JFHMI();
    virtual ~JFHMI();
    /**
     * �������ܣ���ʼ��һ����Ϸ
     * ��������
     * ����ֵ����
     */
    void StartGame(void);

public:
    void OnLButtonDown(const CPoint& point);
    void OnLButtonUp(const CPoint& point);
    void OnMouseMove(const CPoint& point);

public:
    /*******************************************
    * Callback begin
    *******************************************/
    /**
     * �������ܣ�������Ӧ��ť״̬�ı�Ļص�����ָ��������ڶ����ָ��
     * ������
     *              fnCallback : �ص�����ָ��
     *              pGobangDlg : �����ڶ���ָ��
     * ����ֵ��	��
     */
    void SetCallbackBtnStateChange(void (*fnCallback)(void* pGobangDlg, void* pBtnID, void* pBtnState), void* pGobangDlg);


    /**
    * �������ܣ����ô�����ť����¼��Ļص�����ָ��������ڶ���ָ��
    * ������
    *              fnCallback : �ص�����ָ��
    *              pGobangDlg : �����ڶ���ָ��
    * ����ֵ��	��
    */
    void SetCallbackBtnClickEvent(void (*fnCallback)(void* pGobangDlg, void* pBtnID), void* pGobangDlg);

    /*******************************************
    * Callback end
    *******************************************/

    /**
     * �������ܣ���ȡ��ť��λ����Ϣ
     * ������
     *          ��
     * ����ֵ��	��ťλ����Ϣ������
     */
    const vector<RECT>& GetVBtnRect(void);

    /**
    * �������ܣ���ȡ��ť��text��Ϣ
    * ������
    *          ��
    * ����ֵ��	��ťtext��Ϣ������
    */
    const vector<string>& GetVBtnText(void);
private:
    /**
     * �������ܣ���ʼ����Դ
     * ��������
     * ����ֵ����	
     */
    void InitializeRes(void);

    /**
     * �������ܣ���ȡ��Point���ڵİ�ť
     * ������
     *          point : �����ϵ�һ����
     * ����ֵ��	
     *          �����ڵİ�ť��ID��������ڰ�ť��
     *          ��IDΪBTN_INVALID
     */
    const JFBtnID GetBtnByPoint(const CPoint& point);

private:
    
    // ��ť���
    vector<RECT>        m_vBtnRect;            // ��ťλ��
    vector<string>      m_vBtnText;            // ��ťtext
    JFBtnID             m_btnDown;             // ��ǰ���µİ�ť��ID 
    JFBtnID             m_btnHighlight;        // ��ǰ���ڸ����İ�ť��ID 
    JFBtnState          m_btnState;            // ��ǰ�ļ���İ�ť��״̬
    void*               m_pGobangDlg;          // CGobangDlg pointer

    // callback function pointer
    // ��Ӧ��ť״̬�ı�
    void (*m_callbackBtnStateChange)(void* pGobangDlg, void* pBtnID, void* pBtnState);

    // ������ť����¼�
    void (*m_callbackBtnClickEvent)(void* pGobangDlg, void* pBtnID);

};

#endif