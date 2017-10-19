#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include "game_function.h"
int board[12][12];  //ボード生成
int tmpboard[12][12]; //関数内で使用する一時的なボードの砂場
int turn=1;   	//ターンカウンタ　　奇数で先手番　偶数で後手番
int MAX=128;			//最大手数
int X,Y;			//ボード上の座標を示す


int main(int argc,char *argv[]){
	FILE *fp;
	char input[128];
	int fo=1;
	int com_plays_first=0;	//com先手なら1,違うなら0
	/*ゲーム開始処理*/{
	//この処理は開発時に不要なためコメントアウト
	if (0){
		printf("最大ターンを入力してください\n");
		scanf("%d",&MAX);
	}
	//この処理は開発時に不要なためコメントアウト

	//この処理は先手後手を決めるための処理
	com_plays_first=first_turn_player_decision();
	//この処理は先手後手を決めるための処理

	boardclear();
	tmp_boardclear();
	if ((fp = fopen(argv[1], "r+")) == NULL) {		//ファイルオープン
		printf("file open error!!\n");
		fo=-1; 
	}
	if(argc==2&&fo==1){
		printf("棋譜ファイル%sをインポートしました\n",argv[1]);
		while(fscanf(fp, "%d%d",&turn,&X)!=EOF){
			pieceputtoboard(X, turn);
		}
	}   
	printf("ゲーム開始処理おわり\n");	

	for(int f=1;f<100;f++){
	printf("%d",com_plays());
	}
}	/*ゲーム開始処理おわり*/

	while(turn<MAX+1){  //ゲーム
		X=turnstart(turn);
			if(turn%2==1){
				printf("X? ");
			}else{
				printf("O? ");	

			}

/////////////////////////////////////////////////com思考処理

			if(turn%2==com_plays_first){

				X=com_plays();



				printf("computer plays%d\n",X);




		} else{

/////////////////////////////////////////////////com思考処理終わり


/////////////////////////////////////////////////入力処理開始

		X=iputprocess(X, input);
	}
/////////////////////////////////////////////////入力処理おわり

		pieceputtoboard(X,turn);
		if(fo==1){
			fprintf(fp, "%d  %d\n",turn,X);		//ターン中の棋譜入力

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

