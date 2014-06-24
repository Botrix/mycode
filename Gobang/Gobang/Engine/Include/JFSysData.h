/**********************************************************************************
 *  JFSysData.h
 *  Implementation of the Class JFSysData
 *  Created on:      26-����-2010 15:26:53
 *  Original author: Jiang Feng
 *********************************************************************************/

#if !defined(EA_9217E43C_3E68_46c6_BCC7_01B48EA3DA67__INCLUDED_)
#define EA_9217E43C_3E68_46c6_BCC7_01B48EA3DA67__INCLUDED_

#include "JFStruct.h"
#include "JFFile.h"


/**
 * JFSysData�����ڹ���ϵͳ��������Ϣ�����統ǰ��֡��˻����������Ӳ��ֵ���Ϣ��
 * JFFileд���ļ��е����ݾ�����������ļ��ж�ȡ������Ҳ����������൱
 * ��һ����פ�ڴ�����ݿ����ϵͳ��
 */
class JFSysData
{

public:

	virtual ~JFSysData();

	/**
	 * ��ʼ���������ݣ��ú�����ϵͳ��ʼ���Ϳ�ʼ�µ�һ��ʱ����
	 */
	void Initialize();


    /**
    * ���ӣ�������������ӵ�ǰ���ӵ���Ϣ
    */
    jf_bool PlaceChessman(const JFChessman& chessman);

    /**
    * �������ܣ���ʼ��Ϸ�������������
    * ��������
    * ����ֵ����	
    */
    void StartGame(void);

    /**
    * �������ܣ�������Ϸ
    * ��������
    * ����ֵ����	
    */
    void EndGame(void);

    /*******************************************************************
     * Get methods
     *******************************************************************/

    /**
    * ��ȡָ������ָ��
    */
    static JFSysData* GetSysData(void);

    /**
    * ��õ�ǰҪ�µ����ӵ�����
    */
    const JFCMType& GetCurCMTypeToPlace() const;

    /**
    * ��ȡ������ִ�����ӵ�����
    */
    const JFCMType& GetCMTypeOfComputer() const;

    /**
    * ��ȡԭʼ��������
    */
    const JFChessman* GetCBData();

    /**
     * �������ܣ���ȡtype���͵�����������
     * ������
     *          type : ��������
     * ����ֵ��
     *          �����ݵĳ�����
     */
    const vector<jf_ushort>& GetLineData(IN const JFCMType& type);

    /**
    * ��ȡ��ǰ��Ϸ�Ѷȼ���
    */
    const JFGameLevel& GetGameLevel() const;

    /**
     * �������ܣ���ȡ��ǰ��Ϸģʽ
     * ��������
     * ����ֵ����ǰ��Ϸģʽ	
     */
    const JFGameMode& GetGameMode(void) const;

    /**
    * �������ܣ���ȡ��ǰ��Ϸ״̬
    * ��������
    * ����ֵ����ǰ��Ϸ״̬	
    */
    const JFGameState& GetGameState(void) const;

    /*******************************************************************
     * Set methods
     *******************************************************************/

	/**
	 * ������һ��Ҫ�µ����ӵ�����
	 */
	void SetCurCMTypeToPlace(const JFCMType type);
	
	
    /**
    * ���õ�ǰ��Ϸ�Ѷȼ���
    */
    void SetGameLevel(const JFGameLevel& level);

    /**
    * �������ܣ����õ�ǰ��Ϸģʽ
    * ������
    *           mode : ��Ϸģʽ
    * ����ֵ��
    *           true : ���óɹ�
    *           false: ���ò��ɹ�����ǰ��Ϸ�������ڽ���
    */
    jf_bool SetGameMode(const JFGameMode& mode);

    /*******************************************
     * �����жϵķ���
     *******************************************/
    /**
     * �������ܣ��ж����̽ڵ�pos�ܹ�����
     * ������
     *          pos : ���̽ڵ�
     * ����ֵ��	
     *         true : pos��������
     *         false: pos����������
     */
    jf_bool IsCurPosAvailable(IN const JFPosInCB& pos);

private:
    JFSysData();

public:
    /**
    * ʵ�ֵ���ģʽ
    */
    static JFSysData *m_pSysData;

private:
    /*******************************************************************
     * ��������
     *******************************************************************/

	JFChessman m_vChessBoard[15][15];   // ����ԭʼ����
	
	vector<JFChessman> m_vBlackCM;      // �����ϵĺ���

	vector<JFChessman> m_vWhiteCM;      // �����ϵİ���

    /*******************************************************************
     * �ڰ��ӵ��������ܹ��У�15+15+29*2 = 88
     * ��P(x,y)�������߶������£�
     * 0-14  �У�[y] 
     * 15-29 �У�[x]
     * 30-58 /�ߣ�[x+y]
     * 59-87 \�ߣ�[x+y+(x>y)*14]
     *******************************************************************/
    vector<jf_ushort>  m_vBlackLineData;   // ���̵ĺ��������� : �С��С�б�С�б��
    vector<jf_ushort>  m_vWhiteLineData;   // ���̵İ���  ������ : �С��С�б�С�б��

	JFCMType m_curCMTypeToPlace;        // ��ǰӦ�����ӵ���������

    JFGameLevel m_curGameLevel;         // ��ǰ��Ϸ�Ѷȼ���

    JFGameMode m_curGameMode;           // ��ǰ��Ϸģʽ

    JFGameState m_gameState;            // ��ǰ��Ϸ״̬

    JFFile *m_JFFile;                   // �����ļ�

};
#endif // !defined(EA_9217E43C_3E68_46c6_BCC7_01B48EA3DA67__INCLUDED_)
