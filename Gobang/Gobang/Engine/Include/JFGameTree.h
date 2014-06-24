/** 
 * �� �� ��: JFGameTree.h
 * ժ    Ҫ: ���ļ������˲�����������
 *
 * ��    ��: ����
 * ����ʱ��: 2010.05.16
 */

/*******************************************
 * ������
 *
 * ϵͳ��ʼ����ʱ�͹��첩�������Ӹ��ڵ㿪ʼ
 * ���µ�ÿ�����Ӷ�Ҫ��¼�ڰ��� :) ��Ϊ��
 * �Ժ���������ݼ��������ݶ�Ҫ��������Ѿ�
 * �µ����Ӻ�ԭʼ�������ݼ��������
 *
 *******************************************/
#ifndef JFGAMETREE_H_
#define JFGAMETREE_H_

#include "JFStruct.h"
#include "JFPublicFunction.h"
#define WEIGHT_DETECT  4            // Ȩֵ��4���ϣ�����4����Ԥ�⽨������

struct JFTreeNode;  // ǰ������

// ���ڲ�������Σ�յ㶨��
struct JFDangerPos 
{
public:
    JFPosInCB       pos;            // Σ�յ�
    jf_uint         uiWeight;       // �������˸�Σ�յ��������ֵ�Ȩֵ
    JFTreeNode*     pNext;          // ָ���ڸ�Σ�յ����Ӻ���������
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
    {// �˴��Ŀ����������
        memcpy(this, &dangerPos, sizeof(JFDangerPos));
    }

    JFDangerPos& operator=(const JFDangerPos& dangerPos)
    {// �˴��Ŀ����������
        memcpy(this, &dangerPos, sizeof(JFDangerPos));

        return *this;
    }
};

// �������ڵ㶨��
struct JFTreeNode
{
    JFDangerPos* pParent;   // ָ�򸸽ڵ�(Σ�յ�)��ָ��
    vector<JFDangerPos> vDangerPos;
};

// ����������
// �ò��������Զ�̬���Ԥ������������ݵ�ǰ���µ�����ɾ�����õ�֦���ﵽ
// ��ʡ�ڴ桢���Ч�ʹ��ܡ�
class JFGameTree
{
public:
    // ���캯��
    JFGameTree();
    ~JFGameTree();

public:
    /**
     * �������ܣ����첩��������ȡ������ӵ�
     * ������
     *          type : �ò�������Ϊ����Ϊtype�����ӽ����ģ���Ŀ����
     *                 ͨ���ò�������Ϊtype���͵����ӣ��ڵ�ǰ�������
     *                 �ҳ�һ�����ŵ����ӵ�
     *          pCBData : ��ǰ���������ݣ���ʱӦ�����ֵ�type���͵���������
     *          uiStep  : Ԥ����Ԥ�ⲽ����Ԥ�ⲽ��Խ����������ӵ�Խ�ӽ��������ӵ�
     *                    ���ǣ��ٶȿ��ܻ�Ƚ�����
     *                    uiStep����Ϊ0��Ϊ0ʱ�����ǹ���һ����������ֻ�и��ڵ㡣
     * ����ֵ��	
     *          ������ӵ�
     */
    const JFPosInCB& GetBestPosToPlace( IN const JFCMType& type,
                                        IN JFChessman* pCBData,
                                        IN const jf_uint& uiStep);

   
private:
    /**
     * �������ܣ���ȫ�ͷ�����
     * ������
     *          ָ��ڵ��ָ��
     * ����ֵ��	��
     */
    void SafeDeleteNode(JFTreeNode* pNode);

    /**
     * �������ܣ����ݸ��ڵ㴴������������
     * ������
     *          pRoot  : ָ����ڵ��ָ��
     *          typeToPlace : ��ǰҪ���ӵ����ӵ�����
     *          uiStep : ҪԤ��Ĳ������������Ĳ���
     *          vCMPlaced : ����ڵ㵽��ǰ���ʱ�Ѿ��µ����ӵļ���
     * ����ֵ��	��
     */
    void ConstructDefenceTree(IN JFTreeNode* pRoot, 
                            IN const JFCMType& typeToPlace, 
                            IN const jf_uint& uiLayer, 
                            IN vector<JFChessman> vCMPlaced);

    /**
    * �������ܣ����ݸ��ڵ㴴������������
    * ������
    *          pRoot  : ָ����ڵ��ָ��
    *          typeToPlace : ��ǰҪ���ӵ����ӵ�����
    *          uiStep : ҪԤ��Ĳ���
    *          vCMPlaced : ����ڵ㵽��ǰ���ʱ�Ѿ��µ����ӵļ���
    * ����ֵ��	��
    */
    void ConstructOffenseTree(IN JFTreeNode* pRoot, 
                                IN const JFCMType& typeToPlace, 
                                IN const jf_uint& uiLayer, 
                                IN vector<JFChessman> vCMPlaced);

    /**
     * �������ܣ������Ѿ������Ĳ�������ȡ��ѷ������ӵ�
     * ������
     *          pRoot : ָ�������ĸ��ڵ��ָ��
     *          uiCurLayer : ��ǰ�ڵ��ڲ������еĲ���
     * ����ֵ�� ��
     */
    void GetBestDefencePos(IN JFTreeNode* pRoot, IN const jf_uint uiCurLayer);

    /**
    * �������ܣ������Ѿ������Ĳ�������ȡ��ѽ������ӵ�
    * ������
    *          pRoot : ָ�������ĸ��ڵ��ָ��
    *          uiCurLayer : ��ǰ�ڵ��ڲ������еĲ���
    * ����ֵ�� ��
    */
    void GetBestOffensePos(IN JFTreeNode* pRoot, IN const jf_uint uiCurLayer);

    /**
     * �������ܣ���ȡΣ�յ㼯���е�Σ�յ��Ȩֵ�ĺ�
     * ������
     *         vDangerCM : Σ�յ㼯��
     * ����ֵ��Ȩֵ֮��
     */
    jf_uint GetMaxWeight(const vector<JFChessman>& vDangerCM);

private:
    JFTreeNode*         m_pRoot;                    // ָ�������ĸ��ڵ㣬Ϊ0��
private:
    jf_uint             m_uiLayer;                  // ����Ĳ������Ĳ���
    JFCMType            m_type;                     // ������������
    JFCMType            m_enemyType;                // �˵���������
    JFChessman          m_srcCBData[15][15];        // ��������, ������Ȩֵ��Ϣ
    vector<jf_ushort>   m_srcSelfLineData;          //  m_type���͵����ӵ�������
    vector<jf_ushort>   m_srcEnemyLineData;         //  m_type���͵ĵ��˵����ӵ�������

    vector<JFChessman> m_vCMPlace;                  // �Ӹ��ڵ㵽���ڵ���������

    JFPosInCB           m_bestPosToPlace;           // ��ǰ������ӵ�
};

#endif