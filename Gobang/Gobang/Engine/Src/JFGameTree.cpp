/** 
 * 文 件 名: JFGameType.cpp
 * 摘    要: 本文件包含了博弈树的定义
 *
 * 作    者: 姜峰
 * 创建时间: 2010.05.16
 */

#include "JFGameTree.h"
#include "JFEngine.h"

JFGameTree::JFGameTree()
: m_pRoot(NULL)       
, m_type(NO_CM)
, m_uiLayer(0)
, m_bestPosToPlace(-1, -1)
{
    // 初始化线数据
    for (int i=0; i<88; ++i)
    {
        m_srcSelfLineData.push_back(0);
        m_srcEnemyLineData.push_back(0);
    }
}

JFGameTree::~JFGameTree()
{
    // 删除博弈树，释放空间
    SafeDeleteNode(m_pRoot);
}


/**
* 函数功能：构造博弈树
* 参数：
*          type : 该博弈树是为类型为type的棋子建立的，其目的是
*                 通过该博弈树，为type类型的棋子，在当前的棋局下
*                 找出一个最优的落子点
*          pCBData : 当前的棋盘数据，此时应该是轮到type类型的棋子落子
*          uiStep  : 预定的预测步数，预测步数越多给出的落子点越接近最有落子点
*                    但是，速度可能会比较慢。
*                    uiStep可以为0，为0时表明是构造一个空树。即只有根节点。
* 返回值：	true    : 构造博弈树成功
*          false   : 构造博弈树失败
*/
const JFPosInCB& JFGameTree::GetBestPosToPlace(IN const JFCMType& type,
                                              IN JFChessman* pCBData,
                                              IN const jf_uint& uiStep)
{
    _ASSERT(type != NO_CM && pCBData != NULL);

    m_uiLayer = uiStep;
    
    // 1、将棋盘数据拷贝到本地保存
    m_type = type;
    m_enemyType = (m_type == BLACK_CM) ? (WHITE_CM) : (BLACK_CM);
    CopyCBData(&m_srcCBData[0][0], pCBData);
    
    // 清除棋盘数据上的权值信息
    for (int row=0; row<=CB_GRID_NUM; ++row)
    {
        for (int col=0; col<=CB_GRID_NUM; ++col)
        {
            m_srcCBData[row][col].uiWeight = 0;
        }
    }

    // 2、将棋盘数据表示成线数据，并保存在本地
    CreateLineData(&m_srcCBData[0][0], m_type, m_srcSelfLineData);
    CreateLineData(&m_srcCBData[0][0], m_enemyType, m_srcEnemyLineData);

    // 查看是否有4缺1的情况
    JFPosInCB bestPosForMyself;
    JFPosInCB bestPosForEnemy;

    // 11110  11101 11011 10111 01111 
    vector<jf_ushort> vPattern;
    vPattern.push_back(30);
    vPattern.push_back(29);
    vPattern.push_back(27);
    vPattern.push_back(23);
    vPattern.push_back(15);

    for (int i=0; i<5; ++i)
    {// 进攻
        bestPosForMyself = GetThePosOfPattern(m_srcSelfLineData, vPattern[i], i);

        if (bestPosForMyself.lX < 0 || bestPosForMyself.lY < 0)
        {
            continue;
        }

        if (JFSysData::GetSysData()->IsCurPosAvailable(bestPosForMyself))
        {
            m_bestPosToPlace = bestPosForMyself;
        }

        return m_bestPosToPlace;
    }

    for (int i=0; i<5; ++i)
    {// 防御
        bestPosForEnemy = GetThePosOfPattern(m_srcEnemyLineData, vPattern[i], i);

        if (bestPosForEnemy.lX < 0 || bestPosForEnemy.lY < 0)
        {
            continue;
        }

        if (JFSysData::GetSysData()->IsCurPosAvailable(bestPosForEnemy))
        {
            m_bestPosToPlace = bestPosForEnemy;
        }

        return m_bestPosToPlace;
    }

    

    // 3、创建两个临时棋盘数据
    /************************************************
    * 获取敌方(人)的棋子导致的危险点的权值，如果
    * 有>=5的，那么就防御。如果没有则获取我方(电脑)
    * 的棋子导致的权值，并实施进攻!
    ************************************************/

    JFChessman vOffenseCBData[15][15];
    JFChessman vDefenceCBData[15][15];

    CopyCBData(&vOffenseCBData[0][0], pCBData);
    CopyCBData(&vDefenceCBData[0][0], pCBData);

    
    // 4、决定是进攻还是防御
    vector<JFChessman> vOffenseDangerCM;
    vector<JFChessman> vDefenceDangerCM;

    JFEngine::GetEngine()->GetNULLCMByWeight(&vOffenseCBData[0][0], 
                                            m_type,
                                            JFCBRect(0,0,CB_GRID_NUM, CB_GRID_NUM), 
                                            WEIGHT_DETECT,          // 权值为5
                                            vOffenseDangerCM);

    JFEngine::GetEngine()->GetNULLCMByWeight(&vDefenceCBData[0][0], 
                                            m_enemyType,
                                            JFCBRect(0,0,CB_GRID_NUM, CB_GRID_NUM), 
                                            WEIGHT_DETECT,          // 权值为5
                                            vDefenceDangerCM);

    jf_bool bOffense = false;       // 标识是否进攻
    if (vOffenseDangerCM.size() > 0 && vDefenceDangerCM.size() > 0)
    {
        if (!ShouldDefence(vDefenceDangerCM))
        {
            bOffense = true;
        }
    }
    else if (vOffenseDangerCM.size() > 0 && vDefenceDangerCM.size() == 0)
    {
        bOffense = true;
    }
    else
    {
        bOffense = false;
    }


    // 进攻
    if (bOffense)
    {
        m_bestPosToPlace = vOffenseDangerCM[0].position;          // 预定先
        m_pRoot = new JFTreeNode;
        if (NULL == m_pRoot)
        {
            return m_bestPosToPlace;
        }

        m_pRoot->pParent = NULL;        // 根节点没有父节点

        jf_uint uiDangerSize = vOffenseDangerCM.size();
        JFDangerPos dangerPos;
        for (jf_uint ui=0; ui<uiDangerSize; ++ui)
        {
            dangerPos.pos = vOffenseDangerCM[ui].position;
            m_pRoot->vDangerPos.push_back(dangerPos);
        }

        ConstructOffenseTree(m_pRoot, type, m_uiLayer-1, m_vCMPlace);

        // 执行倒推
        GetBestOffensePos(m_pRoot, m_uiLayer);

        // 获取权值最大者
        if (m_pRoot != NULL)
        {
            jf_uint uiMax = 0;
            int iMaxIndex = 0;
            for (int i=0; i<uiDangerSize; ++i)
            {
                if (uiMax <= m_pRoot->vDangerPos[i].uiWeight)
                {
                    iMaxIndex = i;
                    uiMax = m_pRoot->vDangerPos[i].uiWeight;
                }
            }

            _ASSERT(m_pRoot->vDangerPos.size() > 0);
            m_bestPosToPlace = m_pRoot->vDangerPos[iMaxIndex].pos;
        }
    }
    // 防御
    else if (!bOffense && (vDefenceDangerCM.size() > 0))
    {
        m_bestPosToPlace = vDefenceDangerCM[0].position;          // 预定先
        m_pRoot = new JFTreeNode;
        if (NULL == m_pRoot)
        {
            return m_bestPosToPlace;
        }

        m_pRoot->pParent = NULL;        // 根节点没有父节点

        jf_uint uiDangerSize = vDefenceDangerCM.size();
        JFDangerPos dangerPos;
        for (jf_uint ui=0; ui<uiDangerSize; ++ui)
        {
            dangerPos.pos = vDefenceDangerCM[ui].position;
            m_pRoot->vDangerPos.push_back(dangerPos);
        }

        ConstructDefenceTree(m_pRoot, type, m_uiLayer-1, m_vCMPlace);

        // 执行倒推
        GetBestDefencePos(m_pRoot, m_uiLayer);

        // 获取权值最小者
        if (m_pRoot != NULL)
        {
            jf_uint uiMin = 0;
            int iMinIndex = 0;
         
            for (int i=0; i<uiDangerSize; ++i)
            {
                if (uiMin >= m_pRoot->vDangerPos[i].uiWeight)
                {
                    iMinIndex = i;
                    uiMin = m_pRoot->vDangerPos[i].uiWeight;
                }
            }

            _ASSERT(m_pRoot->vDangerPos.size() > 0);
            m_bestPosToPlace = m_pRoot->vDangerPos[iMinIndex].pos;
        }
    }

    return m_bestPosToPlace;
}


/**
* 函数功能：根据根节点创建博弈树
* 参数：
*          pRoot  : 指向根节点的指针
*          typeToPlace : 当前要落子的棋子的类型
*          uiStep : 子树的层数
*          vCMPlaced : 其根节点到当前棋局时已经下的棋子的集合
* 返回值：	无
*/
void JFGameTree::ConstructDefenceTree(IN JFTreeNode* pRoot, 
                                       IN const JFCMType& typeToPlace, 
                                       IN const jf_uint& uiLayer, 
                                       IN vector<JFChessman> vCMPlaced)
{
    _ASSERT(pRoot != NULL 
            && typeToPlace != NO_CM
            && uiLayer >= 0);

    // 1、假设在这些危险点落子
    vector<JFDangerPos>& vDangerPos = pRoot->vDangerPos;        // 此处的pRoot如果误写成m_pRoot就完了
    jf_uint uiDangerSize = vDangerPos.size();
    for (jf_uint ui=0; ui<uiDangerSize; ++ui)
    {
        // 1、根据根节点的棋盘数据和目前已经落子的集合
        //    计算出当前的棋盘数据的线数据
        JFChessman vCurCBData[15][15];
        CopyCBData(&vCurCBData[0][0], &m_srcCBData[0][0]);      // 拷贝棋盘数据
        AddCMToCBData(&vCurCBData[0][0], vCMPlaced);

        vector<jf_ushort> vLineData;
        if (typeToPlace == m_type)
        {//此时电脑要落子，需要的是电脑的线数据
            vLineData.assign(m_srcSelfLineData.begin(), m_srcSelfLineData.end());
        }
        else
        {
            vLineData.assign(m_srcEnemyLineData.begin(), m_srcEnemyLineData.end());
        }

        jf_uint uiPlaceSize = vCMPlaced.size();
        for (jf_uint uiIndex=0; uiIndex<uiPlaceSize; ++uiIndex)
        {
            if (vCMPlaced[uiIndex].type == typeToPlace)
            {
                AddCMToLineData(vLineData, vCMPlaced[uiIndex]);
            }
        }

        // 2、落子
        JFChessman cmData(vDangerPos[ui].pos.lX, vDangerPos[ui].pos.lY, typeToPlace, 0);
        AddCMToCBData(&vCurCBData[0][0], cmData);
        AddCMToLineData(vLineData, cmData);

        vector<JFChessman> vNULLCM;
        // 以防御的姿态讲：电脑落子是为了减小人的子导致的权值
        //                 人落子是为了增大人的子导致的权值
        JFEngine::GetEngine()->GetNULLCMByWeight(&vCurCBData[0][0], 
                                                    m_enemyType, 
                                                    JFCBRect(0, 0, CB_GRID_NUM, CB_GRID_NUM),
                                                    WEIGHT_DETECT, 
                                                    vNULLCM);
 
        // 3、决定是否继续建立子树 
        vector<jf_ushort> vPattern;
        vPattern.push_back(31);     // 2进制：11111
        jf_uint uiWinNum = CheckPatternNum(vPattern, vLineData);     // 计算五子连珠数

        if (1 == uiLayer)
        {
            // 将新的棋局的权值信息添加到危险点的权值域中
            jf_uint uiNULLSize = vNULLCM.size();
            for (jf_uint uiNULLIndex=0; uiNULLIndex<uiNULLSize; ++uiNULLIndex)
            {
                pRoot->vDangerPos[ui].uiWeight += vNULLCM[uiNULLIndex].uiWeight/* + m_uiLayer - uiLayer*/;
            }
 
            if (ui == uiDangerSize - 1)
            {// 当前已经是最后一个危险点
                return;
            }
            else
            {
                continue;
            }
        }
        else if(vNULLCM.size() == 0)
        {
            continue;
        }
        else if (uiWinNum != 0)
        {
            // 将新的棋局的权值信息添加到危险点的权值域中
            jf_uint uiNULLSize = vNULLCM.size();
            for (jf_uint uiNULLIndex=0; uiNULLIndex<uiNULLSize; ++uiNULLIndex)
            {
                pRoot->vDangerPos[ui].uiWeight += vNULLCM[uiNULLIndex].uiWeight/* + m_uiLayer - uiLayer*/;
            }

            pRoot->vDangerPos[ui].uiWeight += uiWinNum * WEIGHT_WIN + m_uiLayer - uiLayer;
            continue;
        }
        else
        {// 继续建立子树

            pRoot->vDangerPos[ui].pNext = new JFTreeNode;
            pRoot->vDangerPos[ui].pNext->pParent = &(pRoot->vDangerPos[ui]);
            //pRoot = pRoot->vDangerPos[ui].pNext;  // 此处万不可修改pRoot
            
            jf_uint uiNextDangerSize = vNULLCM.size();
            for (jf_uint uiNext=0; uiNext<uiNextDangerSize; ++uiNext)
            {
                pRoot->vDangerPos[ui].pNext->vDangerPos.push_back(JFDangerPos(vNULLCM[uiNext].position, 0, (JFTreeNode*)NULL));
            }            
            
            /*******************************************
             * 只有决定继续建立子树后才能将当前危险点添加到
             * 已经落子集合中
             *******************************************/
            //vCMPlaced.push_back(cmData);        
            vector<JFChessman> vPlaced(vCMPlaced);
            vPlaced.push_back(cmData);                  // 添加到已经落子集合中
            JFCMType otherType = (typeToPlace == BLACK_CM) ? (WHITE_CM) : (BLACK_CM);
            ConstructDefenceTree(pRoot->vDangerPos[ui].pNext, 
                                otherType, 
                                uiLayer-1, 
                                vPlaced);            
        }
    } // end for (jf_uint ui=0; ui<uiDangerSize; ++ui)
}


/**
* 函数功能：根据根节点创建进攻博弈树
* 参数：
*          pRoot  : 指向根节点的指针
*          typeToPlace : 当前要落子的棋子的类型
*          uiStep : 要预测的步数
*          vCMPlaced : 其根节点到当前棋局时已经下的棋子的集合
* 返回值：	无
*/
void JFGameTree::ConstructOffenseTree(IN JFTreeNode* pRoot, 
                                      IN const JFCMType& typeToPlace, 
                                      IN const jf_uint& uiLayer, 
                                      IN vector<JFChessman> vCMPlaced)
{
    _ASSERT(pRoot != NULL 
        && typeToPlace != NO_CM
        && uiLayer >= 0);

    // 1、假设在这些危险点落子
    vector<JFDangerPos>& vDangerPos = pRoot->vDangerPos;        // 此处的pRoot如果误写成m_pRoot就完了
    jf_uint uiDangerSize = vDangerPos.size();
    for (jf_uint ui=0; ui<uiDangerSize; ++ui)
    {
        // 1、根据根节点的棋盘数据和目前已经落子的集合
        //    计算出当前的棋盘数据的线数据
        JFChessman vCurCBData[15][15];
        CopyCBData(&vCurCBData[0][0], &m_srcCBData[0][0]);      // 拷贝棋盘数据
        AddCMToCBData(&vCurCBData[0][0], vCMPlaced);

        vector<jf_ushort> vLineData;
        if (typeToPlace == m_type)
        {//此时电脑要落子，需要的是电脑的线数据
            vLineData.assign(m_srcSelfLineData.begin(), m_srcSelfLineData.end());
        }
        else
        {
            vLineData.assign(m_srcEnemyLineData.begin(), m_srcEnemyLineData.end());
        }

        jf_uint uiPlaceSize = vCMPlaced.size();
        for (jf_uint uiIndex=0; uiIndex<uiPlaceSize; ++uiIndex)
        {
            if (vCMPlaced[uiIndex].type == typeToPlace)
            {
                AddCMToLineData(vLineData, vCMPlaced[uiIndex]);
            }
        }

        // 2、落子
        JFChessman cmData(vDangerPos[ui].pos.lX, vDangerPos[ui].pos.lY, typeToPlace, 0);
        AddCMToCBData(&vCurCBData[0][0], cmData);
        AddCMToLineData(vLineData, cmData);

        vector<JFChessman> vNULLCM;
        // 以进攻的姿态讲：电脑落子是为了增大电脑落子导致的权值
        //                 人落子是为了增大人的子导致的权值
        JFEngine::GetEngine()->GetNULLCMByWeight(&vCurCBData[0][0],
                                                    m_type,
                                                    JFCBRect(0, 0, CB_GRID_NUM, CB_GRID_NUM),
                                                    WEIGHT_DETECT, 
                                                    vNULLCM);
       

        // 3、决定是否继续建立子树 
        vector<jf_ushort> vPattern;
        vPattern.push_back(31);     // 2进制：11111
        jf_uint uiWinNum = CheckPatternNum(vPattern, vLineData);     // 计算五子连珠数

        if (1 == uiLayer)
        {
            // 将新的棋局的权值信息添加到危险点的权值域中
            jf_uint uiNULLSize = vNULLCM.size();
            for (jf_uint uiNULLIndex=0; uiNULLIndex<uiNULLSize; ++uiNULLIndex)
            {
                pRoot->vDangerPos[ui].uiWeight += vNULLCM[uiNULLIndex].uiWeight /*+ m_uiLayer - uiLayer*/;
            }

            if (ui == uiDangerSize - 1)
            {// 当前已经是最后一个危险点
                return;
            }
            else
            {
                continue;
            }
        }
        else if(vNULLCM.size() == 0)
        {
            continue;
        }
        else if (uiWinNum != 0)
        {
            // 将新的棋局的权值信息添加到危险点的权值域中
            jf_uint uiNULLSize = vNULLCM.size();
            for (jf_uint uiNULLIndex=0; uiNULLIndex<uiNULLSize; ++uiNULLIndex)
            {
                pRoot->vDangerPos[ui].uiWeight += vNULLCM[uiNULLIndex].uiWeight;
            }

            pRoot->vDangerPos[ui].uiWeight += uiWinNum * WEIGHT_WIN /*+ m_uiLayer - uiLayer*/;
            continue;
        }
        else
        {// 继续建立子树

            pRoot->vDangerPos[ui].pNext = new JFTreeNode;
            pRoot->vDangerPos[ui].pNext->pParent = &(pRoot->vDangerPos[ui]);
            //pRoot = pRoot->vDangerPos[ui].pNext;  // 此处万不可修改pRoot

            jf_uint uiNextDangerSize = vNULLCM.size();
            for (jf_uint uiNext=0; uiNext<uiNextDangerSize; ++uiNext)
            {
                pRoot->vDangerPos[ui].pNext->vDangerPos.push_back(JFDangerPos(vNULLCM[uiNext].position, 0, (JFTreeNode*)NULL));
            }            

            /*******************************************
            * 只有决定继续建立子树后才能将当前危险点添加到
            * 已经落子集合中
            *******************************************/
            //vCMPlaced.push_back(cmData);        
            vector<JFChessman> vPlaced(vCMPlaced);
            vPlaced.push_back(cmData);                  // 添加到已经落子集合中
            JFCMType otherType = (typeToPlace == BLACK_CM) ? (WHITE_CM) : (BLACK_CM);
            ConstructOffenseTree(pRoot->vDangerPos[ui].pNext, 
                                otherType, 
                                uiLayer-1, 
                                vPlaced);

        }
    } // end for (jf_uint ui=0; ui<uiDangerSize; ++ui)
}

/**
* 函数功能：安全释放子树
* 参数：
*          指向节点的指针
* 返回值：	无
*/
void JFGameTree::SafeDeleteNode(JFTreeNode* pNode)
{
    if (NULL == pNode)
    {
        return;
    }

    jf_uint uiPosSize = pNode->vDangerPos.size();
    for (jf_uint ui=0; ui<uiPosSize; ++ui)
    {
        if (pNode->vDangerPos[ui].pNext != NULL)
        {
            SafeDeleteNode(pNode->vDangerPos[ui].pNext);
        }
    }

    delete pNode;
    pNode = NULL;
}

/**
* 函数功能：根据已经建立的博弈树获取最佳落子点
* 参数：
*          pRoot : 指向子树的根节点的指针
*          uiCurLayer : 当前节点在博弈树中的层数
* 返回值：	
*/
void JFGameTree::GetBestDefencePos(IN JFTreeNode* pRoot, IN const jf_uint uiCurLayer)
{
    if (NULL == pRoot)
    {
        return ;
    }

    jf_uint ivSize = pRoot->vDangerPos.size();
   
    for (jf_uint ui = 0; ui < ivSize; ++ui)
    {
        GetBestDefencePos(pRoot->vDangerPos[ui].pNext, uiCurLayer-1);
    }

    jf_uint uiMaxMinValue = 0;
    if (ivSize != 0)
    {
        uiMaxMinValue = pRoot->vDangerPos[0].uiWeight;                  // 极大极小值
    }

    // 偶数层，其危险点的权值是由电脑落子产生的，因此要选择危险值最小值
    if (uiCurLayer % 2 == 0)
    {
        for (jf_uint ui = 1; ui < ivSize; ++ui)
        {
            uiMaxMinValue = (uiMaxMinValue <= pRoot->vDangerPos[ui].uiWeight) ? (uiMaxMinValue) : (pRoot->vDangerPos[ui].uiWeight);
        }
    }
    // 奇数层，其危险点的权值是由人落子产生的，因此要选择最大值，即立足于最坏情况
    else
    {
        for (jf_uint ui = 1; ui < ivSize; ++ui)
        {
            uiMaxMinValue = (uiMaxMinValue >= pRoot->vDangerPos[ui].uiWeight) ? (uiMaxMinValue) : (pRoot->vDangerPos[ui].uiWeight);
        }
    }

    if (pRoot->pParent != NULL)
    {
        pRoot->pParent->uiWeight = uiMaxMinValue;
    }
}


/**
* 函数功能：根据已经建立的博弈树获取最佳进攻落子点
* 参数：
*          pRoot : 指向子树的根节点的指针
*          uiCurLayer : 当前节点在博弈树中的层数
* 返回值： 无
*/
void JFGameTree::GetBestOffensePos(IN JFTreeNode* pRoot, IN const jf_uint uiCurLayer)
{
    if (NULL == pRoot)
    {
        return ;
    }

    jf_int32 ivSize = pRoot->vDangerPos.size();

    for (jf_uint ui = 0; ui < ivSize; ++ui)
    {
        GetBestOffensePos(pRoot->vDangerPos[ui].pNext, uiCurLayer-1);
    }

    jf_uint uiMaxMinValue = 0;
    if (ivSize > 0)
    {
        uiMaxMinValue = pRoot->vDangerPos[0].uiWeight;                  // 极大极小值
    }

    for (jf_uint ui = 1; ui < ivSize; ++ui)
    {
        uiMaxMinValue = (uiMaxMinValue >= pRoot->vDangerPos[ui].uiWeight) ? (uiMaxMinValue) : (pRoot->vDangerPos[ui].uiWeight);
    }

    if (pRoot->pParent != NULL)
    {
        pRoot->pParent->uiWeight = uiMaxMinValue;
    }
}

/**
* 函数功能：获取危险点集合中的危险点的权值的和
* 参数：
*         vDangerCM : 危险点集合
* 返回值：权值之和
*/
jf_uint JFGameTree::GetMaxWeight(const vector<JFChessman>& vDangerCM)
{
    jf_uint uiMax = 0;
    jf_int32 iMaxIndex = 0;
    jf_int32 uiSize = vDangerCM.size();

    for (jf_int32 ui=0; ui < uiSize; ++ui)
    {
        if (uiMax <= vDangerCM[ui].uiWeight)
        {
            uiMax = vDangerCM[ui].uiWeight;
            iMaxIndex = ui;
        }
    }

    return vDangerCM[iMaxIndex].uiWeight;
}