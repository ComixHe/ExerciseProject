#include <stdio.h>
#include <stdbool.h>
bool isValid(char *s);

int main(void)
{
    if(isValid("[])"))
        printf("true");
    else
        printf("false");
    return 0;
}

bool isValid(char *s)
{
    bool flag=false;
    if(*s=='\0')
        flag=true;
    char *p=s;
    int count;
    while(*p!='\0')
    {
        count++;
        p++;
    }
    char stack[count/2+1];
    p=s,count=-1;
    while(*p!='\0')
    {
        if(*p=='('||*p=='{'||*p=='[')
        {
            stack[++count]=*p;
            p++;
        }
        else
        {
            if(count==-1)
            {
                flag=false;
                goto end;
            }
            if((stack[count]=='('&&*p==')')||
                (stack[count]=='{'&&*p=='}')||
                (stack[count]=='['&&*p==']')
              )
            {
                count--;
                p++;
                flag=true;
            }
            else
                flag=false;
            if(!flag)
                goto end;
        }
    }
    if(count!=-1)
        flag=false;
    end:
    return flag;
}