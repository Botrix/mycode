/**********************************************************************************
 *  JFEngine.h
 *  Implementation of the Class JFEngine
 *  Created on:      26-����-2010 15:26:50
 *  Original author: Jiang Feng
 *********************************************************************************/

#if !defined(EA_43F6C58C_8FB2_477f_8C4A_C252D1C4C293__INCLUDED_)
#define EA_43F6C58C_8FB2_477f_8C4A_C252D1C4C293__INCLUDED_

#include "JFStruct.h"
#include "JFSysData.h"

#define DANGER_RANGE 4              // ��Σ�յ�ʱ���뾶Ϊ4����
#define FORWARD_DETECT_STEP 1       // ��ģʽ����ǰԤ�ⲽ��Ϊ1


/**
 * ����������
 */
class JFEngine
{

public:
	
	virtual ~JFEngine();

    /**
     * �������ܣ���ȡָ������ָ��
     * ��������
     * ����ֵ��ָ������ָ��	
     */
    static JFEngine* GetEngine(void);

    /**
    * �������ܣ���ʼ����������
    * ��������
    * ����ֵ����	
    */
    void Initialize(void);

    /**
    * �������ܣ���ʼ��Ϸ�������������
    * ��������
    * ����ֵ����	
    */
    void StartGame(void);

    /**
     * �������ܣ���ȡ��ѷ�����
     * ��������
     * ����ֵ����ǰ�����ϵ�һ���յ����̽ڵ��λ��	
     */
	const JFPosInCB& GetBestPosForDefence();

    /**
     * �������ܣ���ȡ��ѽ�����
     * ��������
     * ����ֵ����ǰ�����ϵ�һ���յ����̽ڵ��λ��	
     */
	const JFPosInCB& GetBestPosForOffense();

    /**
     * �������ܣ���ȡ������ӵ�
     * ������
     *          CMType : �����������ӵ������
     * ����ֵ����ǰ�����ϵ�һ���յ����̽ڵ��λ��		
     */
	const JFPosInCB& GetBestPosForPlaceCM(const JFCMType& CMType);

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
    void GetNULLCMByWeight( IN JFChessman* pCBData,
                            IN const JFCMType& type, 
                            IN const JFCBRect& rect, 
                            IN const jf_uint& uiWeight,
                            OUT vector<JFChessman>& vNULLCM);
	 
    /**
     * �������ܣ���ȡ����type�����ӵ��µ����
     * ������
     *          type      : ��������
     *          situation : �������
     * ����ֵ��	��
     */
	void GetCurSituation(IN const JFCMType& type, OUT JFCBSituation& situation);

private:
    // ˽�л����캯����ʵ�ֵ���ģʽ
    JFEngine();

    /**
    * �������ܣ���ȡ������һ�������ڵ�Σ�յ�
    * ������
    *          pCBData  : ��������
    *          selfType : �������ͣ������Σ�յ��Ƕ���selfType�����Ӷ��ԣ��Է����ܻ����ӵĵ�
    *          rect     : ������pCBData�ϵ�һ������
    *          vDangerousCM : ��õ�Σ�յ㼯�ϣ��Ѿ�����Ȩֵ�Ӵ�С����
    * ����ֵ��	��
    */
    void GetDangerNULLCM(IN JFChessman* pCBData,
                        IN const JFCMType& selfType, 
                        IN const JFCBRect& rect, 
                        OUT vector<JFChessman>& vDangerousCM);

    /**
     * �������ܣ���ȡ������һ��������һ�����͵�����
     * ������
     *          pCBData : ��������
     *          rect    : ������pCBData�ϵ�һ������
     *          type    : Ҫ��ȡ�����ӵ�����
     *          vCMInRect : ��ȡ�����Ӽ���
     * ����ֵ����	
     */
    void GetCMInRect(IN const JFChessman* pCBData, 
                     IN const JFCBRect& rect, 
                     IN const JFCMType& type, 
                     OUT vector<JFChessman>& vCMInRect);

    /**
    * �������ܣ���ȡ����������һ�����͵�Σ�յ�
    * ������
    *          pCBData : ��������
    *          rect    : ������pCBData�ϵ�һ������
    *          uiWeight: Ҫ��ȡ�Ŀ������ӽڵ��Ȩֵ��Сֵ
    *          vDanger : ��ȡ�����Ӽ���
    * ����ֵ����	
    */
    void RetriveDangerCMByWeight(IN JFChessman* pCBData, 
                               IN const JFCBRect& rect,
                               IN const jf_uint& uiWeight,
                               OUT vector<JFChessman>& vDanger);
	 
    /**
     * �������ܣ�����ԭʼ�������ݵ�pDest
     * ������
     *          pDest : Ŀ�ĵ�
     * ����ֵ����	
     */
	void CopySrcCBData(IN JFChessman* pDest);
	 
    /**
     * �������ܣ���������chessman������pCBData�ϵ�����rect�ڲ�����Ȩֵ
     *           ��ЩȨֵ���ӵ�rect�ڵı�Ӱ�쵽�Ŀ����̽ڵ��Ȩֵ��
     * ������
     *          pCBData  : ��������
     *          rect     : ����pCBData�ڵ�һ������
     *          chessman : ����pCBData�ϵ�һ������
     * ����ֵ��	��
     */
	inline void CalcTheWeight(IN JFChessman* pCBData, IN const JFCBRect& rect, IN const JFChessman& chessman);

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
    inline void DoCalcWeight(IN JFChessman* pCBData,
                             IN const JFCBRect& rect,
                             IN const JFChessman& chessman,
                             IN const JFDirection& direction,
                             IN const jf_int32& iRange);

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
    inline void JFEngine::DoCalcWeightBack(IN JFChessman* pCBData,
                                            IN const JFCBRect& rect,
                                            IN const JFChessman& chessman,
                                            IN const JFDirection& direction,
                                            IN const jf_int32& iRange);

    /**
     * �������ܣ���ȡһ������ķ�����
     * ������
     *          direction : һ������
     *          rDirection: ��ȡ��direction�ķ�����
     * ����ֵ��	��
     * ע�⣺�ú����ϸ���JFDirection��ö�ٵķ�����㣬��ˣ����JFDirectionö�ٸı�
     *       �÷����Ϳ��ܳ���
     */
    inline void GetRDirection(IN const JFDirection& direction, OUT JFDirection& rDirection);


private:
    // ָ������ָ��
    static JFEngine* m_pEngine;

    /** 
    * ���
    */
    JFCBSituation m_CBSituation;

    /**
    * ����������
    */
    JFEngine *m_pGobangEngine;

    /** 
     * ��ѷ�����
     */
    JFPosInCB m_bestDefencePos;

    /** 
    * ��ѽ�����
    */
    JFPosInCB m_bestOffensePos;

    /** 
    * ������ӵ�
    */
    JFPosInCB m_bestPlacePos;

    jf_long32 m_vXCalc[8];      // ����x���������
    jf_long32 m_vYCalc[8];      // ����y���������
};
#endif // !defined(EA_43F6C58C_8FB2_477f_8C4A_C252D1C4C293__INCLUDED_)
