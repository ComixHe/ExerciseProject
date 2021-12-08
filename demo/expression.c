#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 20

typedef struct ele{
    char op;
    int va;
    int point;
}ele;

int istack[SIZE];
char cstack[SIZE];
char buffer[100];
int isp=-1,csp=-1;
void pushi(int a);
void pushc(char c); 
int popi();
char popc();
int cal(int a,int b,char c);
int judge(char c);
ele read(char *buf,int p);

int main()
{
    char temp;
    printf("please input an expression:");
    scanf("%s",buffer);
    int p=0,size=strlen(buffer);
    ele a;
    pushc('#');
    p++;
    while(p<size)
    {
        a=read(buffer,p);
        p=a.point;
        if(a.op=='#')
            break;
        else if(a.op!='\0')
        {
            if(judge(a.op)>judge(cstack[csp]))
            {
                if(a.op==')')
                {
                    while((temp=popc())!='(')
                        pushi(cal(popi(),popi(),temp));
                }
                else
                    pushc(a.op);
            }
            else 
            {
                if(a.op=='(')
                    pushc(a.op);
                else
                {
                    pushi(cal(popi(),popi(),popc()));
                    pushc(a.op);
                }
            }
        }
        else 
            pushi(a.va);
    }
    while((temp=popc())!='#')
        pushi(cal(popi(),popi(),temp));
    printf("%d",popi());
    return 0;
}

int cal(int a,int b,char c)
{
    int result=0;
    switch(c)
    {
        case '+':result=a+b; break;
        case '-':result=a-b; break;
        case '*':result=a*b; break;
        case '/':result=a/b; break;
    }
    return result;
}

int judge(char c)
{
    switch (c)
    {
        case '#':return 0;
        case '+':return 1;
        case '-':return 1;
        case '*':return 2;
        case '/':return 2;
        case '(':return 0;
        case ')':return 3;
    }
    return -1;
}

void pushi(int a)
{
    istack[++isp]=a;
}

void pushc(char c)
{
    cstack[++csp]=c;
}

int popi()
{
    int a=istack[isp];
    istack[isp--]=0;
    return a;
}

char popc()
{
    char c=cstack[csp];
    cstack[csp--]='\0';
    return c;
}

ele read(char *buf,int p)
{
    int value=0,sig=0;
    ele temp;
    if(buf[p]>='0'&&buf[p]<='9')
    {
        while(buf[p]>='0'&&buf[p]<='9')
        {
            sig=buf[p]-'0';
            value+=sig*10;
            p++;
        }
        if(sig!=0)
            value/=10;
        temp.va=value;
        temp.point=p;
        temp.op='\0';
    }
    else
    {
        temp.op=buf[p];
        p++;
        temp.point=p;
    }
    return temp;
}