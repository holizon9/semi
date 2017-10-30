//#define GAMEMODE 1  //pvp
//#define GAMEMODE 2 //pvc
#define GAMEMODE 3 //cvc

#include <stdlib.h>
#include<stdio.h>
#include <string.h>
#include <time.h>
#include "game_function.h"

int board[12][12];  //ボード生成
int tmpboard[12][12]; //関数内で使用する一時的なボードの砂場
int turn=1;   	//ターンカウンタ　　奇数で先手番　偶数で後手番
int MAX=25;			//最大手数
int X,Y;			//ボード上の座標を示す
int fo=0;


int main(int argc,char *argv[]){
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FILE *fp; 
	char input[128];
	char filename[128];
	time_t timer;
	struct tm *t_st;
	int playfirst;
	int seed;
	seed = (unsigned)time(NULL);
	seed = 1509335351;
	srand(seed);

	/*ゲーム開始処理*/{
	//この処理は開発時に不要なためコメントアウト
	if (0){
		printf("最大ターンを入力してください\n");
		scanf("%d",&MAX);
	}
	//この処理は開発時に不要なためコメントアウト



	boardclear();
	tmp_boardclear();
	if(argc==2){
		if(!strcmp(argv[1],"new")){		//棋譜ファイルを新規作成
			time(&timer);
			t_st=localtime(&timer);
			sprintf(filename,"./kihu/kihu_%d年%d月%d日%d：%d：%d.dat",t_st->tm_year+1900,t_st->tm_mon+1,t_st->tm_mday,t_st->tm_hour,t_st->tm_min,t_st->tm_sec);
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
			turn++;
		}
	}  else{ 							//棋譜ファイルを使わない

		printf("棋譜ファイルを使用しません\n");
	}
	//srand((unsigned)time(NULL));
	printboard();

	//この処理は先手後手を決めるための処理
	if(GAMEMODE==2){
		playfirst=first_turn_player_decision();

	}

	//この処理は先手後手を決めるための処理


	printf("ゲーム開始処理おわり\n");	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  基本的にいじるひつようなし
}	/*ゲーム開始処理おわり*/

	while(turn<MAX+1){  //ゲーム
		X=turnstart(turn);
		if(turn%2==1){
			printf("X? ");
		}else{
			printf("O? ");	

		}

/////////////////////////////////////////////////ゲームモードに対応した入力処理


		switch(GAMEMODE){
			case 1:
			X=iputprocess(X, input);
			break;
			case 2:
			if((playfirst+turn)%2==1){
				X=iputprocess(X, input);
			}else{
				X=com_plays();
				printf("%d",X);
				if(X<0||11<X){
					printf("com retires\n");
					break;
				}
			}
			break;
			case 3:
			X=com_plays();
			if(X<0||11<X){
				printf("com retires\n");
				break;
			}
				printf("%d",X);
			break;

			default:
			X=iputprocess(X, input);

		}



/////////////////////////////////////////////////入力処理おわり

		pieceputtoboard(X,turn);
		if(fo==1){
			fprintf(fp, "%d  %d\n",turn,X);		//ターン中の棋譜入力

		}
		printboard_color(X);
		if(game_end_message(turn)==1){
			break; 
		}


		turn++;

	}
	/////////////////////////////////////////////////ゲーム終了処理

	end_printboard();		///end_printboardをつくる
	game_end_message(turn);
	printf("seed is %d\n",seed);
	if(fo==1){
		fclose(fp);
	}
	/////////////////////////////////////////////////ゲーム終了処理おわり

}

