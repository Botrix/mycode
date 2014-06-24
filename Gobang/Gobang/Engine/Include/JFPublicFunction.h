/** 
 * �� �� ��: JFPublicFunction.h
 * ժ    Ҫ: ���ļ�������һЩ���õĺ����Ķ���
 *
 * ��    ��: ����
 * ����ʱ��: 2010.05.15
 */

#ifndef JFPUBLICFUNCTION_H_
#define JFPUBLICFUNCTION_H_

#include "JFType.h"
#include "JFStruct.h"

/**
 * �������ܣ�������������
 * ������
 *          pDest : Ŀ�ĵ�
 *          pSrc  : ԭʼ�������ڵ�
 * ����ֵ��	��
 */
inline void CopyCBData(IN JFChessman* pDest, IN JFChessman* pSrc)
{
    _ASSERT(pDest != NULL && pSrc != NULL);

    memcpy(pDest, pSrc, (CB_GRID_NUM+1)*(CB_GRID_NUM+1)*sizeof(JFChessman));
}

/**
* �������ܣ���ȡ����ΪselfType�����ӵĵз�����������
* ������
*          selfType  : �����ӵ�����
*          enemyType : �з����ӵ�����
* ����ֵ��	��
*/
inline void GetEnemyCMType(IN const JFCMType& selfType, OUT JFCMType& enemyType)
{
    _ASSERT(NO_CM != selfType);

    enemyType = (BLACK_CM == selfType) ? WHITE_CM : BLACK_CM ;
}

/**
* �������ܣ��ж����̽ڵ�pos�Ƿ�����������rect��(�����ڱ�Ե��)
* ������
*          pos  : ���̽ڵ�
*          rect : ��������
* ����ֵ��	
*          true : ������̽ڵ�������������
*          false : ���̽ڵ㲻��������
*/
inline jf_bool IsPosInCBRect(IN const JFPosInCB& pos, IN const JFCBRect& rect)
{
    return (pos.lX <= rect.bottomrightPos.lX 
            && pos.lX  >= rect.topleftPos.lX
            && pos.lY  <= rect.bottomrightPos.lY
            && pos.lY  >= rect.topleftPos.lY);
}


/**
 * �������ܣ�����������������������
 * ������
 *          vLineData : �����ݣ��ڴ��Ѿ�����ã�Ԫ��������88��
 * ����ֵ��	
 *          true  : ���������ӳɹ�
 *          false : ����������ʧ�� 
 */
/*******************************************************************
* �ڰ����������ܹ��У�15+15+29*2 = 88
* ��P(x,y)�������߶������£�
* 0-14  �У�[y] 
* 15-29 �У�[x]
* 30-58 /�ߣ�[x+y]
* 59-87 \�ߣ�[x+y+(x>y)*14]
*******************************************************************/
inline void AddCMToLineData(INOUT vector<jf_ushort>& vLineData, IN const JFChessman& chessman)
{
    _ASSERT(vLineData.size() >= 88);
    JFCBRect CBRect(0, 0, 14, 14);
    _ASSERT(IsPosInCBRect(chessman.position, CBRect));

    // �����ӱ�ʾ��������
    // ������
    jf_int32 iRow = chessman.position.lY;
    jf_int32 indexRow = chessman.position.lX;               // �ڸ��������е��±�
    // ������
    jf_int32 iCol = 15 + chessman.position.lX;
    jf_int32 indexCol = chessman.position.lY;               // �ڸ��������е��±�
    // ����/��
    jf_int32 iForwardSlash = 30 + (chessman.position.lX + chessman.position.lY);
    jf_int32 indexForwardSlash = chessman.position.lX;      // �ڸ��������е��±�
    // ����\��
    jf_int32 iBackwardSlash = 59 + (chessman.position.lX - chessman.position.lY + CB_GRID_NUM);
    jf_int32 indexBackwardSlash = chessman.position.lX;     // �ڸ��������е��±�


    // ���ӵ�--����
    // ������ߵĵ��ʾ������Ӧ���ڸ�λ
    jf_ushort& RowData = vLineData[iRow];
    jf_ushort usRowTmpData = 1;
    usRowTmpData <<= (CB_GRID_NUM + 1 - indexRow); 
    // ��ǰ�˵�Ӧ����û�����ݵ�
    _ASSERT((RowData & usRowTmpData) != usRowTmpData);
    RowData |= usRowTmpData;

    // ���ӵ�|����
    jf_ushort& ColData = vLineData[iCol];
    jf_ushort usColTmpData = 1;
    usColTmpData <<= (CB_GRID_NUM + 1 - indexCol); 
    // ��ǰ�˵�Ӧ����û�����ݵ�
    _ASSERT((ColData & usColTmpData) != usColTmpData);
    ColData |= usColTmpData;

    // ���ӵ�/������
    jf_ushort& ForwardData = vLineData[iForwardSlash];
    jf_ushort usForwardTmpData = 1;
    usForwardTmpData <<= (CB_GRID_NUM + 1 - indexForwardSlash);
    // ��ǰ�˵�Ӧ����û�����ݵ�
    _ASSERT((ForwardData & usForwardTmpData) != usForwardTmpData);
    ForwardData |= usForwardTmpData;

    // ���ӵ�\������
    jf_ushort& BackwardData = vLineData[iBackwardSlash];
    jf_ushort usBackwardTmpData = 1;
    usBackwardTmpData <<= (CB_GRID_NUM + 1 - indexBackwardSlash);
    // ��ǰ�˵�Ӧ����û�����ݵ�
    _ASSERT((BackwardData & usBackwardTmpData) != usBackwardTmpData);
    BackwardData |= usBackwardTmpData;
}

/**
 * �������ܣ���ȡ����������ģʽusPattern�ļ��������еĿ���ڵ��λ��
 * ������
 *          vLineData : ������
 *          usPattern : ģʽ����
 *          uiZeroPos : ģʽ�����п���ڵ��λ��
 * ����ֵ��	
 */
inline JFPosInCB GetThePosOfPattern(IN vector<jf_ushort>& vLineData, IN jf_ushort usAPattern, IN const jf_uint& uiZeroPos)
{
    _ASSERT(vLineData.size() >= 88);
    jf_int32 iIndexInVLineData = -1;            // �������ݼ����е�λ��
    jf_int32 iIndexInData = -1;                 // ��ȡ��һ���������е�λ��

    jf_uint uiLineDataSize = vLineData.size();
    for (size_t uiLineData=0; uiLineData<uiLineDataSize; ++uiLineData)
    {
        jf_ushort usPattern = usAPattern;
        const jf_ushort& usLineData = vLineData[uiLineData];

        // ִ��10������
        // ������һλ����Ϊ���һλû�����ݡ�15--16
        // ���һλ��0
        usPattern <<= 1;    // ����1λ

        for (int count=0; count<10; ++count)
        {
            if ((usLineData & usPattern) == usPattern)    // ˵����������ģʽ
            {
                iIndexInVLineData = uiLineData;

                // ��15�����ӽڵ��е�λ��
                //      01234   012345678901234
                // ���� 11011 ��000000000011011�е�λ��Ϊ12
                iIndexInData = CB_GRID_NUM - (count + uiZeroPos);

                break;
            }
            usPattern <<= 1;    // ����1λ
        }
    }

    JFPosInCB posReturn;        // �����صĵ�
    posReturn.lX = iIndexInData;

    if (-1 == iIndexInData || -1 == iIndexInVLineData)
    {
        return JFPosInCB(-1, -1);
    }
    else if (iIndexInVLineData >= 59)       // \������
    {
        // 59 + (chessman.position.lX - chessman.position.lY + CB_GRID_NUM)
        posReturn.lY = posReturn.lX + CB_GRID_NUM - iIndexInVLineData - 59;
    }
    else if (iIndexInVLineData >= 30)       // /������
    {
        // 30 + (chessman.position.lX + chessman.position.lY)
        posReturn.lY = iIndexInVLineData - posReturn.lX - 30;
    }
    else if (iIndexInVLineData >= 15)       // |������
    {
        // 15 + chessman.position.lX
        posReturn.lX = iIndexInVLineData - 15;
        posReturn.lY = iIndexInData;
    }
    else if (iIndexInVLineData >= 0)       // һ������
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
 * �������ܣ�ͨ���������ݣ���������Ϊtype�����ӵ�������
 * ������
 *              pCBData : ��������
 *              type    : ��������
 *              vlineData : �����������
 * ����ֵ��	
 *              true : �����ݹ���ɹ�
 *              false: �����ݹ���ʧ��
 */
inline jf_bool CreateLineData(IN const JFChessman* pCBData,
                              IN const JFCMType& type,
                              OUT vector<jf_ushort>& vLineData)
{
    _ASSERT(pCBData != NULL && type != NO_CM);

    // ������ȡ
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
* �������ܣ����������������ӵ�������
* ������
*              vChessman : ���̼���
*              vlineData : �����������
* ����ֵ��	
*              true : ���������ݳɹ�
*              false: ����������ʧ��
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
* �������ܣ����������������ӵ�����������
* ������
*              pCBData   : ��������ָ��
*              chessman : �����ӵ�����
* ����ֵ��	
*              true : ���������ݳɹ�
*              false: ����������ʧ��
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
* �������ܣ����������������ӵ�����������
* ������
*              pCBData   : ��������ָ��
*              vChessman : �����ӵ����Ӽ���
* ����ֵ��	
*              true : ���������ݳɹ�
*              false: ����������ʧ��
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
 * �������ܣ�ͳ����������vusPattern�е�pattern���ݳ��ֵĴ���
 * ������
 *          vusPattern : ģʽ���ݼ���
 *          vLineData  : ������
 * ����ֵ��	
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

            // ִ��10������
            // ������һλ����Ϊ���һλû�����ݡ�15--16
            // ���һλ��0
            usPattern <<= 1;    // ����1λ

            for (int count=0; count<10; ++count)
            {
                if ((usLineData & usPattern) == usPattern)    // ˵����������ģʽ
                {
                    uiAcount++;
                }
                usPattern <<= 1;    // ����1λ
            }
        }  

    }

    return uiAcount;
}

/**
 * �������ܣ��жϵ�ǰ�Ƿ�Ӧ�÷���
 * ������
 *           vEnemyDangerCM : ������ɵ�Σ�յ㼯��
 * ����ֵ��	
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