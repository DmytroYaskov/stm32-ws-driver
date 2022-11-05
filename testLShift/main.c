#include "stdio.h"

#define     LSHIFT(v,n) (((unsigned int)(v) << n))

int main(){
    printf("%d\n",LSHIFT(0,2));
    return 0;
}