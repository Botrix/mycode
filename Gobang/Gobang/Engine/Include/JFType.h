/** 
 * 文 件 名: JFType.h
 * 摘    要: 本文件包含了基本的类型重定义
 *
 * 作    者: 姜峰
 * 创建时间: 2010.03.21
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

#define JFCALLBACK          // 回调函数
#define CM_RADIUS 10        // 棋子半径
#define CB_GRID_WIDTH 32    // 棋盘格子宽度
#define CB_GRID_NUM 14      // 棋盘上每行格子数
#define HMI_FONT_HEIGHT 20  // 人机交互界面上的字体的高度

#define WEIGHT_DANGER  6    // 权值在6以上（包括6）即为危险点

#define WEIGHT_WIN              15    // 胜利权值

#define PLAYER_INFO_BK_WIDTH   80     // 信息区背景宽度
#define PLAYER_INFO_BK_HEIGHT  30     // 信息区背景高

#endif