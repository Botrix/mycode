#ifndef SAVE_PRINCESS_H
#define SAVE_PRINCESS_H

/*
 * 公主被魔王抓走了，王子需要拯救出美丽的公主。他进入了魔王的城堡，魔王的城堡是一座很大的迷宫。
 * 为了使问题简单化，我们假设这个迷宫是一个N*M的二维方格。迷宫里有一些墙，王子不能通过。王子只
 * 能移动到相邻（上下左右四个方向)的方格内，并且一秒只能移动一步，就是说，如果王子在(x,y）一步
 * 只能移动到(x-1,y),(x+1,y),(x,y-1),(x,y+1)其中的一个位置上。地图由‘S’，‘P’，‘.’，‘*’
 * 四种符号构成，‘.’表示王子可以通过，‘*’表示墙，王子不能通过；'S'表示王子的位置；‘P’表示公主
 * 的位置；T表示公主存活的剩余时间，王子必须在T秒内到达公主的位置，才能救活公主。如下图所示：
 */

/* M行N列迷宫 
 * 如果能够救则返回1， 否则返回0
 */
extern int save_princess(int M, int N, char* maze_data, int time);

#endif