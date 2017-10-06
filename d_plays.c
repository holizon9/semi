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
	/*ゲーム開始処理*/{
	//この処理は開発時に不要なためコメントアウト
	if (0){
		printf("最大ターンを入力してください\n");
		scanf("%d",&MAX);
	}
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


   
	printf("ゲーム開始処理おわり\n");		/*ゲーム開始処理おわり*/
}

	while(turn<MAX+1){  //ゲーム
		X=turnstart(turn);
		if(turn%2==1){
			printf("X? ");
		}else{
			printf("O? ");		
		}

/////////////////////////////////////////////////入力処理開始
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

