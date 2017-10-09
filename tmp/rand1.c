#include <stdio.h> 
#include <stdlib.h> /* for srand48(), drand48() */
#include <math.h>   /* for sqrt(), log() */

double nrand();
int x;

int main(){
         int i;
         int seed = 123457; /* use any number as you like */
         int x;
         
         srand48( seed );
         
         for (i=0; i<10; i++){
         	while(x<0||x>11){
                x = (int)round(nrand()*3+6.0);
         	}
            
                printf("%d\n",x);
         }
}

/* 
  nrand() :  a random sample from standard normal distribution  
  一様分布 drand48() を使い標準正規分布に従うデータを出力する関数 
 */
double nrand()
{
        static int sw=0;
        static double r1,r2,s;

        if (sw==0){
                sw=1;
                do {
                        r1=2.0*drand48()-1.0;
                        r2=2.0*drand48()-1.0;
                        s=r1*r1+r2*r2;
                } while (s>1.0 || s==0.0);
                        s=sqrt(-2.0*log(s)/s);
                        return(r1*s);
        }
        else {
                sw=0;
                return(r2*s);
        }
}