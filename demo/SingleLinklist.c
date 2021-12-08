#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    int i,m;
    struct node {
        int n;
        struct node *next;
    };
    struct node *head;
    struct node *body;
    struct node *bef;
    struct node *s;
    printf("please input a link node number which you want to create:");
    scanf("%d",&i);
    m=i;
    while(i--)
    {
        body=malloc(sizeof(struct node));
        if(body==NULL)
        {
            printf("no space");
            return 0;
        }
        printf("please input a number:");
        scanf("%d",&body->n);
        if(i==0)
            body->next=NULL;
        if(i==m-1)
        {
            head=body;
            bef=body;
            body=NULL;
            continue;
        }
        bef->next=body;
        bef=body;
        body=NULL; 
    }
    s=head;
    for(int a=0;a<m;s=s->next,a++)
        printf("%d ",s->n);
    return 0;
}