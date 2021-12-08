#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STACKLENGTH 10

typedef union value{
    int number;
    char operator;
}value;

typedef struct node{
    value values;
    struct node *leftnode;
    struct node *rightnode;
}node;

node *left,*right,*con,*temp,*root;
int csp=-1,isp=-1,nsp=-1;
char cstack[STACKLENGTH];
int istack[STACKLENGTH];
node *nstack[2];
void PrefixProcess();
//void InfixProcess();
//void SuffixProcess();
char *getexper();
void tree(int mode);
int ctoi(char **p);
int hasnext(char *p);
void destroy(node *p);
int main()
{
    int choice;
    printf("mode:\n1.Prefix\n2.Infix\n3.Suffix\n");
    scanf("%d",&choice);
    switch(choice)
    {
        case 1:PrefixProcess();break;
        //case 2:InfixProcess();break;
        //case 3:SuffixProcess();break;
    }
    root=nstack[0];
    destroy(root);
    return 0;
}

char *getexper()
{
    char exec,*string;
    unsigned int count=0;
    unsigned int space=20;
    string=malloc(sizeof(char)*space);
    if(!string)
    {
        printf("1.exper memory error!\n");
        return NULL;
    }
    getchar();
    while((exec=getchar())!='\n')
    {
        if(count==space)
        {
            space+=20;
            string=realloc(string,space);
            if(!string)
            {
                printf("2.exper memory error!\n");
                return NULL;
            }
        }
        string[count++]=exec;
    }
    string=realloc(string,space+2);
    if(!string)
    {
        printf("2.exper memory error!\n");
        return NULL;
    }
    string[count]='\0';
    return string;
}

void PrefixProcess()
{
    char *pres=NULL;
    printf("please input Prefix Expression:");
    if((pres=getexper())==NULL)
    {
        printf("memory error!\n");
        exit(EXIT_FAILURE);
    }
    char *p=pres;
    int outflag=0;
    while(*p!='\0')
    {
        if(csp>=10||isp>=10)
        {
            printf("stack overflow!\n");
            exit(EXIT_FAILURE);
        }
        while(*p==' ')
            p++;
        if(*p=='+'||*p=='-'||*p=='*'||*p=='/')
        {
            cstack[++csp]=*p;
            p++;
            outflag=0;
        }
        if(*p>='0'&&*p<='9')
        {
            istack[++isp]=ctoi(&p);
            outflag++;
            if(hasnext(p))
            {
                istack[++isp]=ctoi(&p);
                tree(1);
                outflag=0;
            }
            else if(nstack[0]!=NULL)
                tree(2);   
        }
    }
    if(csp>-1||isp>-1)
    {
        printf("expression error!\n");
        exit(EXIT_FAILURE);
    }
}

int ctoi(char **p)
{
    int temp=0;
    if(*p==NULL)
    {
        printf("null string");
        exit(EXIT_FAILURE);
    }
    while(**p==' ')
        (*p)++;
    while(**p!=' ')
    {
        if(**p=='\0')
            break;
        temp+=((*((*p)++))-48);
        temp*=10;
    }
    temp/=10;
    return temp;
}

void tree(int mode)
{
    if(mode==1)
    {
        if(isp < -1)
        {
            printf("stack underflow!");
            exit(EXIT_FAILURE);
        }
        left=malloc(sizeof(node));
        right=malloc(sizeof(node));
        con=malloc(sizeof(node));
        if(!left||!right||!con)
        {
            printf("mode 1 malloc error!\n");
            exit(EXIT_FAILURE);
        }
        left->values.number=istack[isp--];
        right->values.number=istack[isp--];
        left->leftnode=NULL;
        left->rightnode=NULL;
        right->rightnode=NULL;
        right->leftnode=NULL;
        con->values.operator=cstack[csp--];
        con->leftnode=left;
        con->rightnode=right;
        nstack[++nsp]=con;
        left=NULL;
        right=NULL;
        con==NULL;
        if(nsp==1)
        {
            temp=malloc(sizeof(node));
            temp->values.operator=cstack[csp--];
            temp->rightnode=nstack[nsp];
            nstack[nsp--]=NULL;
            temp->leftnode=nstack[nsp];
            nstack[nsp--]=NULL;
            nstack[++nsp]=temp;
            temp=NULL;
        } 
    }
    else if(mode==2)
    {
        con=malloc(sizeof(node));
        right=malloc(sizeof(node));
        if(!right||!con)
        {
            printf("mode 2 malloc error!\n");
            exit(EXIT_FAILURE);
        }
        right->values.number=istack[isp--];
        right->leftnode=NULL;
        right->rightnode=NULL;
        con->values.operator=cstack[csp--];
        con->leftnode=nstack[nsp];
        con->rightnode=right;
        nstack[nsp]=con;
        con=NULL;
        right==NULL;
    }
    else
    {
        printf("error mode!\n");
        exit(EXIT_FAILURE);
    }
}

int hasnext(char *p)
{
    while(*p==' ')
        p++;
    if(*p>='0'&&*p<='9')
        return 1;
    else
        return 0;
}

void destroy(node *p)
{
    
}