#include <stdio.h>
#include <stdlib.h>

typedef struct tree{
        struct tree *leftsnode;
        char data;
        struct tree *rightsnode;
    }tree;

tree *create(tree *node);
void preorder(tree *node);
void midorder(tree *node);
void posorder(tree *node);
tree *pre;

int main(void)
{
    tree *root;
    root=create(root);
    if(root!=NULL)
        printf("successfully create tree\n");
    preorder(root);
    putchar('\n');
    midorder(root);
    putchar('\n');
    posorder(root);
    return 0;
}

tree *create(tree *node)
{
    char ch;
    scanf("%c",&ch);
    if(ch=='#')
        node=NULL;
    else
    {
        node=malloc(sizeof(tree));
        if(node==NULL)
        {
            printf("can't create node!");
            exit(EXIT_FAILURE);
        }
        node->data=ch;
        node->leftsnode=create(node->leftsnode);
        node->rightsnode=create(node->rightsnode);
    }
    return node;
}

void preorder(tree *node)
{
    if(node==NULL)
        return;
    else
    {
        printf("%c ",node->data);
        preorder(node->leftsnode);
        preorder(node->rightsnode);
    }
}

void midorder(tree *node)
{
    if(node==NULL)
        return;
    else
    {
        midorder(node->leftsnode);
        printf("%c ",node->data);
        midorder(node->rightsnode);
    }
}

void posorder(tree *node)
{
    if(node==NULL)
        return;
    else
    {
        posorder(node->leftsnode);
        posorder(node->rightsnode);
        printf("%c ",node->data);
    }
}