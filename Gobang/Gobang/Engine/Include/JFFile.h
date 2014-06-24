/**********************************************************************************
 *  JFFile.h
 *  Implementation of the Class JFFile
 *  Created on:      26-����-2010 15:26:52
 *  Original author: Jiang Feng
 *********************************************************************************/

#if !defined(EA_20F8F1DE_9412_4a5b_9434_F2F175F59686__INCLUDED_)
#define EA_20F8F1DE_9412_4a5b_9434_F2F175F59686__INCLUDED_

#include "JFType.h"

/**
 * ��JFFile��������ļ���д������
 */
class JFFile
{

public:
	JFFile();
	virtual ~JFFile();

	/**
	 * ��������������
	 */
	jf_bool SaveFile();
	/**
	 * ����������replay
	 */
	jf_bool SaveReplay();
	/**
	 * ��������������
	 */
	jf_bool LoadFile();
	/**
	 * ����������replay
	 */
	jf_bool LoadReplay();

};
#endif // !defined(EA_20F8F1DE_9412_4a5b_9434_F2F175F59686__INCLUDED_)
