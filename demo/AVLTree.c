#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int value;
    int height;
    int count;
    struct node *left;
    struct node *right;
}node;

node *avlinsert(node *p,int v);
node *singlerotatewithleft(node *p);
node *doublerotatewithleft(node *p);
node *singlerotatewithright(node *p);
node *doublerotatewithright(node *p);
node *avlsearch(node *p,int v);
void avldelete(node *p,int v);
int height(node *p);
int max(int a,int b);

int main()
{
    node *root=NULL;
    int temp;
    while(scanf("%d",&temp)!=EOF)
        root=avlinsert(root,temp);
    return 0;
}

int height(node *p)
{
    if(p==NULL)
        return -1;
    else
        return p->height;
}

int max(int a,int b)
{
    return a>b?a:b;
}

node *avlinsert(node *p,int v)
{
    if(p==NULL)
    {
        p=malloc(sizeof(node));
        if(p==NULL)
        {
            printf("malloc error!");
            exit(EXIT_FAILURE);
        }
        else
        {
            p->left=NULL;
            p->right=NULL;
            p->value=v;
            p->height=0;
            p->count=1;
        }
    }
    else if(v<p->value)
    {
        p->left=avlinsert(p->left,v);
        if(height(p->left)-height(p->right)==2)
            if(v<p->left->value)
                p=singlerotatewithleft(p);
            else
                p=doublerotatewithleft(p);
    }
    else if(v>p->value)
    {
        p->right=avlinsert(p->right,v);
        if(height(p->right)-height(p->left)==2)
            if(v>p->right->value)
                p=singlerotatewithright(p);
            else
                p=doublerotatewithright(p);
    }
    else if(v==p->value)
        (p->count)++;
    p->height=max(height(p->left),height(p->right))+1;
    return p;
}

node *singlerotatewithleft(node *p)
{
    node *q;
    q=p->left;
    p->left=q->right;
    q->right=p;
    p->height=max(height(p->left),height(p->right))+1;
    q->height=max(height(q->left),p->height)+1;
    return q;
}

node *singlerotatewithright(node *p)
{
    node *q;
    q=p->right;
    p->right=q->left;
    q->left=p;
    p->height=max(height(p->left),height(p->right))+1;
    q->height=max(height(q->right),p->height)+1;
    return q;
}
node *doublerotatewithleft(node *p)
{
    p->left=singlerotatewithright(p->left);
    return singlerotatewithleft(p);
}

node *doublerotatewithright(node *p)
{
    p->right=singlerotatewithleft(p);
    return singlerotatewithright(p);
}

node *avlsearch(node *p,int v)
{
    if(p==NULL)
        return NULL;
    if(v==p->value)
        return p;
    else if(v<p->value)
        return avlsearch(p->left,v);
    else if(v>p->value)
        return avlsearch(p->right,v);
}

void avldelete(node *p,int v)
{
    node *q=avlinsert(p,v);
    if(q==NULL||q->count==0)
    {
        printf("there is no %d",v);
        exit(EXIT_FAILURE);
    }
    else
        (q->count)--;
}