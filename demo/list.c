#include <stdio.h>
#include <stdlib.h>
#define LASIZE 10
#define LBSIZE 10
typedef struct sqlist{
    int lenth;
    int listsize;
    int *elem;
}sqlist;
int main()
{
    sqlist la,lb,lc;
    int i;
    la.elem=malloc(sizeof(int)*LASIZE);
    lb.elem=malloc(sizeof(int)*LBSIZE);
    lc.elem=malloc(sizeof(int)*(LASIZE+LBSIZE));
    if(la.elem==NULL||lb.elem==NULL||lc.elem==NULL)
    {
        printf("malloc error!");
        exit(EXIT_FAILURE);
    }
    la.lenth=0,lb.lenth=0,lc.lenth=0;
    la.listsize=LASIZE,lb.listsize=LBSIZE,lc.listsize=LASIZE+LBSIZE;
    printf("please input %d numbers for LA array:",LASIZE);
    for(i=0;i<LASIZE;i++)
    {
        scanf("%d",&la.elem[i]);
        la.lenth++;
    }
    printf("please input %d numbers for LB array:",LBSIZE);
    for(i=0;i<LBSIZE;i++)
    {
        scanf("%d",&lb.elem[i]);
        lb.lenth++;
    }
    int lap=0,lbp=0,lcp=0;
    while(lcp<LASIZE+LBSIZE&&lap<LASIZE&&lbp<LBSIZE)
    {
        lc.elem[lcp++]=la.elem[lap]>lb.elem[lbp]?lb.elem[lbp++]:la.elem[lap++];
        lc.lenth++;
    }
    while(lap<LASIZE)
    {
        lc.elem[lcp++]=la.elem[lap++];
        lc.lenth++;
    }
    while(lbp<LBSIZE)
    {
        lc.elem[lcp++]=lb.elem[lbp++];
        lc.lenth++;
    }
    for(i=0;i<LASIZE+LBSIZE;i++)
        printf("%d ",lc.elem[i]);
    return 0;
}