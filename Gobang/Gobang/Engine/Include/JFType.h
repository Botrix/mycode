/** 
 * �� �� ��: JFType.h
 * ժ    Ҫ: ���ļ������˻����������ض���
 *
 * ��    ��: ����
 * ����ʱ��: 2010.03.21
 */

#ifndef JFTYPE_H_ 
#define JFTYPE_H_

#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

typedef bool						jf_bool;
typedef unsigned int				jf_uint;
typedef unsigned short              jf_ushort;
typedef char						jf_char;
typedef int							jf_int32;
typedef long                        jf_long32;
typedef float						jf_float;
typedef double						jf_double;
typedef WORD						jf_word;
typedef DWORD						jf_dword;
typedef byte						jf_byte;
typedef string						jf_string;
typedef LONGLONG					jf_int64;

#define IN
#define OUT
#define INOUT

#define JFCALLBACK          // �ص�����
#define CM_RADIUS 10        // ���Ӱ뾶
#define CB_GRID_WIDTH 32    // ���̸��ӿ��
#define CB_GRID_NUM 14      // ������ÿ�и�����
#define HMI_FONT_HEIGHT 20  // �˻����������ϵ�����ĸ߶�

#define WEIGHT_DANGER  6    // Ȩֵ��6���ϣ�����6����ΪΣ�յ�

#define WEIGHT_WIN              15    // ʤ��Ȩֵ

#define PLAYER_INFO_BK_WIDTH   80     // ��Ϣ���������
#define PLAYER_INFO_BK_HEIGHT  30     // ��Ϣ��������

#endif