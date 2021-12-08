#include <stdio.h>
#include <stdlib.h>
typedef struct node{
    short ltag,rtag;
    char data;
    struct node *lchild,*rchild;
}node;

void inthreading(node *tree);
void createbitree(node *Node);
int inorder(node *tree);
node *pre;

int main(void)
{
    node *tree;
    createbitree(tree);
    printf("successfully create tree!\n");
    inthreading(tree);
    printf("successfully inorder threading\n");
    inorder(tree);
    printf("successfully display this tree\n");
    return 0;
}

void createbitree(node *tree)
{
    char ch;
    scanf("%c",&ch);
    if(ch=='#')
        tree=NULL;
    else
    {
        tree=malloc(sizeof(node));
        if(tree==NULL)
        {
            printf("can't create tree's node");
            exit(EXIT_FAILURE);
        }
        tree->data=ch;
        createbitree(tree->lchild);
        createbitree(tree->rchild);
    }
    
}

void inthreading(node *Node)
{
    if(Node!=NULL)
    {
        inthreading(Node->lchild);
        if(Node->lchild!=NULL)
        {
            Node->ltag=1;
            Node->lchild=pre;
        }
        if(pre->rchild!=NULL)
        {
            pre->rtag=1;
            pre->rchild=Node;
        }
        pre=Node;
        inthreading(Node->rchild);
    }
}

int inorder(node *tree)
{
    node *temp;
    temp=tree->lchild;
    while(temp!=tree)
    {
        while(temp->ltag==0)
            temp=temp->lchild;
        printf("%c",temp->data);
        while(temp->rtag)
        {
            temp=temp->rchild;
            printf("%c",temp->data);
        }
        temp=temp->rchild;
    }
    return 1;
}