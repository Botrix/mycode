/**********************************************************************************
 *  JFFile.h
 *  Implementation of the Class JFFile
 *  Created on:      26-四月-2010 15:26:52
 *  Original author: Jiang Feng
 *********************************************************************************/

#if !defined(EA_20F8F1DE_9412_4a5b_9434_F2F175F59686__INCLUDED_)
#define EA_20F8F1DE_9412_4a5b_9434_F2F175F59686__INCLUDED_

#include "JFType.h"

/**
 * 类JFFile处理关于文件读写的事务。
 */
class JFFile
{

public:
	JFFile();
	virtual ~JFFile();

	/**
	 * 保存五子棋数据
	 */
	jf_bool SaveFile();
	/**
	 * 保存五子棋replay
	 */
	jf_bool SaveReplay();
	/**
	 * 加载五子棋数据
	 */
	jf_bool LoadFile();
	/**
	 * 加载五子棋replay
	 */
	jf_bool LoadReplay();

};
#endif // !defined(EA_20F8F1DE_9412_4a5b_9434_F2F175F59686__INCLUDED_)
