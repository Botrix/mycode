/**********************************************************************************
*  JFStruct.h
*  该文件包含了用到的结构体的定义
*  Created on:      18-四月-2010 12:03:04
*  Original author: Jiang Feng
*********************************************************************************/

#ifndef JFSTRUCT_H_ 
#define JFSTRUCT_H_

#include "JFType.h"

/**
* JFPosInCB (CB->ChessBoard)
* 棋盘上的棋子所在的位置结构
*/
struct JFPosInCB
{

public:
    /**
    * 列数，从0开始
    */
    jf_long32 lX;
    /**
    * 行数，从0开始
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
* 棋盘上的棋子节点的类型
* 1、无棋子
* 2、黑棋
* 3、白棋
*/
enum JFCMType
{
    /**
    * 棋盘节点上没有棋子
    */
    NO_CM = 0,
    /**
    * 棋盘节点上的棋子为黑棋
    */
    BLACK_CM = 1,
    /**
    * 棋盘节点上的棋子为白棋
    */
    WHITE_CM = 2
};

/**
* 玩家类型
* 1、无类型
* 2、电脑
* 3、人
*/
enum JFPlayerType
{
    /** 
     * 无类型
     */
    PLAYER_NO = 0, 

    /**
     * 人
     */
    PLAYER_HUMAN = 1,

    /**
     * 电脑
     */
    PLAYER_COMPUTER = 2
};

/**
* 棋盘区域结构，该结构为矩形
* 因此，只有左上角和右下角两个成员
*/
struct JFCBRect
{

public:
    /**
    * 区域的左上角
    */
    JFPosInCB topleftPos;
    /**
    * 区域的右下角
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
* 棋盘数据中的棋子描述
*/
struct JFChessman
{

public:
    /**
    * 棋子在棋盘上的位置
    */
    JFPosInCB position;
    /**
    * 棋盘节点上的棋子的类型
    */
    JFCMType type;
    /**
    * 该棋子的权值，在求最佳点时使用
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

    // 用于sort排序，权值从大到小排序
    static jf_bool WeightLarger(const JFChessman& _left, const JFChessman& _right)
    {
        return (_left.uiWeight > _right.uiWeight);
    }
};


/**
* 棋盘界面上的棋子描述
*/
struct JFCMForDisp
{

public:
    /**
    * 棋子在棋盘上的x坐标
    */
    jf_long32 lx;

    /**
    * 棋子在棋盘上的y坐标
    */
    jf_long32 ly;

    /**
    * 棋子的类型
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
 * 五子连珠
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
* 棋局
*/
struct JFCBSituation
{
public:
    JFCMType typeWin;                   // 当前赢的棋子的类型       
    vector<JFFiveRenju> vFiveRenju;     // 当前五子连珠的棋子集合，可能不止5颗子

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
    DIRECTION_NORTH = 0,            // 北
    DIRECTION_NORTH_EAST = 1,       // 东北
    DIRECTION_EAST = 2,             // 东
    DIRECTION_SOUTH_EAST = 3,       // 东南
    DIRECTION_SOUTH = 4,            // 南
    DIRECTION_SOUTH_WEST = 5,       // 西南
    DIRECTION_WEST = 6,             // 西
    DIRECTION_NORTH_WEST = 7        // 西北
};


// 游戏模式
// 1、人人对战
// 2、人机对战
enum JFGameMode
{
    HUMAN_VS_HUMAN = 0,     // 人人
    HUMAN_VS_COMPUTER = 1,  // 人机
};

// 游戏提示
enum JFTips
{
    BLACK_WIN = 0,      // 黑子赢
    WHITE_WIN = 1,      // 白子赢
};

// 游戏状态
enum JFGameState
{
    GAME_ON = 0,
    GAME_OVER = 1,
};

// 界面上的按钮ID枚举
enum JFBtnID
{
    BTN_SAVE    = 0,    // 保存
    BTN_LOAD    = 1,    // 加载
    BTN_START   = 2,    // 开始
    BTN_PEACE   = 3,    // 求和
    BTN_GIVEIN  = 4,    // 认输
    BTN_CANCEL  = 5,    // 退出
    BTN_INVALID = 6,    // 非法按钮
};

// 按钮的三态
enum JFBtnState
{
    BTN_STATE_NORMAL = 0,       // 常态
    BTN_STATE_HIGHLIGHT = 1,    // 高亮
    BTN_STATE_DOWN = 2,         // 按下
    BTN_STATE_INVALID = 3,      // 非法
};

// 当前需要重绘的界面元素枚举
enum JFRepaintElement
{
    REPAINT_BTN = 0,        // 按钮需要重绘 
    REPAINT_CM = 1,         // 棋子需要重绘
    REPAINT_ALL = 2,        // 所有都要重绘
    REPAINT_BASIC_INFO = 3, // 棋手区的基本信息
    REPAINT_NO,             // 没有区域需要重绘
};

// 棋手信息区各个信息索引
enum JFPlayerInfoIndex
{
    BLACK_NAME = 0,         // 执黑者姓名
    BLACK_SCORE = 1,        // 执黑者得分
    BLACK_TIME_COST = 2,    // 执黑者耗时

    WHITE_NAME = 3,         // 执白者姓名
    WHITE_SCORE = 4,        // 执白者得分
    WHITE_TIME_COST = 5,    // 执白者耗时

    PLAYER_INFO_TIMER = 6,  // 计时器
};


#endif