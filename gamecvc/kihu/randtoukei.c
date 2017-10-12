#include <stdio.h>
#include <stdlib.h>


int main(){
	FILE *fp; 
	int tx=0,y[12];
	char filename[128] = "debug.log";
	if ((fp = fopen(filename, "r+")) == NULL) {		//ファイル作成
		printf("file open error!! A\n");
	}
	for(int i=0;i<12;i++){
	y[i]=0;
	}

	while(fscanf(fp, "%d",&tx)!=EOF){
				y[tx]++;
			}

	for(int i=0;i<12;i++){
		printf("%d\n",y[i]);
	}



}