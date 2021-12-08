#include<stdio.h>
int main()
{
    char s[105];
    while(gets(s)!=NULL)
    {
        char *p=s;
        while(*p!='\0')
        {
            while(*p==' ')
                p++;
            if(*p>='a'&&*p<='z')
                *p-=32;
            while(*p!=' '&&*p!='\0')
                p++;
        }
        printf("%s\n",s);
    }
    return 0;
}