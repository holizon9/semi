#include <stdlib.h>
#include<stdio.h>
#include <string.h>

int board[12][12];  //ボード生成
int turn=1;   	//ターンカウンタ　　奇数で先手番　偶数で後手番
int MAX=128;			//最大手数
int X,Y;			//ボード上の座標を示す
void boardclear();  //ボードをクリア
void printboard();  //標準出力にゲームボードを表示
//void readscore(FILE *fp);
void pieceputtoboard(int X,int turn);	//ボードに駒を配置
int inputerrorcheck(char *input,int X);
int turnstart(int turn);	//ターン開始時の処理をまとめたもの
int  hexconversion(char *input);
int victory_decision();  //turn player wins=1 or not=0
int game_end_message(int turn);  //ゲーム終了メッセージの表示

int main(int argc,char *argv[]){
	FILE *fp;
	char input[128];
	int fo=1;
	/*ゲーム開始処理*/{
	//この処理は開発時に不要なためコメントアウト
	if (0){
		printf("最大ターンを入力してください\n");
		scanf("%d",&MAX);
	}
	boardclear();
	if ((fp = fopen(argv[1], "r+")) == NULL) {		//ファイルオープン
			if ((fp = fopen(argv[1], "a+")) == NULL) {		//ファイルオープン
		printf("file open error!!\n");
		fo=-1;
	}

	}
	if(argc==2){
		printf("棋譜ファイル%sをインポートしました\n",argv[1]);
		while(fscanf(fp, "%d%d",&turn,&X)!=EOF){
			pieceputtoboard(X, turn);
		}
	}

		fprintf(fp, "\n");



	/*ゲーム開始処理おわり*/
}

	while(turn<MAX+1){  //ゲーム
		X=turnstart(turn);
		if(turn%2==1){
			printf("X? ");
		}else{
			printf("O? ");		
		}


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

		pieceputtoboard(X,turn);
		if(fo==1){
			fprintf(fp, "%d  %d\n",turn,X);

		}
		if(game_end_message(turn)==1){
			break;
		}





		turn++;

	}
	printboard();
	if(fo==1){
		fclose(fp);
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


int inputerrorcheck(char *input,int X){  //入力が適正でない場合-1を、適正な場合1を返す

	//適正な入力とは、A,B,0~9であり、ボード上に配置できる入力とする。

	if(!strcmp(input,"A")||!strcmp(input,"B")||(strcmp(input,"/")>0&&strcmp(input,"9")<0))
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
void boardclear(){
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			board[i][j]=0;
		}
	}
}
