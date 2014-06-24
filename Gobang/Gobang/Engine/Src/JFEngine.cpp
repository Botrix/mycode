/**********************************************************************************
 *  JFEngine.h
 *  Implementation of the Class JFEngine
 *  Created on:      26-四月-2010 15:26:51
 *  Original author: Jiang Feng
 *********************************************************************************/

#include "JFEngine.h"
#include "JFPublicFunction.h"
#include "JFGameTree.h"

JFEngine* JFEngine::m_pEngine = NULL;

JFEngine::JFEngine()
{
    Initialize();
}


JFEngine::~JFEngine()
{
    if (m_pEngine != NULL)
    {
        delete m_pEngine;
        m_pEngine = NULL;
    }
}

/**
* 函数功能：获取指向单例的指针
* 参数：无
* 返回值：指向单例的指针	
*/
JFEngine* JFEngine::GetEngine(void)
{
    if (NULL == m_pEngine)
    {
        m_pEngine = new JFEngine;
    }

    return m_pEngine;
}

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
void JFEngine::DoCalcWeight( IN JFChessman* pCBData,
                             IN const JFCBRect& rect,
                             IN const JFChessman& chessman,
                             IN const JFDirection& direction,
                             IN const jf_int32& iRange)
{
    jf_long32 lCurX = chessman.position.lX;
    jf_long32 lCurY = chessman.position.lY;
    jf_uint uiCurWeight = 1;
    jf_bool bNullInside = false;        // 标识五个棋子节点中间是否有空节点
    jf_uint uiNullNode = 0;        // 中途遇到的空节点的个数，遇到2个就退出

    for (int i=0; i<iRange; ++i)
    {
        lCurX += m_vXCalc[direction];   // 移动点
        lCurY += m_vYCalc[direction];

        /*******************************************
         * 判断点是否在棋盘内应该在此处判断
         *******************************************/
        if (!IsPosInCBRect(JFPosInCB(lCurX, lCurY), rect))
        {
            break;
        }

        JFChessman& cmData = *(pCBData+lCurX+lCurY*(CB_GRID_NUM+1));

        _ASSERT(cmData.position.lX >= 0 
            && cmData.position.lY >= 0
            && cmData.position.lX <= CB_GRID_NUM
            && cmData.position.lY <= CB_GRID_NUM);

        if (cmData.type == NO_CM)
        {// 空节点
            
            if (uiNullNode >= 1)
            {
                break;
            }

            uiNullNode++;
            bNullInside = true;
            cmData.uiWeight += uiCurWeight;
        }
        else if (cmData.type == chessman.type)
        {// 中途遇到类型相同的棋子，权值加1
            ++uiCurWeight; 

            if (bNullInside)
            {// 如果前面有空节点，则其权值应该再加
                JFDirection rDirection;
                GetRDirection(direction, rDirection);
                DoCalcWeightBack(pCBData, rect, cmData, rDirection, i);
            }

            cmData.uiWeight += uiCurWeight;
        }
        else
        {// 中途遇到敌方棋子
            break;
        }
    }
}


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
void JFEngine::DoCalcWeightBack(IN JFChessman* pCBData,
                                IN const JFCBRect& rect,
                                IN const JFChessman& chessman,
                                IN const JFDirection& direction,
                                IN const jf_int32& iRange)
{
    jf_long32 lCurX = chessman.position.lX;
    jf_long32 lCurY = chessman.position.lY;
    jf_uint uiCurWeight = 1;
    jf_uint uiNullNode = 0;        // 中途遇到的空节点的个数，遇到2个就退出

    for (int i=0; i<iRange && IsPosInCBRect(JFPosInCB(lCurX, lCurY), rect); ++i)
    {
        lCurX += m_vXCalc[direction];   // 移动点
        lCurY += m_vYCalc[direction];

        JFChessman& cmData = *(pCBData+lCurX+lCurY*(CB_GRID_NUM+1));

        if (cmData.type == NO_CM)
        {// 空节点
            
            if (uiNullNode >= 1)
            {
                break;
            }

            cmData.uiWeight += uiCurWeight;
            uiNullNode++;
        }
        else if (cmData.type == chessman.type)
        {// 中途遇到类型相同的棋子，权值加1
            ++uiCurWeight; 

            cmData.uiWeight += uiCurWeight;
        }
        else
        {// 中途遇到敌方棋子
            break;
        }
    }
}


/**
* 函数功能：获取一个方向的反方向
* 参数：
*          direction : 一个方向
*          rDirection: 获取的direction的反方向
* 返回值：	无
* 注意：该函数严格按照JFDirection中枚举的方向计算，因此，如果JFDirection枚举改变
*       该方法就可能出错
*/
void JFEngine::GetRDirection(IN const JFDirection& direction, OUT JFDirection& rDirection)
{
    rDirection = static_cast<JFDirection>((direction + 4) % 8);   // 8 代表8个方向
}


/**
* 函数功能：初始化引擎数据
* 参数：空
* 返回值：空	
*/
void JFEngine::Initialize(void)
{
    // 计算x的数组的初始化
    m_vXCalc[DIRECTION_NORTH] = 0;
    m_vXCalc[DIRECTION_NORTH_EAST] = 1;
    m_vXCalc[DIRECTION_EAST] = 1;
    m_vXCalc[DIRECTION_SOUTH_EAST] = 1;
    m_vXCalc[DIRECTION_SOUTH] = 0;
    m_vXCalc[DIRECTION_SOUTH_WEST] = -1;
    m_vXCalc[DIRECTION_WEST] = -1;
    m_vXCalc[DIRECTION_NORTH_WEST] = -1;

    // 计算y的数组的初始化
    m_vYCalc[DIRECTION_NORTH] = -1;
    m_vYCalc[DIRECTION_NORTH_EAST] = -1;
    m_vYCalc[DIRECTION_EAST] = 0;
    m_vYCalc[DIRECTION_SOUTH_EAST] = 1;
    m_vYCalc[DIRECTION_SOUTH] = 1;
    m_vYCalc[DIRECTION_SOUTH_WEST] = 1;
    m_vYCalc[DIRECTION_WEST] = 0;
    m_vYCalc[DIRECTION_NORTH_WEST] = -1;
}

/**
* 函数功能：开始游戏，重置相关数据
* 参数：无
* 返回值：无	
*/
void JFEngine::StartGame(void)
{
    m_CBSituation = JFCBSituation();
    m_bestDefencePos = JFPosInCB();
    m_bestOffensePos = JFPosInCB();
    m_bestPlacePos = JFPosInCB();
}

/**
* 函数功能：获取最佳防御点
* 参数：无
* 返回值：当前棋盘上的一个空的棋盘节点的位置	
*/
const JFPosInCB& JFEngine::GetBestPosForDefence()
{
	return  m_bestDefencePos;
}


/**
* 函数功能：获取最佳进攻点
* 参数：无
* 返回值：当前棋盘上的一个空的棋盘节点的位置	
*/
const JFPosInCB& JFEngine::GetBestPosForOffense()
{
	return  m_bestOffensePos;
}


/**
* 函数功能：获取最佳落子点
* 参数：
*          CMType : 所求的最佳落子点的类型
* 返回值：当前棋盘上的一个空的棋盘节点的位置		
*/
const JFPosInCB& JFEngine::GetBestPosForPlaceCM(const JFCMType& CMType)
{

    // 首先获取当前整个棋盘上的危险点集合
    vector<JFChessman> vDangerousCM;       

    // 获取棋盘原始数据
    JFChessman vCBData[15][15];
    CopySrcCBData(&vCBData[0][0]);

    JFGameTree gameTree;
    m_bestPlacePos = gameTree.GetBestPosToPlace(CMType, &vCBData[0][0], 6);

    if (m_bestPlacePos.lX < 0 || m_bestPlacePos.lY < 0)
    {
        GetDangerNULLCM(&vCBData[0][0], 
            CMType,
            JFCBRect(0, 0, CB_GRID_NUM, CB_GRID_NUM),
            vDangerousCM );

        // 防御
        JFChessman chessman;
        if (vDangerousCM.size() > 0)
        {
            chessman = vDangerousCM[0];
            m_bestPlacePos = chessman.position;
        } 

        // 进攻
        JFCMType enemyType;
        GetEnemyCMType(CMType, enemyType);
        vDangerousCM.clear();

        JFChessman vCBData_offence[CB_GRID_NUM+1][CB_GRID_NUM+1];
        CopySrcCBData(&vCBData_offence[0][0]);
        GetDangerNULLCM(&vCBData_offence[0][0],
                        enemyType,
                        JFCBRect(0, 0, CB_GRID_NUM, CB_GRID_NUM),
                        vDangerousCM );

        if (vDangerousCM.size() > 0)
        {
            if (vDangerousCM[0].uiWeight > chessman.uiWeight)
            {
                m_bestPlacePos = vDangerousCM[0].position;
            }
        }
    }

    
	return  m_bestPlacePos;
}


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
void JFEngine::GetNULLCMByWeight(  IN JFChessman* pCBData,
                                   IN const JFCMType& type, 
                                   IN const JFCBRect& rect, 
                                   IN const jf_uint& uiWeight,
                                   OUT vector<JFChessman>& vNULLCM)
{
    vector<JFChessman> vCMType;

    GetCMInRect(pCBData, rect, type, vCMType);

    vector<JFChessman>::size_type sizeVType = vCMType.size();

    for (vector<JFChessman>::size_type i=0; i<sizeVType; ++i)
    {
        CalcTheWeight(pCBData, rect, vCMType[i]);
    }

    // 获取区域rect内的空的棋盘节点
    RetriveDangerCMByWeight(pCBData, rect, uiWeight, vNULLCM);

    sort(vNULLCM.begin(), vNULLCM.end(), JFChessman::WeightLarger);
}


/**
* 函数功能：获取棋盘上一定区域内的危险点
* 参数：
*          pCBData  : 棋盘数据
*          selfType : 棋子类型，所求的危险点是对于selfType的棋子而言，对方可能会落子的点
*          rect     : 在棋盘pCBData上的一个区域
*          vDangerousCM : 获得的危险点集合，已经按照权值从大到小排序
* 返回值：	空
*/
void JFEngine::GetDangerNULLCM(IN JFChessman* pCBData, 
                                    IN const JFCMType& selfType, 
                                    IN const JFCBRect& rect,
                                    OUT vector<JFChessman>& vDangerousCM)
{
    vector<JFChessman> vChessmanEnemyType;
    
    JFCMType enemyType = (BLACK_CM == selfType) ? WHITE_CM : BLACK_CM;

    GetCMInRect(pCBData, rect, enemyType, vChessmanEnemyType);

    vector<JFChessman>::size_type sizeVSelfType = vChessmanEnemyType.size();

    for (vector<JFChessman>::size_type i=0; i<sizeVSelfType; ++i)
    {
        CalcTheWeight(pCBData, rect, vChessmanEnemyType[i]);
    }

    // 获取区域rect内的空的棋盘节点
    RetriveDangerCMByWeight(pCBData, rect, 1, vDangerousCM);

    sort(vDangerousCM.begin(), vDangerousCM.end(), JFChessman::WeightLarger);
}


/**
* 函数功能：获取棋盘上一定区域内一定类型的棋子
* 参数：
*          pCBData : 棋盘数据
*          rect    : 在棋盘pCBData上的一个区域
*          type    : 要提取的棋子的类型
*          vCMInRect : 提取的棋子集合
* 返回值：空	
*/
void JFEngine::GetCMInRect(IN const JFChessman* pCBData, 
                           IN const JFCBRect& rect, 
                           IN const JFCMType& type, 
                           OUT vector<JFChessman>& vCMInRect)
{
    _ASSERT(rect.topleftPos.lX >= 0 
            && rect.topleftPos.lY >= 0
            && rect.bottomrightPos.lX <= CB_GRID_NUM
            && rect.bottomrightPos.lY <= CB_GRID_NUM);

    // 逐行提取
    for (jf_long32 y=rect.topleftPos.lY; y<=rect.bottomrightPos.lY; ++y)
    {
        for (jf_long32 x=rect.topleftPos.lX; x<=rect.bottomrightPos.lX; ++x)
        {
            const JFChessman& cmData = *(pCBData + x + y*(CB_GRID_NUM+1));

            if (type == cmData.type)
            {
                vCMInRect.push_back(cmData);
            }
        }
    }
}

/**
* 函数功能：获取棋盘区域内一定类型的危险点
* 参数：
*          pCBData : 棋盘数据
*          rect    : 在棋盘pCBData上的一个区域
*          uiWeight: 要提取的空余棋子节点的权值最小值
*          vDanger : 提取的棋子集合
* 返回值：空	
*/
void JFEngine::RetriveDangerCMByWeight(IN JFChessman* pCBData, 
                                         IN const JFCBRect& rect,
                                         IN const jf_uint& uiWeight,
                                         OUT vector<JFChessman>& vDanger)
{
    _ASSERT(rect.topleftPos.lX >= 0 
        && rect.topleftPos.lY >= 0
        && rect.bottomrightPos.lX <= CB_GRID_NUM
        && rect.bottomrightPos.lY <= CB_GRID_NUM);

    // 逐行提取
    for (jf_long32 y=rect.topleftPos.lY; y<=rect.bottomrightPos.lY; ++y)
    {
        for (jf_long32 x=rect.topleftPos.lX; x<=rect.bottomrightPos.lX; ++x)
        {
            const JFChessman& cmData = *(pCBData + x + y*(CB_GRID_NUM+1));

            if (cmData.type == NO_CM && cmData.uiWeight >= uiWeight)
            {
                vDanger.push_back(cmData);
            }
        }
    }
}

/**
* 函数功能：拷贝原始棋盘数据到pDest
* 参数：
*          pDest : 目的地
* 返回值：空	
*/
void JFEngine::CopySrcCBData(IN JFChessman* pDest)
{
    _ASSERT(pDest != NULL);

    const JFChessman* pSrcCBData = JFSysData::GetSysData()->GetCBData();

    memcpy(pDest, pSrcCBData, (CB_GRID_NUM+1)*(CB_GRID_NUM+1)*sizeof(JFChessman));
}


/**
* 函数功能：计算棋子chessman在棋盘pCBData上的区域rect内产生的权值
*           这些权值被加到rect内的被影响到的空棋盘节点的权值中
* 参数：
*          pCBData  : 棋盘数据
*          rect     : 棋盘pCBData内的一块区域
*          chessman : 棋盘pCBData上的一颗棋子
* 返回值：	空
*/
void JFEngine::CalcTheWeight(IN JFChessman* pCBData, const JFCBRect& rect, const JFChessman& chessman)
{
    // 对chessman周围8个方向上的空棋盘节点求其权值
    for (size_t i=DIRECTION_NORTH; i<=DIRECTION_NORTH_WEST; ++i)
    {
        DoCalcWeight(pCBData, rect, chessman, static_cast<JFDirection>(i), 4);
    }
}

/**
* 函数功能：获取类型type的棋子导致的棋局
* 参数：
*          type      : 棋子类型
*          situation : 棋局数据
* 返回值：	空
*/
void JFEngine::GetCurSituation(IN const JFCMType& type, OUT JFCBSituation& situation)
{
    if (NO_CM == type)
    {
        situation.typeWin = NO_CM;
        return;
    }

    const vector<jf_ushort>& vLineData = JFSysData::GetSysData()->GetLineData(type);
    size_t uiLineDataSize = vLineData.size();
    
    for (size_t ui=0; ui<uiLineDataSize; ++ui)
    {
        // 计算当前棋局
        jf_ushort usFiveData = 31;  // (11111) 2进制数

        const jf_ushort& usLineData = vLineData[ui];

        // 执行11次运算
        // 先左移一位，因为最后以为没有数据。15--16
        usFiveData <<= 1;    // 左移1位
        for (int count=0; count<11; ++count)
        {
            if ((usLineData & usFiveData) == usFiveData)    // 说明有五个子连珠
            {
                m_CBSituation.typeWin = type;
                situation = m_CBSituation;
                return ;
            }
            usFiveData <<= 1;    // 左移1位
        }
    }   
}
