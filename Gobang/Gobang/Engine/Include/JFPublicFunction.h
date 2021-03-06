/** 
 * 文 件 名: JFPublicFunction.h
 * 摘    要: 本文件包含了一些公用的函数的定义
 *
 * 作    者: 姜峰
 * 创建时间: 2010.05.15
 */

#ifndef JFPUBLICFUNCTION_H_
#define JFPUBLICFUNCTION_H_

#include "JFType.h"
#include "JFStruct.h"

/**
 * 函数功能：拷贝棋盘数据
 * 参数：
 *          pDest : 目的地
 *          pSrc  : 原始数据所在地
 * 返回值：	无
 */
inline void CopyCBData(IN JFChessman* pDest, IN JFChessman* pSrc)
{
    _ASSERT(pDest != NULL && pSrc != NULL);

    memcpy(pDest, pSrc, (CB_GRID_NUM+1)*(CB_GRID_NUM+1)*sizeof(JFChessman));
}

/**
* 函数功能：获取类型为selfType的棋子的敌方的棋子类型
* 参数：
*          selfType  : 我棋子的类型
*          enemyType : 敌方棋子的类型
* 返回值：	无
*/
inline void GetEnemyCMType(IN const JFCMType& selfType, OUT JFCMType& enemyType)
{
    _ASSERT(NO_CM != selfType);

    enemyType = (BLACK_CM == selfType) ? WHITE_CM : BLACK_CM ;
}

/**
* 函数功能：判断棋盘节点pos是否在棋盘区域rect内(包括在边缘上)
* 参数：
*          pos  : 棋盘节点
*          rect : 棋盘区域
* 返回值：	
*          true : 如果棋盘节点在棋盘区域内
*          false : 棋盘节点不在棋盘内
*/
inline jf_bool IsPosInCBRect(IN const JFPosInCB& pos, IN const JFCBRect& rect)
{
    return (pos.lX <= rect.bottomrightPos.lX 
            && pos.lX  >= rect.topleftPos.lX
            && pos.lY  <= rect.bottomrightPos.lY
            && pos.lY  >= rect.topleftPos.lY);
}


/**
 * 函数功能：向线数据中添加棋子数据
 * 参数：
 *          vLineData : 线数据，内存已经分配好，元素至少有88个
 * 返回值：	
 *          true  : 线数据添加成功
 *          false : 线数据添加失败 
 */
/*******************************************************************
* 黑白子线数据总共有：15+15+29*2 = 88
* 点P(x,y)所属的线定义如下：
* 0-14  行：[y] 
* 15-29 列：[x]
* 30-58 /线：[x+y]
* 59-87 \线：[x+y+(x>y)*14]
*******************************************************************/
inline void AddCMToLineData(INOUT vector<jf_ushort>& vLineData, IN const JFChessman& chessman)
{
    _ASSERT(vLineData.size() >= 88);
    JFCBRect CBRect(0, 0, 14, 14);
    _ASSERT(IsPosInCBRect(chessman.position, CBRect));

    // 将棋子表示成线数据
    // 所属行
    jf_int32 iRow = chessman.position.lY;
    jf_int32 indexRow = chessman.position.lX;               // 在该线数据中的下标
    // 所属列
    jf_int32 iCol = 15 + chessman.position.lX;
    jf_int32 indexCol = chessman.position.lY;               // 在该线数据中的下标
    // 所属/线
    jf_int32 iForwardSlash = 30 + (chessman.position.lX + chessman.position.lY);
    jf_int32 indexForwardSlash = chessman.position.lX;      // 在该线数据中的下标
    // 所属\线
    jf_int32 iBackwardSlash = 59 + (chessman.position.lX - chessman.position.lY + CB_GRID_NUM);
    jf_int32 indexBackwardSlash = chessman.position.lX;     // 在该线数据中的下标


    // 添加到--数据
    // 棋盘左边的点表示成数据应该在高位
    jf_ushort& RowData = vLineData[iRow];
    jf_ushort usRowTmpData = 1;
    usRowTmpData <<= (CB_GRID_NUM + 1 - indexRow); 
    // 当前此点应该是没有数据的
    _ASSERT((RowData & usRowTmpData) != usRowTmpData);
    RowData |= usRowTmpData;

    // 添加到|数据
    jf_ushort& ColData = vLineData[iCol];
    jf_ushort usColTmpData = 1;
    usColTmpData <<= (CB_GRID_NUM + 1 - indexCol); 
    // 当前此点应该是没有数据的
    _ASSERT((ColData & usColTmpData) != usColTmpData);
    ColData |= usColTmpData;

    // 添加到/线数据
    jf_ushort& ForwardData = vLineData[iForwardSlash];
    jf_ushort usForwardTmpData = 1;
    usForwardTmpData <<= (CB_GRID_NUM + 1 - indexForwardSlash);
    // 当前此点应该是没有数据的
    _ASSERT((ForwardData & usForwardTmpData) != usForwardTmpData);
    ForwardData |= usForwardTmpData;

    // 添加到\线数据
    jf_ushort& BackwardData = vLineData[iBackwardSlash];
    jf_ushort usBackwardTmpData = 1;
    usBackwardTmpData <<= (CB_GRID_NUM + 1 - indexBackwardSlash);
    // 当前此点应该是没有数据的
    _ASSERT((BackwardData & usBackwardTmpData) != usBackwardTmpData);
    BackwardData |= usBackwardTmpData;
}

/**
 * 函数功能：获取棋盘上满足模式usPattern的几个棋子中的空余节点的位置
 * 参数：
 *          vLineData : 线数据
 *          usPattern : 模式数字
 *          uiZeroPos : 模式数字中空余节点的位置
 * 返回值：	
 */
inline JFPosInCB GetThePosOfPattern(IN vector<jf_ushort>& vLineData, IN jf_ushort usAPattern, IN const jf_uint& uiZeroPos)
{
    _ASSERT(vLineData.size() >= 88);
    jf_int32 iIndexInVLineData = -1;            // 在线数据集合中的位置
    jf_int32 iIndexInData = -1;                 // 获取在一个线数据中的位置

    jf_uint uiLineDataSize = vLineData.size();
    for (size_t uiLineData=0; uiLineData<uiLineDataSize; ++uiLineData)
    {
        jf_ushort usPattern = usAPattern;
        const jf_ushort& usLineData = vLineData[uiLineData];

        // 执行10次运算
        // 先左移一位，因为最后一位没有数据。15--16
        // 最后一位是0
        usPattern <<= 1;    // 左移1位

        for (int count=0; count<10; ++count)
        {
            if ((usLineData & usPattern) == usPattern)    // 说明有这样的模式
            {
                iIndexInVLineData = uiLineData;

                // 在15个棋子节点中的位置
                //      01234   012345678901234
                // 例如 11011 在000000000011011中的位置为12
                iIndexInData = CB_GRID_NUM - (count + uiZeroPos);

                break;
            }
            usPattern <<= 1;    // 左移1位
        }
    }

    JFPosInCB posReturn;        // 待返回的点
    posReturn.lX = iIndexInData;

    if (-1 == iIndexInData || -1 == iIndexInVLineData)
    {
        return JFPosInCB(-1, -1);
    }
    else if (iIndexInVLineData >= 59)       // \线数据
    {
        // 59 + (chessman.position.lX - chessman.position.lY + CB_GRID_NUM)
        posReturn.lY = posReturn.lX + CB_GRID_NUM - iIndexInVLineData - 59;
    }
    else if (iIndexInVLineData >= 30)       // /线数据
    {
        // 30 + (chessman.position.lX + chessman.position.lY)
        posReturn.lY = iIndexInVLineData - posReturn.lX - 30;
    }
    else if (iIndexInVLineData >= 15)       // |线数据
    {
        // 15 + chessman.position.lX
        posReturn.lX = iIndexInVLineData - 15;
        posReturn.lY = iIndexInData;
    }
    else if (iIndexInVLineData >= 0)       // 一线数据
    {
        // chessman.position.lY
        posReturn.lY = iIndexInVLineData;
    }
    else
    {
        _ASSERT(false);
    }

    return posReturn;
}

/**
 * 函数功能：通过棋盘数据，构造类型为type的棋子的线数据
 * 参数：
 *              pCBData : 棋盘数据
 *              type    : 棋子类型
 *              vlineData : 构造的线数据
 * 返回值：	
 *              true : 线数据构造成功
 *              false: 线数据构造失败
 */
inline jf_bool CreateLineData(IN const JFChessman* pCBData,
                              IN const JFCMType& type,
                              OUT vector<jf_ushort>& vLineData)
{
    _ASSERT(pCBData != NULL && type != NO_CM);

    // 逐行提取
    for (jf_long32 row=0; row<=CB_GRID_NUM; ++row)
    {
        for (jf_long32 col=0; col<=CB_GRID_NUM; ++col)
        {
            const JFChessman& cmData = *(pCBData + col + row*(CB_GRID_NUM+1));
            
            if (type == cmData.type)
            {
                AddCMToLineData(vLineData, cmData);
            }  
        }
    }

    return true;
} 

/**
* 函数功能：将给定的棋子添加到线数据
* 参数：
*              vChessman : 棋盘集合
*              vlineData : 构造的线数据
* 返回值：	
*              true : 添加线数据成功
*              false: 添加线数据失败
*/
inline jf_bool AddCMToLineData(IN const vector<JFChessman>& vChessman,
                               INOUT vector<jf_ushort>& vLineData)
{
    _ASSERT(vLineData.size() >= 0);

    size_t uiSize = vLineData.size();
    for (size_t ui=0; ui<uiSize; ++ui)
    {
        _ASSERT(vChessman[ui].type != NO_CM);
        AddCMToLineData(vLineData, vChessman[ui]);
    }
}

/**
* 函数功能：将给定的棋子添加到棋盘数据中
* 参数：
*              pCBData   : 棋盘数据指针
*              chessman : 待添加的棋子
* 返回值：	
*              true : 添加线数据成功
*              false: 添加线数据失败
*/
inline void AddCMToCBData(IN JFChessman* pCBData,
                          OUT const JFChessman& chessman)
{
    _ASSERT(pCBData != NULL);

    _ASSERT(chessman.type != NO_CM);

    JFChessman& cmData = *(pCBData + chessman.position.lX + chessman.position.lY*(CB_GRID_NUM + 1));
    cmData = chessman;

}

/**
* 函数功能：将给定的棋子添加到棋盘数据中
* 参数：
*              pCBData   : 棋盘数据指针
*              vChessman : 待添加的棋子集合
* 返回值：	
*              true : 添加线数据成功
*              false: 添加线数据失败
*/
inline void AddCMToCBData(IN JFChessman* pCBData,
                          IN const vector<JFChessman>& vChessman)
{
    _ASSERT(pCBData != NULL);

    size_t uiSize = vChessman.size();
    for (size_t ui=0; ui<uiSize; ++ui)
    {
        _ASSERT(vChessman[ui].type != NO_CM);
        
        JFChessman& cmData = *(pCBData + vChessman[ui].position.lX + vChessman[ui].position.lY*(CB_GRID_NUM + 1));
        cmData = vChessman[ui];
    }
}

/**
 * 函数功能：统计线数据中vusPattern中的pattern数据出现的次数
 * 参数：
 *          vusPattern : 模式数据集合
 *          vLineData  : 线数据
 * 返回值：	
 */
inline jf_uint CheckPatternNum(IN const vector<jf_ushort>& vusPattern, IN const vector<jf_ushort>& vLineData)
{
    _ASSERT(vLineData.size() >= 88);
    jf_uint uiAcount = 0;

    jf_uint uiPatternSize = vusPattern.size();
    for (jf_uint uiPattern=0; uiPattern<uiPatternSize; ++uiPattern)
    {
        jf_uint uiLineDataSize = vLineData.size();
        for (size_t uiLineData=0; uiLineData<uiLineDataSize; ++uiLineData)
        {
            jf_ushort usPattern = vusPattern[uiPattern];

            const jf_ushort& usLineData = vLineData[uiLineData];

            // 执行10次运算
            // 先左移一位，因为最后一位没有数据。15--16
            // 最后一位是0
            usPattern <<= 1;    // 左移1位

            for (int count=0; count<10; ++count)
            {
                if ((usLineData & usPattern) == usPattern)    // 说明有这样的模式
                {
                    uiAcount++;
                }
                usPattern <<= 1;    // 左移1位
            }
        }  

    }

    return uiAcount;
}

/**
 * 函数功能：判断当前是否应该防守
 * 参数：
 *           vEnemyDangerCM : 对手造成的危险点集合
 * 返回值：	
 */
inline jf_bool ShouldDefence(vector<JFChessman> vEnemyDangerCM)
{
    jf_int32 iSize = vEnemyDangerCM.size();

    if (iSize <= 1)
    {
        return false;
    }
    else
    {
        jf_uint uiWeightSum = vEnemyDangerCM[0].uiWeight +  vEnemyDangerCM[1].uiWeight;

        if (uiWeightSum >= 12)
        {
            return true;
        }
    }

    return false;
}

#endif