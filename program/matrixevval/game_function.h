/////////////////  AI思考ロジックの選択
//#define MODE 1		//正規分布を使ったランダムアルゴ
//#define MODE 2	//評価AI
//#define MODE 3	//上下左右3マスをみるあるご
#define MODE 4		//minimax
/////////////////

/////////////////	αβ探索深度の設定
#define DEPTH 1
/////////////////





#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include <string.h>
#include <time.h>

extern int board[12][12];  //ボード生成
extern int tmpboard[12][12]; //関数内で使用する一時的なボードの砂場
extern int turn;   	//ターンカウンタ　　奇数で先手番　偶数で後手番
extern int MAX;			//最大手数
extern int X,Y;			//ボード上の座標を示す
int mmboard[12][12];

void tmp_boardclear();  //ボードをクリア
void tmp_pieceputtoboard(int X,int turn);
int tmp_victory_decision();		//boardを読んで勝ちを判定する　勝ちなら1を返す　
void tmp_printboard();
void tmp_undo(int X);

void printboard();  //標準出力にゲームボードを表示
void printboard_color(int X);
void pieceputtoboard(int X,int turn);	//ボードに駒を配置
int inputerrorcheck(char *input,int X);
int turnstart(int turn);	//ターン開始時の処理をまとめたもの
int  hexconversion(char *input);
void boardclear();  //ボードをクリア
int victory_decision();  //turn player wins=1 or not=0
int game_end_message(int turn);  //ゲーム終了メッセージの表示
int first_turn_player_decision();
void board_copy();
void printhint();
int  can_win(int X);
int will_lose(int X);
int iputprocess(int X,char *input);
int com_plays();					//0~11までのコンピューターの手を返す
int board_eval(int X);			//盤面評価関数
int MatrixEvaluation();			// 行列評価関数
int n_distribution();

int MM_max(int depth);
int MM_min(int depth);
int mm_victory_decision();
int mm_eval(int depth);
void mm_printboard();
void mm_undo(int X);
int mm_pieceputtoboard(int X,int turns);
void mm_board_copy();

int inputerrorcheck(char *input,int X){  //入力が適正でない場合-1を、適正な場合1を返す
	//オプションとして、hintを入力された場合2を返す。
	//返り値でオプションを区別する

	//適正な入力とは、A,B,0~9であり、ボード上に配置できる入力とする。
	if(!strcmp(input, "hint")){		//printhintの呼び出し
		printhint();
		return 2;

	}else if(!strcmp(input, "debug")){
		printf("debug mode\n");
		printf("next_play suggests %d\n",com_plays());




	}else if(!strcmp(input, "undo")){
		/*

			あとで待ったを実装する

			*/

	}else if(!strcmp(input,"A")||!strcmp(input,"B")||(strcmp(input,"/")>0&&strcmp(input,":")<0)){
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
	return -1;

}








int n_distribution(){			//正規分布	
	for(int i=0;i<12;i++){
		if(rand()<(RAND_MAX/2)){		
			X++;
		}
	}

	return X;


}








int MatrixEvaluation(){
	return -1;
}

int board_eval(int X){		//tmpboardに石を追加しその盤面の評価を返す	
	int E=0;


///////////////////////////有利な評価
	E=E+will_lose(X)*50000;



//////////////////////////不利な評価
	return E;
}




int com_plays(){
	int m;
	if(MODE==1){							//1.正規分布		


		for(int i=0;i<12;i++){
			if(can_win(i)==1){
				return i;
			}
		}
		for(int i=0;i<12;i++){
			if(will_lose(i)==1){
				return i;
			}
		}
		return n_distribution();


	}else if(MODE==2){						//2.評価AI
		int Eval[12];
		int Eval_MAX;
		Eval_MAX=-999999;

		int temp;
		for(int i=0;i<12;i++){
			Eval[i]=0;
			temp=0;
			for(int j=0;j<12;j++){
				if(rand()<(RAND_MAX/2)){		
					temp++;
				}
				Eval[temp]++;
			}
			Eval[i]+=board_eval(i);
			printf("%dへの評価は%dです\n",i,Eval[i]);
			if(Eval_MAX<Eval[i]){
				Eval_MAX=Eval[i];
				m=i;
			}
		}
		return m;




	}else if(MODE==3){
		return MatrixEvaluation();

	}else if(MODE==4){		//alpha-beta	
		/*
		まずは3手詰めを見つけるプログラムを目標にする

		*/
		int depth=1,max=-9999,tmp=-9999;
		int choice;
		mm_board_copy();
		for(int i=0;i<12;i++){
			if(mm_pieceputtoboard(i, turn)==0){

				if(depth==DEPTH){
					tmp=mm_eval(depth);
				}else{
					tmp=MM_min(depth+1);
				}
				if(max<tmp){max=tmp;
					choice=i;
				}

				 mm_printboard();
				 printf("about prime,vicdec is %d,max is %d ,tmp is%d\n",mm_victory_decision(),max,tmp);
				 printf("evaluation value is %d\n\n",tmp);





				mm_undo(i);
			}
		}
		return choice;
	}

}
///////////////////////////////////////////minimax関数群
int MM_max(int depth){		//X=0~12まで置いて、depth=DEPTHなら盤面評価、depth<DEPTHならさらに探索
	int tmp=0,max=-9999;
	for(int i=0;i<12;i++){
		if(mm_pieceputtoboard(i, turn+depth-1)==0){

			if(depth==DEPTH){
				tmp=mm_eval(depth);
			}else{
				tmp=MM_min(depth);
			}
			if(max<tmp){max=tmp;
			}
			mm_printboard();
			printf("about mmmax,vicdec is %d,max is %d ,tmp is%d\n",mm_victory_decision(),max,tmp);
			printf("evaluation value is %d\n\n",tmp);
			mm_undo(i);
		}
	}
	return max;
}
int MM_min(int depth){		//X=0~12まで置いて、depth=DEPTHなら盤面評価、depth<DEPTHならさらに探索
	int tmp=0,min=9999;
	for(int i=0;i<12;i++){
		if(mm_pieceputtoboard(i, turn+depth-1)==0){

			if(depth==DEPTH){
				tmp=mm_eval(depth);
			}else{
				tmp=MM_max(depth+1);
			}
			if(min>tmp){
				min=tmp;
			}
			mm_printboard();
			printf("about mmmin,vicdec is %d,min is %d ,tmp is%d\n",mm_victory_decision(),min,tmp);
			printf("evaluation value is %d\n\n",tmp);
			mm_undo(i);
		}
	}
	return min;
}


void mm_printboard(){
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j< 12; ++j)
		{
			if(mmboard[i][j]==0){
				printf(". ");
			}
			if(mmboard[i][j]==1){
				printf("X ");
			}
			if(mmboard[i][j]==-1){
				printf("O ");
			}
		}
		printf("\n");
	}
	printf("0 1 2 3 4 5 6 7 8 9 A B\n");
}


void mm_undo(int X){
	for(int i=0;i<12;i++){
		if(mmboard[i][X]!=0){

			mmboard[i][X]=0;
			break;
		}
	}
}
int mm_pieceputtoboard(int X,int turns){
	if(mmboard[0][X]!=0){
		return -1;
	}

	for (int i = 11; i >=0; --i)
	{

		if (mmboard[i][X]==0)
		{
			if (turns%2==0)
			{
				mmboard[i][X]=-1;
			}else{
				mmboard[i][X]=1;
			}
			break;
		}
	}
	return 0;
}

void mm_board_copy(){
	int i,j;
	for(i=0;i<12;i++){
		for(j=0;j<12;j++){
			mmboard[i][j]=board[i][j];
		}
	}

}

int mm_eval(int depth){
	int eval=0;
	eval = mm_victory_decision()*100;
	if(turn%2==0){eval=-eval;
	}
	return eval;
}
int mm_victory_decision(){   //boardを読んで勝ちを判定する  先手勝ちなら1 後手かちなら-1を返す
	int i,j;
	for(i=0;i<12;i++){
		for(j=0;j<12;j++){
			if(mmboard[i][j]!=0){
					if(abs(mmboard[i][j]+mmboard[i+1][j]+mmboard[i+2][j]+mmboard[i+3][j])==4){  //縦4つ勝利判定
						return mmboard[i][j];
					}
					else if(abs(mmboard[i][j]+mmboard[i][j+1]+mmboard[i][j+2]+mmboard[i][j+3])==4)  //横四つ判定
					{
						return mmboard[i][j];

					}
					else if(abs(mmboard[i][j]+mmboard[i+1][j+1]+mmboard[i+2][j+2]+mmboard[i+3][j+3])==4){ //右斜め判定
						return mmboard[i][j];

					}
					else if(abs(mmboard[i][j]+mmboard[i-1][j+1]+mmboard[i-2][j+2]+mmboard[i-3][j+3])==4){ //左斜め判定
						return mmboard[i][j];

					}

				}

			}
		}
		return 0;
		
	}


///////////////////////////////////////////

	void board_copy(){
		int i,j;
		for(i=0;i<12;i++){
			for(j=0;j<12;j++){
				tmpboard[i][j]=board[i][j];
			}
		}



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





int can_win(int X){		//ボードを参照しXに置いて勝てない場合0を,勝てる場合勝ち手の数を返す.
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
					if(abs(board[i][j]+board[i+1][j-1]+board[i+2][j-2]+board[i+3][j-3])==4){ //左斜め判定

						return 1;
					}

				}

			}
		}
		return 0;
	}
int tmp_victory_decision(){   //boardを読んで勝ちを判定する　勝ちなら1を返す　
	int i,j,k=0;
	for(i=0;i<12;i++){
		for(j=0;j<12;j++){
			if(tmpboard[i][j]!=0){
					if(abs(tmpboard[i][j]+tmpboard[i+1][j]+tmpboard[i+2][j]+tmpboard[i+3][j])==4){  //縦4つ勝利判定
						k++;
					}
					else if(abs(tmpboard[i][j]+tmpboard[i][j+1]+tmpboard[i][j+2]+tmpboard[i][j+3])==4)  //横四つ判定
					{
						k++;
					}
					else if(abs(tmpboard[i][j]+tmpboard[i+1][j+1]+tmpboard[i+2][j+2]+tmpboard[i+3][j+3])==4){ //右斜め判定
						k++;
					}
					else if(abs(tmpboard[i][j]+tmpboard[i-1][j+1]+tmpboard[i-2][j+2]+tmpboard[i-3][j+3])==4){ //左斜め判定
						k++;
					}

				}

			}
		}
		return k;
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





	int  turnstart(int turn){
			/*ターン開始処理*/
		printf("///////////////////////第%dターン",turn);
		if(turn%2==0){
			printf("後手番\n" );
		}else{
			printf("先手番\n");
		}
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
void printboard_color(int X){
	int y;
	for(int i=0	;i<12;i++){
		if(board[i][X]!=0){
			y=i;

			break;
		}
	}
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j< 12; ++j)
		{
			if(board[i][j]==0){
				printf(". ");
			}
			if(board[i][j]==1){
				if(i==y&&j==X){
					printf("\033[45m");
					printf("X ");
					printf("\033[0m");

				}else{

				printf("X ");
				}

			}
			if(board[i][j]==-1){
				if(i==y&&j==X){
					printf("\033[45m");
					printf("O ");
					printf("\033[0m");

				}else{

				printf("O ");
				}
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

void tmp_undo(int X){
	for(int i=0;i<12;i++){
		if(tmpboard[i][X]!=0){

			tmpboard[i][X]=0;
			break;
		}
	}
}
void undo(int X){
	for(int i=0;i<12;i++){			
		if(board[i][X]!=0){

			board[i][X]=0;
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



/////////////////////////////AI用関数群


















