/**********************************************************************************
 *  JFEngine.h
 *  Implementation of the Class JFEngine
 *  Created on:      26-四月-2010 15:26:50
 *  Original author: Jiang Feng
 *********************************************************************************/

#if !defined(EA_43F6C58C_8FB2_477f_8C4A_C252D1C4C293__INCLUDED_)
#define EA_43F6C58C_8FB2_477f_8C4A_C252D1C4C293__INCLUDED_

#include "JFStruct.h"
#include "JFSysData.h"

#define DANGER_RANGE 4              // 求危险点时，半径为4颗子
#define FORWARD_DETECT_STEP 1       // 简单模式下向前预测步数为1


/**
 * 五子棋引擎
 */
class JFEngine
{

public:
	
	virtual ~JFEngine();

    /**
     * 函数功能：获取指向单例的指针
     * 参数：无
     * 返回值：指向单例的指针	
     */
    static JFEngine* GetEngine(void);

    /**
    * 函数功能：初始化引擎数据
    * 参数：空
    * 返回值：空	
    */
    void Initialize(void);

    /**
    * 函数功能：开始游戏，重置相关数据
    * 参数：无
    * 返回值：无	
    */
    void StartGame(void);

    /**
     * 函数功能：获取最佳防御点
     * 参数：无
     * 返回值：当前棋盘上的一个空的棋盘节点的位置	
     */
	const JFPosInCB& GetBestPosForDefence();

    /**
     * 函数功能：获取最佳进攻点
     * 参数：无
     * 返回值：当前棋盘上的一个空的棋盘节点的位置	
     */
	const JFPosInCB& GetBestPosForOffense();

    /**
     * 函数功能：获取最佳落子点
     * 参数：
     *          CMType : 所求的最佳落子点的类型
     * 返回值：当前棋盘上的一个空的棋盘节点的位置		
     */
	const JFPosInCB& GetBestPosForPlaceCM(const JFCMType& CMType);

    /**
    * 函数功能：根据权值最小值限定获取棋盘上一定区域内的空余节点
    *           这些权值是由棋子类型为type的棋子造成的。
    * 参数：
    *          pCBData  : 棋盘数据
    *          type     : 棋子类型，所求的空余节点是type的棋子造成的
    *          rect     : 在棋盘pCBData上的一个区域
    *          weight   : 要获取的空余棋盘节点的权值
    *          vNULLCM  : 获得的空余节点集合，已经按照权值从大到小排序
    * 返回值：	空
    */
    void GetNULLCMByWeight( IN JFChessman* pCBData,
                            IN const JFCMType& type, 
                            IN const JFCBRect& rect, 
                            IN const jf_uint& uiWeight,
                            OUT vector<JFChessman>& vNULLCM);
	 
    /**
     * 函数功能：获取类型type的棋子导致的棋局
     * 参数：
     *          type      : 棋子类型
     *          situation : 棋局数据
     * 返回值：	空
     */
	void GetCurSituation(IN const JFCMType& type, OUT JFCBSituation& situation);

private:
    // 私有化构造函数，实现单例模式
    JFEngine();

    /**
    * 函数功能：获取棋盘上一定区域内的危险点
    * 参数：
    *          pCBData  : 棋盘数据
    *          selfType : 棋子类型，所求的危险点是对于selfType的棋子而言，对方可能会落子的点
    *          rect     : 在棋盘pCBData上的一个区域
    *          vDangerousCM : 获得的危险点集合，已经按照权值从大到小排序
    * 返回值：	空
    */
    void GetDangerNULLCM(IN JFChessman* pCBData,
                        IN const JFCMType& selfType, 
                        IN const JFCBRect& rect, 
                        OUT vector<JFChessman>& vDangerousCM);

    /**
     * 函数功能：获取棋盘上一定区域内一定类型的棋子
     * 参数：
     *          pCBData : 棋盘数据
     *          rect    : 在棋盘pCBData上的一个区域
     *          type    : 要提取的棋子的类型
     *          vCMInRect : 提取的棋子集合
     * 返回值：空	
     */
    void GetCMInRect(IN const JFChessman* pCBData, 
                     IN const JFCBRect& rect, 
                     IN const JFCMType& type, 
                     OUT vector<JFChessman>& vCMInRect);

    /**
    * 函数功能：获取棋盘区域内一定类型的危险点
    * 参数：
    *          pCBData : 棋盘数据
    *          rect    : 在棋盘pCBData上的一个区域
    *          uiWeight: 要提取的空余棋子节点的权值最小值
    *          vDanger : 提取的棋子集合
    * 返回值：空	
    */
    void RetriveDangerCMByWeight(IN JFChessman* pCBData, 
                               IN const JFCBRect& rect,
                               IN const jf_uint& uiWeight,
                               OUT vector<JFChessman>& vDanger);
	 
    /**
     * 函数功能：拷贝原始棋盘数据到pDest
     * 参数：
     *          pDest : 目的地
     * 返回值：空	
     */
	void CopySrcCBData(IN JFChessman* pDest);
	 
    /**
     * 函数功能：计算棋子chessman在棋盘pCBData上的区域rect内产生的权值
     *           这些权值被加到rect内的被影响到的空棋盘节点的权值中
     * 参数：
     *          pCBData  : 棋盘数据
     *          rect     : 棋盘pCBData内的一块区域
     *          chessman : 棋盘pCBData上的一颗棋子
     * 返回值：	空
     */
	inline void CalcTheWeight(IN JFChessman* pCBData, IN const JFCBRect& rect, IN const JFChessman& chessman);

    /**
    * 函数功能：用来执行权值操作
    * 参数：
    *          pCBData     : 棋盘数据
    *          rect        : 棋盘区域
    *          chessman    : 某个棋子，以该棋子为参考
    *          direction   : 方向
    *          iRange      : 危险半径
    * 返回值： 无	
    */
    inline void DoCalcWeight(IN JFChessman* pCBData,
                             IN const JFCBRect& rect,
                             IN const JFChessman& chessman,
                             IN const JFDirection& direction,
                             IN const jf_int32& iRange);

    /**
    * 函数功能：当中途遇到空节点时反向增加其权值时调用该函数
    * 参数：
    *          pCBData     : 棋盘数据
    *          rect        : 棋盘区域
    *          chessman    : 某个棋子，以该棋子为参考
    *          direction   : 方向
    *          iRange      : 危险半径
    * 返回值： 无	
    */
    inline void JFEngine::DoCalcWeightBack(IN JFChessman* pCBData,
                                            IN const JFCBRect& rect,
                                            IN const JFChessman& chessman,
                                            IN const JFDirection& direction,
                                            IN const jf_int32& iRange);

    /**
     * 函数功能：获取一个方向的反方向
     * 参数：
     *          direction : 一个方向
     *          rDirection: 获取的direction的反方向
     * 返回值：	无
     * 注意：该函数严格按照JFDirection中枚举的方向计算，因此，如果JFDirection枚举改变
     *       该方法就可能出错
     */
    inline void GetRDirection(IN const JFDirection& direction, OUT JFDirection& rDirection);


private:
    // 指向单例的指针
    static JFEngine* m_pEngine;

    /** 
    * 棋局
    */
    JFCBSituation m_CBSituation;

    /**
    * 五子棋引擎
    */
    JFEngine *m_pGobangEngine;

    /** 
     * 最佳防御点
     */
    JFPosInCB m_bestDefencePos;

    /** 
    * 最佳进攻点
    */
    JFPosInCB m_bestOffensePos;

    /** 
    * 最佳落子点
    */
    JFPosInCB m_bestPlacePos;

    jf_long32 m_vXCalc[8];      // 用于x计算的数组
    jf_long32 m_vYCalc[8];      // 用于y计算的数组
};
#endif // !defined(EA_43F6C58C_8FB2_477f_8C4A_C252D1C4C293__INCLUDED_)
