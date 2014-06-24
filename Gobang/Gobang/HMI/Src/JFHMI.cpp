/** 
* �� �� ��: JFHMI.cpp
* ժ    Ҫ: ���ļ������˽��������������
*
* ��    ��: ����
* ����ʱ��: 2010.05.22
*/

#include "JFHMI.h"

JFHMI::JFHMI()
: m_btnDown(BTN_INVALID)
, m_btnHighlight(BTN_INVALID)
, m_pGobangDlg(NULL)
, m_btnState(BTN_STATE_INVALID)
{
    InitializeRes();
}


JFHMI::~JFHMI()
{

}

/**
* �������ܣ���ʼ��һ����Ϸ
* ��������
* ����ֵ����
*/
void JFHMI::StartGame(void)
{
    m_btnDown = BTN_INVALID;
    m_btnHighlight = BTN_INVALID;
    m_btnState = BTN_STATE_INVALID;
}


void JFHMI::OnLButtonDown(const CPoint& point)
{
    JFBtnID btnID = GetBtnByPoint(point);

    if (BTN_INVALID != btnID)
    {
        m_btnDown = btnID;        // ��¼��ǰ���ڷǳ�̬�İ�ť��ID

        m_btnState = BTN_STATE_DOWN;
        m_callbackBtnStateChange(m_pGobangDlg, &m_btnDown, &m_btnState);
        m_btnState = BTN_STATE_INVALID;
    }
}

void JFHMI::OnLButtonUp(const CPoint& point)
{
    JFBtnID btnID = GetBtnByPoint(point);

    if (BTN_INVALID != btnID)
    {
        m_btnDown = btnID;        // ��¼��ǰ���ڷǳ�̬�İ�ť��ID

        m_btnState = BTN_STATE_HIGHLIGHT;
        m_callbackBtnStateChange(m_pGobangDlg, &m_btnDown, &m_btnState);
        m_btnState = BTN_STATE_INVALID;
        m_btnHighlight = btnID;
    }

    // ���Up�İ�ť�Ǳ�Down�İ�ť����ô�������ð�ť��Ӧ���¼�
    if (btnID == m_btnDown)
    {
        m_callbackBtnClickEvent(m_pGobangDlg, &m_btnDown);
        m_btnDown = BTN_INVALID;
    }
}

void JFHMI::OnMouseMove(const CPoint& point)
{
    JFBtnID btnID = GetBtnByPoint(point);

    // 1����ǰ�а�ť���ڸ���
    if (BTN_INVALID != m_btnHighlight)
    {
        // 1.1 ��ǰ��ť���Ǵ��ڸ����İ�ť
        if (btnID == m_btnHighlight)
        {
            // ��������
        }
        // 1.2 ��ǰ��ť���ǵ�ǰ�����ڸ���̬�İ�ť
        else
        {
            // 1.2.1 �ָ��������Ǹ���ť��״̬
            m_btnState = BTN_STATE_NORMAL;
            m_callbackBtnStateChange(m_pGobangDlg, &m_btnHighlight, &m_btnState);
            m_btnState = BTN_STATE_INVALID;
            m_btnHighlight = BTN_INVALID;

            // 1.2.2 ����ǰ��ť����Ϊ����̬
            if (BTN_INVALID != btnID)
            {
                m_btnHighlight = btnID;
                m_btnState = BTN_STATE_HIGHLIGHT;
                m_callbackBtnStateChange(m_pGobangDlg, &m_btnHighlight, &m_btnState);  
                m_btnState = BTN_STATE_INVALID;
            }
        } // end else
    }// end if
    // 2����ǰû�а�ť���ڸ���̬
    else
    {
        if (BTN_INVALID == btnID)
        {
            // ��ǰ�������κΰ�ť��
        }
        else
        {
            m_btnHighlight = btnID;
            m_btnState = BTN_STATE_HIGHLIGHT;
            m_callbackBtnStateChange(m_pGobangDlg, &m_btnHighlight, &m_btnState); 
            m_btnState = BTN_STATE_INVALID;
        }
    }
}

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
void JFHMI::SetCallbackBtnStateChange(void (*fnCallback)(void* pGobangDlg, void* pBtnID, void* pBtnState), 
                                      void* pGobangDlg)
{
    _ASSERT(NULL != fnCallback && NULL != pGobangDlg);

    m_callbackBtnStateChange = fnCallback;
    m_pGobangDlg = pGobangDlg;
}

/**
* �������ܣ����ô�����ť����¼��Ļص�����ָ��������ڶ���ָ��
* ������
*              fnCallback : �ص�����ָ��
*              pGobangDlg : �����ڶ���ָ��
* ����ֵ��	��
*/
void JFHMI::SetCallbackBtnClickEvent(void (*fnCallback)(void* pGobangDlg, void* pBtnID), void* pGobangDlg)
{
    _ASSERT(NULL != fnCallback && NULL != pGobangDlg);

    m_callbackBtnClickEvent = fnCallback;
    m_pGobangDlg = pGobangDlg;
}

/*******************************************
* Callback end
*******************************************/
/**
* �������ܣ���ȡ��ť��λ����Ϣ
* ������
*          ��
* ����ֵ��	��ťλ����Ϣ������
*/
const vector<RECT>& JFHMI::GetVBtnRect(void)
{
    return m_vBtnRect;
}

/**
* �������ܣ���ȡ��ť��text��Ϣ
* ������
*          ��
* ����ֵ��	��ťtext��Ϣ������
*/
const vector<string>& JFHMI::GetVBtnText(void)
{
    return m_vBtnText;
}

/**
* �������ܣ���ʼ����Դ
* ��������
* ����ֵ����	
*/
void JFHMI::InitializeRes(void)
{
    // ��ʼ����ťλ��
    // 5�����ܰ�ť
    RECT rect;
    rect.left = BTN_TOPLEFT_X;
    rect.top  = BTN_TOPLEFT_Y;
    rect.right= BTN_TOPLEFT_X + BTN_WIDTH;
    rect.bottom = BTN_TOPLEFT_Y + BTN_HEIGHT;

    m_vBtnRect.push_back(rect);
    // �����ĸ�
    for (int i=0; i<4; ++i)
    {
        rect.left = rect.right + BTN_BETWEEN_WIDTH;
        rect.right = rect.left + BTN_WIDTH;

        m_vBtnRect.push_back(rect);
    }

    m_vBtnText.push_back("����");
    m_vBtnText.push_back("����");
    m_vBtnText.push_back("��ʼ");
    m_vBtnText.push_back("���");
    m_vBtnText.push_back("����");

    // �˳���ť
    rect.left = 840;
    rect.top = 580;
    rect.right = 890;
    rect.bottom = 630;

    m_vBtnRect.push_back(rect);
    m_vBtnText.push_back("�˳�");
}


/**
* �������ܣ���ȡ��Point���ڵİ�ť
* ������
*          point : �����ϵ�һ����
* ����ֵ��	
*          �����ڵİ�ť��ID��������ڰ�ť��
*          ��IDΪBTN_INVALID
*/
const JFBtnID JFHMI::GetBtnByPoint(const CPoint& point)
{
    JFBtnID index = BTN_INVALID; // ��ťID

    size_t uiRectSize = m_vBtnRect.size();
    for (size_t ui=0; ui<uiRectSize; ++ui)
    {
        const RECT& rect = m_vBtnRect[ui]; 

        if (point.x >= rect.left
            && point.x <= rect.right
            && point.y >= rect.top
            && point.y <= rect.bottom)
        {
            index = JFBtnID(ui);
        }
    }

    return index;
}
