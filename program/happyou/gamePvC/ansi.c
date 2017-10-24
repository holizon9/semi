#include<stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
int bin_to_dec(char *bits);


int main(int argc,char *argv[]){
	char *ptr;
	ptr= &*argv[1];
	if(argc!=2||strlen(argv[1])!=4){
		printf("input error\n");
		return -1;
	}

	for(int i=0;i<strlen(argv[1]);i++){
		if(ptr[i]!='0'&&ptr[i]!='1'){
		printf("input error\n");
		return -1;
		}

	}
	

	int dec =0;
	dec =bin_to_dec(argv[1]);


	printf("%s to %d\n",argv[1],dec);


}
int bin_to_dec(char *bits){
	int tmp,sum=0;
	tmp=atoi(bits);
	for(int i=0;i<4;i++){
		sum+=(tmp%10)*pow(2,i);
		tmp=tmp/10;
	}

return sum;

}
