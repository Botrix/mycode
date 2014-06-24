/** 
 * 文 件 名: JFGameTree.h
 * 摘    要: 本文件包含了博弈树的声明
 *
 * 作    者: 姜峰
 * 创建时间: 2010.05.16
 */

/*******************************************
 * 博弈树
 *
 * 系统开始运行时就构造博弈树，从根节点开始
 * 落下的每个棋子都要记录在案。 :) 因为，
 * 以后的棋盘数据及其线数据都要根据这个已经
 * 下的棋子和原始棋盘数据计算而来。
 *
 *******************************************/
#ifndef JFGAMETREE_H_
#define JFGAMETREE_H_

#include "JFStruct.h"
#include "JFPublicFunction.h"
#define WEIGHT_DETECT  4            // 权值在4以上（包括4）即预测建立子树

struct JFTreeNode;  // 前向声明

// 用于博弈树的危险点定义
struct JFDangerPos 
{
public:
    JFPosInCB       pos;            // 危险点
    jf_uint         uiWeight;       // 由于下了该危险点产生的棋局的权值
    JFTreeNode*     pNext;          // 指向在该危险点落子后产生的棋局
public:
    JFDangerPos()
        : pos(-1, -1)
        , uiWeight(0)
        , pNext(NULL)
    {}

    JFDangerPos(const JFPosInCB& position, const jf_uint& weight, JFTreeNode* p)
    {
        pos      = position;
        uiWeight = weight;
        pNext    = p;
    }

    JFDangerPos(const JFDangerPos& dangerPos)
    {// 此处的拷贝不是深拷贝
        memcpy(this, &dangerPos, sizeof(JFDangerPos));
    }

    JFDangerPos& operator=(const JFDangerPos& dangerPos)
    {// 此处的拷贝不是深拷贝
        memcpy(this, &dangerPos, sizeof(JFDangerPos));

        return *this;
    }
};

// 博弈树节点定义
struct JFTreeNode
{
    JFDangerPos* pParent;   // 指向父节点(危险点)的指针
    vector<JFDangerPos> vDangerPos;
};

// 博弈树定义
// 该博弈树可以动态添加预测层数，并根据当前已下的棋子删掉无用的枝来达到
// 节省内存、提高效率功能。
class JFGameTree
{
public:
    // 构造函数
    JFGameTree();
    ~JFGameTree();

public:
    /**
     * 函数功能：构造博弈树，获取最佳落子点
     * 参数：
     *          type : 该博弈树是为类型为type的棋子建立的，其目的是
     *                 通过该博弈树，为type类型的棋子，在当前的棋局下
     *                 找出一个最优的落子点
     *          pCBData : 当前的棋盘数据，此时应该是轮到type类型的棋子落子
     *          uiStep  : 预定的预测步数，预测步数越多给出的落子点越接近最有落子点
     *                    但是，速度可能会比较慢。
     *                    uiStep可以为0，为0时表明是构造一个空树。即只有根节点。
     * 返回值：	
     *          最佳落子点
     */
    const JFPosInCB& GetBestPosToPlace( IN const JFCMType& type,
                                        IN JFChessman* pCBData,
                                        IN const jf_uint& uiStep);

   
private:
    /**
     * 函数功能：安全释放子树
     * 参数：
     *          指向节点的指针
     * 返回值：	无
     */
    void SafeDeleteNode(JFTreeNode* pNode);

    /**
     * 函数功能：根据根节点创建防御博弈树
     * 参数：
     *          pRoot  : 指向根节点的指针
     *          typeToPlace : 当前要落子的棋子的类型
     *          uiStep : 要预测的步数，控制树的层数
     *          vCMPlaced : 其根节点到当前棋局时已经下的棋子的集合
     * 返回值：	无
     */
    void ConstructDefenceTree(IN JFTreeNode* pRoot, 
                            IN const JFCMType& typeToPlace, 
                            IN const jf_uint& uiLayer, 
                            IN vector<JFChessman> vCMPlaced);

    /**
    * 函数功能：根据根节点创建进攻博弈树
    * 参数：
    *          pRoot  : 指向根节点的指针
    *          typeToPlace : 当前要落子的棋子的类型
    *          uiStep : 要预测的步数
    *          vCMPlaced : 其根节点到当前棋局时已经下的棋子的集合
    * 返回值：	无
    */
    void ConstructOffenseTree(IN JFTreeNode* pRoot, 
                                IN const JFCMType& typeToPlace, 
                                IN const jf_uint& uiLayer, 
                                IN vector<JFChessman> vCMPlaced);

    /**
     * 函数功能：根据已经建立的博弈树获取最佳防御落子点
     * 参数：
     *          pRoot : 指向子树的根节点的指针
     *          uiCurLayer : 当前节点在博弈树中的层数
     * 返回值： 无
     */
    void GetBestDefencePos(IN JFTreeNode* pRoot, IN const jf_uint uiCurLayer);

    /**
    * 函数功能：根据已经建立的博弈树获取最佳进攻落子点
    * 参数：
    *          pRoot : 指向子树的根节点的指针
    *          uiCurLayer : 当前节点在博弈树中的层数
    * 返回值： 无
    */
    void GetBestOffensePos(IN JFTreeNode* pRoot, IN const jf_uint uiCurLayer);

    /**
     * 函数功能：获取危险点集合中的危险点的权值的和
     * 参数：
     *         vDangerCM : 危险点集合
     * 返回值：权值之和
     */
    jf_uint GetMaxWeight(const vector<JFChessman>& vDangerCM);

private:
    JFTreeNode*         m_pRoot;                    // 指向博弈树的根节点，为0层
private:
    jf_uint             m_uiLayer;                  // 构造的博弈树的层数
    JFCMType            m_type;                     // 电脑棋子类型
    JFCMType            m_enemyType;                // 人的棋子类型
    JFChessman          m_srcCBData[15][15];        // 棋盘数据, 不带有权值信息
    vector<jf_ushort>   m_srcSelfLineData;          //  m_type类型的棋子的线数据
    vector<jf_ushort>   m_srcEnemyLineData;         //  m_type类型的敌人的棋子的线数据

    vector<JFChessman> m_vCMPlace;                  // 从根节点到现在的落子序列

    JFPosInCB           m_bestPosToPlace;           // 当前最佳落子点
};

#endif