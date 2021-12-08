#include <stdio.h>
#include <stdlib.h>

typedef struct tablenode{
    int distance;
    int number;
    struct tablenode *next;
}tablenode;

typedef struct searchtable{
    size_t known;
    int dis;
    int node;
}searchtable;


void createnode(tablenode *p,int nodenumber,int distance)
{
    while(p->next!=NULL)
        p=p->next;
    p->next=malloc(sizeof(tablenode));
    if(p->next==NULL)
    {
        printf("malloc error!");
        exit(EXIT_FAILURE);
    }
    p->next->distance=distance;
    p->next->number=nodenumber;
    p->next->next=NULL;
}

tablenode *initmap()
{
    tablenode *a=malloc(sizeof(tablenode)*7);
    if(a==NULL)
    {
        printf("malloc error!");
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<7;i++)
    {
        a[i].distance=0;
        a[i].next=NULL;
        a[i].number=i+1;
    }
    createnode(&a[0],2,2);
    createnode(&a[0],4,1);
    createnode(&a[1],4,3);
    createnode(&a[1],5,10);
    createnode(&a[2],1,4);
    createnode(&a[2],6,5);
    createnode(&a[3],3,2);
    createnode(&a[3],5,2);
    createnode(&a[3],6,8);
    createnode(&a[3],7,4);
    createnode(&a[4],7,6);
    createnode(&a[6],6,1);
    return a;
}

searchtable *initsearch()
{
    searchtable *a=malloc(sizeof(searchtable)*7);
    if(a==NULL)
    {
        printf("malloc error!");
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<7;i++)
    {
        a[i].known=0;
        a[i].dis=0;
        a[i].node=0;
    }
    return a;
}

int check(searchtable *t)
{
    for(int i=0;i<7;i++)
        if(t[i].known==0)
            return 0;
    return 1;
}

int getmin(searchtable *t)
{
    int min=1;
    for(int i=0;i<7;i++)
        if(t[i].known==0)
        {
            min=i+1;
            break;
        }
    for(int i=min-1;i<7;i++)
        if(t[i].dis<t[min-1].dis&&t[i].known==0&&t[i].dis!=0)
            min=i+1;
    return min;
}

int *dijkstra(tablenode *map,int from,int to)
{
    int *path=malloc(sizeof(int)*7);
    int entrance=from;
    searchtable *search=initsearch();
    for(int i=0;i<7;i++)
        path[i]=-1;
    search[from-1].known=1;
    search[from-1].dis=0;
    search[from-1].node=0;
    while(!check(search))
    {
        tablenode *temp=map[from-1].next;
        while(temp!=NULL)
        {
            if(search[temp->number-1].known==0)
            {
                if(search[temp->number-1].dis==0)
                {
                    search[temp->number-1].node=from;
                    search[temp->number-1].dis=temp->distance+search[from-1].dis;
                }
                else
                {
                    if(temp->distance+search[from-1].dis<search[temp->number-1].dis)
                    {
                        search[temp->number-1].node=from;
                        search[temp->number-1].dis=temp->distance+search[from-1].dis;
                    }
                }
            }
            temp=temp->next;
        }
        int min=getmin(search);
        search[min-1].known=1;
        from=min;
    }
    int j;
    for(j=0;to!=entrance;j++)
    {
        path[j]=to;
        to=search[to-1].node;
    }
    path[j]=entrance;
    free(search);
    return path;
}

int allvisit(int *a)
{
    for(int i=0;i<7;i++)
        if(a[i]==0)
            return 1;
    return 0;
}

void dfs(tablenode *table)
{
    int stack[7],visit[7],node=1,sp=-1;
    for(int i=0;i<7;i++)
    {
        stack[i]=0;
        visit[i]=0;
    }
    tablenode *temp=&table[0];
    printf("dfs: ");
    while(allvisit(visit))
    {
        stack[++sp]=temp->number;
        visit[temp->number-1]=1;
        printf("%d ",temp->number);
        if(temp->next!=NULL)
        {
            temp=temp->next;
            while(temp!=NULL)
            {
                if(visit[temp->number-1]!=1)
                {
                    temp=&table[temp->number-1];
                    break;
                }
                else
                    temp=temp->next;
            }
            if(temp==NULL)
                goto lable3;
        }
        else
        {
            lable3:
            if(sp==-1)
            {
                lable2:
                for(int i=0;i<7;i++)
                    if(visit[i]==0)
                    {
                        temp=&table[i];
                        break;
                    }
            }
            else
            {
                int now;
                stack[sp--]=0;
                lable1:
                now=stack[sp];
                stack[sp--]=0;
                temp=&table[now-1];
                while(temp!=NULL)
                {
                    if(visit[temp->number-1]==0)
                        break;
                    temp=temp->next;
                }
                if(temp==NULL)
                {
                    if(sp!=-1)
                        goto lable1;
                    else
                        goto lable2;
                }
            }
        }
    }
    printf("\n");
}

void infosearch(tablenode *p,int number)
{
    int flag=0;
    printf("this is node %d\n",number);
    printf("linknode: ");
    tablenode *temp=p[number-1].next;
    while(temp!=NULL)
    {
        flag=1;
        printf("%d ",temp->number);
        temp=temp->next;
    }
    if(flag==0)
        printf("NULL\n");
    else
        printf("\n");
}

int main()
{
    char s='a';
    tablenode *map=initmap();
    while(1)
    {
        printf("please choose one fuction:\n\
            a:get the shortest path bettwen two position.\n\
            b:depth first traversal.\n\
            c:get some infomation about one position.\n\
            d:quit the program.\n");
        s=getchar();
        getchar();
        switch(s)
        {
            case 'a':{
                int from,to;
                printf("input departure(1-7):");
                scanf("%d",&from);
                printf("input destination(1-7):");
                scanf("%d",&to);
                getchar();
                int *path=dijkstra(map,from,to);
                printf("the shortest path:");
                for(int i=6;i>=0;i--)
                    if(path[i]!=-1)
                        printf("%d ",path[i]);
                printf("\n");
                free(path);
            }break;
            case 'b':{
                dfs(map);
            }break;
            case 'c':{
                int p;
                printf("which position's infomation you want to know?(1-7):");
                scanf("%d",&p);
                getchar();
                infosearch(map,p);
            }break;
            case 'd':{
                goto end;
            }
        }
    }
    end:
    free(map);
    return 0;
}