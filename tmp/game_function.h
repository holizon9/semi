
#include <stdlib.h>
#include<stdio.h>
#include <string.h>
extern int board[12][12];  //ボード生成
extern int tmpboard[12][12]; //関数内で使用する一時的なボードの砂場
extern int turn;   	//ターンカウンタ　　奇数で先手番　偶数で後手番
extern int MAX;			//最大手数
extern int X,Y;			//ボード上の座標を示す
void boardclear();  //ボードをクリア
void tmp_boardclear();  //ボードをクリア
void printboard();  //標準出力にゲームボードを表示
void tmp_printboard();
//void readscore(FILE *fp);
void pieceputtoboard(int X,int turn);	//ボードに駒を配置
void tmp_pieceputtoboard(int X,int turn);
int inputerrorcheck(char *input,int X);
int turnstart(int turn);	//ターン開始時の処理をまとめたもの
int  hexconversion(char *input);
int victory_decision();  //turn player wins=1 or not=0
int game_end_message(int turn);  //ゲーム終了メッセージの表示
void board_copy();
void printhint();
int  can_win(int X);
int tmp_victory_decision();

void board_copy(){
	int i,j;
	for(i=0;i<12;i++){
		for(j=0;j<12;j++){
			tmpboard[i][j]=board[i][j];
		}
	}



}


int can_win(int X){		//ボードを参照しXに置いて勝てる場合1を、勝てない場合0を返す.
	board_copy();
	tmp_pieceputtoboard(X,turn);
	return tmp_victory_decision();





}
void printhint(){


	for(int i=0;i<12;i++){
		if(can_win(i)==1){
			printf("%dに置けば勝てます\n",i);

		}
	}



}


int victory_decision(){   //boardを読んで勝ちを判定する　勝ちなら1を返す　
		int i,j;
		for(i=0;i<12;i++){
			for(j=0;j<12;j++){
				if(board[i][j]!=0){
					if(abs(board[i][j]+board[i+1][j]+board[i+2][j]+board[i+3][j])==4){  //縦4つ勝利判定
						return 1;
					}
					if(abs(board[i][j]+board[i][j+1]+board[i][j+2]+board[i][j+3])==4)  //横四つ判定
					{
						return 1;
					}
					if(abs(board[i][j]+board[i+1][j+1]+board[i+2][j+2]+board[i+3][j+3])==4){ //右斜め判定
						return 1;
					}
					if(abs(board[i][j]+board[i-1][j-1]+board[i-2][j-2]+board[i-3][j-3])==4){ //左斜め判定
						return 1;
					}

				}

			}
		}
		return 0;
}
int tmp_victory_decision(){   //boardを読んで勝ちを判定する　勝ちなら1を返す　
		int i,j;
		for(i=0;i<12;i++){
			for(j=0;j<12;j++){
				if(tmpboard[i][j]!=0){
					if(abs(tmpboard[i][j]+tmpboard[i+1][j]+tmpboard[i+2][j]+tmpboard[i+3][j])==4){  //縦4つ勝利判定
						return 1;
					}
					if(abs(tmpboard[i][j]+tmpboard[i][j+1]+tmpboard[i][j+2]+tmpboard[i][j+3])==4)  //横四つ判定
					{
						return 1;
					}
					if(abs(tmpboard[i][j]+tmpboard[i+1][j+1]+tmpboard[i+2][j+2]+tmpboard[i+3][j+3])==4){ //右斜め判定
						return 1;
					}
					if(abs(tmpboard[i][j]+tmpboard[i-1][j-1]+tmpboard[i-2][j-2]+tmpboard[i-3][j-3])==4){ //左斜め判定
						return 1;
					}

				}

			}
		}
		return 0;
}

int game_end_message(int turn){
		if (victory_decision()==1){
			if (turn%2==1)
			{
				printf("X wins\n");
			}else{
				printf("O wins\n");
			}
			return 1;
		}
		return 0;

}
void pieceputtoboard(int X,int turn){


		for (int i = 11; i >=0; --i)
		{

			if (board[i][X]==0)
			{
				if (turn%2==0)
				{
					board[i][X]=-1;
				}else{
					board[i][X]=1;
				}
				break;
			}
		}
}
void tmp_pieceputtoboard(int X,int turn){


		for (int i = 11; i >=0; --i)
		{

			if (tmpboard[i][X]==0)
			{
				if (turn%2==0)
				{
					tmpboard[i][X]=-1;
				}else{
					tmpboard[i][X]=1;
				}
				break;
			}
		}
}


int inputerrorcheck(char *input,int X){  //入力が適正でない場合-1を、適正な場合1を返す
	//オプションとして、hintを入力された場合2を返す。
	//返り値でオプションを区別する

	//適正な入力とは、A,B,0~9であり、ボード上に配置できる入力とする。
	if(!strcmp(input, "hint")){		//printhintの呼び出し
		printhint();
		return 2;

	}else if(!strcmp(input,"A")||!strcmp(input,"B")||(strcmp(input,"/")>0&&strcmp(input,":")<0))
	{
		if((board[0][X]==0)&&(X<12)){
			return 1;
		}else{
			printf("invalid input\n");
			return -1;
		}
	}else{
		printf("invalid input\n");
		return -1;
	}

}


int  turnstart(int turn){
			/*ターン開始処理*/
	printf("///////////////////////第%dターン",turn);
	if(turn%2==0){
		printf("後手番\n" );
	}else{
		printf("先手番\n");
	}
	printboard();
	X=-1; //エラー検出のため
	return X;

		/*ターン開始処理おわり*/

}



int  hexconversion(char *input){
	int tmp_x=0;
	if(!strcmp(input,"A")){
		tmp_x=10;
	}else if(!strcmp(input,"B")){
		tmp_x=11;
	}else{
		tmp_x=atoi(input);
	}

	return tmp_x;
}
void printboard(){
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j< 12; ++j)
		{
			if(board[i][j]==0){
				printf(". ");
			}
			if(board[i][j]==1){
				printf("X ");
			}
			if(board[i][j]==-1){
				printf("O ");
			}
		}
		printf("\n");
	}
	printf("0 1 2 3 4 5 6 7 8 9 A B\n");
}
void tmp_printboard(){
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j< 12; ++j)
		{
			if(tmpboard[i][j]==0){
				printf(". ");
			}
			if(tmpboard[i][j]==1){
				printf("X ");
			}
			if(tmpboard[i][j]==-1){
				printf("O ");
			}
		}
		printf("\n");
	}
	printf("0 1 2 3 4 5 6 7 8 9 A B\n");
}
void boardclear(){
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			board[i][j]=0;
		}
	}
}
void tmp_boardclear(){
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			tmpboard[i][j]=0;
		}
	}
}
