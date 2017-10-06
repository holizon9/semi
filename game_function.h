
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

void pieceputtoboard(int X,int turn);	//ボードに駒を配置
void tmp_pieceputtoboard(int X,int turn);
int inputerrorcheck(char *input,int X);
int turnstart(int turn);	//ターン開始時の処理をまとめたもの
int  hexconversion(char *input);
int victory_decision();  //turn player wins=1 or not=0
int game_end_message(int turn);  //ゲーム終了メッセージの表示
int first_turn_player_decision();
void board_copy();
void printhint();
int  can_win(int X);
int will_lose(int X);
int iputprocess(int X,char *input);
int com_plays();					//0~11までのコンピューターの手を返す
int tmp_victory_decision();

void board_copy(){
	int i,j;
	for(i=0;i<12;i++){
		for(j=0;j<12;j++){
			tmpboard[i][j]=board[i][j];
		}
	}



}
int com_plays(){	
	int X;	
	X=rand()%12;
	for(int i=0;i<12;i++){
		if(can_win(i)==1){
			return X;
		}
	}
	for(int i=0;i<12;i++){
		if(will_lose(i)==1){
			return X;
		}
	}

	return X;
}



int first_turn_player_decision(){
	char input[128];

	printf("プレイヤーが先手を持ちますか？y/n\n");

	while(1){
		scanf("%s",input);
		if(!strcmp(input, "y")){
			return 0;
			break;
		}
		if(!strcmp(input, "n")){
			return 1;
			break;
		}
	}

	

}


int iputprocess(int X,char *input){		//入力処理を統括

	scanf("%s",input);		 			
	X=hexconversion(input);
		while(inputerrorcheck(input,X)!=1){  //無効な入力の場合再入力を求める
			if(turn%2==1){
				printf("X? ");
			}else{
				printf("O? ");		
			}
			scanf("%s",input);
			X=hexconversion(input);
		}
		return X;

	}



int can_win(int X){		//ボードを参照しXに置いて勝てる場合1を、勝てない場合0を返す.
	board_copy();
	tmp_pieceputtoboard(X,turn);
	return tmp_victory_decision();
}

int will_lose(int X){
	board_copy();
	tmp_pieceputtoboard(X, turn+1);

	return tmp_victory_decision();

}

void printhint(){
	int counter=0;

	for(int i=0;i<12;i++){
		if(can_win(i)==1){
			if(i==10){
				printf("Aに置けば勝てます\n");
			}
			if(i==11){
				printf("Bに置けば勝てます\n");
			}
			printf("%dに置けば勝てます\n",i);
			counter++;

		}
	}
	if(counter==0){
		printf("勝利手はありません\n");
	}
	counter=0;

	for(int i=0;i<12;i++){
		if(will_lose(i)==1){
			if(i==10){
				printf("Aに置かれると負けます\n");
			}
			if(i==11){
				printf("Bに置かれると負けます\n");
			}
			printf("%dに置かれると負けます\n",i);
			counter++;

		}
	}
	if(counter==0){
		printf("敗北手はありません\n");
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
					else if(abs(tmpboard[i][j]+tmpboard[i][j+1]+tmpboard[i][j+2]+tmpboard[i][j+3])==4)  //横四つ判定
					{
						return 1;
					}
					else if(abs(tmpboard[i][j]+tmpboard[i+1][j+1]+tmpboard[i+2][j+2]+tmpboard[i+3][j+3])==4){ //右斜め判定
						return 1;
					}
					else if(abs(tmpboard[i][j]+tmpboard[i-1][j+1]+tmpboard[i-2][j+2]+tmpboard[i-3][j+3])==4){ //左斜め判定
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
	void tmp_pieceputtoboard(int X,int turns){


		for (int i = 11; i >=0; --i)
		{

			if (tmpboard[i][X]==0)
			{
				if (turns%2==0)
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
