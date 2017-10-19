#include <stdio.h>
#include <math.h>
#include "random.h"
 
int main(void){
    int i;
    for(i=0;i<10000;i++){  printf("%lf\n",rand_normal(0.0,1.0));   }
}