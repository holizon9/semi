#include<stdio.h>
#include "game_function.h"


extern int board[12][12];  //ボード生成
extern int tmpboard[12][12]; //関数内で使用する一時的なボードの砂場
extern int turn;   	//ターンカウンタ　　奇数で先手番　偶数で後手番
extern int MAX;			//最大手数
extern int X,Y;			//ボード上の座標を示す


int test(){
	boardclear();
	return 0;
}