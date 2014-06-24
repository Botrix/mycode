/**********************************************************************************
 *  JFSysData.h
 *  Implementation of the Class JFSysData
 *  Created on:      26-四月-2010 15:26:53
 *  Original author: Jiang Feng
 *********************************************************************************/

#if !defined(EA_9217E43C_3E68_46c6_BCC7_01B48EA3DA67__INCLUDED_)
#define EA_9217E43C_3E68_46c6_BCC7_01B48EA3DA67__INCLUDED_

#include "JFStruct.h"
#include "JFFile.h"


/**
 * JFSysData类用于管理系统的数据信息，比如当前棋局、人机各方的棋子布局等信息。
 * JFFile写到文件中的数据就来自这里。从文件中读取的数据也存在这里。它相当
 * 于一个常驻内存的数据库管理系统。
 */
class JFSysData
{

public:

	virtual ~JFSysData();

	/**
	 * 初始化棋盘数据，该函数在系统初始化和开始新的一局时调用
	 */
	void Initialize();


    /**
    * 落子，即向数据中添加当前棋子的信息
    */
    jf_bool PlaceChessman(const JFChessman& chessman);

    /**
    * 函数功能：开始游戏，重置相关数据
    * 参数：无
    * 返回值：无	
    */
    void StartGame(void);

    /**
    * 函数功能：结束游戏
    * 参数：无
    * 返回值：无	
    */
    void EndGame(void);

    /*******************************************************************
     * Get methods
     *******************************************************************/

    /**
    * 获取指向单例的指针
    */
    static JFSysData* GetSysData(void);

    /**
    * 获得当前要下的棋子的类型
    */
    const JFCMType& GetCurCMTypeToPlace() const;

    /**
    * 获取电脑所执的棋子的类型
    */
    const JFCMType& GetCMTypeOfComputer() const;

    /**
    * 获取原始棋盘数据
    */
    const JFChessman* GetCBData();

    /**
     * 函数功能：获取type类型的棋子线数据
     * 参数：
     *          type : 棋子类型
     * 返回值：
     *          对数据的常引用
     */
    const vector<jf_ushort>& GetLineData(IN const JFCMType& type);

    /**
    * 获取当前游戏难度级别
    */
    const JFGameLevel& GetGameLevel() const;

    /**
     * 函数功能：获取当前游戏模式
     * 参数：无
     * 返回值：当前游戏模式	
     */
    const JFGameMode& GetGameMode(void) const;

    /**
    * 函数功能：获取当前游戏状态
    * 参数：无
    * 返回值：当前游戏状态	
    */
    const JFGameState& GetGameState(void) const;

    /*******************************************************************
     * Set methods
     *******************************************************************/

	/**
	 * 设置下一个要下的棋子的类型
	 */
	void SetCurCMTypeToPlace(const JFCMType type);
	
	
    /**
    * 设置当前游戏难度级别
    */
    void SetGameLevel(const JFGameLevel& level);

    /**
    * 函数功能：设置当前游戏模式
    * 参数：
    *           mode : 游戏模式
    * 返回值：
    *           true : 设置成功
    *           false: 设置不成功，当前游戏可能正在进行
    */
    jf_bool SetGameMode(const JFGameMode& mode);

    /*******************************************
     * 用于判断的方法
     *******************************************/
    /**
     * 函数功能：判断棋盘节点pos能够落子
     * 参数：
     *          pos : 棋盘节点
     * 返回值：	
     *         true : pos处可落子
     *         false: pos处不可落子
     */
    jf_bool IsCurPosAvailable(IN const JFPosInCB& pos);

private:
    JFSysData();

public:
    /**
    * 实现单例模式
    */
    static JFSysData *m_pSysData;

private:
    /*******************************************************************
     * 棋盘数据
     *******************************************************************/

	JFChessman m_vChessBoard[15][15];   // 棋盘原始数据
	
	vector<JFChessman> m_vBlackCM;      // 棋盘上的黑棋

	vector<JFChessman> m_vWhiteCM;      // 棋盘上的白子

    /*******************************************************************
     * 黑白子的线数据总共有：15+15+29*2 = 88
     * 点P(x,y)所属的线定义如下：
     * 0-14  行：[y] 
     * 15-29 列：[x]
     * 30-58 /线：[x+y]
     * 59-87 \线：[x+y+(x>y)*14]
     *******************************************************************/
    vector<jf_ushort>  m_vBlackLineData;   // 棋盘的黑子线数据 : 行、列、斜行、斜行
    vector<jf_ushort>  m_vWhiteLineData;   // 棋盘的白子  线数据 : 行、列、斜行、斜行

	JFCMType m_curCMTypeToPlace;        // 当前应该落子的棋子类型

    JFGameLevel m_curGameLevel;         // 当前游戏难度级别

    JFGameMode m_curGameMode;           // 当前游戏模式

    JFGameState m_gameState;            // 当前游戏状态

    JFFile *m_JFFile;                   // 保存文件

};
#endif // !defined(EA_9217E43C_3E68_46c6_BCC7_01B48EA3DA67__INCLUDED_)
