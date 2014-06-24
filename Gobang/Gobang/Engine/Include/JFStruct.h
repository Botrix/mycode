/**********************************************************************************
*  JFStruct.h
*  ���ļ��������õ��Ľṹ��Ķ���
*  Created on:      18-����-2010 12:03:04
*  Original author: Jiang Feng
*********************************************************************************/

#ifndef JFSTRUCT_H_ 
#define JFSTRUCT_H_

#include "JFType.h"

/**
* JFPosInCB (CB->ChessBoard)
* �����ϵ��������ڵ�λ�ýṹ
*/
struct JFPosInCB
{

public:
    /**
    * ��������0��ʼ
    */
    jf_long32 lX;
    /**
    * ��������0��ʼ
    */
    jf_long32 lY;

public:
    // constructor
    JFPosInCB()
        : lX(0)
        , lY(0)
    {
    }

    JFPosInCB(jf_long32 x, jf_long32 y)
    {
        lX = x;
        lY = y;
    }

    JFPosInCB(const JFPosInCB& point)
    {
        memcpy(this, &point, sizeof(JFPosInCB));
    }

    // assignment operator
    JFPosInCB& operator=(const JFPosInCB& point)
    {
        memcpy(this, &point, sizeof(JFPosInCB));

        return *this;
    }
};


/**
* �����ϵ����ӽڵ������
* 1��������
* 2������
* 3������
*/
enum JFCMType
{
    /**
    * ���̽ڵ���û������
    */
    NO_CM = 0,
    /**
    * ���̽ڵ��ϵ�����Ϊ����
    */
    BLACK_CM = 1,
    /**
    * ���̽ڵ��ϵ�����Ϊ����
    */
    WHITE_CM = 2
};

/**
* �������
* 1��������
* 2������
* 3����
*/
enum JFPlayerType
{
    /** 
     * ������
     */
    PLAYER_NO = 0, 

    /**
     * ��
     */
    PLAYER_HUMAN = 1,

    /**
     * ����
     */
    PLAYER_COMPUTER = 2
};

/**
* ��������ṹ���ýṹΪ����
* ��ˣ�ֻ�����ϽǺ����½�������Ա
*/
struct JFCBRect
{

public:
    /**
    * ��������Ͻ�
    */
    JFPosInCB topleftPos;
    /**
    * ��������½�
    */
    JFPosInCB bottomrightPos;

public:
    // constructor
    JFCBRect()
        : topleftPos(JFPosInCB())
        , bottomrightPos(JFPosInCB())
    {

    }

    JFCBRect(jf_long32 topX, jf_long32 topY, jf_long32 bottomX, jf_long32 bottomY)
    {
        topleftPos.lX = topX;
        topleftPos.lY = topY;
        bottomrightPos.lX = bottomX;
        bottomrightPos.lY = bottomY;
    }

    JFCBRect(const JFCBRect& rect)
    {
        memcpy(this, &rect, sizeof(JFCBRect));
    }

    // assignment operator
    JFCBRect& operator=(const JFCBRect& rect)
    {
        memcpy(this, &rect, sizeof(JFCBRect));

        return *this;
    }

};

/**
* ���������е���������
*/
struct JFChessman
{

public:
    /**
    * �����������ϵ�λ��
    */
    JFPosInCB position;
    /**
    * ���̽ڵ��ϵ����ӵ�����
    */
    JFCMType type;
    /**
    * �����ӵ�Ȩֵ��������ѵ�ʱʹ��
    */
    jf_uint uiWeight;

public:
    // constructor
    JFChessman()
        : position(0, 0)
        , type(NO_CM)
        , uiWeight(0)
    {
    }

    JFChessman(jf_long32 x, jf_long32 y, JFCMType _type, jf_uint _weight)
    {
        position.lX = x;
        position.lY = y;
        type   = _type;
        uiWeight  = _weight;
    }

    JFChessman(const JFChessman& node)
    {
        memcpy(this, &node, sizeof(JFChessman));
    }

    // assignment operator
    JFChessman& operator=(const JFChessman& node)
    {
        memcpy(this, &node, sizeof(JFChessman));

        return *this;
    }

    // ����sort����Ȩֵ�Ӵ�С����
    static jf_bool WeightLarger(const JFChessman& _left, const JFChessman& _right)
    {
        return (_left.uiWeight > _right.uiWeight);
    }
};


/**
* ���̽����ϵ���������
*/
struct JFCMForDisp
{

public:
    /**
    * �����������ϵ�x����
    */
    jf_long32 lx;

    /**
    * �����������ϵ�y����
    */
    jf_long32 ly;

    /**
    * ���ӵ�����
    */
    JFCMType type;

public:
    // constructor
    JFCMForDisp()
        : lx(0)
        , ly(0)
        , type(NO_CM)
    {
    }

    JFCMForDisp(jf_long32 x, jf_long32 y, JFCMType type)
    {
        lx = x;
        ly = y;
        type   = NO_CM;
    }

    JFCMForDisp(const JFCMForDisp& node)
    {
        memcpy(this, &node, sizeof(JFCMForDisp));
    }

    // assignment operator
    JFCMForDisp& operator=(const JFCMForDisp& node)
    {
        memcpy(this, &node, sizeof(JFCMForDisp));

        return *this;
    }

    // compare operator
    bool operator==(const JFCMForDisp& node)
    {
        return (this->lx == node.lx
                && this->ly == node.ly
                && this->type == node.type);
    }
};


/** 
 * ��������
 */
struct JFFiveRenju
{
public:
    JFChessman fiveChessman[5];
public:
    JFFiveRenju()
    {
        JFChessman chessman;

        for (jf_int32 i=0; i<5; ++i)
        {
            memcpy((fiveChessman+i), &chessman, sizeof(JFChessman));
        }
    }

    JFFiveRenju(const JFFiveRenju& fiveRenju)
    {
        memcpy(this, &fiveRenju, sizeof(JFFiveRenju));
    }

    JFFiveRenju& operator=(const JFFiveRenju& fiveRenju)
    {
        memcpy(this, &fiveRenju, sizeof(JFFiveRenju));

        return *this;
    }
};

/**
* ���
*/
struct JFCBSituation
{
public:
    JFCMType typeWin;                   // ��ǰӮ�����ӵ�����       
    vector<JFFiveRenju> vFiveRenju;     // ��ǰ������������Ӽ��ϣ����ܲ�ֹ5����

public:
    JFCBSituation()
        : typeWin(NO_CM)
    {
    }

    JFCBSituation(const JFCBSituation& situation)
    {
        this->typeWin = situation.typeWin;
        this->vFiveRenju.assign(situation.vFiveRenju.begin(), situation.vFiveRenju.end());
    }

    JFCBSituation& operator=(const JFCBSituation& situation)
    {
        this->typeWin = situation.typeWin;
        this->vFiveRenju.assign(situation.vFiveRenju.begin(), situation.vFiveRenju.end());

        return *this;
    }
};

enum JFGameLevel
{
    EASY_LEVEL = 1,
    HARD_LEVEL = 2
};

enum JFDirection
{
    DIRECTION_NORTH = 0,            // ��
    DIRECTION_NORTH_EAST = 1,       // ����
    DIRECTION_EAST = 2,             // ��
    DIRECTION_SOUTH_EAST = 3,       // ����
    DIRECTION_SOUTH = 4,            // ��
    DIRECTION_SOUTH_WEST = 5,       // ����
    DIRECTION_WEST = 6,             // ��
    DIRECTION_NORTH_WEST = 7        // ����
};


// ��Ϸģʽ
// 1�����˶�ս
// 2���˻���ս
enum JFGameMode
{
    HUMAN_VS_HUMAN = 0,     // ����
    HUMAN_VS_COMPUTER = 1,  // �˻�
};

// ��Ϸ��ʾ
enum JFTips
{
    BLACK_WIN = 0,      // ����Ӯ
    WHITE_WIN = 1,      // ����Ӯ
};

// ��Ϸ״̬
enum JFGameState
{
    GAME_ON = 0,
    GAME_OVER = 1,
};

// �����ϵİ�ťIDö��
enum JFBtnID
{
    BTN_SAVE    = 0,    // ����
    BTN_LOAD    = 1,    // ����
    BTN_START   = 2,    // ��ʼ
    BTN_PEACE   = 3,    // ���
    BTN_GIVEIN  = 4,    // ����
    BTN_CANCEL  = 5,    // �˳�
    BTN_INVALID = 6,    // �Ƿ���ť
};

// ��ť����̬
enum JFBtnState
{
    BTN_STATE_NORMAL = 0,       // ��̬
    BTN_STATE_HIGHLIGHT = 1,    // ����
    BTN_STATE_DOWN = 2,         // ����
    BTN_STATE_INVALID = 3,      // �Ƿ�
};

// ��ǰ��Ҫ�ػ�Ľ���Ԫ��ö��
enum JFRepaintElement
{
    REPAINT_BTN = 0,        // ��ť��Ҫ�ػ� 
    REPAINT_CM = 1,         // ������Ҫ�ػ�
    REPAINT_ALL = 2,        // ���ж�Ҫ�ػ�
    REPAINT_BASIC_INFO = 3, // �������Ļ�����Ϣ
    REPAINT_NO,             // û��������Ҫ�ػ�
};

// ������Ϣ��������Ϣ����
enum JFPlayerInfoIndex
{
    BLACK_NAME = 0,         // ִ��������
    BLACK_SCORE = 1,        // ִ���ߵ÷�
    BLACK_TIME_COST = 2,    // ִ���ߺ�ʱ

    WHITE_NAME = 3,         // ִ��������
    WHITE_SCORE = 4,        // ִ���ߵ÷�
    WHITE_TIME_COST = 5,    // ִ���ߺ�ʱ

    PLAYER_INFO_TIMER = 6,  // ��ʱ��
};


#endif