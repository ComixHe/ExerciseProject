#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    int t=0;
    char *a="9876543210987654321";
    char *b="1122334455667788";
    int p=strlen(a),q=strlen(b);
    char temp[p+q+1],res[p+q+1];
    memset(res,'0',p+q+1);
    memset(temp,'0',p+q+1);
    res[p+q+1]='\0';
    temp[p+q+1]='\0';
    for(int i=q-1;i>=0;i--)
    {
        int gw=0,jw=0;
        char *last=temp+p+q-t;
        for(int j=p-1;j>=0;j--)
        {
            int sig=(int)(*(a+j)-'0')*(int)(*(b+i)-'0')+jw;
            if(sig>=10)
            {
                jw=sig/10;
                gw=sig%10;
            }
            else
            {
                jw=0;
                gw=sig;
            }
            *last=(char)(gw+48);
            last--;
        }
        *last=(char)(jw+48);
        t++;
        char *d=res+p+q-1;
        char *f=temp+p+q-1;
        gw=0,jw=0;
        for(int j=p+q;j>=0;j--)
        {
            int sig=(int)(temp[j]-'0')+(int)(res[j]-'0')+jw;
            if(sig>=10)
            {
                jw=sig/10;
                gw=sig%10;
            }
            else
            {
                jw=0;
                gw=sig;
            }
            res[j]=(char)(gw+48);
        }
        memset(temp,'0',p+q+1);
    }
    char *z=res;
    while(*z!='\0'&&*z=='0')
        z++;
    printf("%s\n",z);
}