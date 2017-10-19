//#define GAMEMODE 1  //pvp
#define GAMEMODE 2 //pvc
//#define GAMEMODE 3 //cvc

#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include <time.h>
#include "game_function.h"
int board[12][12];  //ボード生成
int tmpboard[12][12]; //関数内で使用する一時的なボードの砂場
int turn=1;   	//ターンカウンタ　　奇数で先手番　偶数で後手番
int MAX=128;			//最大手数
int X,Y;			//ボード上の座標を示す



int main(int argc,char *argv[]){

	FILE *fp; 
	char input[128];
	char filename[128];
	time_t timer;
	struct tm *t_st;

	int fo=0;
	int com_plays_first=0;	//com先手なら1,違うなら0
    srand((unsigned)time(NULL));

	/*ゲーム開始処理*/{
	//この処理は開発時に不要なためコメントアウト
	if (0){
		printf("最大ターンを入力してください\n");
		scanf("%d",&MAX);
	}
	//この処理は開発時に不要なためコメントアウト

	//この処理は先手後手を決めるための処理
	
	//com_plays_first=first_turn_player_decision();
	com_plays_first=rand()%2;

	if(com_plays_first%2==0){
		printf("プレイヤーが先手です\n");

	}else{
		printf("コンピューターが先手です\n");


	}
	//この処理は先手後手を決めるための処理


	boardclear();
	tmp_boardclear();
	if(argc==2){
		if(!strcmp(argv[1],"new")){		//棋譜ファイルを新規作成
			time(&timer);
			t_st=localtime(&timer);
			sprintf(filename,"./kihu/kihu_%d_%d%d_%d.%d.%d.dat",t_st->tm_year+1900,t_st->tm_mon+1,t_st->tm_mday,t_st->tm_hour,t_st->tm_min,t_st->tm_sec);
			//sprintf(filename,"./kihu/kihu_a");
			if ((fp = fopen(filename, "w+")) == NULL) {		//ファイル作成
				printf("file open error!! A\n");
				fo=-1; 
			}else{
				printf("棋譜ファイル%sを作成しました\n",filename);
				fo=1;
			}
		}else{							//棋譜ファイルを読み込み
			if ((fp = fopen(argv[1], "r+")) == NULL) {		//ファイルオープン
				printf("file open error!! B\n");
				fo=-1; 
			}
			printf("棋譜ファイル%sをインポートしました\n",argv[1]);
			while(fscanf(fp, "%d%d",&turn,&X)!=EOF){
				pieceputtoboard(X, turn);
			}
		}
	}  else{ 							//棋譜ファイルを使わない

		printf("棋譜ファイルを使用しません\n");
	}
	srand((unsigned)time(NULL));


	printf("ゲーム開始処理おわり\n");	
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



			printf("computer B plays%d\n",X);




		} else{

/////////////////////////////////////////////////com思考処理終わり


/////////////////////////////////////////////////入力処理開始
			if(GAMEMODE==2){	
		X=iputprocess(X, input);

			}else{

			X=com_plays();



			printf("computer A plays%d\n",X);
}

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
	/////////////////////////////////////////////////ゲーム終了処理

	printboard();
	if(fo==1){
		fclose(fp);
	}
	/////////////////////////////////////////////////ゲーム終了処理おわり

}

