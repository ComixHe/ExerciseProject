#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int value;
    struct node *left;
    struct node *right;
    struct node *parent;
}node;
node *create(int v,node *parent)
{
    node *temp=malloc(sizeof(node));
    if(temp==NULL)
    {
        printf("malloc error!");
        return NULL;
    }
    temp->left=NULL;
    temp->parent=parent;
    temp->right=NULL;
    temp->value=v;
    return temp;
}

node *leftrotate(node *p)
{
    if(p==NULL)
        return NULL;
    node *parent=p->parent;
    node *gparent=parent->parent;
    parent->right=p->left;
    if(p->left)
        p->left->parent=parent;
    p->left=parent;
    parent->parent=p;
    p->parent=gparent;
    if(gparent)
    {
        if(gparent->left==parent)
            gparent->left=p;
        else
            gparent->right=p;
    }
    return p;
}

node *rightrotate(node *p)
{
    if(p==NULL)
        return NULL;
    node *parent=p->parent;
    node *gparent=parent->parent;
    parent->left=p->right;
    if(p->right)
        p->right->parent=parent;
    p->right=parent;
    parent->parent=p;
    p->parent=gparent;
    if(gparent)
    {
        if(gparent->left==parent)
            gparent->left=p;
        else
            gparent->right=p;
    }
    return p;
}

node *doubleleftrotate(node *p)
{
    p=leftrotate(p->parent);
    p=leftrotate(p->right);
    return p;
}

node *doublerightrotate(node *p)
{
    p=rightrotate(p->parent);
    p=rightrotate(p->left);
    return p;
}

node *leftrigthrotate(node *p)
{
    p=leftrotate(p);
    p=rightrotate(p);
    return p;
}

node *rightleftrotate(node *p)
{
    p=rightrotate(p);
    p=leftrotate(p);
    return p;
}

node *up(node *p)
{
    int i,j;
    node *parent,*gparent;
    parent=p->parent;
    gparent=parent->parent;
    if(gparent==NULL)
        return p;
    i=gparent->left==parent?-1:1;
    j=parent->left==p?-1:1;
    if(i==-1&&j==-1)
        p=doublerightrotate(p);
    else if(i==-1&&j==1)
        p=leftrigthrotate(p);
    else if(i==1&&j==-1)
        p=rightleftrotate(p);
    else
        p=doubleleftrotate(p);
    return p;
}

node *splay(node *p,node *root)
{
    while(root->left!=p&&root->right!=p&&p->parent!=NULL)
        p=up(p);
    if(root->left==p)
        p=rightrotate(p);
    else if(root->right==p)
        p=leftrotate(p);
    return p;
}

node *searchv(node *p,int v)
{
    if(p==NULL)
        return NULL;
    if(p->value>v)
        p=searchv(p->left,v);
    else if(p->value<v)
        p=searchv(p->right,v);
    else if(p->value==v)
        printf("%d exist.\n",v);
    return p;
}

node *search(node *root,int v)
{
    node *temp=searchv(root,v);
    if(temp!=NULL&&temp!=root)
    {
        temp=splay(temp,root);
        return temp;
    }
    return NULL;
}

node *insert(node *root,node *parent,int v)
{
    if(root==NULL)
        return create(v,parent);
    else if(root->value>v)
        root->left=insert(root->left,root,v);
    else if(root->value<v)
        root->right=insert(root->right,root,v);
    return root;
}

node *findmin(node *p)
{
    if(p->left)
        p=findmin(p->left);
    return p;
}

node *delete(node *root,int v)
{
    node *temp;
    node *replace_1,*replace_2;
    temp=searchv(root,v);
    if(temp!=NULL)
    {
        if(temp!=root)
            root=splay(temp,root);
        if(root->right)
        {
            replace_1=findmin(root->right);
            if(replace_1==root->right)
                replace_1=NULL;
            if(replace_1!=NULL)
            {
                root->value=replace_1->value;
                if(replace_1->right==NULL)
                    free(replace_1);
                else
                {
                    replace_2=replace_1->right;
                    replace_2->parent=replace_1->parent;
                    replace_2->parent->left=replace_2;
                    free(replace_1);
                }
            }
            else
            {
                replace_2=root;
                root->right->left=root->left;
                root->left->parent=root->right;
                root=root->right;
                free(replace_2);
            }
        }
        else
        {
            replace_2=root;
            root=root->left;
            free(replace_2);
        }
    }
    else
        printf("%d is not exists.\n",v);
    root->parent=NULL;
    return root;
}


int main()
{
    node *root=NULL,*parent=NULL;
    root=insert(root,parent,11);
    root=insert(root,parent,7);
    root=insert(root,parent,18);
    root=insert(root,parent,3);
    root=insert(root,parent,9);
    root=insert(root,parent,16);
    root=insert(root,parent,26);
    root=insert(root,parent,11);
    root=insert(root,parent,14);
    root=insert(root,parent,15);
    printf("insert successfully\n");
    root=search(root,14);
    root=search(root,9);
    root=delete(root,16);
    return 0;
}