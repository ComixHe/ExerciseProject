#include <stdio.h>
void in(int *q);
void out(int *q);
void serch(int *q);
void show(int *q);
int head=0,tail=0;
int main(void)
{
    int q[10],n;
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {
        scanf("%d",&q[i]);
        tail++;
    }
    tail--;
    s:printf("1.in;2.out;3.serch;4.show;0:exit;\n");
    int p;
    scanf("%d",&p);
    switch (p)
    {
        case 1: in(q);break;
        case 2: out(q);break;
        case 3: serch(q);break;
        case 4: show(q);break;
        case 0: return 0;
    }
    goto s;
}

void in(int *q)
{
    int v;
    scanf("%d",&v);
    if((tail+1)%10==head)
        {
            fprintf(stderr,"full\n");
            return;
        }
    tail=(tail+1)%10;
    q[tail]=v;
    return;
}

void out(int *q)
{
    int v;
    scanf("%d",&v);
    if(head==tail)
    {
        fprintf(stderr,"empty\n");
        return;
    }
    printf("the value is %d\n",q[head]);
    head=(head+1)%10;
    return;
}

void serch(int *q)
{
    int v;
    scanf("%d",&v);
    if(head>tail)
    {
        for(int i=head;i<10;i++)
        {
            if(q[i]==v)
            {
                printf("exist\n");
                return;
            }
        }
        for(int i=0;i<=tail;i++)
        {
            if(q[i]==v)
            {
                printf("exist\n");
                return;
            }
        }
        printf("not exist\n");
        return;
    }
    else
    {
        for(int i=head;i<=tail;i++)
        {
            if(q[i]==v)
            {
                printf("exist\n");
                return;
            }
        }
        printf("not exist\n");
        return;
    }
}

void show(int *q)
{
    if(head<tail)
    {
        for(int i=head;i<=tail;i++)
            printf("%d ",q[i]);
        printf("\n");
    }
    else
    {
        for(int i=head;i<10;i++)
            printf("%d ",q[i]);
        for(int i=0;i<=tail;i++)
            printf("%d ",q[i]);
        printf("\n");
    }
    printf("head:%d tail:%d\n",head,tail);
    return;
}