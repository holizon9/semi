#include <stdio.h>
#include <stdlib.h>
#include<string.h>


int main(void)
{

	char *input;
	scanf("%s",input);

	if(!strcmp(input,"A")||!strcmp(input,"B")||(strcmp(input,"0")>0&&strcmp(input,"9")<0))
	{
		printf("match\n");
	}else{
		printf("invalid input\n");
	}

}