/**********************************************************************************
 *  JFEngine.h
 *  Implementation of the Class JFEngine
 *  Created on:      26-����-2010 15:26:51
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
* �������ܣ���ȡָ������ָ��
* ��������
* ����ֵ��ָ������ָ��	
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
* �������ܣ�����ִ��Ȩֵ����
* ������
*          pCBData     : ��������
*          rect        : ��������
*          chessman    : ĳ�����ӣ��Ը�����Ϊ�ο�
*          direction   : ����
*          iRange      : Σ�հ뾶
* ����ֵ�� ��	
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
    jf_bool bNullInside = false;        // ��ʶ������ӽڵ��м��Ƿ��пսڵ�
    jf_uint uiNullNode = 0;        // ��;�����Ŀսڵ�ĸ���������2�����˳�

    for (int i=0; i<iRange; ++i)
    {
        lCurX += m_vXCalc[direction];   // �ƶ���
        lCurY += m_vYCalc[direction];

        /*******************************************
         * �жϵ��Ƿ���������Ӧ���ڴ˴��ж�
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
        {// �սڵ�
            
            if (uiNullNode >= 1)
            {
                break;
            }

            uiNullNode++;
            bNullInside = true;
            cmData.uiWeight += uiCurWeight;
        }
        else if (cmData.type == chessman.type)
        {// ��;����������ͬ�����ӣ�Ȩֵ��1
            ++uiCurWeight; 

            if (bNullInside)
            {// ���ǰ���пսڵ㣬����ȨֵӦ���ټ�
                JFDirection rDirection;
                GetRDirection(direction, rDirection);
                DoCalcWeightBack(pCBData, rect, cmData, rDirection, i);
            }

            cmData.uiWeight += uiCurWeight;
        }
        else
        {// ��;�����з�����
            break;
        }
    }
}


/**
* �������ܣ�����;�����սڵ�ʱ����������Ȩֵʱ���øú���
* ������
*          pCBData     : ��������
*          rect        : ��������
*          chessman    : ĳ�����ӣ��Ը�����Ϊ�ο�
*          direction   : ����
*          iRange      : Σ�հ뾶
* ����ֵ�� ��	
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
    jf_uint uiNullNode = 0;        // ��;�����Ŀսڵ�ĸ���������2�����˳�

    for (int i=0; i<iRange && IsPosInCBRect(JFPosInCB(lCurX, lCurY), rect); ++i)
    {
        lCurX += m_vXCalc[direction];   // �ƶ���
        lCurY += m_vYCalc[direction];

        JFChessman& cmData = *(pCBData+lCurX+lCurY*(CB_GRID_NUM+1));

        if (cmData.type == NO_CM)
        {// �սڵ�
            
            if (uiNullNode >= 1)
            {
                break;
            }

            cmData.uiWeight += uiCurWeight;
            uiNullNode++;
        }
        else if (cmData.type == chessman.type)
        {// ��;����������ͬ�����ӣ�Ȩֵ��1
            ++uiCurWeight; 

            cmData.uiWeight += uiCurWeight;
        }
        else
        {// ��;�����з�����
            break;
        }
    }
}


/**
* �������ܣ���ȡһ������ķ�����
* ������
*          direction : һ������
*          rDirection: ��ȡ��direction�ķ�����
* ����ֵ��	��
* ע�⣺�ú����ϸ���JFDirection��ö�ٵķ�����㣬��ˣ����JFDirectionö�ٸı�
*       �÷����Ϳ��ܳ���
*/
void JFEngine::GetRDirection(IN const JFDirection& direction, OUT JFDirection& rDirection)
{
    rDirection = static_cast<JFDirection>((direction + 4) % 8);   // 8 ����8������
}


/**
* �������ܣ���ʼ����������
* ��������
* ����ֵ����	
*/
void JFEngine::Initialize(void)
{
    // ����x������ĳ�ʼ��
    m_vXCalc[DIRECTION_NORTH] = 0;
    m_vXCalc[DIRECTION_NORTH_EAST] = 1;
    m_vXCalc[DIRECTION_EAST] = 1;
    m_vXCalc[DIRECTION_SOUTH_EAST] = 1;
    m_vXCalc[DIRECTION_SOUTH] = 0;
    m_vXCalc[DIRECTION_SOUTH_WEST] = -1;
    m_vXCalc[DIRECTION_WEST] = -1;
    m_vXCalc[DIRECTION_NORTH_WEST] = -1;

    // ����y������ĳ�ʼ��
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
* �������ܣ���ʼ��Ϸ�������������
* ��������
* ����ֵ����	
*/
void JFEngine::StartGame(void)
{
    m_CBSituation = JFCBSituation();
    m_bestDefencePos = JFPosInCB();
    m_bestOffensePos = JFPosInCB();
    m_bestPlacePos = JFPosInCB();
}

/**
* �������ܣ���ȡ��ѷ�����
* ��������
* ����ֵ����ǰ�����ϵ�һ���յ����̽ڵ��λ��	
*/
const JFPosInCB& JFEngine::GetBestPosForDefence()
{
	return  m_bestDefencePos;
}


/**
* �������ܣ���ȡ��ѽ�����
* ��������
* ����ֵ����ǰ�����ϵ�һ���յ����̽ڵ��λ��	
*/
const JFPosInCB& JFEngine::GetBestPosForOffense()
{
	return  m_bestOffensePos;
}


/**
* �������ܣ���ȡ������ӵ�
* ������
*          CMType : �����������ӵ������
* ����ֵ����ǰ�����ϵ�һ���յ����̽ڵ��λ��		
*/
const JFPosInCB& JFEngine::GetBestPosForPlaceCM(const JFCMType& CMType)
{

    // ���Ȼ�ȡ��ǰ���������ϵ�Σ�յ㼯��
    vector<JFChessman> vDangerousCM;       

    // ��ȡ����ԭʼ����
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

        // ����
        JFChessman chessman;
        if (vDangerousCM.size() > 0)
        {
            chessman = vDangerousCM[0];
            m_bestPlacePos = chessman.position;
        } 

        // ����
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
* �������ܣ�����Ȩֵ��Сֵ�޶���ȡ������һ�������ڵĿ���ڵ�
*           ��ЩȨֵ������������Ϊtype��������ɵġ�
* ������
*          pCBData  : ��������
*          type     : �������ͣ�����Ŀ���ڵ���type��������ɵ�
*          rect     : ������pCBData�ϵ�һ������
*          weight   : Ҫ��ȡ�Ŀ������̽ڵ��Ȩֵ
*          vNULLCM  : ��õĿ���ڵ㼯�ϣ��Ѿ�����Ȩֵ�Ӵ�С����
* ����ֵ��	��
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

    // ��ȡ����rect�ڵĿյ����̽ڵ�
    RetriveDangerCMByWeight(pCBData, rect, uiWeight, vNULLCM);

    sort(vNULLCM.begin(), vNULLCM.end(), JFChessman::WeightLarger);
}


/**
* �������ܣ���ȡ������һ�������ڵ�Σ�յ�
* ������
*          pCBData  : ��������
*          selfType : �������ͣ������Σ�յ��Ƕ���selfType�����Ӷ��ԣ��Է����ܻ����ӵĵ�
*          rect     : ������pCBData�ϵ�һ������
*          vDangerousCM : ��õ�Σ�յ㼯�ϣ��Ѿ�����Ȩֵ�Ӵ�С����
* ����ֵ��	��
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

    // ��ȡ����rect�ڵĿյ����̽ڵ�
    RetriveDangerCMByWeight(pCBData, rect, 1, vDangerousCM);

    sort(vDangerousCM.begin(), vDangerousCM.end(), JFChessman::WeightLarger);
}


/**
* �������ܣ���ȡ������һ��������һ�����͵�����
* ������
*          pCBData : ��������
*          rect    : ������pCBData�ϵ�һ������
*          type    : Ҫ��ȡ�����ӵ�����
*          vCMInRect : ��ȡ�����Ӽ���
* ����ֵ����	
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

    // ������ȡ
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
* �������ܣ���ȡ����������һ�����͵�Σ�յ�
* ������
*          pCBData : ��������
*          rect    : ������pCBData�ϵ�һ������
*          uiWeight: Ҫ��ȡ�Ŀ������ӽڵ��Ȩֵ��Сֵ
*          vDanger : ��ȡ�����Ӽ���
* ����ֵ����	
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

    // ������ȡ
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
* �������ܣ�����ԭʼ�������ݵ�pDest
* ������
*          pDest : Ŀ�ĵ�
* ����ֵ����	
*/
void JFEngine::CopySrcCBData(IN JFChessman* pDest)
{
    _ASSERT(pDest != NULL);

    const JFChessman* pSrcCBData = JFSysData::GetSysData()->GetCBData();

    memcpy(pDest, pSrcCBData, (CB_GRID_NUM+1)*(CB_GRID_NUM+1)*sizeof(JFChessman));
}


/**
* �������ܣ���������chessman������pCBData�ϵ�����rect�ڲ�����Ȩֵ
*           ��ЩȨֵ���ӵ�rect�ڵı�Ӱ�쵽�Ŀ����̽ڵ��Ȩֵ��
* ������
*          pCBData  : ��������
*          rect     : ����pCBData�ڵ�һ������
*          chessman : ����pCBData�ϵ�һ������
* ����ֵ��	��
*/
void JFEngine::CalcTheWeight(IN JFChessman* pCBData, const JFCBRect& rect, const JFChessman& chessman)
{
    // ��chessman��Χ8�������ϵĿ����̽ڵ�����Ȩֵ
    for (size_t i=DIRECTION_NORTH; i<=DIRECTION_NORTH_WEST; ++i)
    {
        DoCalcWeight(pCBData, rect, chessman, static_cast<JFDirection>(i), 4);
    }
}

/**
* �������ܣ���ȡ����type�����ӵ��µ����
* ������
*          type      : ��������
*          situation : �������
* ����ֵ��	��
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
        // ���㵱ǰ���
        jf_ushort usFiveData = 31;  // (11111) 2������

        const jf_ushort& usLineData = vLineData[ui];

        // ִ��11������
        // ������һλ����Ϊ�����Ϊû�����ݡ�15--16
        usFiveData <<= 1;    // ����1λ
        for (int count=0; count<11; ++count)
        {
            if ((usLineData & usFiveData) == usFiveData)    // ˵�������������
            {
                m_CBSituation.typeWin = type;
                situation = m_CBSituation;
                return ;
            }
            usFiveData <<= 1;    // ����1λ
        }
    }   
}
